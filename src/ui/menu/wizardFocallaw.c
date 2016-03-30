/*
 * wizardFocallaw.c
 *
 *  Created on: 2012-5-29
 *      Author: lzk
 */


#include "menuFunc.h"
#include "../../map/menuMap.h"
#include "../../string/_string.h"
#include "../../lzk/common.h"


#include <gtk/gtk.h>
#include <assert.h>
#include "../../base.h"
#include "../../drawui.h"
#include "../../main.h"
#include "../../callback.h"
#include "../authorization.h"

static void wizardFocallaw_back_press(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		//显示图标
		menu_union.pos.menu4 = pMenuData->data;//用于找label
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,pMenuData))
		{
			switch(pMenuData->data)
			{
			case WIZARD_FOCLAW_STEP_GROUP_SELECT:
			case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
			case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
			case WIZARD_FOCLAW_STEP_WAVE_TYPE:
			case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
			case WIZARD_FOCLAW_STEP_GENERATE:
				--pMenuData->data;
				break;
			case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
				--pMenuData->data;
				--pMenuData->data;
				break;

			case WIZARD_FOCLAW_STEP_MAX:
			default:
				pMenuData->data = WIZARD_FOCLAW_STEP_START;
				break;
			}
			g_debug("wizard focallaw step:%s" ,getMainDictString(MAINSTRINGDICT_STEP_FOCALLAW ,pMenuData->data));

			gCurrentMenuPos.pos.menu4 = pMenuData->data;//用于找label

			menu3Update();
		}
	}
}

static void wizardFocallaw_back_update(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		switch(pMenuData->data)
		{
		case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
		case WIZARD_FOCLAW_STEP_WAVE_TYPE:
		case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
		case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
		case WIZARD_FOCLAW_STEP_GENERATE:
		case WIZARD_FOCLAW_STEP_GROUP_SELECT:
			gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
			gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
			break;

		case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
			gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
			break;
		default:
			gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
			break;
		}
		//显示图标
		menu_union.pos.menu4 = pMenuData->data;//用于找label
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_BIGMENU ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardFocallaw_next_press(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
	//	//显示图标
		menu_union.pos.menu4 = pMenuData->data;//用于找label
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,pMenuData))
		{
			switch(pMenuData->data)
			{
			case WIZARD_FOCLAW_STEP_GROUP_SELECT:
				++pMenuData->data;
				break;

			case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
			case WIZARD_FOCLAW_STEP_WAVE_TYPE:
			case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
			case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
			case WIZARD_FOCLAW_STEP_START:
				++pMenuData->data;
				break;
			case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
				++pMenuData->data;
				++pMenuData->data;
				break;

			case WIZARD_FOCLAW_STEP_GENERATE:
				pMenuData->data = WIZARD_FOCLAW_STEP_START;
				break;

			case WIZARD_FOCLAW_STEP_MAX:
			default:
				pMenuData->data = WIZARD_FOCLAW_STEP_START;
				break;
			}
			g_debug("wizard focallaw step:%s" ,getMainDictString(MAINSTRINGDICT_STEP_FOCALLAW ,pMenuData->data));

			gCurrentMenuPos.pos.menu4 = pMenuData->data;//用于找label

			menu3Update();
		}
	}
}

static void wizardFocallaw_next_update(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int i ,num_of_group ,num_of_ut_group = 0;
		num_of_group = get_group_qty(pp->p_config);
		switch(pMenuData->data)
		{
		case WIZARD_FOCLAW_STEP_GROUP_SELECT://
	//		menu3_mode = LABELBUTTON_MODE_BIGMENU;
			gtk_widget_set_sensitive(pp->eventbox2[0],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[3],FALSE);
			gtk_widget_set_sensitive(pp->menubar,FALSE);

			for(i = 0; i < num_of_group ;++i)
			{
				if( PA != pp->p_config->group[i].group_mode )
				{
					++num_of_ut_group;
				}
			}
			break;

		case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
		case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
		case WIZARD_FOCLAW_STEP_WAVE_TYPE:
		case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
		case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
		case WIZARD_FOCLAW_STEP_GENERATE:
	//		menu3_mode = LABELBUTTON_MODE_DATA3;
			gtk_widget_set_sensitive(pp->eventbox2[0],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[3],FALSE);
			gtk_widget_set_sensitive(pp->menubar,FALSE);
			break;

		default:
			gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
			gtk_widget_set_sensitive(pp->menubar,TRUE);
			break;
		}
		//显示图标
		menu_union.pos.menu4 = pMenuData->data;//用于找label
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);

		if(num_of_ut_group == num_of_group)//没有PA的组
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);//disable next
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_BIGMENU ,LABELBUTTON_STATUS_RELEASE);
	}
}

