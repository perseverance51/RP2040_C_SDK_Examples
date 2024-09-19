
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_Test.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_Test.elf verify reset exit"

    ADC通道：0 - 3  =》 ADC0 - ADC3  GPIO26 - GPIO29
    4通道A内部温度

*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

volatile bool adc_irq_flag = false;
volatile uint16_t level;

#define SAMPLES 5
// Ideally the signal should be bandwidth limited to sample_frequency/2
#define SAMPLING_FREQUENCY 14000 // Sampling frequency in Hz

volatile  uint16_t sampleBuffer[SAMPLES];

static void ADC_IRQ_FIFO_Callback(void)
{

  //  adc_raw = adc_fifo_get();
    level = adc_fifo_get_level();
    while (level-- > 0)
    {
        sampleBuffer[level] = adc_fifo_get();

    }
    // adc_run(false);
    //  adc_fifo_drain();
    adc_irq_flag = true;
    //  printf("ADC IRQ: %u\n", val);
    //  adc_irq_set_enabled(false);
    //  adc_run(false);
}

void adc()
{
    adc_init();
    adc_gpio_init(26);
    //  adc_set_clkdiv(0); // 125kHz ADC clock
    // adc_set_temp_sensor_enabled(true);
    adc_select_input(0);
    //  adc_set_round_robin(0x0f); // ADC0, ADC1, ADC2, ADC3
    adc_set_round_robin(0b1);
    const uint32_t clock_hz = clock_get_hz(clk_adc);
    const uint32_t target_hz = 500;
    const float divider = 1.0f * clock_hz / target_hz;
    adc_set_clkdiv(divider);
 //adc_set_clkdiv((48000000/SAMPLING_FREQUENCY) - 1);
    //   sleep_ms(1000);
    irq_set_priority(ADC_IRQ_FIFO, PICO_HIGHEST_IRQ_PRIORITY); // Set the priority of the ADC IRQ
    irq_set_enabled(ADC_IRQ_FIFO, true);                       // Enable the ADC IRQ在执行核心上启用/禁用多个中断
    adc_fifo_setup(
        true,  // Write each completed conversion to the sample FIFO
        false, // Enable DMA data request (DREQ)
        SAMPLES,     // DREQ (and IRQ) asserted when at least 1 sample present
        false, // We won't see the ERR bit because of 8 bit reads; disable.
        // Shift each sample to 8 bits when pushing to FIFO [true] Changed to false for 12bit values
        false // Shift each sample to 8 bits when pushing to FIFO
    );
    irq_set_exclusive_handler(ADC_IRQ_FIFO, ADC_IRQ_FIFO_Callback); // Set the callback function for the ADC IRQ

    adc_irq_set_enabled(true);
    adc_fifo_drain();
    adc_run(true);
}

int main()
{
    stdio_init_all();
    uart_init(uart0, 115200); // 115200 baud
    printf("adc test!\n");

    // set_sys_clock_khz(133000, true); // 325us
    // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    adc();

    while (true)
    { // Loop forever
        // gpio_put(BUILTIN_LED, 1); // LED on
        // sleep_ms(500); // 500ms
        // gpio_put(BUILTIN_LED, 0); // LED off
        // sleep_ms(500); // 500ms
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        if (adc_irq_flag)
        {
            //  adc_run(false);
            // printf("adc: input: %u, empty: %d, level: %u, input: %u\n",
            //        adc_get_selected_input(),
            //        adc_fifo_is_empty(),
            //        level,
            //        adc_get_selected_input());
                    // while (level-- > 0)
                    // {
                    // uint16_t    val = adc_fifo_get();
                    //     if (val & 0x1000)
                    //     {
                    //         printf("\t%u: %u (error)\n", level, val);
                    //     }
                    //     else
                    //     {
                    //         printf("\t%u: %u\n", level, val);
                    //     }
                    // }
            //   adc_fifo_drain(); // Drain the FIFO before we start

           // printf("ADC raw: %u, level: %u\n", adc_raw, level);
            // uint adc_channel = adc_get_selected_input();
            // printf("ADC channel: %d\n", adc_channel);

            // adc_select_input(0);       // Change the ADC input
            // adc_set_round_robin(0x01); // Change the ADC inputs
            // hw_set_alias(adc_hw)->inte = 0x80;//Interrupt Enable
            //   adc_fifo_setup(true, false, 1, false, false); // Re-configure the ADC FIFO
            //    irq_set_enabled(ADC_IRQ_FIFO, true); // Enable the ADC IRQ
            //     adc_irq_set_enabled(true);
    for (size_t  i = 0; i < SAMPLES; i++)
    {
       printf("  [%u]: %u", i, sampleBuffer[i]);
        if (i % SAMPLES == 4)
            printf("\n");
    }

            adc_irq_flag = false;
            //  sleep_ms(1000);
        }
    }

    return 0;
}
