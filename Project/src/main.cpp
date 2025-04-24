#include "std_types.h"
extern "C" {
  #include "DIO-DRIVER/dio_int.h"
  #include "LED-HANDLER/led_int.h"
}
int main() {

  DIO_u8SetPinMode(PORT_A, PIN_1, OUTPUT);
  while(true){
    LED_u8LedBlink(PORT_A, PIN_0, 40);
  }
}