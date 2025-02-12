#ifndef __TEST_UI_H__
#define __TEST_UI_H__

#include "oled.h"
#include "oled_g.h"
#include "oled_ui.h"

#define SETTING_PAGE_NUM     13

extern Option setting_option_array[SETTING_PAGE_NUM] ;

void TestUI_Init(void);
void TestUI_Proc(void);

#endif
