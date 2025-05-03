// src/main.cpp
#include <Arduino.h>
#include "std_types.h"
#include <util/delay.h>
#include <arduino.h>
#include "ULTRASONIC-HANDLER/ultrasonic_int.h"
#include "GPIO/GPIO_int.h"
#include "TIMER1/TIMER1_int.h"
#include <arduino.h>
#include <SoftwareSerial.h>
#include <BLUETOOTH-HANDLER/bluetooth_int.h>
#include "std_types.h"
#include <util/delay.h>
#include <arduino.h>
#include "ULTRASONIC-HANDLER/ultrasonic_int.h"
#include "GPIO/GPIO_int.h"
#include "TIMER1/TIMER1_int.h"

# define DISTANCE_THRESHOLD 20
#define DELAY_PER_CM 21
// #define DISTANCE_THRESHOLD 0.5
static ultrasonic_config_t FRONT_SENSOR = {.echo = {PORT_B, PIN_0}, .trigger = {PORT_D, PIN_2}};
static ultrasonic_config_t RIGHT_SENSOR = {.echo = {PORT_B, PIN_0}, .trigger = {PORT_D, PIN_4}};
static ultrasonic_config_t LEFT_SENSOR = {.echo = {PORT_B, PIN_0}, .trigger = {PORT_D, PIN_7}};
static uint8_t PARKING_TYPE = '|';
static Timer1_params timer1_conf = {
    .timer1_mode = TIMER1_ICU_MODE,
    .timer1_prescaler = TIMER1_PRESCALER_64, // Have No idea why
    .compare_val = 255,                      // Dummy but Works for CTC.
    .com_rega = COM_NON_INVERTING,
    .com_regb = COM_NON_INVERTING,
    .OCR1A_val = 175,
    .OCR1B_val = 175,
    .icu_edge = ICU_RISING_EDGE
};

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

static uint8_t parking_side; // 'R' or 'L'
static ultrasonic_config_t* SLOT_SIDE_SENSOR = NULL;// Function declarations
#define PARKING_GAP_MARGIN 5
#define CAR_LENGTH 25
#define CAR_WIDTH 10
void initMotors();
void moveForward();
void moveBackward();
void moveRight();
void moveLeft();
void stopMotors();
void Car_RotateRight90();
void Car_RotateLeft90();

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


