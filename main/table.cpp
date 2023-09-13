#include "table.h"

table *create_table(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t rows, uint16_t cols) {
  table *new_table = (table *)malloc(sizeof(table));
  new_table->pos = {x, y};
  new_table->width = width;
  new_table->height = height;
  new_table->rows = rows;
  new_table->columns = cols;
  new_table->hlines = (float *)malloc(sizeof(float) * rows);
  new_table->vlines = (float *)malloc(sizeof(float) * cols);
  new_table->field_colors = (rgb_color *)malloc(sizeof(rgb_color) * rows * cols);
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

    display.Draw_Fast_VLine(table->pos.x + (table->vlines)[i], table->pos.y, table->height);
  }

  for(uint16_t i = 0; i < table->rows; i++) {
    if((table->hlines)[i] < 0) {
      //(table->hlines)[i] = 0;
      continue;
    }

    if((table->hlines)[i] <= 1) {
      (table->hlines)[i] = (table->hlines)[i] * table->height;
    }

    display.Draw_Fast_HLine(table->pos.x, table->pos.y + (table->hlines)[i], table->width);
  }
}

position get_table_entry_position(table *table, uint16_t column, uint16_t row) {
  uint16_t x = column < table->columns ? table->pos.x + table->vlines[column] : table->pos.x + table->width;
  uint16_t y = row < table->rows ? table->pos.y + table->hlines[row] : table->pos.y + table->height;
    
  return {x,y};
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