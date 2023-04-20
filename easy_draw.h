#ifndef __MJ_EASY_DRAW_H__
#define	__MJ_EASY_DRAW_H__

#include <stdint.h>
#include <stdbool.h>


typedef struct {
    int16_t x;
    int16_t y;
}ed_point_t;

typedef struct {
    int16_t dx;
    int16_t dy;
}ed_vector_t;


typedef struct
{
  
    int      (*ed_init)(void);
    void     (*ed_pixel_draw)(int16_t x, int16_t y, uint32_t color);
    uint32_t (*ed_pixel_get)(int16_t x, int16_t y);

    void     (*ed_line_draw)(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
    void     (*ed_rect_draw)(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
    void     (*ed_rect_fill_draw)(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
    void     (*ed_arc_draw)(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color);
    void     (*ed_sector_draw)(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color);

} easy_draw_driver_t;



//typedef struct
//{
//    //void (*ed_pixel_color_invert)(bool invert);
//    void (*ed_log_printf)(const char * string);
//
//}easy_draw_user_t;


//typedef struct
//{
//    uint8_t ed_graphics_enabled;
//    easy_draw_driver_t * p_lcd_drv;
//} easy_draw_ctrl_t;


#define ED_SUCCESS (0)
#define ED_ERROR (-1)
#define ED_ERR_PARAM_INVAIL (-2)

#define ED_PI 3.141592653589793

#define ED_SWAP(a, b,type) { type temp = a; a = b; b = temp; }
#define ED_SWAP_INT(a,b)  ED_SWAP((a),(b),int)

//#define ED_SWAP(x, y ,type) \
//	(y) = (x) + (y); \
//	(x) = (y) - (x); \
//	(y) = (y) - (x);

#define ED_IS_NULL(x) ((x) == NULL ? true : false)
#define ED_IS_MEMBER_NULL(ptr, member) ((ptr)->member == NULL ? true : false)
#define ED_IS_DRIVER_NULL(ptr,member) ( ED_IS_NULL(ptr) || ED_IS_MEMBER_NULL(ptr,member) ? true : false )


//#define ED_MAX(x, y) ((x) > (y) ? (x) : (y))
//#define ED_MIN(x, y) ((x) < (y) ? (x) : (y))

#ifdef __cplusplus 
extern "C" { 
#endif


//int easy_draw_create_drawhandle(uint8_t buffer_len,)

int  easy_draw_init(easy_draw_driver_t* easy_draw_driver, uint16_t width, uint16_t height);
void easy_draw_set_brush_size(uint8_t size);

void easy_draw_pixel(int16_t x, int16_t y, uint32_t color);
void easy_draw_line(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, uint32_t color);
void easy_draw_vertical_line(int16_t x, int16_t y, uint16_t h, uint32_t color);
void easy_draw_horizon_line(int16_t x, int16_t y, uint16_t w, uint32_t color);
void easy_draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void easy_draw_fillRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);

void easy_draw_arc(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color);
void easy_draw_sector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color);
void easy_draw_fillSector(int xc, int yc, int r, int start_angle, int end_angle, uint32_t color);

void easy_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
void easy_draw_fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

void easy_draw_polygon(ed_point_t* points, uint16_t num_points, uint32_t color);
void easy_draw_fillPolygon(ed_point_t* points, uint16_t num_points, uint32_t color);


ed_point_t easy_helper_rotate_point(ed_point_t point, ed_point_t center, uint16_t angle_degrees);
int easy_helper_is_polygon(ed_point_t* points, int num_points);

#ifdef __cplusplus
}
#endif



#endif // !__MJ_EASY_DRAW_H__

