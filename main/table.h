#pragma once

#include <stdint.h>
#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include "graphics.h"

void draw_table(table table);
position get_table_entry_position(table table, uint16_t column, uint16_t row);
void write_text_to_field(table table, uint16_t column, uint16_t row, char *text);
void change_field_color(table table, uint16_t column, uint16_t row, rgb_color color);
