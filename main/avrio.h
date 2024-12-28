#pragma once

#include <stdint.h>

char is_valid_pin(uint8_t pin);
uint8_t get_pin(char port, uint8_t pin_num);
volatile uint8_t* get_port_address(uint8_t pin);
volatile uint8_t* get_pin_address(uint8_t pin);
volatile uint8_t* get_ddr_address(uint8_t pin);
void set_ddr(uint8_t pin, char data_direction);
void write(uint8_t pin, uint8_t value);
void toggle(uint8_t pin);
void pulse(uint8_t pin);
char read(uint8_t pin);