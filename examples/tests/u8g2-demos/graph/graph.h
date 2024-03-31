#pragma once


typedef struct {
  int height;
  int width;
  int min;
  int max;
} graph_t;

void graph_init(graph_t* graph, int height, int width, int value_min, int value_max);

void graph_draw(u8g2_t* u8g2, graph_t* graph, int* values, int len);