void wizardGroup_source_press(MENU_UNION menu_union);
#define wizardFocallaw_group_select_press(x) wizardGroup_source_press(x)

//组选择，如果没有PA就禁止下一步，禁止UT的弹出选项
static void wizardFocallaw_group_select_update(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int num_of_group = get_group_qty(pp->p_config);
		int num_of_item = num_of_group;
		const char* current_label[num_of_item];
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_item);

		int i;
		int item_status = 0;
		for(i = 0; i < num_of_group ;++i)
		{
			if( PA != pp->p_config->group[i].group_mode )
			{
				item_status |= (1 << i);
				if(i == pMenuData->lastPopPos)
				{
					pMenuData->lastPopPos = i + 1;
				}
			}
		}
		pMenuData->itemStatus = item_status;
		if(pMenuData->lastPopPos >= num_of_item)//如果其他地方组的个数有变动，有可能造成上次选中的位置大于组个数
		{
			assert(num_of_item > 0);
			int lastPopPos = 0;
			for(i = 0; i < num_of_group ;++i)
			{
				if( PA != pp->p_config->group[i].group_mode )
				{
					if(i == lastPopPos)
					{
						++lastPopPos;
					}
				}else{
					break;//找到一个就退出
				}
			}
			if(lastPopPos >= num_of_item)
				lastPopPos = 0;
			pMenuData->lastPopPos = lastPopPos;
		}
		menuPopdownWithMalloc(menu_union ,current_label ,num_of_item);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

//#define wizardFocallaw_law_type_press(x)  menuPopupWithMap((x) ,-1)
static void wizardFocallaw_law_type_press(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		//pMenuData->itemStatus = (0x01 << 2) |(0x01 << 3);
		menuPopupWithMap(menu_union ,-1);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}
#define wizardFocallaw_law_type_update(x) menuPopdownWithMap((x) ,g_tmp_group_struct.law_info.Focal_type)


static void wizardFocallaw_focal_type_press(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
		{
            if ( PITCH_CATCH == g_tmp_group_struct.rx_tx_mode )
			{
				pMenuData->itemStatus = 0x0c;
			}
			else if ((g_tmp_group_struct.law_info.Focal_type == DEPTH_SCAN)
					|| (g_tmp_group_struct.law_info.Focal_type == STATIC_SCAN))
			{
				pMenuData->itemStatus = 0x1d;//0x1c;
			}
			else
			{
				pMenuData->itemStatus = 0x10;
			}
            if (g_tmp_group_struct.part.Geometry != 0) {
                pMenuData->itemStatus = 0x0d;
            }
			menuPopupWithMap(menu_union ,-1);
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}
#define wizardFocallaw_focal_type_update(x) menuPopdownWithMap((x) ,g_tmp_group_struct.law_info.Focal_point_type)

static void wizardFocallaw_position_start_press(MENU_UNION menu_union)
{
    double low, upper;
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 0.1;
	//		else if(pMenuData->step == 0.1)		pMenuData->step = 0.01;
			else								pMenuData->step = 1.0;
		}
        if ((menu_union.pos.menu3 == 2) && (menu_union.pos.menu4 == 3)) {
            RefractAngleRange(&low, &upper, &g_tmp_group_struct, 1);
            pMenuData->lower = low;
            pMenuData->upper = upper;
        } else if ((menu_union.pos.menu3 == 2) && (menu_union.pos.menu4 == 6)) {
            RefractAngleRange(&low, &upper, &g_tmp_group_struct, 0);
            pMenuData->lower = low;
            pMenuData->upper = upper;
        } else if ((menu_union.pos.menu3 == 3) && (menu_union.pos.menu4 == 6)) {
            RefractAngleRange(&low, &upper, &g_tmp_group_struct, 0);
            pMenuData->lower = low;
            pMenuData->upper = upper;
        }
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardFocallaw_position_start_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Position_start / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_position_end_press(x) wizardFocallaw_position_start_press(x)
static void wizardFocallaw_position_end_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Position_end / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_position_step_press(x) wizardFocallaw_position_start_press(x)
static void wizardFocallaw_position_step_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Position_step / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardFocallaw_offset_start_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 5.0) 			pMenuData->step = 50.0;
			else if(pMenuData->step == 50.0)	pMenuData->step = 0.5;
	//		else if(pMenuData->step == 0.1)		pMenuData->step = 0.01;
			else								pMenuData->step = 5.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardFocallaw_offset_start_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Offset_start / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_offset_end_press(x) wizardFocallaw_offset_start_press(x)
