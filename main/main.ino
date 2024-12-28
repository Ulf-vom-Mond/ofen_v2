#include <SPI.h>
#include "temperature.h"
#include "user_input.h"
#include "user_interface.h"
#include "pins.h"
#include "avrio.h"
#include "adc.h"
#include "list.h"
#include "keypad.h"
#include "timer.h"
#include "mux.h"

#define BUF_LEN 10
#define MUX_MAX_CH 3

// void key_event_listener(uint16_t keys) {
//   Serial.println((keys), BIN);
// }

char data_ready = 1;
uint8_t mux_state = 0;
uint8_t buf_state = 0;

int16_t adc_buf[8][BUF_LEN] = {0};

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void adc_callback(int16_t data) {
  data_ready = 1;
  adc_buf[mux_state][buf_state] = data;
  // Serial.println(adc_buf[mux_state][buf_state]);
  mux_state++;
  if(mux_state > MUX_MAX_CH) {
    mux_state = 0;
    buf_state++;
    if(buf_state > BUF_LEN) buf_state = 0;
  }
  // Serial.println("adc called back!");
}

void tmr_evnt_lstnr1(void *params) {
  // Serial.println(get_voltage(THERMISTOR_CH), 5);
  // Serial.print("Thermistor: ");
  // Serial.print(get_thermistor_temp());
  // Serial.println(" C");
  // Serial.print("Thermocouple: ");
  // Serial.print(get_thermocouple_temp());
  // Serial.println(" C");
  // Serial.print("Temperature: ");
  // Serial.print(get_temp());
  // Serial.println(" C");
  // select_mux_channel(REF_CH);
  // Serial.println(convert());
  // select_mux_channel(GND_CH);
  // Serial.println(convert());
  // select_mux_channel(THERMISTOR_CH);
  // Serial.println(convert());
  //delay(5000);
  // Serial.println();
  //get_thermistor_temp();

  int32_t adc_buf_avg[8] = {0};
  for(uint8_t j = 0; j<=MUX_MAX_CH; j++) {
    // Serial.print("Channel ");
    // Serial.print(j);
    // Serial.print(": ");
    for(uint8_t i = 0; i<BUF_LEN; i++) {
      adc_buf_avg[j] += adc_buf[j][i];
      // Serial.print(adc_buf[j][i]);
      // Serial.print(", ");
    }
    // Serial.println();
    adc_buf_avg[j] /= BUF_LEN;
    // Serial.println(adc_buf_avg[j]);
  }
  Serial.print("Thermistor: ");
  // Serial.print(adc_to_voltage(adc_buf_avg[THERMISTOR_CH], adc_buf_avg[GND_CH], adc_buf_avg[REF_CH]), 5);
  Serial.print(thermistor_voltage_to_temp(adc_to_voltage(adc_buf_avg[THERMISTOR_CH], adc_buf_avg[GND_CH], adc_buf_avg[REF_CH])));
  Serial.println(" C");
  Serial.print("Thermocouple: ");
  Serial.print(thermocouple_voltage_to_temp(adc_to_voltage(adc_buf_avg[THERMOCOUPLE_CH], adc_buf_avg[GND_CH], adc_buf_avg[REF_CH])));
  Serial.println(" C");
  Serial.println();


  return;
  data_ready = 0;
  convert_async(adc_callback);
  // adc_callback(convert());
  Serial.println("trigger conversion");

  // write(ADC_CS_PIN, LOW);
  // Serial.println(SPI.transfer(DEVICE_ADDRESS << 6 | 0b00000001), BIN);
  // write(ADC_CS_PIN, HIGH);
  

  // write(ADC_IRQ_PIN, 0);
  // delay(1);
  // write(ADC_IRQ_PIN, 1);
  // data_ready = 1;
  // Serial.print("Temperature: ");
  // Serial.print(get_temp());
  // Serial.println(" C");
}

void tmr_evnt_lstnr5(void *params) {
  Serial.println("5sec");
}

void setup() {
  pin_config();

  Serial.begin(115200);
  delay(10);
  Serial.println("Hello!");
  write(ADC_CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  adc_config();
  timer_config();
  sei();

  add_timer_event_listener(tmr_evnt_lstnr1, 1, 0);
  // add_timer_event_listener(tmr_evnt_lstnr5, 5, 0);

  // add_event_listener((void* (*)(uint16_t))&key_event_listener);

  // TCNT1 = 206;
  // TCCR1A =0x00;
	// TCCR1B |= (1<<CS01);		
	// TIMSK1 |= (1<<TOIE1);		// enable timer overflow interrupt
	// sei();

  launch_ui();
  write(MUX_C_PIN, 0);
  Serial.println(read(MUX_C_PIN), BIN);
  write(MUX_C_PIN, 1);
  Serial.println(read(MUX_C_PIN), BIN);
  write(MUX_C_PIN, 0);
  Serial.println(read(MUX_C_PIN), BIN);
  write(MUX_C_PIN, 1);
  Serial.println(read(MUX_C_PIN), BIN);
  write(MUX_C_PIN, 0);
  Serial.println(read(MUX_C_PIN), BIN);
}

void loop() {
  emit_key_events();
  emit_timer_events();
  if(data_ready) {
    select_mux_channel(mux_state);
    // select_mux_channel(THERMISTOR_CH);
    data_ready = 0;
    convert_async(adc_callback);
    // adc_callback(convert());
    // Serial.println("trigger conversion");
  }
  // if(read(ADC_IRQ_PIN) == 0) Serial.println("IRQ Pin Low");
  // delay(1000);
  // Serial.println(get_voltage(0));
  // Serial.println(get_voltage(0), 5);
  // Serial.println("GND_CH");
  // select_mux_channel_delay(GND_CH, 0);
  // for(uint8_t i = 0; i<30; i++) {
  //   Serial.println(convert());
  //   //delay(13);
  // }
  // Serial.println("REF_CH");
  // select_mux_channel_delay(REF_CH, 0);
  // for(uint8_t i = 0; i<30; i++) {
  //   Serial.println(convert());
  //   //delay(13);
  // }
  // delay(10);
  // select_mux_channel(REF_CH);
  // delay(5000);
  // select_mux_channel(THERMISTOR_CH);
  // delay(5000);
}