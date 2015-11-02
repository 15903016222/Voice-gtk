/*
 * wizardGroup.c
 *
 *  Created on: 2012-5-23
 *      Author: lzk
 */

#include "menuFunc.h"
#include "../../map/menuMap.h"
#include "../../string/_string.h"
#include "../../lzk/common.h"
#include "../authorization.h"
#include "../../drawui.h"
#include "../../string/mainStringDict.h"
#include <assert.h>

static void wizardGroup_back_press(MENU_UNION menu_union)
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
			case WIZARD_GROUP_STEP_MATERIAL:
			case WIZARD_GROUP_STEP_ADD:
				break;

			case WIZARD_GROUP_STEP_GROUP_SELECT:
			case WIZARD_GROUP_STEP_GROUP_MODE:
			case WIZARD_GROUP_STEP_TX_RX_MODE:
			case WIZARD_GROUP_STEP_PROBE_PE:
			case WIZARD_GROUP_STEP_WEDGE_PE:
			case WIZARD_GROUP_STEP_POSITION:
			case WIZARD_GROUP_STEP_FINISH:
				--(pMenuData->data);
				break;

			case WIZARD_GROUP_STEP_MAX:
			default:
				pMenuData->data = WIZARD_GROUP_STEP_START;
				break;
			}
			g_debug("wizard group step:%s" ,getMainDictString(MAINSTRINGDICT_STEP_GROUP ,pMenuData->data));

			gCurrentMenuPos.pos.menu4 = pMenuData->data;//用于找label

			menu3Update();
		}
	}
}

static void wizardGroup_back_update(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		switch(pMenuData->data)
		{
		case WIZARD_GROUP_STEP_GROUP_SELECT:
		case WIZARD_GROUP_STEP_GROUP_MODE:
		case WIZARD_GROUP_STEP_TX_RX_MODE:
		case WIZARD_GROUP_STEP_PROBE_PE:
		case WIZARD_GROUP_STEP_WEDGE_PE:
		case WIZARD_GROUP_STEP_POSITION:
		case WIZARD_GROUP_STEP_FINISH:
			gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
			gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
			break;

		case WIZARD_GROUP_STEP_ADD:
		case WIZARD_GROUP_STEP_MATERIAL:
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

static void wizardGroup_next_press(MENU_UNION menu_union)
{
    int _nGroupQty, index;
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
	//	//显示图标
		menu_union.pos.menu4 = pMenuData->data;//用于找label
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,pMenuData))
		{
			MENU_UNION menu_group_select = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_GROUP_SELECT}};
	        MENU_UNION menu_group_source = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_ADD}};
			switch(pMenuData->data)
			{
			case WIZARD_GROUP_STEP_GROUP_SELECT:
				//选组时如果修改数据则把数据导到临时变量g_tmp_group_struct里
				//增加组时增加完就可以了
				if(get_group_qty(pp->p_config) != popGetLastPos(menu_group_select))//不是add,跳过第2步
				{//modify
					pMenuData->data = WIZARD_GROUP_STEP_MATERIAL;
				}
				else
				{//add
                    popSetLastPos(menu_group_source, get_group_qty(pp->p_config));
					++(pMenuData->data);
				}
				break;

			case WIZARD_GROUP_STEP_START:
			case WIZARD_GROUP_STEP_MATERIAL:
			case WIZARD_GROUP_STEP_GROUP_MODE:
			case WIZARD_GROUP_STEP_TX_RX_MODE:
			case WIZARD_GROUP_STEP_PROBE_PE:
			case WIZARD_GROUP_STEP_WEDGE_PE:
			case WIZARD_GROUP_STEP_POSITION:
				++(pMenuData->data);
				break;


			case WIZARD_GROUP_STEP_ADD:
		//		pMenuData->data = WIZARD_GROUP_STEP_START;
		//		break;
	            _nGroupQty = get_group_qty(pp->p_config);
                index = popGetLastPos(menu_group_source);
	            if (index+1 == _nGroupQty) {
			        pMenuData->data = WIZARD_GROUP_STEP_MATERIAL;
                    pp->help_yn = TRUE;
                } else {
				    pMenuData->data = WIZARD_GROUP_STEP_START;
                }
				break;
			case WIZARD_GROUP_STEP_FINISH:
		//		pMenuData->data = WIZARD_GROUP_STEP_START;
		//		break;
			case WIZARD_GROUP_STEP_MAX:
			default:
				pMenuData->data = WIZARD_GROUP_STEP_START;
				break;
			}
			g_debug("wizard group step:%s" ,getMainDictString(MAINSTRINGDICT_STEP_GROUP ,pMenuData->data));

			gCurrentMenuPos.pos.menu4 = pMenuData->data;//用于找label

			menu3Update();
		}
	}
}

