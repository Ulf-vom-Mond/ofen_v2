#pragma once

#include <stdint.h>

template <typename T>
struct list;

typedef struct setpoint {
  char time[6];
  uint16_t temp;
} setpoint;

uint16_t time_str_to_min(char time_str[6]);
void min_to_time_str(uint16_t minutes, char *dest);
uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp, uint8_t offset);
uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp);
list<setpoint>* remove_setpoint(struct list<setpoint> *setpoint_list, uint8_t index);