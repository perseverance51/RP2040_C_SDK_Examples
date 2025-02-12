#include "OLED.h"

// #include "oledfont.h"
// #include "bmp.h"
// #include "i2c.h"
// #include "driver/i2c_master.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include "esp_log.h"

uint8_t OLED_GRAM[128][8];
uint8_t OLED_GRAMbuf[8][128];

/*********************全局变量*/
// OLED设备句柄
//i2c_master_dev_handle_t dev_handle = NULL;
// I2C总线句柄
//i2c_master_bus_handle_t bus_handle = NULL;

const uint8_t MAX_X = X_WIDTH - 1;
const uint8_t MAX_Y = Y_WIDTH - 1;

/**
 * 函    数：OLED写命令
 * 参    数：Command 要写入的命令值，范围：0x00-0xFF
 * 返 回 值：无
 */
void OLED_WriteCommand(uint8_t Command)
{
  uint8_t writebuffer[2];

  writebuffer[0] = 0x00;
  writebuffer[1] = Command;
 //i2c_master_transmit(dev_handle, writebuffer, 2, -1);
 // 使用i2c_write_blocking替代i2c_master_transmit
 i2c_write_blocking(i2c0, OLED_I2C_ADDRESS, writebuffer, 2, false);
}

/**
 * 函    数：OLED写数据
 * 参    数：Data 要写入数据的起始地址
 * 参    数：Count 要写入数据的数量
 * 返 回 值：无
 */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
  uint8_t i;
  uint8_t writebuffer[Count + 1];

  writebuffer[0] = 0x40;

  for (i = 0; i < Count; i++)
  {
    writebuffer[i + 1] = Data[i];
  }
 // ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, writebuffer, Count + 1, -1));
  i2c_write_blocking(i2c0, OLED_I2C_ADDRESS, writebuffer,  Count + 1, false);
}

uint32_t oled_pow(uint8_t m, uint8_t n)
{
  uint32_t result = 1;

  while (n--)
    result *= m;

  return result;
}

uint8_t check_num_len(uint32_t num)
{
  uint32_t tmp;
  uint8_t i;
  for (i = 1; i < 10; i++)
  {
    tmp = oled_pow(10, i);
    if (num < tmp)
    {
      return i;
    }
  }
  return 0;
}