static void wizardGroup_next_update(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		switch(pMenuData->data)
		{
		case WIZARD_GROUP_STEP_GROUP_SELECT://显示大图标
//			gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
//			gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
//			gtk_widget_set_sensitive(pp->eventbox2[3],FALSE);
//			gtk_widget_set_sensitive(pp->menubar,FALSE);
//			break;
		case WIZARD_GROUP_STEP_ADD:
		case WIZARD_GROUP_STEP_MATERIAL:
		case WIZARD_GROUP_STEP_GROUP_MODE:
		case WIZARD_GROUP_STEP_TX_RX_MODE:
		case WIZARD_GROUP_STEP_PROBE_PE:
		case WIZARD_GROUP_STEP_WEDGE_PE:
		case WIZARD_GROUP_STEP_POSITION:
		case WIZARD_GROUP_STEP_FINISH:
	//		menu3Mode = LABELBUTTON_MODE_DATA3;
			gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[3],FALSE);
			gtk_widget_set_sensitive(pp->menubar,FALSE);
			break;

		default:
			gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
			gtk_widget_set_sensitive(pp->menubar,TRUE);
			break;
		}
		//显示图标
		menu_union.pos.menu4 = pMenuData->data;//用于找label
		if (pMenuData->data == WIZARD_GROUP_STEP_ADD) {
            MENU_UNION menu_group_source = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_ADD}};
            if (popGetLastPos(menu_group_source) == get_group_qty(pp->p_config)) {
		        menu_union.pos.menu4 = WIZARD_GROUP_STEP_MATERIAL;
            }
        }
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_BIGMENU ,LABELBUTTON_STATUS_RELEASE);
	}
}
//组选择
//包含1个add按钮
static void wizardGroup_group_select_press(MENU_UNION menu_union)
{
#if HIGH_POWER
#else
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int num_of_group = get_group_qty(pp->p_config);
		int num_of_item = num_of_group + 1;//+ ADD

		const char* current_label[num_of_item];//使能的组个数加1个add标签
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_group);
		getMainDictStrings(pMenuData->stringIndex2 ,current_label + num_of_group ,num_of_item - num_of_group);
        //if (num_of_group==1) {
        //    pMenuData->itemStatus = 4;
        //}
		menuPopupWithMalloc(menu_union ,current_label ,num_of_item);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
#endif
}

//更新组选择，动态更新组的个数
static void wizardGroup_group_select_update(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int num_of_group = get_group_qty(pp->p_config);
		int num_of_item = num_of_group + 1;//+ ADD

		const char* current_label[num_of_item];
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_item);

        if(pMenuData->lastPopPos > num_of_item)//如果其他地方组的个数有变动，有可能造成上次选中的位置大于组个数
		{
			assert(num_of_item > 0);
			pMenuData->lastPopPos = 0;
		}
		if(num_of_group >= 8)
		{
			pMenuData->itemStatus = 1 << 8;
		}
		else
		{
			pMenuData->itemStatus = 0;
		}
		menuPopdownWithMalloc(menu_union ,current_label ,num_of_item);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardGroup_group_delete_press(MENU_UNION menu_union)
{
    int grpQty = get_group_qty(pp->p_config);
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
    MENU_UNION menu_GroupSelect = {{0, 0, 2, WIZARD_GROUP_STEP_GROUP_SELECT}};
	popDataStruct* pMenuGroupSelectData = (popDataStruct*)getMenuDataAddress(menu_GroupSelect);
    int grp = popGetLastPos(menu_GroupSelect);
    char notice[128];
    memset(notice, 0, 128);
    sprintf(notice, "%s %s?", getDictString(_STRING_NOTICE_DELETE_GROUP),
            getDictString(mainStringDict[pMenuGroupSelectData->stringIndex1].index[grp]));
	if(pMenuData)
	{
        if (grp < grpQty) {
		    menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);
            if (dialogWarning(pp->window, notice) == 1) {
    		    menu4FuncTrans(menu_union, &grp);
            }
        }
    }
}

