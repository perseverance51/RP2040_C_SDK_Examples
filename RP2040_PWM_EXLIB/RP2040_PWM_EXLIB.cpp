
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_PWM_EXLIB.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_PWM_EXLIB.elf verify reset exit"


*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

#define _PWM_LOGLEVEL_ 3
#include "RP2040_PWM.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25
// creates pwm instance
RP2040_PWM *PWM_Instance;

float frequency = 20000;
float dutyCycle = 60;

#define pinToUse 6

int main()
{
    stdio_init_all();

    // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    // Tell GPIO 6 and 7 they are allocated to the PWM
    //  gpio_set_function(6, GPIO_FUNC_PWM);
    //   gpio_set_function(7, GPIO_FUNC_PWM);
    // assigns pin 6 , with frequency of 20 KHz and a duty cycle of 0%
    PWM_Instance = new RP2040_PWM(pinToUse, 15000, 0);

    while (true)
    {
        PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
                                           // sleep_ms(1000);
                                           // frequency = 20000;
                                           // dutyCycle = 90;
                                           // PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
                                           // sleep_ms(1000);
                                           // dutyCycle = 10;
                                           // PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
        tight_loop_contents();
    }

    //  return 0;
}

uint8_t bStage; //命令序列检测状态
void CheckCustomCmd( uint8_t dat)                   //检测命令序列:"@STCISP#"
{
    switch (bStage++)
    {
    default:
L_Check1st:
    case 0:                                     //检测命令头
        bStage = (dat == '@');
        break;
    case 1:
        if (dat != 'S') goto L_Check1st;
        break;
    case 2:
        if (dat != 'T') goto L_Check1st;
        break;
    case 3:
        if (dat != 'C') goto L_Check1st;
        break;
    case 4:
        if (dat != 'I') goto L_Check1st;
        break;
    case 5:
        if (dat != 'S') goto L_Check1st;
        break;
    case 6:
        if (dat != 'P') goto L_Check1st;
        break;
    case 7:
        if (dat != '#') goto L_Check1st;
        sleep_ms(10);                           //检测到正确的命令序列后复位到系统区,此处的延时可省去
 //       IAP_CONTR = 0x60;                       //复位到系统区
        break;
    }
}
