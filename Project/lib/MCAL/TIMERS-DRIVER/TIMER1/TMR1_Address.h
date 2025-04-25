#ifndef TMR1_ADDRESS_H_
#define TMR1_ADDRESS_H_

#include "std_types.h"

// Timer1 Registers
#define TCCR1A (*((volatile uint8_t *)0x80)) // Timer/Counter Control Register A
#define TCCR1B (*((volatile uint8_t *)0x81)) // Timer/Counter Control Register B
#define TCNT1 (*((volatile uint16_t *)0x84)) // Timer/Counter Register (16-bit)
#define OCR1A (*((volatile uint16_t *)0x88)) // Output Compare Register A
#define OCR1B (*((volatile uint16_t *)0x8A)) // Output Compare Register B
#define ICR1 (*((volatile uint16_t *)0x86))  // Input Capture Register
#define TIMSK1 (*((volatile uint8_t *)0x6F)) // Timer Interrupt Mask Register
#define TIFR1 (*((volatile uint8_t *)0x36))  // Timer Interrupt Flag Register

// TCCR1A Bit Positions
#define COM1A1 7 // Compare Output Mode A1
#define COM1A0 6 // Compare Output Mode A0
#define COM1B1 5 // Compare Output Mode B1
#define COM1B0 4 // Compare Output Mode B0
#define WGM11 1  // Waveform Generation Mode 11
#define WGM10 0  // Waveform Generation Mode 10

// TCCR1B Bit Positions
#define ICNC1 7 // Input Capture Noise Canceler
#define ICES1 6 // Input Capture Edge Select
#define WGM13 4 // Waveform Generation Mode 13
#define WGM12 3 // Waveform Generation Mode 12
#define CS12 2  // Clock Select 2
#define CS11 1  // Clock Select 1
#define CS10 0  // Clock Select 0

// TIMSK1 Bit Positions
#define ICIE1 5  // Input Capture Interrupt Enable
#define OCIE1B 2 // Output Compare B Interrupt Enable
#define OCIE1A 1 // Output Compare A Interrupt Enable
#define TOIE1 0  // Overflow Interrupt Enable

// TIFR1 Bit Positions
#define ICF1 5  // Input Capture Flag
#define OCF1B 2 // Output Compare B Flag
#define OCF1A 1 // Output Compare A Flag
#define TOV1 0  // Overflow Flag

#endif