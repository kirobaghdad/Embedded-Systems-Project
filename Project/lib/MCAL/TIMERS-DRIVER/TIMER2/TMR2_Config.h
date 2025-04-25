#ifndef TMR2_CONFIGURATION_H_
#define TMR2_CONFIGURATION_H_

#include "CPU_Configuration.h"

// Timer2 Modes
typedef enum
{
    TMR2_MODE_NORMAL = 0, // Normal mode
    TMR2_MODE_CTC = 1,     // Clear Timer on Compare Match
    TMR2_MODE_PWM = 2     // Fast PWM
} TMR2_Mode_t;

// Timer2 Clock Source (Prescaler)
typedef enum
{
    TMR2_PRESCALER_NO = 0b001,  // No prescaler (fosc/1)
    TMR2_PRESCALER_8 = 0b010,   // fosc/8
    TMR2_PRESCALER_32 = 0b011,  // fosc/32
    TMR2_PRESCALER_64 = 0b100,  // fosc/64
    TMR2_PRESCALER_128 = 0b101, // fosc/128
    TMR2_PRESCALER_256 = 0b110, // fosc/256
    TMR2_PRESCALER_1024 = 0b111 // fosc/1024
} TMR2_Prescaler_t;

// Default Configuration
#define TMR2_MODE TMR2_MODE_CTC
#define TMR2_PWM_MODE TMR2_MODE_PWM
#define TMR2_PRESCALER TMR2_PRESCALER_64
#define TMR2_DEFAULT_TICKS 250 // Default OCR2A value for CTC mode
#define TMR2_PWM_DEFAULT_DUTY 0 // Default PWM duty cycle (0–255)
#endif