
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

static void easy_draw_brush(int16_t x, int16_t y, uint32_t color)
{

	int8_t brush_half_size_s = 0;
	int8_t brush_half_size_e = 0;
	if (sg_ed_brush_size % 2 == 0)
	{
		 brush_half_size_s = sg_ed_brush_size / 2-1;
		 brush_half_size_e = sg_ed_brush_size / 2;
	}
	else
	{
		brush_half_size_s = sg_ed_brush_size / 2;
		brush_half_size_e = sg_ed_brush_size / 2;
	}


	for (int i = -brush_half_size_s; i <= brush_half_size_e; i++)
	{
		for (int j = -brush_half_size_s; j <= brush_half_size_e; j++)
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


// ��������ת�����Ƕ�
static ed_vector_t easy_helper_rotate_vector(ed_vector_t v, uint16_t angle_degrees)
{
	// ���Ƕ�ת��Ϊ����
	double angle_radians = angle_degrees * ED_PI / 180.0;

	// Ӧ����ת����
	ed_vector_t result = 
	{
		.dx = v.dx * cos(angle_radians) - v.dy * sin(angle_radians),
		.dy = v.dx * sin(angle_radians) + v.dy * cos(angle_radians)
	};

	return result;
}


// ����Χ�����ĵ���תһ���Ƕȣ��������µ�����
ed_point_t easy_helper_rotate_point(ed_point_t point, ed_point_t center, uint16_t angle_degrees)
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
	ed_vector_t rotated_direction = easy_helper_rotate_vector(direction, angle_degrees);

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

//��ˢ����ʹ������
void easy_draw_set_brush_size(uint8_t size)
{
	sg_ed_brush_size = size;
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
	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_pixel_get))
		return;
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

		//int tmp = sqrtf(dx * dx + dy * dy);

		//if (tmp == 0)
		//{
		//	intensity = 1.0f;
		//}
		//else
		//{
		//	intensity = sqrtf((dx * dx + dy * dy) - (err * err)) / tmp;
		//}
		
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

	int x, y, d;
	float radians, start_radians, end_radians;
	ed_point_t last_point = { 0 };

	// ���Ƕ�ת��Ϊ����
	start_radians = start_angle * 3.1415926 / 180.0;
	end_radians = end_angle * 3.1415926 / 180.0;

	// ��ʼ����ʼ�������
	x = r * cos(start_radians) + xc;
	y = r * sin(start_radians) + yc;
	last_point.x = x;
	last_point.y = y;

	// ����Բ��
	for (radians = start_radians; radians <= end_radians; radians += 0.1) {
		// ������һ�����ص������
		x = r * cos(radians) + xc;
		y = r * sin(radians) + yc;
		//
		easy_draw_line(last_point.x, last_point.y, x, y, color);
		last_point.x = x;
		last_point.y = y;
		// ���Ƶ�ǰ���ص�
		//easy_draw_pixel(x, y, color);
	}
	easy_draw_line(last_point.x, last_point.y, r * cos(end_radians) + xc, r * sin(end_radians) + yc, color);

	// �����߶�
	easy_draw_line(xc, yc, r * cos(start_radians) + xc, r * sin(start_radians) + yc, color);
	easy_draw_line(xc, yc, r * cos(end_radians) + xc, r * sin(end_radians) + yc, color);
}

//ʹ�����������
void easy_draw_fillSector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color)
{

	int x, y, d;
	float radians, start_radians, end_radians;
	ed_point_t triangle_points[3] = { 0 }, last_point = {0};

	triangle_points[0].x = xc;
	triangle_points[0].y = yc;

	// ���Ƕ�ת��Ϊ����
	start_radians = start_angle * 3.1415926 / 180.0;
	end_radians = end_angle * 3.1415926 / 180.0;

	// ��ʼ����ʼ�������
	x = r * cos(start_radians) + xc;
	y = r * sin(start_radians) + yc;

	last_point.x = x;
	last_point.y = y;

	// ����Բ��
	for (radians = start_radians; radians <= end_radians; radians += 0.1) {
		// ������һ�����ص������
		triangle_points[1] = last_point;

		x = r * cos(radians) + xc;
		y = r * sin(radians) + yc;
		easy_draw_line(last_point.x, last_point.y, x, y, color);
		triangle_points[2].x = x;
		triangle_points[2].y = y;

		last_point = triangle_points[2];

		// ���Ƶ�ǰ���ص�
		//easy_draw_pixel(x, y, color);

		easy_draw_fillPolygon(triangle_points, 3, color);

	}
	easy_draw_line(last_point.x, last_point.y, r * cos(end_radians) + xc, r * sin(end_radians) + yc, color);
	triangle_points[1] = last_point;
	triangle_points[2].x = r * cos(end_radians) + xc;
	triangle_points[2].y = r * sin(end_radians) + yc;

	easy_draw_fillPolygon(triangle_points, 3, color);

	// �����߶�
	easy_draw_line(xc, yc, r * cos(start_radians) + xc, r * sin(start_radians) + yc, color);
	easy_draw_line(xc, yc, r * cos(end_radians) + xc, r * sin(end_radians) + yc, color);
}


void easy_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	easy_draw_line(x0, y0, x1, y1, color);
	easy_draw_line(x1, y1, x2, y2, color);
	easy_draw_line(x2, y2, x0, y0, color);
}

void easy_draw_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	easy_draw_line(x0, y0, x1, y1, color);
	easy_draw_line(x1, y1, x2, y2, color);
	easy_draw_line(x2, y2, x0, y0, color);
	ed_point_t polygon[3] = { 0 };
	polygon[0].x = x0;
	polygon[0].y = y0;
	polygon[1].x = x1;
	polygon[1].y = y1;
	polygon[2].x = x2;
	polygon[2].y = y2;

	easy_draw_fillPolygon(polygon, 3, color);
}


