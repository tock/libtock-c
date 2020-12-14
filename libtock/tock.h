#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (subscribe_cb)(int, int, int, void*);

typedef enum {
  TOCK_SYSCALL_FAILURE             =   0,
  TOCK_SYSCALL_FAILURE_U32         =   1,
  TOCK_SYSCALL_FAILURE_U32_U32     =   2,
  TOCK_SYSCALL_FAILURE_U64         =   3,
  TOCK_SYSCALL_SUCCESS             = 128,
  TOCK_SYSCALL_SUCCESS_U32         = 129,
  TOCK_SYSCALL_SUCCESS_U32_U32     = 130,
  TOCK_SYSCALL_SUCCESS_U64         = 131,
  TOCK_SYSCALL_SUCCESS_U32_U32_U32 = 132,
  TOCK_SYSCALL_SUCCESS_U64_U32     = 133
} syscall_rtype_t;

typedef enum {
  TOCK_ERROR_FAIL        = 0,
  TOCK_ERROR_BUSY        = 1,
  TOCK_ERROR_ALREADY     = 2,
  TOCK_ERROR_OFF         = 3,
  TOCK_ERROR_RESERVE     = 4,
  TOCK_ERROR_INVAL       = 5,
  TOCK_ERROR_SIZE        = 6,
  TOCK_ERROR_CANCEL      = 7,
  TOCK_ERROR_NOMEM       = 8,
  TOCK_ERROR_NOSUPPORT   = 9,
  TOCK_ERROR_NODEVICE    = 10,
  TOCK_ERROR_UNINSTALLED = 11,
  TOCK_ERROR_NOACK       = 12
} tock_error_t;

typedef struct {
  syscall_rtype_t type;
  uint32_t data[3];
} syscall_return_t;

typedef struct {
  bool success;
  subscribe_cb* callback;
  void* userdata;
  tock_error_t error;
} subscribe_return_t;

typedef struct {
  bool success;
  void* ptr;
  size_t size;
  tock_error_t error;
} allow_rw_return_t;

typedef struct {
  bool success;
  const void* ptr;
  size_t size;
  tock_error_t error;
} allow_ro_return_t;


    
int tock_enqueue(subscribe_cb cb, int arg0, int arg1, int arg2, void* ud);

void yield(void);
void yield_for(bool*);

__attribute__ ((warn_unused_result))
int command(uint32_t driver, uint32_t command, int data, int arg2);

__attribute__ ((warn_unused_result))
syscall_return_t command2(uint32_t driver, uint32_t command, int data, int arg2);
__attribute__ ((warn_unused_result))
int subscribe(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata);

__attribute__ ((warn_unused_result))
subscribe_return_t subscribe2(uint32_t driver, uint32_t subscribe,
			      subscribe_cb cb, void* userdata);

__attribute__ ((warn_unused_result))
int allow(uint32_t driver, uint32_t allow, void* ptr, size_t size);

__attribute__ ((warn_unused_result))
allow_rw_return_t allow_readwrite(uint32_t driver, uint32_t allow, void* ptr, size_t size);

__attribute__ ((warn_unused_result))
allow_ro_return_t allow_readonly(uint32_t driver, uint32_t allow, const void* ptr, size_t size);

// op_type can be:
// 0: brk, arg1 is pointer to new memory break
// 1: sbrk, arg1 is increment to increase/decrease memory break
void* memop(uint32_t op_type, int arg1);

// Wrappers around memop to support app introspection
void* tock_app_memory_begins_at(void);
void* tock_app_memory_ends_at(void);
void* tock_app_flash_begins_at(void);
void* tock_app_flash_ends_at(void);
void* tock_app_grant_begins_at(void);
int tock_app_number_writeable_flash_regions(void);
void* tock_app_writeable_flash_region_begins_at(int region_index);
void* tock_app_writeable_flash_region_ends_at(int region_index);


// Checks to see if the given driver number exists on this platform.
bool driver_exists(uint32_t driver);

#define TOCK_SUCCESS       0
#define TOCK_FAIL         -1
#define TOCK_EBUSY        -2
#define TOCK_EALREADY     -3
#define TOCK_EOFF         -4
#define TOCK_ERESERVE     -5
#define TOCK_EINVAL       -6
#define TOCK_ESIZE        -7
#define TOCK_ECANCEL      -8
#define TOCK_ENOMEM       -9
#define TOCK_ENOSUPPORT   -10
#define TOCK_ENODEVICE    -11
#define TOCK_EUNINSTALLED -12
#define TOCK_ENOACK       -13

// Pass this to the subscribe syscall as a function pointer to deactivate the callback.
#define TOCK_DEACTIVATE_CALLBACK    0

// Pass this to the allow syscall as pointer to revoke the "allow"-syscall.
#define TOCK_REVOKE_ALLOW           0

const char* tock_strerror(int tock_errno);
void tock_expect(int expected, int actual, const char* file, unsigned line);
#define TOCK_EXPECT(_e, _a) tock_expect((_e), (_a), __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif
