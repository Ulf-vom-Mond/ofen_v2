#include "keypad.h"
#include "shift_reg.h"
#include "avrio.h"
#include "pins.h"
#include <stdlib.h>
#include <Arduino.h>

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

char map_key(uint16_t key_code) { // maps the keycode to a character using the key_map array
  for(uint8_t i = 0; i < 16; i++) {
    if(key_map[i].key_code & key_code) {return key_map[i].key;}
  }
  return 0;
}

uint8_t get_keypad_state() { // gives a bit pattern representing the key states of the currently selecet keypad column. 1 means held down, 0 means not pressed
  uint8_t shift_reg = read_shift_reg();
  return shift_reg & 0b00001111;
}

void select_keypad_column(uint8_t column) { // write the first two bits of column to pins DEMUX_A0 and DEMUX_A1 to select one of 4 keypad columns
  write(DEMUX_A0_PIN, column & 0x01); // write first bit of column
  write(DEMUX_A1_PIN, (column >> 1) & 0x01); // write second bit of columns
}

uint16_t read_keypad() { // reads the keypad state, determines the active key event for every key and notifies all the registered event_listeners
  uint16_t keys = 0;
  for(uint8_t column = 0; column < 4; column++) { // read all 4 keypad columns and save the state of every key in the according bit of keys
    select_keypad_column(column);
    uint16_t keypad_state = get_keypad_state();
    keys |= keypad_state << (4 * column);
  }

  return keys;
}