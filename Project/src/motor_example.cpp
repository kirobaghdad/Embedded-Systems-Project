#include <Arduino.h>
#include <util/delay.h>
#include "std_types.h"

extern "C" {
    #include "DIO-DRIVER/dio_int.h"
    #include "MOTOR-HANDLER/motor_int.h"
}

#define CAR_SPEED 50 // Speed of the car (0-100%)

// Motor configurations with PWM callback assignments
static MOTOR_CONFIG motor_config_FL = { { {PORT_B, PIN2}, {PORT_B, PIN0}, {PORT_B, PIN1} }, TIMER1_CallBack };
static MOTOR_CONFIG motor_config_FR = { { {PORT_B, PIN4}, {PORT_B, PIN5}, {PORT_D, PIN3} }, TIMER2_CallBack };
static MOTOR_CONFIG motor_config_RL = { { {PORT_C, PIN0}, {PORT_C, PIN1}, {PORT_B, PIN1} }, TIMER1_CallBack };
static MOTOR_CONFIG motor_config_RR = { { {PORT_D, PIN0}, {PORT_D, PIN1}, {PORT_D, PIN3} }, TIMER2_CallBack };

// ===== Car Movement Functions =====

void Car_Init()
{
    MOTOR_u8MotorInit(&motor_config_FL); 
    MOTOR_u8MotorInit(&motor_config_FR);
    MOTOR_u8MotorInit(&motor_config_RL);
    MOTOR_u8MotorInit(&motor_config_RR);
}

void Car_MoveForward()
{
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED);
}

void Car_MoveBackward()
{
    MOTOR_u8LeftRotate(&motor_config_FL,CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_FR,CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RL,CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RR,CAR_SPEED);
}

void Car_TurnLeft()
{
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED / 2);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED / 2);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED);
}

void Car_TurnRight()
{
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED / 2);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED / 2);
}

void Car_RotateLeftInPlace()
{
    MOTOR_u8LeftRotate(&motor_config_FL,CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RL,CAR_SPEED );
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED);
}

void Car_RotateRightInPlace()
{
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RR, CAR_SPEED);
}

void Car_Stop()
{
    MOTOR_u8MotorOff(&motor_config_FL);
    MOTOR_u8MotorOff(&motor_config_FR);
    MOTOR_u8MotorOff(&motor_config_RL);
    MOTOR_u8MotorOff(&motor_config_RR);
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