static void wizardFocallaw_offset_end_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Offset_end / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_depth_start_press(x) wizardFocallaw_offset_start_press(x)
static void wizardFocallaw_depth_start_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Depth_start / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_depth_end_press(x) wizardFocallaw_offset_start_press(x)
static void wizardFocallaw_depth_end_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			pMenuData->digit = 2;
		}
		else
		{
			pMenuData->digit = 3;
		}
		pMenuData->current_value =  g_tmp_group_struct.law_info.Depth_end / 1000.0;//?
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardFocallaw_wave_type_press(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	int material = g_tmp_group_struct.part.Material_pos ;
	if(pMenuData)
	{
		int num_of_item ;
		if(	(g_tmp_group_struct.velocity == get_material_lw(material))
			||(g_tmp_group_struct.velocity == get_material_sw(material)))
		{
			num_of_item = 2;
			if(pMenuData->lastPopPos >= 2)
			{
				pMenuData->lastPopPos = 0;
			}
		}else{
			num_of_item = 3;
		}
		if(0 == get_material_sw(material))
		{
			pMenuData->itemStatus = (1 << 1);
		}
	//	const char** current_label = NULL;
	//	current_label = malloc(num_of_item * sizeof(const char*));//使能的组个数加1个add标签
		const char* current_label[num_of_item];
		current_label[0] = g_strdup_printf("%s:%dm/s" ,getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,0) ,get_material_lw(material) / 100);
		current_label[1] = g_strdup_printf ("%s:%dm/s",getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,1) ,get_material_sw(material) / 100);
		if(3 == num_of_item)
		{
			current_label[2] = g_strdup_printf ("%d m/s", g_tmp_group_struct.velocity / 100);
		}
		menuPopupWithMalloc(menu_union ,current_label ,num_of_item);
	//	free(current_label);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardFocallaw_wave_type_update(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	int material = g_tmp_group_struct.part.Material_pos ;
	if(pMenuData)
	{
		int num_of_item ;
		if(g_tmp_group_struct.velocity == get_material_lw(material))
		{
			num_of_item = 2;
			pMenuData->lastPopPos = 0;
		}
		if(g_tmp_group_struct.velocity == get_material_sw(material))
		{
			num_of_item = 2;
			pMenuData->lastPopPos = 1;
		}else{
			num_of_item = 3;
			pMenuData->lastPopPos = 2;
		}
	//	const char** current_label = NULL;
	//	current_label = malloc(num_of_item * sizeof(const char*));//使能的组个数加1个add标签
		const char* current_label[num_of_item];
		current_label[0] = g_strdup_printf("%s:%dm/s" ,getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,0) ,get_material_lw(material) / 100);
		current_label[1] = g_strdup_printf ("%s:%dm/s",getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,1) ,get_material_sw(material) / 100);
		if(3 == num_of_item)
		{
			current_label[2] = g_strdup_printf ("%d m/s", g_tmp_group_struct.velocity / 100);
		}

		menuPopdownWithMalloc(menu_union ,current_label ,num_of_item);

		if (get_auto_focal (pp->p_config) != AUTO_FOCAL_ON)
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_material_press(x)  menuPopupWithMap((x) ,0)
#define wizardFocallaw_material_update(x) menuPopdownWithMap((x) ,g_tmp_group_struct.part.Material_pos)

