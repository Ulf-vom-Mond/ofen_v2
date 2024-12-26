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
  // Serial.println(get_voltage(THERMISTOR_CH), 5);
  Serial.print("Thermistor: ");
  Serial.print(get_thermistor_temp());
  Serial.println(" C");
  Serial.print("Thermocouple: ");
  Serial.print(get_thermocouple_temp());
  Serial.println(" C");
  Serial.print("Temperature: ");
  Serial.print(get_temp());
  Serial.println(" C");
  // select_mux_channel(REF_CH);
  // Serial.println(convert());
  // select_mux_channel(GND_CH);
  // Serial.println(convert());
  // select_mux_channel(THERMISTOR_CH);
  // Serial.println(convert());
  //delay(5000);
  // Serial.println();
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
  // add_timer_event_listener(tmr_evnt_lstnr5, 5, 0);

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
  // Serial.println(get_voltage(0));
  // Serial.println(get_voltage(0), 5);
  // Serial.println("GND_CH");
  // select_mux_channel_delay(GND_CH, 0);
  // for(uint8_t i = 0; i<30; i++) {
  //   Serial.println(convert());
  //   //delay(13);
  // }
  // Serial.println("REF_CH");
  // select_mux_channel_delay(REF_CH, 0);
  // for(uint8_t i = 0; i<30; i++) {
  //   Serial.println(convert());
  //   //delay(13);
  // }
  // delay(10);
  // select_mux_channel(REF_CH);
  // delay(5000);
  // select_mux_channel(THERMISTOR_CH);
  // delay(5000);
}