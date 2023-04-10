#pragma once

#define GND_CH          1
#define REF_CH          0
#define THERMISTOR_CH   3
#define THERMOCOUPLE_CH 2
#define THREE_WIRE_CH   4

void select_mux_channel(uint8_t channel, uint16_t wait_ms);
void select_mux_channel(uint8_t channel);