static void wizardFocallaw_element_qty_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);

	/* 计算最大激发阵元数 */
	float upper =  MIN(gMaxElementTriggerQty, g_tmp_group_struct.probe.Elem_qty);
	upper =  MIN(upper , MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) - g_tmp_group_struct.law_info.First_tx_elem + 1);
	pMenuData->upper = upper ;
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 100.0;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardFocallaw_element_qty_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		// 计算最大激发阵元数
		//pMenuData->upper = MIN( MIN(gMaxElementTriggerQty , g_tmp_group_struct.probe.Elem_qty),
		//		(g_tmp_group_struct.probe.Elem_qty - g_tmp_group_struct.law_info.First_tx_elem + 1));

		float upper =  MIN(gMaxElementTriggerQty, g_tmp_group_struct.probe.Elem_qty);
		upper =  MIN(upper , MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) - g_tmp_group_struct.law_info.First_tx_elem + 1);
		pMenuData->upper = upper ;
		pMenuData->current_value = g_tmp_group_struct.law_info.Elem_qty;//更新需要读取数据

		menuSpinRelease(menu_union);

		if (get_auto_focal (pp->p_config) != AUTO_FOCAL_ON)
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

//#define wizardFocallaw_first_element_press(x) wizardFocallaw_element_qty_press(x)

static void wizardFocallaw_first_element_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);

	/* 计算最大激发阵元数 */
	float upper =  (float)(g_tmp_group_struct.probe.Elem_qty - g_tmp_group_struct.law_info.Elem_qty + 1) ;
	upper = MIN(upper , gMaxElementRecieveQty + 1 - g_tmp_group_struct.law_info.Elem_qty) ;
	pMenuData->upper = upper ;
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 100.0;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}


static void wizardFocallaw_first_element_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		// 计算最大值
		//pMenuData->upper = MIN((g_tmp_group_struct.probe.Elem_qty - g_tmp_group_struct.law_info.Elem_qty + 1)
		//					,(gMaxElementRecieveQty + 1 - g_tmp_group_struct.pulser1));

		float upper =  (float)(g_tmp_group_struct.probe.Elem_qty - g_tmp_group_struct.law_info.Elem_qty + 1) ;
		upper = MIN(upper , gMaxElementRecieveQty + 1 - g_tmp_group_struct.law_info.Elem_qty) ;
		pMenuData->upper = upper ;

		pMenuData->current_value = g_tmp_group_struct.law_info.First_tx_elem;//更新需要读取数据

		menuSpinRelease(menu_union);

		if (get_auto_focal (pp->p_config) != AUTO_FOCAL_ON)
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

