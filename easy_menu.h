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
    UI_LIST_NORMAL,         //���ı��б���ʾ
    UI_LIST_ICON_TEXT,       //ͼ��+�����б� 
    UI_LIST_ICON_BIG,       //��ͼ�꣨ȫ����
    UI_LIST_ICON_MEDIUM,    //�е�ͼ�꣨��   
    UI_LIST_ICON_SMALL,     //Сͼ��  ����
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
    MEMBER_TYPE_EXEC,       //��ִ��
    MEMBER_TYPE_OPEN,     //�ɴ򿪣�������һ��

}menu_member_type;


typedef struct _ui_menu_item
{
    ui_icon_t* icon;   //ͼ��
    char* member_name; //��Ա����
    menu_member_type member_type;  //��ǰ���ͣ��������ɴ򿪣���ִ��
    void* sub_menu;  //�Ӳ˵�
    void* par_menu;  //�����˵�
}ui_menu_item_t;

typedef struct _easy_menu
{
    ui_menu_show_type show_type;  //չʾ����
    uint8_t cur_menu_depth;       //��ǰ���
    ui_menu_item_t* member;       //��Ա
    uint8_t member_num;           //�˵���Ŀ����
    uint8_t member_index;        //ָ��λ��
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
//    ui_module_type type; //�������
//    uint8_t isActive;    //�Ƿ񼤻�
//    void* module;        //���
//}ui_module_t;
//
//
//typedef struct _ui_node
//{
//    ui_module_t*module;     //���
//    struct _ui_node* prev;  //ǰ��ָ��
//    struct _ui_node* next;  //���ָ��
//}ui_node_t;


#endif //__MJ_EASY_DRAW_H__

