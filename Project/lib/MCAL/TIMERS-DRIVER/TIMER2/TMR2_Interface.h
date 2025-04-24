#ifndef TMR2_INTERFACE_H_
#define TMR2_INTERFACE_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Config.h"

// Initialize Timer2 with configured mode and prescaler
uint8_t TMR2_Init(void);

// Start Timer2 for a given time in milliseconds
uint8_t TMR2_Start(uint16_t milliseconds);

// Stop Timer2
uint8_t TMR2_Stop(void);

// Set callback function for timer interrupts
uint8_t TMR2_SetCallback(void (*callbackFunc)(void));

#endif /* MCAL_TMR2_DRIVER_TMR2_INT_H_ */