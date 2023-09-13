#include <Arduino.h>
#include <LCDWIKI_GUI.h>
#include "user_interface.h"
#include "display.h"
#include "list.h"
#include "keypad.h"
#include "table.h"
#include "temperature.h"


void arrow_key_event_listener(uint16_t keys, struct temp_table *temp_table);
void number_key_event_listener(uint16_t keys, struct temp_table *temp_table);
void esc_key_event_listener(uint16_t keys, struct temp_table *temp_table);
void enter_key_event_listener(uint16_t keys, struct temp_table *temp_table);

float cols[] = {-1, 0.55};
float rows[] = {-1, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252};
//table *temp_table = create_table(TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT, 13, 2);

uint16_t time_str_to_min(char time_str[6]) {
  return char_to_int(time_str[0]) * 600 + char_to_int(time_str[1]) * 60 + char_to_int(time_str[3]) * 10 + char_to_int(time_str[4]);
}

void min_to_time_str(uint16_t minutes, char *dest) {
  sprintf(dest, "%02d:%02d", minutes/60, minutes%60);
}

uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp, uint8_t offset) {
  struct list<setpoint> *setpoint_iter = setpoint_list;
  uint8_t i = 1;
  while(setpoint_iter->next && time_str_to_min(setpoint_iter->next->data.time) < time + (1 & (offset > i))) {
    setpoint_iter = setpoint_iter->next;
    i++;
  }
  // char time_str[6] = {};
  // min_to_time_str(time, time_str);
  add_next<setpoint>(setpoint_iter, (setpoint){"", temp});
  min_to_time_str(time, setpoint_iter->next->data.time);
  return i;
}

uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp) {
  add_setpoint(setpoint_list, time, temp, -1);
}

list<setpoint>* remove_setpoint(struct list<setpoint> *setpoint_list, uint8_t index) {
  if(!index) {
    return remove_first(setpoint_list);
  }
  remove_next(get_index(setpoint_list, index - 1));
  return setpoint_list;
}

void fill_field(struct temp_table *temp_table, uint8_t column, uint8_t row) {
  char field_str[6] = "";
  if(column%2) {
    uint16_t temp = get_index(temp_table->setpoint_list, row)->data.temp;
    sprintf(field_str, "%4d", temp);
    change_field_color(&temp_table->temp_table, column, row + 1, temperature_to_color(temp));
  } else {
    //uint16_t minutes = get_index(temp_table->setpoint_list, row)->data.time;
    //sprintf(field_str, "%02d:%02d", minutes/60, minutes%60);
    strcpy(field_str, get_index(temp_table->setpoint_list, row)->data.time);
    change_field_color(&temp_table->temp_table, column, row + 1, {200, 200, 200});
  }
  write_string_to_field(&temp_table->temp_table, column, row + 1, field_str);
}

void fill_table(struct temp_table *temp_table) {
  struct list<setpoint> *setpoint_iter = temp_table->setpoint_list;
  uint8_t i = 0;
  while(setpoint_iter->next) {
    fill_field(temp_table, 0, i);
    fill_field(temp_table, 1, i);
    i++;
    setpoint_iter = setpoint_iter->next;
  }
  for(; i<temp_table->temp_table.rows - 1; i++) {
    change_field_color(&temp_table->temp_table, 0, i + 1, {200, 200, 200});
    change_field_color(&temp_table->temp_table, 1, i + 1, {200, 200, 200});
  }
  display.Set_Draw_color(0, 50, 255);
}

void launch_ui() {
  struct list<setpoint> *setpoint_list = new_list<setpoint>();
  struct temp_table *temp_table = (struct temp_table *)malloc(sizeof(struct temp_table));
  table *table = create_table(TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT, 13, 2);
  *temp_table = (struct temp_table){.temp_table = *table, .selected_field = 0, .setpoint_list = setpoint_list};
  temp_table->temp_table.hlines = rows;
  temp_table->temp_table.vlines = cols;
  add_event_listener((event_listener*)&arrow_key_event_listener, KEY_ARROW, temp_table);
  add_event_listener((event_listener*)&number_key_event_listener, KEY_NUMBER, temp_table);
  add_event_listener((event_listener*)&esc_key_event_listener, KEY_ESC, temp_table);
  add_event_listener((event_listener*)&enter_key_event_listener, KEY_ENTER, temp_table);

  add_last<setpoint>(setpoint_list, (setpoint){"00:00", 0});
  // add_last<setpoint>(setpoint_list, (setpoint){"00:20", 150});
  // add_last<setpoint>(setpoint_list, (setpoint){"00:80", 1200});
  // add_last<setpoint>(setpoint_list, (setpoint){"01:10", 800});
  // add_last<setpoint>(setpoint_list, (setpoint){"04:20", 200});
  // add_last<setpoint>(setpoint_list, (setpoint){"69:69", 0});

  // add_setpoint(setpoint_list, 0, 0);
  add_setpoint(setpoint_list, 20, 150);
  add_setpoint(setpoint_list, 70, 800);
  add_setpoint(setpoint_list, 260, 200);
  add_setpoint(setpoint_list, 80, 1200);
  add_setpoint(setpoint_list, 80, 1000);
  add_setpoint(setpoint_list, 4140, 0);
  
  draw_interface();
  
  draw_temp_table(&temp_table->temp_table);

  fill_table(temp_table);
  highlight_field(&temp_table->temp_table, 0, 1);
}

