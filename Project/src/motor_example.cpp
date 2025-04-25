#include <Arduino.h>
#include <util/delay.h>
#include "std_types.h"
extern "C" {
    #include "DIO-DRIVER/dio_int.h"
    #include "MOTOR-HANDLER/motor_int.h"
}

int main(void)
{
    uint8_t status;
    Serial.begin(9600); // Initialize serial communication for debugging
    Serial.println("Motor Control Example");
    // Initialize motor (includes PWM setup)
    status = MOTOR_u8MotorInit();
    if (status != E_OK)
        while (1){
            Serial.println("Motor Init Failed");
        } // Halt on failure

    // Main loop
    while (1)
    {

        // Rotate right for 3 seconds
        status = MOTOR_u8RightRotate(255);
        Serial.println("Motor Right Rotate");
        if (status != E_OK)
        while (1){
            Serial.println("Motor Init Failed");
        } // Halt on failure
        // if (status != E_OK)
        //     while (1); // Halt on failure
        _delay_ms(3000);

        // // Stop for 1 second
        status = MOTOR_u8MotorOff();
        // if (status != E_OK)
        //     while (1); // Halt on failure
        _delay_ms(1000);

        // // Set speed to 100% (255/255)

        // // Rotate left for 3 seconds
        status = MOTOR_u8LeftRotate(200);
        // if (status != E_OK)
        //     while (1); // Halt on failure
        _delay_ms(3000);

        // // Stop for 1 second
        status = MOTOR_u8MotorOff();
        // if (status != E_OK)
        //     while (1); // Halt on failure
        _delay_ms(1000);
    }

    return 0;
}