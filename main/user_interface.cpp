#include <Arduino.h>
#include <LCDWIKI_GUI.h>
#include "user_interface.h"
#include "display.h"
#include "list.h"
#include "keypad.h"
#include "user_input.h"
#include "table.h"
#include "key_event_handlers.h"
#include "temp_graph.h"
#include "setpoint.h"


float cols[] = {-1, 0.55};
float rows[] = {-1, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252};

table *status_table;

void update_status(float temp) {
  char temp_str[8] = {0};
  sprintf(temp_str, "%4d C", (int16_t)(temp+0.5));
  // erase_field(status_table, 1, 0);
  refresh_text(status_table, 1, 0, temp_str);
}

void launch_ui() {
  struct list<setpoint> *setpoint_list = new_list<setpoint>();
  struct temp_table *temp_table = (struct temp_table *)malloc(sizeof(struct temp_table));
  table *temp_table_table = create_table(TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT, 13, 2, 5);
  graph *temperature_graph = (graph*)malloc(sizeof(graph));
  *temperature_graph = {{GRAPH_X, GRAPH_Y}, GRAPH_WIDTH, GRAPH_HEIGHT, 0, 400, 0, 1300, 10};
  *temp_table = (struct temp_table){.temp_table = *temp_table_table, .selected_field = 0, .pre_edit = "00:00", .setpoint_list = setpoint_list, .temp_graph = temperature_graph};
  temp_table->temp_table.hlines = rows;
  temp_table->temp_table.vlines = cols;
  
  add_event_listener((event_listener*)&arrow_click_event_listener, KEY_RIGHT|KEY_LEFT, CLICK, temp_table);
  add_event_listener((event_listener*)&arrow_click_event_listener, KEY_UP|KEY_DOWN, CLICK|MULTI_CLICK, temp_table);
  add_event_listener((event_listener*)&number_click_event_listener, KEY_NUMBER, CLICK, temp_table);
  add_event_listener((event_listener*)&esc_click_event_listener, KEY_ESC, CLICK, temp_table);
  add_event_listener((event_listener*)&enter_click_event_listener, KEY_ENTER, CLICK, temp_table);
  add_event_listener((event_listener*)&delete_row_event_listener, KEY_LEFT, LONG_PRESS, temp_table);
  add_event_listener((event_listener*)&add_row_event_listener, KEY_RIGHT, LONG_PRESS, temp_table);

  add_last<setpoint>(setpoint_list, (setpoint){"00:00", 0});
  add_setpoint(setpoint_list, 20, 150);
  add_setpoint(setpoint_list, 70, 800);
  add_setpoint(setpoint_list, 260, 200);
  add_setpoint(setpoint_list, 80, 1200);
  add_setpoint(setpoint_list, 80, 1000);
  add_setpoint(setpoint_list, 400, 0);
  
  draw_interface();
  
  draw_temp_table(&temp_table->temp_table);

  fill_table(temp_table);
  display.Set_Draw_color(0, 50, 255);
  highlight_field(&temp_table->temp_table, 0, 1);

  display.Set_Draw_color(0, 0, 0);
  
  draw_graph(temperature_graph);
  draw_temp_curve(temperature_graph, setpoint_list);


  float status_rows[] = {0, 1};
  float status_cols[] = {0, 0.3, 0.7, 1}; 
  status_table = create_table(TOP_ROW_X, TOP_ROW_Y, TOP_ROW_WIDTH, TOP_ROW_HEIGHT, 2, 4, 7);
  status_table->hlines = status_rows;
  status_table->vlines = status_cols;
  change_field_color(status_table, 1, 0, {200, 200, 200});
  draw_table(status_table);
  
  
}