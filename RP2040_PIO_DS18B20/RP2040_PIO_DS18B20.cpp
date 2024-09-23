/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_PIO_DS18B20.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_PIO_DS18B20.elf verify reset exit"

*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "DS1820.pio.h"
#include "pico/time.h"
#include "DS18B20.h"
#include "hardware/adc.h" //读取内部温度需要

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25
#define DS18B20_PIN 7
#define DS18B20_PIO pio0

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
    uint32_t time;
    float temp;

    DS18B20 ds = DS18B20(DS18B20_PIO, DS18B20_PIN);

    stdio_init_all();
    // setup_default_uart();
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, GPIO_OUT);
    gpio_pull_up(BUILTIN_LED);
    adc_init();
    adc_set_temp_sensor_enabled(true);

    sleep_ms(3000);
    printf("GO\n");
    while (1)
    {

        time = to_ms_since_boot(get_absolute_time());
        ds.convert();
        time = to_ms_since_boot(get_absolute_time()) - time;
        printf("Covert time %d ms \n", time);

        sleep_ms(1000);

        time = to_ms_since_boot(get_absolute_time());
        temp = ds.getTemperature();
        time = to_ms_since_boot(get_absolute_time()) - time;
        printf("DS18B20 Temp %f in %d ms\r\n", temp, time);

        printf("Pico Temp was %f\n", getTemp());
        sleep_ms(500);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
    }

    return 0;
}
