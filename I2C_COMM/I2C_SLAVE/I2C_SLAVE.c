#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <pico/i2c_slave.h>
#include "hardware/clocks.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // 25号引脚

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

// 从设备的SDA和SCL引脚
static const uint I2C_SLAVE_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // GP4
static const uint I2C_SLAVE_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // GP5

// 从设备的256字节内存
static struct
{
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
    bool data_received; // 标记是否接收到数据
} context;

// I2C从设备的中断处理程序
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    switch (event)
    {
    case I2C_SLAVE_RECEIVE: // 主设备写入数据
        if (!context.mem_address_written)
        {
            // 写入内存地址
            context
                .mem_address = i2c_read_byte_raw(i2c);
            context
                .mem_address_written = true;
        }
        else
        {
            // 写入数据到内存
            uint8_t data = i2c_read_byte_raw(i2c);
            context
            .mem[context.mem_address] = data;
                        context
            .mem_address++;
            context.data_received = true; // 标记接收到数据
                 // 通过串口打印接收到的数据
           // printf("Received data at 0x%02X: 0x%02X ('%c')\n", context.mem_address - 1, data, data);
        }
        break;
    case I2C_SLAVE_REQUEST: // 主设备请求数据
        // 从内存读取数据并发送给主设备
        i2c_write_byte_raw(i2c, context.mem[context.mem_address]);
        context
            .mem_address++;
        break;
    case I2C_SLAVE_FINISH: // 主设备发送Stop/Restart信号
    if (context.data_received) {
        // 数据接收完成，打印接收到的数据
       // printf("Data received (length: %d): ", context.mem_address);
        for (int i = 0; i < context.mem_address; i++) {
            //printf("0x%02X ('%c') ", context.mem[i], context.mem[i]);
            uart_putc(uart0, context.mem[i]); // 使用UART发送数据
        }
        uart_putc(uart0,'\n');

        // 重置标志和内存地址
        context.data_received = false;
        context.mem_address_written = false;
        context.mem_address = 0;
    }
        break;
    default:
        break;
    }
}

// 初始化从设备
static void setup_slave()
{
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
    i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}

int main()
{
    stdio_init_all();
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    puts("I2C Slave Device Initialized");
    setup_slave();

    // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks

    while (true)
    {
       // gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
       // sleep_ms(1000);
        if (context.data_received) {
            // 数据接收完成，打印接收到的数据
            printf("Data received (length: %d): ", context.mem_address);
            for (int i = 0; i < context.mem_address; i++) {
                //printf("0x%02X ('%c') ", context.mem[i], context.mem[i]);
                uart_putc(uart0, context.mem[i]); // 使用UART发送数据
            }
            printf("\n");

            // 重置标志和内存地址
            context.data_received = false;
context.mem_address_written = false;
context.mem_address = 0;
        }
    }
    // 从设备主循环，无需额外操作
    tight_loop_contents();
}
