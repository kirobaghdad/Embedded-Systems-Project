#include <BLUETOOTH-HANDLER/bluetooth_int.h>
int main() {
    BLUETOOTH_init();
    BLUETOOTH_sendString((uint8_t *)"Hello from Arduino!\n");
    BLUETOOTH_sendChar('A');
    BLUETOOTH_sendString((uint8_t *)"Bluetooth test complete.\n");
    uint8_t receivedChar = BLUETOOTH_receiveChar();
    BLUETOOTH_sendChar(receivedChar); // Echo back the received character
    return 0;
}