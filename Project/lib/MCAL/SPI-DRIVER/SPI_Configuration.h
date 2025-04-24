#ifndef SPI_CONFIGURATION_H_
#define SPI_CONFIGURATION_H_

#include "CPU_Configuration.h"
#include "DIO-DRIVER/dio_int.h"

// SPI Modes (CPOL and CPHA)
typedef enum
{
	SPI_MODE_0 = 0b00, // CPOL=0, CPHA=0
	SPI_MODE_1 = 0b01, // CPOL=0, CPHA=1
	SPI_MODE_2 = 0b10, // CPOL=1, CPHA=0
	SPI_MODE_3 = 0b11  // CPOL=1, CPHA=1
} SPI_Mode_t;

// SPI Data Order
typedef enum
{
	SPI_MSB_FIRST = 0, // MSB transmitted first
	SPI_LSB_FIRST = 1  // LSB transmitted first
} SPI_DataOrder_t;

// SPI Clock Rate (fosc / divider, with SPI2X)
typedef enum
{
	SPI_CLOCK_DIV2 = 0b000,	 // SPI2X=1, SPR1:0=00
	SPI_CLOCK_DIV4 = 0b100,	 // SPI2X=0, SPR1:0=00
	SPI_CLOCK_DIV8 = 0b001,	 // SPI2X=1, SPR1:0=01
	SPI_CLOCK_DIV16 = 0b101, // SPI2X=0, SPR1:0=01
	SPI_CLOCK_DIV32 = 0b010, // SPI2X=1, SPR1:0=10
	SPI_CLOCK_DIV64 = 0b110, // SPI2X=0, SPR1:0=10
	SPI_CLOCK_DIV128 = 0b111 // SPI2X=0, SPR1:0=11
} SPI_ClockRate_t;

// SPI Pins (fixed on ATmega328P)
#define SPI_SS_PORT PORT_B
#define SPI_SS_PIN PIN_2 // PB2
#define SPI_MOSI_PORT PORT_B
#define SPI_MOSI_PIN PIN_3 // PB3
#define SPI_MISO_PORT PORT_B
#define SPI_MISO_PIN PIN_4 // PB4
#define SPI_SCK_PORT PORT_B
#define SPI_SCK_PIN PIN_5 // PB5

// Default SPI Configuration
#define SPI_DEFAULT_MODE SPI_MODE_0
#define SPI_DEFAULT_DATA_ORDER SPI_MSB_FIRST
#define SPI_DEFAULT_CLOCK_RATE SPI_CLOCK_DIV128

// Master/Slave Modes
#define SPI_MASTER 1
#define SPI_SLAVE 0

#endif /* MCAL_SPI_DRIVER_SPI_CONFIG_H_ */