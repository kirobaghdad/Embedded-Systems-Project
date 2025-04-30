#include <avr/io.h>
#include "TIMER1/TIMER1_int.h"
#include "GPIO/GPIO_int.h"
#include <Arduino.h>
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
static uint8_t TIMER1_CalculateTicks(Timer1_params *timer1_conf, uint16_t milliseconds, uint16_t *matches)
{
    uint32_t prescaler_val = 1;
    switch (timer1_conf->timer1_prescaler)
    {
    case TIMER1_PRESCALER_NO:
        prescaler_val = 1;
        break;
    case TIMER1_PRESCALER_8:
        prescaler_val = 8;
        break;
    case TIMER1_PRESCALER_64:
        prescaler_val = 64;
        break;
    case TIMER1_PRESCALER_256:
        prescaler_val = 256;
        break;
    case TIMER1_PRESCALER_1024:
        prescaler_val = 1024;
        break;
    }
    uint32_t total_ticks = ((uint32_t)F_CPU / (prescaler_val * 1000)) * milliseconds;
    *matches = total_ticks / 256;
    return total_ticks % 256;
}
void TIMER1_INIT(Timer1_params *timer_conf)
{
    TIMER1_Mode = timer_conf->timer1_mode; // Set the mode
    TCCR1A = 0;
    TCCR1B = 0;
    switch (timer_conf->timer1_mode)
    {
    case TIMER1_NORMAL_MODE:
        CLR_BIT(TCCR1A, WGM00);
        CLR_BIT(TCCR1A, WGM01);
        CLR_BIT(TCCR1B, WGM02); // Normal mode
        SET_BIT(TIMSK1, TOIE1); // Enable overflow interrupt
        break;
    case TIMER1_CTC_MODE:
        CLR_BIT(TCCR1A, WGM00);
        SET_BIT(TCCR1A, WGM01);
        CLR_BIT(TCCR1B, WGM02);        // CTC mode
        OCR0A = timer_conf->OCR1A_val; // Set compare value
        SET_BIT(TIMSK0, OCIE1A);       // Enable compare match A interrupt
        break;
    case TIMER1_FAST_PWM_MODE:
        SET_BIT(TCCR1A, WGM00);
        SET_BIT(TCCR1A, WGM01);
        CLR_BIT(TCCR1B, WGM02); // WGM02:0 = 011, TOP = 0xFF
        switch (timer_conf->com_rega)
        {
        case COM_DISCONNECT:
            CLR_BIT(TCCR1A, COM1A0);
            CLR_BIT(TCCR1A, COM1A1); // COM0A1:0 = 00
            break;
        case COM_NON_INVERTING:
            CLR_BIT(TCCR1A, COM1A0);
            SET_BIT(TCCR1A, COM1A1); // COM0A1:0 = 10
            break;
        case COM_INVERTING:
            SET_BIT(TCCR1A, COM1A0);
            SET_BIT(TCCR1A, COM1A1); // COM0A1:0 = 11
            break;
        }
        switch (timer_conf->com_regb)
        {
        case COM_DISCONNECT:
            CLR_BIT(TCCR1A, COM1B0);
            CLR_BIT(TCCR1A, COM1B1);
            break;
        case COM_NON_INVERTING:
            CLR_BIT(TCCR1A, COM1B0);
            SET_BIT(TCCR1A, COM1B1);
            break;
        case COM_INVERTING:
            SET_BIT(TCCR1A, COM1B0);
            SET_BIT(TCCR1A, COM1B1);
            break;
        }
        OCR1A = timer_conf->OCR1A_val;          // Updated to use OCR1A
        OCR1B = timer_conf->OCR1B_val;          // Updated to use OCR1B
        SET_BIT(TIMSK1, OCIE1A);                // Enable compare match A interrupt
        SET_BIT(TIMSK1, OCIE1B);                // Enable compare match B interrupt
        GPIO_SetPinMode(PORT_D, PIN_6, OUTPUT); // Set OC1A (PD6) as output
        GPIO_SetPinMode(PORT_D, PIN_5, OUTPUT); // Set OC1B (PD5) as output
        break;
    case TIMER1_ICU_MODE:
        CLR_BIT(TCCR1A, WGM10);
        CLR_BIT(TCCR1A, WGM11);
        CLR_BIT(TCCR1B, WGM12);
        CLR_BIT(TCCR1B, WGM13);
        if (timer_conf->icu_edge == ICU_RISING_EDGE)
            SET_BIT(TCCR1B, ICES1);
        else
            CLR_BIT(TCCR1B, ICES1);
        SET_BIT(TIMSK1, ICIE1);

        SET_BIT(TIFR1, ICF1);
        
        GPIO_SetPinMode(PORT_B, PIN_0, INPUT);
        break;
    }
    TCCR1B = (TCCR1B & ~(0x07)) | (timer_conf->timer1_prescaler & 0x07);
    GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}
// Set callback for Normal mode (overflow)
void TIMER1_SetOverflowCallback(void (*callbackFunc)(void))
{

    TIMER1_OverflowCallback = callbackFunc;
}

// Set callback for CTC or PWM mode (OCR0A)
void TIMER1_SetCompareACallback(void (*callbackFunc)(void))
{

    TIMER1_CompareACallback = callbackFunc;
}

// Set callback for PWM mode (OCR0B)
void TIMER1_SetCompareBCallback(void (*callbackFunc)(void))
{

    TIMER1_CompareBCallback = callbackFunc;
}
void TIMER1_SetICUCallback(void (*callbackFunc)(void))
{
    TIMER1_ICUCallback = callbackFunc;
}

// Timer1 Overflow ISR (Normal mode)
ISR(TIMER1_OVF_vect)
{
    if (TIMER1_OverflowCallback != NULL_PTR)
    {
        TIMER1_OverflowCallback();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (TIMER1_CompareACallback != NULL_PTR)
    {
        if (TIMER1_Mode == TIMER1_CTC_MODE)
        {
            TIMER1_CurrentMatches++;
            if (TIMER1_CurrentMatches >= TIMER1_RequiredMatches)
            {
                TIMER1_CompareACallback();
                TIMER1_CurrentMatches = 0; // Reset for next cycle
            }
        }
        else if (TIMER1_Mode == TIMER1_FAST_PWM_MODE)
        {
            TIMER1_CompareACallback(); // Direct callback for PWM
        }
    }
}

// Timer0 Compare Match B ISR (PWM mode)
ISR(TIMER1_COMPB_vect)
{
    if (TIMER1_CompareBCallback != NULL_PTR && TIMER1_Mode == TIMER1_FAST_PWM_MODE)
    {
        TIMER1_CompareBCallback();
    }
}
void TIMER1_Set_PWM_DutyCycle_A(Timer1_params *timer_conf, uint8_t speed)
{
    OCR1A = speed;
    timer_conf->OCR1A_val = speed;
}
void TIMER1_Set_PWM_DutyCycle_B(Timer1_params *timer_conf, uint8_t speed)
{
    OCR1B = speed;
    timer_conf->OCR1B_val = speed;
}
ISR(TIMER1_CAPT_vect)
{
    // Serial.println("ICU Callback Triggered");
    (*TIMER1_ICUCallback)();
}