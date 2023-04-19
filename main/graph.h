#pragma once

#include <stdint.h>
#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include "graphics.h"

position point_to_position(graph graph, uint16_t x, uint16_t y);
void add_point(graph graph, uint16_t x, uint16_t y, char marker);
void add_y_mark(graph graph, uint16_t y);
void add_x_mark(graph graph, uint16_t x);
void add_line(graph graph, float x1, float y1, float x2, float y2);
void add_pattern_line(graph graph, float x1, float y1, float x2, float y2, char pattern);
void draw_graph(graph graph);