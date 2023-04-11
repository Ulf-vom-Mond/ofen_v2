#include "user_input.h"
#include "shift_reg.h"
#include "avrio.h"
#include "pins.h"

uint8_t get_rotary_switch_state() {
  uint8_t shift_reg = read_shift_reg();
  return (shift_reg >> 4) & 0b00000111;
}

uint8_t get_keypad_state() {
  uint8_t shift_reg = read_shift_reg();
  return (shift_reg >> 1) & 0b00001111;
}

void select_keypad_column(uint8_t column) {
  write(DEMUX_A0_PIN, column & 0x01);
  write(DEMUX_A1_PIN, (column >> 1) & 0x01);
}