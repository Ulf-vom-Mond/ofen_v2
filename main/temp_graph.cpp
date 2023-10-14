//#include <cstdint>
#include "list.h"
#include "graph.h"
#include "temp_graph.h"
#include "graphics.h"
#include "setpoint.h"

void draw_temp_graph(graph *temperature_graph, struct list<setpoint> *setpoint_list) {
  
}

void draw_temp_curve(graph *temperature_graph, struct list<setpoint> *setpoint_list) {
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

void edit_point(graph *temperature_graph, struct list<setpoint> *setpoint_list, setpoint old_point, uint8_t old_id) {
  uint8_t first = !old_id;
  list<setpoint> *prev_setpoint;
  list<setpoint> *new_setpoint;
  list<setpoint> *next_setpoint;
  if(!first) {
    prev_setpoint = get_index(setpoint_list, old_id - 1);
    new_setpoint = prev_setpoint->next;
  } else {
    new_setpoint = get_index(setpoint_list, old_id);
  }
  if(new_setpoint->data.temp == old_point.temp && !strcmp(new_setpoint->data.time, old_point.time)) return;
  next_setpoint = new_setpoint->next;
  uint8_t last = !next_setpoint->next;

  display.Set_Draw_color(BACKGROUND_COLOR);
  if(!first) add_line(temperature_graph, time_str_to_min(prev_setpoint->data.time), prev_setpoint->data.temp, time_str_to_min(old_point.time), old_point.temp);
  if(!last) add_line(temperature_graph, time_str_to_min(old_point.time), old_point.temp, time_str_to_min(next_setpoint->data.time), next_setpoint->data.temp);

  display.Set_Draw_color(0, 0, 0);
  if(!first) add_line(temperature_graph, time_str_to_min(prev_setpoint->data.time), prev_setpoint->data.temp, time_str_to_min(new_setpoint->data.time), new_setpoint->data.temp);
  if(!last) add_line(temperature_graph, time_str_to_min(new_setpoint->data.time), new_setpoint->data.temp, time_str_to_min(next_setpoint->data.time), next_setpoint->data.temp);

}

void remove_point(graph *temperature_graph, struct list<setpoint> *setpoint_list, uint8_t remove_id) {
  list<setpoint> *prev_setpoint = get_index(setpoint_list, remove_id - 1);
  list<setpoint> *old_setpoint = prev_setpoint->next;
  list<setpoint> *next_setpoint = old_setpoint->next;
  uint8_t last = !next_setpoint->next;

  display.Set_Draw_color(BACKGROUND_COLOR);
  add_line(temperature_graph, time_str_to_min(prev_setpoint->data.time), prev_setpoint->data.temp, time_str_to_min(old_setpoint->data.time), old_setpoint->data.temp);
  if(!last) add_line(temperature_graph, time_str_to_min(old_setpoint->data.time), old_setpoint->data.temp, time_str_to_min(next_setpoint->data.time), next_setpoint->data.temp);

  display.Set_Draw_color(0, 0, 0);
  if(!last) add_line(temperature_graph, time_str_to_min(prev_setpoint->data.time), prev_setpoint->data.temp, time_str_to_min(next_setpoint->data.time), next_setpoint->data.temp);
}

void add_point(graph *temperature_graph, struct list<setpoint> *setpoint_list, uint8_t add_id) {
  list<setpoint> *prev_setpoint = get_index(setpoint_list, add_id - 1);
  list<setpoint> *new_setpoint = prev_setpoint->next;
  list<setpoint> *next_setpoint = new_setpoint->next;
  uint8_t last = !next_setpoint->next;

  display.Set_Draw_color(BACKGROUND_COLOR);
  if(!last) add_line(temperature_graph, time_str_to_min(prev_setpoint->data.time), prev_setpoint->data.temp, time_str_to_min(next_setpoint->data.time), next_setpoint->data.temp);

  display.Set_Draw_color(0, 0, 0);
  add_line(temperature_graph, time_str_to_min(prev_setpoint->data.time), prev_setpoint->data.temp, time_str_to_min(new_setpoint->data.time), new_setpoint->data.temp);
  if(!last) add_line(temperature_graph, time_str_to_min(new_setpoint->data.time), new_setpoint->data.temp, time_str_to_min(next_setpoint->data.time), next_setpoint->data.temp);
}