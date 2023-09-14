#include "easy_menu.h"
#include "easy_button.h"
#include "easy_draw.h"

#include <stdio.h>
#include "windows.h"
#pragma comment(lib,"Winmm.lib")


static_menu_array_t g_menu_arry_sub3[] =
{
	{"3test1",NULL,NULL},
	{"3test2",NULL,NULL},
	{"3test3",NULL,NULL},
	{"3test4",NULL,NULL},
	{"3test5",NULL,NULL},
	{NULL,NULL,NULL},
};

static_menu_array_t g_menu_arry_sub2[] =
{
	{"2test1",NULL,NULL},
	{"2test2",NULL,NULL},
	{"2test3",NULL,NULL},
	{"2test4",NULL,NULL},
	{"2test5",NULL,g_menu_arry_sub3,UI_LIST_NORMAL},
	{NULL,NULL,NULL},
};

static_menu_array_t g_menu_arry[] =
{
	{"test1",NULL,g_menu_arry_sub2,UI_LIST_NORMAL},
	{"test2",NULL,NULL},
	{"test3",NULL,NULL},
	{"test4",NULL,NULL},
	{"test5",NULL,NULL},
	{"test6",NULL,NULL},
	{"test7",NULL,NULL},
	{"test8",NULL,NULL},
	{NULL,NULL,NULL},
};


void ui_button_evt_handler(ui_button_type button_type, int button_act)
{
    switch (button_type)
    {
    case UI_BUTTON_UP:
        if (button_act == UI_BUTTON_RELEASE || button_act == UI_BUTTON_LONG_PRESS)
        {

        }
        break;
    case UI_BUTTON_DWN:
        if (button_act == UI_BUTTON_RELEASE || button_act == UI_BUTTON_LONG_PRESS)
        {

        }

        break;
    case UI_BUTTON_LTF:
        if (button_act == UI_BUTTON_RELEASE)
        {

        }
        break;
    case UI_BUTTON_RIT:
        if (button_act == UI_BUTTON_RELEASE)
        {

          
        }
        break;
    case UI_BUTTON_A:
        if (button_act == UI_BUTTON_CLICK)
        {

        }
        break;
    case UI_BUTTON_B:
        if (button_act == UI_BUTTON_CLICK)
        {

        }
        break;

    default:
        break;
    }
}

int sys_button_return_read(int *key)
{
    if (GetAsyncKeyState(*key))
    {
        return UI_BUTTON_PRESS;
    }
    else
    {
        return UI_BUTTON_RELEASE;
    }
}

int key_up = VK_UP;
int key_down = VK_DOWN;
int key_left = VK_LEFT;
int key_right = VK_RIGHT;
int key_A = 0x41;
int key_B = 0x42;

void easy_menu_button_init()
{
    
  ui_button_cb_regist(UI_BUTTON_UP,&key_up, sys_button_return_read);
  ui_button_cb_regist(UI_BUTTON_DWN, &key_down, sys_button_return_read);
  ui_button_cb_regist(UI_BUTTON_LTF, &key_left, sys_button_return_read);
  ui_button_cb_regist(UI_BUTTON_RIT, &key_right, sys_button_return_read);

  ui_button_evt_regist(ui_button_evt_handler);
}

int button_update_flag = 0;

void WINAPI TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    ui_button_run(10);
}

void easy_button_run_timer()
{
   timeSetEvent(10, 1, (LPTIMECALLBACK)TimerCallback, NULL, TIME_PERIODIC);
}


int easy_menu_test()
{
    easy_menu_init(g_menu_arry, UI_LIST_NORMAL);

    easy_menu_button_init();

    easy_button_run_timer();

    while (1)
    {
        easy_menu_run();
    }

    return 0;
}
