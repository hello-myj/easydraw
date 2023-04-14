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