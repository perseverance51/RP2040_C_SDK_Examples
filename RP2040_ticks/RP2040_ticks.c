
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_ticks.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_ticks.elf verify reset exit"


*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/exception.h"
// #include "hardware/watchdog.h"
#include "hardware/timer.h"
#include "hardware/structs/systick.h"
#include "hardware/clocks.h"
// #include "hardware/uart.h"
// #include "hardware/irq.h"
// #include "hardware/structs/scb.h"
#include <tusb.h>
// #include "hardware/vreg.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

void measure_freqs(void);
void MicrosBlink(uint32_t currentTime);
void disableAllInts();
uint32_t previousBlinkTime;
uint32_t blinkInterval = 1000000; // LED闪烁时间间隔
bool toggle;
uint32_t ST_period = 1000000 - 1;

void init_systick()
{
  systick_hw->csr = 0;   // Disable
  systick_hw->rvr = 999; // (rvr value + 1)/(1 MHz alternate clock for Systick) = 1 mSec
  systick_hw->cvr = 0;   // Clear the count to force initial reload
  systick_hw->csr = 0x3; // Enable Systick (0x1), Enable Exceptions (0x2), Alternate clock (0x4=0)
}

void enableSysTick()
{
                                                                             // 1mS
  systick_hw->rvr = ST_period;                                                                                   // 24 bits
  systick_hw->csr = M0PLUS_SYST_CSR_CLKSOURCE_BITS | M0PLUS_SYST_CSR_ENABLE_BITS | M0PLUS_SYST_CSR_TICKINT_BITS; // enable with processor clock
 // systick_hw->csr = 0x07;  //Enable timer with interrupt
}

void Systick_Handler_Callback(void)
{

    systick_hw->csr = 0x00; // Disable systick
     // systick_hw->csr &= ~M0PLUS_SYST_CSR_ENABLE_BITS; // Disable systick
     systick_hw->rvr = ST_period ; // reload the systick counter
 systick_hw->cvr = 0; // Clear the count to force reload

   // systick_hw->rvr = ST_period; // reload the systick counter
  toggle = (toggle == 1) ? 0 : 1;
  gpio_put(BUILTIN_LED, toggle); // toggle LED
  printf("toggle = %d\n", toggle);

 // systick_hw->csr |= M0PLUS_SYST_CSR_ENABLE_BITS; // Enable systick
    systick_hw->csr = 0x03; // Enable timer with interrupt
}



int main()
{

  // set_sys_clock_khz(133000, true); // 325us
  // set_sys_clock_khz(270000, true);
  set_sys_clock_khz(125000, 0);   //clk_sys125MHz
  stdio_init_all();
  // GPIO initialisation.
  gpio_init(BUILTIN_LED);
  gpio_set_dir(BUILTIN_LED, GPIO_OUT);
  gpio_pull_up(BUILTIN_LED);
  // vreg_set_voltage(VREG_VOLTAGE_1_30);
  sleep_ms(1);

  uint32_t fcpu = clock_get_hz(clk_sys);
  while (!tud_cdc_connected())
  {
    sleep_ms(100);
  }
  printf("tud_cdc_connected()\n");

  systick_hw->csr = 0x5;

  systick_hw->rvr = 0x00FFFFFF;

  uint32_t new, old, t0, t1;
  old = systick_hw->cvr;

  t0 = time_us_32();
  sleep_us(49999);
  new = systick_hw->cvr;
  t1 = time_us_32();

  printf("\n          old_cvr=%d,new_cvr=%d\n", old, new);
  printf("\n          old-new=%d\n", old - new);
  printf("            t1-t0=%d\n", t1 - t0);
  printf("(old-new)/(t1-t0)=%.1f\n", (old - new) / (t1 - t0 * 1.0)); // 频率
  measure_freqs();

   enableSysTick();
  // register the Systick Handler
  exception_set_exclusive_handler(SYSTICK_EXCEPTION, Systick_Handler_Callback);
  irq_set_priority(SYSTICK_EXCEPTION, PICO_HIGHEST_IRQ_PRIORITY); // set systick highest priority
 // disableAllInts();
  while (true)
  {
    // tight_loop_contents();
    // sleep_ms(1000);
    // gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED

    //    uint32_t currentMillis = time_us_32(); // 获取当前时间

    //  MicrosBlink(currentMillis);

    // measure_freqs();
  }

  return 0;
}

void measure_freqs(void)
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

void MicrosBlink(uint32_t currentTime)
{
  // 检查是否到达时间间隔
  if (currentTime - previousBlinkTime >= blinkInterval)
  { // 如果时间间隔达到了
    toggle = (toggle == 1) ? 0 : 1;
    gpio_put(BUILTIN_LED, toggle);
    previousBlinkTime = currentTime; // 将上一次时间复位

    printf("toggle = %d\n", toggle);
  }
  // else if (currentTime-previousBlinkTime <= 0) {   // 如果时间间隔小于0时间溢出
  //   previousBlinkTime = currentTime;
  // }
}

void disableAllInts()
{
  int i;
  for (i = 0; i < 32; i++)
  { // 32 interrupts on NVIC
    irq_set_enabled(i, false);
  }
}
