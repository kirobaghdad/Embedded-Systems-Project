// src/main.cpp
#include <Arduino.h>

#define MOV_DELAY 2000
// Motor pin definitions
// RIGHT motors
#define RIGHT_MOTORS_IN1 A2
#define RIGHT_MOTORS_IN2 A3
#define ENA 5

// left motors
#define LEFT_MOTORS_IN1 A1
#define LEFT_MOTORS_IN2 A0
#define ENB 6

// Constants
const int MOTOR_SPEED = 150; // PWM speed (0-255)

// Function declarations
void initMotors();
void moveForward();
void moveBackward();
void moveRight();
void moveLeft();
void stopMotors();

void initMotors()
{
    // Initialize motor pins as outputs
    pinMode(LEFT_MOTORS_IN1, OUTPUT);
    pinMode(LEFT_MOTORS_IN2, OUTPUT);
    pinMode(RIGHT_MOTORS_IN1, OUTPUT);
    pinMode(RIGHT_MOTORS_IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Ensure motors are stopped initially
    stopMotors();
}

void moveForward()
{
    // Left motors forward
    digitalWrite(LEFT_MOTORS_IN1, HIGH);
    digitalWrite(LEFT_MOTORS_IN2, LOW);
    analogWrite(ENA, MOTOR_SPEED);

    // Right motors forward
    digitalWrite(RIGHT_MOTORS_IN1, HIGH);
    digitalWrite(RIGHT_MOTORS_IN2, LOW);
    analogWrite(ENB, MOTOR_SPEED);
}

void moveBackward()
{
    // Left motors backward
    digitalWrite(LEFT_MOTORS_IN1, LOW);
    digitalWrite(LEFT_MOTORS_IN2, HIGH);
    analogWrite(ENA, MOTOR_SPEED);

    // Right motors backward
    digitalWrite(RIGHT_MOTORS_IN1, LOW);
    digitalWrite(RIGHT_MOTORS_IN2, HIGH);
    analogWrite(ENB, MOTOR_SPEED);
}

void moveRight()
{
    // Left motors forward
    digitalWrite(LEFT_MOTORS_IN1, HIGH);
    digitalWrite(LEFT_MOTORS_IN2, LOW);
    analogWrite(ENA, MOTOR_SPEED);

    // Right motors backward
    digitalWrite(RIGHT_MOTORS_IN1, LOW);
    digitalWrite(RIGHT_MOTORS_IN2, HIGH);
    analogWrite(ENB, MOTOR_SPEED);
}

void moveLeft()
{
    // Left motors backward
    digitalWrite(LEFT_MOTORS_IN1, LOW);
    digitalWrite(LEFT_MOTORS_IN2, HIGH);
    analogWrite(ENA, MOTOR_SPEED);

    // Right motors forward
    digitalWrite(RIGHT_MOTORS_IN1, HIGH);
    digitalWrite(RIGHT_MOTORS_IN2, LOW);
    analogWrite(ENB, MOTOR_SPEED);
}

void stopMotors()
{
    // Stop all motors
    digitalWrite(LEFT_MOTORS_IN1, LOW);
    digitalWrite(LEFT_MOTORS_IN2, LOW);
    digitalWrite(RIGHT_MOTORS_IN1, LOW);
    digitalWrite(RIGHT_MOTORS_IN2, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

void setup()
{
    // Initialize Serial for debugging
    Serial.begin(9600);
    initMotors();
    Serial.println("Motor Control Initialized");

    delay(7000); // to remove the wire ;)

    moveForward();
    delay(MOV_DELAY);

    moveBackward();
    delay(MOV_DELAY);

    moveRight();
    delay(MOV_DELAY);

    moveLeft();
    delay(MOV_DELAY);

    stopMotors();
}

void loop()
{
    // Main loop can be used for manual testing
}