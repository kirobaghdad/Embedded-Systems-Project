#include "std_types.h"
#include "CPU_Configuration.h"
#include "DIO-DRIVER/dio_int.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_config.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_Interface.h"
#include "TIMERS-DRIVER/TIMER1/TMR1_Address.h"

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

// Helper function to calculate ticks
static uint8_t TMR1_CalculateTicks(uint16_t milliseconds, uint16_t *ticks, uint32_t *matches)
{
	if (!IS_VALID_MILLISECONDS(milliseconds))
		return E_NOK;

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
	default:
		return E_NOK;
	}

	// Calculate ticks: (F_CPU / (prescaler * 1000)) * milliseconds
	uint32_t total_ticks = ((uint32_t)F_CPU / (prescaler_value * 1000)) * milliseconds;

	// For CTC mode, ticks must fit in OCR1A (0–65535)
	if (total_ticks > 65535)
	{
		*matches = (total_ticks + 65534) / 65535;		  // Ceiling division
		*ticks = (total_ticks + *matches - 1) / *matches; // Distribute ticks
	}
	else
	{
		*matches = 1;
		*ticks = total_ticks;
	}

	return (*ticks > 0) ? E_OK : E_NOK;
}

/*
 * Initialize Timer1
 */
uint8_t TMR1_Init(void)
{
	// Clear control registers
	TCCR1A = 0;
	TCCR1B = 0;

	// Set mode and interrupts
#if TMR1_DEFAULT_MODE == TMR1_MODE_NORMAL
	CLR_BIT(TCCR1A, WGM11);
	CLR_BIT(TCCR1A, WGM10);
	CLR_BIT(TCCR1B, WGM13);
	CLR_BIT(TCCR1B, WGM12); // Normal mode
	SET_BIT(TIMSK1, TOIE1); // Enable overflow interrupt
#elif TMR1_DEFAULT_MODE == TMR1_MODE_CTC
	CLR_BIT(TCCR1A, WGM11);
	CLR_BIT(TCCR1A, WGM10);
	CLR_BIT(TCCR1B, WGM13);
	SET_BIT(TCCR1B, WGM12);	 // CTC mode, TOP=OCR1A
	SET_BIT(TIMSK1, OCIE1A); // Enable compare match A interrupt
#elif TMR1_DEFAULT_MODE == TMR1_MODE_PWM_FAST
	SET_BIT(TCCR1A, WGM11);
	CLR_BIT(TCCR1A, WGM10);
	SET_BIT(TCCR1B, WGM13);
	SET_BIT(TCCR1B, WGM12);	 // Fast PWM, TOP=ICR1
	SET_BIT(TCCR1A, COM1A1); // Non-inverting mode, OC1A
	SET_BIT(TCCR1A, COM1B1); // Non-inverting mode, OC1B
	// Configure PWM pins (OC1A=PB1, OC1B=PB2)
	DIO_u8SetPinMode(TMR1_OC1A_PORT, TMR1_OC1A_PIN, OUTPUT);
	DIO_u8SetPinMode(TMR1_OC1B_PORT, TMR1_OC1B_PIN, OUTPUT);
	// Set default TOP and duty cycle
	ICR1 = (uint16_t)((F_CPU / (TMR1_DEFAULT_PRESCALER * TMR1_DEFAULT_PWM_FREQ)) - 1);
	OCR1A = 0;
	OCR1B = 0;
#else
	return E_NOK; // Invalid mode
#endif

	// Set prescaler
	TCCR1B = (TCCR1B & ~(0x07)) | (TMR1_DEFAULT_PRESCALER & 0x07);

	// Enable global interrupts
	return GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}

/*
 * Start Timer1 for the specified time
 */
uint8_t TMR1_Start(uint16_t milliseconds)
{
	uint16_t ticks = 0;
	uint32_t matches = 0;

	// Calculate ticks and required matches
	uint8_t status = TMR1_CalculateTicks(milliseconds, &ticks, &matches);
	if (status != E_OK)
		return E_NOK;

	// Configure based on mode
#if TMR1_DEFAULT_MODE == TMR1_MODE_CTC
	OCR1A = ticks;
	TCNT1 = 0;
	TMR1_RequiredMatches = matches;
	TMR1_CurrentMatches = 0;
#elif TMR1_DEFAULT_MODE == TMR1_MODE_NORMAL
	TCNT1 = 65535 - ticks;
	overflow_counter = 0;
	TMR1_RequiredMatches = matches;
#else
	return E_NOK; // Not applicable for PWM mode
#endif

	return E_OK;
}

