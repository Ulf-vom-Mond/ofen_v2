#include "utils.h"

uint8_t count_decimals(int16_t num) {
  if(num < 10 && num > -10) {
    return 1;
  } else {
    return count_decimals(num / 10) + 1;
  }
}

uint8_t char_to_int(char character) { // ascii character to digit conversion
  if(character < 48) {return 10;}
  if(character > 57) {return 10;}
  return character & 0x0F;
}