#include "temp_table.h"
#include <stdio.h>
#include "utils.h"
#include "temperature.h"
#include "setpoint.h"
#include "temp_graph.h"
#include "graphics.h"

void set_temp(struct temp_table *temp_table, uint8_t row, uint16_t new_temp) {
  if(new_temp > MAX_TEMP) return; // save new temp only if it is smaller than 1300°C

  //struct list<setpoint> *old_setpoint_list = temp_table->setpoint_list;
  // display.Set_Draw_color(BACKGROUND_COLOR);
  // draw_temp_curve(temp_table->temp_graph, temp_table->setpoint_list);
  setpoint *new_setpoint = &get_index(temp_table->setpoint_list, row)->data;
  // setpoint old_setpoint;
  // memcpy(&old_setpoint, new_setpoint, sizeof(setpoint));
  new_setpoint->temp = new_temp;
  // display.Set_Draw_color(0, 0, 0);
  // draw_temp_curve(temp_table->temp_graph, temp_table->setpoint_list);
  //update_temp_curve(old_setpoint_list, temp_table->setpoint_list);

  //edit_point(temp_table->temp_graph, temp_table->setpoint_list, old_setpoint, row, row);
}

uint16_t get_temp(struct temp_table *temp_table, uint8_t row) {
  return get_index(temp_table->setpoint_list, row)->data.temp;
}

void set_time(struct temp_table *temp_table, uint8_t row, char *new_time_str) {
  if(!row) return;

  setpoint *edit_setpoint = &get_index(temp_table->setpoint_list, row)->data;
  char *time_str = edit_setpoint->time;
  if(is_valid_time_str(time_str)) strcpy(temp_table->pre_edit, time_str);

  if(is_valid_time_str(new_time_str)) { // only sort setpoint_list if the new time string is valid
    uint8_t new_row = row;
    setpoint old_setpoint;
    old_setpoint.temp = edit_setpoint->temp;
    strcpy(old_setpoint.time, temp_table->pre_edit);

    uint16_t prev_time = time_str_to_min(get_index(temp_table->setpoint_list, row - 1)->data.time);
    uint16_t curr_time = time_str_to_min(new_time_str);
    uint16_t next_time = time_str_to_min(get_index(temp_table->setpoint_list, row + 1)->data.time);    
    
    if(curr_time < prev_time || curr_time > next_time) { // checks whether the list has to be sorted again, i.e. if the new time value is higher/lower than the time value of adjacent rows
      uint16_t temp = edit_setpoint->temp; // save temperature value
      
      strcpy(time_str, temp_table->pre_edit);
      remove_point(temp_table->temp_graph, temp_table->setpoint_list, row); // removes point from graph
      Serial.println(edit_setpoint->time);
      Serial.println(edit_setpoint->temp);
      remove_setpoint(temp_table->setpoint_list, row); // remove setpoint and add it again so it is inserted in the correct position (sorted in a pre-sorted list)
      new_row = add_setpoint(temp_table->setpoint_list, time_str_to_min(new_time_str), temp, row); // add setpoint as close as possible to its original position if it has the same timestamp as another setpoint
      add_point(temp_table->temp_graph, temp_table->setpoint_list, new_row);

      fill_table_section(temp_table, min(row, new_row), max(row, new_row)); // redraw the part of the table between the old and the new position of the setpoint

      temp_table->selected_field = new_row * 2; // select the field where the setpoint is now
    } else { // otherwise, just add the new time string in place
      strcpy(time_str, new_time_str);
      edit_point(temp_table->temp_graph, temp_table->setpoint_list, old_setpoint, row);
    }

    
  } else {
    strcpy(time_str, new_time_str);
  }
}

char *get_time(struct temp_table *temp_table, uint8_t row) {
  return get_index(temp_table->setpoint_list, row)->data.time;
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

void fill_table_section(struct temp_table *temp_table, uint8_t start_row, uint8_t end_row) {
  struct list<setpoint> *setpoint_iter = get_index(temp_table->setpoint_list, start_row);
  uint8_t i = start_row;
  while(setpoint_iter->next) { // fill fields which get a value
    fill_row(temp_table, i);
    i++;
    setpoint_iter = setpoint_iter->next;
  }
  i++;
  for(; i<temp_table->temp_table.rows && i <= end_row; i++) { // fill empty fields
    clear_row(temp_table, i);
  }
}

// fill all the fields with their value and background color
void fill_table(struct temp_table *temp_table) {
  fill_table_section(temp_table, 0, temp_table->temp_table.rows - 1);
}

uint8_t is_valid_time_str(char *time_str) {
  for(uint8_t i = 0; time_str[i] != '\0'; i++) { // iterate over time string and check for spaces
    if(time_str[i] == ' ') {
      return 0;
    }
  }
  return 1;
}

// makes a time string valid again by replacing it with the saved previous value of the time field if the current time field is incomplete, i.e. containing spaces
// also returns if the time string was valid or had to be changed
uint8_t validate_time_str(char *time_str, char *replacement) {
  if(!is_valid_time_str(time_str)) {
    strcpy(time_str, replacement); // replace time string with previous value
    return 0;
  }
  return 1;
}