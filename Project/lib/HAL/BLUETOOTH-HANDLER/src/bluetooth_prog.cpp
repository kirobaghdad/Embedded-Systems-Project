#include <util/delay.h>
#include "GPIO/GPIO_int.h"
#include "USART-DRIVER/USART_Interface.h"
#include "BLUETOOTH-HANDLER/bluetooth_int.h"

#define Bluetooth_RX_PORT (uint8_t)PORT_D
#define Bluetooth_RX_PIN (uint8_t)PIN_0
#define Bluetooth_TX_PORT (uint8_t)PORT_D
#define Bluetooth_TX_PIN (uint8_t)PIN_1

void BLUETOOTH_init()
{
    GPIO_SetPinMode(Bluetooth_RX_PORT, Bluetooth_RX_PIN, INPUT);
    GPIO_SetPinMode(Bluetooth_TX_PORT, Bluetooth_TX_PIN, OUTPUT);

    USART_Init(USART_DEFAULT_BAUD_RATE, USART_CHAR_SIZE_8, USART_PARITY_NONE, USART_STOP_1);
}

void BLUETOOTH_sendChar(uint8_t character)
{
    GPIO_SetPinMode(Bluetooth_RX_PORT, Bluetooth_RX_PIN, INPUT);
    GPIO_SetPinMode(Bluetooth_TX_PORT, Bluetooth_TX_PIN, OUTPUT);

    USART_Transmit(character);
}

void BLUETOOTH_sendString(uint8_t *message)
{
    GPIO_SetPinMode(Bluetooth_RX_PORT, Bluetooth_RX_PIN, INPUT);
    GPIO_SetPinMode(Bluetooth_TX_PORT, Bluetooth_TX_PIN, OUTPUT);

    USART_TransmitString(message, 0);
}

uint8_t BLUETOOTH_receiveChar(void)
{
    GPIO_SetPinMode(Bluetooth_RX_PORT, Bluetooth_RX_PIN, INPUT);
    GPIO_SetPinMode(Bluetooth_TX_PORT, Bluetooth_TX_PIN, OUTPUT);

    return USART_Receive();
}

void BLUETOOTH_receiveString(uint8_t *buffer)
{
    USART_ReceiveString(buffer);
}