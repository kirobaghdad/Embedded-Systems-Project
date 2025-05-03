#include "TIMER0/TIMER0_int.h"
#include "GPIO/GPIO_int.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
static uint8_t TIMER0_CalculateTicks(Timer0_params *timer0_conf, uint16_t milliseconds, uint16_t *matches)
{
    uint32_t prescaler_val = 1;
    switch (timer0_conf->timer0_prescaler)
    {
    case TIMER0_PRESCALER_NO:
        prescaler_val = 1;
        break;
    case TIMER0_PRESCALER_8:
        prescaler_val = 8;
        break;
    case TIMER0_PRESCALER_64:
        prescaler_val = 64;
        break;
    case TIMER0_PRESCALER_256:
        prescaler_val = 256;
        break;
    case TIMER0_PRESCALER_1024:
        prescaler_val = 1024;
        break;
    }
    uint32_t total_ticks = ((uint32_t)F_CPU / (prescaler_val * 1000)) * milliseconds;
    *matches = total_ticks / 256;
    return total_ticks % 256;
}
void TIMER0_INIT(Timer0_params *timer_conf)
{
    TMR0_Mode = timer_conf->timer0_mode; // Set the mode
    TCCR0A = 0;
    TCCR0B = 0;
    switch (timer_conf->timer0_mode)
    {
    case TIMER0_NORMAL_MODE:
        CLR_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02); // Normal mode
        // SET_BIT(TIMSK0, TOIE0); // Enable overflow interrupt
        break;
    case TIMER0_CTC_MODE:
        CLR_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);        // CTC mode
        OCR0A = timer_conf->OCR0A_val; // Set compare value
        SET_BIT(TIMSK0, OCIE0A);       // Enable compare match A interrupt
        break;
    case TIMER0_FAST_PWM_MODE:
        SET_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02); // WGM02:0 = 011, TOP = 0xFF
        switch (timer_conf->com_rega)
        {
        case COM_DISCONNECT:
            CLR_BIT(TCCR0A, COM0A0);
            CLR_BIT(TCCR0A, COM0A1); // COM0A1:0 = 00
            break;
        case COM_NON_INVERTING:
            CLR_BIT(TCCR0A, COM0A0);
            SET_BIT(TCCR0A, COM0A1); // COM0A1:0 = 10
            break;
        case COM_INVERTING:
            SET_BIT(TCCR0A, COM0A0);
            SET_BIT(TCCR0A, COM0A1); // COM0A1:0 = 11
            break;
        }
        switch (timer_conf->com_regb)
        {
        case COM_DISCONNECT:
            CLR_BIT(TCCR0A, COM0B0);
            CLR_BIT(TCCR0A, COM0B1);
            break;
        case COM_NON_INVERTING:
            CLR_BIT(TCCR0A, COM0B0);
            SET_BIT(TCCR0A, COM0B1);
            break;
        case COM_INVERTING:
            SET_BIT(TCCR0A, COM0B0);
            SET_BIT(TCCR0A, COM0B1);
            break;
        }
        OCR0A = timer_conf->OCR0A_val;
        OCR0B = timer_conf->OCR0B_val;
        SET_BIT(TIMSK0, OCIE0A);                // Enable compare match A interrupt
        SET_BIT(TIMSK0, OCIE0B);                // Enable compare match B interrupt
        GPIO_SetPinMode(PORT_D, PIN_6, OUTPUT); // Set OC0A (PD6) as output
        GPIO_SetPinMode(PORT_D, PIN_5, OUTPUT); // Set OC0B (PD5) as output
        break;
    }
    TCCR0B = (TCCR0B & ~(0x07)) | (timer_conf->timer0_prescaler & 0x07);
    GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}
// Set callback for Normal mode (overflow)
void TMR0_SetOverflowCallback(void (*callbackFunc)(void))
{

    TMR0_OverflowCallback = callbackFunc;
}

// Set callback for CTC or PWM mode (OCR0A)
void TMR0_SetCompareACallback(void (*callbackFunc)(void))
{

    TMR0_CompareACallback = callbackFunc;
}

// Set callback for PWM mode (OCR0B)
void TMR0_SetCompareBCallback(void (*callbackFunc)(void))
{

    TMR0_CompareBCallback = callbackFunc;
}

// Timer0 Overflow ISR (Normal mode)
ISR(TIMER0_OVF_vect)
{
    if (TMR0_OverflowCallback != NULL_PTR)
    {
        TMR0_OverflowCallback();
    }
}

ISR(TIMER0_COMPA_vect)
{
    if (TMR0_CompareACallback != NULL_PTR)
    {
        if (TMR0_Mode == TIMER0_CTC_MODE)
        {
            TMR0_CurrentMatches++;
            if (TMR0_CurrentMatches >= TMR0_RequiredMatches)
            {
                TMR0_CompareACallback();
                TMR0_CurrentMatches = 0; // Reset for next cycle
            }
        }
        else if (TMR0_Mode == TIMER0_FAST_PWM_MODE)
        {
            TMR0_CompareACallback(); // Direct callback for PWM
        }
    }
}

// Timer0 Compare Match B ISR (PWM mode)
ISR(TIMER0_COMPB_vect)
{
    if (TMR0_CompareBCallback != NULL_PTR && TMR0_Mode == TIMER0_FAST_PWM_MODE)
    {
        TMR0_CompareBCallback();
    }
}
void TIMER0_Set_PWM_DutyCycle_A(Timer0_params *timer_conf, uint8_t speed)
{
    OCR0A = speed;
    timer_conf->OCR0A_val = speed;
}
void TIMER0_Set_PWM_DutyCycle_B(Timer0_params *timer_conf, uint8_t speed)
{
    OCR0B = timer_conf->OCR0B_val;
    timer_conf->OCR0B_val = speed;
}