#pragma once

#define MODEL ILI9488_18
#define LED  -1 //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V
#define CS    7
#define CD    9
#define RST   8

#define TOPROW_HEIGHT 48
#define TABLE_WIDTH 128

#define WHITE 0xFFFF
#define BLACK 0x0000

#define TEMP_MAX 1300

typedef struct { 
  uint16_t x;
  uint16_t y;
} position;

typedef struct {
  position pos;
  uint16_t width;
  uint16_t height;
  uint8_t rows;
  uint8_t columns;
  float *hlines; // y position of horizontal lines. relative to height if <= 1
  float *vlines; // x position of vertical lines. relative to width if <= 1
} table;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_color;

typedef struct {
  position pos;
  uint16_t width;
  uint16_t height;
  float x_min;
  float x_max;
  float y_min;
  float y_max;
} graph;

void draw_table(table table);
position get_table_entry_position(table table, uint16_t column, uint16_t row);
void write_text_to_field(table table, uint16_t column, uint16_t row, char *text);
void change_field_color(table table, uint16_t column, uint16_t row, rgb_color color);
void draw_interface();
