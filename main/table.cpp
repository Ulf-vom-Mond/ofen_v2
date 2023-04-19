#include "table.h"

void draw_table(table table) {  
  for(uint16_t i = 0; i < table.columns; i++) {
    if((table.vlines)[i] < 0) {
      (table.vlines)[i] = 0;
      continue;      
    }

    if((table.vlines)[i] <= 1) {
      (table.vlines)[i] = (table.vlines)[i] * table.width;
    }

    display.Draw_Fast_VLine(table.pos.x + (table.vlines)[i], table.pos.y, table.height);
  }

  for(uint16_t i = 0; i < table.rows; i++) {
    if((table.hlines)[i] < 0) {
      (table.hlines)[i] = 0;
      continue;
    }

    if((table.hlines)[i] <= 1) {
      (table.hlines)[i] = (table.hlines)[i] * table.height;
    }

    display.Draw_Fast_HLine(table.pos.x, table.pos.y + (table.hlines)[i], table.width);
  }
}

position get_table_entry_position(table table, uint16_t column, uint16_t row) {
  uint16_t x = column < table.columns ? table.pos.x + table.vlines[column] : table.pos.x + table.width;
  uint16_t y = row < table.rows ? table.pos.y + table.hlines[row] : table.pos.y + table.height;
    
  return {x,y};
}

void write_text_to_field(table table, uint16_t column, uint16_t row, char *text) {
  display.Set_Text_Mode(1);
  display.Set_Text_Size((table.hlines[row + 1] - table.hlines[row]) / 8);
  position entry_position = get_table_entry_position(table, column, row);
  display.Print_String(text, entry_position.x+4, entry_position.y+4);
}

void change_field_color(table table, uint16_t column, uint16_t row, rgb_color color) {
  position entry_position = get_table_entry_position(table, column, row);
  position next_entry_position = get_table_entry_position(table, column + 1, row + 1);
  
  uint16_t rect_width = next_entry_position.x - entry_position.x - 1;
  uint16_t rect_height = next_entry_position.y - entry_position.y - 1;

  display.Fill_Rect(entry_position.x+1, entry_position.y+1, rect_width, rect_height, display.Color_To_565(color.r, color.g, color.b));
}