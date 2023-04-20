#include "easy_draw_ex.h"





void easy_draw_fill_sector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color)
{

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

// �Ƚ��������x����
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

	// �ҵ���С������y����
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

	// �������ϵĽ�������
	ed_point_t* intersections = malloc(sizeof(ed_point_t) * num_points);
	uint16_t num_intersections = 0;

	// ����ÿһ��ɨ���ߣ��ҵ��ཻ���߶β����
	for (int16_t y = min_y; y <= max_y; y++) {
		num_intersections = 0;

		// ����ÿһ���ߣ��ҵ��ཻ�ĵ�
		for (uint16_t i = 0; i < num_points; i++) {
			uint16_t j = (i + 1) % num_points;

			int16_t x0 = points[i].x;
			int16_t y0 = points[i].y;
			int16_t x1 = points[j].x;
			int16_t y1 = points[j].y;

			// �����ǰ�кͱ�û�н��㣬����
			if ((y < y0 && y < y1) || (y > y0 && y > y1)) {
				continue;
			}

			// ���㽻���x���겢������뽻��������
			intersections[num_intersections].x = x0 + (int32_t)(x1 - x0) * (int32_t)(y - y0) / (int32_t)(y1 - y0);
			intersections[num_intersections].y = y;
			num_intersections++;
		}

		// �Խ��������������
		qsort(intersections, num_intersections, sizeof(ed_point_t), easy_helper_comparePoints);

		// ���ÿһ�����ڵĽ���֮�������
		for (uint16_t i = 0; i < num_intersections - 1; i += 2) {
			easy_draw_line(intersections[i].x, y, intersections[i + 1].x, y, color);
		}
	}

	// �ͷŽ�������
	free(intersections);
}

