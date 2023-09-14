#ifndef __MJ_EASY_BUTTON_H__
#define __MJ_EASY_BUTTON_H__
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>

typedef enum
{
    UI_BUTTON_LTF, //
    UI_BUTTON_RIT, //
    UI_BUTTON_DWN, //
    UI_BUTTON_UP,  //
    UI_BUTTON_A,
    UI_BUTTON_B,
    UI_BUTTON_L1,
    UI_BUTTON_R1,
    UI_BUTTON_MAX,
} ui_button_type;

typedef int (*ui_button_cb)(void* p_button);
typedef int (*ui_button_cb)(void* p_button);

typedef void (*ui_button_evt_cb)(ui_button_type button_type, int button_act);

typedef struct _ui_button
{
    void* p_button;
    ui_button_cb cb_callback;
    int last_pressed;
    bool ui_button_enable;
    int press_times;
    uint32_t click_times;
    uint32_t click_start_times;
} ui_button_t;

#define UI_BUTTON_PRESS 0
#define UI_BUTTON_RELEASE 1
#define UI_BUTTON_CLICK 2
#define UI_BUTTON_DOUBLE_CLICK 3
#define UI_BUTTON_LONG_PRESS 4

#define UI_BUTTION_SHORT_PRESS_TIME 300 // ms
#define UI_BUTTION_LONG_PRESS_TIME 1000 // ms

#ifdef __cplusplus
extern "C"
{
#endif

void ui_button_cb_regist(ui_button_type button, void* p_button, ui_button_cb cb);
void ui_button_evt_regist(ui_button_evt_cb cb);
void ui_button_run(int delayms);

#ifdef __cplusplus
}
#endif

#endif // ! __EASY_BUTTON_H__

