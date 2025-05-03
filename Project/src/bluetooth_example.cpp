#include<Arduino.h>
#include "BLUETOOTH-HANDLER/bluetooth_int.h"

int main(void) {
    BLUETOOTH_init();
    // BLUETOOTH_sendString((uint8_t *)"Hello from Arduino!\n");
    // BLUETOOTH_sendChar('A');
    // BLUETOOTH_sendString((uint8_t *)"Bluetooth test complete.\n");
    uint8_t receivedChar = BLUETOOTH_receiveChar();
    Serial.print(receivedChar); // Echo back the received character
    // BLUETOOTH_sendChar(receivedChar); // Echo back the received character via Bluetooth
    // uint8_t buffer[100];
    // // uint8_t length = BLUETOOTH_receiveString(buffer, sizeof(buffer));
    // Serial.print("Received string length: ");
    // Serial.println(length);
    return 0;
}