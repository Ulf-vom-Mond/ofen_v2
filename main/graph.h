#pragma once

#include <stdint.h>
#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include "graphics.h"

typedef struct {
  position pos;
  uint16_t width;
  uint16_t height;
  float x_min;
  float x_max;
  float y_min;
  float y_max;
  uint8_t padding;
} graph;

position point_to_position(graph *graph, uint16_t x, uint16_t y);
void add_point(graph *graph, uint16_t x, uint16_t y, char marker);
void add_y_mark(graph *graph, uint16_t y);
void add_x_mark(graph *graph, uint16_t x);
void add_line(graph *graph, float x1, float y1, float x2, float y2);
void add_pattern_line(graph *graph, float x1, float y1, float x2, float y2, char pattern);
void draw_graph(graph *graph);