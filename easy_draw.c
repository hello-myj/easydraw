
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

	while (x0 != x1 || y0 != y1) 
	{
		float intensity;
		if (dx == 0 && dy == 0) 
		{
			intensity = 1.0f;
		}
		else 
		{
			int16_t tmp = sqrtf(dx * dx + dy * dy);
			if (tmp == 0)
			{
				intensity = 1.0f;
			}
			else
			{
				intensity = sqrtf((dx * dx + dy * dy) - (err * err)) / tmp;
			}
			
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
void easy_draw_fillSector(int16_t xc, int16_t yc, int16_t r, int16_t start_angle, int16_t end_angle, uint32_t color)
{

	volatile int16_t  mid_x, mid_y,d, sx,sy,ex,ey;
	float radians, start_radians, end_radians;
	ed_point_t triangle_points[3] = { 0 }, last_point = {0};


	// ���Ƕ�ת��Ϊ����
	start_radians = start_angle * ED_PI / 180.0;
	end_radians = end_angle * ED_PI / 180.0;

	// ��ʼ����ʼ�������
	//x = r * cos(start_radians) + xc;
	//y = r * sin(start_radians) + yc;

	sx= r * cos(start_radians) + xc;
	sy = r * sin(start_radians) + yc;

	ex = r * cos(end_radians) + xc;
	ey = r * sin(end_radians) + yc;

	triangle_points[0].x = xc;
	triangle_points[0].y = yc;
	triangle_points[1].x = sx;
	triangle_points[1].y = sy;
	triangle_points[2].x = ex;
	triangle_points[2].y = ey;

	easy_draw_fillTriangle(
	triangle_points[0].x, triangle_points[0].y,
	triangle_points[1].x, triangle_points[1].y,
	triangle_points[2].x, triangle_points[2].y,
	color
);
	//easy_draw_triangle2(triangle_points, color);



	//triangle_points[0].x = (triangle_points[1].x + triangle_points[2].x) / 2;
	//triangle_points[0].y = (triangle_points[1].y + triangle_points[2].y) / 2;

	// ����Բ��
	for (radians = start_radians; radians <= end_radians; radians += 0.01) {
		// ������һ�����ص������

		triangle_points[2].x = r * cos(radians) + xc;
		triangle_points[2].y = r * sin(radians) + yc;

		easy_draw_line(triangle_points[1].x, triangle_points[1].y, triangle_points[2].x, triangle_points[2].y, color);
		
		// ���Ƶ�ǰ���ص�
		//easy_draw_pixel(x, y, color);

		//easy_draw_triangle2(triangle_points, color);
		easy_draw_fillTriangle(
			triangle_points[0].x, triangle_points[0].y,
			triangle_points[1].x, triangle_points[1].y,
			triangle_points[2].x, triangle_points[2].y,
			color
		);
		triangle_points[1] = triangle_points[2];

	}

	triangle_points[2].x = ex;
	triangle_points[2].y = ey;
	easy_draw_line(triangle_points[1].x, triangle_points[1].y, triangle_points[2].x, triangle_points[2].y, color);
	easy_draw_fillTriangle(
		triangle_points[0].x, triangle_points[0].y,
		triangle_points[1].x, triangle_points[1].y,
		triangle_points[2].x, triangle_points[2].y,
		color
	);


	// �����߶�
	easy_draw_line(xc, yc, sx, sy, color);
	easy_draw_line(xc, yc, ex,ey, color);
}

void easy_draw_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

	int16_t a, b, y, last;
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		ED_SWAP_INT(y0, y1); ED_SWAP_INT(x0, x1);
	}
	if (y1 > y2)
	{
		ED_SWAP_INT(y2, y1); ED_SWAP_INT(x2, x1);
	}
	if (y0 > y1)
	{
		ED_SWAP_INT(y0, y1); ED_SWAP_INT(x0, x1);
	}

	if (y0 == y2)
	{ // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
		{
			a = x1;
		}
		else if (x1 > b)
		{
			b = x1;
		}
		if (x2 < a)
		{
			a = x2;
		}
		else if (x2 > b)
		{
			b = x2;
		}
		easy_draw_horizon_line(a, y0, b - a + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1,
		sa = 0,
		sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
	{
		last = y1;   // Include y1 scanline
	}
	else
	{
		last = y1 - 1; // Skip it
	}


	for (y = y0; y <= last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if (a > b)
		{
			ED_SWAP_INT(a, b);
		}

		easy_draw_horizon_line(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);

	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if (a > b)
		{
			ED_SWAP_INT(a, b);
		}

		easy_draw_horizon_line(a, y, b - a + 1, color);
	}
}


void draw_fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint32_t color)
{
	int16_t i, y, ymax, xmax1, xmax2, temp;
	float m1, m2;
	if (y1 > y2) {
		temp = y1; y1 = y2; y2 = temp;
		temp = x1; x1 = x2; x2 = temp;
	}
	if (y1 > y3) {
		temp = y1; y1 = y3; y3 = temp;
		temp = x1; x1 = x3; x3 = temp;
	}
	if (y2 > y3) {
		temp = y2; y2 = y3; y3 = temp;
		temp = x2; x2 = x3; x3 = temp;
	}
	m1 = (float)(x2 - x1) / (y2 - y1);
	m2 = (float)(x3 - x1) / (y3 - y1);
	for (y = y1; y <= y3; y++) {
		if (y < y2) {
			xmax1 = x1 + (m1 * (y - y1));
			xmax2 = x1 + (m2 * (y - y1));
		}
		else {
			xmax1 = x2 + (m1 * (y - y2));
			xmax2 = x3 + (m2 * (y - y3));
		}
		if (xmax1 > xmax2) {
			temp = xmax1; xmax1 = xmax2; xmax2 = temp;
		}
		for (i = xmax1; i <= xmax2; i++) {
			easy_draw_pixel(i, y, color);
		}
	}

}


void easy_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	easy_draw_line(x0, y0, x1, y1, color);
	easy_draw_line(x1, y1, x2, y2, color);
	easy_draw_line(x2, y2, x0, y0, color);
}

void easy_draw_triangle2(ed_point_t* points, uint32_t color)
{
	easy_draw_line(points[0].x, points[0].y, points[1].x, points[1].y, color);
	easy_draw_line(points[1].x, points[1].y, points[2].x, points[2].y, color);
	easy_draw_line(points[2].x, points[2].y, points[0].x, points[0].y, color);
}

//void easy_draw_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
//{
//	easy_draw_line(x0, y0, x1, y1, color);
//	easy_draw_line(x1, y1, x2, y2, color);
//	easy_draw_line(x2, y2, x0, y0, color);
//	ed_point_t polygon[3] = { 0 };
//	polygon[0].x = x0;
//	polygon[0].y = y0;
//	polygon[1].x = x1;
//	polygon[1].y = y1;
//	polygon[2].x = x2;
//	polygon[2].y = y2;
//
//	easy_draw_fillPolygon(polygon, 3, color);
//}


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

