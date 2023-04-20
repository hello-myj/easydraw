
#include "easy_draw.h"


static int8_t sg_ed_graphics_enabled;
static uint16_t sg_screen_width=0;
static uint16_t sg_screen_height = 0;
static uint8_t sg_ed_brush_size = 1;
static easy_draw_driver_t* sg_ed_driver = NULL;

////计算向量的长度
//double easy_draw_vector_length(ed_vector_t v) 
//{
//	return sqrt(v.dx * v.dx + v.dy * v.dy);
//}
//
//// 将向量归一化为单位向量
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


// 将向量旋转给定角度
static ed_vector_t easy_helper_rotate_vector(ed_vector_t v, uint16_t angle_degrees)
{
	// 将角度转换为弧度
	double angle_radians = angle_degrees * ED_PI / 180.0;

	// 应用旋转矩阵
	ed_vector_t result = 
	{
		.dx = v.dx * cos(angle_radians) - v.dy * sin(angle_radians),
		.dy = v.dx * sin(angle_radians) + v.dy * cos(angle_radians)
	};

	return result;
}


// 将点围绕中心点旋转一定角度，并返回新的坐标
ed_point_t easy_helper_rotate_point(ed_point_t point, ed_point_t center, uint16_t angle_degrees)
{
	// 计算方向向量
	ed_vector_t direction = 
	{
		.dx = point.x - center.x,
		.dy = point.y - center.y
	};

	//double length = easy_draw_vector_length(direction);

	//direction = easy_draw_normalize_vector(direction);

	// 将方向向量旋转给定角度
	ed_vector_t rotated_direction = easy_helper_rotate_vector(direction, angle_degrees);

	// 将旋转后的方向向量与中心点相加，得到新的坐标点
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

//笔刷建议使用奇数
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

	// 绘制左边线
	easy_draw_line(x, y, x, y + height - 1, color);

	// 绘制下边线
	easy_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color);

	// 绘制右边线
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

	// 将角度转换为弧度
	start_radians = start_angle * 3.1415926 / 180.0;
	end_radians = end_angle * 3.1415926 / 180.0;

	// 初始化起始点的坐标
	x = r * cos(start_radians) + xc;
	y = r * sin(start_radians) + yc;

	// 绘制圆弧
	for (radians = start_radians; radians <= end_radians; radians += 0.01) {
		// 计算下一个像素点的坐标
		x = r * cos(radians) + xc;
		y = r * sin(radians) + yc;

		// 绘制当前像素点
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

	// 将角度转换为弧度
	start_radians = start_angle * 3.1415926 / 180.0;
	end_radians = end_angle * 3.1415926 / 180.0;

	// 初始化起始点的坐标
	x = r * cos(start_radians) + xc;
	y = r * sin(start_radians) + yc;

	// 绘制圆弧
	for (radians = start_radians; radians <= end_radians; radians += 0.01) {
		// 计算下一个像素点的坐标
		x = r * cos(radians) + xc;
		y = r * sin(radians) + yc;

		// 绘制当前像素点
		easy_draw_pixel(x, y, color);
	}
	// 绘制线段
	easy_draw_line(xc, yc, r * cos(start_radians) + xc, r * sin(start_radians) + yc, color);
	easy_draw_line(xc, yc, r * cos(end_radians) + xc, r * sin(end_radians) + yc, color);
}





void easy_draw_fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3,uint32_t color) 
{
	// 为了方便，我们将三个点按 y 坐标排序
	if (y1 > y2) { ED_SWAP_INT(y1, y2); ED_SWAP_INT(x1, x2); }
	if (y1 > y3) { ED_SWAP_INT(y1, y3); ED_SWAP_INT(x1, x3); }
	if (y2 > y3) { ED_SWAP_INT(y2, y3); ED_SWAP_INT(x2, x3); }

	// 计算斜率，确保没有除以 0 的情况
	float slope1 = y2 == y1 ? 0 : (float)(x2 - x1) / (y2 - y1);
	float slope2 = y3 == y1 ? 0 : (float)(x3 - x1) / (y3 - y1);
	float slope3 = y3 == y2 ? 0 : (float)(x3 - x2) / (y3 - y2);

	// 初始化扫描线
	int scanlineY = y1;
	float leftX = x1, rightX = x1;

	// 开始扫描线
	while (scanlineY <= y3) {
		// 确定左右端点的 x 坐标
		if (scanlineY >= y2) {
			leftX += slope1;
		}
		else {
			leftX += slope2;
		}

		if (scanlineY >= y3) {
			rightX += slope2;
		}
		else {
			rightX += slope3;
		}

		// 填充扫描线
		for (int x = ceil(leftX); x < floor(rightX); ++x) {
			easy_draw_pixel(x, scanlineY, color);
		}

		// 移动到下一行
		++scanlineY;
	}
}


