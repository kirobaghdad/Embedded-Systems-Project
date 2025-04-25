#ifndef SPI_ADDRESS_H_
#define SPI_ADDRESS_H_

#include "std_types.h"

// SPI Registers
#define SPCR (*((volatile uint8_t *)0x4C)) // SPI Control Register
#define SPSR (*((volatile uint8_t *)0x4D)) // SPI Status Register
#define SPDR (*((volatile uint8_t *)0x4E)) // SPI Data Register

// SPCR Bit Positions
#define SPR0 0 // Clock Rate Select 0
#define SPR1 1 // Clock Rate Select 1
#define CPHA 2 // Clock Phase
#define CPOL 3 // Clock Polarity
#define MSTR 4 // Master/Slave Select
#define DORD 5 // Data Order
#define SPE 6  // SPI Enable
#define SPIE 7 // SPI Interrupt Enable

// SPSR Bit Positions
#define SPI2X 0 // Double SPI Speed
#define WCOL 6  // Write Collision Flag
#define SPIF 7  // SPI Interrupt Flag

#endif 