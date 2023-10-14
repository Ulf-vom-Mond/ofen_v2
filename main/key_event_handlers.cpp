//#include <cstdint>
//#include <cstdint>
#include "HardwareSerial.h"
#include <avr/pgmspace.h>
//#include <cstring>
#include "key_event_handlers.h"
#include "keypad.h"
#include "utils.h"
#include "temperature.h"
#include "setpoint.h"
#include "temp_graph.h"

void arrow_click_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t rows = list_size<setpoint>(temp_table->setpoint_list); // total number of setpoint list entries
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  setpoint old_setpoint;
  display.Set_Draw_color(0, 0, 0); // draw color gets manipulated at the end of this function, set it to black

  if(!col) { // previously selected field is a time field, (but not the first one?). those fields may require the table to be sorted (by time) again if the time value has been changed
    char *new_time_str = get_time(temp_table, row);
    validate_time_str(new_time_str, temp_table->pre_edit);
    set_time(temp_table, row, new_time_str);
  } else {
    memcpy(&old_setpoint.temp, temp_table->pre_edit, 2);
    strcpy(old_setpoint.time, get_index(temp_table->setpoint_list, row)->data.time);

    edit_point(temp_table->temp_graph, temp_table->setpoint_list, old_setpoint, row);
  }
  fill_field(temp_table, col, row);

  // move the selected field according to the pressed arrow key and check whether movement collides with the table border
  uint8_t old_selected_field = temp_table->selected_field;
  if(keys == KEY_LEFT && temp_table->selected_field > 0) { // left
    temp_table->selected_field -= 1;
  } else if(keys == KEY_RIGHT && temp_table->selected_field < rows*2 - 1) { // right
    temp_table->selected_field += 1;
  } else if(keys == KEY_DOWN && temp_table->selected_field < rows*2 - 2) { // down
    temp_table->selected_field += 2;
  } else if(keys == KEY_UP && temp_table->selected_field > 1) { // up
    temp_table->selected_field -= 2;
  }

  col = temp_table->selected_field % 2;
  row = temp_table->selected_field / 2;

  if(col) {
    memcpy(temp_table->pre_edit, &get_index(temp_table->setpoint_list, row)->data.temp, 2);
  }

  //if (old_selected_field != temp_table->selected_field) {
    // highlight new field
    display.Set_Draw_color(0, 50, 255);
    
    highlight_field(&temp_table->temp_table, col, row + 1);

    //
  //}
  
}

void number_click_event_listener(uint16_t keys, struct temp_table *temp_table) { // number keys add a digit
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(col) { // selected field is temperature
    set_temp(temp_table, row, get_temp(temp_table, row) * 10 + char_to_int(map_key(keys))); // left shift field contents and add new digit to the right
  } else { // selected field is time
    char time_str[6] = "";
    strcpy(time_str, get_time(temp_table, row)); // copy time string

    uint8_t i = 0;
    while(time_str[i] != ' ' && time_str[i+1] != '\0') i++; // iterate over the time string up to the first empty digit or up to the end

    time_str[i] = map_key(keys); // fill first empty digit with clicked number
    if(char_to_int(time_str[3]) > 5) time_str[3] = ' '; // cannot add a digit higher than 5 directly after the colon because maximum value for minutes is :59
    
    set_time(temp_table, row, time_str);
  }
  fill_field(temp_table, col, row); // redraw field
  col = temp_table->selected_field % 2;
  row = temp_table->selected_field / 2;
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void esc_click_event_listener(uint16_t keys, struct temp_table *temp_table) { // escape deletes the last digit
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(!col && !row) return; // cannot edit the very first timestamp (always 00:00)
  if(col) { // selected field is temperature
    set_temp(temp_table, row, get_temp(temp_table, row) / 10); // divide temperature by 10, effectively a decimal right shift to delete last digit
  } else { // selected field is time
    char time_str[6] = "";
    strcpy(time_str, get_time(temp_table, row)); // copy time string

    uint8_t i = 0;
    while(time_str[i] != ' ' && time_str[i] != '\0') i++; // iterate over the time string up to the first empty digit or up to the end

    if(i == 3) time_str[i - 2] = ' '; // if "cursor" is right before the colon, delete the digit left of the colon, not the colon itself
    else if(i) time_str[i - 1] = ' '; // otherwise, delete right most digit

    set_time(temp_table, row, time_str);
  }
  fill_field(temp_table, col, row); // redraw field
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void enter_click_event_listener(uint16_t keys, struct temp_table *temp_table) { // enter clears the entry of the corresponding field
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(!col && !row) return; // cannot edit the very first timestamp (always 00:00)
  if(col) { // selected field is temperature
    set_temp(temp_table, row, 0); // set temperature to 0
  } else { // selected field is time
    set_time(temp_table, row, "  :  ");
  }
  fill_field(temp_table, col, row); // update the field
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void delete_row_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t rows = list_size<setpoint>(temp_table->setpoint_list); // total number of setpoint list entries
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(row) { // cannot delete very first row
    remove_point(temp_table->temp_graph, temp_table->setpoint_list, row); // removes point from graph
    remove_setpoint(temp_table->setpoint_list, row); // removes point from list
    fill_table_section(temp_table, row, rows); // removes point from table
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
  fill_table_section(temp_table, row, rows);
  temp_table->selected_field += 2; // select and highlight newly created row
  highlight_field(&temp_table->temp_table, col, row + 2);

  add_point(temp_table->temp_graph, temp_table->setpoint_list, row + 1);
}