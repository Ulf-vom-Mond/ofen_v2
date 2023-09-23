#include "setpoint.h"
#include "list.h"
#include "utils.h"
#include <stdio.h>

uint16_t time_str_to_min(char time_str[6]) {
  return char_to_int(time_str[0]) * 600 + char_to_int(time_str[1]) * 60 + char_to_int(time_str[3]) * 10 + char_to_int(time_str[4]);
}

void min_to_time_str(uint16_t minutes, char *dest) {
  sprintf(dest, "%02d:%02d", minutes/60, minutes%60);
}

// add a new setpoint in a way to keep the setpoint list sorted by time. When there are multiple setpoints with the same timestamp, the offset value determines the position of the new setpoint
uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp, uint8_t offset) {
  struct list<setpoint> *setpoint_iter = setpoint_list;
  uint8_t i = 1;
  while(setpoint_iter->next && time_str_to_min(setpoint_iter->next->data.time) < time + (offset > i)) { // iterate and check where to put the setpoint
    setpoint_iter = setpoint_iter->next;
    i++;
  }
  add_next<setpoint>(setpoint_iter, (setpoint){"", temp});
  min_to_time_str(time, setpoint_iter->next->data.time);
  return i; // return the final position of the new setpoint
}

// add setpoint at latest possible position
uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp) {
  add_setpoint(setpoint_list, time, temp, -1);
}

// remove setpoint at given index and return the head of the new list. this is neccessary if the first setpoint should be removed
list<setpoint>* remove_setpoint(struct list<setpoint> *setpoint_list, uint8_t index) {
  if(!index) {
    return remove_first(setpoint_list);
  }
  remove_next(get_index(setpoint_list, index - 1));
  return setpoint_list;
}