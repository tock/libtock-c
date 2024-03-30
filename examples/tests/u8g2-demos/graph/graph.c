#include <u8g2-tock.h>
#include <u8g2.h>

#include "graph.h"

void graph_init(graph_t* graph, int height, int width, int value_max, int value_min) {
  graph->height = height;
  graph->width = width;
  graph->max = value_max;
  graph->min = value_min;
}

void graph_draw(u8g2_t* u8g2, graph_t* graph, int* values, int len) {
  u8g2_ClearBuffer(u8g2);

  // Y axis
  u8g2_DrawLine(u8g2, 1, 1, 1, graph->height - 2);
  // X axis
  u8g2_DrawLine(u8g2, 1, graph->height - 2, graph->width-2, graph->height - 2);

  int y_pixels = graph->height - 4;
  int spacing = (graph->width - 4) / len;
  int x_offset = 2;

  int x = x_offset;
  for (int i=0; i<len; i++) {
    int s = values[i];

    int y_vert;
    if (s <= graph->min) {
      y_vert = 0;
    } else if (s >= graph->max) {
      y_vert = y_pixels;
    } else {
      y_vert = ((s - graph->min)* y_pixels) / (graph->max - graph->min);
    }

    int y = graph->height - 2 - y_vert;

    u8g2_DrawPixel(u8g2, x, y);

    x += spacing;
  }

  u8g2_SendBuffer(u8g2);
}