//源选择
void wizardGroup_source_press(MENU_UNION menu_union)
{
    int num_of_group;
	int num_of_item;
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		num_of_group = get_group_qty(pp->p_config);
		num_of_item = num_of_group;
        if (menu_union.pos.menu2 == 0) {
            num_of_item++;
        }

		const char* current_label[num_of_item];
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_group);
		getMainDictStrings(pMenuData->stringIndex2 ,current_label + num_of_group,
                num_of_item - num_of_group);

		menuPopupWithMalloc(menu_union ,current_label ,num_of_item);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardGroup_source_update(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int num_of_group = get_group_qty(pp->p_config);
		int num_of_item = num_of_group + 1;

		const char* current_label[num_of_item];
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_group);
		getMainDictStrings(pMenuData->stringIndex2 ,current_label + num_of_group,
                num_of_item - num_of_group);

		if(pMenuData->lastPopPos >= num_of_item)//如果其他地方组的个数有变动，有可能造成上次选中的位置大于组个数
		{
			pMenuData->lastPopPos = 0;
		}
		menuPopdownWithMalloc(menu_union ,current_label ,num_of_group);
		getMainDictStrings(pMenuData->stringIndex2 ,current_label + num_of_group,
                num_of_item - num_of_group);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardGroup_geometry_press(x)  menuPopupWithMap((x) ,-1)
#define wizardGroup_geometry_update(x) menuPopdownWithMap((x) ,g_tmp_group_struct.part.Geometry)


static void wizardGroup_thickness_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 0.1;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

//根据形状，直径更新厚度
static void wizardGroup_thickness_update(MENU_UNION menu_union)
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
		int geometry = g_tmp_group_struct.part.Geometry;
		float diameter =  g_tmp_group_struct.part.Diameter / 1000.0 ; //get_part_diameter(pp->p_config) / 1000.0;
		switch(geometry)
		{
		case 0://PLATE_PART
		case 2://ID_PART
			pMenuData->upper = 1000.0;
			break;

		case 1://OD_PART
			pMenuData->upper = diameter / 2;
			break;

		default:
			pMenuData->upper = (diameter - 10) / 2;
			break;
		}
		if(pMenuData->upper < 0.05)
			pMenuData->upper = 0.05;
		pMenuData->current_value = g_tmp_group_struct.part.Thickness / 1000.0 ;
		if(pMenuData->current_value > pMenuData->upper)
		{
			pMenuData->current_value = pMenuData->upper;
		}
		else if(pMenuData->current_value < pMenuData->lower)
		{
			pMenuData->current_value = pMenuData->lower;
		}
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardGroup_diameter_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 0.1;
			else if(pMenuData->step == 0.1)		pMenuData->step = 0.01;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

//根据形状，厚度等更新直径
static void wizardGroup_diameter_update(MENU_UNION menu_union)
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

		int geometry = g_tmp_group_struct.part.Geometry;
		float thickness =  g_tmp_group_struct.part.Thickness / 1000.0 ;
		switch(geometry)
		{
		case 0:
			gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
			break;

		case 1:
			//更新，以应对诸如单位切换之类的更新
			pMenuData->lower = 2 * thickness;
			break;

		case 2:
			//更新，以应对诸如单位切换之类的更新
			pMenuData->lower = 20;
			break;

		default:
			break;
		}
		pMenuData->current_value = g_tmp_group_struct.part.Diameter / 1000.0 ; //get_part_diameter(pp->p_config) / 1000.0;//?
		if(pMenuData->current_value > pMenuData->upper)
		{
			pMenuData->current_value = pMenuData->upper;
		}
		else if(pMenuData->current_value < pMenuData->lower)
		{
			pMenuData->current_value = pMenuData->lower;
		}
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardGroup_material_press(x)  menuPopupWithMap((x) ,0)
#define wizardGroup_material_update(x) menuPopdownWithMap((x) ,g_tmp_group_struct.part.Material_pos)


#if HIGH_POWER
static void wizardGroup_group_mode_press(MENU_UNION menu_union)
{

}
#else
#define wizardGroup_group_mode_press(x)  menuPopupWithMap((x) ,-1)
#endif
#define wizardGroup_group_mode_update(x) menuPopdownWithMap((x) ,g_tmp_group_struct.group_mode)

