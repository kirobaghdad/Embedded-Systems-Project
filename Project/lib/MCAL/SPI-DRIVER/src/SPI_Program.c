#include "std_types.h"
#include "DIO-DRIVER/dio_int.h"
#include "SPI-DRIVER/SPI_Configuration.h"
#include "SPI-DRIVER/SPI_Interface.h"
#include "SPI-DRIVER/SPI_Address.h"

// Helper macros for input validation
#define IS_VALID_MODE(mode) ((mode) <= SPI_MODE_3)
#define IS_VALID_DATA_ORDER(order) ((order) <= SPI_LSB_FIRST)
#define IS_VALID_CLOCK_RATE(rate) ((rate) <= SPI_CLOCK_DIV128)
#define IS_VALID_DATA_PTR(ptr) ((ptr) != NULL_PTR)

// Helper function to wait for SPIF and check WCOL
static uint8_t SPI_CheckStatus(void)
{
	while (!(SPSR & (1 << SPIF))) // Wait for SPIF
	{
		asm("NOP");
	}
	return (SPSR & (1 << WCOL)) ? E_NOK : E_OK; // Check write collision
}

/*
 * Initialize SPI in master mode
 */
uint8_t SPI_InitMaster(SPI_Mode_t mode, SPI_DataOrder_t dataOrder, SPI_ClockRate_t clockRate)
{
	if (!IS_VALID_MODE(mode) || !IS_VALID_DATA_ORDER(dataOrder) || !IS_VALID_CLOCK_RATE(clockRate))
		return E_NOK;

	// Initialize SPI pins
	DIO_u8SetPinMode(SPI_MOSI_PORT, SPI_MOSI_PIN, OUTPUT); // MOSI output
	DIO_u8SetPinMode(SPI_SCK_PORT, SPI_SCK_PIN, OUTPUT);   // SCK output
	DIO_u8SetPinMode(SPI_MISO_PORT, SPI_MISO_PIN, INPUT);  // MISO input
	DIO_u8SetPinMode(SPI_SS_PORT, SPI_SS_PIN, OUTPUT);	   // SS output
	DIO_u8SetPinValue(SPI_SS_PORT, SPI_SS_PIN, HIGH);	   // SS high

	// Configure SPCR: enable SPI, master mode, mode, data order, clock rate
	SPCR = (1 << SPE) | (1 << MSTR) | (dataOrder << DORD) | (mode << CPHA) | (clockRate & 0x03);

	// Configure SPI2X in SPSR
	if (clockRate & 0x04) // Check SPI2X bit
		SET_BIT(SPSR, SPI2X);
	else
		CLR_BIT(SPSR, SPI2X);

	return E_OK;
}

/*
 * Initialize SPI in slave mode
 */
uint8_t SPI_InitSlave(SPI_Mode_t mode, SPI_DataOrder_t dataOrder)
{
	if (!IS_VALID_MODE(mode) || !IS_VALID_DATA_ORDER(dataOrder))
		return E_NOK;

	// Initialize SPI pins
	DIO_u8SetPinMode(SPI_MISO_PORT, SPI_MISO_PIN, OUTPUT); // MISO output
	DIO_u8SetPinMode(SPI_MOSI_PORT, SPI_MOSI_PIN, INPUT);  // MOSI input
	DIO_u8SetPinMode(SPI_SCK_PORT, SPI_SCK_PIN, INPUT);	   // SCK input
	DIO_u8SetPinMode(SPI_SS_PORT, SPI_SS_PIN, INPUT);	   // SS input

	// Configure SPCR: enable SPI, slave mode, mode, data order
	SPCR = (1 << SPE) | (dataOrder << DORD) | (mode << CPHA);

	return E_OK;
}

/*
 * Transmit data byte
 */
uint8_t SPI_Transmit(uint8_t data)
{
	SPDR = data; // Start transmission
	return SPI_CheckStatus();
}

/*
 * Receive data byte
 */
uint8_t SPI_Receive(uint8_t *data)
{
	if (!IS_VALID_DATA_PTR(data))
		return E_NOK;

	uint8_t status = SPI_CheckStatus();
	if (status == E_OK)
		*data = SPDR; // Read received data
	return status;
}

/*
 * Transmit and receive data byte
 */
uint8_t SPI_Transceive(uint8_t data, uint8_t *received_data)
{
	if (!IS_VALID_DATA_PTR(received_data))
		return E_NOK;

	SPDR = data; // Start transmission
	uint8_t status = SPI_CheckStatus();
	if (status == E_OK)
		*received_data = SPDR; // Read received data
	return status;
}