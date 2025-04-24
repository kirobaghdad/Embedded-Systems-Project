#include<util/delay.h>
#include <avr/io.h>
#include "std_types.h"
extern "C" {
#include "DIO-DRIVER/dio_int.h"
#include "LED-HANDLER/led_int.h"
}
int main() {

  DIO_u8SetPinMode(PORT_B, PIN_1, OUTPUT); // Set pin 1 of PORTB as output
  while(true){
    LED_u8LedBlink(PORT_B, PIN_1, 50); // Blink the LED on pin 1 of PORTB every 1000 ms
  }
}