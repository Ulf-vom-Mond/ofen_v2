#include "avrio.h"


// HELPER functions

char is_valid_pin(uint8_t pin) {
  return
    pin > 9 &&      // no port A on 328p
    pin < 38 &&     // PD7 is highest pin
    pin % 10 < 8 && // Pin numbers range from 0 to 7
    pin != 27;      // no PC7 on 328p
}

uint8_t get_pin(char port, uint8_t pin_num) { // convert port letter and pin number to unique integer port id
  uint8_t pin = 0;

  switch(port) {
    case 'b':
    case 'B':
      pin = 10;
      break;
    case 'c':
    case 'C':
      pin = 20;
      break;
    case 'd':
    case 'D':
      pin = 30;
      break;      
  }

  if(!is_valid_pin(pin)) {
    return 0;
  }

  return pin + pin_num;
}


// REGISTERS

volatile uint8_t* get_port_address(uint8_t pin) { // convert unique integer port id to port address
  uint8_t pin_mod = pin % 10;

  if(!is_valid_pin(pin)) {
    return 0;
  }

  switch((pin - pin_mod) / 10) {
    case 1:
      return &PORTB;
    case 2:
      return &PORTC;
    case 3:
      return &PORTD;
  }
}

volatile uint8_t* get_pin_address(uint8_t pin) { // convert unique integer port id to port address
  uint8_t pin_mod = pin % 10;

  if(!is_valid_pin(pin)) {
    return 0;
  }

  switch((pin - pin_mod) / 10) {
    case 1:
      return &PINB;
    case 2:
      return &PINC;
    case 3:
      return &PIND;
  }
}

volatile uint8_t* get_ddr_address(uint8_t pin) { // convert unique integer port id to port address
  uint8_t pin_mod = pin % 10;

  if(!is_valid_pin(pin)) {
    return 0;
  }

  switch((pin - pin_mod) / 10) {
    case 1:
      return &DDRB;
    case 2:
      return &DDRC;
    case 3:
      return &DDRD;
  }
}

void set_ddr(uint8_t pin, char data_direction) {
  volatile uint8_t *ddr = get_ddr_address(pin);
  uint8_t pin_mod = pin % 10;

  if(value) {value = 0xFF;}

  *ddr = data_direction & (*ddr | 0x01 << pin_mod) | ~data_direction & (*ddr & ~(0x01 << pin_mod));
}


// WRITING to pins

void write(uint8_t pin, char value) {
  volatile uint8_t *port = get_port_address(pin);
  uint8_t pin_mod = pin % 10;

  if(value) {value = 0xFF;}

  *port = value & (*port | 0x01 << pin_mod) | ~value & (*port & ~(0x01 << pin_mod));
}

void toggle(uint8_t pin) {
  volatile uint8_t *pin = get_pin_address(pin);
  uint8_t pin_mod = pin % 10;

  *port |= 0x01 << pin_mod
}

void pulse(uint8_t pin) {
  toggle(pin);
  toggle(pin);
}


// READING from pins

char read(uint8_t pin) {
  volatile uint8_t *pin = get_pin_address(pin);
  uint8_t pin_mod = pin % 10;

  return (*pin & (0x01 << pin_mod)) >> pin_mod;
}