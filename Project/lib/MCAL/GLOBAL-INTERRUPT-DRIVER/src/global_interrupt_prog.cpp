#include "std_types.h"
#include "CPU_Configuration.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_register.h"

void GLOBAL_INTERRUPT_vidGlobalInterruptEnable(uint8_t mode)
{

	if (mode == ENABLED)
		SET_BIT(SREG, 7); // Set I-bit
	else
		CLR_BIT(SREG, 7); // Clear I-bit
}