/*
 * Stop Timer1
 */
uint8_t TMR1_Stop(void)
{
	// Stop timer by clearing clock select bits
	TCCR1B &= ~(0x07);
	return E_OK;
}

/*
 * Set callback for Compare Match A
 */
uint8_t TMR1_SetCallbackA(void (*callbackFunc)(void))
{
	if (!IS_VALID_CALLBACK(callbackFunc))
		return E_NOK;

	TMR1_CallbackA = callbackFunc;
	SET_BIT(TIMSK1, OCIE1A);
	return E_OK;
}

/*
 * Set callback for Compare Match B
 */
uint8_t TMR1_SetCallbackB(void (*callbackFunc)(void))
{
	if (!IS_VALID_CALLBACK(callbackFunc))
		return E_NOK;

	TMR1_CallbackB = callbackFunc;
	SET_BIT(TIMSK1, OCIE1B);
	return E_OK;
}

/*
 * Set callback for Overflow
 */
uint8_t TMR1_SetOverflowCallback(void (*callbackFunc)(void))
{
	if (!IS_VALID_CALLBACK(callbackFunc))
		return E_NOK;

	TMR1_OverflowCallback = callbackFunc;
	SET_BIT(TIMSK1, TOIE1);
	return E_OK;
}

/*
 * Set PWM duty cycle for Channel A (0-100%)
 */
uint8_t TMR1_PWM_SetDutyCycleA(uint8_t duty_cycle)
{
	if (!IS_VALID_DUTY_CYCLE(duty_cycle) || TMR1_DEFAULT_MODE != TMR1_MODE_PWM_FAST)
		return E_NOK;

	OCR1A = (uint16_t)((duty_cycle * (uint32_t)ICR1) / 100);
	return E_OK;
}

/*
 * Set PWM duty cycle for Channel B (0-100%)
 */
uint8_t TMR1_PWM_SetDutyCycleB(uint8_t duty_cycle)
{
	if (!IS_VALID_DUTY_CYCLE(duty_cycle) || TMR1_DEFAULT_MODE != TMR1_MODE_PWM_FAST)
		return E_NOK;

	OCR1B = (uint16_t)((duty_cycle * (uint32_t)ICR1) / 100);
	return E_OK;
}

/*
 * Set PWM frequency
 */
uint8_t TMR1_PWM_SetFrequency(uint32_t frequency)
{
	if (!IS_VALID_FREQUENCY(frequency) || TMR1_DEFAULT_MODE != TMR1_MODE_PWM_FAST)
		return E_NOK;

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
	default:
		return E_NOK;
	}

	// Calculate TOP: F_CPU / (prescaler * frequency) - 1
	uint32_t top_value = (F_CPU / (prescaler_value * frequency)) - 1;
	if (top_value > 65535)
		return E_NOK;

	ICR1 = (uint16_t)top_value;
	return E_OK;
}

/*
 * Timer1 Compare Match A Interrupt Service Routine
 */
void __vector_11(void) __attribute__((signal));
void __vector_11(void)
{
#if TMR1_DEFAULT_MODE == TMR1_MODE_CTC
	TMR1_CurrentMatches++;
	if (TMR1_CurrentMatches >= TMR1_RequiredMatches)
	{
		if (TMR1_CallbackA != NULL_PTR)
			TMR1_CallbackA();
		TMR1_CurrentMatches = 0;
	}
#endif
}

/*
 * Timer1 Compare Match B Interrupt Service Routine
 */
void __vector_12(void) __attribute__((signal));
void __vector_12(void)
{
	if (TMR1_CallbackB != NULL_PTR)
		TMR1_CallbackB();
}

/*
 * Timer1 Overflow Interrupt Service Routine
 */
void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
#if TMR1_DEFAULT_MODE == TMR1_MODE_NORMAL
	TMR1_CurrentMatches++;
	if (TMR1_CurrentMatches >= TMR1_RequiredMatches)
	{
		if (TMR1_OverflowCallback != NULL_PTR)
			TMR1_OverflowCallback();
		TMR1_CurrentMatches = 0;
	}
#endif
}