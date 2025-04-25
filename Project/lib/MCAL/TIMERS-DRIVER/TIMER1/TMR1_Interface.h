#ifndef TMR1_INTERFACE_H_
#define TMR1_INTERFACE_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_config.h"

// Static variables
static void (*TMR1_CallbackA)(void) = NULL_PTR;
static void (*TMR1_CallbackB)(void) = NULL_PTR;
static void (*TMR1_OverflowCallback)(void) = NULL_PTR;
static uint32_t TMR1_RequiredMatches = 0;
static uint32_t TMR1_CurrentMatches = 0;

// Helper macros for validation
#define IS_VALID_MILLISECONDS(ms) ((ms) > 0)
#define IS_VALID_DUTY_CYCLE(dc) ((dc) <= 100)
#define IS_VALID_FREQUENCY(freq) ((freq) > 0)
#define IS_VALID_CALLBACK(func) ((func) != NULL_PTR)

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

#endif