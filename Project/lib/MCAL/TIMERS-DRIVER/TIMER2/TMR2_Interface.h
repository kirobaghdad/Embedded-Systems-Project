#ifndef TMR2_INTERFACE_H_
#define TMR2_INTERFACE_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Config.h"
// Static variables
static void (*TMR2_Callback)(void) = NULL_PTR;
static uint16_t TMR2_RequiredMatches = 0;
static uint16_t TMR2_CurrentMatches = 0;

// Helper macros for validation
#define IS_VALID_MILLISECONDS(ms) ((ms) > 0)
#define IS_VALID_CALLBACK(func) ((func) != NULL_PTR)

// Initialize Timer2 with configured mode and prescaler
uint8_t TMR2_Init(void);
uint8_t TMR2_InitPWM(void);
uint8_t TMR2_SetPWMDutyCycle(uint8_t duty_cycle);
// Start Timer2 for a given time in milliseconds
uint8_t TMR2_Start(uint16_t milliseconds);

// Stop Timer2
uint8_t TMR2_Stop(void);

// Set callback function for timer interrupts
uint8_t TMR2_SetCallback(void (*callbackFunc)(void));

#endif 