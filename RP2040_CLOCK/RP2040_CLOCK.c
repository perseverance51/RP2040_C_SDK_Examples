// This code is used to test the clocks of the RP2040 chip.
/*
时钟倍频参数计算："\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_clocks\scripts\vcocalc.py"
计算方法：vcocalc.py 133

  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_CLOCK.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_RTC.elf verify reset exit"


 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN    // LED is on the same pin as the default LED 25

void measure_freqs(void) {
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

int main()
{
    stdio_init_all();
    sleep_ms(3500);
    printf("RP204 Clock Test\n");
    measure_freqs();
// Change clk_sys to be 48MHz. The simplest way is to take this from PLL_USB
    // which has a source frequency of 48MHz
    // clock_configure(clk_sys,
    //                 CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
    //                 CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
    //                 48 * MHZ,
    //                 48 * MHZ);

    // Turn off PLL sys for good measure
 //   pll_deinit(pll_sys);
// pll_init(pll_sys, 1, 1596 * MHZ, 6, 2);

    // clock_configure(clk_sys,//设置系统时钟，设置源为PLL_SYS，辅助源为CLK_SYS_AUX，目标频率为133MHz
    //                 CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
    //                 CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
    //                 133 * MHZ,//辅助源频率
    //                 133 * MHZ);//目标频率

    // clock_configure(clk_peri,
    //                 0,
    //                 CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
    //                 125 * MHZ,
    //                 125 * MHZ);
     //set_sys_clock_khz(124000, true); // 346us
  //set_sys_clock_khz(126000, true); // 340us
  //set_sys_clock_khz(128000, true); // 335us
  //set_sys_clock_khz(130000, true); // 330us
  //set_sys_clock_khz(131000, true); // 328us
    set_sys_clock_khz(130000, true);// 325us

    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    gpio_init(8);
    gpio_set_dir(8, GPIO_OUT);
        gpio_put(8, 0);
    while(true){

            sleep_ms(1000);
    gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
    gpio_put(8, 1); // Turn on the LED
    sleep_ms(1000);
    gpio_put(8, 0); // Turn off the LED
    measure_freqs();
     __asm volatile ("nop\n");
    }






    return 0;
}
