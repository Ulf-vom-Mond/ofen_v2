#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include "display.h"
#include "graph.h"
#include "table.h"
#include "graphics.h"


void init_display() {
  display.Init_LCD();
  display.Set_Rotation(1);
  display.Set_Draw_color(BLACK);
  // display.Fill_Screen(WHITE);
  display.Set_Text_colour(BLACK);
  display.Set_Text_Back_colour(WHITE);
  display.Set_Text_Mode(1);
}

void draw_temp_table(table *temp_table) {  
  draw_table(temp_table);
  write_string_to_field(temp_table, 0, 0, "Time");
  write_string_to_field(temp_table, 1, 0, "Temp");
}

void draw_interface() {
  init_display();
  //draw_temp_graph();
}