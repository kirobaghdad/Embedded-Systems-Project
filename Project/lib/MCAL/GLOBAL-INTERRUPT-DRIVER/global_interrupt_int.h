#ifndef GLOBAL_INTERRUPT_INT_H_
#define GLOBAL_INTERRUPT_INT_H_

#include "std_types.h"

#define ENABLED (uint8_t)1
#define DISABLED (uint8_t)0

void GLOBAL_INTERRUPT_vidGlobalInterruptEnable(uint8_t mode);

#endif