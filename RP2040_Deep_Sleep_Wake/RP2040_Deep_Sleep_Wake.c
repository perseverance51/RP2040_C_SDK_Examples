
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_Deep_Sleep_Wake.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_Deep_Sleep_Wake.elf verify reset exit"



*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/time.h"

#define LED_PIN 25
#define EXT_INT_PIN 5

static void measure_freqs(void);

void disable_pll() {
  clock_configure(clk_sys,
    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF,
    0,
    12 * MHZ,
    12 * MHZ);
}

void enable_pll() {
  clock_configure(clk_sys,
    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
    125 * MHZ,
    125 * MHZ);
}
void gpio_irq_handler_core_0()
{
       if (gpio_get_irq_event_mask(EXT_INT_PIN) & GPIO_IRQ_LEVEL_HIGH)
    {
         gpio_acknowledge_irq(EXT_INT_PIN, GPIO_IRQ_LEVEL_HIGH);  //clear irq flag
        for (uint8_t i=0; i<6; i++) {
        gpio_put(PICO_DEFAULT_LED_PIN, false);
        sleep_ms(100);
        gpio_put(PICO_DEFAULT_LED_PIN, true);
        sleep_ms(100);
    }
    }
}
int main() {

   // stdio_init_all();
   sleep_ms(2500);
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  gpio_put(PICO_DEFAULT_LED_PIN, true);
  gpio_init(EXT_INT_PIN);
    gpio_set_dir(EXT_INT_PIN, GPIO_IN);// sio_hw->gpio_oe_set = mask;
    gpio_set_pulls(EXT_INT_PIN, false, true); // 下拉
     // 单独中断
//    gpio_set_irq_enabled(EXT_INT_PIN, GPIO_IRQ_LEVEL_HIGH, true);
//    gpio_add_raw_irq_handler(EXT_INT_PIN, gpio_irq_handler_core_0);
//     irq_set_enabled(IO_IRQ_BANK0, true); // 使能中断控制器
    // gpio_set_input_enabled(EXT_INT_PIN, true);
   // gpio_set_dormant_irq_enabled(EXT_INT_PIN, IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS, true);
   gpio_set_dormant_irq_enabled(EXT_INT_PIN, IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS, true);

    disable_pll();
    xosc_dormant(); // WARNING: This stops the xosc until woken up by an irq
  //  gpio_acknowledge_irq(EXT_INT_PIN, IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS);//清除标志位
    enable_pll();
    stdio_init_all();
  while (true) {


    measure_freqs();
 //  gpio_xor_mask(1ul << PICO_DEFAULT_LED_PIN); // Toggle the LED
    sleep_ms(1000);


  }
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

