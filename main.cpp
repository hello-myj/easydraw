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
	if (color == 0)
		putpixel(x, y, RGB(0, 0, 0));
	else
		putpixel(x, y, RGB(255, 255, 255));
}

static uint32_t easyx_pixel_get(int16_t x, int16_t y)
{
	COLORREF color1 = getpixel(x, y);
	if (color1 > 0)
		return 1;
	else
		return 0;
}

static easy_draw_driver_t easy_draw_drver = {
	.ed_init = easyx_init,
	.ed_pixel_draw = easyx_draw_pixel,
	.ed_pixel_get = easyx_pixel_get,
};

int main()
{
	easy_draw_init(&easy_draw_drver, WIDTH, HEIGHT);

	easy_draw_set_brush_size(1);
	//easy_draw_triangle(10, 100, 200, 400, 400, 300, 1);

	//easy_draw_pixel(100, 100, 1);
	//
	//easy_draw_set_pixel_size(4);
	//easy_draw_line(10, 20, 300, 200, 1);
	//easy_draw_set_pixel_size(2);
	//easy_draw_line(10,20,300,200,0);

	//easy_draw_set_pixel_size(4);
	//easy_draw_rect(20, 30, 300, 50, 1);

	//easy_draw_set_pixel_size(2);
	//easy_draw_rect(20, 30, 300, 50, 0);

	//easy_draw_set_pixel_size(6);
	//easy_draw_arc(100, 200, 100, 80, 360, 1);
	//easy_draw_set_pixel_size(4);
	//easy_draw_arc(100, 200, 100, 0, 10, 0);


	//for(int i=0;i<120;i++)
	//{
	//	ed_point_t c = { 200,300 };
	//	ed_point_t result1 = { 0 }, result2 = {0};
	//	ed_point_t src1 = { 350,300 };
	//	ed_point_t src2 = { 250,300 };

	//	result1 = easy_draw_rotate_point(src1, c, i * 3);
	//	result2 =easy_draw_rotate_point(src2, c, i * 3);
	//	//easy_draw_pixel(result1.x, result1.y,1);
	//	//easy_draw_pixel(result2.x, result2.y, 1);
	//	easy_draw_line(result1.x, result1.y, result2.x, result2.y, 1);
	//	Sleep(10);
	//}

	easy_draw_fillTriangle(100, 100, 10, 50, 400, 500);

	//for (int i = 0;i < 120;i++)
	//{
	//	ed_point_t c = { 200+400,300 };
	//	ed_point_t result1 = { 0 }, result2 = { 0 };
	//	ed_point_t src1 = { 350+400,300 };
	//	ed_point_t src2 = { 200+400,300 };

	//	result1 = easy_draw_rotate_point(src1, c, i * 3);
	//	result2 = easy_draw_rotate_point(src2, c, i * 3);
	//	//easy_draw_pixel(result1.x, result1.y,1);
	//	//easy_draw_pixel(result2.x, result2.y, 1);
	//	easy_draw_line(result1.x, result1.y, result2.x, result2.y, 1);

	//	Sleep(10);
	//}




	//easy_draw_sector(100, 200, 100, 0, 100, 1);
	//easy_draw_fillSector(100, 100, 50, 30, 120, 1);

	//easy_draw_fill_sector(100, 200, 100, 0, 100, 1);
	//initgraph(640, 480);	// 创建绘图窗口，大小为 640x480 像素
	//circle(200, 200, 100);	// 画圆，圆心(200, 200)，半径 100
	_getch();				// 按任意键继续
	//closegraph();			// 关闭绘图窗口
	return 0;
}


