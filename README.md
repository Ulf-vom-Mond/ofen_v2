# Ofen v2
This is a project about building an oven for high temperatures. Included in this repository are the schematics and the layout for the control PCB and the microcontroller firmware.

## Building with Arduino IDE
You need to install the following libraries:

### LCDWiki library
This is neccessary for SPI communication with the ILI9488 LCD driver.
Follow this Page for installation: http://www.lcdwiki.com/3.5inch_SPI_Module_ILI9488_SKU:MSP3520

### MiniCore
The schematic uses a bare ATMega328p without the Arduino overhead. For an effective use of all its features, you need to follow this installation guide:
https://github.com/MCUdude/MiniCore

IMPORTANT: You need to set the clock to "Internal 8 MHz". If you select anything called "external", the ATMega will not run and you cannot program it anymore.

