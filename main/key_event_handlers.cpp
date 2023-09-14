#include "key_event_handlers.h"
#include "keypad.h"
#include "utils.h"
#include "temperature.h"

char pre_edit[6] = "00:00";

void arrow_click_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t rows = list_size<setpoint>(temp_table->setpoint_list); // total number of setpoint list entries
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  display.Set_Draw_color(0, 0, 0); // draw color gets manipulated at the end of this function, set it to black

  if(!col && row) { // previously selected field is a time field, but not the first one. those fields may require the table to be sorted (by time) again if the time value has been changed
    setpoint *edit_setpoint = &get_index(temp_table->setpoint_list, row)->data;
    
    validate_time_str(edit_setpoint->time, pre_edit); // make time string valid of it contains spaces

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