#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uart.h"

typedef struct putstr_data {
  char* buf;
  int len;
  bool called;
  struct putstr_data* next;
} putstr_data_t;

#define MAX_UARTS 2

struct putstr_queue {
  putstr_data_t *putstr_head;
  putstr_data_t *putstr_tail;
};

volatile struct putstr_queue putstr_queues[MAX_UARTS];

static void uart_putstr_cb(int _x __attribute__ ((unused)),
                           int uart_num,
                           int _z __attribute__ ((unused)),
                           void* ud __attribute__ ((unused))) {

  putstr_data_t* data = putstr_queues[uart_num].putstr_head;
  data->called = true;
  putstr_queues[uart_num].putstr_head = data->next;

  if (putstr_queues[uart_num].putstr_head == NULL) {
    putstr_queues[uart_num].putstr_tail = NULL;
  } else {
    int ret;
    ret = uart_putnstr_async(uart_num, putstr_queues[uart_num].putstr_head->buf,
                             putstr_queues[uart_num].putstr_head->len, uart_putstr_cb, NULL);
    if (ret < 0) {
      // XXX There's no path to report errors currently, so just drop it
      uart_putstr_cb(0, uart_num, 0, NULL);
    }
  }
}

int uart_putnstr(uint8_t uart_num, const char *str, size_t len) {
  int ret = TOCK_SUCCESS;

  putstr_data_t* data = (putstr_data_t*)malloc(sizeof(putstr_data_t));
  if (data == NULL) return TOCK_ENOMEM;

  data->len    = len;
  data->called = false;
  data->buf    = (char*)malloc(len * sizeof(char));
  if (data->buf == NULL) {
    ret = TOCK_ENOMEM;
    goto putnstr_fail_buf_alloc;
  }
  strncpy(data->buf, str, len);
  data->next = NULL;

  if (putstr_queues[uart_num].putstr_tail == NULL) {
    // Invariant, if tail is NULL, head is also NULL
    ret = uart_putnstr_async(uart_num, data->buf, data->len, uart_putstr_cb, NULL);
    if (ret < 0) goto putnstr_fail_async;
    putstr_queues[uart_num].putstr_head = data;
    putstr_queues[uart_num].putstr_tail = data;
  } else {
    putstr_queues[uart_num].putstr_tail->next = data;
    putstr_queues[uart_num].putstr_tail       = data;
  }

  yield_for(&data->called);

putnstr_fail_async:
  free(data->buf);
putnstr_fail_buf_alloc:
  free(data);

  return ret;
}

int uart_putnstr_async(uint8_t uart_num, const char *str, size_t len, subscribe_cb cb, void* userdata) {
  int ret;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
  // Currently, allow gives RW access, but we should have a richer set of
  // options, such as kernel RO, which would be let us preserve type semantics
  // all the way down
  void* buf = (void*) str;
#pragma GCC diagnostic pop

  ret = allow(DRIVER_NUM_CONSOLE, 1 | (uart_num << 16), buf, len);
  if (ret < 0) return ret;

  ret = subscribe(DRIVER_NUM_CONSOLE, 1 | (uart_num << 16), cb, userdata);
  if (ret < 0) return ret;

  ret = command(DRIVER_NUM_CONSOLE, 1 | (uart_num << 16), len, 0);
  return ret;
}

int uart_getnstr_async(uint8_t uart_num, char *str, size_t len, subscribe_cb cb, void* userdata) {
  int ret;

  ret = allow(DRIVER_NUM_CONSOLE, 2 | (uart_num << 16), str, len);
  if (ret < 0) return ret;

  ret = subscribe(DRIVER_NUM_CONSOLE, 2 | (uart_num << 16), cb, userdata);
  if (ret < 0) return ret;

  ret = command(DRIVER_NUM_CONSOLE, 2 | (uart_num << 16), len, 0);
  return ret;
}

typedef struct getnstr_data {
  bool called;
  int result;
} getnstr_data_t;

struct getnstr_data getnstr_datas[MAX_UARTS] = {{ true, 0 }, { true, 0 }};

static void getnstr_cb(int result,
                       int _x __attribute__ ((unused)),
                       int uart_num,
                       void* ud __attribute__ ((unused))) {
  getnstr_datas[uart_num].result = result;
  getnstr_datas[uart_num].called = true;
}

int uart_getnstr(uint8_t uart_num, char *str, size_t len) {
  int ret;
  if (!getnstr_datas[uart_num].called) {
    // A call is already in progress
    return TOCK_EALREADY;
  }
  getnstr_datas[uart_num].called = false;

  ret = uart_getnstr_async(uart_num, str, len, getnstr_cb, NULL);
  if (ret < 0) return ret;
  yield_for(&getnstr_datas[uart_num].called);
  return getnstr_datas[uart_num].result;
}

int uart_getch(uint8_t uart_num) {
  int r;
  char buf[1];

  r = uart_getnstr(uart_num, buf, 1);

  return (r == TOCK_SUCCESS) ? buf[0] : TOCK_FAIL;
}

int uart_getnstr_abort(uint8_t uart_num) {
  return command(DRIVER_NUM_CONSOLE | (uart_num >> 16), 3, 0, 0);
}
