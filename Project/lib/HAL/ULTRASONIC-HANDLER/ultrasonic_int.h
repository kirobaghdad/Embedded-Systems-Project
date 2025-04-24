#ifndef _ULTRASONIC_INT_H_
#define _ULTRASONIC_INT_H_
#include "TYPES.h"
void Ultrasonic_init(void);
void Ultrasonic_Trigger(void);
uint16_t Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void);
// void Ultrasonic_setThreshold(uint16_t threshold);
#endif