#ifndef USART_CONFIGURATION_H_
#define USART_CONFIGURATION_H_

#include "CPU_Configuration.h"

// Character Size Options
typedef enum
{
	USART_CHAR_SIZE_5 = 0b000, // 5-bit
	USART_CHAR_SIZE_6 = 0b001, // 6-bit
	USART_CHAR_SIZE_7 = 0b010, // 7-bit
	USART_CHAR_SIZE_8 = 0b011, // 8-bit
	USART_CHAR_SIZE_9 = 0b111  // 9-bit
} USART_CharSize_t;

// Parity Mode Options
typedef enum
{
	USART_PARITY_NONE = 0b00, // No parity
	USART_PARITY_EVEN = 0b10, // Even parity
	USART_PARITY_ODD = 0b11	  // Odd parity
} USART_Parity_t;

// Stop Bit Options
typedef enum
{
	USART_STOP_1 = 0, // 1 stop bit
	USART_STOP_2 = 1  // 2 stop bits
} USART_StopBits_t;

// Default Configuration
#define USART_DEFAULT_BAUD_RATE 9600
#define USART_DEFAULT_CHAR_SIZE USART_CHAR_SIZE_8
#define USART_DEFAULT_PARITY USART_PARITY_NONE
#define USART_DEFAULT_STOP_BITS USART_STOP_1
#define USART_DEFAULT_U2X 0 // Disable double-speed mode by default

#endif