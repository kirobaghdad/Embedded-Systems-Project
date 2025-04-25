#ifndef EXE_INTERRUPT_INT_H_
#define EXE_INTERRUPT_INT_H_
#include "std_types.h"


#define ENABLED (uint8_t)1
#define DISABLED (uint8_t)0


#define INTERRUPT_0 (uint8_t)0 // INT0
#define INTERRUPT_1 (uint8_t)1 // INT1


#define LOW_LEVEL (uint8_t)0
#define LOGICAL_CHANGE (uint8_t)1
#define FALLING_EDGE (uint8_t)2
#define RISING_EDGE (uint8_t)3

/*
 * Function prototypes
 */
void EXE_INTERRUPT_vidExeInterruptInit(uint8_t interrupt, uint8_t interrupt_mode);
void EXE_INTERRUPT_vidExeInterruptEnable(uint8_t interrupt, uint8_t mode);
void EXE_INTERRUPT_vidExeInterruptMode(uint8_t interrupt, uint8_t interrupt_mode);
void EXE_INTERRUPT_vidSetCallBack(uint8_t interrupt, void (*func)(void));
void EXE_INTERRUPT_vidExeInterruptSetPullUpMode(uint8_t interrupt);

#endif 