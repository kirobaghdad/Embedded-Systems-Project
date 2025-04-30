#ifndef TMR0_ADDRESS_H_
#define TMR0_ADDRESS_H_

#include "std_types.h"


#define TCCR0A (*((volatile uint8_t *)0x44)) 
#define TCCR0B (*((volatile uint8_t *)0x45)) 
#define TCNT0 (*((volatile uint8_t *)0x46))  
#define OCR0A (*((volatile uint8_t *)0x47))  
#define OCR0B (*((volatile uint8_t *)0x48))  
#define TIMSK0 (*((volatile uint8_t *)0x6E)) 
#define TIFR0 (*((volatile uint8_t *)0x35))  
#define WGM00 0  
#define WGM01 1  
#define COM0B0 4 
#define COM0B1 5 
#define COM0A0 6 
#define COM0A1 7 
#define CS00 0  
#define CS01 1  
#define CS02 2  
#define WGM02 3 
#define FOC0B 6 
#define FOC0A 7 
#define TOIE0 0  
#define OCIE0A 1 
#define OCIE0B 2 
#define TOV0 0  
#define OCF0A 1 
#define OCF0B 2 

#endif