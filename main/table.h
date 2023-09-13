#pragma once

#include <stdint.h>
#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include "graphics.h"

typedef struct {
  position pos;
  uint16_t width;
  uint16_t height;
  uint8_t rows;
  uint8_t columns;
  float *hlines; // y position of horizontal lines. relative to height if <= 1
  float *vlines; // x position of vertical lines. relative to width if <= 1
  rgb_color *field_colors;
} table;

table *create_table(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t rows, uint16_t cols);
void delete_table(table *my_table);
void draw_table(table *table);
position get_table_entry_position(table *table, uint16_t column, uint16_t row);
void write_string_to_field(table *table, uint16_t column, uint16_t row, char *text);
void change_field_color(table *table, uint16_t column, uint16_t row, rgb_color color);
void erase_field(table *table, uint16_t column, uint16_t row);
void highlight_field(table *table, uint16_t column, uint16_t row);