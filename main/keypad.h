#pragma once

#include "list.h"

#define LONG_PRESS_MS 200
#define MULTI_CLICK_MS 50
#define CLICK 1
#define LONG_PRESS 2
#define MULTI_CLICK 4

struct key{
  uint16_t key_code;
  char key;
};

enum key_codes {
  KEY_ESC   = 1<<0,
  KEY_ENTER = 1<<15,

  KEY_0 = 1<<1,
  KEY_1 = 1<<12,
  KEY_2 = 1<<13,
  KEY_3 = 1<<14,
  KEY_4 = 1<<8,
  KEY_5 = 1<<9,
  KEY_6 = 1<<10,
  KEY_7 = 1<<4,
  KEY_8 = 1<<5,
  KEY_9 = 1<<6,

  KEY_UP    = 1<<11,
  KEY_DOWN  = 1<<7,
  KEY_LEFT  = 1<<2,
  KEY_RIGHT = 1<<3,

  KEY_ARROW = KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT,
  KEY_NUMBER = KEY_0|KEY_1|KEY_2|KEY_3|KEY_4|KEY_5|KEY_6|KEY_7|KEY_8|KEY_9
};

char map_key(uint16_t key_code);
uint8_t get_keypad_state();
void select_keypad_column(uint8_t column);
uint16_t read_keypad();