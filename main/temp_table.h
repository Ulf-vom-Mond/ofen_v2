#include "graph.h"
#pragma once

#include <stdint.h>
#include "list.h"
#include "table.h"

struct setpoint;

struct temp_table {
  table temp_table;
  uint8_t selected_field;
  char *pre_edit;
  struct list<setpoint> *setpoint_list;
  graph *temp_graph;
};

void set_temp(struct temp_table *temp_table, uint8_t row, uint16_t new_temp);
uint16_t get_temp(struct temp_table *temp_table, uint8_t row);
void set_time(struct temp_table *temp_table, uint8_t row, char *new_time);
char *get_time(struct temp_table *temp_table, uint8_t row);
void fill_field(struct temp_table *temp_table, uint8_t column, uint8_t row);
void fill_row(struct temp_table *temp_table, uint8_t row);
void clear_row(struct temp_table *temp_table, uint8_t row);
void fill_table_section(struct temp_table *temp_table, uint8_t start_row, uint8_t end_row);
void fill_table(struct temp_table *temp_table);
uint8_t is_valid_time_str(char *time_str);
uint8_t validate_time_str(char *time_str, char *replacement);