//#define wizardFocallaw_last_element_press(x) wizardFocallaw_element_qty_press(x)
static void wizardFocallaw_last_element_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);

	/* 计算最大激发阵元数 */
	float lower = g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty - 1;
	float upper = MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) ;
	pMenuData->upper = upper ;
	pMenuData->lower = lower ;
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 100.0;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardFocallaw_last_element_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		float lower = g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty - 1;
		float upper = MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) ;

		//pMenuData->lower = g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty - 1;

		pMenuData->upper = upper ; //MIN(g_tmp_group_struct.probe.Elem_qty ,gMaxElementRecieveQty - g_tmp_group_struct.pulser1 + 1);
		pMenuData->lower = lower ; //MIN(pMenuData->upper , gMaxElementRecieveQty - g_tmp_group_struct.receiver1 + 1) ;

		pMenuData->current_value = g_tmp_group_struct.law_info.Last_tx_elem;//更新需要读取数据

		menuSpinRelease(menu_union);

		// 聚焦法则自动计算开启时
		if ((get_auto_focal (pp->p_config) != AUTO_FOCAL_ON)
		//	||	(LAW_VAL(Focal_type) != LINEAR_SCAN))
			||	(g_tmp_group_struct.law_info.Focal_type != LINEAR_SCAN))
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}
//upper = MIN(gMaxElementRecieveQty , GROUP_VAL_POS (grp , probe.Elem_qty)) ;
//#define wizardFocallaw_element_step_press(x) wizardFocallaw_element_qty_press(x)
static void wizardFocallaw_element_step_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);

	/* 计算最大激发阵元数 */
	float upper = MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) ;
	pMenuData->upper = upper ;
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 100.0;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardFocallaw_element_step_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		float upper = MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) ;
		pMenuData->upper = upper ; //g_tmp_group_struct.probe.Elem_qty;

		pMenuData->current_value = g_tmp_group_struct.law_info.Elem_step;//更新需要读取数据

		menuSpinRelease(menu_union);

		if ((get_auto_focal (pp->p_config) != AUTO_FOCAL_ON)
		//	||	(LAW_VAL(Focal_type) == LINEAR_SCAN))
			||	(g_tmp_group_struct.law_info.Focal_type != LINEAR_SCAN))
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_min_angle_press(x) wizardFocallaw_position_start_press(x)
static void wizardFocallaw_min_angle_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
	/*	float temp_beam = 0.0;
		// 最大不能超过最大Angle_max
		// 计算lower为妙
		temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
	//	TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) -
	//			LAW_VAL(Angle_beam_skew_min)) /
	//		LAW_VAL(Angle_beam_skew_step) + 1;
		TMP(beam_skew_num)	= (g_tmp_group_struct.law_info.Angle_beam_skew_max -
				g_tmp_group_struct.law_info.Angle_beam_skew_min) /
			g_tmp_group_struct.law_info.Angle_beam_skew_step + 1;
		temp_beam = temp_beam / TMP(beam_skew_num);

		pMenuData->lower =  MAX (((gint)(g_tmp_group_struct.law_info.Angle_max) -
				(gint)(temp_beam * g_tmp_group_struct.law_info.Angle_step)) / 100.0, -89.9);

		pMenuData->upper = g_tmp_group_struct.law_info.Angle_max / 100.0;//*/

		pMenuData->current_value = g_tmp_group_struct.law_info.Angle_min / 100.0;//更新需要读取数据

		menuSpinRelease(menu_union);

		if (get_auto_focal (pp->p_config) != AUTO_FOCAL_ON)
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_max_angle_press(x) wizardFocallaw_min_angle_press(x)
static void wizardFocallaw_max_angle_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
	/*	float temp_beam = 0.0;

		temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
	//	TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) -
	//			LAW_VAL(Angle_beam_skew_min)) /
	//		LAW_VAL(Angle_beam_skew_step) + 1;
		TMP(beam_skew_num)	= (g_tmp_group_struct.law_info.Angle_beam_skew_max -
				g_tmp_group_struct.law_info.Angle_beam_skew_min) /
				g_tmp_group_struct.law_info.Angle_beam_skew_step + 1;
		temp_beam = temp_beam / TMP(beam_skew_num);

		pMenuData->upper = MIN ((gint)(g_tmp_group_struct.law_info.Angle_min +
				(gint)(temp_beam * g_tmp_group_struct.law_info.Angle_step)) / 100.0, 89.9);

		pMenuData->lower = g_tmp_group_struct.law_info.Angle_min / 100.0;//*/

	//	para_union.para_float = LAW_VAL(Angle_max) / 100.0;//更新需要读取数据
		pMenuData->current_value = g_tmp_group_struct.law_info.Angle_max / 100.0;//更新需要读取数据

		menuSpinRelease(menu_union);

		if ((g_tmp_group_struct.law_info.Focal_type != AZIMUTHAL_SCAN)
			||	(get_auto_focal (pp->p_config) != AUTO_FOCAL_ON))
			// 聚焦法则自动计算开启时并且是角度扫查, Max Angle 才可调节
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_angle_step_press(x) wizardFocallaw_min_angle_press(x)
static void wizardFocallaw_angle_step_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		float temp_beam = 0.0;
		temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);

	//	TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) -
	//			LAW_VAL(Angle_beam_skew_min)) /
	//		LAW_VAL(Angle_beam_skew_step) + 1;
		TMP(beam_skew_num)	= (g_tmp_group_struct.law_info.Angle_beam_skew_max -
				g_tmp_group_struct.law_info.Angle_beam_skew_min) /
				g_tmp_group_struct.law_info.Angle_beam_skew_step + 1;
		temp_beam = temp_beam / TMP(beam_skew_num);

		pMenuData->lower = MAX (((gint)(g_tmp_group_struct.law_info.Angle_max) -
										(gint)(g_tmp_group_struct.law_info.Angle_min))
										/ (100.0 * temp_beam), 0.1);

		pMenuData->current_value = g_tmp_group_struct.law_info.Angle_step / 100.0;//更新需要读取数据

		menuSpinRelease(menu_union);

		if ((g_tmp_group_struct.law_info.Focal_type != AZIMUTHAL_SCAN)
			||	(get_auto_focal (pp->p_config) != AUTO_FOCAL_ON))
			// 角度扫查时开始自动计算聚焦法则时候可以调节
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardFocallaw_focus_depth_press(x) wizardFocallaw_element_qty_press(x)
static void wizardFocallaw_focus_depth_update(MENU_UNION menu_union)
{
	menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
/*	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->current_value = (float)get_group_val (&g_tmp_group_struct, GROUP_PULSER);//更新需要读取数据
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}//*/
}

