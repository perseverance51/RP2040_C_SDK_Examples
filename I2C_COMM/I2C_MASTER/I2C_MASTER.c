#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // 25号引脚

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

// 主设备的SDA和SCL引脚
static const uint I2C_MASTER_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // GP4
static const uint I2C_MASTER_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // GP5

// 修改后的 run_master 函数，接收要传输的数据作为参数
static void run_master(const char *data_to_send)
{

    // for (uint8_t mem_address = 0;; mem_address = (mem_address + 32) % 256)
    // {
        // 使用传入的数据
        char msg[32];
        snprintf(msg, sizeof(msg), "%s - 0x%02X", data_to_send, 0x00);
        uint8_t msg_len = strlen(msg);

        uint8_t buf[32];
        buf[0] = 0x00;          // 第一个字节是内存地址
        memcpy(buf + 1, msg, msg_len); // 后续字节是要传输的数据

        // 向从设备写入数据
        // printf("Write at 0x%02X: '%s'\n", mem_address, msg);
        int count = i2c_write_blocking(i2c0, I2C_SLAVE_ADDRESS, buf, 1 + msg_len, false);
        if (count < 0)
        {
            puts("Couldn't write to slave, please check your wiring!");
            return;
        }

        // 从从设备读取数据
        count = i2c_write_blocking(i2c0, I2C_SLAVE_ADDRESS, buf, 1, true); // 发送内存地址

        count = i2c_read_blocking(i2c0, I2C_SLAVE_ADDRESS, buf, msg_len, false); // 读取数据
        buf[count] = '\0';
        printf("Read  at 0x%02X:'%s'\n", 0x00, buf);

        // 验证读取的数据是否正确
        if (memcmp(buf, msg, msg_len) != 0)
        {
            puts("Data mismatch!");
            return;
        }

       // puts("");
        sleep_ms(2000);
  //  }
}

int main()
{
    // 定义要传输的数据
    const char *data_to_send = "Hello from Master";
    stdio_init_all();
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    gpio_init(I2C_MASTER_SDA_PIN);
    gpio_set_function(I2C_MASTER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_MASTER_SDA_PIN);

    gpio_init(I2C_MASTER_SCL_PIN);
    gpio_set_function(I2C_MASTER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_MASTER_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);

    puts("I2C Master Device Initialized");

    while (true)
    {
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        sleep_ms(3500);
        // 调用 run_master 函数并传入数据
        run_master(data_to_send);
        sleep_ms(2500);
    }
}
