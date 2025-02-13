/*
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_I2C_SH1106.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_I2C_SH1106.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "OLED.h"




int main()
{
    stdio_init_all();
    OLED_Init();

    while (1)
    {
        //正常显示
        OLED_Clear();
        OLED_ShowChar(0, 0, 'H', OLED_8X16);
        OLED_ShowString(0, 16, "Hello World!", OLED_8X16);
        OLED_ShowNum(0, 32, 123456, 6, OLED_8X16);
        OLED_ShowSignedNum(0, 48, -123456, 6, OLED_8X16);
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_ShowHexNum(0, 0, 0x78, 2, OLED_8X16);
        OLED_ShowBinNum(0, 16, 0x78, 8, OLED_8X16);
        OLED_ShowFloatNum(0, 32, 123.456, 3, 3, OLED_8X16);
        OLED_ShowChinese(0, 48, "你好世界");
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_ShowImage(0, 0, 32, 32, Lil_jx_32);
        OLED_Printf(0, 48, OLED_8X16, "Hello, %d", 123);
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_DrawPoint(5, 5);
        OLED_DrawLine(0, 10, 50, 10);
        OLED_DrawRectangle(60, 10, 30, 20, OLED_FILLED);
        OLED_DrawTriangle(0, 60, 100, 60, 25, 50, OLED_UNFILLED);
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_DrawCircle(20, 20, 20, OLED_FILLED);
        OLED_DrawEllipse(80, 20, 20, 10, OLED_UNFILLED);
        OLED_DrawArc(100, 20, 20, 0, 90, OLED_UNFILLED);
        OLED_Update();

        sleep_ms(1000);


        //反色显示
        OLED_Clear();
        OLED_ShowChar(0, 0, 'H', OLED_6X8);
        OLED_ShowString(0, 16, "Hello World!", OLED_6X8);
        OLED_ShowNum(0, 32, 123456, 6, OLED_6X8);
        OLED_ShowSignedNum(0, 48, -123456, 6, OLED_6X8);
        OLED_Reverse();
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_ShowHexNum(0, 0, 0x78, 2, OLED_6X8);
        OLED_ShowBinNum(0, 16, 0x78, 8, OLED_6X8);
        OLED_ShowFloatNum(0, 32, 123.456, 3, 3, OLED_6X8);
        OLED_ShowChinese(0, 48, "你好世界");
        OLED_Reverse();
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_ShowImage(0, 0, 32, 32, Lil_jx_32);
        OLED_Printf(0, 48, OLED_6X8, "Hello, %d", 123);
        OLED_Reverse();
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_DrawPoint(5, 5);
        OLED_DrawLine(0, 10, 50, 10);
        OLED_DrawRectangle(60, 10, 30, 20, OLED_FILLED);
        OLED_DrawTriangle(0, 60, 100, 60, 25, 50, OLED_UNFILLED);
        OLED_Reverse();
        OLED_Update();

        sleep_ms(1000);

        OLED_Clear();
        OLED_DrawCircle(20, 20, 20, OLED_FILLED);
        OLED_DrawEllipse(80, 20, 20, 10, OLED_UNFILLED);
        OLED_DrawArc(100, 20, 20, 0, 90, OLED_UNFILLED);
        OLED_Reverse();
        OLED_Update();

        sleep_ms(1000);
    }

   // puts("Hello, world!");

    return 0;
}
