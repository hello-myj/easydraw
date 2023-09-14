#ifndef __MJ_EASY_MENU_H__
#define __MJ_EASY_MENU_H__
#include <stdint.h>


#define UI_MALLOC_CHECK(x)               \
    do                                   \
    {                                    \
        if (x == NULL)                   \
        {                                \
            printf("error: malloc\r\n"); \
            return;                      \
        }                                \
    } while (0)


typedef enum 
{
    UI_LIST_NULL=-1,
    UI_LIST_NORMAL,         //纯文本列表显示
    UI_LIST_ICON_TEXT,       //图标+名称列表 
    UI_LIST_ICON_BIG,       //大图标（全屏）
    UI_LIST_ICON_MEDIUM,    //中等图标（）   
    UI_LIST_ICON_SMALL,     //小图标  （）
}ui_menu_show_type;

typedef enum
{
    UI_DRAW_UPDATE_READY,
    UI_DRAW_UPDATE_COMPLETE,
    UI_DRAW_MENU_RUN,
}ui_draw_state_type;

typedef struct _ui_image
{
    int16_t pos_x;
    int16_t pos_y;
    uint16_t width;
    uint16_t height;
    const uint8_t* data;
} ui_image_t;

typedef struct _ui_icon
{
    ui_image_t* icon_big;    
    ui_image_t* icon_medium; 
    ui_image_t* icon_small;  
} ui_icon_t;

typedef enum
{
    MEMBER_TYPE_NULL,       // 
    MEMBER_TYPE_EXEC,       //可执行
    MEMBER_TYPE_OPEN,     //可打开，进入下一级

}menu_member_type;


typedef struct _ui_menu_item
{
    ui_icon_t* icon;   //图标
    char* member_name; //成员名称
    menu_member_type member_type;  //当前类型，正常、可打开，可执行
    void* sub_menu;  //子菜单
    void* par_menu;  //父级菜单
}ui_menu_item_t;

typedef struct _easy_menu
{
    ui_menu_show_type show_type;  //展示类型
    uint8_t cur_menu_depth;       //当前深度
    ui_menu_item_t* member;       //成员
    uint8_t member_num;           //菜单条目数量
    uint8_t member_index;        //指标位置
}easy_menu_t;



typedef struct _easy_menu_ctrl
{
    easy_menu_t *cur_mov_node;
    ui_draw_state_type draw_state;
}easy_menu_ctrl_t;


typedef struct _static_menu_array
{
    char* node_name;
    ui_icon_t* icon;
    struct _static_menu_array  *statc_sub_menu_array;
    ui_menu_show_type sub_menu_show_type;
}static_menu_array_t;


#ifdef __cplusplus
extern "C"
{
#endif

void easy_menu_init(static_menu_array_t* menu_nodes, ui_menu_show_type show_type);
void easy_menu_run();

int easy_menu_test();

#ifdef __cplusplus
}
#endif

//typedef enum
//{
//    ui_module_list,
//    
//}ui_module_type;
//
//typedef struct _ui_module
//{
//    ui_module_type type; //组件类型
//    uint8_t isActive;    //是否激活
//    void* module;        //组件
//}ui_module_t;
//
//
//typedef struct _ui_node
//{
//    ui_module_t*module;     //组件
//    struct _ui_node* prev;  //前驱指针
//    struct _ui_node* next;  //后继指针
//}ui_node_t;


#endif //__MJ_EASY_DRAW_H__

