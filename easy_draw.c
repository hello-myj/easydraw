
#include "easy_draw.h"


static int8_t sg_ed_graphics_enabled;
static uint16_t sg_screen_width=0;
static uint16_t sg_screen_height = 0;
static uint8_t sg_ed_brush_size = 1;
static easy_draw_driver_t* sg_ed_driver = NULL;

////���������ĳ���
//double easy_draw_vector_length(ed_vector_t v) 
//{
//	return sqrt(v.dx * v.dx + v.dy * v.dy);
//}
//
//// ��������һ��Ϊ��λ����
//ed_vector_t easy_draw_normalize_vector(ed_vector_t v)
//{
//	double length = easy_draw_vector_length(v);
//	ed_vector_t result = 
//	{
//		.dx = v.dx / length,
//		.dy = v.dy / length
//	};
//	return result;
//}


// ��������ת�����Ƕ�
ed_vector_t easy_draw_rotate_vector(ed_vector_t v, uint16_t angle_degrees)
{
	// ���Ƕ�ת��Ϊ����
	double angle_radians = angle_degrees * ED_PI / 180.0;

	// Ӧ����ת����
	ed_vector_t result = {
		.dx = v.dx * cos(angle_radians) - v.dy * sin(angle_radians),
		.dy = v.dx * sin(angle_radians) + v.dy * cos(angle_radians)
	};

	return result;
}


// ����Χ�����ĵ���תһ���Ƕȣ��������µ�����
ed_point_t easy_draw_rotate_point(ed_point_t point, ed_point_t center, uint16_t angle_degrees)
{
	// ���㷽������
	ed_vector_t direction = 
	{
		.dx = point.x - center.x,
		.dy = point.y - center.y
	};

	//double length = easy_draw_vector_length(direction);

	//direction = easy_draw_normalize_vector(direction);

	// ������������ת�����Ƕ�
	ed_vector_t rotated_direction = easy_draw_rotate_vector(direction, angle_degrees);

	// ����ת��ķ������������ĵ���ӣ��õ��µ������
	//ed_point_t result =
	//{
	//	.x = center.x + rotated_direction.dx* length,
	//	.y = center.y + rotated_direction.dy* length
	//};

	ed_point_t result =
	{
		.x = center.x + rotated_direction.dx ,
		.y = center.y + rotated_direction.dy 
	};

	return result;
}


void easy_draw_set_pixel_size(uint8_t size)
{
	sg_ed_brush_size = size;
}


static void easy_draw_brush(int16_t x, int16_t y, uint32_t color)
{
	int8_t brush_half_size = sg_ed_brush_size / 2;

	for (int i = -brush_half_size; i <= brush_half_size; i++)
	{
		for (int j = -brush_half_size; j <= brush_half_size; j++)
		{
			int16_t xi = x + i;
			int16_t yj = y + j;
			if (xi >= 0 && xi < sg_screen_width && yj >= 0 && yj < sg_screen_height)
			{
				sg_ed_driver->ed_pixel_draw(xi, yj, color);
			}
		}
	}
}


void easy_draw_pixel(int16_t x, int16_t y, uint32_t color)
{
	if ( ED_IS_DRIVER_NULL(sg_ed_driver, ed_pixel_draw) )
		return;

	if (sg_ed_brush_size > 1)
	{
		easy_draw_brush(x, y, color);
	}
	else
	{
		if (x >= 0 && x < sg_screen_width && y >= 0 && y < sg_screen_height)
		{
			sg_ed_driver->ed_pixel_draw(x, y, color);
		}
	}
}


uint32_t easy_get_pixel(int16_t x, int16_t y)
{
	return sg_ed_driver->ed_pixel_get(x, y);
}


void easy_draw_horizon_line(int16_t x, int16_t y, uint16_t w, uint32_t color)
{
	int16_t end = x + w;
	for (int a = x; a < end; a++)
	{
		easy_draw_pixel(a, y, color);
	}
}


void easy_draw_vertical_line(int16_t x,int16_t y, uint16_t h,uint32_t color)
{
	int16_t end = y + h;
	for (int a = y; a < end; a++)
	{
		easy_draw_pixel(x, a, color);
	}
}


void easy_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color) 
{
	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_line_draw) == false)
	{
		sg_ed_driver->ed_line_draw(x0, y0, x1, y1, color);
		return;
	}

	int16_t dx = abs(x1 - x0);
	int16_t dy = abs(y1 - y0);
	int16_t sx = x0 < x1 ? 1 : -1;
	int16_t sy = y0 < y1 ? 1 : -1;
	int16_t err = dx - dy;

	while (x0 != x1 || y0 != y1) {
		float intensity;
		if (dx == 0 && dy == 0) {
			intensity = 1.0f;
		}
		else {
			intensity = sqrtf((dx * dx + dy * dy) - (err * err)) / sqrtf(dx * dx + dy * dy);
		}
		
		//easy_draw_pixel(x0, y0, color + (int)(255 * intensity) * 0x010101);
		easy_draw_pixel(x0, y0, color);
		int16_t e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
	easy_draw_pixel(x0, y0, color);
}