void menu010(MENU_UNION menu_union)//0 ,1 ,0 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		wizardFocallaw_back_press(menu_union);
	}
	else
	{
		wizardFocallaw_back_update(menu_union);
	}

}

void menu011(MENU_UNION menu_union)//0 ,1 ,1 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		wizardFocallaw_next_press(menu_union);
	}
	else
	{
		wizardFocallaw_next_update(menu_union);
	}
}

void menu012(MENU_UNION menu_union)//0 ,1 ,2 ,0
{
    int ret;
	MENU_UNION menuStep = {{0 ,1 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			case WIZARD_FOCLAW_STEP_GROUP_SELECT:		wizardFocallaw_group_select_press(menu_union);		break;
			case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:	wizardFocallaw_law_type_press(menu_union);			break;
			case WIZARD_FOCLAW_STEP_WAVE_TYPE:			wizardFocallaw_wave_type_press(menu_union);			break;
			case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:		wizardFocallaw_element_qty_press(menu_union);		break;
			case WIZARD_FOCLAW_STEP_BEAM_ANGLE:			wizardFocallaw_min_angle_press(menu_union);			break;
			case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
                printf("%s %d\n", __FILE__, __LINE__);
				switch(g_tmp_group_struct.law_info.Focal_point_type)
				{
				case 0:
                    printf("%s %d\n", __FILE__, __LINE__);
				    menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE_HALF;
					wizardFocallaw_position_start_press(menu_union);
					break;
				case 1:
                    printf("%s %d\n", __FILE__, __LINE__);
				//	menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE;
					wizardFocallaw_position_start_press(menu_union);
					break;
				case 2:
                    printf("%s %d\n", __FILE__, __LINE__);
					menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
					wizardFocallaw_offset_start_press(menu_union);
					break;
				case 3:
                    printf("%s %d\n", __FILE__, __LINE__);
					menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
					wizardFocallaw_offset_start_press(menu_union);
					break;
				default:
                    printf("%s %d\n", __FILE__, __LINE__);
					break;
				}
				break;
			case WIZARD_FOCLAW_STEP_GENERATE:
				default:
				break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			case WIZARD_FOCLAW_STEP_GROUP_SELECT:		wizardFocallaw_group_select_update(menu_union);	break;
			case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:	wizardFocallaw_law_type_update(menu_union);		break;
			case WIZARD_FOCLAW_STEP_WAVE_TYPE:			wizardFocallaw_wave_type_update(menu_union);	break;
			case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:		wizardFocallaw_element_qty_update(menu_union);	break;
			case WIZARD_FOCLAW_STEP_BEAM_ANGLE:			wizardFocallaw_min_angle_update(menu_union);	break;
			case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
                printf("%s %d\n", __FILE__, __LINE__);
				switch(g_tmp_group_struct.law_info.Focal_point_type)
				{
				case 0:
                    printf("%s %d\n", __FILE__, __LINE__);
				    menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE_HALF;
					wizardFocallaw_position_start_update(menu_union);
                    break;
				case 1:
                    printf("%s %d\n", __FILE__, __LINE__);
				//	menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE;
					wizardFocallaw_position_start_update(menu_union);
					break;
				case 2:
                    printf("%s %d\n", __FILE__, __LINE__);
					menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
					wizardFocallaw_offset_start_update(menu_union);
					break;
				case 3:
                    printf("%s %d\n", __FILE__, __LINE__);
					menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
					wizardFocallaw_offset_start_update(menu_union);
					break;
				default:
                    printf("%s %d\n", __FILE__, __LINE__);
					menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
					break;
				}
				break;
			case WIZARD_FOCLAW_STEP_GENERATE:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
				break;
			}
		}
	}
}


