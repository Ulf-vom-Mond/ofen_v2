#include "mux.h"
#include "adc.h"
#include "pins.h"
#include "avrio.h"
#include <Arduino.h>

void select_mux_channel_delay(uint8_t channel, uint16_t wait_ms) {
  // Serial.println(channel);
  if(channel > 7) { // CD4097 only has 8 Channels
    return;
  }

  write(MUX_A_PIN, (channel >> 0) & 0x01); // bitwise AND 0b00000001 to get 1st bit
  write(MUX_B_PIN, (channel >> 1) & 0x01); // bitwise AND 0b00000010 and right shift 1 to get 2nd bit
  write(MUX_C_PIN, (channel >> 2) & 0x01); // bitwise AND 0b00000100 and right shift 2 to get 3rd bit
  // write(MUX_C_PIN, 0);
  // convert();      // a dummy convert and a delay helps in minimizing cross-channel influence
  delay(wait_ms);
}

void select_mux_channel(uint8_t channel) {
  select_mux_channel_delay(channel, 0);
}

uint8_t get_mux_channel() {
  uint8_t channel = 0;
  channel |= (read(MUX_A_PIN) & 0x01) << 0;
  channel |= (read(MUX_B_PIN) & 0x01) << 1;
  channel |= (read(MUX_C_PIN) & 0x01) << 2;
  // Serial.print(read(MUX_C_PIN), BIN);
  // Serial.print(read(MUX_B_PIN), BIN);
  // Serial.println(read(MUX_A_PIN), BIN);
  return channel;
}