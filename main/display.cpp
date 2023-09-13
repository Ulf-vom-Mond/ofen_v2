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

void draw_temp_graph() {
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
}



void draw_temp_table(table *temp_table) {  
  draw_table(temp_table);
  write_string_to_field(temp_table, 0, 0, "Time");
  write_string_to_field(temp_table, 1, 0, "Temp");
  // for (uint16_t i = 1; i<temp_table->rows - 1; i++) {
  //   char time_str[6]="";
  //   sprintf(time_str, "12:%d", i);
  //   char temp_str[6]="";
  //   sprintf(temp_str, "%d", 118*i);
  //   write_string_to_field(temp_table, 0, i, time_str);
  //   change_field_color(temp_table, 1, i, temperature_to_color(118*i));
  //   write_string_to_field(temp_table, 1, i, temp_str);
  // }
}


void draw_interface() {
  init_display();
  draw_temp_graph();
}

