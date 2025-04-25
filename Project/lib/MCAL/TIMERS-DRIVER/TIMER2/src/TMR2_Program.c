#include "std_types.h"
#include "CPU_Configuration.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Config.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Interface.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Address.h"

// Helper function to calculate ticks
static uint8_t TMR2_CalculateTicks(uint16_t milliseconds, uint16_t *ticks, uint16_t *matches)
{
	if (!IS_VALID_MILLISECONDS(milliseconds))
		return E_NOK;

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
	default:
		return E_NOK;
	}

	// Calculate ticks: (F_CPU / (prescaler * 1000)) * milliseconds
	uint32_t total_ticks = ((uint32_t)F_CPU / (prescaler_value * 1000)) * milliseconds;

	// For CTC mode, ticks must fit in OCR2A (0–255)
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
 * Initialize Timer2
 */
uint8_t TMR2_Init(void)
{
	// Clear control registers
	TCCR2A = 0;
	TCCR2B = 0;

	// Set mode
#if TMR2_MODE == TMR2_MODE_NORMAL
	CLR_BIT(TCCR2A, WGM20);
	CLR_BIT(TCCR2A, WGM21);
	CLR_BIT(TCCR2B, WGM22); // Normal mode
	SET_BIT(TIMSK2, TOIE2); // Enable overflow interrupt
#elif TMR2_MODE == TMR2_MODE_CTC
	CLR_BIT(TCCR2A, WGM20);
	SET_BIT(TCCR2A, WGM21);
	CLR_BIT(TCCR2B, WGM22);	 // CTC mode
	SET_BIT(TIMSK2, OCIE2A); // Enable compare match A interrupt
#else
	return E_NOK; // Invalid mode
#endif

	// Enable global interrupts
	return GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}

/*
 * Initialize Timer2 for PWM mode
 */
uint8_t TMR2_InitPWM(void)
{
	// Fast PWM Mode (WGM2[2:0] = 0b111)
	SET_BIT(TCCR2A, WGM20);
	SET_BIT(TCCR2A, WGM21);
	SET_BIT(TCCR2B, WGM22);

	// Non-inverting mode for OC2B (COM2B1=1, COM2B0=0)
	SET_BIT(TCCR2A, COM2B1);
	CLR_BIT(TCCR2A, COM2B0);

	OCR2B = 0; // Start with 0% duty cycle

	// Set prescaler (e.g., 64)
	TCCR2B = (TCCR2B & ~0x07) | (TMR2_PRESCALER_64 & 0x07);

	return E_OK;
}
/*
 * Start Timer2 for the specified time
 */
uint8_t TMR2_Start(uint16_t milliseconds)
{
	uint16_t ticks = 0;
	uint16_t matches = 0;

	// Calculate ticks and required matches
	uint8_t status = TMR2_CalculateTicks(milliseconds, &ticks, &matches);
	if (status != E_OK)
		return E_NOK;

	// Set compare value for CTC mode
#if TMR2_MODE == TMR2_MODE_CTC
	OCR2A = (uint8_t)ticks;
#endif

	// Initialize counters
	TMR2_RequiredMatches = matches;
	TMR2_CurrentMatches = 0;
	TCNT2 = 0; // Reset counter

	// Start timer with prescaler
	TCCR2B = (TCCR2B & ~(0x07)) | (TMR2_PRESCALER & 0x07);

	return E_OK;
}

/*
 * Stop Timer2
 */
uint8_t TMR2_Stop(void)
{
	// Stop timer by clearing clock select bits
	TCCR2B &= ~(0x07);
	return E_OK;
}

/*
 * Set callback for timer interrupts
 */
uint8_t TMR2_SetCallback(void (*callbackFunc)(void))
{
	if (!IS_VALID_CALLBACK(callbackFunc))
		return E_NOK;

	TMR2_Callback = callbackFunc;
	return E_OK;
}

/*
 * Set PWM duty cycle (0%–100%)
 */
uint8_t TMR2_SetPWMDutyCycle(uint8_t duty_cycle)
{
	if (!IS_VALID_DUTY_CYCLE(duty_cycle))
		return E_NOK;
	OCR2B = (duty_cycle * 255) / 100;
	return E_OK;
}
