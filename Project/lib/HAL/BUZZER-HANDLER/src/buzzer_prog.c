#include "BUZZER-HANDLER/buzzer_int.h"
#include "BUZZER-HANDLER/buzzer_cfg.h"
#include "DIO-DRIVER/dio_int.h"
#include "std_types.h"
#include <util/delay.h>

uint8_t Buzzer_Init(void)
{

    uint8_t error_status = DIO_u8SetPinMode(BUZZER_PORT, BUZZER_PIN, OUTPUT);

    if (error_status == E_OK)
    {
        error_status = DIO_u8SetPinValue(BUZZER_PORT, BUZZER_PIN, BUZZER_OFF_STATE);
    }

    return error_status;
}

uint8_t Buzzer_On(void)
{

    return DIO_u8SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH);
}

uint8_t Buzzer_Off(void)
{

    return DIO_u8SetPinValue(BUZZER_PORT, BUZZER_PIN, BUZZER_OFF_STATE);
}

uint8_t Buzzer_Beep(uint32_t duration_ms)
{
    uint8_t error_status;

    error_status = Buzzer_On();
    if (error_status != E_OK)
    {
        return error_status;
    }

    while (duration_ms > 0)
    {

        uint16_t delay_val = (duration_ms > 65535) ? 65535 : (uint16_t)duration_ms;
        _delay_ms(delay_val);
        duration_ms -= delay_val;
    }

    error_status = Buzzer_Off();

    return error_status;
}