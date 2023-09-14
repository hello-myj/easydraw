#include "easy_menu.h"
#include "easy_draw.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static easy_menu_t sg_easy_menu = { 0 };
static easy_menu_ctrl_t g_easy_menu_ctrl = { 0 };


uint8_t static_item_count(static_menu_array_t * menu_nodes)
{
	int count = 0;
	for (;;)
	{
		if (menu_nodes[count++].node_name == NULL)
		{
			return count-1;
		}		
	}
}
//¾²Ì¬Ìí¼Ó×ÓÀý
void static_add_menu_items(static_menu_array_t *menu_nodes, easy_menu_t * parent_node,uint8_t depth)
{
	int count = static_item_count(menu_nodes);
	parent_node->member_num = count;
	parent_node->member = (ui_menu_item_t * )calloc(count,sizeof(ui_menu_item_t));
	UI_MALLOC_CHECK(parent_node->member);
	
	for(int i=0;i< count;i++)
	{
		ui_menu_item_t* item = &parent_node->member[i];

		item->icon = menu_nodes[i].icon;
		item->member_name = menu_nodes[i].node_name;
		item->member_type = MEMBER_TYPE_NULL;
		item->par_menu = parent_node;
		item->sub_menu = NULL;
		if (menu_nodes[i].statc_sub_menu_array != NULL)
		{
			item->sub_menu = (easy_menu_t*)calloc(1,sizeof(easy_menu_t));
			UI_MALLOC_CHECK(item->sub_menu);

			((easy_menu_t*)item->sub_menu)->show_type = menu_nodes[i].sub_menu_show_type;
			static_add_menu_items(menu_nodes[i].statc_sub_menu_array, item->sub_menu, depth+1);
			item->member_type = MEMBER_TYPE_OPEN;
		}
	}
}

void easy_menu_init(static_menu_array_t* menu_nodes, ui_menu_show_type show_type)
{
	sg_easy_menu.show_type = show_type;
	static_add_menu_items(menu_nodes, &sg_easy_menu,0);
	g_easy_menu_ctrl.cur_mov_node = &sg_easy_menu;
	g_easy_menu_ctrl.draw_state = UI_DRAW_UPDATE_READY;
}


static void easy_menu_draw_show_normal()
{
	uint8_t buffer[128 * 8] = { 0 };
	ed_helper_buffer_t* ed_buffer = easy_draw_create_buffer(128, 64, buffer);
	if (ed_buffer == NULL)
		return;
	easy_draw_set_buffer_active(ed_buffer);

	int count = g_easy_menu_ctrl.cur_mov_node->member_num;
	for (int i = 0; i < count; i++)
	{
		easy_draw_text_string(0, i * 8, g_easy_menu_ctrl.cur_mov_node->member[i].member_name, 1);
	}
	easy_draw_buffer_cover(0, 0, ed_buffer);
	easy_draw_destroy_buffer(&ed_buffer);
}


//»æÖÆ
static void easy_menu_exec_drawing(void)
{

	switch (g_easy_menu_ctrl.cur_mov_node->show_type)
	{
	case UI_LIST_NORMAL:
		easy_menu_draw_show_normal();
		break;
	case UI_LIST_ICON_TEXT:
		break;
	case UI_LIST_ICON_BIG:
		break;
	case UI_LIST_ICON_MEDIUM:
		break;
	case UI_LIST_ICON_SMALL:
		break;
	default:
		break;
	}	
}
//äÖÈ¾
static void easy_menu_exec_render(void)
{


}


void easy_menu_run()
{

	switch (g_easy_menu_ctrl.draw_state)
	{
	case UI_DRAW_UPDATE_READY:
		easy_menu_exec_drawing();
		break;
	case UI_DRAW_UPDATE_COMPLETE:
		//easy_menu_exec_render();
		break;
	default:
		break;
	}
}



//uint8_t buffer[128 * 8] = { 0 };
//ed_helper_buffer_t* ed_buffer = easy_draw_create_buffer(128, 64, 1, buffer);
//if (ed_buffer == NULL)
//return;
//easy_draw_set_buffer_active(ed_buffer);
//
//int count = g_easy_menu_ctrl.cur_mov_node->member_num;
//for (int i = 0; i < count; i++)
//{
//	easy_draw_text_string(0, i * 8, g_easy_menu_ctrl.cur_mov_node->member[i].member_name, 1);
//}
//easy_draw_buffer_cover(0, 0, ed_buffer);
//easy_draw_destroy_buffer(&ed_buffer);