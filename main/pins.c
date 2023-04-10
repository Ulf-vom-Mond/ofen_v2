#include "pins.h"
#include "avrio.h"

void pin_config() {
  DDRB = 0b11101111;
  // Shift CP (OUT), Shift PL (OUT), SCK (OUT), MISO (IN), MOSI (OUT), SD CS (OUT), LCD D/C (OUT), LCD RST (OUT)

  DDRC = 0b00111111;
  // RST (IN), HEATER (OUT), MUX A (OUT), MUX B (OUT), MUX C (OUT), ADC CS (OUT), Touch CS (OUT)

  DDRD = 0b11100010;
  // LCD CS (OUT), DEMUX A0 (OUT), DEMUX A1 (OUT), Shift Q7 (IN), ADC IRQ (Interrupt IN), Touch IRQ (Interrupt IN), TXD (OUT), RXD (IN)
}