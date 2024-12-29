#include <SPI.h>
#include "adc.h"
#include "mux.h"
#include "avrio.h"
#include "pins.h"

#include <avr/io.h>            
#include <avr/interrupt.h>

void (*async_callback)(uint16_t, uint8_t);

void enable_int1() {
  EIMSK |= 0b00000010;
}

void disable_int1() {
  EIMSK &= 0b11111101;
}

void interrupt_config() {
  EICRA = 0b00001000; // configure external interrupt 1 as falling edge trigger
  // enable_int1(); // enable external interrupt 1 on pin PD3  
}

ISR(INT1_vect) {
  // Serial.println("ISR called");
  disable_int1();
  write(ADC_CS_PIN, LOW);
  uint8_t status = SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001);
  write(ADC_CS_PIN, HIGH);
  
  if(status & 0b00000100) {
    Serial.println(status, BIN);
    async_callback(0, 1);
    // write(ADC_CS_PIN, LOW);
    // status = SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001);
    // write(ADC_CS_PIN, HIGH);
    // Serial.println(status, BIN);
    return;
  }
  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001);
  char val_msb = SPI.transfer(0x00);
  char val_lsb = SPI.transfer(0x00);
  write(ADC_CS_PIN, HIGH);
  // Serial.print("ch: ");
  // Serial.print(get_mux_channel());
  // Serial.print(", val:");
  // Serial.println((val_msb << 8) | (val_lsb & 0x00FF));
  async_callback((val_msb << 8) | (val_lsb & 0x00FF), 0);
  // return (val_msb << 8) | (val_lsb & 0x00FF);
}

void adc_config() {
  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00111000); // device full reset
  write(ADC_CS_PIN, HIGH);
  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000110);
  SPI.transfer(0b11100000); //CONFIG0:  CLK_SEL
  // SPI.transfer(0b00111100); //CONFIG1:  OSR
  SPI.transfer(0b00101100); //CONFIG1:  OSR
  SPI.transfer(0b10101011); //CONFIG2:  GAIN, AZ_MUX
  SPI.transfer(0b00000000); //CONFIG3:  Default
  SPI.transfer(0b00000110); //IRQ:      IRQ_MODE, EN_STP
  write(ADC_CS_PIN, HIGH);
  interrupt_config();
}

void print_binary(char number) {
  for(int8_t i = 7; i >= 0; i--) {
    Serial.print((number >> i) & 1);
  }
}

void print_line_binary(char number) {
  print_binary(number);
  Serial.println();
}

void read_all(char device_address) { // reads all register contents from ADC
  write(ADC_CS_PIN, LOW);
  for(uint8_t i = 0; i < 30; i++) {
    print_binary(SPI.transfer(device_address << 6 | 0b00000011));
  }
  write(ADC_CS_PIN, HIGH);
}

int16_t convert() {
  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00101000);
  write(ADC_CS_PIN, HIGH);
  // delay(10);
  // write(ADC_CS_PIN, LOW);
  // while(SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001) & 0b00000100) {
  //   write(ADC_CS_PIN, LOW);
  //   write(ADC_CS_PIN, HIGH);
  //   Serial.print(read(ADC_IRQ_PIN)?"1":"0");
  // }
  // while(1) {
  //   uint8_t irq = read(ADC_IRQ_PIN);
  //   Serial.print(irq);
  //   if(!irq) break;
  // }
  while(read(ADC_IRQ_PIN) == 0x01); // wait for IRQ pin to go low which signals a finished conversion
  // Serial.println();
  // write(ADC_CS_PIN, HIGH);

  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001);
  char val_msb = SPI.transfer(0x00);
  char val_lsb = SPI.transfer(0x00);
  write(ADC_CS_PIN, HIGH);
  return (val_msb << 8) | (val_lsb & 0x00FF);
}

void convert_async(void callback(uint16_t, uint8_t)) {
  // Serial.println("A");
  enable_int1();
  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00101000);
  write(ADC_CS_PIN, HIGH);
  // Serial.println("B");
  async_callback = callback;
  //return;
  // Serial.println("C");
  return;
  // while(read(ADC_IRQ_PIN) == 0x01) ;
  // if(SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001) & 0b00000100) {
  //   Serial.println("return");
  //   return;
  // }
  // write(ADC_CS_PIN, LOW);
  // SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001);
  // char val_msb = SPI.transfer(0x00);
  // char val_lsb = SPI.transfer(0x00);
  // write(ADC_CS_PIN, HIGH);
  // Serial.println("D");
  // async_callback((val_msb << 8) | (val_lsb & 0x00FF));
  // //Serial.println(&callback, HEX);
  // //Serial.println(&async_callback, HEX);
  // Serial.println("E");
}

float adc_to_voltage(int16_t adc, int16_t zero, int16_t ref) {
  float slope = U_REF / ((float)ref - (float)zero);
  float voltage = slope * ((float)adc - (float)zero);

  return voltage;
}

float get_voltage(uint8_t channel) {
  if(channel > 7) { // CD4097 only has 8 Channels
    return 0;
  }

  select_mux_channel(GND_CH);
  int16_t min = convert();
  select_mux_channel(REF_CH);
  int16_t max = convert();
  select_mux_channel(channel);
  int16_t adc = convert();

  return adc_to_voltage(adc, min, max);
}
