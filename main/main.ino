#include <SPI.h>
#include "temperature.h"
#include "keypad.h"
#include "display.h"
#include "pins.h"
#include "avrio.h"
#include "adc.h"
#include "list.h"
#include "keypad.h"

void key_event_listener(uint16_t keys) {
  Serial.println(map_key(keys));
}

void setup() {
  pin_config();

  Serial.begin(115200);
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));

  adc_config();

  Serial.println("Hello!");

  add_event_listener((void* (*)(uint16_t))&key_event_listener);
}

void loop() {
  read_keypad();
  delay(10);
}