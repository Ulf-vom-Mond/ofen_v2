#include "keypad.h"
#include "shift_reg.h"
#include "avrio.h"
#include "pins.h"
#include <stdlib.h>
#include <Arduino.h>

struct list *event_listener_list = new_list(); // this list contains all the callback functions which shall be called on a keystroke

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

uint16_t old_keys = 0;
// uint16_t pressed_keys = 0;
// uint16_t released_keys = 0;

// uint32_t time_of_last_read = 0;

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

void add_event_listener(event_listener *new_event_listener) {
  add_last(event_listener_list, (void *)new_event_listener);
}

void read_keypad() {
  uint16_t keys = 0;

  for(uint8_t column = 0; column < 4; column++) {
    select_keypad_column(column);
    uint16_t keypad_state = get_keypad_state();
    keys |= keypad_state << (4 * column);
  }

  uint16_t pressed_keys  = ~old_keys & keys; // all the keys that were not pressed on the last call but that are pressed now
  // released_keys =  keys & ~new_keys; // all the keys that were pressed on the last call but that are not pressed now
  // uint16_t clicked_keys =  // all the keys that were pressed only for a short amount auf time
  // uint16_t held_keys // all the keys that are currently pressed
  // uint16_t double_clicked_keys // all the keys that were pressed twice for a short time

  old_keys = keys;

  if(pressed_keys == 0) {return;}

  struct list *current_event_listener = event_listener_list;
  while(current_event_listener->next){
    (**((event_listener*)current_event_listener->data))(pressed_keys);
    current_event_listener = current_event_listener->next;
  }

  // time_of_last_read = millis();
}