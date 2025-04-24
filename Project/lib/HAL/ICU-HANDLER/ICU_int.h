#ifndef ICU_INT_H_
#define ICU_INT_H_

#include "std_types.h"
#include "../MCAL/TIMERS-DRIVER/TIMER1/TMR1_Address.h"

typedef enum
{
    ICU_EDGE_FALLING = 0,
    ICU_EDGE_RISING = 1
} ICU_EdgeType;

void ICU_Init(void);
void ICU_DeInit(void);
void ICU_SetEdgeDetection(ICU_EdgeType edge);
uint16_t ICU_GetCaptureValue(void);
void ICU_SetCallback(void (*callbackFunc)(void));
void ICU_ClearTimer(void);
void ICU_EnableInterrupt(void);
void ICU_DisableInterrupt(void);

#endif