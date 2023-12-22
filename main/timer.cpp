#include "timer.h"
#include <Arduino.h>
#include <avr/interrupt.h>

struct list<struct timer_event_listener_conf> *timer_event_listener_list = new_list<struct timer_event_listener_conf>();

// register a new event listener to be called when the counter reaches the match value
void add_timer_event_listener(timer_event_listener *new_event_listener, uint8_t match, void *params) {
  add_last<struct timer_event_listener_conf>(timer_event_listener_list, {new_event_listener, 0, match, params});
}

void timer_config() {
  TCCR1A = 0;
  TCCR1B = 0b00001101;
  OCR1A = 7812;
  TIMSK1 |= 1 << OCIE1A;
}

void emit_timer_events() {
  struct list<struct timer_event_listener_conf> *event_listener_iter = timer_event_listener_list;
  while(event_listener_iter->next){ // iterate over all registered timer event listeners
    if(event_listener_iter->data.counter >= event_listener_iter->data.match) {
    	event_listener_iter->data.counter = 0;
    	event_listener_iter->data.callback(event_listener_iter->data.params);
    }
    event_listener_iter = event_listener_iter->next;
  }
}

ISR(TIMER1_COMPA_vect) {
  struct list<struct timer_event_listener_conf> *event_listener_iter = timer_event_listener_list;
  while(event_listener_iter->next){ // iterate over all registered timer event listeners
    event_listener_iter->data.counter += 1;
    event_listener_iter = event_listener_iter->next;
  }
}