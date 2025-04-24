#include "TIMERS-DRIVER/TIMER1/TMR1_Config.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_Interface.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_Address.h"

static void (*TMR1_CallbackA)(void) = 0;
static void (*TMR1_CallbackB)(void) = 0;
static void (*TMR1_OverflowCallback)(void) = 0;

static uint32_t required_overflows = 0;
static uint32_t overflow_counter = 0;
static uint16_t compare_match_counter = 0;

static uint16_t TMR1_CalculateTicks(uint16_t milliseconds, uint32_t *overflow_count);

// Initialize Timer1 with the default mode and prescaler
void TMR1_Init(void)
{
// Set mode based on configuration
#if TMR1_DEFAULT_MODE == TMR1_MODE_NORMAL
	TCCR1A_REG = 0;
	CLR_BIT(TCCR1B_REG, WGM12); // Normal mode (WGM bits = 0000)
	SET_BIT(TIMSK_REG, TOIE1);	// Enable overflow interrupt

#elif TMR1_DEFAULT_MODE == TMR1_MODE_CTC
	TCCR1A_REG = 0;
	SET_BIT(TCCR1B_REG, WGM12); // CTC mode (WGM bits = 0100)
	SET_BIT(TIMSK_REG, OCIE1A); // Enable Compare Match A interrupt

#elif TMR1_DEFAULT_MODE == TMR1_MODE_PWM_FAST
	SET_BIT(TCCR1B_REG, WGM12);	 // Set WGM bits for Fast PWM mode
	SET_BIT(TCCR1B_REG, WGM13);	 // Set WGM bits for Fast PWM mode
	SET_BIT(TCCR1A_REG, WGM11);	 // Fast PWM mode, use ICR1 as TOP
	SET_BIT(TCCR1A_REG, COM1A1); // Non-inverting mode on OC1A

#elif TMR1_DEFAULT_MODE == TMR1_MODE_PWM_PHASE
	SET_BIT(TCCR1A_REG, WGM11); // Phase-correct PWM mode
	SET_BIT(TCCR1A_REG, WGM13);
#endif

	// Set prescaler based on configuration
	switch (TMR1_DEFAULT_PRESCALER)
	{
	case TMR1_PRESCALER_NO:
		SET_BIT(TCCR1B_REG, CS10); // No prescaler
		break;
	case TMR1_PRESCALER_8:
		SET_BIT(TCCR1B_REG, CS11);
		break;
	case TMR1_PRESCALER_64:
		SET_BIT(TCCR1B_REG, CS10);
		SET_BIT(TCCR1B_REG, CS11);
		break;
	case TMR1_PRESCALER_256:
		SET_BIT(TCCR1B_REG, CS12); // Set prescaler to 256
		break;
	case TMR1_PRESCALER_1024:
		SET_BIT(TCCR1B_REG, CS10); // Set prescaler to 1024
		SET_BIT(TCCR1B_REG, CS12);
		break;
	}
}

// Start Timer1 with a specified time in milliseconds
void TMR1_Start(uint16_t milliseconds)
{
	uint32_t overflow_count = 0;
	uint16_t ticks = TMR1_CalculateTicks(milliseconds, &overflow_count);

// Handle CTC mode by setting the top value in OCR1A
#if TMR1_DEFAULT_MODE == TMR1_MODE_CTC
	OCR1A_REG = ticks;					 // Set the top value for CTC mode
	TCNT1_REG = 0;						 // Reset the counter to 0
	compare_match_counter = 0;			 // Reset compare match counter
	required_overflows = overflow_count; // Set number of compare matches needed
#elif TMR1_DEFAULT_MODE == TMR1_MODE_NORMAL
	TCNT1_REG = 65535 - ticks;			 // Set the counter to achieve the desired delay
	overflow_counter = 0;				 // Reset overflow counter
	required_overflows = overflow_count; // Set number of overflows needed
#endif
}

// Stop Timer1
void TMR1_Stop(void)
{
	// Stop the timer by clearing the clock select bits (CS12:0)
	CLR_BIT(TCCR1B_REG, CS10);
	CLR_BIT(TCCR1B_REG, CS12);
	CLR_BIT(TCCR1B_REG, CS11);
}

