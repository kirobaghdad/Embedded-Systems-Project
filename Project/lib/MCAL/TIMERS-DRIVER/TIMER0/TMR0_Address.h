#ifndef TMR0_ADDRESS_H_
#define TMR0_ADDRESS_H_

#include "std_types.h"

// Timer0 Registers
#define TCCR0A (*((volatile uint8_t *)0x44)) // Timer/Counter Control Register A
#define TCCR0B (*((volatile uint8_t *)0x45)) // Timer/Counter Control Register B
#define TCNT0 (*((volatile uint8_t *)0x46))  // Timer/Counter Register
#define OCR0A (*((volatile uint8_t *)0x47))  // Output Compare Register A
#define OCR0B (*((volatile uint8_t *)0x48))  // Output Compare Register B
#define TIMSK0 (*((volatile uint8_t *)0x6E)) // Timer Interrupt Mask Register
#define TIFR0 (*((volatile uint8_t *)0x35))  // Timer Interrupt Flag Register

// TCCR0A Bit Positions
#define WGM00 0  // Waveform Generation Mode 0
#define WGM01 1  // Waveform Generation Mode 1
#define COM0B0 4 // Compare Output Mode B0
#define COM0B1 5 // Compare Output Mode B1
#define COM0A0 6 // Compare Output Mode A0
#define COM0A1 7 // Compare Output Mode A1

// TCCR0B Bit Positions
#define CS00 0  // Clock Select 0
#define CS01 1  // Clock Select 1
#define CS02 2  // Clock Select 2
#define WGM02 3 // Waveform Generation Mode 2
#define FOC0B 6 // Force Output Compare B
#define FOC0A 7 // Force Output Compare A

// TIMSK0 Bit Positions
#define TOIE0 0  // Timer Overflow Interrupt Enable
#define OCIE0A 1 // Output Compare A Interrupt Enable
#define OCIE0B 2 // Output Compare B Interrupt Enable

// TIFR0 Bit Positions
#define TOV0 0  // Timer Overflow Flag
#define OCF0A 1 // Output Compare A Flag
#define OCF0B 2 // Output Compare B Flag

#endif