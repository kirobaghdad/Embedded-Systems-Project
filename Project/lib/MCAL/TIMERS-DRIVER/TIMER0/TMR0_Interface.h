#ifndef TMR0_INTERFACE_H_
#define TMR0_INTERFACE_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER0/TMR0_Config.h"

// Initialize Timer0 with configured mode and prescaler
uint8_t TMR0_Init(void);

// Start Timer0 for a given time in milliseconds
uint8_t TMR0_Start(uint16_t milliseconds);

// Stop Timer0
uint8_t TMR0_Stop(void);

// Set callback function for timer interrupts
uint8_t TMR0_SetCallback(void (*callbackFunc)(void));

#endif /* MCAL_TMR0_DRIVER_TMR0_INT_H_ */