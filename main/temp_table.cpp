#include "temp_table.h"
#include <stdio.h>
#include "utils.h"

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

void fill_field(struct temp_table *temp_table, uint8_t column, uint8_t row) { // fill a field with its value and color
  char field_str[6] = "";
  if(column%2) { // selected field is a temperature field
    uint16_t temp = get_index(temp_table->setpoint_list, row)->data.temp;
    sprintf(field_str, "%4d", temp); // write temperature to field string with right alignment
    change_field_color(&temp_table->temp_table, column, row + 1, temperature_to_color(temp));
  } else { // selected field is a time field
    strcpy(field_str, get_index(temp_table->setpoint_list, row)->data.time);
    change_field_color(&temp_table->temp_table, column, row + 1, {200, 200, 200});
  }
  write_string_to_field(&temp_table->temp_table, column, row + 1, field_str); // update field contents
}

void fill_row(struct temp_table *temp_table, uint8_t row) { // fill both fields in a row with their value and background color
  fill_field(temp_table, 0, row);
  fill_field(temp_table, 1, row);
}

void clear_row(struct temp_table *temp_table, uint8_t row) { // fill both fields in a row with a grey color (emptying them)
  change_field_color(&temp_table->temp_table, 0, row, {200, 200, 200});
  change_field_color(&temp_table->temp_table, 1, row, {200, 200, 200});
}

// fill all the fields with their value and background color
void fill_table(struct temp_table *temp_table) {
  struct list<setpoint> *setpoint_iter = temp_table->setpoint_list;
  uint8_t i = 0;
  while(setpoint_iter->next) { // fill fields which get a value
    fill_row(temp_table, i);
    i++;
    setpoint_iter = setpoint_iter->next;
  }
  for(; i<temp_table->temp_table.rows - 1; i++) { // fill empty fields
    clear_row(temp_table, i + 1);
  }
}

// makes a time string valid again by replacing it with the saved previous value of the time field if the current time field is incomplete, i.e. containing spaces
// also returns if the time string was valid or had to be changed
uint8_t validate_time_str(char *time_str, char *replacement) {
  for(uint8_t i = 0; time_str[i] != '\0'; i++) { // iterate over time string and check for spaces
    if(time_str[i] == ' ') {
      strcpy(time_str, replacement); // replace time string with previous value
      return 1;
    }
  }
  return 0;
}