#include "table.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

table *create_table(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t rows, uint16_t cols, uint8_t max_text_len) {
  table *new_table = (table *)malloc(sizeof(table));
  new_table->pos = {x, y};
  new_table->width = width;
  new_table->height = height;
  new_table->rows = rows;
  new_table->columns = cols;
  new_table->hlines = (float *)malloc(sizeof(float) * rows);
  new_table->vlines = (float *)malloc(sizeof(float) * cols);
  new_table->field_colors = (rgb_color *)malloc(sizeof(rgb_color) * rows * cols);
  uint16_t text_size = rows * cols * (max_text_len + 1);
  new_table->text = (char *)malloc(text_size);
  memset(new_table->text, 0, text_size);
  new_table->max_text_len = max_text_len;
  return new_table;
}

void delete_table(table *my_table) {
  free(my_table->vlines);
  free(my_table->hlines);
  free(my_table->field_colors);
  free(my_table);
}

void draw_table(table *table) {  
  for(uint16_t i = 0; i < table->columns; i++) {
    if((table->vlines)[i] < 0) {
      //(table->vlines)[i] = 0;
      continue;      
    }

    if((table->vlines)[i] <= 1) {
      (table->vlines)[i] = (table->vlines)[i] * table->width;
    }

    display.Draw_Fast_VLine(table->pos.x + (table->vlines)[i], table->pos.y, table->height + 1);
  }

  for(uint16_t i = 0; i < table->rows; i++) {
    if((table->hlines)[i] < 0) {
      //(table->hlines)[i] = 0;
      continue;
    }

    if((table->hlines)[i] <= 1) {
      (table->hlines)[i] = (table->hlines)[i] * table->height;
    }

    display.Draw_Fast_HLine(table->pos.x, table->pos.y + (table->hlines)[i], table->width + 1);
  }
}

position get_table_entry_position(table *table, uint16_t column, uint16_t row) {
  uint16_t x = column < table->columns ? table->pos.x + table->vlines[column] : table->pos.x + table->width + 1;
  uint16_t y = row < table->rows ? table->pos.y + table->hlines[row] : table->pos.y + table->height + 1;
    
  return {x,y};
}

void refresh_text(table *table, uint16_t column, uint16_t row, char *str) {
  //Serial.println("refresh text");
  char* old_str = (char*)&((char[column][row][table->max_text_len+1])(table->text))[column][row];
  char clear_str[table->max_text_len+1] = "";
  char new_str[table->max_text_len+1] = "";

  uint8_t str_len = strlen(str);
  uint8_t old_len = strlen(old_str);

  for(uint8_t i = 0; i <= table->max_text_len; i++) {
    if(i >= str_len && i >= old_len) {
      clear_str[i] = '\0';
      new_str[i] = '\0';
      break;
    }
    if(i >= str_len) {
      clear_str[i] = old_str[i];
      new_str[i] = '\0';
      continue;
    }
    if(i >= old_len) {
      clear_str[i] = '\0';
      new_str[i] = str[i];
      continue;
    }
    if(old_str[i] == str[i]) {
      clear_str[i] = ' ';
      new_str[i] = ' ';
    } else {
      clear_str[i] = old_str[i];
      new_str[i] = str[i];
    }
  }

  rgb_color color = table->field_colors[row * table->columns + column];
  display.Set_Text_colour(display.Color_To_565(color.r, color.g, color.b));
  write_string_to_field(table, column, row, clear_str);
  //Serial.println("clear");
  //Serial.println(&((char[column][row])(table->text))[column][row]);
  //text = "AB";
  // Serial.println(table->text);
  // Serial.println((uint8_t)*table->text, HEX);
  strcpy(old_str, str);

  //Serial.println("copy");
  display.Set_Text_colour(BLACK);
  write_string_to_field(table, column, row, new_str);
  //Serial.println("write");
}

void write_string_to_field(table *table, uint16_t column, uint16_t row, char *text) {
  display.Set_Text_Mode(1);
  display.Set_Text_Size((table->hlines[row + 1] - table->hlines[row]) / 8);
  position entry_position = get_table_entry_position(table, column, row);
  display.Print_String(text, entry_position.x+4, entry_position.y+4);
}

void change_field_color(table *table, uint16_t column, uint16_t row, rgb_color color) {
  table->field_colors[row * table->columns + column] = color;
  erase_field(table, column, row);
}

void erase_field(table *table, uint16_t column, uint16_t row) {
  position entry_position = get_table_entry_position(table, column, row);
  position next_entry_position = get_table_entry_position(table, column + 1, row + 1);
  
  uint16_t rect_width = next_entry_position.x - entry_position.x - 1;
  uint16_t rect_height = next_entry_position.y - entry_position.y - 1;

  rgb_color color = table->field_colors[row * table->columns + column];

  display.Fill_Rect(entry_position.x+1, entry_position.y+1, rect_width, rect_height, display.Color_To_565(color.r, color.g, color.b));
}

void highlight_field(table *table, uint16_t column, uint16_t row) {
  position entry_position = get_table_entry_position(table, column, row);
  position next_entry_position = get_table_entry_position(table, column + 1, row + 1);

  display.Draw_Rectangle(entry_position.x+1, entry_position.y+1, next_entry_position.x-1, next_entry_position.y-1);
}