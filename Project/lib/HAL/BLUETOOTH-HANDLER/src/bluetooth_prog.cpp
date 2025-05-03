#include <SoftwareSerial.h>
#include "BLUETOOTH-HANDLER/bluetooth_int.h"
#include "BLUETOOTH-HANDLER/bluetooth_cfg.h"
#include "GPIO/GPIO_int.h"
#include<arduino.h>
#include <string>
SoftwareSerial bluetoothSerial(11,10); // RX, TX

void BLUETOOTH_init()
{
    // GPIO_SetPinMode(ARD_RX_PORT, ARD_RX_PIN, INPUT);
    // GPIO_SetPinMode(ARD_TX_PORT, ARD_TX_PIN, OUTPUT);
    Serial.begin(9600); // For debugging on Serial Monitor
    bluetoothSerial.begin(9600);
}

void BLUETOOTH_sendChar(uint8_t character)
{
    bluetoothSerial.write(character);
}

void BLUETOOTH_sendString(uint8_t *message)
{
    bluetoothSerial.print((char *)message);
}

uint8_t BLUETOOTH_receiveChar(void) {
    if (bluetoothSerial.available()) { // Check if data is available
         Serial.println("Waiting for data in the loop...");
        return bluetoothSerial.read(); // Read and return the character
    }
    return 0; 
}

std::string BLUETOOTH_receiveString(void) {
    std::string mesg;

    while(!bluetoothSerial.available())
        ;

    // start receiving string
    while(bluetoothSerial.available())
    {
        mesg += BLUETOOTH_receiveChar();
    }
    
    return mesg; 
}