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
	setbkcolor(WHITE);
	cleardevice();

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

#include<math.h>
#define PI acos(-1.0)
double th = PI / 180;

void DrawEllipse(int x0, int y0, int a, int b, int k, int color)
{
	double i;
	double x, y, tx, ty;
	for (i = -180; i <= 180; i = i + 0.5)
	{
		x = a * cos(i * th);
		y = b * sin(i * th);
		tx = x;
		ty = y;
		x = tx * cos(k * th) - ty * sin(k * th) + x0;
		y = y0 - (ty * cos(k * th) + tx * sin(k * th));
		easy_draw_fillCircle((int)x, (int)y, 1, color);
	}
}

#if 1
int main()
{
	easy_draw_init(&easy_draw_drver, WIDTH, HEIGHT);

	//easy_draw_fillEllipse(200, 200,100 , 200, 1);
	easy_draw_set_brush_size(2);
	// 左耳朵
	easy_draw_rect(175, 266, 15, 59,0);
	easy_draw_rect(159, 281, 16, 34,0);
	// 右耳朵
	easy_draw_rect(393, 268, 17, 56,0);
	easy_draw_rect(410, 286, 13, 25,0);
	easy_draw_fillEllipse(292,299,210/2,206/2,1);
	easy_draw_ellipse2(187, 196, 397, 402, 0);

	//角
	easy_draw_fillRoundedRect(288, 146, 14, 96, 5, 1);
	easy_draw_roundedrect(288, 146, 14, 96, 5, 0);
	//// 绘制左右眼睛
	// 
	easy_draw_set_brush_size(1);
	DrawEllipse(243, 297, 38, 30,-30, 0);
	DrawEllipse(350, 297, 38, 30,30, 0);
	easy_draw_set_brush_size(2);

	easy_draw_pixel(243, 297, 0);
	easy_draw_pixel(350, 297, 0);

	
	// 
	// 
	//setfillcolor(RGB(248, 245, 143));
	//floodfill(243, 297, BLACK);
	//floodfill(350, 297, BLACK);
	//line(296, 422, 249, 394);
	//line(296, 422, 336, 394);
	//setfillcolor(RGB(235, 110, 69));
	//floodfill(295, 410, BLACK);
	//setfillcolor(RGB(137, 211, 211));
	//fillcircle(294, 432, 10);
	//// 绘制身体
	//arc(222, 399, 286, 591, 145.0 / 180 * PI, PI + 145.0 / 180 * PI);
	//arc(305, 413, 364, 591, PI + 35.0 / 180 * PI, 55.0 / 180 * PI);
	//line(224, 485, 359, 485);
	//line(224, 511, 278, 549);
	//line(278, 549, 312, 549);
	//line(312, 549, 360, 515);
	//setfillcolor(RGB(235, 110, 69));
	//floodfill(294, 517, BLACK);
	//setfillcolor(RGB(238, 238, 238));
	//floodfill(252, 554, BLACK);
	//floodfill(334, 559, BLACK);
	//// 绘制左边胳膊
	//arc(189, 387, 353, 647, 109.0 / 180 * PI, PI);
	//arc(189, 480, 223, 537, 10.0 / 180.0 * PI + PI, 0);
	//line(196, 471, 222, 491);
	//setfillcolor(RGB(235, 110, 69));
	//floodfill(207, 501, BLACK);
	//// 绘制右胳膊
	//arc(230, 319, 424, 455, 110.0 / 180 * PI + PI, 5.0 / 180 * PI);
	//arc(392, 360, 424, 395, -5.0 / 180 * PI, PI + PI / 2);
	//arc(310, 286, 402, 394, 70.0 / 180 * PI + PI, 150.0 / 180 * PI + PI);
	//line(372, 390, 394, 431);
	//setfillcolor(RGB(235, 110, 69));
	//floodfill(399, 402, BLACK);
	//// 给身体颜色
	//setfillcolor(RGB(238, 238, 238));
	//floodfill(296, 458, BLACK);
	//// 连接气球
	//line(463, 187, 422, 365);
	//heart(464, 67, 30, BLACK);
	//setfillcolor(RGB(235, 110, 69));
	//floodfill(464, 70, BLACK);
	//setfillcolor(RGB(255, 232, 201));
	//solidcircle(508, 70, 6);



	_getch();				// 按任意键继续
	return 0;
}

#endif



