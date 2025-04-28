#ifndef BLUETOOTH_CFG_H_
#define BLUETOOTH_CFG_H_

#include "CPU_Configuration.h"
#include "std_types.h"
#define BLUETOOTH_BAUD_RATE 9600UL // Use UL suffix for unsigned long constant

/*
 * Hardware USART pins on ATmega328P (Arduino Uno):
 * RXD: PD0 (Arduino Pin 0)
 * TXD: PD1 (Arduino Pin 1)
 * These pins are automatically configured by the USART_Init function
 * when hardware USART is used. No need to define them here for manual DIO setup.
 */

/* Optional: Define pins for HC-05 STATE or EN/KEY if you plan to use them
 * Example (adjust pins as per your wiring):
 * #define BLUETOOTH_STATE_PORT   PORT_D // Example: Connect STATE to PD2 (Arduino Pin 2)
 * #define BLUETOOTH_STATE_PIN    PIN_2
 * #define BLUETOOTH_EN_PORT      PORT_D // Example: Connect EN/KEY to PD3 (Arduino Pin 3)
 * #define BLUETOOTH_EN_PIN       PIN_3
 */

#endif /* BLUETOOTH_CFG_H_ */