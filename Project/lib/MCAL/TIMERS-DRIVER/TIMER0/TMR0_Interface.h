#ifndef TMR0_INTERFACE_H_
#define TMR0_INTERFACE_H_

#include "std_types.h"
#include "TIMERS-DRIVER/TIMER0/TMR0_Config.h"

// Static variables
static void (*TMR0_Callback)(void) = NULL_PTR;
static uint16_t TMR0_RequiredMatches = 0;
static uint16_t TMR0_CurrentMatches = 0;

// Helper macros for validation
#define IS_VALID_MILLISECONDS(ms) ((ms) > 0)
#define IS_VALID_CALLBACK(func) ((func) != NULL_PTR)

// Initialize Timer0 with configured mode and prescaler
uint8_t TMR0_Init(void);

// Start Timer0 for a given time in milliseconds
uint8_t TMR0_Start(uint16_t milliseconds);

// Stop Timer0
uint8_t TMR0_Stop(void);

// Set callback function for timer interrupts
uint8_t TMR0_SetCallback(void (*callbackFunc)(void));

#endif