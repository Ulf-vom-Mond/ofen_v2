#include <SPI.h>
#include "adc.h"
#include "mux.h"
#include "avrio.h"
#include "pins.h"


void adc_config() {
  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000110);
  SPI.transfer(0b11100000); //CONFIG0:  CLK_SEL
  SPI.transfer(0b00111100); //CONFIG1:  OSR
  SPI.transfer(0b10101011); //CONFIG2:  GAIN, AZ_MUX
  SPI.transfer(0b00000000); //CONFIG3:  Default
  SPI.transfer(0b00000110); //IRQ:      IRQ_MODE, EN_STP
  write(ADC_CS_PIN, HIGH);
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
  delay(10);
  write(ADC_CS_PIN, LOW);
  while(SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001) & 0b00000100) {
    write(ADC_CS_PIN, LOW);
    write(ADC_CS_PIN, HIGH);
  }
  write(ADC_CS_PIN, HIGH);

  write(ADC_CS_PIN, LOW);
  SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001);
  char val_msb = SPI.transfer(0x00);
  char val_lsb = SPI.transfer(0x00);
  write(ADC_CS_PIN, HIGH);

  return (val_msb << 8) | (val_lsb & 0x00FF);
}

float get_voltage(uint8_t channel) {
  if(channel > 7) { // CD4097 only has 8 Channels
    return 0;
  }

  int32_t min = 0;
  int32_t max = 32767;
  int32_t adc = 0;

  float voltage = 0;

  select_mux_channel(GND_CH);
  min = convert();

  select_mux_channel(REF_CH);
  max = convert();

  select_mux_channel(channel);
  adc = convert();
  Serial.print("max: ");
  Serial.println(max);
  Serial.print("min: ");
  Serial.println(min);
  Serial.print("adc: ");
  Serial.println(adc);

  float slope = U_REF / (max - min);
  voltage = slope * (adc - min);

  return voltage;
}
