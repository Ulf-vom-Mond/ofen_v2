#pragma once

#include "graph.h"
#include "list.h"

struct setpoint;

void draw_temp_curve(graph temperature_graph, struct list<setpoint> *setpoint_list);
void update_temp_curve(struct list<setpoint> *old_setpoint_list, struct list<setpoint> *new_setpoint_list);