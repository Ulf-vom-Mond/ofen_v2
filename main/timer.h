#pragma once

#include <stdint.h>
#include "list.h"

typedef void *timer_event_listener(void *params); // type definition for the callback function

struct timer_event_listener_conf {
  timer_event_listener *callback;
  uint8_t counter;
  uint8_t match;
  void *params;
};

extern struct list<struct timer_event_listener_conf> *timer_event_listener_list;

void add_timer_event_listener(timer_event_listener *new_event_listener, uint8_t match, void *params);
void timer_config();
void emit_timer_events();