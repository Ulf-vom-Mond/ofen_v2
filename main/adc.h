#pragma once

#define DEVICE_ADDRESS 01

#define U_REG 1.225 // voltage output of regulator
#define U_REF 0.05212766 // reference voltage of ADC

void adc_config();
void print_binary(char number, char newline);
void print_binary(char number);
void read_all(char device_address);
int16_t convert();
float get_voltage(uint8_t channel);