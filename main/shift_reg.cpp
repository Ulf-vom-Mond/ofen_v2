#include "shift_reg.h"
#include "avrio.h"
#include "pins.h"

uint8_t read_shift_reg() {
  char shift_reg = 0;

  write(SHIFT_LOAD_PIN, 0);
  pulse(SHIFT_CLK_PIN);
  write(SHIFT_LOAD_PIN, 1);

  for(uint8_t i = 0; i < 8; i++) {
    pulse(SHIFT_CLK_PIN);
    shift_reg |= read(SHIFT_Q_PIN) << i;
  }

  return shift_reg;
}

char read_shift_reg_bit(uint8_t bit) {
  uint8_t shift_reg = read_shift_reg();
  return (shift_reg >> (7 - bit)) & 0x01;
}