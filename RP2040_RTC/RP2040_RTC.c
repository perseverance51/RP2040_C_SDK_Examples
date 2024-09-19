/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-09-04 23:07:38
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-09-06 15:32:16
 * @FilePath: \RP2040_RTC\RP2040_RTC.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program debugprobe.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_RTC.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
// #include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

// GPIO defines
// Example uses GPIO 2
#define GPIO 25

#define BUILTIN_LED PICO_DEFAULT_LED_PIN

#define ALARM_AT_SECONDS 5
#define ALARM_REPEAT_FOREVER true

volatile bool alarmTriggered = false;
bool setAlarmDone = false;
datetime_t t = {
    .year = 2024,
    .month = 0x09,
    .day = 04,
    .dotw = 3, // 0 is Sunday, so 5 is Friday
    .hour = 23,
    .min = 30,
    .sec = 00};

datetime_t alarmT;

//static volatile bool fired = false;

//////////////////////////////////////////

// This is ISR. Be careful. No Serial.print here.
void rtcCallback(void)
{
  alarmTriggered = true;
}

void set_RTC_Alarm(datetime_t *alarmTime)
{
  rtc_set_alarm(alarmTime, rtcCallback);
}

void setAlarmOnce(uint8_t alarmSeconds)
{
  rtc_get_datetime(&alarmT);

  if (alarmSeconds > alarmT.sec)
    alarmT.sec = alarmSeconds;
  else
  {
    // Alarm in next minute
    alarmT.sec = alarmSeconds;
    alarmT.min += 1;
  }

  set_RTC_Alarm(&alarmT);

  printf("Set One-time Alarm @ alarmSeconds = %d\n", alarmSeconds);
}

void setAlarmRepeat(uint8_t alarmSeconds)
{

  alarmT.min = alarmT.hour = alarmT.day = alarmT.dotw = alarmT.month = alarmT.year = -1;
  alarmT.sec = (t.sec + ALARM_AT_SECONDS)%60;
  set_RTC_Alarm(&alarmT);
 // irq_set_enabled(RTC_IRQ, true);
   printf("Set Repeatitive Alarm @ alarmSeconds = %d\n", alarmT.sec);
}

void setAlarm()
{
#if ALARM_REPEAT_FOREVER
  setAlarmRepeat(ALARM_AT_SECONDS);
#else
  setAlarmOnce(ALARM_AT_SECONDS);
#endif

  setAlarmDone = true;
}
static void alarm_callback(void)
{
  alarmTriggered = true;
  // datetime_t t = {0};
  rtc_get_datetime(&t);
  char datetime_buf[256];
  char *datetime_str = &datetime_buf[0];
  datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  printf("Alarm Fired At %s\n", datetime_str);
  stdio_flush();
  alarmT.min = alarmT.hour = alarmT.day = alarmT.dotw = alarmT.month = alarmT.year = -1;
  alarmT.sec = (t.sec + ALARM_AT_SECONDS)%60;


}
int main()
{
  char datetime_buf[256];
  char *datetime_str = &datetime_buf[0];
  stdio_init_all();
  //set_sys_clock_khz(133000, true); // 324us
  uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
  uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
  printf("clk_rtc  = %dkHz\n", f_clk_rtc);
  printf("clk_sys  = %dkHz\n", f_clk_sys);
  // Set up our UART
  //  uart_init(UART_ID, BAUD_RATE);
  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  //  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  //  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  printf("Hello RTC!\n");

  // char datetime_buf[256];
  // char *datetime_str = &datetime_buf[0];

  // Start on Friday 5th of June 2020 15:45:00

  // Start the RTC
  rtc_init();
  rtc_set_datetime(&t);
  bool rtc_status = rtc_running(); //
  printf("RTC is running = %d\n", rtc_status);

  // rtc_set_alarm (&alarm, &alarm_callback); // Set an alarm for 5 second from now
  // irq_set_enabled(RTC_IRQ, true);
  // irq_set_priority (RTC_IRQ, 1); //设置中断优先级
  //  clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
  //  tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
  sleep_us(64);

  // GPIO initialisation.
  // We will make this GPIO an input, and pull it up by default
  gpio_init(BUILTIN_LED);
  gpio_set_dir(BUILTIN_LED, 1);
  gpio_pull_up(BUILTIN_LED);
  alarmT.min = alarmT.hour = alarmT.day = alarmT.dotw = alarmT.month = alarmT.year = -1;
  alarmT.sec = (t.sec + ALARM_AT_SECONDS)%60;//每间隔5秒钟报警一次
  //alarmT.sec = 6;//每1分钟第6秒报警一次
set_RTC_Alarm(&alarmT);
//setAlarm();
  //rtc_set_alarm(&alarmT, &alarm_callback);
  while (true)
  {
    rtc_get_datetime(&t);
    datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
    printf("\r%s \n", datetime_str);
    stdio_flush();
    sleep_ms(1000);
    gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED

    // if (!setAlarmDone)
    // {
    //   setAlarm();
    //   printf("Set Repeatitive Alarm @ alarmSeconds = %d\n",alarmT.sec );

    // }

    if (alarmTriggered)
    {
      alarmTriggered = false;
      printf("clk_sys = %dkHz,clk_rtc = %dkHz,rtc_status:%d\n", f_clk_sys, rtc_running());
      // rtc_set_alarm(&alarmT, &alarm_callback);
      setAlarm();//配置下一次报警时间
    // printf("Set Repeatitive Alarm @ alarmSeconds = %d\n", alarmT.sec);
    }
  }

  return 0;
}