void validate_time_str(char *time_str) {
  while(*time_str != '\0') {
    if(*time_str == ' ') *time_str = '0';
    time_str++;
  }
}

void arrow_key_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t columns = list_size<setpoint>(temp_table->setpoint_list);
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  display.Set_Draw_color(0, 0, 0);
  //draw_temp_table(temp_table);
  //erase_field(&temp_table->temp_table, col, row + 1);
  if(!col && row) {
    setpoint *edit_setpoint = &get_index(temp_table->setpoint_list, row)->data;
    uint16_t prev_time = time_str_to_min(get_index(temp_table->setpoint_list, row - 1)->data.time);
    uint16_t curr_time = time_str_to_min(edit_setpoint->time);
    uint16_t next_time = time_str_to_min(get_index(temp_table->setpoint_list, row + 1)->data.time);
    
    if(curr_time < prev_time || curr_time > next_time) {
      char time_str[6] = "";
      strcpy(time_str, edit_setpoint->time);
      validate_time_str(time_str);
      uint16_t temp = edit_setpoint->temp;
      
      remove_setpoint(temp_table->setpoint_list, row);
      uint8_t new_row = add_setpoint(temp_table->setpoint_list, time_str_to_min(time_str), temp, row);
      Serial.println(row);
      Serial.println(new_row);
      Serial.println(min(row, new_row));
      Serial.println(max(row, new_row));

      for(uint8_t i = min(row, new_row); i <= max(row, new_row); i++) {
        fill_field(temp_table, 0, i);
        fill_field(temp_table, 1, i);
      }

      temp_table->selected_field = new_row * 2;
    }
  }  
  fill_field(temp_table, col, row);

  if(keys == KEY_LEFT && temp_table->selected_field > 0) { // left
    temp_table->selected_field -= 1;
  } else if(keys == KEY_RIGHT && temp_table->selected_field < columns*2 - 1) { // right
    temp_table->selected_field += 1;
  } else if(keys == KEY_DOWN && temp_table->selected_field < columns*2 - 2) { // down
    temp_table->selected_field += 2;
  } else if(keys == KEY_UP && temp_table->selected_field > 1) { // up
    temp_table->selected_field -= 2;
  }

  display.Set_Draw_color(0, 50, 255);
  col = temp_table->selected_field % 2;
  row = temp_table->selected_field / 2;
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void number_key_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(col) {
    uint16_t *temp = &get_index(temp_table->setpoint_list, row)->data.temp;
    uint16_t new_temp = *temp * 10 + char_to_int(map_key(keys));
    if(new_temp <= MAX_TEMP) *temp = new_temp;
  } else {
    char *time_char = get_index(temp_table->setpoint_list, row)->data.time;
    uint8_t i = 0;
    while(*time_char != ' ' && *time_char != '\0') {
      time_char++;
      i++;
    }
    if(*time_char) *time_char = map_key(keys);
    if(char_to_int(*time_char) > 5 && i == 3) *time_char = ' ';
  }
  fill_field(temp_table, col, row);
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void esc_key_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(!col && !row) return;
  if(col) {
    get_index(temp_table->setpoint_list, row)->data.temp /= 10;
  } else {
    char *time_char = get_index(temp_table->setpoint_list, row)->data.time;
    uint8_t i = 0;
    while(*time_char != ' ' && *time_char != '\0') {
      time_char++;
      i++;
    }
    if(i == 3) *(time_char - 2) = ' ';
    else if(i) *(time_char - 1) = ' ';
  }
  fill_field(temp_table, col, row);
  highlight_field(&temp_table->temp_table, col, row + 1);
}

void enter_key_event_listener(uint16_t keys, struct temp_table *temp_table) {
  uint8_t col = temp_table->selected_field % 2;
  uint8_t row = temp_table->selected_field / 2;
  if(!col && !row) return;
  if(col) {
    get_index(temp_table->setpoint_list, row)->data.temp = 0;
  } else {
    strcpy(get_index(temp_table->setpoint_list, row)->data.time, "  :  ");
  }
  fill_field(temp_table, col, row);
  highlight_field(&temp_table->temp_table, col, row + 1);
}