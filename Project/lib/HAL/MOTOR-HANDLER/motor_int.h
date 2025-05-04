#ifndef MOTOR_INT_H_
#define MOTOR_INT_H_

#include "TIMER0/TIMER0_int.h"
#include "std_types.h"
typedef enum
{
	TIMER0_CallBack_A,
	TIMER0_CallBack_B
} MOTOR_CALLBACK_TYPE;
typedef struct
{
	IO motor_pins[3];
	MOTOR_CALLBACK_TYPE PWM_Callback;
} MOTOR_CONFIG;

void MOTOR_u8MotorInit(MOTOR_CONFIG *motor_config, Timer0_params *timer0_conf);

void MOTOR_u8RightRotate(MOTOR_CONFIG *motor_config, uint8_t motorSpeed, Timer0_params *timer_conf);

void MOTOR_u8LeftRotate(MOTOR_CONFIG *motor_config, uint8_t motorSpeed, Timer0_params *timer_conf);

void MOTOR_u8MotorOff(MOTOR_CONFIG *motor_config, Timer0_params *timer_conf);

void MOTOR_u8SetSpeed(uint8_t duty_cycle, MOTOR_CALLBACK_TYPE callbackType, Timer0_params *timer_conf);

#endif