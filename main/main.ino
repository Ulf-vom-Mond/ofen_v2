#include <SPI.h>
#include "temperature.h"
#include "user_input.h"
#include "user_interface.h"
#include "pins.h"
#include "avrio.h"
#include "adc.h"
#include "list.h"
#include "keypad.h"
#include "timer.h"
#include "mux.h"

// void key_event_listener(uint16_t keys) {
//   Serial.println((keys), BIN);
// }

void tmr_evnt_lstnr1(void *params) {
  // Serial.println(get_thermistor_temp());
  // Serial.println(get_voltage(0), 5);
  select_mux_channel(0);
  Serial.println(convert());
  select_mux_channel(1);
  Serial.println(convert());
  select_mux_channel(3);
  Serial.println(convert());
  //delay(5000);
  Serial.println();
  //get_thermistor_temp();
}

void tmr_evnt_lstnr5(void *params) {
  Serial.println("5sec");
}

void setup() {
  pin_config();

  Serial.begin(115200);
  Serial.println("Hello!");
  write(ADC_CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  adc_config();
  timer_config();
  sei();

  add_timer_event_listener(tmr_evnt_lstnr1, 5, 0);
  //add_timer_event_listener(tmr_evnt_lstnr5, 5, 0);

  // add_event_listener((void* (*)(uint16_t))&key_event_listener);

  // TCNT1 = 206;
  // TCCR1A =0x00;
	// TCCR1B |= (1<<CS01);		
	// TIMSK1 |= (1<<TOIE1);		// enable timer overflow interrupt
	// sei();

  launch_ui();
}

void loop() {
  emit_key_events();
  emit_timer_events();
  //Serial.println(get_voltage(0));
  // Serial.println(get_voltage(0), 5);
  delay(10);
}