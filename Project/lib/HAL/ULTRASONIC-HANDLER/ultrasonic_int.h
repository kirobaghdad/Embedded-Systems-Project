#ifndef ULTRASONIC_INT_H_
#define ULTRASONIC_INT_H_
#include "std_types.h"
#include "TIMER1/TIMER1_int.h"
typedef struct
{
    IO echo;
    IO trigger;
} ultrasonic_config_t;

void Ultrasonic_init(ultrasonic_config_t *ultrasonic_config, Timer1_params *timer1_conf);
void Ultrasonic_Trigger(ultrasonic_config_t *ultrasonic_config);
uint16_t Ultrasonic_readDistance(ultrasonic_config_t *ultrasonic_config, Timer1_params *timer_conf);
void Ultrasonic_edgeProcessing(void);
#endif