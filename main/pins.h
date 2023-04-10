#pragma once

#define IN  0
#define OUT 1

void pin_config();

// Pin definitions

// AVR pins get a number assigned according to the following scheme:
// The second decimal digit corresponds to the Port, starting at 0 for PORTA
// The first decimal digit corresponds to the Pin going from 0 thru 7
// Examples: PB5 is 15, PD6 is 36

// SD Card:
#define SD_CS_PIN       12 // PB2
#define SD_CS_DDR       OUT

// ILI9488 LCD:
#define LCD_CS_PIN      37 // PD7
#define LCD_CS_DDR      OUT
#define LCD_RST_PIN     10 // PB0
#define LCD_RST_DDR     OUT
#define LCD_DC_PIN      11 // PB1
#define LCD_DC_DDR      OUT

// XPT2046 Touch Controller:
#define TOUCH_CS_PIN    20 // PC0
#define TOUCH_CS_DDR    OUT
#define TOUCH_IRQ_PIN   32 // PD2
#define TOUCH_IRQ_DDR   IN

// MCP3461 ADC:
#define ADC_CS_PIN      21 // PC1
#define ADC_CD_DDR      OUT
#define ADC_IRQ_PIN     33 // PD3
#define ADC_IRQ_DDR     IN

// CD4097 Multiplexer:
#define MUX_A_PIN       24 // PC4
#define MUX_A_DDR       OUT
#define MUX_B_PIN       23 // PC3
#define MUX_B_DDR       OUT
#define MUX_C_PIN       21 // PC1
#define MUX_C_DDR       OUT

// CD4017 Demultiplexer:
#define DEMUX_A0_PIN    36 // PD6
#define DEMUX_A0_DDR    OUT
#define DEMUX_A1_PIN    35 // PD5
#define DEMIX_A1_DDR    OUT

// 74HC165 Shift Register:
#define SHIFT_Q_PIN     34 // PD4
#define SHIFT_Q_DDR     IN
#define SHIFT_CLK_PIN   17 // PB7
#define SHIFT_CLK_DDR   OUT
#define SHIFT_LOAD_PIN  16 // PB6
#define SHIFT_LOAD_DDR  OUT

// Heater Relais:
#define HEATER_PIN      25 // PC5
#define HEATER_DDR      OUT


// SPI:
#define MISO_PIN      14  
#define MISO_DDR      IN
#define MOSI_PIN      13
#define MOSI_DDR      OUT
#define SCK_PIN       15
#define SCK_DDR       OUT

// UART:
#define RX_PIN        30
#define RX_DDR        IN
#define TX_PIN        31
#define TX_DDR        OUT

// RESET
#define RST_PIN       26
#define RST_DDR       IN