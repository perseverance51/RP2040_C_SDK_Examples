/*
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_I2C_AT24C02.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_I2C_AT24C02.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

// 板载LED连接的GPIO引脚
#define LED_PIN 25
// I2C defines
// This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

#define AT24C02_ADDR 0x50   // AT24C02的地址

void i2c_setup() {
    // I2C Initialisation. Using it at 100Khz.
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);//开启内部上拉
    gpio_pull_up(I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));
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



// 向AT24C02写入一个字节
void at24c02_write_byte(uint8_t addr, uint8_t data) {
    uint8_t buffer[2];
    buffer[0] = addr;
    buffer[1] = data;
    i2c_write_blocking(I2C_PORT, AT24C02_ADDR, buffer, 2, false);
    sleep_ms(5); // AT24C02写入需要一定时间，这里简单延时等待
}

// 从AT24C02读取一个字节
uint8_t at24c02_read_byte(uint8_t addr) {

    i2c_write_blocking(I2C_PORT, AT24C02_ADDR, &addr,1, true);
    uint8_t data;
    i2c_read_blocking(I2C_PORT, AT24C02_ADDR, &data, 1, false);
    return data;
}

// 从AT24C02读取指定长度的数据
void at24c02_read(uint8_t addr, uint8_t *data, size_t length) {

    i2c_write_blocking(I2C_PORT, AT24C02_ADDR, &addr, 1, true);
    i2c_read_blocking(I2C_PORT, AT24C02_ADDR, data, length, false);
}

int main()
{
    uint8_t EP_data[35];
    stdio_init_all();
    i2c_setup();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
     scan_i2c_addresses();
     at24c02_write_byte(255, 'A');

     while (1) {
        // tight_loop_contents();
        // 翻转LED状态
        gpio_put(LED_PIN,!gpio_get(LED_PIN));
        // 读取AT24C02的状态
        uint8_t data = at24c02_read_byte(255);
        printf("AT24C02 Address 255 data: %c\n", data);
        at24c02_read(0, EP_data, 35);
        printf("EP_data: %s\n", EP_data);

        sleep_ms(1000);

    }
  //  puts("Hello, world!");

    return 0;
}
