#ifndef ICU_CFG_H_
#define ICU_CFG_H_


// Include Timer1 config to reuse prescaler defines
#include "../MCAL/TIMERS-DRIVER/TIMER1/TMR1_config.h"

// --- ICU Configuration Options ---

// Select the default clock source (prescaler) for Timer1 when used by ICU
// Options: TMR1_PRESCALER_NO, TMR1_PRESCALER_8, TMR1_PRESCALER_64,
//          TMR1_PRESCALER_256, TMR1_PRESCALER_1024
#define ICU_DEFAULT_PRESCALER TMR1_PRESCALER_64

// Enable or disable the Input Capture Noise Canceler
// Options: 1 (Enabled), 0 (Disabled)
#define ICU_NOISE_CANCELER_ENABLE 1

// Define the initial edge detection state upon initialization
// Options: ICU_EDGE_FALLING (0), ICU_EDGE_RISING (1)
#define ICU_DEFAULT_INITIAL_EDGE ICU_EDGE_RISING // Match enum values below


#endif