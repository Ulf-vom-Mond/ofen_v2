#include "temp_graph.h"
#include "graphics.h"
#include "setpoint.h"

void draw_temp_graph(graph temperature_graph, struct list<setpoint> *setpoint_list) {
  
}

void draw_temp_curve(graph temperature_graph, struct list<setpoint> *setpoint_list) {
  struct list<setpoint> *setpoint_iter = setpoint_list;
  while(setpoint_iter->next->next) {
    add_line(temperature_graph, time_str_to_min(setpoint_iter->data.time), setpoint_iter->data.temp, time_str_to_min(setpoint_iter->next->data.time), setpoint_iter->next->data.temp);
    //add_x_mark(temperature_graph, time_str_to_min(setpoint_iter->data.time));
    //add_y_mark(temperature_graph, setpoint_iter->data.temp);
    setpoint_iter = setpoint_iter->next;
  }
}

void update_temp_curve(struct list<setpoint> *old_setpoint_list, struct list<setpoint> *new_setpoint_list) {
  display.Set_Draw_color(BACKGROUND_COLOR);
  //draw_temp
}