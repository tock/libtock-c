#include <stdio.h>
#include <stdlib.h>
#include <u8g2-tock.h>
#include <u8g2.h>

#include <libtock-sync/services/alarm.h>

#include "graph.h"

u8g2_t u8g2;
graph_t graph;

#define SAMPLES 120

int samples[SAMPLES] = {0};

static void append(int* values, int len, int value) {
  for (int i = 0; i < len - 1; i++) {
    values[i] = values[i + 1];
  }
  values[len - 1] = value;
}

int main(void) {
  u8g2_tock_init(&u8g2);

  int width  = u8g2_GetDisplayWidth(&u8g2);
  int height = u8g2_GetDisplayHeight(&u8g2);

  graph_init(&graph, height, width, 0, 20);

  int count = 0;
  while (1) {
    append(samples, SAMPLES, count);
    graph_draw(&u8g2, &graph, samples, SAMPLES);
    count = (count + 2) % 20;
    libtocksync_alarm_delay_ms(1000);
  }
}
