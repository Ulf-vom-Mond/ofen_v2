#pragma once

#include <stdint.h>
#include "list.h"

typedef void *event_listener(uint16_t keys, void *params); // type definition for the callback function. Callback functions get passed a list which contains all pressed keys

struct event_listener_conf {
  event_listener *callback;
  uint16_t mask;
  uint8_t event_types;
  void *params;
};

extern struct list<struct event_listener_conf> *event_listener_list;

uint8_t get_rotary_switch_state();
void add_event_listener(event_listener *new_event_listener, uint16_t mask, uint8_t event_types, void *params);
void emit_key_events();