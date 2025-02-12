#ifndef __OLED__H
#define __OLED__H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include <stdint.h>

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

#define SH1106_OLED    0
#define SSD1306_OLED    1


//#define	 SCREEN_TYPE_SSD1306	//0.96" SSD1306 I2C OLED屏幕使用
#define  SCREEN_TYPE_SH1106    //1.3" SH1106 I2C OLED屏幕使用

#if defined(SCREEN_TYPE_SSD1306) && defined(SCREEN_TYPE_SH1106)
    #error "Only one SCREEN_TYPE can be defined at a time!"
#elif defined(SCREEN_TYPE_SSD1306)
    // 使用 SSD1306 的代码
    #define SCREEN_TYPE SSD1306_OLED
//		#warning "Using SSD1306 screen type."
#elif defined(SCREEN_TYPE_SH1106)
    // 使用 SH1106_OLED 的代码
    #define SCREEN_TYPE SH1106_OLED
//		#warning "Using SH1106_OLED screen type."
#else
    #error "No SCREEN_TYPE defined!"
#endif


// the I2C address of oled
#define OLED_I2C_ADDRESS    0x3C
#define I2C_MASTER_FREQ_HZ  400000 // I2C clock of SSD1306 can run at 400 kHz max.
#define I2C_TICKS_TO_WAIT 100	  // Maximum ticks to wait before issuing a timeout.
//the resolution of oled   128*64
#define MAX_COLUMN      128
#define MAX_ROW         64

#define X_WIDTH         MAX_COLUMN
#define Y_WIDTH         MAX_ROW

#define OLED_CMD        0x00
#define OLED_DATA       0x01

//#define CHAR_SIZE_WIDTH 6
//#define CHAR_SIZE_HIGHT 12


typedef enum
{
    PEN_CLEAR = 0x00,
    PEN_WRITE = 0x01,
    PEN_INVERSION= 0x02,
}pen_typedef;



/**
  * @brief          初始化OLED模块，
  * @param[in]      none
  * @retval         none
  */
void OLED_Init(void);

uint32_t oled_pow(uint8_t m, uint8_t n);
uint8_t check_num_len(uint32_t num);
//void OLED_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t mode, uint8_t len);
//void OLED_show_floatnum(uint8_t x, uint8_t y, float num, uint8_t mode);

//void OLED_display_on(void);
//void OLED_display_off(void);

/**
  * @brief          操作GRAM内存(128*8char数组)
  * @param[in]      pen: 操作类型.
                    PEN_CLEAR: 设置为0x00
                    PEN_WRITE: 设置为0xff
                    PEN_INVERSION: 按位取反
  * @retval         none
  */
void OLED_operate_gram(pen_typedef pen);

/**
  * @brief          设置光标起点(x,y)
  * @param[in]      x:x轴, 从 0 到 127
  * @param[in]      y:y轴, 从 0 到 7
  * @retval         none
  */
void OLED_set_pos(uint8_t x, uint8_t y);


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
void OLED_draw_point(uint8_t x, uint8_t y, pen_typedef pen);

/**
  * @brief          画一条直线，从(x1,y1)到(x2,y2)
  * @param[in]      x1: 起点
  * @param[in]      y1: 起点
  * @param[in]      x2: 终点
  * @param[in]      y2: 终点
  * @param[in]      pen: 操作类型,PEN_CLEAR,PEN_WRITE,PEN_INVERSION.
  * @retval         none
  */
//void OLED_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, pen_typedef pen);


/**
  * @brief          show a character
  * @param[in]      row: start row of character
  * @param[in]      col: start column of character
  * @param[in]      chr: the character ready to show
  * @retval         none
  */
/**
  * @brief          显示一个字符
  * @param[in]      row: 字符的开始行
  * @param[in]      col: 字符的开始列
  * @param[in]      chr: 字符
  * @retval         none
  */
//void OLED_show_char(uint8_t row, uint8_t col, uint8_t chr);

/**
  * @brief          show a character string
  * @param[in]      row: row of character string begin
  * @param[in]      col: column of character string begin
  * @param[in]      chr: the pointer to character string
  * @retval         none
  */
/**
  * @brief          显示一个字符串
  * @param[in]      row: 字符串的开始行
  * @param[in]      col: 字符串的开始列
  * @param[in]      chr: 字符串
  * @retval         none
  */
//void OLED_show_string(uint8_t row, uint8_t col, uint8_t *chr);

/**
  * @brief          formatted output in oled 128*64
  * @param[in]      row: row of character string begin, 0 <= row <= 4;
  * @param[in]      col: column of character string begin, 0 <= col <= 20;
  * @param          *fmt: the pointer to format character string
  * @note           if the character length is more than one row at a time, the extra characters will be truncated
  * @retval         none
  */
/**
  * @brief          格式输出
  * @param[in]      row: 开始列，0 <= row <= 4;
  * @param[in]      col: 开始行， 0 <= col <= 20;
  * @param[in]      *fmt:格式化输出字符串
  * @note           如果字符串长度大于一行，额外的字符会换行
  * @retval         none
  */
//void OLED_printf(uint8_t row, uint8_t col, const char *fmt,...);


//void OLED_LOGO(void); //图片显示不可用
/**
  * @brief          send the data of gram to oled sreen
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          发送数据到OLED的GRAM
  * @param[in]      none
  * @retval         none
  */
//void OLED_refresh_gram(void);
//void OLED_show_char2(uint8_t row, uint8_t col, uint8_t chr,uint8_t size);
void OLED_showString(uint8_t row, uint8_t col, uint8_t *chr,uint8_t size);

//void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
//void OLED_LOGO(void);
void OLED_SendBuff(uint8_t buff[8][128]); //将8*128字节的buff一次性全部发送的函数
#endif

