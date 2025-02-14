/*
  CMSIS-DAPÉÕÂ¼ÃüÁî£ºopenocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_I2C_SH1106_C.elf verify reset exit"

 jlinkÃüÁî: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_I2C_SH1106_C.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "sh1106_i2c.h"
#include "font_inconsolata.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

unsigned char dat[16] = {
    /*'上',0*/
    0x00, 0x00, 0x00, 0xF8, 0x20, 0x20, 0x00, 0x00, 0x00, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x00,
};

void setup_i2c() {
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

int main()
{
    stdio_init_all();
    setup_i2c();
    sh1106_t mydisp;
    SH1106_init(&mydisp, i2c0, 0x3C, 128, 64);
    SH1106_clear(&mydisp);
    while (1)
    {
        // SH1106_drawString(&mydisp, "T: 20.0 ms", 0, 0, 1, inconsolata);
        // SH1106_drawString(&mydisp, "Ton: 10.0 ms", 0, 16, 1, inconsolata);
        // SH1106_drawString(&mydisp, "OFF", 0, 32, 1, inconsolata);
        SH1106_drawString(&mydisp, "Analog", 0, 48, 1, inconsolata);//反色显示
        // SH1106_drawBitmap(&mydisp, 0, 0, 8, 12, bitmap, 1);
        OLED_ShowImage(0, 0, 8, 12, dat); //显示8X12汉字‘上’
        SH1106_draw(&mydisp);
    }



//   puts("Hello, world!");

    return 0;
}
