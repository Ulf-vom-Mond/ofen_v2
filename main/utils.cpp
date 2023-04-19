#include "utils.h"

uint8_t count_decimals(int16_t num) {
  if(num < 10 && num > -10) {
    return 1;
  } else {
    return count_decimals(num / 10) + 1;
  }
}