static void wizardGroup_tx_rx_mode_press(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);

	if(pMenuData)
	{
		switch(g_tmp_group_struct.group_mode)//group mode
		{
		case PA://
			pMenuData->itemStatus = (1 << 3);
			break;

		default:
		{
#if HIGH_POWER
			pMenuData->itemStatus = 0xc;
#else
			unsigned int status   = gTofdEnable ? 0 : 0x8 ;
			pMenuData->itemStatus = status;
#endif
		}
			break;
		}
		menuPopupWithMap(menu_union ,-1);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 , LABELBUTTON_MODE_DATA3 , LABELBUTTON_STATUS_PRESS);
	}
	if(!gPithCatchEnable && g_tmp_group_struct.group_mode<= PA_SCAN)
	{
		gtk_widget_set_sensitive (pp->eventbox30[1], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[1], FALSE);
	}
}

//#define wizardGroup_tx_rx_mode_update(x) menuPopdownWithMap((x) ,get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE))

static void wizardGroup_tx_rx_mode_update(MENU_UNION menu_union)
{
	menuPopdownWithMap((menu_union) ,get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE)) ;
	if(!gPithCatchEnable && g_tmp_group_struct.group_mode<= PA_SCAN)
	{
		gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
	}
}

static void wizardGroup_pulse_press(MENU_UNION menu_union)
{
	switch(g_tmp_group_struct.group_mode)//group mode
	{
	case PA://PA  Connection P
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_TX_RX_MODE_PA;
		break;

	default:
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_TX_RX_MODE;
		break;
	}
	gCurrentMenuPos.key = menu_union.key;

	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
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

static void wizardGroup_group_delete_update(MENU_UNION menu_union)
{
    int grpQty = get_group_qty(pp->p_config);
    MENU_UNION menu_group_select = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_GROUP_SELECT}};
    menuSetDataWithMap(menu_union, LABELBUTTON_STATUS_RELEASE);
    if (grpQty == 1 || popGetLastPos(menu_group_select) == grpQty) {
        gtk_widget_set_sensitive(pp->eventbox30[3], FALSE);
        gtk_widget_set_sensitive(pp->eventbox31[3], FALSE);
    } else {
    }
}

static void wizardGroup_pulse_update(MENU_UNION menu_union)
{
	if((g_tmp_group_struct.group_mode == UT1_SCAN )|| (g_tmp_group_struct.group_mode == UT2_SCAN))
	{
		gtk_widget_hide(pp->eventbox30[menu_union.pos.menu3]);
		gtk_widget_hide(pp->eventbox31[menu_union.pos.menu3]);
		return ;
	}

	switch(g_tmp_group_struct.group_mode)//group mode
	{
	case PA://PA  Connection P
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_TX_RX_MODE_PA;
		break;

	default:
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_TX_RX_MODE;
		break;
	}

	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->upper = (float) (gMaxElementRecieveQty + 1 - g_tmp_group_struct.law_info.Last_tx_elem);
		pMenuData->current_value = (float)get_group_val (&g_tmp_group_struct, GROUP_PULSER);//更新需要读取数据
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardGroup_receive_press(x) wizardGroup_pulse_press(x)

static void wizardGroup_receive_update(MENU_UNION menu_union)
{

	if((g_tmp_group_struct.group_mode == UT1_SCAN )|| (g_tmp_group_struct.group_mode == UT2_SCAN))
	{
		gtk_widget_hide(pp->eventbox30[menu_union.pos.menu3]);
		gtk_widget_hide(pp->eventbox31[menu_union.pos.menu3]);
		return ;
	}
	switch(g_tmp_group_struct.group_mode)//group mode
	{
	case PA://PA  Connection P
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_TX_RX_MODE_PA;
		break;

	default:
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_TX_RX_MODE;
		break;
	}

	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->upper = (float) (gMaxElementRecieveQty + 1 - g_tmp_group_struct.law_info.Last_tx_elem);
		pMenuData->current_value = (float)get_group_val (&g_tmp_group_struct, GROUP_RECEIVER);//更新需要读取数据
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}

	if(g_tmp_group_struct.group_mode < UT1_SCAN)//Connection r
	{
		if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)	// 脉冲回波模式不可以调节
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
		}
	}
}

