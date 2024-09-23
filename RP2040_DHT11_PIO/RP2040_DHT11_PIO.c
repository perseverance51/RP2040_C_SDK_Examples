/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_DHT11_PIO.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_DHT11_PIO.elf verify reset exit"

*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/adc.h" //读取内部温度需要
#include "dht.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

// change this to match your setup
static const dht_model_t DHT_MODEL = DHT11;
static const uint DATA_PIN = 7;//DHT11数据引脚定义

static float celsius_to_fahrenheit(float temperature)
{
    return temperature * (9.0f / 5) + 32;
}

//芯片内部温度读取函数
float getTemp()
{
    const float conversion_factor = 3.3f / (1 << 12);
    adc_select_input(4);

    float v = (float)adc_read() * conversion_factor;
    float t = 27.0 - ((v - 0.706) / 0.001721);

    return t;
}

int main()
{
    stdio_init_all();
    // setup_default_uart();
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, GPIO_OUT);
    gpio_pull_up(BUILTIN_LED);
    adc_init();
    adc_set_temp_sensor_enabled(true);
    puts("\nDHT test");

    dht_t dht;
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */);
    do
    {
        dht_start_measurement(&dht);

        float humidity;
        float temperature_c;
        dht_result_t result = dht_finish_measurement_blocking(&dht, &humidity, &temperature_c);
        if (result == DHT_RESULT_OK)
        {
            printf("%.1f C (%.1f F), %.1f%% humidity\n", temperature_c, celsius_to_fahrenheit(temperature_c), humidity);
        }
        else if (result == DHT_RESULT_TIMEOUT)
        {
            puts("DHT sensor not responding. Please check your wiring.");
        }
        else
        {
            assert(result == DHT_RESULT_BAD_CHECKSUM);
            puts("Bad checksum");
        }
         printf("Pico Temp was %f\n", getTemp()); //打印芯片内部温度
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        sleep_ms(2000);
    } while (true);

    return 0;
}
