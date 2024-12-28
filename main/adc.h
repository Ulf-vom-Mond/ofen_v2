#pragma once

#include <stdint.h>

#define DEVICE_ADDRESS 01

#define U_REG 1.225 // voltage output of regulator
#define U_REF 0.05212766 // reference voltage of ADC

void adc_config();
void print_line_binary(char number);
void print_binary(char number);
void read_all(char device_address);
int16_t convert();
void convert_async(void callback(uint16_t));
float get_voltage(uint8_t channel);
float adc_to_voltage(int16_t adc, int16_t zero, int16_t ref);