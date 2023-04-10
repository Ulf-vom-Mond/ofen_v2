#include <SPI.h>
#include "temperature.h"
#include "user_input.h"
#include "display.h"
#include "pins.h"
#include "adc.h"

void setup() {
  pin_config();

  Serial.begin(115200);
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));

  adc_config();
}

void loop() {

}