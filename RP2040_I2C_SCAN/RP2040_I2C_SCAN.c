/*
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_I2C_SCAN.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_I2C_SCAN.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// 板载LED连接的GPIO引脚
#define LED_PIN 25
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

void i2c_setup() {
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

void scan_i2c_addresses() {
    for (uint8_t addr = 0x03; addr <= 0x7f; addr++) {
        uint8_t data = 0x00;
        int result = i2c_write_blocking(I2C_PORT, addr, &data, 1, false);
        if (result == 1) {
            printf("I2C device found at address 0x%02X\n", addr);
        }
    }
}

int main()
{
    stdio_init_all();
    // 设置LED_PIN为输出模式
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // 初始化I2C
    i2c_setup();
    scan_i2c_addresses();

    while (1) {
        tight_loop_contents();
        // 翻转LED状态
        gpio_put(LED_PIN,!gpio_get(LED_PIN));
        // 等待0.5秒
        sleep_ms(3500);
        scan_i2c_addresses();
    }
 //   puts("Hello, world!");

    return 0;
}
