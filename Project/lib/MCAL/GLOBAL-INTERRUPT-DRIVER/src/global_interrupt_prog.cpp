#include <avr/io.h>
#include "std_types.h"
#include "CPU_Configuration.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"

void GLOBAL_INTERRUPT_GlobalInterruptEnable(uint8_t mode)
{

	if (mode == ENABLED)
		SET_BIT(SREG, 7); // Set I-bit
	else
		CLR_BIT(SREG, 7); // Clear I-bit
}