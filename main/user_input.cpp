#include "user_input.h"
#include "shift_reg.h"
#include "avrio.h"
#include "pins.h"
#include <stdlib.h>
#include <Arduino.h>

uint8_t get_rotary_switch_state() {
  uint8_t shift_reg = read_shift_reg();
  return (shift_reg >> 4) & 0b00000111;
}