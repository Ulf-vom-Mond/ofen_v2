#include "user_input.h"
#include "shift_reg.h"
#include "keypad.h"
#include "avrio.h"
#include "pins.h"
#include <stdlib.h>
#include <Arduino.h>

struct list<struct event_listener_conf> *event_listener_list = new_list<struct event_listener_conf>(); // this list contains all the callback functions which shall be called on a keystroke

uint16_t multi_click_keys = 0; // saves which keys have been pressed long enough to activate multi click
uint16_t key_times[16] = {0}; // saves for how long a key has been pressed. also used for multi click
unsigned long last_update = 0; // saves the millis() value when read_keys() was executed the last time

uint8_t get_rotary_switch_state() {
  uint8_t shift_reg = read_shift_reg();
  return (shift_reg >> 4) & 0b00000111;
}

// register a new event listener to be called when the event specified by event_types (CLICK, LONG_PRESS, MULTI_CLICK) happen to the keys specified by mask
void add_event_listener(event_listener *new_event_listener, uint16_t mask, uint8_t event_types, void *params) {
  add_last<struct event_listener_conf>(event_listener_list, {new_event_listener, mask, event_types, params});
}

void emit_key_events() { // reads the keypad state, determines the active key event for every key and notifies all the registered event_listeners
  uint16_t keys = read_keypad();

  unsigned long current_ms = millis(); // update timer
  unsigned long passed_ms = current_ms - last_update;
  last_update = current_ms;

  uint16_t clicked_keys = 0; // keys that reveived a click shorter than LONG_PRESS_MS
  uint16_t long_pressed_keys = 0; // keys that received a click longer than LONG_PRESS_MS

  for(uint8_t i; i<16; i++) { // check following conditions for every key
    uint16_t key_bit_pattern = 0b1 << i; // bit position of the current key in keys
    if(keys & key_bit_pattern) { // key is pressed
      key_times[i] += passed_ms; // update for how long the key has been pressed
    } else { // key is not pressed
      if(key_times[i] < LONG_PRESS_MS && key_times[i] > 0) { // check if key has been pressed short enough to be a clicked key
        clicked_keys |= key_bit_pattern; // add key to clicked_keys
      }
      key_times[i] = 0; // reset key timer
      multi_click_keys &= ~key_bit_pattern; // reset multi click flag
    }
    if(key_times[i] >= LONG_PRESS_MS && !(multi_click_keys & key_bit_pattern)) { // check if key has been pressed long enough to trigger a long press event but is not in multi click mode
      long_pressed_keys |= key_bit_pattern; // add to long_pressed_keys
      multi_click_keys |= key_bit_pattern; // set multi click flag
    }
    if(key_times[i] >= LONG_PRESS_MS + MULTI_CLICK_MS) { // check if key has been pressed long enough to trigger a multi click event
      key_times[i] -= MULTI_CLICK_MS; // reduce key timer value to trigger the next multi click event in MULTI_CLICK_MS
      clicked_keys |= key_bit_pattern; // add to clicked_keys
    }
  }

  if(!long_pressed_keys && !clicked_keys) return; // return if no key events were triggered

  struct list<struct event_listener_conf> *event_listener_iter = event_listener_list;
  while(event_listener_iter->next){ // iterate over all registered key event listeners
    struct event_listener_conf listener = event_listener_iter->data; // get data of current key event listener

    // bit pattern which holds all the keys that received an event which is subscribed to by the current event listener
    uint16_t registered_keys = listener.mask & ( // only listen to keys that where specified by the mask parameter on the call to add_event_listener()
      (listener.event_types & CLICK ? clicked_keys & ~multi_click_keys : 0) | // all the keys that are clicked but not multi clicked if the event listener subscribed to CLICK
      (listener.event_types & LONG_PRESS ? long_pressed_keys : 0) | // all the keys that are long pressed if the event listener subscribed to LONG_PRESS
      ((listener.event_types & MULTI_CLICK) ? clicked_keys & multi_click_keys : 0)); // all the keys that are multi clicked if tha event listener subscribed to MULTI_CLICK

    if(registered_keys) {
      (**((event_listener*)listener.callback))(registered_keys, listener.params); // call the event listener function with the parameters specified on the call to add_event_listener()
    }
    event_listener_iter = event_listener_iter->next;
  }
}