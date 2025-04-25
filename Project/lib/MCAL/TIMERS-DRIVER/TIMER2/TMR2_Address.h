#ifndef TMR2_ADDRESS_H_
#define TMR2_ADDRESS_H_

#include "std_types.h"

// Timer2 Registers
#define TCCR2A (*((volatile uint8_t *)0xB0)) // Timer/Counter Control Register A
#define TCCR2B (*((volatile uint8_t *)0xB1)) // Timer/Counter Control Register B
#define TCNT2 (*((volatile uint8_t *)0xB2))  // Timer/Counter Register
#define OCR2A (*((volatile uint8_t *)0xB3))  // Output Compare Register A
#define OCR2B (*((volatile uint8_t *)0xB4))  // Output Compare Register B
#define TIMSK2 (*((volatile uint8_t *)0x70)) // Timer Interrupt Mask Register
#define TIFR2 (*((volatile uint8_t *)0x37))  // Timer Interrupt Flag Register

// TCCR2A Bit Positions
#define WGM20 0  // Waveform Generation Mode 0
#define WGM21 1  // Waveform Generation Mode 1
#define COM2B0 4 // Compare Output Mode B0
#define COM2B1 5 // Compare Output Mode B1
#define COM2A0 6 // Compare Output Mode A0
#define COM2A1 7 // Compare Output Mode A1

// TCCR2B Bit Positions
#define CS20 0  // Clock Select 0
#define CS21 1  // Clock Select 1
#define CS22 2  // Clock Select 2
#define WGM22 3 // Waveform Generation Mode 2
#define FOC2B 6 // Force Output Compare B
#define FOC2A 7 // Force Output Compare A

// TIMSK2 Bit Positions
#define TOIE2 0  // Timer Overflow Interrupt Enable
#define OCIE2A 1 // Output Compare A Interrupt Enable
#define OCIE2B 2 // Output Compare B Interrupt Enable

// TIFR2 Bit Positions
#define TOV2 0  // Timer Overflow Flag
#define OCF2A 1 // Output Compare A Flag
#define OCF2B 2 // Output Compare B Flag

#endif 