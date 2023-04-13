//DDA 算法
void easy_draw_line(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end,uint32_t color)
{
	// 计算斜率
	float dx = x_end - x_start;
	float dy = y_end - y_start;
	float slope = fabs(dy / dx);

	// 计算步长
	float x_step = (dx > 0) ? 1 : -1;
	float y_step = (dy > 0) ? 1 : -1;

	// 初始化坐标值
	float x = x_start + 0.5;
	float y = y_start + 0.5;

	// 绘制起点
	easy_draw_pixel((int16_t)x, (int16_t)y, color);

	// 绘制直线
	if (slope <= 1) {
		// 斜率小于等于 1，以 x 为步长递增
		float p = fabs(2 * dy / dx) - 1;
		for (int i = 0; i < fabs(dx); i++) {
			if (p >= 0) {
				y += y_step;
				p -= 2;
			}
			x += x_step;
			p += 2 * fabs(dy / dx);
			easy_draw_pixel((int16_t)x, (int16_t)y, color);
		}
	}
	else {
		// 斜率大于 1，以 y 为步长递增
		float p = fabs(2 * dx / dy) - 1;
		for (int i = 0; i < fabs(dy); i++) {
			if (p >= 0) {
				x += x_step;
				p -= 2;
			}
			y += y_step;
			p += 2 * fabs(dx / dy);
			easy_draw_pixel((int16_t)x, (int16_t)y, color);
		}
	}
}

//时钟相关
static uint32_t sg_sys_time = 0;
static volatile uint8_t sg_tick_irq_flag;

void easy_draw_tick_inc(uint8_t tick_val)
{

	sg_tick_irq_flag = 0;
	sg_sys_time += tick_val;
}

uint32_t easy_draw_tick_get()
{

	uint32_t result;
	do {

		sg_tick_irq_flag = 1;
		result = sg_sys_time;

	} while (!sg_tick_irq_flag);
	return result;
}


uint32_t easy_draw_tick_elaps(uint32_t prev_tick)
{
	uint32_t act_time = easy_draw_tick_get();

	if (act_time >= prev_tick) 
	{
		prev_tick = act_time - prev_tick;
	}
	else 
	{
		prev_tick = UINT32_MAX - prev_tick + 1;
		prev_tick += act_time;
	}

	return prev_tick;
}


//
//void easy_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color) {
//	int16_t dx = abs(x1 - x0);
//	int16_t dy = abs(y1 - y0);
//	int16_t sx = x0 < x1 ? 1 : -1;
//	int16_t sy = y0 < y1 ? 1 : -1;
//	int16_t err = dx - dy;
//
//	while (x0 != x1 || y0 != y1) {
//		float intensity = sqrt((dx * dx + dy * dy) - (err * err)) / sqrt(dx * dx + dy * dy);
//		easy_draw_pixel(x0, y0, color + (int)(255 * intensity) * 0x010101);
//		int16_t e2 = 2 * err;
//		if (e2 > -dy) {
//			err -= dy;
//			x0 += sx;
//		}
//		if (e2 < dx) {
//			err += dx;
//			y0 += sy;
//		}
//	}
//	easy_draw_pixel(x0, y0, color);
//}



//void easy_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color) {
//	bool steep = abs(y1 - y0) > abs(x1 - x0);
//	if (steep) {
//		ED_SWAP(x0, y0, int16_t);
//		ED_SWAP(x1, y1, int16_t);
//	}
//	if (x0 > x1) {
//		ED_SWAP(x0, x1, int16_t);
//		ED_SWAP(y0, y1, int16_t);
//	}
//	int16_t dx = x1 - x0;
//	int16_t dy = abs(y1 - y0);
//	int16_t err = dx / 2;
//	int16_t ystep = (y0 < y1) ? 1 : -1;
//	int16_t y = y0;
//	for (int16_t x = x0; x <= x1; x++) {
//		if (steep) {
//			easy_draw_pixel(y, x, color);
//		}
//		else {
//			easy_draw_pixel(x, y, color);
//		}
//		err -= dy;
//		if (err < 0) {
//			y += ystep;
//			err += dx;
//		}
//		if (err > 0) {
//			y -= ystep;
//			err -= dx;
//		}
//	}
//}
//
//void easy_draw_line1(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color) 
//{
//
//	if (ED_IS_DRIVER_NULL(sg_ed_driver, ed_line_draw) == false)
//	{
//		sg_ed_driver->ed_line_draw(x0, y0, x1, y1, color);
//		return;
//	}
//
//	int16_t dx = abs(x1 - x0);
//	int16_t dy = abs(y1 - y0);
//	int16_t x_step = (x0 < x1) ? 1 : -1;
//	int16_t y_step = (y0 < y1) ? 1 : -1;
//
//	// 计算斜率大于等于 1 的情况下，dx 和 dy 的交换
//	int16_t temp;
//	int16_t swap_flag = 0;
//	if (dy > dx) {
//		temp = dx;
//		dx = dy;
//		dy = temp;
//		swap_flag = 1;
//	}
//
//	// 初始化计数器和累加器
//	int16_t count = 0;
//	int16_t acc = dy;
//
//	// 初始化坐标值
//	int16_t x = x0;
//	int16_t y = y0;
//
//	// 绘制起点
//	easy_draw_pixel(x, y, color);
//
//	// 绘制直线
//	while (count < dx) {
//		count++;
//		if (acc >= dx) {
//			if (swap_flag) {
//				x += x_step;
//			}
//			else {
//				y += y_step;
//			}
//			acc -= dx;
//		}
//		if (swap_flag) {
//			y += y_step;
//		}
//		else {
//			x += x_step;
//		}
//		easy_draw_pixel(x, y, color);
//		acc += dy;
//	}
//}



///
//static void EasyDraw_LogPrintf(const char* fmt, ...)
//{
//#if ED_LOG_ENABLE==1
//
//	va_list args;
//	if (sg_easy_draw_ctrl.p_user_drv == NULL)
//	{
//		return;
//	}
//
//	if (sg_easy_draw_ctrl.p_user_drv->ed_log_printf != NULL)
//	{
//		char  sg_text_buf[MAX_LOG_MSG_LEN + 1] = {0};
//		va_start(args, fmt);
//		vsnprintf(sg_text_buf, MAX_LOG_MSG_LEN, fmt, args);
//		va_end(args);
//
//		sg_easy_draw_ctrl.p_user_drv->ed_log_printf(sg_text_buf);
//	}
//	else
//	{
//		va_start(args, fmt);
//		vprintf(fmt, args);
//		va_end(args);
//		//fflush(1);
//	}
//	#endif
//}


//static int EasyDraw_SetRotate(uint16_t rotate_val)
//{
//	switch (rotate_val)
//	{
//	case 0:
//		break;
//	case 90:
//		break;
//	case 180:
//		break;
//	case 270:
//		break;
//	default:
//		EasyDraw_LogPrintf("The input parameters are incorrect");
//		return ED_ERR_PARAM_INVAIL;
//	}
//	return ED_SUCCESS;
//}