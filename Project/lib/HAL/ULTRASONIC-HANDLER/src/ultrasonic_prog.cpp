#include <util/delay.h>
#include <arduino.h>
#include "ULTRASONIC-HANDLER/ultrasonic_int.h"
#include "ULTRASONIC-HANDLER/ultrasonic_cfg.h"
#include "GPIO/GPIO_int.h"
#include "TIMER1/TIMER1_int.h"

static volatile uint8_t g_edgeCount = 0;
static volatile uint16_t g_T_edge = 0;
static volatile uint8_t g_timeoutOccurred = 0;

void Ultrasonic_init(ultrasonic_config_t *ultrasonic_config, Timer1_params *timer1_conf)
{
    TIMER1_INIT(timer1_conf);

    TIMER1_SetICUCallback(Ultrasonic_edgeProcessing);
    TIMER1_SetOverflowCallback(Ultrasonic_timeoutHandler);

    // Enable Timer1 overflow interrupt
    SET_BIT(TIMSK1, TOIE1);

    GPIO_SetPinMode(ultrasonic_config->trigger.port, ultrasonic_config->trigger.pin, OUTPUT);
}

void Ultrasonic_Trigger(ultrasonic_config_t *ultrasonic_config)
{
    GPIO_SetPinValue(ultrasonic_config->trigger.port, ultrasonic_config->trigger.pin, HIGH);
    _delay_us(TRIGGER_DELAY);
    GPIO_SetPinValue(ultrasonic_config->trigger.port, ultrasonic_config->trigger.pin, LOW);
}

uint16_t Ultrasonic_readDistance(ultrasonic_config_t *ultrasonic_config, Timer1_params *timer1_conf)
{
    g_edgeCount = 0;
    g_timeoutOccurred = 0;

    Ultrasonic_Trigger(ultrasonic_config);

    while (g_edgeCount < 2 && !g_timeoutOccurred)
        ;

    if (g_timeoutOccurred)
    {
        return 0; // Return 0 if timeout occurred
    }

    uint16_t preScaler = 1;

    if (timer1_conf->timer1_prescaler == TIMER1_PRESCALER_NO)
        preScaler = 1;
    else if (timer1_conf->timer1_prescaler == TIMER1_PRESCALER_8)
        preScaler = 8;
    else if (timer1_conf->timer1_prescaler == TIMER1_PRESCALER_64)
        preScaler = 64;
    else if (timer1_conf->timer1_prescaler == TIMER1_PRESCALER_256)
        preScaler = 256;
    else if (timer1_conf->timer1_prescaler == TIMER1_PRESCALER_1024)
        preScaler = 1024;

    float timeInSeconds = (g_T_edge) * ((preScaler / (F_CPU + 0.0)) / 2.0);

    return timeInSeconds * 34300 + 1;
}

void Ultrasonic_edgeProcessing(void)
{
    g_edgeCount++;

    switch (g_edgeCount)
    {
    case 1:
        TCNT1 = 0;              // Clear Timer1 counter
        CLR_BIT(TCCR1B, ICES1); // Toggle edge detection (falling -> rising or vice versa)
        break;
    case 2:
        g_T_edge = ICR1;        // Capture pulse width
        SET_BIT(TCCR1B, ICES1); // Toggle edge detection (rising -> falling)
        break;
    }
}

void Ultrasonic_timeoutHandler(void)
{
    g_timeoutOccurred = 1; // Set timeout flag
}