/* �ж������߶��Ƿ��ཻ */
int easy_helper_is_intersect(ed_point_t a1, ed_point_t a2, ed_point_t b1, ed_point_t b2) 
{
	int c1 = (a2.x - a1.x) * (b1.y - a1.y) - (b1.x - a1.x) * (a2.y - a1.y);
	int c2 = (a2.x - a1.x) * (b2.y - a1.y) - (b2.x - a1.x) * (a2.y - a1.y);
	int c3 = (b2.x - b1.x) * (a1.y - b1.y) - (a1.x - b1.x) * (b2.y - b1.y);
	int c4 = (b2.x - b1.x) * (a2.y - b1.y) - (a2.x - b1.x) * (b2.y - b1.y);
	return (c1 * c2 < 0 && c3 * c4 < 0);
}

int easy_helper_is_convex_polygon(ed_point_t* points, int num_points) 
{
	int i, j, k;
	int flag = 0;
	for (i = 0; i < num_points; i++) 
	{
		j = (i + 1) % num_points;
		k = (j + 1) % num_points;
		int cross = (points[j].x - points[i].x) * (points[k].y - points[j].y) - (points[j].y - points[i].y) * (points[k].x - points[j].x);
		if (i == 0) 
		{
			flag = cross > 0;
		}
		else if (flag != (cross > 0))
		{
			return 0;  /* ���ڹ����ڲ��������˵������͹����� */
		}
	}
	return 1;  /* �����ڹ����ڲ��������˵����͹����� */
}


/* �ж�����������Ƿ��ܹ����һ������� */
int easy_helper_is_polygon(ed_point_t* points, int num_points)
{
	int i, j;
	if (num_points < 3)
		return 0;  /* ������Ҫ3���������ɶ���� */
	for (i = 0; i < num_points; i++) {
		for (j = i + 1; j < num_points; j++) 
		{
			/* �ж�ÿ�����Ƿ���������ཻ */
			if (easy_helper_is_intersect(points[i], points[(i + 1) % num_points], points[j], points[(j + 1) % num_points])) 
			{
				return 0;  /* �����ཻ�������˵���޷���ɶ���� */
			}
		}
	}
	return 1;  /* �������ཻ�������˵��������ɶ���� */
}

void easy_draw_polygon(ed_point_t * points, uint16_t num_points,uint32_t color)
{
	int i;
	if (!easy_helper_is_polygon(points, num_points)) 
		return;
	for (i = 0; i < num_points; i++) 
	{
		easy_draw_line(points[i].x, points[i].y, points[(i + 1) % num_points].x, points[(i + 1) % num_points].y, color);
	}
}


ed_polygonRect_helper_t easy_helper_ex_getPolygonRect(uint16_t num_points, ed_point_t tar_polygon[])
{


	ed_polygonRect_helper_t boxRect;

	boxRect.minX = 50000;
	boxRect.minY = 50000;
	boxRect.maxX = -50000;
	boxRect.maxY = -50000;

	for (int i = 0; i < num_points; i++) {


		if (tar_polygon[i].x < boxRect.minX) boxRect.minX = tar_polygon[i].x;
		if (tar_polygon[i].y < boxRect.minY) boxRect.minY = tar_polygon[i].y;
		if (tar_polygon[i].x > boxRect.maxX) boxRect.maxX = tar_polygon[i].x;
		if (tar_polygon[i].y > boxRect.maxY) boxRect.maxY = tar_polygon[i].y;
	}
	return boxRect;
}



bool easy_helper_ex_inPolygon(uint16_t num_points, ed_point_t tar_polygon[], ed_point_t testPoint)
{

	if (num_points < 3)
		return false;

	bool  inSide = false;
	float lineSlope, interSectX;
	int   i = 0, j = num_points - 1;

	for (i = 0; i < num_points; i++) {


		if ((tar_polygon[i].y < testPoint.y && tar_polygon[j].y >= testPoint.y ||
			tar_polygon[j].y < testPoint.y && tar_polygon[i].y >= testPoint.y) &&
			(tar_polygon[i].x <= testPoint.x || tar_polygon[j].x <= testPoint.x)) {


			if (tar_polygon[j].x != tar_polygon[i].x) {


				lineSlope = (float)(tar_polygon[j].y - tar_polygon[i].y) / (tar_polygon[j].x - tar_polygon[i].x);
				interSectX = (int)(tar_polygon[i].x + (testPoint.y - tar_polygon[i].y) / lineSlope);
			}
			else
				interSectX = tar_polygon[i].x;
			if (interSectX < testPoint.x)
				inSide = !inSide;
		}
		j = i;
	}

	return inSide;
}


//��㷨��� //https://www.codetd.com/article/11761841
void easy_draw_fillPolygon(ed_point_t* points, uint16_t num_points, uint32_t color)
{

	ed_polygonRect_helper_t polyRect;

	polyRect = easy_helper_ex_getPolygonRect(num_points, points);

	ed_point_t testPoint;
	//����С�㵽����һ���жϣ�����ڸö�����ڲ�����������
	for (testPoint.x = polyRect.minX; testPoint.x < polyRect.maxX; testPoint.x++)
		for (testPoint.y = polyRect.minY; testPoint.y < polyRect.maxY; testPoint.y++) {


			if (easy_helper_ex_inPolygon(num_points, points, testPoint))
				easy_draw_pixel(testPoint.x, testPoint.y, color);

		}
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

