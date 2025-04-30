#include <Arduino.h>

// Ultrasonic Sensors
#define ECHO 8
#define SEL_A 12
#define SEL_B 13

// Right sensor
#define TRIG_RIGHT 4

// Left sensor
#define TRIG_LEFT 7

// Front sensor
#define TRIG_FRONT 2

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(ECHO, INPUT);
  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(TRIG_FRONT, OUTPUT);
}

long getDistance(int trigPin)
{
  // Send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse
  long duration = pulseIn(ECHO, HIGH);

  // Calculate distance (in cm)
  long distance = duration * 0.034 / 2;

  return distance;
}

void loop()
{
  long distance;

  // Select Right Sensor (SEL_A = 0, SEL_B = 0)
  digitalWrite(SEL_A, LOW);
  digitalWrite(SEL_B, LOW);
  delay(10); // Small delay for mux settling
  distance = getDistance(TRIG_RIGHT);
  Serial.print("Right Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Select Left Sensor (SEL_A = 1, SEL_B = 0)
  digitalWrite(SEL_A, HIGH);
  digitalWrite(SEL_B, LOW);
  delay(10);
  distance = getDistance(TRIG_LEFT);
  Serial.print("Left Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Select Front Sensor (SEL_A = 0, SEL_B = 1)
  digitalWrite(SEL_A, LOW);
  digitalWrite(SEL_B, HIGH);
  delay(10);
  distance = getDistance(TRIG_FRONT);
  Serial.print("Front Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.println("----------------");
  delay(500); // Delay between measurements
}
