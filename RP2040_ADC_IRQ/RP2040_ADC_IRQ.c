
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_ADC_IRQ.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_ADC_IRQ.elf verify reset exit"

    ADC通道：0 - 3  =》 ADC0 - ADC3  GPIO26 - GPIO29
    4通道A内部温度

*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/uart.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "pico/unique_id.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

/* Choose 'C' for Celsius or 'F' for Fahrenheit. */
#define TEMPERATURE_UNITS 'C'

volatile bool adc_irq_flag = false;
volatile uint16_t level;
volatile  uint16_t sampleBuffer;

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

static  void ADC_IRQ_FIFO_Callback(void)
{
  //  adc_irq_flag = true;
  level = adc_fifo_get_level();
    sampleBuffer = adc_fifo_get();
         adc_irq_flag = true;
}



int main()
{
    stdio_init_all();
    // stdio_set_translate_crlf(&stdio_usb, false); // Don't replace outgoing chars.
    // while (!stdio_usb_connected()){} // Block until connection to serial port.
    sleep_ms(50);
    sleep_ms(2500);
    printf("adc test!\n");
 //   set_sys_clock_khz(133000, true); // 325us
                                     // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    adc_init();
     adc_set_clkdiv(0);   // 125kHz ADC clock
    //  使能温度传感器
    //   adc_set_temp_sensor_enabled(true);
    //    adc_select_input(4);
    //  Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // adc_gpio_init(27);
    // adc_gpio_init(28);
    // adc_gpio_init(29);
    // adc_set_round_robin(0x01);
    adc_select_input(0); // 设置通道0
                         //   adc_set_round_robin(0b11);
    // const uint32_t clock_hz = clock_get_hz(clk_adc);
    // const uint32_t target_hz = 500;
    // const float divider = 1.0f * clock_hz / target_hz;
    // adc_set_clkdiv(divider);
    adc_fifo_setup(
        true,  // Write each completed conversion to the sample FIFO
        false, // Enable DMA data request (DREQ)
        1,     // DREQ (and IRQ) asserted when at least 1 sample present
        false, // We won't see the ERR bit because of 8 bit reads; disable.
        // Shift each sample to 8 bits when pushing to FIFO [true] Changed to false for 12bit values
        false // Shift each sample to 8 bits when pushing to FIFO
    );

     irq_set_priority(ADC_IRQ_FIFO, PICO_HIGHEST_IRQ_PRIORITY); // Set the priority of the ADC IRQ
    irq_set_exclusive_handler(ADC_IRQ_FIFO, ADC_IRQ_FIFO_Callback);
    irq_set_enabled(ADC_IRQ_FIFO, true); // Enable the ADC IRQ.IRQ在执行核心上启用/禁用多个中断
    adc_irq_set_enabled(true); // Enable the ADC IRQ.
  //  adc_fifo_drain();
    adc_run(true);//启用或禁用自由运行采样模式

    while (true)
    {
        // Read the temperature from the onboard temperature sensor.
        //  float temperature = read_onboard_temperature(TEMPERATURE_UNITS);
        //   printf("Onboard temperature = %.02f %c\n", temperature, TEMPERATURE_UNITS);
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        // tight_loop_contents();
       // measure_freqs();
        if (adc_irq_flag)
        {
                pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);
    printf("Board ID: %08x%08x%08x%08x\n", board_id.id[0], board_id.id[1], board_id.id[2], board_id.id[3]);
            printf("level = %d, sampleBuffer = %d\n", level, sampleBuffer);
          adc_irq_flag = false;
        }
    }

    return 0;
}

static void measure_freqs(void)
{
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
