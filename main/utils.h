#pragma once

#include <stdint.h>

uint8_t count_decimals(int16_t num);
uint8_t char_to_int(char character);

template <typename T>
T min(T a, T b) {
  return a<b?a:b;
}

template <typename T>
T max(T a, T b) {
  return a>b?a:b;
}