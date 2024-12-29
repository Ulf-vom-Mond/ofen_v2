#pragma once

#include <stdint.h>

#define MODEL ILI9488_18
// Theses are Arduino pins which is necessary for the LCD library
#define LED  -1 //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V
#define CS    7
#define CD    9
#define RST   8

#define HEIGHT 320
#define WIDTH  480

#define MARGIN 4

#define TOP_ROW_HEIGHT 32
#define TOP_ROW_WIDTH  WIDTH - 2 * MARGIN
#define TOP_ROW_X      MARGIN
#define TOP_ROW_Y      MARGIN

#define TABLE_HEIGHT   HEIGHT - TOP_ROW_HEIGHT - 4 * MARGIN
#define TABLE_WIDTH    128
#define TABLE_X        WIDTH - TABLE_WIDTH - MARGIN
#define TABLE_Y        TOP_ROW_HEIGHT + 3 * MARGIN

#define GRAPH_HEIGHT   TABLE_HEIGHT
#define GRAPH_WIDTH    WIDTH - TABLE_WIDTH - 4 * MARGIN
#define GRAPH_X        MARGIN
#define GRAPH_Y        TABLE_Y

#define WHITE   0xFFFF
#define GREY    0x7BEF
#define BLACK   0x0000
#define RED     0xF800
#define BLUE    0x001F
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define ORANGE  0xF870

#define BACKGROUND_COLOR 0xFFFF
#define LINE_COLOR 0x0000
#define HIGHLIGHT_COLOR 0x00FF

#define DOTS      0b01010101
#define DASHS     0b00001111
#define SOLID     0b11111111
#define DOTDASH   0b00100111
#define SHORTDASH 0b00110011

extern LCDWIKI_SPI display;

typedef struct { 
  uint16_t x;
  uint16_t y;
} position;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_color;

rgb_color temperature_to_color(uint16_t temp);
void draw_arrow(position head, uint8_t direction, uint16_t length, uint16_t width);
void draw_pattern_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, char pattern);