void perform_parking() {
    stopMotors();
    _delay_ms(1000);
    if (parking_side == 'R') {
        stopMotors();
        _delay_ms(250);
        Car_RotateLeft90();
        stopMotors();
        _delay_ms(10);
        moveBackward();
        _delay_ms(350);
        if (PARKING_TYPE=='-')
        {
            stopMotors();
        _delay_ms(1000);
            Car_RotateRight90();
        }
        


    } else {
        stopMotors();
        _delay_ms(250);
        Car_RotateRight90();
        stopMotors();
        _delay_ms(10);
        moveBackward();
        _delay_ms(350);
        if (PARKING_TYPE=='-')
            {
                stopMotors();
        _delay_ms(1000);
            Car_RotateLeft90();
            }
    }

    stopMotors();
}
void Car_RotateRight90() {
    moveRight();
    // if(PARKING_TYPE=='|')
    // {
        _delay_ms(490);
    // }
    // else
        // _delay_ms(200);
    stopMotors();
}
void Car_RotateLeft90() {
    moveLeft();
    // if(PARKING_TYPE=='|')
    // {
        _delay_ms(490);
    // }
    // else
    //     _delay_ms(200); // TODO: Should be adjusted based on actual testing
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
void slot_detection_and_parking() {
    uint16_t right_distance = 0;
    uint16_t left_distance = 0;
    uint16_t measured_gap = 0;
    uint8_t gap_found = 0;

    moveForward();

    while (1) {
        GPIO_SetPinValue(PORT_B, PIN_4, LOW);   // Set echo pin to low
        GPIO_SetPinValue(PORT_B, PIN_5, LOW);
        right_distance = Ultrasonic_readDistance(&RIGHT_SENSOR,&timer1_conf);
        GPIO_SetPinValue(PORT_B, PIN_4, HIGH);   // Set echo pin to low
        GPIO_SetPinValue(PORT_B, PIN_5, LOW);
        left_distance = Ultrasonic_readDistance(&LEFT_SENSOR,&timer1_conf);

        if (right_distance > DISTANCE_THRESHOLD) {
            if (!gap_found) {
                gap_found = 1;
                measured_gap = 0;
                parking_side = 'R';
                SLOT_SIDE_SENSOR = &RIGHT_SENSOR;
            }
            
            measured_gap += 12;
            _delay_ms(DELAY_PER_CM);
            // _delay_ms(30);
            if ((PARKING_TYPE == '-' && measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN)/2) ||
    (PARKING_TYPE == '|' && measured_gap >= (CAR_WIDTH + PARKING_GAP_MARGIN)/2)) {
                perform_parking();
                stopMotors();
                break;
            }
        } else if (left_distance > DISTANCE_THRESHOLD) {
            if (!gap_found) {
                gap_found = 1;
                measured_gap = 0;
                parking_side = 'L';
                SLOT_SIDE_SENSOR = &LEFT_SENSOR;
            }
            measured_gap += 12;
            //_delay_ms(30);
            _delay_ms(DELAY_PER_CM);
            if ((PARKING_TYPE == '-' && measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN)/2) ||
    (PARKING_TYPE == '|' && measured_gap >= (CAR_WIDTH + PARKING_GAP_MARGIN)/2)){
                perform_parking();
                stopMotors();
                break;
            }
        } else {
            if (gap_found) {
                if ((PARKING_TYPE == '-' && measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN)/2) ||
    (PARKING_TYPE == '|' && measured_gap >= (CAR_WIDTH + PARKING_GAP_MARGIN)/2)) {
                    stopMotors();
                    perform_parking();
                    stopMotors();
                    // indicate_parking_success();
                    BLUETOOTH_sendChar('p');
                    stopMotors();
                    perform_parking();
                    stopMotors();

                    break;
                } else {
                    gap_found = 0;
                    measured_gap = 0;
                }
            }
            _delay_ms(100);
        }
    }
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
    init();
    BLUETOOTH_init();
    Serial.begin(9600);
    initMotors();
    Serial.println("Motor Control Initialized");
    GPIO_SetPinMode(PORT_B, PIN_4, OUTPUT); // Set echo pin as input
    GPIO_SetPinMode(PORT_B, PIN_5, OUTPUT); // Set echo pin as input
       // Set echo pin to low

    Serial.begin(9600);
    Ultrasonic_init(&FRONT_SENSOR,&timer1_conf);
    Ultrasonic_init(&RIGHT_SENSOR,&timer1_conf);
    Ultrasonic_init(&LEFT_SENSOR,&timer1_conf);
    // delay(7000); // to remove the wire ;)

    /* moveForward();
    delay(MOV_DELAY);

    moveBackward();
    delay(MOV_DELAY);

    moveRight();
    delay(MOV_DELAY);

    moveLeft();
    delay(MOV_DELAY);

    stopMotors(); */
    uint8_t receivedChar = BLUETOOTH_receiveChar();
    while(receivedChar==0)
    {
        receivedChar = BLUETOOTH_receiveChar();
    }

     Serial.println(receivedChar);
     if (receivedChar == 'p')
     { // Only process if data was received
        uint8_t receivedChar = BLUETOOTH_receiveChar();
        while(receivedChar==0)
           {
               receivedChar =BLUETOOTH_receiveChar();
           }
        Serial.println(receivedChar);
         if (receivedChar == 'L')
         {
             PARKING_TYPE = '|';
         }
         else if (receivedChar == 'R')
         {
             PARKING_TYPE = '-';
         }
         else
         {
             Serial.println("Invalid parking type received.");
             return;
         }
         // moveForward(); // Move forward on receiving any character
         slot_detection_and_parking(); // Call the parking function
         // Car_RotateLeft90();
         //_delay_ms(1450); // Delay 300ms
         // Car_RotateRight90();
         stopMotors(); // Stop after moving forward
     }  
}

// 70 cm -- > 1450 n
// 25 --> t
void loop()
{  

}