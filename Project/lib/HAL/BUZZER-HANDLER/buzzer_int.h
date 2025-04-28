#ifndef BUZZER_INT_H_
#define BUZZER_INT_H_

#include "std_types.h" 

uint8_t Buzzer_Init(void);
uint8_t Buzzer_On(void);
uint8_t Buzzer_Off(void);
uint8_t Buzzer_Beep(uint32_t duration_ms);
#endif