#pragma once

#include "graph.h"
#include "list.h"

struct setpoint;

void draw_temp_curve(graph *temperature_graph, struct list<setpoint> *setpoint_list);
void update_temp_curve(struct list<setpoint> *old_setpoint_list, struct list<setpoint> *new_setpoint_list);
void edit_point(graph *temperature_graph, struct list<setpoint> *setpoint_list, setpoint old_point, uint8_t old_id);
void remove_point(graph *temperature_graph, struct list<setpoint> *setpoint_list, uint8_t remove_id);
void add_point(graph *temperature_graph, struct list<setpoint> *setpoint_list, uint8_t add_id);