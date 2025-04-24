#ifndef TMR1_INTERFACE_H_
#define TMR1_INTERFACE_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_config.h"

// Initialize Timer1 with configured mode and prescaler
uint8_t TMR1_Init(void);

// Start Timer1 for a given time in milliseconds
uint8_t TMR1_Start(uint16_t milliseconds);

// Stop Timer1
uint8_t TMR1_Stop(void);

// Set callback for Compare Match A interrupt
uint8_t TMR1_SetCallbackA(void (*callbackFunc)(void));

// Set callback for Compare Match B interrupt
uint8_t TMR1_SetCallbackB(void (*callbackFunc)(void));

// Set callback for Overflow interrupt in Normal mode
uint8_t TMR1_SetOverflowCallback(void (*callbackFunc)(void));

// Set PWM duty cycle for Channel A (0-100%)
uint8_t TMR1_PWM_SetDutyCycleA(uint8_t duty_cycle);

// Set PWM duty cycle for Channel B (0-100%)
uint8_t TMR1_PWM_SetDutyCycleB(uint8_t duty_cycle);

// Set PWM frequency for Fast PWM mode
uint8_t TMR1_PWM_SetFrequency(uint32_t frequency);

#endif /* MCAL_TMR1_DRIVER_TMR1_INT_H_ */