#include "USART-DRIVER/USART_Configuration.h"
#include "USART-DRIVER/USART_Interface.h"
#include "USART-DRIVER/USART_Address.h"

void USART_Init(USART_BaudRate_t baud, USART_CharSize_t charSize, USART_Parity_t parity, USART_StopBits_t stopBits)
{
	// Set baud rate
	UBRRL = (uint8_t)baud;
	UBRRH = (uint8_t)(baud >> 8);

	// Enable receiver and transmitter
	SET_BIT(UCSRB, RXEN); // Enable receiver
	SET_BIT(UCSRB, TXEN); // Enable transmitter

	// Set frame format: character size, parity, and stop bits
	SET_BIT(UCSRC, URSEL); // Select UCSRC register
	UCSRC |= (parity << 4) | (stopBits << 3) | (charSize << 1);
}

void USART_Transmit(uint8_t data)
{
	// Wait until the transmit buffer is empty
	while (!(UCSRA & (1 << UDRE)))
		;

	// Put data into the buffer, which sends it
	UDR = data;
}

uint8_t USART_Receive(void)
{
	// Wait for data to be received
	while (!(UCSRA & (1 << RXC)))
		;

	// Get and return received data from buffer
	return UDR;
}

void USART_TransmitString(uint8_t *str)
{
	while ((*str) != NULL_CHAR)
	{
		USART_Transmit((*str));
		str++;
		_delay_ms(30);
	}
	USART_Transmit(NULL_CHAR); // Send null character to indicate end of string
}
