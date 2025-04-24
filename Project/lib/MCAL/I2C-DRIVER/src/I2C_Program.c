#include "std_types.h"
#include "DIO-DRIVER/dio_int.h"
#include "I2C-DRIVER/I2C_Configuration.h"
#include "I2C-DRIVER/I2C_Interface.h"
#include "I2C-DRIVER/I2C_Address.h"

// Helper macro to validate inputs
#define IS_VALID_SLAVE_ADDR(addr) ((addr) <= 0x7F) // 7-bit address
#define IS_VALID_PRESCALER(prescaler) ((prescaler) <= I2C_PRESCALER_64)
#define IS_VALID_DATA_PTR(ptr) ((ptr) != NULL_PTR)

// Helper function to wait for TWINT and check status
static uint8_t I2C_CheckStatus(uint8_t expected_status)
{
	while (!(TWCR & (1 << TWINT))) // Wait for TWINT
	{
		asm("NOP");
	}
	uint8_t status = TWSR & 0xF8; // Mask prescaler bits
	return (status == expected_status) ? E_OK : E_NOK;
}

/*
 * Initialize I2C in master mode
 */
uint8_t I2C_InitMaster(I2C_Clock_t clock, I2C_Prescaler_t prescaler)
{
	if (!IS_VALID_PRESCALER(prescaler))
		return E_NOK;

	// Initialize SDA and SCL pins (PC4, PC5)
	DIO_u8SetPinMode(I2C_SDA_PORT, I2C_SDA_PIN, INPUT);
	DIO_u8SetPinMode(I2C_SCL_PORT, I2C_SCL_PIN, INPUT);
	DIO_u8SetPullUpMode(I2C_SDA_PORT, I2C_SDA_PIN, I2C_DEFAULT_PULLUP);
	DIO_u8SetPullUpMode(I2C_SCL_PORT, I2C_SCL_PIN, I2C_DEFAULT_PULLUP);

	// Set clock frequency and prescaler
	TWBR = (uint8_t)clock;
	TWSR = prescaler & 0x03; // TWPS1:TWPS0

	// Enable TWI
	SET_BIT(TWCR, TWEN);

	return E_OK;
}

/*
 * Send START condition
 */
uint8_t I2C_MasterSendStartCondition(void)
{
	SET_BIT(TWCR, TWINT); // Clear TWINT flag
	SET_BIT(TWCR, TWSTA); // Send START condition
	SET_BIT(TWCR, TWEN);  // Enable TWI

	return I2C_CheckStatus(I2C_START_TRANSMITTED);
}

/*
 * Send STOP condition
 */
uint8_t I2C_MasterSendStopCondition(void)
{
	SET_BIT(TWCR, TWINT); // Clear TWINT flag
	SET_BIT(TWCR, TWSTO); // Send STOP condition
	SET_BIT(TWCR, TWEN);  // Enable TWI
	return E_OK;		  // No status check needed for STOP
}

/*
 * Send repeated START condition
 */
uint8_t I2C_MasterSendRepeatedStartCondition(void)
{
	SET_BIT(TWCR, TWINT); // Clear TWINT flag
	SET_BIT(TWCR, TWSTA); // Send repeated START condition
	SET_BIT(TWCR, TWEN);  // Enable TWI
	return I2C_CheckStatus(I2C_REPEATED_START_TRANSMITTED);
}

/*
 * Send slave address with write operation
 */
uint8_t I2C_MasterSendSlaveAddressWithWrite(I2C_SlaveAddress_t slaveAddress)
{
	if (!IS_VALID_SLAVE_ADDR(slaveAddress))
		return E_NOK;

	TWDR = (slaveAddress << 1); // Address + write (0)
	SET_BIT(TWCR, TWINT);		// Clear TWINT flag
	SET_BIT(TWCR, TWEN);		// Enable TWI
	return I2C_CheckStatus(I2C_SLA_W_ACK);
}

/*
 * Send slave address with read operation
 */
uint8_t I2C_MasterSendSlaveAddressWithRead(I2C_SlaveAddress_t slaveAddress)
{
	if (!IS_VALID_SLAVE_ADDR(slaveAddress))
		return E_NOK;

	TWDR = (slaveAddress << 1) | 1; // Address + read (1)
	SET_BIT(TWCR, TWINT);			// Clear TWINT flag
	SET_BIT(TWCR, TWEN);			// Enable TWI
	return I2C_CheckStatus(I2C_SLA_R_ACK);
}

/*
 * Transmit a data byte
 */
uint8_t I2C_MasterTransmitDataByte(uint8_t data)
{
	TWDR = data;
	SET_BIT(TWCR, TWINT); // Clear TWINT flag
	SET_BIT(TWCR, TWEN);  // Enable TWI
	return I2C_CheckStatus(I2C_DATA_TRANSMITTED_ACK);
}

/*
 * Receive a data byte with ACK
 */
uint8_t I2C_MasterReceiveDataByteWithACK(uint8_t *data)
{
	if (!IS_VALID_DATA_PTR(data))
		return E_NOK;

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Enable ACK
	uint8_t status = I2C_CheckStatus(I2C_DATA_RECEIVED_ACK);
	if (status == E_OK)
		*data = TWDR;
	return status;
}

/*
 * Receive a data byte without ACK
 */
uint8_t I2C_MasterReceiveDataByteWithNoACK(uint8_t *data)
{
	if (!IS_VALID_DATA_PTR(data))
		return E_NOK;

	TWCR = (1 << TWINT) | (1 << TWEN); // No ACK
	uint8_t status = I2C_CheckStatus(I2C_DATA_RECEIVED_NO_ACK);
	if (status == E_OK)
		*data = TWDR;
	return status;
}

/*
 * Initialize I2C in slave mode
 */
uint8_t I2C_InitSlave(I2C_SlaveAddress_t slaveAddress)
{
	if (!IS_VALID_SLAVE_ADDR(slaveAddress))
		return E_NOK;

	// Initialize SDA and SCL pins
	DIO_u8SetPinMode(I2C_SDA_PORT, I2C_SDA_PIN, INPUT);
	DIO_u8SetPinMode(I2C_SCL_PORT, I2C_SCL_PIN, INPUT);
	DIO_u8SetPullUpMode(I2C_SDA_PORT, I2C_SDA_PIN, I2C_DEFAULT_PULLUP);
	DIO_u8SetPullUpMode(I2C_SCL_PORT, I2C_SCL_PIN, I2C_DEFAULT_PULLUP);

	// Set slave address (7-bit, shifted left)
	TWAR = (slaveAddress << 1);

	// Enable TWI and ACK
	TWCR = (1 << TWEN) | (1 << TWEA);

	return E_OK;
}

/*
 * Slave transmit data byte
 */
uint8_t I2C_SlaveTransmitDataByte(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Start transmission
	return I2C_CheckStatus(I2C_DATA_TRANSMITTED_ACK);
}

/*
 * Slave receive data byte
 */
uint8_t I2C_SlaveReceiveDataByte(uint8_t *data)
{
	if (!IS_VALID_DATA_PTR(data))
		return E_NOK;

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Start reception
	uint8_t status = I2C_CheckStatus(I2C_DATA_RECEIVED_ACK);
	if (status == E_OK)
		*data = TWDR;
	return status;
}