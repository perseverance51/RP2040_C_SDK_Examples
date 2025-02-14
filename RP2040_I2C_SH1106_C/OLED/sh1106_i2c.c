//
// Created by sztuka on 17.06.2023.
//


#include "sh1106_i2c.h"

uint8_t pageBuffer[8][128];
const uint8_t bytes_per_char = 16 * (9 / 8 + ((9 % 8) ? 1 : 0));
#define FONT_HEIGHT 16
#define FONT_WIDTH 8

inline static void swap(uint8_t *a, uint8_t *b) {
    uint8_t *t=a;
    *a=*b;
    *b=*t;
};

int getOffset(char c){
// offset = (ascii_code(character) - ascii_code(' ')) * bytes_per_char
    return (int)(((int)c - 32) * bytes_per_char);
};

void SH1106_init(sh1106_t *sh1106, i2c_inst_t *i2c, uint8_t address, uint8_t width, uint8_t height) {
    sh1106->address = address;
    sh1106->width = width;
    sh1106->height = height;
    sh1106->pages = height / 8;
    sh1106->i2c = i2c;
    sh1106->buffer = (uint8_t **) pageBuffer;
    SH1106_Write_CMD(sh1106, SET_DISP | 0x01);
    for(uint8_t i = 0; i < 8; i++){ //dark screen
        for(uint8_t j = 0; j < 128; j++){
            pageBuffer[i][j] = 0x00;
        }
    }
    SH1106_Write_CMD(sh1106, SET_SEG_REMAP | 0x01); //flip left-right
    SH1106_Write_CMD(sh1106, SET_SCAN_DIR | 0x08);  //flip top-bottom
}

void SH1106_Write_CMD(sh1106_t *sh1106, uint8_t command) {
    uint8_t buffer[2];
    buffer[0] = 0x80;
    buffer[1] = command;
    i2c_write_blocking(sh1106->i2c, sh1106->address, buffer, 2, false);
}
void SH1106_Write_Data(sh1106_t *sh1106, uint8_t* data) {
    size_t bufsize = sh1106->width+1;
    uint8_t broadCastBuffer[bufsize];
    broadCastBuffer[0] = 0x40;
    for (int i = 0; i < sh1106->width; i++) {
        broadCastBuffer[i+1] = data[i];
    }
    i2c_write_blocking(sh1106->i2c, sh1106->address, broadCastBuffer, bufsize, false);
}

void SH1106_draw(sh1106_t *sh1106){
    for(uint8_t page = 0; page < sh1106->pages; page++){
        SH1106_Write_CMD(sh1106, SET_PAGE_ADDR | page);
        SH1106_Write_CMD(sh1106, LOW_COL_ADDR | 0x02);
        SH1106_Write_CMD(sh1106, HIGH_COL_ADDR | 0x00);
        SH1106_Write_Data(sh1106, pageBuffer[page]);
    }
}

void SH1106_drawPixel(sh1106_t *sh1106, uint8_t x, uint8_t y, uint8_t color){
    if(x > sh1106->width || y > sh1106->height){
        return;
    }
    if(color == 0){
        pageBuffer[y/8][x] &= ~(1 << (y % 8));
    }else{
        pageBuffer[y/8][x] |= (1 << (y % 8));
    }

}

void SH1106_draw_hline(sh1106_t *sh1106, uint8_t x, uint8_t y, uint8_t w, uint8_t color){
    if(x > sh1106->width || y > sh1106->height){
        return;
    }
    if((x + w) > sh1106->width){
        w = sh1106->width - x;
    }
    for(uint8_t i = 0; i < w; i++){
        SH1106_drawPixel(sh1106, x + i, y, color);
    }
};


void SH1106_clear(sh1106_t *sh1106){
    for(uint8_t i = 0; i < 8; i++){ //dark screen
        for(uint8_t j = 0; j < 128; j++){
            pageBuffer[i][j] = 0x00;
        }
    }
};


void SH1106_drawRectangle(sh1106_t *sh1106, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
    for(uint8_t i=0; i<width; ++i)
        for(uint8_t j=0; j<height; ++j)
            SH1106_drawPixel(sh1106, x+i, y+j, color);

};

