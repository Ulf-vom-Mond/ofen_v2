#include <Arduino.h>
#include <LCDWIKI_GUI.h>
#include "user_interface.h"
#include "display.h"
#include "list.h"
#include "keypad.h"
#include "table.h"
#include "temperature.h"


void arrow_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void number_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void esc_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void enter_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void delete_row_event_listener(uint16_t keys, struct temp_table *temp_table);
void add_row_event_listener(uint16_t keys, struct temp_table *temp_table);

float cols[] = {-1, 0.55};
float rows[] = {-1, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252};
char pre_edit[6] = "00:00";

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

void launch_ui() {
  struct list<setpoint> *setpoint_list = new_list<setpoint>();
  struct temp_table *temp_table = (struct temp_table *)malloc(sizeof(struct temp_table));
  table *table = create_table(TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT, 13, 2);
  *temp_table = (struct temp_table){.temp_table = *table, .selected_field = 0, .setpoint_list = setpoint_list};
  temp_table->temp_table.hlines = rows;
  temp_table->temp_table.vlines = cols;
  
  add_event_listener((event_listener*)&arrow_click_event_listener, KEY_RIGHT|KEY_LEFT, CLICK, temp_table);
  add_event_listener((event_listener*)&arrow_click_event_listener, KEY_UP|KEY_DOWN, CLICK|MULTI_CLICK, temp_table);
  add_event_listener((event_listener*)&number_click_event_listener, KEY_NUMBER, CLICK, temp_table);
  add_event_listener((event_listener*)&esc_click_event_listener, KEY_ESC, CLICK, temp_table);
  add_event_listener((event_listener*)&enter_click_event_listener, KEY_ENTER, CLICK, temp_table);
  add_event_listener((event_listener*)&delete_row_event_listener, KEY_LEFT, LONG_PRESS, temp_table);
  add_event_listener((event_listener*)&add_row_event_listener, KEY_RIGHT, LONG_PRESS, temp_table);

  add_last<setpoint>(setpoint_list, (setpoint){"00:00", 0});
  add_setpoint(setpoint_list, 20, 150);
  add_setpoint(setpoint_list, 70, 800);
  add_setpoint(setpoint_list, 260, 200);
  add_setpoint(setpoint_list, 80, 1200);
  add_setpoint(setpoint_list, 80, 1000);
  add_setpoint(setpoint_list, 4140, 0);
  
  draw_interface();
  
  draw_temp_table(&temp_table->temp_table);

  fill_table(temp_table);
  display.Set_Draw_color(0, 50, 255);
  highlight_field(&temp_table->temp_table, 0, 1);
}

// makes a time string valid again by replacing it with the saved previous value of the time field if the current time field is incomplete, i.e. containing spaces
// also returns if the time string was valid or had to be changed
uint8_t validate_time_str(char *time_str) {
  for(uint8_t i = 0; time_str[i] != '\0'; i++) { // iterate over time string and check for spaces
    if(time_str[i] == ' ') {
      strcpy(time_str, pre_edit); // replace time string with previous value
      return 1;
    }
  }
  return 0;
}

