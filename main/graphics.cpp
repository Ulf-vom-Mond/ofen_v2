#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <SPI.h>
#include "graphics.h"

LCDWIKI_SPI display(MODEL,CS,CD,RST,LED);

rgb_color temperature_to_color(uint16_t temp) { // gives a green-yellow-red gradient based on the given temperature
  uint8_t r = 255;
  uint8_t g = 255;
  uint8_t b = 30;

  if(temp * 2 > TEMP_MAX){
    g = 255 * (TEMP_MAX - (uint32_t)temp) / (uint32_t)temp;
  } else {
    r = 255 * (uint32_t)temp / (TEMP_MAX - (uint32_t)temp);
  }
  return {r, g, b};
}

void draw_arrow(position head, uint8_t direction, uint16_t length, uint16_t width) {
  int16_t size_x = width;
  int16_t size_y = length;

  switch(direction) {
    case 1:
      size_x = -length;
      size_y = width;
    break;
    case 2:
      size_y = -length;
    break;
    case 3:
      size_x = length;
      size_y = width;
    break;
  }

  display.Draw_Line(head.x, head.y, head.x + size_x, head.y + size_y);

  switch(direction) {
    case 0:
    case 2:
      size_x = -size_x;
    break;
    case 1:
    case 3:
      size_y = -size_y;
    break;
  }

  display.Draw_Line(head.x, head.y, head.x + size_x, head.y + size_y);
}

void draw_pattern_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, char pattern) {
  if(x2 == x1 && y2 == y1) {
    display.Draw_Pixel(x1, y1);
  }

  int16_t Dx = x2 - x1;
  int16_t Dy = y2 - y1;

  float dx = 1;
  float dy = 1;

  uint16_t i_max;

  if(abs(Dx) > abs(Dy)) {
    dy = (float)Dy / (float)Dx;
    i_max = abs(Dx);

    if(Dx < 0) {
      dx = -1;
      dy = -dy;
    }
  } else {
    dx = (float)Dx / (float)Dy;
    i_max = abs(Dy);

    if(Dy < 0) {
      dx = -dx;
      dy = -1;
    }
  }

  for(uint16_t i = 0; i < i_max; i++) {
    if(pattern & (0x1 << i % 8)) {
      display.Draw_Pixel(x1 + i * dx, y1 + i * dy);
    }
  }
}

