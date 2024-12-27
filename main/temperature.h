#pragma once

#define THERMISTOR_A 0.0004224501353 // constants for Steinhart-Hart thermistor model
#define THERMISTOR_B 0.0002692412786
#define THERMISTOR_C -0.00000008836440272
#define THERMISTOR_RS 3900.0 // series resistor of thermistor
#define THERMISTOR_RM 180.0  // measurement resistor

#define MAX_TEMP 1300

float thermocouple_voltage_to_temp(float thermocouple_voltage);
float get_thermocouple_temp();
float thermistor_voltage_to_temp(float thermistor_voltage);
float get_thermistor_temp();
float get_temp();