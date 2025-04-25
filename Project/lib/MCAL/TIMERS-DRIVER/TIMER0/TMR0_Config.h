#ifndef TMR0_CONFIGURATION_H_
#define TMR0_CONFIGURATION_H_

#include "CPU_Configuration.h"

// Timer0 Modes
typedef enum
{
    TMR0_MODE_NORMAL = 0, // Normal mode
    TMR0_MODE_CTC = 1     // Clear Timer on Compare Match
} TMR0_Mode_t;

// Timer0 Clock Source (Prescaler)
typedef enum
{
    TMR0_PRESCALER_NO = 0b001,  // No prescaler (fosc/1)
    TMR0_PRESCALER_8 = 0b010,   // fosc/8
    TMR0_PRESCALER_64 = 0b011,  // fosc/64
    TMR0_PRESCALER_256 = 0b100, // fosc/256
    TMR0_PRESCALER_1024 = 0b101 // fosc/1024
} TMR0_Prescaler_t;

// Default Configuration
#define TMR0_MODE TMR0_MODE_CTC
#define TMR0_PRESCALER TMR0_PRESCALER_64
#define TMR0_DEFAULT_TICKS 250 // Default OCR0A value for reasonable interrupt frequency

#endif