#include "std_types.h"
#include "CPU_Configuration.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "TIMERS-DRIVER/TIMER0/TMR0_Config.h"
#include "TIMERS-DRIVER/TIMER0/TMR0_Interface.h"
#include "TIMERS-DRIVER/TIMER0/TMR0_Address.h"


// Helper function to calculate ticks
static uint8_t TMR0_CalculateTicks(uint16_t milliseconds, uint16_t *ticks, uint16_t *matches)
{
	if (!IS_VALID_MILLISECONDS(milliseconds))
		return E_NOK;

	uint32_t prescaler_value = 1;
	switch (TMR0_PRESCALER)
	{
	case TMR0_PRESCALER_NO:
		prescaler_value = 1;
		break;
	case TMR0_PRESCALER_8:
		prescaler_value = 8;
		break;
	case TMR0_PRESCALER_64:
		prescaler_value = 64;
		break;
	case TMR0_PRESCALER_256:
		prescaler_value = 256;
		break;
	case TMR0_PRESCALER_1024:
		prescaler_value = 1024;
		break;
	default:
		return E_NOK;
	}

	// Calculate ticks: (F_CPU / (prescaler * 1000)) * milliseconds
	uint32_t total_ticks = ((uint32_t)F_CPU / (prescaler_value * 1000)) * milliseconds;

	// For CTC mode, ticks must fit in OCR0A (0–255)
	if (total_ticks > 255)
	{
		*matches = (total_ticks + 254) / 255;			  // Ceiling division
		*ticks = (total_ticks + *matches - 1) / *matches; // Distribute ticks
	}
	else
	{
		*matches = 1;
		*ticks = total_ticks;
	}

	return (*ticks > 0 && *ticks <= 255) ? E_OK : E_NOK;
}

/*
 * Initialize Timer0
 */
uint8_t TMR0_Init(void)
{
	// Clear control registers
	TCCR0A = 0;
	TCCR0B = 0;

	// Set mode
#if TMR0_MODE == TMR0_MODE_NORMAL
	CLR_BIT(TCCR0A, WGM00);
	CLR_BIT(TCCR0A, WGM01);
	CLR_BIT(TCCR0B, WGM02); // Normal mode
	SET_BIT(TIMSK0, TOIE0); // Enable overflow interrupt
#elif TMR0_MODE == TMR0_MODE_CTC
	CLR_BIT(TCCR0A, WGM00);
	SET_BIT(TCCR0A, WGM01);
	CLR_BIT(TCCR0B, WGM02);	 // CTC mode
	SET_BIT(TIMSK0, OCIE0A); // Enable compare match A interrupt
#else
	return E_NOK; // Invalid mode
#endif

	// Enable global interrupts
	return GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}

/*
 * Start Timer0 for the specified time
 */
uint8_t TMR0_Start(uint16_t milliseconds)
{
	uint16_t ticks = 0;
	uint16_t matches = 0;

	// Calculate ticks and required matches
	uint8_t status = TMR0_CalculateTicks(milliseconds, &ticks, &matches);
	if (status != E_OK)
		return E_NOK;

	// Set compare value for CTC mode
#if TMR0_MODE == TMR0_MODE_CTC
	OCR0A = (uint8_t)ticks;
#endif

	// Initialize counters
	TMR0_RequiredMatches = matches;
	TMR0_CurrentMatches = 0;
	TCNT0 = 0; // Reset counter

	// Start timer with prescaler
	TCCR0B = (TCCR0B & ~(0x07)) | (TMR0_PRESCALER & 0x07);

	return E_OK;
}

/*
 * Stop Timer0
 */
uint8_t TMR0_Stop(void)
{
	// Stop timer by clearing clock select bits
	TCCR0B &= ~(0x07);
	return E_OK;
}

/*
 * Set callback for timer interrupts
 */
uint8_t TMR0_SetCallback(void (*callbackFunc)(void))
{
	if (!IS_VALID_CALLBACK(callbackFunc))
		return E_NOK;

	TMR0_Callback = callbackFunc;
	return E_OK;
}

/*
 * Timer0 Compare Match A Interrupt Service Routine
 */
void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
#if TMR0_MODE == TMR0_MODE_CTC
	TMR0_CurrentMatches++;
	if (TMR0_CurrentMatches >= TMR0_RequiredMatches)
	{
		if (TMR0_Callback != NULL_PTR)
			TMR0_Callback();
		TMR0_CurrentMatches = 0; // Reset for next cycle
	}
#endif
}

/*
 * Timer0 Overflow Interrupt Service Routine
 */
void __vector_16(void) __attribute__((signal));
void __vector_16(void)
{
#if TMR0_MODE == TMR0_MODE_NORMAL
	if (TMR0_Callback != NULL_PTR)
		TMR0_Callback();
#endif
}