int wizardGroup_getProbeMode()
{
//	MENU_UNION menu_tx_rx_mode = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_TX_RX_MODE}};
	int txrxMode;

	int probe_mode = 0;

//	MENU_UNION menu_group_mode = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_GROUP_MODE}};
//	int groupMode;
//	switch((groupMode = popGetLastPos(menu_group_mode)))//group mode
	switch(g_tmp_group_struct.group_mode)//group mode
	{
	case PA://PA  Connection P
		probe_mode = 1;
		break;

	default:
//		switch((txrxMode = popGetLastPos(menu_tx_rx_mode)))//
		switch((txrxMode = get_group_val(&g_tmp_group_struct, GROUP_TX_RX_MODE)))//
		{
		case PE://PE
			probe_mode = 0;
			break;

		default:
			probe_mode = 1;
			break;
		}
		break;
	}
	return probe_mode;
}

static void wizardGroup_select_probe_press(MENU_UNION menu_union)
{
	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE;
	}
	else
	{//probe_mode == 0 ,PE
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE_PE;
	}
	gCurrentMenuPos.key = menu_union.key;
	menuPopupWithMap(menu_union ,-1);
}

static void wizardGroup_select_probe_update(MENU_UNION menu_union)
{
	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE;
	}
	else
	{//probe_mode == 0 ,PE
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE_PE;
	}
	menuPopdownWithMap(menu_union ,-1);
//	menuPopdownWithMap(menu_union ,get_probe_select(pp->p_config));
}

static void wizardGroup_define_probe_press(MENU_UNION menu_union)
{
	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE;
	}else{//probe_mode == 0 ,PE
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE_PE;
	}
	gCurrentMenuPos.key = menu_union.key;
	menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

	if(menu4FuncTrans(menu_union ,NULL))
	{
		menu3Update();
	}
}

static void wizardGroup_define_probe_update(MENU_UNION menu_union)
{
	int probeSelect;

	LABELBUTTON_MODE_ENUM menu3Mode = LABELBUTTON_MODE_BIGMENU;
	const char* label3;
	const char* data3;

	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		MENU_UNION menu_probe = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_PROBE}};//获取当前菜单的索引
		switch((probeSelect = popGetLastPos(menu_probe)))//
		{
		case 0://select tx
			menu3Mode = LABELBUTTON_MODE_DATA3;
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.probe.Model
			|| (0 == *(int*)g_tmp_group_struct.probe.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.probe.Model)))
			{
				data3 = "";
			}
			else
			{
				data3 = g_tmp_group_struct.probe.Model;
			}

			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;

		case 1://select rx
			menu3Mode = LABELBUTTON_MODE_DISABLE;
			//gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
			//gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
			break;

		case 2://define
			menu3Mode = LABELBUTTON_MODE_DATA3;
			menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE_DEFINE;//只用于get label3
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.probe.Model
			|| (0 == *(int*)g_tmp_group_struct.probe.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.probe.Model)))
			{
				data3 = "";//unknown
			}
			else
			{
				data3 = g_tmp_group_struct.probe.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;
		default:
			break;
		}
	}
	else
	{//probe_mode == 0 ,PE
		MENU_UNION menu_probe_pe = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_PROBE_PE}};//选择定义菜单的索引
		switch((probeSelect = popGetLastPos(menu_probe_pe)))//
//		switch((probeSelect = get_probe_select(pp->p_config)))//
		{
		case 0://select tx/rx
			menu3Mode = LABELBUTTON_MODE_DATA3;
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.probe.Model
			|| (0 == *(int*)g_tmp_group_struct.probe.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.probe.Model)))
			{
				data3 = "";//unknown
			}
			else
			{
				data3 = g_tmp_group_struct.probe.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;

		case 1://define
			menu3Mode = LABELBUTTON_MODE_DATA3;
			menu_union.pos.menu4 = WIZARD_GROUP_STEP_PROBE_DEFINE;//只用于get label3
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.probe.Model
			|| (0 == *(int*)g_tmp_group_struct.probe.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.probe.Model)))
			{
				data3 = "";//unknown
			}
			else
			{
				data3 = g_tmp_group_struct.probe.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;

		default:
			break;
		}
	}
	if(get_auto_detect (pp->p_config))
	{
		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
	}
}

static void wizardGroup_probe_auto_detect_press(MENU_UNION menu_union)
{
	menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

	if(menu4FuncTrans(menu_union ,NULL))
	{
		menu3Update();
	}
}