void easy_draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color)
{
	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_rect_draw) == false)
	{
		sg_ed_driver->ed_rect_draw(x, y, width, height, color);
		return;
	}

	easy_draw_line(x, y, x + width - 1, y, color);

	// ���������
	easy_draw_line(x, y, x, y + height - 1, color);

	// �����±���
	easy_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color);

	// �����ұ���
	easy_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);
}

void easy_draw_fillRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color)
{

	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_rect_fill_draw) == false)
	{
		sg_ed_driver->ed_rect_fill_draw(x, y, width, height, color);
	}

	uint16_t i;
	for (i = 0; i < height; i++) {
		easy_draw_line(x, y + i, x + width - 1, y + i, color);
	}
}

void easy_draw_arc(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color)
{
	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_arc_draw) == false)
	{
		sg_ed_driver->ed_arc_draw(xc, yc, r, start_angle, end_angle, color);
	}

	int x, y, d;
	float radians, start_radians, end_radians;

	// ���Ƕ�ת��Ϊ����
	start_radians = start_angle * 3.1415926 / 180.0;
	end_radians = end_angle * 3.1415926 / 180.0;

	// ��ʼ����ʼ�������
	x = r * cos(start_radians) + xc;
	y = r * sin(start_radians) + yc;

	// ����Բ��
	for (radians = start_radians; radians <= end_radians; radians += 0.01) {
		// ������һ�����ص������
		x = r * cos(radians) + xc;
		y = r * sin(radians) + yc;

		// ���Ƶ�ǰ���ص�
		easy_draw_pixel(x, y, color);
	}
}


void easy_draw_sector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color)
{
	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_sector_draw) == false)
	{
		sg_ed_driver->ed_sector_draw(xc, yc, r, start_angle, end_angle, color);
	}

	int x, y, d;
	float radians, start_radians, end_radians;

	// ���Ƕ�ת��Ϊ����
	start_radians = start_angle * 3.1415926 / 180.0;
	end_radians = end_angle * 3.1415926 / 180.0;

	// ��ʼ����ʼ�������
	x = r * cos(start_radians) + xc;
	y = r * sin(start_radians) + yc;

	// ����Բ��
	for (radians = start_radians; radians <= end_radians; radians += 0.01) {
		// ������һ�����ص������
		x = r * cos(radians) + xc;
		y = r * sin(radians) + yc;

		// ���Ƶ�ǰ���ص�
		easy_draw_pixel(x, y, color);
	}
	// ��������
	easy_draw_line(xc, yc, r * cos(start_radians) + xc, r * sin(start_radians) + yc, color);
	easy_draw_line(xc, yc, r * cos(end_radians) + xc, r * sin(end_radians) + yc, color);
}



void easy_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint32_t color) {
	easy_draw_line(x1, y1, x2, y2, color);
	easy_draw_line(x2, y2, x3, y3, color);
	easy_draw_line(x3, y3, x1, y1, color);
}

void easy_draw_fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint32_t color) {
	easy_draw_line(x1, y1, x2, y2, color);
	easy_draw_line(x2, y2, x3, y3, color);
	easy_draw_line(x3, y3, x1, y1, color);


}


//ɨ��������㷨
void easy_draw_scanLineFill(int x, int y, uint32_t color)
{


	//int pRight, pLeft;
	//std::stack<CPoint> stk;
	//int mColor = pDC->GetPixel(x, y);	//��������

	//stk.push(CPoint(x, y));
	//while (!stk.empty()) {


	//	CPoint p = stk.top();	//ջ�����س�ջ
	//	stk.pop();

	//	pRight = SetRP(p.x, p.y, color, mColor, pDC);	//�������ҽ������
	//	pLeft = SetLP(p.x, p.y, color, mColor, pDC);

	//	//��������ɨ���ߴ���
	//	NewLineSeed(&stk, pLeft, pRight, p.y + 1, color, mColor, pDC);
	//	NewLineSeed(&stk, pLeft, pRight, p.y - 1, color, mColor, pDC);
	//}
}
// 
//��㷨����λ���




void easy_draw_fill_sector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color)
{

}





int easy_draw_init(easy_draw_driver_t* easy_draw_driver,uint16_t width, uint16_t height)
{

	if (ED_IS_NULL(easy_draw_driver))
		return ED_ERROR;

	if (ED_IS_MEMBER_NULL(easy_draw_driver, ed_init))
		return ED_ERROR;

	sg_screen_width = width;
	sg_screen_height = height;

	sg_ed_driver = easy_draw_driver;

	sg_ed_driver->ed_init();
		
	return ED_SUCCESS;
}

