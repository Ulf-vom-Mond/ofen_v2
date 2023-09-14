#pragma once

#include <stdint.h>
#include "list.h"
#include "table.h"

typedef struct {
  char time[6];
  uint16_t temp;
} setpoint;

struct temp_table {
  table temp_table;
  uint8_t selected_field;
  struct list<setpoint> *setpoint_list;
};

uint16_t time_str_to_min(char time_str[6]);
void min_to_time_str(uint16_t minutes, char *dest);
uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp, uint8_t offset);
uint8_t add_setpoint(struct list<setpoint> *setpoint_list, uint16_t time, uint16_t temp);
list<setpoint>* remove_setpoint(struct list<setpoint> *setpoint_list, uint8_t index);
void fill_field(struct temp_table *temp_table, uint8_t column, uint8_t row);
void fill_row(struct temp_table *temp_table, uint8_t row);
void clear_row(struct temp_table *temp_table, uint8_t row);
void fill_table(struct temp_table *temp_table);
uint8_t validate_time_str(char *time_str, char *replacement);