static void wizardGroup_probe_auto_detect_update(MENU_UNION menu_union)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);

	if(pMenuData)//用于找数据,函数
	{
		LABELBUTTON_MODE_ENUM menu3Mode = LABELBUTTON_MODE_DATA3;
		const char* label3 = getMainLabelString(menu_union);
		int switch_state = get_auto_detect(pp->p_config);
		assert(pMenuData->data < getMainDictSize(MAINSTRINGDICT_SWITCH));
		const char* data3 = getMainDictString(MAINSTRINGDICT_SWITCH ,switch_state);
		menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
	}
	else
	{
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardGroup_select_wedge_press(MENU_UNION menu_union)
{
	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE;
	}else{//probe_mode == 0 ,PE
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE_PE;
	}
	gCurrentMenuPos.key = menu_union.key;
	menuPopupWithMap(menu_union ,-1);
}

static void wizardGroup_select_wedge_update(MENU_UNION menu_union)
{
	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE;
	}else{//probe_mode == 0 ,PE
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE_PE;
	}
	menuPopdownWithMap(menu_union ,-1);
//	menuPopdownWithMap(menu_union ,get_probe_select(pp->p_config));
}

static void wizardGroup_define_wedge_press(MENU_UNION menu_union)
{
	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE;
	}else{//probe_mode == 0 ,PE
		menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE_PE;
	}
	gCurrentMenuPos.key = menu_union.key;
	menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

	if(menu4FuncTrans(menu_union ,NULL))
	{
		menu3Update();
	}
}


static void wizardGroup_define_wedge_update(MENU_UNION menu_union)
{
	int wedgeSelect;

	LABELBUTTON_MODE_ENUM menu3Mode = LABELBUTTON_MODE_BIGMENU;
	const char* label3;
	const char* data3;

	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		MENU_UNION menu_wedge = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_WEDGE}};//获取当前菜单的索引
		switch((wedgeSelect = popGetLastPos(menu_wedge)))//
//		switch((wedgeSelect = get_probe_select(pp->p_config)))//
		{
		case 0://select tx
			menu3Mode = LABELBUTTON_MODE_DATA3;
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.wedge.Model
			|| (0 == *(int*)g_tmp_group_struct.wedge.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.wedge.Model)))
			{
				data3 = "";//getDictString(_STRING_Unknown);
			}
			else
			{
				data3 = g_tmp_group_struct.wedge.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;

		case 1://select rx
			menu3Mode = LABELBUTTON_MODE_DISABLE;
			//gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
			//gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
			break;

		case 2://define
			menu3Mode = LABELBUTTON_MODE_DATA3;
			menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE_DEFINE;//只用于get label3
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.probe.Model
			|| (0 == *(int*)g_tmp_group_struct.probe.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.probe.Model)))
			{
				data3 = "";//unknown
			}
			else
			{
				data3 = g_tmp_group_struct.wedge.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;
		default:
			break;
		}
	}
	else
	{//probe_mode == 0 ,PE
		MENU_UNION menu_wedge_pe = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_WEDGE_PE}};//获取当前菜单的索引
		switch((wedgeSelect = popGetLastPos(menu_wedge_pe)))//
//		switch((wedgeSelect = get_probe_select(pp->p_config)))//
		{
		case 0://select tx/rx
			menu3Mode = LABELBUTTON_MODE_DATA3;
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.wedge.Model
			|| (0 == *(int*)g_tmp_group_struct.wedge.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.wedge.Model)))
			{
				data3 = "";//unknown
			}
			else
			{
				data3 = g_tmp_group_struct.wedge.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;

		case 1://define
			menu3Mode = LABELBUTTON_MODE_DATA3;
			menu_union.pos.menu4 = WIZARD_GROUP_STEP_WEDGE_DEFINE;//只用于get label3
			label3 = getMainLabelString(menu_union);
			if(NULL == g_tmp_group_struct.wedge.Model
			|| (0 == *(int*)g_tmp_group_struct.wedge.Model)
			|| ( 0 == strcmp(" " ,g_tmp_group_struct.wedge.Model)))
			{
				data3 = "";
			}
			else
			{
				data3 = g_tmp_group_struct.wedge.Model;
			}
			menuSetLabel(menu_union.pos.menu3 ,menu3Mode ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
			break;

		default:
			break;
		}
	}
}

static void wizardGroup_scan_offset_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 5.0;
			else if(pMenuData->step == 5.0)		pMenuData->step = 0.1;
		//	else if(pMenuData->step == 0.1)		pMenuData->step = 0.01;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardGroup_scan_offset_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->current_value = g_tmp_group_struct.scan_offset * 0.1;
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

