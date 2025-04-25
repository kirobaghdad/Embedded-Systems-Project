#include "std_types.h"
#include "DIO-DRIVER/dio_int.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "EXE-INTERRUPT-DRIVER/exe_interrupt_register.h"
#include "EXE-INTERRUPT-DRIVER/exe_interrupt_config.h"
#include "EXE-INTERRUPT-DRIVER/exe_interupt_int.h"

typedef struct
{
	uint8_t port;
	uint8_t pin;
} EXE_INTERRUPT_PIN;

EXE_INTERRUPT_PIN EXE_INTERRUPT_PINS[2] = {EXE_INTERRUPT_0, EXE_INTERRUPT_1};

static void (*INT0_Callback)(void) = NULL_PTR;
static void (*INT1_Callback)(void) = NULL_PTR;

#define IS_VALID_INTERRUPT(interrupt) ((interrupt) <= INTERRUPT_1)
#define IS_VALID_MODE(mode) ((mode) <= RISING_EDGE)

/*
 * Initialize the specified interrupt with the given mode
 */
void EXE_INTERRUPT_vidExeInterruptInit(uint8_t interrupt, uint8_t interrupt_mode)
{
	if (!IS_VALID_INTERRUPT(interrupt) || !IS_VALID_MODE(interrupt_mode))
		return;

	// Set pin as input
	DIO_u8SetPinMode(EXE_INTERRUPT_PINS[interrupt].port, EXE_INTERRUPT_PINS[interrupt].pin, INPUT);

	// Configure interrupt mode
	EXE_INTERRUPT_vidExeInterruptMode(interrupt, interrupt_mode);

	// Enable global interrupts and the specific interrupt
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
	EXE_INTERRUPT_vidExeInterruptEnable(interrupt, ENABLED);
}

/*
 * Enable or disable the specified interrupt
 */
void EXE_INTERRUPT_vidExeInterruptEnable(uint8_t interrupt, uint8_t mode)
{
	if (!IS_VALID_INTERRUPT(interrupt) || (mode != ENABLED && mode != DISABLED))
		return;

	switch (interrupt)
	{
	case INTERRUPT_0:
		mode == ENABLED ? SET_BIT(EIMSK, 0) : CLR_BIT(EIMSK, 0); // INT0
		break;
	case INTERRUPT_1:
		mode == ENABLED ? SET_BIT(EIMSK, 1) : CLR_BIT(EIMSK, 1); // INT1
		break;
	}
}

/*
 * Set the trigger mode for the specified interrupt
 */
void EXE_INTERRUPT_vidExeInterruptMode(uint8_t interrupt, uint8_t interrupt_mode)
{
	if (!IS_VALID_INTERRUPT(interrupt) || !IS_VALID_MODE(interrupt_mode))
		return;

	switch (interrupt)
	{
	case INTERRUPT_0:
		MCUCR &= ~(0x03);				  // Clear ISC01:ISC00
		MCUCR |= (interrupt_mode & 0x03); // Set mode (ISC01:ISC00 = mode)
		break;
	case INTERRUPT_1:
		MCUCR &= ~(0x0C);						 // Clear ISC11:ISC10
		MCUCR |= ((interrupt_mode & 0x03) << 2); // Set mode (ISC11:ISC10 = mode)
		break;
	}
}

/*
 * Set the pull-up resistor for the specified interrupt pin
 */
void EXE_INTERRUPT_vidExeInterruptSetPullUpMode(uint8_t interrupt)
{
	if (!IS_VALID_INTERRUPT(interrupt))
		return;

	DIO_u8SetPullUpMode(EXE_INTERRUPT_PINS[interrupt].port, EXE_INTERRUPT_PINS[interrupt].pin, ENABLED);
}

/*
 * Set the callback function for the specified interrupt
 */
void EXE_INTERRUPT_vidSetCallBack(uint8_t interrupt, void (*func)(void))
{
	if (!IS_VALID_INTERRUPT(interrupt) || func == NULL_PTR)
		return;

	switch (interrupt)
	{
	case INTERRUPT_0:
		INT0_Callback = func;
		break;
	case INTERRUPT_1:
		INT1_Callback = func;
		break;
	}
}

/*
 * INT0 Interrupt Service Routine
 */
void __vector_1(void) __attribute__((signal));
void __vector_1(void)
{
	if (INT0_Callback != NULL_PTR)
		INT0_Callback();
}

/*
 * INT1 Interrupt Service Routine
 */
void __vector_2(void) __attribute__((signal));
void __vector_2(void)
{
	if (INT1_Callback != NULL_PTR)
		INT1_Callback();
}