void easy_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	easy_draw_line(x0, y0, x1, y1, color);
	easy_draw_line(x1, y1, x2, y2, color);
	easy_draw_line(x2, y2, x0, y0, color);
}


/* 判断两条线段是否相交 */
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
	for (i = 0; i < num_points; i++) {
		j = (i + 1) % num_points;
		k = (j + 1) % num_points;
		int cross = (points[j].x - points[i].x) * (points[k].y - points[j].y) - (points[j].y - points[i].y) * (points[k].x - points[j].x);
		if (i == 0) {
			flag = cross > 0;
		}
		else if (flag != (cross > 0)) {
			return 0;  /* 存在拐向内部的情况，说明不是凸多边形 */
		}
	}
	return 1;  /* 不存在拐向内部的情况，说明是凸多边形 */
}

/* 判断坐标点数组是否能够组成一个多边形 */
int easy_helper_is_polygon(ed_point_t* points, int num_points)
{
	int i, j;
	if (num_points < 3)
		return 0;  /* 至少需要3个点才能组成多边形 */
	for (i = 0; i < num_points; i++) {
		for (j = i + 1; j < num_points; j++) 
		{
			/* 判断每条边是否和其他边相交 */
			if (easy_helper_is_intersect(points[i], points[(i + 1) % num_points], points[j], points[(j + 1) % num_points])) 
			{
				return 0;  /* 存在相交的情况，说明无法组成多边形 */
			}
		}
	}
	return 1;  /* 不存在相交的情况，说明可以组成多边形 */
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

void easy_draw_fillPolygon(ed_point_t* points, uint16_t num_points, uint32_t color)
{
	if (!easy_helper_is_convex_polygon(points, num_points))
		return;

	// 找到最小y值
	int16_t min_y = points[0].y;
	for (uint16_t i = 1; i < num_points; i++) {
		if (points[i].y < min_y) {
			min_y = points[i].y;
		}
	}

	// 找到最大y值
	int16_t max_y = points[0].y;
	for (uint16_t i = 1; i < num_points; i++) {
		if (points[i].y > max_y) {
			max_y = points[i].y;
		}
	}

	// 对于每一行扫描线，找到相交的线段并填充
	for (int16_t y = min_y; y <= max_y; y++) {
		uint16_t num_intersections = 0;
		ed_point_t intersections[2];

		// 对于每一条边，找到相交的点
		for (uint16_t i = 0; i < num_points; i++) {
			uint16_t j = (i + 1) % num_points;

			int16_t x0 = points[i].x;
			int16_t y0 = points[i].y;
			int16_t x1 = points[j].x;
			int16_t y1 = points[j].y;

			// 如果当前行和边没有交点，跳过
			if ((y < y0 && y < y1) || (y > y0 && y > y1)) {
				continue;
			}

			// 计算交点的x坐标
			int16_t x = x0 + (int32_t)(x1 - x0) * (int32_t)(y - y0) / (int32_t)(y1 - y0);

			// 将交点加入交点数组中
			if (num_intersections == 0 || x != intersections[num_intersections - 1].x) {
				intersections[num_intersections].x = x;
				intersections[num_intersections].y = y;
				num_intersections++;
			}
		}

		// 对交点进行排序
		for (uint16_t i = 0; i < num_intersections - 1; i++) {
			for (uint16_t j = i + 1; j < num_intersections; j++) {
				if (intersections[i].x > intersections[j].x) {
					ed_point_t temp = intersections[i];
					intersections[i] = intersections[j];
					intersections[j] = temp;
				}
			}
		}

		// 填充每一对相邻的交点之间的区域
		for (uint16_t i = 0; i < num_intersections - 1; i += 2) {
			int16_t x0 = intersections[i].x;
			int16_t x1 = intersections[i + 1].x;
			easy_draw_line(x0, y, x1, y, color);
		}
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