/*
void SH1106_drawChar(sh1106_t *sh1106, uint8_t x, uint8_t y, uint8_t scale, const uint8_t *font, char c, uint8_t color) {
    if(c<font[3]||c>font[4])
        return;

    uint32_t parts_per_line=(font[0]>>3)+((font[0]&7)>0);
    for(uint8_t w=0; w<font[1]; ++w) { // width
        uint32_t pp=(c-font[3])*font[1]*parts_per_line+w*parts_per_line+5;
        for(uint32_t lp=0; lp<parts_per_line; ++lp) {
            uint8_t line=font[pp];

            for(int8_t j=0; j<8; ++j, line>>=1) {
                if(line & 1)
                    SH1106_drawRectangle(sh1106, x+w*scale, y+((lp<<3)+j)*scale, scale, scale, color);
            }

            ++pp;
        }
    }
}

*/
void SH1106_drawChar(sh1106_t * sh1106, char c, uint8_t x, uint8_t y, uint8_t color, const uint8_t* font) {
    uint8_t i,j;
    uint32_t index;
    if (c == ' '){
        index = 0;
    }
    else if ( c < '!' || c > '~'){
        index = 0;
    }
    else{
        index = (c - 32) * 16; // -32 ascii {!} * 16 = bytes per char
    }
   for (i = 0; i < FONT_HEIGHT; i++){
       for(j = 0; j < FONT_WIDTH; j++){
           if(x+8-j > sh1106->width){
               return;
           }
           if(y+i > sh1106->height){
               return;
           }
           if (font[index+i] & (1 << j)){
               SH1106_drawPixel(sh1106, x+8-j, y + i, color);
           }
           else{
                SH1106_drawPixel(sh1106, x+8-j, y + i, !color);
           }
       }

   }
   if(color==0){
       SH1106_drawRectangle(sh1106, x, y, (FONT_WIDTH+1), 2, 0);
   }
}

void SH1106_drawString(sh1106_t *sh1106, char* str, uint8_t x, uint8_t y, uint8_t color, const uint8_t* font){
    uint8_t i = 0;
    while(str[i] != '\0'){
        if(x + i*(FONT_WIDTH+1) > sh1106->width){
            return;
        }
        SH1106_drawChar(sh1106, str[i], x + i*(FONT_WIDTH+1), y, color, font);

        if(color==0){ //fill black gaps between letters if text is inverted
            for(uint8_t j = 2; j < (FONT_HEIGHT); j++){
                SH1106_drawPixel(sh1106, x + i*9, y + j, 1);
            };
        }
        i++;
    }
}

/**
  * 函    数：将OLED显存数组部分清零
  * 参    数：X 指定区域左上角的横坐标，范围：-32768-32767，屏幕区域：0-127
  * 参    数：Y 指定区域左上角的纵坐标，范围：-32768-32767，屏幕区域：0-63
  * 参    数：Width 指定区域的宽度，范围：0-128
  * 参    数：Height 指定区域的高度，范围：0-64
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
 void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
 {
     int16_t i, j;

     for (j = Y; j < Y + Height; j ++)		//遍历指定页
     {
         for (i = X; i < X + Width; i ++)	//遍历指定列
         {
             if (i >= 0 && i <= 127 && j >=0 && j <= 63)				//超出屏幕的内容不显示
             {
                 pageBuffer[j / 8][i] &= ~(0x01 << (j % 8));	//将显存数组指定数据清零
             }
         }
     }
 }

 /**
  * 函    数：OLED显示图像
  * 参    数：X 指定图像左上角的横坐标，范围：-32768-32767，屏幕区域：0-127
  * 参    数：Y 指定图像左上角的纵坐标，范围：-32768-32767，屏幕区域：0-63
  * 参    数：Width 指定图像的宽度，范围：0-128
  * 参    数：Height 指定图像的高度，范围：0-64
  * 参    数：Image 指定要显示的图像
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数.（Image取模方式:垂直扫描，从左到右，从上到下）
  */
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i = 0, j = 0;
	int16_t Page, Shift;

	/*将图像所在区域清空*/
	OLED_ClearArea(X, Y, Width, Height);

	/*遍历指定图像涉及的相关页*/
	/*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		/*遍历指定图像涉及的相关列*/
		for (i = 0; i < Width; i ++)
		{
			if (X + i >= 0 && X + i <= 127)		//超出屏幕的内容不显示
			{
				/*负数坐标在计算页地址和移位时需要加一个偏移*/
				Page = Y / 8;
				Shift = Y % 8;
				if (Y < 0)
				{
					Page -= 1;
					Shift += 8;
				}

				if (Page + j >= 0 && Page + j <= 7)		//超出屏幕的内容不显示
				{
					/*显示图像在当前页的内容*/
					pageBuffer[Page + j][X + i] |= Image[j * Width + i] << (Shift);
				}

				if (Page + j + 1 >= 0 && Page + j + 1 <= 7)		//超出屏幕的内容不显示
				{
					/*显示图像在下一页的内容*/
					pageBuffer[Page + j + 1][X + i] |= Image[j * Width + i] >> (8 - Shift);
				}
			}
		}
	}
}
void SH1106_drawBitmap(sh1106_t *sh1106, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap, uint8_t color) {
    uint8_t i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (x + j > sh1106->width || y + i > sh1106->height) {
                continue; // 超出屏幕范围则跳过
            }
            if (bitmap[i * width + j]) {
                SH1106_drawPixel(sh1106, x + j, y + i, color);
            } else {
                SH1106_drawPixel(sh1106, x + j, y + i, !color);
            }
        }
    }
}
