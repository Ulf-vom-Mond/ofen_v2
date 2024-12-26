#include "temperature.h"
#include "adc.h"
#include "type_k_array.h"
#include "mux.h"
#include <Arduino.h>



float get_thermocouple_temp() {
  float thermocouple_voltage = get_voltage(THERMOCOUPLE_CH);
  
  for(int16_t i = 1; i < sizeof(voltages) / sizeof(uint16_t); i++){
    uint16_t voltages_i = pgm_read_word_near(&voltages[i]);
        
    if(voltages_i < thermocouple_voltage * 1000000.0 + 6458.0){ // convert voltage to mikrovolts and add the voltage offset
      continue;
    }

    float   inf_volt = pgm_read_word_near(&voltages[i - 1]); // inferior voltage
    float   sup_volt = voltages_i; // superior voltage
    int16_t inf_temp = i - 271; // inferior temperature
    int16_t sup_temp = i - 270; // superior temperature

    return (thermocouple_voltage * 1000000.0 + 6458.0 - inf_volt) / (sup_volt - inf_volt) + inf_temp;
  }
}

float get_thermistor_temp() {
  float thermistor_voltage = get_voltage(THERMISTOR_CH);
  float thermistor_resistance_log = logf(U_REG * THERMISTOR_RM / thermistor_voltage - THERMISTOR_RM - THERMISTOR_RS);
  // Serial.println(U_REG * THERMISTOR_RM / thermistor_voltage - THERMISTOR_RM - THERMISTOR_RS, 5);
  return 1/(THERMISTOR_A + THERMISTOR_B * thermistor_resistance_log + THERMISTOR_C * pow(thermistor_resistance_log, 3)) - 273.15;
}

float get_temp() {
  // return roundf((get_thermocouple_temp() + get_thermistor_temp()) * 10) / 10.0;
  return get_thermocouple_temp() + get_thermistor_temp();
}