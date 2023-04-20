#include "easy_draw_ex.h"





void easy_draw_fill_sector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color)
{

}
//扫描线填充算法
void easy_draw_scanLineFill(int x, int y, uint32_t color)
{


	//int pRight, pLeft;
	//std::stack<CPoint> stk;
	//int mColor = pDC->GetPixel(x, y);	//给定种子

	//stk.push(CPoint(x, y));
	//while (!stk.empty()) {


	//	CPoint p = stk.top();	//栈顶像素出栈
	//	stk.pop();

	//	pRight = SetRP(p.x, p.y, color, mColor, pDC);	//向左向右进行填充
	//	pLeft = SetLP(p.x, p.y, color, mColor, pDC);

	//	//上下两条扫描线处理
	//	NewLineSeed(&stk, pLeft, pRight, p.y + 1, color, mColor, pDC);
	//	NewLineSeed(&stk, pLeft, pRight, p.y - 1, color, mColor, pDC);
	//}
}
// 
//逐点法多边形绘制

// 比较两个点的x坐标
static int easy_helper_comparePoints(const void* a, const void* b)
{
	ed_point_t* point_a = (ed_point_t*)a;
	ed_point_t* point_b = (ed_point_t*)b;

	return point_a->x - point_b->x;
}

void easy_drawex_fillPolygon(ed_point_t* points, uint16_t num_points, uint32_t color)
{
	if (!easy_helper_is_convex_polygon(points, num_points)) {
		return;
	}

	// 找到最小和最大的y坐标
	int16_t min_y = points[0].y;
	int16_t max_y = points[0].y;
	for (uint16_t i = 1; i < num_points; i++) {
		if (points[i].y < min_y) {
			min_y = points[i].y;
		}
		else if (points[i].y > max_y) {
			max_y = points[i].y;
		}
	}

	// 创建堆上的交点数组
	ed_point_t* intersections = malloc(sizeof(ed_point_t) * num_points);
	uint16_t num_intersections = 0;

	// 对于每一行扫描线，找到相交的线段并填充
	for (int16_t y = min_y; y <= max_y; y++) {
		num_intersections = 0;

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

			// 计算交点的x坐标并将其加入交点数组中
			intersections[num_intersections].x = x0 + (int32_t)(x1 - x0) * (int32_t)(y - y0) / (int32_t)(y1 - y0);
			intersections[num_intersections].y = y;
			num_intersections++;
		}

		// 对交点数组进行排序
		qsort(intersections, num_intersections, sizeof(ed_point_t), easy_helper_comparePoints);

		// 填充每一对相邻的交点之间的区域
		for (uint16_t i = 0; i < num_intersections - 1; i += 2) {
			easy_draw_line(intersections[i].x, y, intersections[i + 1].x, y, color);
		}
	}

	// 释放交点数组
	free(intersections);
}