void arrow_click_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t rows = list_size<setpoint>(temp_table->setpoint_list); // total number of setpoint list entries
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  display.Set_Draw_color(0, 0, 0); // draw color gets manipulated at the end of this function, set it to black

  if(!col && row) { // previously selected field is a time field, but not the first one. those fields may require the table to be sorted (by time) again if the time value has been changed
    setpoint *edit_setpoint = &get_index(temp_table->setpoint_list, row)->data;
    
    validate_time_str(edit_setpoint->time); // make time string valid of it contains spaces

    uint16_t prev_time = time_str_to_min(get_index(temp_table->setpoint_list, row - 1)->data.time);
    uint16_t curr_time = time_str_to_min(edit_setpoint->time);
    uint16_t next_time = time_str_to_min(get_index(temp_table->setpoint_list, row + 1)->data.time);    
    
    if(curr_time < prev_time || curr_time > next_time) { // checks whether the list has to be sorted again, i.e. if the new time value is higher/lower than the time value of adjacent rows
      char time_str[6] = "";
      strcpy(time_str, edit_setpoint->time); // save time value
      uint16_t temp = edit_setpoint->temp; // save temperature value
      
      remove_setpoint(temp_table->setpoint_list, row); // remove setpoint and add it again so it is inserted in the correct position (sorted in a pre-sorted list)
      uint8_t new_row = add_setpoint(temp_table->setpoint_list, time_str_to_min(time_str), temp, row); // add setpoint as close as possible to its original position if it has the same timestamp as another setpoint

      for(uint8_t i = min(row, new_row); i <= max(row, new_row); i++) { // redraw the part of the table between the old and the new position of the setpoint
        fill_row(temp_table, i);
      }

      temp_table->selected_field = new_row * 2; // select the field where the setpoint is now
    }
  }
  fill_field(temp_table, col, row);

  // move the selected field according to the pressed arrow key and check whether movement collides with the table border
  if(keys == KEY_LEFT && temp_table->selected_field > 0) { // left
    temp_table->selected_field -= 1;
  } else if(keys == KEY_RIGHT && temp_table->selected_field < rows*2 - 1) { // right
    temp_table->selected_field += 1;
  } else if(keys == KEY_DOWN && temp_table->selected_field < rows*2 - 2) { // down
    temp_table->selected_field += 2;
  } else if(keys == KEY_UP && temp_table->selected_field > 1) { // up
    temp_table->selected_field -= 2;
  }

  // highlight new field
  display.Set_Draw_color(0, 50, 255);
  col = temp_table->selected_field % 2;
  row = temp_table->selected_field / 2;
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void number_click_event_listener(uint16_t keys, struct temp_table *temp_table) { // number keys add a digit
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(col) { // selected field is temperature
    uint16_t *temp = &get_index(temp_table->setpoint_list, row)->data.temp;
    uint16_t new_temp = *temp * 10 + char_to_int(map_key(keys)); // left shift field contents and add new digit to the right
    if(new_temp <= MAX_TEMP) *temp = new_temp; // save new temp if smaller than 1300°C
  } else { // selected field is time
    char *time_char = get_index(temp_table->setpoint_list, row)->data.time; // get pointer to time string and create iterator variable
    uint8_t i = 0;
    while(*time_char != ' ' && *time_char != '\0') { // iterate over the time string up to the first empty digit or up to the end
      time_char++;
      i++;
    }
    if(*time_char) *time_char = map_key(keys); // fill first empty digit with clicked number if loop did not reach the end of the time string
    if(char_to_int(*time_char) > 5 && i == 3) *time_char = ' '; // cannot add a digit higher than 5 directly after the colon because maximum value for minutes is :59
  }
  fill_field(temp_table, col, row); // redraw field
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void esc_click_event_listener(uint16_t keys, struct temp_table *temp_table) { // escape deletes the last digit
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(!col && !row) return; // cannot edit the very first timestamp (always 00:00)
  if(col) { // selected field is temperature
    get_index(temp_table->setpoint_list, row)->data.temp /= 10; // divide temperature by 10, effectively a decimal right shift to delete last digit
  } else { // selected field is time
    char *time_str = get_index(temp_table->setpoint_list, row)->data.time; // get pointer to time string
    char *time_char = time_str; // create iterator over the time string characters
    uint8_t i = 0;
    while(*time_char != ' ' && *time_char != '\0') { // iterate
      time_char++;
      i++;
    }
    if(i == 5) strcpy(pre_edit, time_str); // if no character is blank, save the previous value of the time field (so don't override pre_edit with an incomplete time string)
    if(i == 3) *(time_char - 2) = ' '; // if "cursor" is right before the colon, delete the digit left of the colon, not the colon itself
    else if(i) *(time_char - 1) = ' '; // otherwise, delete right most digit
  }
  fill_field(temp_table, col, row); // redraw field
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void enter_click_event_listener(uint16_t keys, struct temp_table *temp_table) { // enter clears the entry of the corresponding field
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(!col && !row) return; // cannot edit the very first timestamp (always 00:00)
  if(col) { // selected field is temperature
    get_index(temp_table->setpoint_list, row)->data.temp = 0; // set temperature to 0
  } else { // selected field is time
    char *time_str = get_index(temp_table->setpoint_list, row)->data.time; // get pointer to time string
    if(*time_str != ' ') strcpy(pre_edit, time_str); // save the previous value of the time field only if the field is not already cleared (could happen on a double click)
    strcpy(time_str, "  :  "); // clear time
  }
  fill_field(temp_table, col, row); // update the field
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void delete_row_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t rows = list_size<setpoint>(temp_table->setpoint_list); // total number of setpoint list entries
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(row) { // cannot delete very first row
    remove_setpoint(temp_table->setpoint_list, row);
    uint8_t i = row;
    for(; i < rows - 1; i++) { // redraw all the rows after the previous row
      fill_row(temp_table, i);
    }
    clear_row(temp_table, i+1); // whiteout the last row as it moved one up
    if(row == rows - 1) { // select the previous row if the delted row is the last one
      row--;
      temp_table->selected_field -= 2;
    }
    highlight_field(&temp_table->temp_table, col, row + 1);
  }
}

void add_row_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t rows = list_size<setpoint>(temp_table->setpoint_list); // total number of setpoint list entries
  if(rows >= temp_table->temp_table.rows - 2) return; // dont add row if table is full
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  struct list<setpoint> *prev_setpoint = get_index(temp_table->setpoint_list, row); // save setpoint that comes before the newly added setpoint
  add_setpoint(temp_table->setpoint_list, time_str_to_min(prev_setpoint->data.time), 0, row + 1); // add setpoint with same timestamp as the previous one, directly after the previous one
  for(uint8_t i = row; i < rows + 1; i++) { // redraw all the rows after the previous row
    fill_row(temp_table, i);
  }
  temp_table->selected_field += 2; // select and highlight newly created row
  highlight_field(&temp_table->temp_table, col, row + 2);
}