#ifndef USART_ADDRESS_H_
#define USART_ADDRESS_H_

#include "std_types.h"

// USART0 Registers
#define UDR0 (*((volatile uint8_t *)0xC6))   // USART I/O Data Register
#define UCSR0A (*((volatile uint8_t *)0xC0)) // USART Control and Status Register A
#define UCSR0B (*((volatile uint8_t *)0xC1)) // USART Control and Status Register B
#define UCSR0C (*((volatile uint8_t *)0xC2)) // USART Control and Status Register C
#define UBRR0L (*((volatile uint8_t *)0xC4)) // USART Baud Rate Register Low
#define UBRR0H (*((volatile uint8_t *)0xC5)) // USART Baud Rate Register High

// UCSR0A Bit Positions
#define RXC0 7  // Receive Complete
#define TXC0 6  // Transmit Complete
#define UDRE0 5 // Data Register Empty
#define FE0 4   // Frame Error
#define DOR0 3  // Data Overrun
#define UPE0 2  // Parity Error
#define U2X0 1  // Double USART Transmission Speed
#define MPCM0 0 // Multi-processor Communication Mode

// UCSR0B Bit Positions
#define RXCIE0 7 // RX Complete Interrupt Enable
#define TXCIE0 6 // TX Complete Interrupt Enable
#define UDRIE0 5 // Data Register Empty Interrupt Enable
#define RXEN0 4  // Receiver Enable
#define TXEN0 3  // Transmitter Enable
#define UCSZ02 2 // Character Size bit 2
#define RXB80 1  // Receive Data Bit 8
#define TXB80 0  // Transmit Data Bit 8

// UCSR0C Bit Positions
#define UMSEL01 7 // USART Mode Select 1
#define UMSEL00 6 // USART Mode Select 0
#define UPM01 5   // Parity Mode 1
#define UPM00 4   // Parity Mode 0
#define USBS0 3   // Stop Bit Select
#define UCSZ01 2  // Character Size bit 1
#define UCSZ00 1  // Character Size bit 0
#define UCPOL0 0  // Clock Polarity (synchronous mode)

#endif 