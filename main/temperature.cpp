#include "temperature.h"
#include "adc.h"
#include "type_k_array.h"
#include "mux.h"
#include <Arduino.h>

float thermocouple_voltage_to_temp(float thermocouple_voltage) {
  for(int16_t i = 1; i < sizeof(voltages) / sizeof(uint16_t); i++){
    uint16_t voltages_i = pgm_read_word_near(&voltages[i]);
        
    if(voltages_i < thermocouple_voltage * 1000000.0 + 6458.0){ // convert voltage to mikrovolts and add the voltage offset
      continue;
    }

    float   inf_volt = pgm_read_word_near(&voltages[i - 1]); // inferior voltage
    float   sup_volt = voltages_i; // superior voltage
    int16_t inf_temp = i - 270 - TEMP_STEP; // inferior temperature
    int16_t sup_temp = i - 270; // superior temperature

    return (thermocouple_voltage * 1000000.0 + 6458.0 - inf_volt) / (sup_volt - inf_volt) + inf_temp;
  }
  return sizeof(voltages) / sizeof(uint16_t) * TEMP_STEP;
}

float get_thermocouple_temp() {
  float thermocouple_voltage = get_voltage(THERMOCOUPLE_CH);
  return thermocouple_voltage_to_temp(thermocouple_voltage);
}

float thermistor_voltage_to_temp(float thermistor_voltage) {
  float thermistor_resistance_log = logf(U_REG * THERMISTOR_RM / thermistor_voltage - THERMISTOR_RM - THERMISTOR_RS);
  return 1/(THERMISTOR_A + THERMISTOR_B * thermistor_resistance_log + THERMISTOR_C * pow(thermistor_resistance_log, 3)) - 273.15;
}

float get_thermistor_temp() {
  float thermistor_voltage = get_voltage(THERMISTOR_CH);
  return thermistor_voltage_to_temp(thermistor_voltage);
}

float get_temp() {
  // return roundf((get_thermocouple_temp() + get_thermistor_temp()) * 10) / 10.0;
  return get_thermocouple_temp() + get_thermistor_temp();
}