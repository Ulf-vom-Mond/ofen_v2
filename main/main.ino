#include <SPI.h>
#include "temperature.h"
#include "keypad.h"
#include "user_interface.h"
#include "pins.h"
#include "avrio.h"
#include "adc.h"
#include "list.h"
#include "keypad.h"


// void key_event_listener(uint16_t keys) {
//   Serial.println((keys), BIN);
// }

void setup() {
  pin_config();

  Serial.begin(115200);
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  adc_config();

  Serial.println("Hello!");

  //add_event_listener((void* (*)(uint16_t))&key_event_listener);

  // TCNT1 = 206;
  // TCCR1A =0x00;
	// TCCR1B |= (1<<CS01);		
	// TIMSK1 |= (1<<TOIE1);		// enable timer overflow interrupt
	// sei();

  launch_ui();
}




void loop() {
  read_keypad();
  delay(10);
}