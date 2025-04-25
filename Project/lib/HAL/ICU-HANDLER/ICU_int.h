#ifndef MCAL_ICU_DRIVER_ICU_INT_H_
#define MCAL_ICU_DRIVER_ICU_INT_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_Address.h"

typedef enum
{
    ICU_EDGE_FALLING = 0,
    ICU_EDGE_RISING = 1
} ICU_EdgeType;

// Initialize the ICU with configured settings
uint8_t ICU_Init(void);

// Deinitialize the ICU (stop timer and disable interrupts)
uint8_t ICU_DeInit(void);

// Set edge detection (rising or falling)
uint8_t ICU_SetEdgeDetection(ICU_EdgeType edge);

// Get the captured timer value
uint16_t ICU_GetCaptureValue(void);

// Set callback function for input capture interrupt
uint8_t ICU_SetCallback(void (*callbackFunc)(void));

// Clear Timer1 counter
uint8_t ICU_ClearTimer(void);

// Enable input capture interrupt
uint8_t ICU_EnableInterrupt(void);

// Disable input capture interrupt
uint8_t ICU_DisableInterrupt(void);

#endif