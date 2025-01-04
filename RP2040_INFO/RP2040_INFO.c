/*
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_INFO.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_INFO.elf verify reset exit"
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 25


/**
 * 打印当前函数的编译信息
 *
 * 该函数打印当前函数的名称、编译日期、编译时间、文件名、行号、ANSI C标准和编译器版本。
 *
 * @return 无返回值
 */
void INFO_Func() {
    // 打印当前函数的名称
    printf("当前函数: %s\n", __func__);
     //   printf("函数名: %s\n", __FUNCTION__);
    // 打印编译日期
    printf("编译日期: %s\n", __DATE__);
    // 打印编译时间
    printf("编译时间: %s\n", __TIME__);
    // 打印文件名
    printf("文件名: %s\n", __FILE__);
    // 打印行号
    printf("行号: %d\n", __LINE__);

    // 打印ANSI C标准
    printf("ANSI C标准: %d\n", __STDC__);
    // 打印编译器版本
    printf("编译器版本: %s\n", __VERSION__);

}

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (1)
    {
        // tight_loop_contents();
        // 翻转LED状态
        gpio_put(LED_PIN,!gpio_get(LED_PIN));
        INFO_Func();
        sleep_ms(1000);
    }


//    puts("Hello, world!");

    return 0;
}
