#include "WATCHDOG-DRIVER/WDT_Config.h"
#include "WATCHDOG-DRIVER/WDT_Interface.h"
#include "WATCHDOG-DRIVER/WDT_Address.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
// Function to enable the Watchdog Timer with a specified timeout
void WDT_Init(uint8_t timeout)
{
	// Disable global interrupts while configuring WDT
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(DISABLED);

	// Start the timed sequence to enable or modify the Watchdog Timer.
	SET_BIT(WDTCR, WDTOE); // Set WDTOE bit to enable the watchdog timer
	SET_BIT(WDTCR, WDE);   // Set WDE bit to enable the watchdog timer

	// Set the watchdog timeout and enable the watchdog timer
	WDTCR |= (timeout & 0x07); // Set WDE and prescaler bits (timeout)

	// Enable global interrupts again
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}

// Function to reset the Watchdog Timer to prevent a system reset
void WDT_Reset(void)
{
	// Assembly instruction to reset ("kick") the Watchdog Timer
	__asm__ __volatile__("wdr");
}

// Function to disable the Watchdog Timer
void WDT_Disable(void)
{
	// Disable global interrupts while disabling WDT
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(DISABLED);

	// Start the timed sequence to disable the Watchdog Timer.
	SET_BIT(WDTCR, WDE);   // Set WDE bit to enable the watchdog timer
	SET_BIT(WDTCR, WDTOE); // Set WDTOE bit to enable the watchdog timer
	// Disable the Watchdog Timer by clearing the WDE bit.
	WDTCR = 0x00;

	// Enable global interrupts again
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}
