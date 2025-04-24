#include "SPI-DRIVER/SPI_Configuration.h"
#include "SPI-DRIVER/SPI_Interface.h"
#include "SPI-DRIVER/SPI_Address.h"
#include "DIO-DRIVER/dio_int.h"
void SPI_InitMaster(SPI_Mode_t mode, SPI_DataOrder_t dataOrder, SPI_ClockRate_t clockRate)
{
	// Set MOSI, SCK as output, MISO as input
	DIO_u8SetPinMode(PORT_B, PIN_5, OUTPUT);
	DIO_u8SetPinMode(PORT_B, PIN_7, OUTPUT);
	DIO_u8SetPinMode(PORT_B, PIN_6, INPUT); // MISO as input

	// Set SS as output (must be held high in master mode)
	DIO_u8SetPinMode(PORT_B, PIN_4, OUTPUT);

	// Enable SPI, set as master, configure mode, data order, and clock rate
	SET_BIT(SPCR, SPE);
	SET_BIT(SPCR, MSTR);

	SPCR |= (dataOrder << DORD) | (mode << CPHA) | (clockRate & 0x03);

	// Set SPI2X in SPSR if needed for double speed
	if (clockRate & 0x04)
	{
		SET_BIT(SPSR, SPI2X);
	}
	else
	{
		CLR_BIT(SPSR, SPI2X);
	}
}

void SPI_InitSlave(SPI_Mode_t mode, SPI_DataOrder_t dataOrder)
{
	// Set MISO as output, MOSI, SCK, and SS as input
	DIO_u8SetPinMode(PORT_B, PIN_6, OUTPUT); // MISO as output
	DIO_u8SetPinMode(PORT_B, PIN_5, INPUT);
	DIO_u8SetPinMode(PORT_B, PIN_7, INPUT);
	DIO_u8SetPinMode(PORT_B, PIN_4, INPUT); // MOSI, SCK, and SS as input

	// Enable SPI, set as slave, configure mode, and data order
	SET_BIT(SPCR, SPE);

	SPCR |= (dataOrder << DORD) | (mode << CPHA);
}

void SPI_Transmit(uint8_t data)
{
	// Start transmission by writing data to SPDR
	SPDR = data;

	// Wait for transmission to complete (SPIF set in SPSR)
	while (!(SPSR & (1 << SPIF)))
		;
}

uint8_t SPI_Receive(void)
{
	// Wait for reception to complete (SPIF set in SPSR)
	while (!(SPSR & (1 << SPIF)))
		;

	// Return the received data from SPDR
	return SPDR;
}

uint8_t SPI_Transceive(uint8_t data)
{
	// Load data into the SPI data register
	SPDR = data;

	// Wait for transmission/reception to complete
	while (!(SPSR & (1 << SPIF)))
		;

	// Return the data received during transmission
	return SPDR;
}