#define wizardGroup_index_offset_press(x) wizardGroup_scan_offset_press(x)

static void wizardGroup_index_offset_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->current_value = g_tmp_group_struct.index_offset * 0.1;
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardGroup_skew_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
			else if(pMenuData->step == 10.0)	pMenuData->step = 100.0;
		//	else if(pMenuData->step == 0.1)		pMenuData->step = 0.01;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void wizardGroup_skew_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->current_value = g_tmp_group_struct.skew * 0.01;//更新需要读取数据
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void wizardGroup_continue_press(MENU_UNION menu_union)
{
	menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

	if(menu4FuncTrans(menu_union ,NULL))
	{
		menu3Update();
	}
}

static void wizardGroup_continue_update(MENU_UNION menu_union)
{
	menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
			,getMainLabelString(menu_union) ,getDictString(_STRING_Law_Wizard));
	if(PA != g_tmp_group_struct.group_mode)
	{
		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,"" ,"");
	}
}

void menu000(MENU_UNION menu_union)//0 ,0 ,0 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		wizardGroup_back_press(menu_union);
	}
	else
	{
		wizardGroup_back_update(menu_union);
	}

}

void menu001(MENU_UNION menu_union)//0 ,0 ,1 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		wizardGroup_next_press(menu_union);
	}
	else
	{
		wizardGroup_next_update(menu_union);
	}
}