// Set callback for Compare Match A
void TMR1_SetCallbackA(void (*callbackFunc)(void))
{
	TMR1_CallbackA = callbackFunc;
	SET_BIT(TIMSK_REG, OCIE1A); // Enable Compare Match A interrupt
}

// Set callback for Compare Match B
void TMR1_SetCallbackB(void (*callbackFunc)(void))
{
	TMR1_CallbackB = callbackFunc;
	SET_BIT(TIMSK_REG, OCIE1B); // Enable Compare Match B interrupt
}

// Set callback for Overflow in Normal mode
void TMR1_SetOverflowCallback(void (*callbackFunc)(void))
{
	TMR1_OverflowCallback = callbackFunc;
	SET_BIT(TIMSK_REG, TOIE1); // Enable Overflow interrupt
}

// ISR for Timer1 Compare Match A (CTC mode)
ISR(TIMER1_COMPA_vect)
{
	compare_match_counter++;
	if (compare_match_counter >= required_overflows)
	{
		compare_match_counter = 0; // Reset counter
		if (TMR1_CallbackA != 0)
		{
			TMR1_CallbackA();
		}
	}
}

// ISR for Timer1 Overflow (Normal mode)
ISR(TIMER1_OVF_vect)
{
	overflow_counter++;
	if (overflow_counter >= required_overflows)
	{
		overflow_counter = 0; // Reset counter
		if (TMR1_OverflowCallback != 0)
		{
			TMR1_OverflowCallback();
		}
	}
}

// Calculate the number of ticks based on the desired time in milliseconds
static uint16_t TMR1_CalculateTicks(uint16_t milliseconds, uint32_t *overflow_count)
{
	uint32_t prescaler_value = 1;
	*overflow_count = 0;

	switch (TMR1_DEFAULT_PRESCALER)
	{
	case TMR1_PRESCALER_NO:
		prescaler_value = 1;
		break;
	case TMR1_PRESCALER_8:
		prescaler_value = 8;
		break;
	case TMR1_PRESCALER_64:
		prescaler_value = 64;
		break;
	case TMR1_PRESCALER_256:
		prescaler_value = 256;
		break;
	case TMR1_PRESCALER_1024:
		prescaler_value = 1024;
		break;
	}

	// Calculate total number of ticks for the given time
	uint32_t total_ticks = ((uint32_t)F_CPU / (prescaler_value * 1000)) * milliseconds;

	// If more than one overflow/compare match is needed
	if (total_ticks > 65535)
	{
		*overflow_count = total_ticks / 65536;
		total_ticks = total_ticks % 65536;
	}

	return (uint16_t)total_ticks;
}

// Set PWM duty cycle for Channel A (0-100%)
void TMR1_PWM_SetDutyCycleA(uint8_t duty_cycle)
{
	OCR1A_REG = duty_cycle; // Set duty cycle for Channel A
}

// Set PWM duty cycle for Channel B (0-100%)
void TMR1_PWM_SetDutyCycleB(uint8_t duty_cycle)
{
	OCR1B_REG = duty_cycle; // Set duty cycle for Channel B
}

// Set PWM frequency by adjusting ICR1 (for Fast PWM or Phase-correct PWM modes)
void TMR1_PWM_SetFrequency(uint32_t frequency)
{
	uint32_t prescaler_value = 1;

	switch (TMR1_DEFAULT_PRESCALER)
	{
	case TMR1_PRESCALER_NO:
		prescaler_value = 1;
		break;
	case TMR1_PRESCALER_8:
		prescaler_value = 8;
		break;
	case TMR1_PRESCALER_64:
		prescaler_value = 64;
		break;
	case TMR1_PRESCALER_256:
		prescaler_value = 256;
		break;
	case TMR1_PRESCALER_1024:
		prescaler_value = 1024;
		break;
	}
	// Calculate TOP value based on desired frequency
	uint32_t top_value = (F_CPU / (prescaler_value * frequency)) - 1;

	// Set TOP value in ICR1 for PWM modes
	ICR1_REG = (uint16_t)top_value;
}