void menu013(MENU_UNION menu_union)//0 ,1 ,3 ,0
{
	MENU_UNION menuStep = {{0 ,1 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//	case WIZARD_FOCLAW_STEP_GROUP_SELECT://
				case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:		wizardFocallaw_focal_type_press(menu_union);	break;
				case WIZARD_FOCLAW_STEP_WAVE_TYPE:				wizardFocallaw_material_press(menu_union);		break;
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:			wizardFocallaw_first_element_press(menu_union);	break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:				wizardFocallaw_max_angle_press(menu_union);		break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					switch(g_tmp_group_struct.law_info.Focal_point_type)
					{
					case 0:
					case 1:
					//	menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE;
						wizardFocallaw_position_end_press(menu_union);
						break;
					case 2:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
						wizardFocallaw_position_end_press(menu_union);
						break;
					case 3:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
						wizardFocallaw_offset_end_press(menu_union);
						break;
					default:
						break;
					}
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
				default:
					break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//	case WIZARD_FOCLAW_STEP_GROUP_SELECT://
				case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:		wizardFocallaw_focal_type_update(menu_union);		break;
				case WIZARD_FOCLAW_STEP_WAVE_TYPE:				wizardFocallaw_material_update(menu_union);			break;
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:			wizardFocallaw_first_element_update(menu_union);	break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:				wizardFocallaw_max_angle_update(menu_union);		break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					switch(g_tmp_group_struct.law_info.Focal_point_type)
					{
					case 0:
					case 1:
					//	menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE;
						wizardFocallaw_position_end_update(menu_union);
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
						break;
					case 2:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
						wizardFocallaw_position_end_update(menu_union);
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
						break;
					case 3:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
						wizardFocallaw_offset_end_update(menu_union);
						break;
					default:
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
						break;
					}
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
				default:
					menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
					break;
			}
		}
	}
}


void menu014(MENU_UNION menu_union)//0 ,1 ,4 ,0
{
	MENU_UNION menuStep = {{0 ,1 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//	case WIZARD_FOCLAW_STEP_GROUP_SELECT://
				case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:  wizardFocallaw_wave_type_press(menu_union);			break;
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:		wizardFocallaw_last_element_press(menu_union);		break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:			wizardFocallaw_angle_step_press(menu_union);		break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					switch(g_tmp_group_struct.law_info.Focal_point_type)
					{
					case 0:
					case 1:
					//	menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE;
						wizardFocallaw_position_step_press(menu_union);
						break;
					case 2:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
						wizardFocallaw_position_step_press(menu_union);
						break;
					case 3:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
						wizardFocallaw_depth_start_press(menu_union);
						break;
					default:
						break;
					}
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
				default:
					break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//	case WIZARD_FOCLAW_STEP_GROUP_SELECT://
				case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:  wizardFocallaw_wave_type_update(menu_union);	break;
			//	case WIZARD_FOCLAW_STEP_WAVE_TYPE:
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:		wizardFocallaw_last_element_update(menu_union);		break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:			wizardFocallaw_angle_step_update(menu_union);		break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					switch(g_tmp_group_struct.law_info.Focal_point_type)
					{
					case 0:
					case 1:
					//	menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE;
						wizardFocallaw_position_step_update(menu_union);
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
						break;
					case 2:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
						wizardFocallaw_position_step_update(menu_union);
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
						break;
					case 3:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
						wizardFocallaw_depth_start_update(menu_union);
						break;
					default:
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
						break;
					}
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
				default:
					menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
					break;
			}
		}
	}
}


void menu015(MENU_UNION menu_union)//0 ,1 ,5 ,0
{
	MENU_UNION menuStep = {{0 ,1 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//	case WIZARD_FOCLAW_STEP_GROUP_SELECT://
			//	case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
			//	case WIZARD_FOCLAW_STEP_WAVE_TYPE:
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:		wizardFocallaw_element_step_press(menu_union);		break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:			wizardFocallaw_focus_depth_press(menu_union);		break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					switch(g_tmp_group_struct.law_info.Focal_point_type)
					{
					case 0:
					case 1:
						break;
					case 2:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
						wizardFocallaw_depth_end_press(menu_union);
						break;
					case 3:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
						wizardFocallaw_depth_end_press(menu_union);
						break;
					default:
						break;
					}
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
				default:
					break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//	case WIZARD_FOCLAW_STEP_GROUP_SELECT://
			//	case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
			//	case WIZARD_FOCLAW_STEP_WAVE_TYPE:
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:		wizardFocallaw_element_step_update(menu_union);		break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:			wizardFocallaw_focus_depth_update(menu_union);		break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					switch(g_tmp_group_struct.law_info.Focal_point_type)
					{
					case 0:
					case 1:
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
						break;
					case 2:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2;
						wizardFocallaw_depth_end_update(menu_union);
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
						break;
					case 3:
						menu_union.pos.menu4 = WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3;
						wizardFocallaw_depth_end_update(menu_union);
						break;
					default:
						menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
						break;
					}
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
				default:
					menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
					break;
			}
		}
	}
}