void menu002(MENU_UNION menu_union)//0 ,0 ,2 ,0
{
	MENU_UNION menuStep = {{0 ,0 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			case WIZARD_GROUP_STEP_GROUP_SELECT:	wizardGroup_group_select_press(menu_union);		break;
			case WIZARD_GROUP_STEP_ADD:				wizardGroup_source_press(menu_union);			break;
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_geometry_press(menu_union);			break;
			case WIZARD_GROUP_STEP_GROUP_MODE:		wizardGroup_group_mode_press(menu_union);	    break;
			case WIZARD_GROUP_STEP_TX_RX_MODE:		wizardGroup_tx_rx_mode_press(menu_union);		break;
			case WIZARD_GROUP_STEP_PROBE_PE:		wizardGroup_select_probe_press(menu_union);		break;
			case WIZARD_GROUP_STEP_WEDGE_PE:		wizardGroup_select_wedge_press(menu_union);		break;
			case WIZARD_GROUP_STEP_POSITION:		wizardGroup_scan_offset_press(menu_union);		break;
			case WIZARD_GROUP_STEP_FINISH:			wizardGroup_continue_press(menu_union);			break;
			default:
				break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			case WIZARD_GROUP_STEP_GROUP_SELECT:	wizardGroup_group_select_update(menu_union);	break;
			case WIZARD_GROUP_STEP_ADD:				wizardGroup_source_update(menu_union);			break;
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_geometry_update(menu_union);		break;
			case WIZARD_GROUP_STEP_GROUP_MODE:		wizardGroup_group_mode_update(menu_union);		break;
			case WIZARD_GROUP_STEP_TX_RX_MODE:		wizardGroup_tx_rx_mode_update(menu_union);		break;
			case WIZARD_GROUP_STEP_PROBE_PE:		wizardGroup_select_probe_update(menu_union);	break;
			case WIZARD_GROUP_STEP_WEDGE_PE:		wizardGroup_select_wedge_update(menu_union);	break;
			case WIZARD_GROUP_STEP_POSITION:		wizardGroup_scan_offset_update(menu_union);		break;
			case WIZARD_GROUP_STEP_FINISH:			wizardGroup_continue_update(menu_union);		break;
			default://不显示
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
	}
}


void menu003(MENU_UNION menu_union)//0 ,0 ,3 ,0
{
	MENU_UNION menuStep = {{0 ,0 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:
			//case WIZARD_GROUP_STEP_ADD:
			//case WIZARD_GROUP_STEP_GROUP_MODE:
			case WIZARD_GROUP_STEP_GROUP_SELECT:    wizardGroup_group_delete_press(menu_union);   break;
			case WIZARD_GROUP_STEP_TX_RX_MODE:		wizardGroup_pulse_press(menu_union);		break;
			case WIZARD_GROUP_STEP_PROBE_PE:		wizardGroup_define_probe_press(menu_union);	break;
			case WIZARD_GROUP_STEP_WEDGE_PE:		wizardGroup_define_wedge_press(menu_union);	break;
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_thickness_press(menu_union);	break;
			case WIZARD_GROUP_STEP_POSITION:		wizardGroup_index_offset_press(menu_union);	break;
			//case WIZARD_GROUP_STEP_FINISH:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:
			//case WIZARD_GROUP_STEP_ADD:
			//case WIZARD_GROUP_STEP_GROUP_MODE:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:    menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);   break;
			case WIZARD_GROUP_STEP_GROUP_SELECT:    wizardGroup_group_delete_update(menu_union);    break;
			case WIZARD_GROUP_STEP_TX_RX_MODE:		wizardGroup_pulse_update(menu_union);			break;
			case WIZARD_GROUP_STEP_PROBE_PE:		wizardGroup_define_probe_update(menu_union);	break;
			case WIZARD_GROUP_STEP_WEDGE_PE:		wizardGroup_define_wedge_update(menu_union);	break;
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_thickness_update(menu_union);		break;
			case WIZARD_GROUP_STEP_POSITION:		wizardGroup_index_offset_update(menu_union);	break;
			//case WIZARD_GROUP_STEP_FINISH:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
	}
}


void menu004(MENU_UNION menu_union)//0 ,0 ,4 ,0
{
	MENU_UNION menuStep = {{0 ,0 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:
			//case WIZARD_GROUP_STEP_ADD:
			//case WIZARD_GROUP_STEP_GROUP_MODE:
			case WIZARD_GROUP_STEP_PROBE_PE:		wizardGroup_probe_auto_detect_press(menu_union);break;
			//case WIZARD_GROUP_STEP_WEDGE_PE:
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_diameter_press(menu_union);			break;
			case WIZARD_GROUP_STEP_POSITION:		wizardGroup_skew_press(menu_union);				break;
			case WIZARD_GROUP_STEP_TX_RX_MODE:		wizardGroup_receive_press(menu_union);			break;
			//case WIZARD_GROUP_STEP_FINISH:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:
			//case WIZARD_GROUP_STEP_ADD:
			//case WIZARD_GROUP_STEP_GROUP_MODE:
			case WIZARD_GROUP_STEP_PROBE_PE:		wizardGroup_probe_auto_detect_update(menu_union);	break;
			//case WIZARD_GROUP_STEP_WEDGE_PE:
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_diameter_update(menu_union);			break;
			case WIZARD_GROUP_STEP_POSITION:		wizardGroup_skew_update(menu_union);				break;
			case WIZARD_GROUP_STEP_TX_RX_MODE:		wizardGroup_receive_update(menu_union);				break;
			//case WIZARD_GROUP_STEP_FINISH:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
	}
}


void menu005(MENU_UNION menu_union)//0 ,0 ,5 ,0
{
	MENU_UNION menuStep = {{0 ,0 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menuStep);

	if(pMenuData)//用于找数据,函数
	{
		menu_union.pos.menu4 = pMenuData->data;//用于找label

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;//主要用于spin,pop
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:
			//case WIZARD_GROUP_STEP_ADD:
			//case WIZARD_GROUP_STEP_GROUP_MODE:
			//case WIZARD_GROUP_STEP_TX_RX_MODE:
			//case WIZARD_GROUP_STEP_PROBE:
			//case WIZARD_GROUP_STEP_WEDGE:
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_material_press(menu_union);		break;
			//case WIZARD_GROUP_STEP_POSITION:
			//case WIZARD_GROUP_STEP_FINISH:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
		else
		{
			switch(pMenuData->data)
			{
			//case WIZARD_GROUP_STEP_START:
			//case WIZARD_GROUP_STEP_GROUP_SELECT:
			//case WIZARD_GROUP_STEP_ADD:
			//case WIZARD_GROUP_STEP_GROUP_MODE:
			//case WIZARD_GROUP_STEP_TX_RX_MODE:
			//case WIZARD_GROUP_STEP_PROBE:
			//case WIZARD_GROUP_STEP_WEDGE:
			case WIZARD_GROUP_STEP_MATERIAL:		wizardGroup_material_update(menu_union);		break;
			//case WIZARD_GROUP_STEP_POSITION:
			//case WIZARD_GROUP_STEP_FINISH:
			default:
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
				break;
			}
		}
	}
}
