#ifndef GLOBAL_INTERRUPT_INT_H_
#define GLOBAL_INTERRUPT_INT_H_

#include "std_types.h"

/*
 * Modes for enabling/disabling global interrupts
 * Assumed to be defined in std_types.h for consistency
 */
#define ENABLED (uint8_t)1
#define DISABLED (uint8_t)0

/*
 * Function to enable or disable global interrupts
 * Returns E_OK on success, E_NOK on invalid mode
 */
uint8_t GLOBAL_INTERRUPT_vidGlobalInterruptEnable(uint8_t mode);

#endif