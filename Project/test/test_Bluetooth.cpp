#include <Arduino.h>
#include <SoftwareSerial.h>

// Define Arduino TX and RX pins for HC-06
#define ARD_RX 10 // Arduino RX -> HC-06 TX
#define ARD_TX 11 // Arduino TX -> HC-06 RX

// Initialize SoftwareSerial for HC-06
SoftwareSerial BTSerial(ARD_RX, ARD_TX); // RX, TX

void setup()
{
    // Start Serial for debugging (USB)
    Serial.begin(9600);
    // Start Bluetooth Serial (HC-06 default baud rate is usually 9600)
    BTSerial.begin(9600);
    Serial.println("HC-06 Test: Send/Receive");
}

void loop()
{
    // Read from HC-06 and send to Serial Monitor
    if (BTSerial.available())
    {
        char received = BTSerial.read();
        Serial.write("Received: ");
        Serial.println(received);
    }

    // Read from Serial Monitor and send to HC-06
    if (Serial.available())
    {
        char toSend = Serial.read();
        BTSerial.write(toSend);
        Serial.print("Sent: ");
        Serial.println(toSend);
    }
}
