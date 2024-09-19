
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_PWM.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_PWM.elf verify reset exit"

// Output PWM signals on pins 0 and 1
*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

#define UART_ID uart0
#define BAUD_RATE 9600
#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

#define frequency 1000000 // 1MHz
#define MIN_PWM_FREQUENCY (7.5f) // 1MHz
#define freq_CPU 125000000 // 125MHz
  float       _actualFrequency;
  float       _frequency;
  bool        _phaseCorrect;

const uint OUTPUT_PIN = 7;
const uint MEASURE_PIN = 3;

float measure_duty_cycle(uint gpio); // Measure duty cycle of PWM signal on specified GPIO
static void measure_freqs(void);
pwm_config _PWM_config;

  bool calc_TOP_and_DIV(const float freq)
  {
    if (freq > 2000.0)
    {
      _PWM_config.div = 1;
    }
    else if (freq >= 200.0)
    {
      _PWM_config.div = 10;
    }
    else if (freq >= 20.0)
    {
      _PWM_config.div = 100;
    }
    else if (freq >= 10.0)
    {
      _PWM_config.div = 200;
    }
    else if (freq >= ( (float) MIN_PWM_FREQUENCY * freq_CPU / 125000000))
    {
      _PWM_config.div = 255;
    }
    else
    {
    printf("Error, freq must be >=", ( (float) MIN_PWM_FREQUENCY * freq_CPU / 125000000));

      return false;
    }

    // Formula => PWM_Freq = ( F_CPU ) / [ ( TOP + 1 ) * ( DIV + DIV_FRAC/16) ]
    _PWM_config.top = ( freq_CPU / freq / _PWM_config.div ) - 1;

    _actualFrequency = ( freq_CPU  ) / ( (_PWM_config.top + 1) * _PWM_config.div );

    // Compensate half freq if _phaseCorrect
    if (_phaseCorrect)
      _PWM_config.top /= 2;

   // PWM_LOGINFO3("_PWM_config.top =", _PWM_config.top, ", _actualFrequency =", _actualFrequency);

    return true;
  }


int main()
{
    //set_sys_clock_khz(133000, true); // 325us
    stdio_init_all();
    sleep_ms(2500);
    printf("PWM test!\n");

    // uart_init(UART_ID, BAUD_RATE);

    // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    // Tell GPIO 6 and 7 they are allocated to the PWM
    gpio_set_function(6, GPIO_FUNC_PWM);
    gpio_set_function(7, GPIO_FUNC_PWM);
   // calc_TOP_and_DIV(frequency); // Calculate TOP and DIV values for the PWM
  //pwm_config_set_clkdiv_int (&_PWM_config, 125); // Set the clock divider to 125 (125MHz / 125 = 1MHz)

    // Set the PWM period to 4 cycles (0 to 3 inclusive)
    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(6);
    //  pwm_config config = pwm_get_default_config();
    // pwm_config_set_clkdiv(&config, 4.f); // Set the clock divider to 4 (125MHz / 4 = 31.25MHz)
    // pwm_config_set_wrap(&config, 3); // Set the PWM period to 4 cycles (0 to 3 inclusive)
    // pwm_init(slice_num, &config, true); // Initialize the PWM with the given config and start it

 pwm_set_clkdiv	(	slice_num,125); // Set the clock divider to 125 (125MHz / 125 = 1MHz)
    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 999);//1000,000/1000=1KHz
    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 250);//25% duty cycle
    // Set initial B output high for three cycles before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 750);//75% duty cycle
    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    float output_duty_cycle = (float)3.0f/4; // 75% duty cycle

    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
    // correct slice and channel for a given GPIO.

    while (true)
    {
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
         float measured_duty_cycle = measure_duty_cycle(MEASURE_PIN);
        printf("Output duty cycle = %.1f%%, measured input duty cycle = %.1f%%\n",
               output_duty_cycle * 100.f, measured_duty_cycle * 100.f);
        // tight_loop_contents();
    //    measure_freqs();
   }


}

float measure_duty_cycle(uint gpio) {
    // Only the PWM B pins can be used as inputs.
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Count once for every 100 cycles the PWM B input is high
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
    pwm_config_set_clkdiv(&cfg, 100);
    pwm_init(slice_num, &cfg, false);
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    pwm_set_enabled(slice_num, true);
    sleep_ms(10);
    pwm_set_enabled(slice_num, false);
    float counting_rate = clock_get_hz(clk_sys) / 100;
    float max_possible_count = counting_rate * 0.01;
    return pwm_get_counter(slice_num) / max_possible_count;
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
