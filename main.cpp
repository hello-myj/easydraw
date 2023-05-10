#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include "easy_draw.h"
#include "easy_draw_ex.h"
#include <stdio.h>
#include <conio.h>
#define WIDTH 800
#define HEIGHT 480

/*
	【项目】 - 【属性】 - 【配置属性】 - 【c / c++】 - 【语言】
	把【c++语言标准】选择为【ISO C++20 标准(/ std:c++20)】
*/
static int easyx_init()
{
	initgraph(WIDTH, HEIGHT);

	return 0;
}

static void  easyx_draw_pixel(int16_t x, int16_t y, uint32_t color)
{
#if 1
	if (color == 0)
		putpixel(x, y, RGB(0, 0, 0));
	else
		putpixel(x, y, RGB(255, 255, 255));
#else

	putpixel(x, y, RGB(color*85, color * 85, color * 85));
#endif
}

int (user_2bit_pic)(int16_t x, int16_t y, const uint8_t* picdata, uint8_t w, uint8_t h, uint8_t isOverlay)
{
	return 0;
}


static easy_draw_driver_t easy_draw_drver = {
	.ed_init = easyx_init,
	.ed_pixel_draw = easyx_draw_pixel,
};


#if 1
int main()
{
	easy_draw_init(&easy_draw_drver, WIDTH, HEIGHT);

	//easy_draw_fillEllipse(200, 200,100 , 200, 1);
	easy_draw_ellipse(200, 200, 100, 200, 1);

	easy_draw_circle(200, 200,200, 1);





	_getch();				// 按任意键继续
	return 0;
}

#endif



