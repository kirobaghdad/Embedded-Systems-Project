#include "std_types.h"
#include "CPU_Configuration.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_register.h"

/*
 * Enable or disable global interrupts by setting/clearing the I-bit in SREG
 * mode: ENABLED (1) to enable, DISABLED (0) to disable
 * Returns E_OK on success, E_NOK on invalid mode
 */
uint8_t GLOBAL_INTERRUPT_vidGlobalInterruptEnable(uint8_t mode)
{
	if (mode != ENABLED && mode != DISABLED)
		return E_NOK;

	if (mode == ENABLED)
		SET_BIT(SREG, 7); // Set I-bit
	else
		CLR_BIT(SREG, 7); // Clear I-bit

	return E_OK;
}