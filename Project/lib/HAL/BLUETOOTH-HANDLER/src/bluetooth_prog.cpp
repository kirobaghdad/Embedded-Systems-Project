#include <SoftwareSerial.h>
#include "BLUETOOTH-HANDLER/bluetooth_int.h"
#include "BLUETOOTH-HANDLER/bluetooth_cfg.h"
#include "DIO-DRIVER/dio_int.h"

SoftwareSerial bluetoothSerial(11,10); // RX, TX

void BLUETOOTH_init()
{
    DIO_u8SetPinMode(ARD_RX_PORT, ARD_RX_PIN, INPUT);
    DIO_u8SetPinMode(ARD_TX_PORT, ARD_TX_PIN, OUTPUT);
    bluetoothSerial.begin(BLUETOOTH_BAUD_RATE);
}

void BLUETOOTH_sendChar(uint8_t character)
{
    bluetoothSerial.write(character);
}

void BLUETOOTH_sendString(uint8_t *message)
{
    bluetoothSerial.print((char *)message);
}

uint8_t BLUETOOTH_receiveChar(void)
{
    while (!bluetoothSerial.available())
        ;
    return bluetoothSerial.read();
}