#include "std_types.h"
#include "CPU_Configuration.h"
#include "USART-DRIVER/USART_Configuration.h"
#include "USART-DRIVER/USART_Interface.h"
#include "USART-DRIVER/USART_Address.h"
#include <HardwareSerial.h>

// Helper macros for validation
#define IS_VALID_BAUD_RATE(rate) ((rate) >= 300 && (rate) <= 115200)
#define IS_VALID_CHAR_SIZE(size) ((size) <= USART_CHAR_SIZE_9)
#define IS_VALID_PARITY(parity) ((parity) <= USART_PARITY_ODD)
#define IS_VALID_STOP_BITS(bits) ((bits) <= USART_STOP_2)
#define IS_VALID_DATA_PTR(ptr) ((ptr) != NULL_PTR)
#define IS_VALID_STRING_PTR(str) ((str) != NULL_PTR)

// Calculate UBRR value based on baud rate and U2X0 mode
static uint8_t USART_CalculateUBRR(uint32_t baud_rate, uint16_t *ubrr)
{
	if (!IS_VALID_BAUD_RATE(baud_rate))
		return E_NOK;

	// UBRR = (F_CPU / (multiplier * baud_rate)) - 1
	uint32_t multiplier = USART_DEFAULT_U2X ? 8 : 16;
	*ubrr = (uint16_t)((F_CPU / (multiplier * baud_rate)) - 1);
	return E_OK;
}

/*
 * Initialize USART
 */
uint8_t USART_Init(uint32_t baud_rate, USART_CharSize_t char_size, USART_Parity_t parity, USART_StopBits_t stop_bits)
{
	// Validate inputs
	if (!IS_VALID_BAUD_RATE(baud_rate) || !IS_VALID_CHAR_SIZE(char_size) ||
		!IS_VALID_PARITY(parity) || !IS_VALID_STOP_BITS(stop_bits))
		return E_NOK;

	// Calculate and set baud rate
	uint16_t ubrr = 0;
	if (USART_CalculateUBRR(baud_rate, &ubrr) != E_OK)
		return E_NOK;

	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;

	// Enable double-speed mode if configured
	if (USART_DEFAULT_U2X)
		SET_BIT(UCSR0A, U2X0);
	else
		CLR_BIT(UCSR0A, U2X0);

	// Enable receiver and transmitter
	SET_BIT(UCSR0B, RXEN0);
	SET_BIT(UCSR0B, TXEN0);

	// Set frame format: character size, parity, stop bits
	UCSR0C = (parity << UPM00) | (stop_bits << USBS0) | ((char_size & 0x03) << UCSZ00);
	if (char_size == USART_CHAR_SIZE_9)
		SET_BIT(UCSR0B, UCSZ02);
	else
		CLR_BIT(UCSR0B, UCSZ02);

	return E_OK;
}

/*
 * Transmit a single byte
 */
uint8_t USART_Transmit(uint8_t data)
{
	// Wait until transmit buffer is empty
	while (!(UCSR0A & (1 << UDRE0)))
		;

	// Put data into buffer
	UDR0 = data;
	return E_OK;
}

/*
 * Receive a single byte
 */
uint8_t USART_Receive()
{

	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)))
		;
	// Get received data
	uint8_t data = UDR0;
	return data;
}

/*
 * Transmit a null_NULL_PTR-terminated string
 */
uint8_t USART_TransmitString(uint8_t *str, uint8_t send_null)
{
	if (!IS_VALID_STRING_PTR(str))
		return E_NOK;

	while (*str != NULL_CHAR)
	{
		uint8_t status = USART_Transmit(*str);
		if (status != E_OK)
			return E_NOK;
		str++;
	}

	// Send nullcharacter if requested
	if (send_null)
		return USART_Transmit(NULL_CHAR);

	return E_OK;
}

void USART_ReceiveString(uint8_t *str)
{
	uint8_t i = 0;

	str[i] = USART_Receive();

	while (str[i] != MSG_DELIM)
	{
		i++;
		str[i] = USART_Receive();
	}

	str[i] = '\0';
}