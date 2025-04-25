#ifndef GLOBAL_INTERRUPT_REGISTER_H_
#define GLOBAL_INTERRUPT_REGISTER_H_

#include "std_types.h"

#define SREG (*((volatile uint8_t *)0x5F)) // Status Register

#endif