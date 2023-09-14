#include "easy_button.h"

static ui_button_t ui_button[UI_BUTTON_MAX] = { 0 };
static ui_button_evt_cb button_evt_cb = NULL;




void ui_button_run(int delayms)
{
    if (button_evt_cb == NULL)
        return;
    for (int i = 0; i < UI_BUTTON_MAX; i++)
    {
        if (ui_button[i].ui_button_enable == 0)
        {
            continue;
        }
        // int button_state = ui_button[i].cb_callback(ui_button[i].p_button);
        if (ui_button[i].cb_callback(ui_button[i].p_button) == UI_BUTTON_PRESS)
        {
            button_evt_cb(i, UI_BUTTON_PRESS);
            ui_button[i].last_pressed = UI_BUTTON_PRESS;
            // 长按
            if (ui_button[i].press_times * delayms > UI_BUTTION_LONG_PRESS_TIME)
            {
                button_evt_cb(i, UI_BUTTON_LONG_PRESS);
            }
            else
            {
                ui_button[i].press_times++;
            }
        }
        else if (ui_button[i].cb_callback(ui_button[i].p_button) == UI_BUTTON_RELEASE)
        {
            ui_button[i].press_times = 0;
            if (ui_button[i].last_pressed == UI_BUTTON_PRESS)
            {
                button_evt_cb(i, UI_BUTTON_RELEASE);
                ui_button[i].click_times++;
                ui_button[i].last_pressed = UI_BUTTON_RELEASE;
            }
        }

        // 短按双击判断、单击判断
        if (ui_button[i].click_start_times * delayms > UI_BUTTION_SHORT_PRESS_TIME)
        {

            if (ui_button[i].click_times == 1)
            {
                button_evt_cb(i, UI_BUTTON_CLICK);
            }
            else if (ui_button[i].click_times == 2)
            {
                button_evt_cb(i, UI_BUTTON_DOUBLE_CLICK);
            }
            ui_button[i].click_start_times = 0;
            ui_button[i].click_times = 0;
        }

        if (ui_button[i].click_times > 0)
        {
            ui_button[i].click_start_times++;
        }
    }
}

int ui_button_read_level(ui_button_type button_type)
{
    if (button_type >= UI_BUTTON_MAX)
        return -1;
    return ui_button[button_type].cb_callback(ui_button[button_type].p_button);
}

void ui_button_cb_regist(ui_button_type button, void* p_button, ui_button_cb cb)
{
    if ((uint32_t)button >= UI_BUTTON_MAX)
        return;
    memset(&ui_button[(int)button], 0, sizeof(ui_button_t));
    ui_button[(int)button].p_button = p_button;
    ui_button[(int)button].cb_callback = cb;
    ui_button[(int)button].ui_button_enable = 1;
    ui_button[(int)button].last_pressed = -1; // 防止初始化后，进入按键
}

void ui_button_evt_regist(ui_button_evt_cb cb)
{
    button_evt_cb = cb;
}