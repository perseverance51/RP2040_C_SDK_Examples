/*
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_I2C_SSD1306.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_I2C_SSD1306.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "ssd1306.h" // Include the SSD1306 library
//#include "font.h" // Include the font8x8 library
#include "acme_5_outlines_font.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

const uint8_t num_chars_per_disp[]={7,7,7,5};
//const uint8_t *fonts[4]= {acme_font, bubblesstandard_font, crackers_font, BMSPA_font};
//const uint8_t *fonts[1]= {font_8x5};
// 板载LED连接的GPIO引脚
#define LED_PIN 25
#define SLEEPTIME 25

/**
 * @brief 初始化I2C接口
 *
 * 该函数用于初始化I2C接口，设置I2C的时钟频率为400KHz，
 * 并将I2C的SDA和SCL引脚设置为I2C功能，同时启用上拉电阻。
 *
 * @param 无
 * @return 无
 */
void i2c_setup() {
    // I2C Initialisation. Using it at 400Khz.
    // 初始化I2C接口，设置I2C的时钟频率为400KHz
    i2c_init(I2C_PORT, 400 * 1000);

    // 将I2C的SDA引脚设置为I2C功能
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);

    // 将I2C的SCL引脚设置为I2C功能
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    // 启用I2C的SDA引脚的上拉电阻
    gpio_pull_up(I2C_SDA_PIN);

    // 启用I2C的SCL引脚的上拉电阻
    gpio_pull_up(I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));//需要包含binary_info.h头文件
}

void animation(void) {
    const char *words[]= {"SSD1306", "DISPLAY", "DRIVER"};

    ssd1306_t disp;
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c0);
    ssd1306_clear(&disp);

    printf("ANIMATION!\n");

    char buf[8];

    for(;;) {
        for(int y=0; y<31; ++y) {
            ssd1306_draw_line(&disp, 0, y, 127, y);
            ssd1306_show(&disp);
            sleep_ms(SLEEPTIME);
            ssd1306_clear(&disp);
        }

        for(int y=0, i=1; y>=0; y+=i) {
            ssd1306_draw_line(&disp, 0, 31-y, 127, 31+y);
            ssd1306_draw_line(&disp, 0, 31+y, 127, 31-y);
            ssd1306_show(&disp);
            sleep_ms(SLEEPTIME);
            ssd1306_clear(&disp);
            if(y==32) i=-1;
        }

        for(int i=0; i<sizeof(words)/sizeof(char *); ++i) {
            ssd1306_draw_string(&disp, 8, 24, 2, words[i]);
            ssd1306_show(&disp);
            sleep_ms(800);
            ssd1306_clear(&disp);
        }

        for(int y=31; y<63; ++y) {
            ssd1306_draw_line(&disp, 0, y, 127, y);
            ssd1306_show(&disp);
            sleep_ms(SLEEPTIME);
            ssd1306_clear(&disp);
        }

              //  ssd1306_draw_string_with_font(&disp, 8, 24, 2, font_8x5, buf);
        ssd1306_draw_string_with_font(&disp, 8, 24, 2, acme_font, (char*)"Hello!");
        ssd1306_draw_string(&disp, 8, 0, 1, "Perseverance51");
                ssd1306_show(&disp);
                sleep_ms(800);
                ssd1306_clear(&disp);

            gpio_put(LED_PIN,!gpio_get(LED_PIN));

       // ssd1306_bmp_show_image(&disp, image_data, image_size);
       // ssd1306_show(&disp);
        sleep_ms(2000);
    }
}

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    i2c_setup();
while (1) {
    //    tight_loop_contents();
        // 翻转LED状态
    //    gpio_put(LED_PIN,!gpio_get(LED_PIN));
        // 等待0.5秒
      //  sleep_ms(2500);
       animation();
    }

//    puts("Hello, world!");

    return 0;
}
