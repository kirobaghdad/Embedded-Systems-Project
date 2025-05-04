#include "std_types.h"
#include "CPU_Configuration.h"
#include "USART-DRIVER/USART_Interface.h"
#include <HardwareSerial.h>

// Helper macros for validation
#define IS_VALID_BAUD_RATE(rate) ((rate) >= 300 && (rate) <= 115200)
#define IS_VALID_CHAR_SIZE(size) ((size) <= USART_CHAR_SIZE_9)
#define IS_VALID_PARITY(parity) ((parity) <= USART_PARITY_ODD)
#define IS_VALID_STOP_BITS(bits) ((bits) <= USART_STOP_2)
#define IS_VALID_DATA_PTR(ptr) ((ptr) != NULL_PTR)
#define IS_VALID_STRING_PTR(str) ((str) != NULL_PTR)

// UBRR -> USART Baud Rate Register
static uint8_t USART_CalculateUBRR(uint32_t baud_rate, uint16_t *ubrr)
{
	if (!IS_VALID_BAUD_RATE(baud_rate))
		return E_NOK;

	// UBRR = (F_CPU / (multiplier * baud_rate)) - 1
	uint32_t multiplier = USART_DEFAULT_U2X ? 8 : 16;
	*ubrr = (uint16_t)((F_CPU / (multiplier * baud_rate)) - 1);
	return E_OK;
}

uint8_t USART_Init(uint32_t baud_rate, USART_CharSize_t char_size, USART_Parity_t parity, USART_StopBits_t stop_bits)
{
	if (!IS_VALID_BAUD_RATE(baud_rate) || !IS_VALID_CHAR_SIZE(char_size) ||
		!IS_VALID_PARITY(parity) || !IS_VALID_STOP_BITS(stop_bits))
		return E_NOK;

	uint16_t ubrr = 0;
	if (USART_CalculateUBRR(baud_rate, &ubrr) != E_OK)
		return E_NOK;

	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;

	if (USART_DEFAULT_U2X)
		SET_BIT(UCSR0A, U2X0);
	else
		CLR_BIT(UCSR0A, U2X0);

	SET_BIT(UCSR0B, RXEN0);
	SET_BIT(UCSR0B, TXEN0);

	UCSR0C = (parity << UPM00) | (stop_bits << USBS0) | ((char_size & 0x03) << UCSZ00);
	if (char_size == USART_CHAR_SIZE_9)
		SET_BIT(UCSR0B, UCSZ02);
	else
		CLR_BIT(UCSR0B, UCSZ02);

	return E_OK;
}

uint8_t USART_Transmit(uint8_t data)
{
	while (!(UCSR0A & (1 << UDRE0)))
		;

	UDR0 = data;
	return E_OK;
}

uint8_t USART_Receive()
{

	while (!(UCSR0A & (1 << RXC0)))
		;
	uint8_t data = UDR0;
	return data;
}

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