void OLED_Init(void)
{
  // 配置I2C总线
 // I2C Initialisation. Using it at 400Khz.
 i2c_init(I2C_PORT, 400*1000);

 gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
 gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
 gpio_pull_up(I2C_SDA);
 gpio_pull_up(I2C_SCL);
 // Make the I2C pins available to picotool
 bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
  // 初始化OLED
uint8_t OLED_Init_CMD[] = {
  0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8,
  0x3F, 0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1,
  0xDA, 0x12, 0xDB, 0x30, 0x8D, 0x14, 0xAF, 0x20, 0x00};
  for (size_t i = 0; i < sizeof(OLED_Init_CMD)/ sizeof(OLED_Init_CMD[0]); i++)
  {
    OLED_WriteCommand(OLED_Init_CMD[i]);
  }

 // OLED_WriteData(OLED_Init_CMD, 29);
 /*写入一系列的命令，对OLED进行初始化配置*/
 /*
 OLED_WriteCommand(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启

 OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
 OLED_WriteCommand(0x80);	//0x00-0xFF

 OLED_WriteCommand(0xA8);	//设置多路复用率
 OLED_WriteCommand(0x3F);	//0x0E-0x3F

 OLED_WriteCommand(0xD3);	//设置显示偏移
 OLED_WriteCommand(0x00);	//0x00-0x7F

 OLED_WriteCommand(0x40);	//设置显示开始行，0x40-0x7F

 OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置

 OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置

 OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
 OLED_WriteCommand(0x12);

 OLED_WriteCommand(0x81);	//设置对比度
 OLED_WriteCommand(0xCF);	//0x00-0xFF

 OLED_WriteCommand(0xD9);	//设置预充电周期
 OLED_WriteCommand(0xF1);

 OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
 OLED_WriteCommand(0x30);

 OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

 OLED_WriteCommand(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色

 OLED_WriteCommand(0x8D);	//设置充电泵
 OLED_WriteCommand(0x14);

 OLED_WriteCommand(0xAF);	//开启显示
 */
//  OLED_operate_gram(PEN_CLEAR);//清缓存
//  OLED_refresh_gram();
//  OLED_Clear();				//清空显存数组
//  OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}

/**
  * @brief          operate the graphic ram(size: 128*8 char)
  * @param[in]      pen: the type of operate.
                    PEN_CLEAR: set ram to 0x00
                    PEN_WRITE: set ram to 0xff
                    PEN_INVERSION: bit inversion
  * @retval         none
  */
/**
  * @brief          操作GRAM内存(128*8char数组)
  * @param[in]      pen: 操作类型.
                    PEN_CLEAR: 设置为0x00
                    PEN_WRITE: 设置为0xff
  * @retval         none
  */
void OLED_operate_gram(pen_typedef pen)
{
  if (pen == PEN_WRITE)
  {
    memset(OLED_GRAM, 0xff, sizeof(OLED_GRAM));
  }
  else if (pen == PEN_CLEAR)
  {
    memset(OLED_GRAM, 0x00, sizeof(OLED_GRAM));
  }
}

/**
 * @brief          cursor set to (x,y) point
 * @param[in]      x:X-axis, from 0 to 127
 * @param[in]      y:Y-axis, from 0 to 7
 * @retval         none
 */
/**
 * @brief          设置光标起点(x,y)
 * @param[in]      x:x轴, 从 0 到 127
 * @param[in]      y:y轴, 从 0 到 7
 * @retval         none
 */
void OLED_set_pos(uint8_t x, uint8_t y)
{
//   OLED_CMDbuf[y][0] = 0x00;
//   OLED_CMDbuf[y][1] = 0xb0 + y;
//   OLED_CMDbuf[y][2] = 0x10;
// #if (SCREEN_TYPE == SH1106_OLED)
//   OLED_CMDbuf[y][3] = 0x02; // sh1106:0x02
// #elif (SCREEN_TYPE == SSD1306_OLED)
//   OLED_CMDbuf[y][3] = 0x00; // ssd1306:0x00
// #endif
#ifdef SH1106_OLED
	x += 2;
#endif
OLED_WriteCommand(0xB0 | y);					//设置页位置
OLED_WriteCommand(0x10 | ((x & 0xF0) >> 4));	//设置X位置高4位
OLED_WriteCommand(0x00 | (x & 0x0F));			//设置X位置低4位
}

/**
  * @brief          draw one bit of graphic raw, operate one point of screan(128*64)
  * @param[in]      x: x-axis, [0, X_WIDTH-1]
  * @param[in]      y: y-axis, [0, Y_WIDTH-1]
  * @param[in]      pen: type of operation,
                        PEN_CLEAR: set (x,y) to 0
                        PEN_WRITE: set (x,y) to 1
                        PEN_INVERSION: (x,y) value inversion
  * @retval         none
  */
/**
  * @brief          操作GRAM中的一个位，相当于操作屏幕的一个点
  * @param[in]      x:x轴,  [0,X_WIDTH-1]
  * @param[in]      y:y轴,  [0,Y_WIDTH-1]
  * @param[in]      pen: 操作类型,
                        PEN_CLEAR: 设置 (x,y) 点为 0
                        PEN_WRITE: 设置 (x,y) 点为 1
                        PEN_INVERSION: (x,y) 值反转
  * @retval         none
  */
void OLED_draw_point(uint8_t  x, uint8_t  y, pen_typedef pen)
{

  /* check the corrdinate */
  // if ((x < 0) || (x > (X_WIDTH - 1)) || (y < 0) || (y > (Y_WIDTH - 1)))
  // {
  //   return;
  // }
   // 检查坐标是否合法
   if (x > MAX_X || y > MAX_Y) {
    return;
}
 // 计算页和行
 uint8_t page = y / 8;
 uint8_t row = y % 8;
 uint8_t bit_mask = 1 << row;

  // 根据操作类型更新GRAM
  switch (pen) {
    case PEN_WRITE:
        OLED_GRAM[x][page] |= bit_mask;
        break;
    case PEN_INVERSION:
        OLED_GRAM[x][page] ^= bit_mask;
        break;
    case PEN_CLEAR:
        OLED_GRAM[x][page] &= ~bit_mask;
        break;
    default:
        // 处理未知操作类型
        break;
}
}

/**
 * @brief          draw a line from (x1, y1) to (x2, y2)
 * @param[in]      x1: the start point of line
 * @param[in]      y1: the start point of line
 * @param[in]      x2: the end point of line
 * @param[in]      y2: the end point of line
 * @param[in]      pen: type of operation,PEN_CLEAR,PEN_WRITE,PEN_INVERSION.
 * @retval         none
 */
/**
 * @brief          画一条直线，从(x1,y1)到(x2,y2)
 * @param[in]      x1: 起点
 * @param[in]      y1: 起点
 * @param[in]      x2: 终点
 * @param[in]      y2: 终点
 * @param[in]      pen: 操作类型,PEN_CLEAR,PEN_WRITE,PEN_INVERSION.
 * @retval         none
 */

void OLED_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, pen_typedef pen)
{
  uint8_t col = 0, row = 0;
  uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
  float k = 0.0f, b = 0.0f;

  if (y1 == y2)
  {
    (x1 <= x2) ? (x_st = x1) : (x_st = x2);
    (x1 <= x2) ? (x_ed = x2) : (x_ed = x1);

    for (col = x_st; col <= x_ed; col++)
    {
      OLED_draw_point(col, y1, pen);
    }
  }
  else if (x1 == x2)
  {
    (y1 <= y2) ? (y_st = y1) : (y_st = y2);
    (y1 <= y2) ? (y_ed = y2) : (y_ed = y1);

    for (row = y_st; row <= y_ed; row++)
    {
      OLED_draw_point(x1, row, pen);
    }
  }
  else
  {
    k = ((float)(y2 - y1)) / (x2 - x1);
    b = (float)y1 - k * x1;

    (x1 <= x2) ? (x_st = x1) : (x_st = x2);
    (x1 <= x2) ? (x_ed = x2) : (x_ed = x2);

    for (col = x_st; col <= x_ed; col++)
    {
      OLED_draw_point(col, (uint8_t)(col * k + b), pen);
    }
  }
}


/**
 * @brief          发送数据到OLED的GRAM
 * @param[in]      none
 * @retval         none
 */

void OLED_SendBuff(uint8_t buff[8][128])
{
  // uint8_t i;
  // uint16_t j;
  //   for (i = 0; i < 8; i++)
  //   {
  //     OLED_set_pos(0, i);
  //     for (j = 0; j < 128; j++)
  //     {
  //       OLED_GRAM[i][j] = buff[i][j];
  //     }
  //     OLED_WriteData(OLED_GRAM[i], 128);/*连续写入128个数据，将显存数组的数据写入到OLED硬件*/
  //   }
  for (uint8_t page = 0; page < 8; page++)
    {
        OLED_set_pos(0, page);
        // 直接将buff中的数据复制到OLED_GRAM对应位置
        memcpy(OLED_GRAM[page], buff[page], 128);
        // 连续写入128个数据，将显存数组的数据写入到OLED硬件
        OLED_WriteData(OLED_GRAM[page], 128);
    }

}
