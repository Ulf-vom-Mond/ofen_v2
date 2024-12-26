#include "mux.h"
#include "adc.h"
#include "pins.h"
#include "avrio.h"
#include <Arduino.h>

void select_mux_channel_delay(uint8_t channel, uint16_t wait_ms) {
  if(channel > 7) { // CD4097 only has 8 Channels
    return;
  }

  write(MUX_A_PIN, (channel >> 0) & 0x01); // bitwise AND 0b00000001 to get 1st bit
  write(MUX_B_PIN, (channel >> 1) & 0x01); // bitwise AND 0b00000010 and right shift 1 to get 2nd bit
  write(MUX_C_PIN, (channel >> 2) & 0x01); // bitwise AND 0b00000100 and right shift 2 to get 3rd bit
  
  // convert();      // a dummy convert and a delay helps in minimizing cross-channel influence
  delay(wait_ms);
}

void select_mux_channel(uint8_t channel) {
  select_mux_channel_delay(channel, 0);
}