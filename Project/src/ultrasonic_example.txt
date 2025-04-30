#include "std_types.h"
#include <util/delay.h>
extern "C" {
    #include "DIO-DRIVER/dio_int.h"
    #include "ULTRASONIC-HANDLER/ultrasonic_int.h"
    #include "LED-HANDLER/led_int.h"
}
#define LED_PORT PORT_B
#define LED_PIN PIN_5  
#define DISTANCE_THRESHOLD 0.5
static ultrasonic_config_t front_sensor = {{PORT_B, PIN_0}, {PORT_D, PIN_2}};
static ultrasonic_config_t right_sensor = {{PORT_B, PIN_0}, {PORT_D, PIN_4}};
static ultrasonic_config_t left_sensor = {{PORT_B, PIN_0}, {PORT_D, PIN_7}};
int main(void)
{
    uint8_t status; 
    uint16_t distance;

    
    Ultrasonic_init(&front_sensor);
    Ultrasonic_init(&right_sensor);
    Ultrasonic_init(&left_sensor);

    DIO_u8SetPinMode(PORT_B, PIN_1, OUTPUT); 
    status = DIO_u8SetPinMode(LED_PORT, LED_PIN, OUTPUT);
    if (status != E_OK)
        while (1); 

    
    while (1)
    {
        
        distance = Ultrasonic_readDistance(&front_sensor);
        LED_u8LedBlink(PORT_B, PIN_1, 50); // Blink the LED on pin 1 of PORTB every 1000 ms
 
        
        if (distance < DISTANCE_THRESHOLD)
        {
            status = DIO_u8SetPinValue(LED_PORT, LED_PIN, HIGH); 
            if (status != E_OK)
                while (1); 
        }
        else
        {
            status = DIO_u8SetPinValue(LED_PORT, LED_PIN, LOW); 
            if (status != E_OK)
                while (1); 
        }

        
        _delay_ms(500);
    }

    return 0;
}