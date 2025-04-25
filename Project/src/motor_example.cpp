#include <Arduino.h>
#include <util/delay.h>
#include "std_types.h"

extern "C" {
    #include "DIO-DRIVER/dio_int.h"
    #include "MOTOR-HANDLER/motor_int.h"
}

#define CAR_SPEED 150

MOTOR_PIN motor_FL[3] = {
    {PORT_B, PIN0},  // IN1
    {PORT_B, PIN1},  // IN2
    {PORT_B, PIN2}   // EN
};

MOTOR_PIN motor_FR[3] = {
    {PORT_B, PIN3},  // IN1
    {PORT_B, PIN4},  // IN2
    {PORT_B, PIN5}   // EN
};

MOTOR_PIN motor_RL[3] = {
    {PORT_C, PIN0},  // IN1
    {PORT_C, PIN1},  // IN2
    {PORT_C, PIN2}   // EN
};

MOTOR_PIN motor_RR[3] = {
    {PORT_D, PIN0},  // IN1
    {PORT_D, PIN1},  // IN2
    {PORT_D, PIN2}   // EN
};

// ===== Car Movement Functions =====

void Car_Init()
{
    MOTOR_u8MotorInit(motor_FL); 
    MOTOR_u8MotorInit(motor_FR);
    MOTOR_u8MotorInit(motor_RL);
    MOTOR_u8MotorInit(motor_RR);
}

void Car_MoveForward()
{
    MOTOR_u8RightRotate(motor_FL, CAR_SPEED);
    MOTOR_u8RightRotate(motor_FR, CAR_SPEED);
    MOTOR_u8RightRotate(motor_RL, CAR_SPEED);
    MOTOR_u8RightRotate(motor_RR, CAR_SPEED);
}

void Car_MoveBackward()
{
    MOTOR_u8LeftRotate(CAR_SPEED, motor_FL);
    MOTOR_u8LeftRotate(CAR_SPEED, motor_FR);
    MOTOR_u8LeftRotate(CAR_SPEED, motor_RL);
    MOTOR_u8LeftRotate(CAR_SPEED, motor_RR);
}

void Car_TurnLeft()
{
    MOTOR_u8RightRotate(motor_FL, CAR_SPEED / 2);
    MOTOR_u8RightRotate(motor_FR, CAR_SPEED);
    MOTOR_u8RightRotate(motor_RL, CAR_SPEED / 2);
    MOTOR_u8RightRotate(motor_RR, CAR_SPEED);
}

void Car_TurnRight()
{
    MOTOR_u8RightRotate(motor_FL, CAR_SPEED);
    MOTOR_u8RightRotate(motor_FR, CAR_SPEED / 2);
    MOTOR_u8RightRotate(motor_RL, CAR_SPEED);
    MOTOR_u8RightRotate(motor_RR, CAR_SPEED / 2);
}

void Car_RotateLeftInPlace()
{
    MOTOR_u8LeftRotate(CAR_SPEED, motor_FL);
    MOTOR_u8RightRotate(motor_FR, CAR_SPEED);
    MOTOR_u8LeftRotate(CAR_SPEED, motor_RL);
    MOTOR_u8RightRotate(motor_RR, CAR_SPEED);
}

void Car_RotateRightInPlace()
{
    MOTOR_u8RightRotate(motor_FL, CAR_SPEED);
    MOTOR_u8LeftRotate(CAR_SPEED, motor_FR);
    MOTOR_u8RightRotate(motor_RL, CAR_SPEED);
    MOTOR_u8LeftRotate(CAR_SPEED, motor_RR);
}

void Car_Stop()
{
    MOTOR_u8MotorOff(motor_FL);
    MOTOR_u8MotorOff(motor_FR);
    MOTOR_u8MotorOff(motor_RL);
    MOTOR_u8MotorOff(motor_RR);
}

// ===== Main Function =====

int main(void)
{
    Car_Init();

    while (1)
    {
        Car_MoveForward();
        _delay_ms(2000);

        Car_TurnLeft();
        _delay_ms(1000);

        Car_MoveBackward();
        _delay_ms(2000);

        Car_RotateRightInPlace();
        _delay_ms(1500);

        Car_Stop();
        _delay_ms(2000);
    }
}
