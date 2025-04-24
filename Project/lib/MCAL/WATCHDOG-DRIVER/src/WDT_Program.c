#include "std_types.h"
#include "CPU_Configuration.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "WATCHDOG-DRIVER/WDT_Config.h"
#include "WATCHDOG-DRIVER/WDT_Interface.h"
#include "WATCHDOG-DRIVER/WDT_Address.h"

// Helper macro for timeout validation
#define IS_VALID_TIMEOUT(timeout) (   \
	(timeout) == WDT_TIMEOUT_16MS ||  \
	(timeout) == WDT_TIMEOUT_32MS ||  \
	(timeout) == WDT_TIMEOUT_64MS ||  \
	(timeout) == WDT_TIMEOUT_125MS || \
	(timeout) == WDT_TIMEOUT_250MS || \
	(timeout) == WDT_TIMEOUT_500MS || \
	(timeout) == WDT_TIMEOUT_1S ||    \
	(timeout) == WDT_TIMEOUT_2S ||    \
	(timeout) == WDT_TIMEOUT_4S ||    \
	(timeout) == WDT_TIMEOUT_8S)

/*
 * Initialize the Watchdog Timer
 */
uint8_t WDT_Init(WDT_Timeout_t timeout)
{
	// Validate timeout
	if (!IS_VALID_TIMEOUT(timeout))
		return E_NOK;

	// Disable global interrupts for timed sequence
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(DISABLED);

	// Timed sequence: set WDCE and WDE, then set WDE and prescaler
	SET_BIT(WDTCSR, WDCE);					// Set WDCE to enable change
	SET_BIT(WDTCSR, WDE);					// Set WDE to enable WDT
	WDTCSR = (1 << WDE) | (timeout & 0x27); // Set WDE and prescaler (mask WDP3, WDP2:0)

	// Enable global interrupts
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);

	return E_OK;
}

/*
 * Reset the Watchdog Timer
 */
uint8_t WDT_Reset(void)
{
	// Assembly instruction to reset ("kick") the Watchdog Timer
	__asm__ __volatile__("wdr");
	return E_OK;
}

/*
 * Disable the Watchdog Timer
 */
uint8_t WDT_Disable(void)
{
	// Disable global interrupts for timed sequence
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(DISABLED);

	// Timed sequence: set WDCE and WDE, then clear WDE
	SET_BIT(WDTCSR, WDCE); // Set WDCE to enable change
	SET_BIT(WDTCSR, WDE);  // Set WDE to enable WDT
	WDTCSR = 0x00;		   // Disable WDT

	// Enable global interrupts
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);

	return E_OK;
}