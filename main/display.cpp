#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include "display.h"
#include "graph.h"
#include "table.h"
#include "graphics.h"


void draw_interface() {
  

  display.Init_LCD();
  display.Set_Rotation(1);
  display.Set_Draw_color(BLACK);
  // display.Fill_Screen(WHITE);
  display.Set_Text_colour(BLACK);
  display.Set_Text_Back_colour(WHITE);
  display.Set_Text_Mode(1);

  graph temperature_graph = {{GRAPH_X, GRAPH_Y}, GRAPH_WIDTH, GRAPH_HEIGHT, 0, 12, 0, 1300, 10};
  draw_graph(temperature_graph);
  
  add_y_mark(temperature_graph, 0);
  add_y_mark(temperature_graph, 1300);
  add_y_mark(temperature_graph, 800);
  add_y_mark(temperature_graph, 200);

  add_x_mark(temperature_graph, 0);
  add_x_mark(temperature_graph, 1);
  add_x_mark(temperature_graph, 5);
  add_x_mark(temperature_graph, 10);

  add_point(temperature_graph, 1, 200, 'x');
  add_point(temperature_graph, 0, 0, 'x');
  add_point(temperature_graph, 5, 900, '+');
  add_point(temperature_graph, 2, 500, '.');
  add_point(temperature_graph, 9, 550, 'o');
  add_point(temperature_graph, 12, 100, 'O');

  add_line(temperature_graph, 1, 200, 9, 550);
  add_line(temperature_graph, 5, 900, 12, 100);

  draw_pattern_line(20, 20, 100, 100, DOTS);
  draw_pattern_line(20, 30, 100, 110, DASHS);
  draw_pattern_line(20, 40, 100, 120, DOTDASH);
  draw_pattern_line(20, 50, 100, 130, SHORTDASH);

  float cols[] = {-1, 0.55};
  float rows[] = {-1, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252};

  table temperature_table = {{TABLE_X, TABLE_Y}, TABLE_WIDTH, TABLE_HEIGHT, 13, 2, rows, cols};
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

  
}

