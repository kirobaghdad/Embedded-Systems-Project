#include "TIMERS-DRIVER/TIMER2/TMR2_Config.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Interface.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Address.h"

// Pointer to the user-defined callback function
static void (*TMR2_Callback)(void) = 0;

// Number of compare matches required to reach the desired time
static uint16_t TMR2_RequiredMatches = 0;

// Counter to track the number of compare matches
static uint16_t TMR2_CurrentMatches = 0;

// Function prototypes
static uint16_t TMR2_CalculateTicks(uint16_t milliseconds);

void TMR2_Init(void)
{
	// Enable global interrupts
	SET_BIT(SREG, 7);
// Set mode based on configuration
#if TMR2_MODE == TMR2_MODE_NORMAL
	CLR_BIT(TCCR2_REG, TCCR2_WGM20);
	CLR_BIT(TCCR2_REG, TCCR2_WGM21); // Normal mode
#elif TMR2_MODE == TMR2_MODE_CTC
	CLR_BIT(TCCR2_REG, TCCR2_WGM20);
	SET_BIT(TCCR2_REG, TCCR2_WGM21); // CTC mode
#elif TMR2_MODE == TMR2_MODE_PWM
	SET_BIT(TCCR2_REG, TCCR2_WGM20);
	SET_BIT(TCCR2_REG, TCCR2_WGM21); // Fast PWM mode
#endif

// Enable Timer interrupt based on the mode
#if TMR2_MODE == TMR2_MODE_NORMAL
	SET_BIT(TIMSK_REG, TIMSK_TOIE2); // Enable Overflow Interrupt
#elif TMR2_MODE == TMR2_MODE_CTC
	SET_BIT(TIMSK_REG, TIMSK_OCIE2); // Enable Compare Match Interrupt
#endif
}

void TMR2_Start(uint16_t milliseconds)
{
	uint16_t ticks = TMR2_CalculateTicks(milliseconds);

	// Set compare value for CTC mode
	if (TMR2_MODE == TMR2_MODE_CTC)
	{
		OCR2_REG = ticks;
	}

	// Reset the match counter
	TMR2_CurrentMatches = 0;

	// Start the timer with the configured prescaler
	switch (TMR2_PRESCALER)
	{
	case TMR2_PRESCALER_NO:
		SET_BIT(TCCR2_REG, TCCR2_CS20); // No prescaler
		break;
	case TMR2_PRESCALER_8:
		SET_BIT(TCCR2_REG, TCCR2_CS21); // Prescaler 8
		break;
	case TMR2_PRESCALER_32:
		SET_BIT(TCCR2_REG, TCCR2_CS20); // Prescaler 32
		SET_BIT(TCCR2_REG, TCCR2_CS21);
		break;
	case TMR2_PRESCALER_64:
		SET_BIT(TCCR2_REG, TCCR2_CS22);
		break;
	case TMR2_PRESCALER_128:
		SET_BIT(TCCR2_REG, TCCR2_CS20); // Prescaler 128
		SET_BIT(TCCR2_REG, TCCR2_CS22);
		break;
	case TMR2_PRESCALER_256:
		SET_BIT(TCCR2_REG, TCCR2_CS21); // Prescaler 256
		SET_BIT(TCCR2_REG, TCCR2_CS22); // Prescaler 256
		break;
	case TMR2_PRESCALER_1024:
		SET_BIT(TCCR2_REG, TCCR2_CS20); // Prescaler 1024
		SET_BIT(TCCR2_REG, TCCR2_CS21); // Prescaler 1024
		SET_BIT(TCCR2_REG, TCCR2_CS22); // Prescaler 1024
		break;
	}
}

void TMR2_Stop(void)
{
	// Stop the timer by clearing the clock source bits
	CLR_BIT(TCCR2_REG, TCCR2_CS20); // Clear WGM bits to stop the timer
	CLR_BIT(TCCR2_REG, TCCR2_CS21); // Clear WGM bits to stop the timer
	CLR_BIT(TCCR2_REG, TCCR2_CS22); // Clear WGM bits to stop the timer
}

void TMR2_SetCallback(void (*callbackFunc)(void))
{
	TMR2_Callback = callbackFunc;
}

// Interrupt Service Routine for Timer2 Compare Match
ISR(TIMER2_COMP_vect)
{
	TMR2_CurrentMatches++;

	// Check if we've reached the required number of matches
	if (TMR2_CurrentMatches >= TMR2_RequiredMatches)
	{
		if (TMR2_Callback != 0)
		{
			TMR2_Callback();
		}
		TMR2_CurrentMatches = 0; // Reset matches for the next cycle
	}
}

// Function to calculate ticks based on the desired time in milliseconds and the configured prescaler
static uint16_t TMR2_CalculateTicks(uint16_t milliseconds)
{
	uint32_t prescaler_value = 1;
	switch (TMR2_PRESCALER)
	{
	case TMR2_PRESCALER_NO:
		prescaler_value = 1;
		break;
	case TMR2_PRESCALER_8:
		prescaler_value = 8;
		break;
	case TMR2_PRESCALER_32:
		prescaler_value = 32;
		break;
	case TMR2_PRESCALER_64:
		prescaler_value = 64;
		break;
	case TMR2_PRESCALER_128:
		prescaler_value = 128;
		break;
	case TMR2_PRESCALER_256:
		prescaler_value = 256;
		break;
	case TMR2_PRESCALER_1024:
		prescaler_value = 1024;
		break;
	}

	// Calculate ticks based on prescaler and the desired time in milliseconds
	uint32_t ticks = ((F_CPU / (prescaler_value * 1000)) * milliseconds);

	// If the number of ticks exceeds 255 (the max value for an 8-bit timer), calculate how many compare matches are required
	if (ticks > 255)
	{
		TMR2_RequiredMatches = ticks / 255;
	}
	else
	{
		TMR2_RequiredMatches = 1; // Only one match is needed
	}

	return (uint16_t)ticks;
}
