#ifndef TMR1_CONFIGURATION_H_
#define TMR1_CONFIGURATION_H_

#include "CPU_Configuration.h"
#include "DIO-DRIVER/dio_int.h"

// Timer1 Modes
typedef enum
{
    TMR1_MODE_NORMAL = 0,  // Normal mode
    TMR1_MODE_CTC = 1,     // Clear Timer on Compare Match (OCR1A)
    TMR1_MODE_PWM_FAST = 2 // Fast PWM, TOP=ICR1
} TMR1_Mode_t;

// Timer1 Clock Source (Prescaler)
typedef enum
{
    TMR1_PRESCALER_NO = 0b001,  // No prescaler (fosc/1)
    TMR1_PRESCALER_8 = 0b010,   // fosc/8
    TMR1_PRESCALER_64 = 0b011,  // fosc/64
    TMR1_PRESCALER_256 = 0b100, // fosc/256
    TMR1_PRESCALER_1024 = 0b101 // fosc/1024
} TMR1_Prescaler_t;

// Timer1 PWM Pins (fixed on ATmega328P)
#define TMR1_OC1A_PORT PORT_B
#define TMR1_OC1A_PIN PIN_1 // PB1 (OC1A)
#define TMR1_OC1B_PORT PORT_B
#define TMR1_OC1B_PIN PIN_2 // PB2 (OC1B)

// Default Configuration
#define TMR1_DEFAULT_MODE TMR1_MODE_CTC
#define TMR1_DEFAULT_PRESCALER TMR1_PRESCALER_256
#define TMR1_DEFAULT_TOP 65535     // Default TOP for CTC/Normal
#define TMR1_DEFAULT_PWM_FREQ 1000 // Default PWM frequency (1 kHz)

#endif