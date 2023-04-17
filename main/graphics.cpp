#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <SPI.h>
#include "graphics.h"

LCDWIKI_SPI display(MODEL,CS,CD,RST,LED);

uint16_t toprow_width;
uint16_t toprow_height;
uint16_t graph_width;
uint16_t graph_height;
uint16_t table_width;
uint16_t table_height;
position toprow_position;
position graph_position;
position table_position;

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

  display.Set_Draw_color(BLACK);
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
  display.Set_Text_colour(BLACK);
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

rgb_color temperature_to_color (uint16_t temp) { // gives a green-yellow-red gradient based on the given temperature
  uint8_t r = 255;
  uint8_t g = 255;
  uint8_t b = 30;

  if(temp * 2 > TEMP_MAX){
    g = 255 * (TEMP_MAX - (uint32_t)temp) / (uint32_t)temp;
  } else {
    r = 255 * (uint32_t)temp / (TEMP_MAX - (uint32_t)temp);
  }
  // if(temp * 2 > TEMP_MAX) {
  //   b = ((uint32_t)temp - TEMP_MAX / 2) * 40 / TEMP_MAX;
  // } else {
  //   b = (TEMP_MAX / 2 - (uint32_t)temp) * 40 / TEMP_MAX;
  // }
  return {r, g, b};
}

void draw_graph(graph graph, uint8_t axis_to_margin_width){
  display.Set_Draw_color(BLACK);
  display.Draw_Fast_HLine(graph.pos.x, graph.pos.y + graph.height - axis_to_margin_width, graph.width);
  display.Draw_Fast_VLine(graph.pos.x + axis_to_margin_width, graph.pos.y, graph.height);
}

void draw_interface()
{
  display.Init_LCD();
  display.Set_Rotation(1);
  //display.Fill_Screen(WHITE);

  toprow_width  = display.Get_Display_Width()-8;
  toprow_height = TOPROW_HEIGHT-6;
  
  graph_width   = display.Get_Display_Width()-TABLE_WIDTH-6;
  graph_height  = display.Get_Display_Height()-TOPROW_HEIGHT-6;

  table_width   = TABLE_WIDTH-6;
  table_height  = display.Get_Display_Height()-TOPROW_HEIGHT-6;

  toprow_position = {4, 4};
  graph_position  = {4, toprow_height+8};
  table_position  = {graph_width+8, toprow_height+8};

  graph temperature_graph = {graph_position, graph_width, graph_height, 0, 12, 0, 1300};
  draw_graph(temperature_graph, 10);


  float cols[] = {-1, 0.55};
  float rows[] = {-1, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252};

  table temperature_table = {table_position, table_width, table_height, 13, 2, rows, cols};
  draw_table(temperature_table);
  write_text_to_field(temperature_table, 0, 0, "Time");
  write_text_to_field(temperature_table, 1, 0, "Temp");
  for (uint16_t i = 1; i<temperature_table.rows - 1; i++) {
      char time_str[6]="";
      sprintf(time_str, "12:%d", i);
      char temp_str[6]="";
      sprintf(temp_str, "%d", 118*i);      
      write_text_to_field(temperature_table, 0, i, time_str);  
      change_field_color(temperature_table, 1, i, temperature_to_color(118*i));
      write_text_to_field(temperature_table, 1, i, temp_str);
  }

  Serial.println(millis());
  
}

