#pragma once

#include <stdint.h>
#include "table.h"
#include "list.h"

typedef struct {
  char time[6];
  uint16_t temp;
} setpoint;

struct temp_table {
  table temp_table;
  uint8_t selected_field;
  struct list<setpoint> *setpoint_list;
};

void launch_ui();