#include "keypad.h"
#include "shift_reg.h"
#include "avrio.h"
#include "pins.h"
#include <stdlib.h>
#include <Arduino.h>

struct list<struct event_listener_conf> *event_listener_list = new_list<struct event_listener_conf>(); // this list contains all the callback functions which shall be called on a keystroke

struct key key_map[16] = {
  {1,     '*'},
  {2,     '0'},
  {4,     '#'},
  {8,     'D'},
  {16,    '7'},
  {32,    '8'},
  {64,    '9'},
  {128,   'C'},
  {256,   '4'},
  {512,   '5'},
  {1024,  '6'},
  {2048,  'B'},
  {4096,  '1'},
  {8192,  '2'},
  {16384, '3'},
  {32768, 'A'}
};


uint16_t multi_click_keys = 0;
uint16_t key_times[16] = {0};
unsigned long last_update = 0;


char map_key(uint16_t key_code) {
  for(uint8_t i = 0; i < 16; i++) {
    if(key_map[i].key_code & key_code) {return key_map[i].key;}
  }
}

uint8_t char_to_int(char character) {
  if(character < 48) {return 10;}
  if(character > 57) {return 10;}
  return character & 0x0F;
}

uint8_t get_keypad_state() {
  uint8_t shift_reg = read_shift_reg();
  return shift_reg & 0b00001111;
}

void select_keypad_column(uint8_t column) {
  write(DEMUX_A0_PIN, column & 0x01);
  write(DEMUX_A1_PIN, (column >> 1) & 0x01);
}

void add_event_listener(event_listener *new_event_listener, uint16_t mask, uint8_t event_types, void *params) {
  add_last<struct event_listener_conf>(event_listener_list, {new_event_listener, mask, event_types, params});
}

void read_keypad() {
  uint16_t keys = 0;

  for(uint8_t column = 0; column < 4; column++) {
    select_keypad_column(column);
    uint16_t keypad_state = get_keypad_state();
    keys |= keypad_state << (4 * column);
  }

  unsigned long current_ms = millis();
  unsigned long passed_ms = current_ms - last_update;
  last_update = current_ms;

  uint16_t clicked_keys = 0;
  uint16_t long_pressed_keys = 0;

  for(uint8_t i; i<16; i++) {
    uint16_t key_bit_pattern = 0b1 << i;
    if(keys & key_bit_pattern) {
      key_times[i] += passed_ms;
    } else {
      if(key_times[i] < LONG_PRESS_MS && key_times[i] > 0) {
        clicked_keys |= key_bit_pattern;
      }
      key_times[i] = 0;
      multi_click_keys &= ~key_bit_pattern;
    }
    if(key_times[i] >= LONG_PRESS_MS && !(multi_click_keys & key_bit_pattern)) {
      long_pressed_keys |= key_bit_pattern;
      multi_click_keys |= key_bit_pattern;
    }
    if(key_times[i] >= LONG_PRESS_MS + MULTI_CLICK_MS) {
      key_times[i] -= MULTI_CLICK_MS;
      clicked_keys |= key_bit_pattern;
    }
  }

  if(!long_pressed_keys && !clicked_keys) return;

  struct list<struct event_listener_conf> *event_listener_iter = event_listener_list;
  while(event_listener_iter->next){
    struct event_listener_conf listener = event_listener_iter->data;

    uint16_t registered_keys = listener.mask & (
      (listener.event_types & CLICK ? clicked_keys & ~multi_click_keys : 0) | 
      (listener.event_types & LONG_PRESS ? long_pressed_keys : 0) |
      ((listener.event_types & MULTI_CLICK) ? clicked_keys & multi_click_keys : 0));

    if(registered_keys) {
      (**((event_listener*)listener.callback))(registered_keys, listener.params);
    }
    event_listener_iter = event_listener_iter->next;
  }
}