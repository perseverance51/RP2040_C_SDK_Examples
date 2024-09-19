
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_ADC.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_ADC.elf verify reset exit"

    ADC通道：0 - 3  =》 ADC0 - ADC3  GPIO26 - GPIO29
    4通道A内部温度

*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

/* Choose 'C' for Celsius or 'F' for Fahrenheit. */
#define TEMPERATURE_UNITS 'C'

static void measure_freqs(void);
/* References for this implementation:
 * raspberry-pi-pico-c-sdk.pdf, Section '4.1.1. hardware_adc'
 * pico-examples/adc/adc_console/adc_console.c */
float read_onboard_temperature(const char unit)
{

    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    if (unit == 'C')
    {
        return tempC;
    }
    else if (unit == 'F')
    {
        return tempC * 9 / 5 + 32;
    }

    return -1.0f;
}

void ADC_Reading(void)
{

    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);
          adc_select_input(2);//设置通道2
        uint16_t adc_2_raw = adc_read();//读取转换通道转换结果
     //   adc_select_input(3);//设置通道2
     adc_set_round_robin(0x08);

        uint16_t adc_3_raw = adc_read();//读取转换通道转换结果
        printf("Raw1 value: 0x%03x, voltage: %f V\n", adc_2_raw,adc_2_raw * conversion_factor);
        printf("Raw2 value: 0x%03x, voltage: %f V\n", adc_3_raw,adc_3_raw * conversion_factor);
}


int main()
{
    stdio_init_all();

    sleep_ms(2500);
    printf("adc test!\n");
    set_sys_clock_khz(133000, true); // 325us
                                     // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    adc_init();
    // 使能温度传感器
  //  adc_set_temp_sensor_enabled(true);
 //   adc_select_input(4);
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(28);
    adc_gpio_init(29);

    while (true)
    {
        // Read the temperature from the onboard temperature sensor.
      //  float temperature = read_onboard_temperature(TEMPERATURE_UNITS);
     //   printf("Onboard temperature = %.02f %c\n", temperature, TEMPERATURE_UNITS);
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        // tight_loop_contents();
        measure_freqs();
        ADC_Reading();
    }

    return 0;
}

static void measure_freqs(void) {
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

    printf("pll_sys  = %dkHz\n", f_pll_sys);
    printf("pll_usb  = %dkHz\n", f_pll_usb);
    printf("rosc     = %dkHz\n", f_rosc);
    printf("clk_sys  = %dkHz\n", f_clk_sys);
    printf("clk_peri = %dkHz\n", f_clk_peri);
    printf("clk_usb  = %dkHz\n", f_clk_usb);
    printf("clk_adc  = %dkHz\n", f_clk_adc);
    printf("clk_rtc  = %dkHz\n", f_clk_rtc);

    // Can't measure clk_ref / xosc as it is the ref
}
