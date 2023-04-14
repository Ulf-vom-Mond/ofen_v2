#pragma once

#include "list.h"


struct key{
  uint16_t key_code;
  char key;
};

extern struct list *event_listener_list;

typedef void *event_listener(uint16_t keys); // type definition for the callback function. Callback functions get passed a list which contains all pressed keys

char map_key(uint16_t key_code);
uint8_t char_to_int(char character);
uint8_t get_keypad_state();
void select_keypad_column(uint8_t column);
void add_event_listener(event_listener *new_event_listener);
void read_keypad();