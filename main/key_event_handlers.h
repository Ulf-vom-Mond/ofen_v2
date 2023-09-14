#pragma once

#include <stdint.h>
#include "list.h"
#include "table.h"
#include "temp_table.h"

void arrow_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void number_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void esc_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void enter_click_event_listener(uint16_t keys, struct temp_table *temp_table);
void delete_row_event_listener(uint16_t keys, struct temp_table *temp_table);
void add_row_event_listener(uint16_t keys, struct temp_table *temp_table);