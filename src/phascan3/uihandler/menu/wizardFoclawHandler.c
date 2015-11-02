/*
 * wizardFoclawHandler.c
 *
 *  Created on: 2012-2-21
 *      Author: lzk
 */
#include "menuHandler.h"
#include "../../string/_string.h"
#include "../../lzk/common.h"
#include "../../map/menuMap.h"
#include "../../ui/menu/menuFunc.h"
#include "../../ui/ui.h"
#include "../../drawui.h"
#include "../../main.h"
#include "../../callback.h"
#include "../../draw_dialog.h"
#include "../../calculation/sizingcurve.h"
#include "../../ui/authorization.h"
#include <assert.h>
extern void showHelp(MENU_UNION menu_union ,int enable);
extern void UTgroupConfigSetting(unsigned char grp);
extern int CheckGroupConfigureAvailable();


void wizardFocallawHandler_setStep(int step)
{
	MENU_UNION menu_step = {{0 ,1 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_step);
	if(pMenuData)
	{
		pMenuData->data = step;
		gCurrentMenuPos.pos.menu4 = step;
	}
}


int wizardFocallawHandler_back(void* pPara)//back
{
	if(pPara)
	{
		menuDataStruct* pMenuData = pPara;
		switch(pMenuData->data)
		{
		case WIZARD_FOCLAW_STEP_GROUP_SELECT:
			pp->help_yn = FALSE;
			showHelp(gCurrentMenuPos ,pp->help_yn);
			break;
		case WIZARD_FOCLAW_STEP_GENERATE:
		case WIZARD_FOCLAW_STEP_START:
		case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
		case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
		case WIZARD_FOCLAW_STEP_WAVE_TYPE:
		case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
		case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
		case WIZARD_FOCLAW_STEP_MAX:
		default:
			break;
		}
	}
	return TRUE;
}

int wizardFocallawHandler_group_select(void* pPara)
{
	MENU_UNION menu_group_select = {{0 ,1 ,2 ,WIZARD_FOCLAW_STEP_GROUP_SELECT}};
	//选组时把数据导到临时变量g_tmp_group_struct里
	memcpy( &g_tmp_group_struct
			,&(pp->p_config->group[popGetLastPos(menu_group_select)])
			,sizeof(struct _Group));
//	pp->wgroup_pos = popGetLastPos(menu_group_select);//选中的组
	return TRUE;
}

int wizardFocallawHandler_generate(void* pPara)
{
	MENU_UNION menu_group_select = {{0 ,1 ,2 ,WIZARD_FOCLAW_STEP_GROUP_SELECT}};
	//GtkWindow* win = GTK_WINDOW(pp->window) ;
	//GtkWidget* dialog ;
	int response      ;
	char* dialog_label;

	dialog_label = g_strdup_printf("\n%s\n      %s ?  \n"
			,getDictString(_STRING_Finish_Focallaw_Setting)
			,getMainDictString(MAINSTRINGDICT_GROUP ,popGetLastPos(menu_group_select)));

	response = dialogWarning(pp->window , dialog_label) ;
	if(!response)
	{
		return FALSE ;
	}
	free(dialog_label);


	GROUP _tmpGroup ;
	memcpy(&_tmpGroup  ,  &(pp->p_config->group[popGetLastPos(menu_group_select)]) ,sizeof(struct _Group));//保存临时数据
	//生成时把数据从临时变量g_tmp_group_struct里导回
	memcpy(&(pp->p_config->group[popGetLastPos(menu_group_select)])
			,&g_tmp_group_struct,sizeof(struct _Group));//保存临时数据
//		pMenuData->data = WIZARD_FOCLAW_STEP_START;

	if(CheckGroupConfigureAvailable())
	{
		memcpy(&(pp->p_config->group[popGetLastPos(menu_group_select)])
					,&_tmpGroup ,sizeof(struct _Group));//保存临时数据
		//set_group_qty (pp->p_config, _nGroupQty);
		dialogError(pp->window, getDictString(_STRING_FOCALLAW_NOT_AVAILABEL));
		return FALSE;
	}

	pp->p_config->groupId = popGetLastPos(menu_group_select);
	if(PA == g_tmp_group_struct.group_mode)
	{   // if not pa , focallaw menu disable
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	}
	else
	{
	    UTgroupConfigSetting(pp->wgroup_pos);
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	}
	// setting filter
	set_filter(pp->p_config->groupId);
	GROUP_VAL_POS(pp->p_config->groupId , WedgeDelayCalibrated) = 0 ;
	GROUP_VAL_POS(pp->p_config->groupId , SensationCalibrated) = 0 ;
	generate_focallaw (pp->p_config->groupId);
	RefreshBeamDelay(pp->p_config->groupId);
	RefreshGainOffset(pp->p_config->groupId);
	InitSizingCurveParameters(pp->p_config->groupId) ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	gtk_widget_queue_draw (pp->status_area);
	draw_area_all();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	return TRUE;
}

//下一步
int wizardFocallawHandler_next(void* pPara)
{
	int ret = TRUE;
	if(pPara)
	{
		menuDataStruct* pMenuData = pPara;
		switch(pMenuData->data)
		{
		case WIZARD_FOCLAW_STEP_GROUP_SELECT:
			ret = wizardFocallawHandler_group_select(pPara);
			break;

		case WIZARD_FOCLAW_STEP_GENERATE:
			ret = wizardFocallawHandler_generate(pPara);
			if(ret)
			{
				pp->help_yn = FALSE;
				showHelp(gCurrentMenuPos ,pp->help_yn);
			}
			break;

		case WIZARD_FOCLAW_STEP_START:
			pp->help_yn = TRUE;
			showHelp(gCurrentMenuPos ,pp->help_yn);
			break;

		case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
		case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
		case WIZARD_FOCLAW_STEP_WAVE_TYPE:
		case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
		case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
		case WIZARD_FOCLAW_STEP_MAX:
		default:
			break;
		}
	}
	return ret;
}

int wizardFocallawHandler_law_type(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;

		g_tmp_group_struct.law_info.Focal_type = para;
		switch(para)
		{
		case LINEAR_SCAN:
			g_tmp_group_struct.law_info.Angle_max = 0;
			g_tmp_group_struct.law_info.Angle_min = 0;
			break;

		case DEPTH_SCAN:
		case STATIC_SCAN:
			g_tmp_group_struct.law_info.Focal_point_type = DEPTH_P;
			// no break
		default:
			g_tmp_group_struct.law_info.Last_tx_elem = g_tmp_group_struct.law_info.First_tx_elem
														+ g_tmp_group_struct.law_info.Elem_qty - 1;
			break;
		}
	}
	return TRUE;
}

int wizardFocallawHandler_focal_type(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;

		g_tmp_group_struct.law_info.Focal_point_type = para;
	}
	return TRUE;
}

int wizardFocallawHandler_position_start(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Position_start = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_position_end(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Position_end = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_position_step(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Position_step = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_offset_start(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Offset_start = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_offset_end(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Offset_end = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_depth_start(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Depth_start = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_depth_end(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.law_info.Depth_end = para * 1000 / multiplier;
	}
	return TRUE;
}

int wizardFocallawHandler_wave_type(void* p_para)
{
	int material = g_tmp_group_struct.part.Material_pos ;
	if(NULL != p_para)
	{
//		MENU_UNION menu_union = {{0 ,1 ,2 ,WIZARD_FOCLAW_STEP_WAVE_TYPE}};//获取当前菜单的索引
		int para = *(int*)p_para;
		switch(para)
		{
		case 0://lw
			g_tmp_group_struct.velocity = get_material_lw (material);
//			popSetLastPos(menu_union ,0);
			break;
		case 1:
			g_tmp_group_struct.velocity = get_material_sw (material);
//			popSetLastPos(menu_union ,1);
			break;
		default:
			break;
		}
	}
	return TRUE;
}

int wizardFocallawHandler_material(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		//set_part_material(pp->p_config, (void*)para);
		g_tmp_group_struct.part.Material_pos = para ;
		g_tmp_group_struct.velocity = get_material_lw (para);
	}
	return TRUE;
}

int wizardFocallawHandler_element_qty(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para + 0.5;

		g_tmp_group_struct.law_info.Elem_qty = para;
		if( AZIMUTHAL_SCAN == g_tmp_group_struct.law_info.Focal_type)
		{
			g_tmp_group_struct.law_info.Last_tx_elem =
					(guchar)(g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1;
		}else{//Linear
			if( g_tmp_group_struct.law_info.Last_tx_elem <
					((guchar) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1) )
			{
				g_tmp_group_struct.law_info.Last_tx_elem =
						(guchar) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1;
			}
		}
		int _nGroupSumGain  ;
		if(!g_tmp_group_struct.sum_gain_pos)
		{
		   if (g_tmp_group_struct.law_info.Elem_qty == 1)
			    _nGroupSumGain	= 4095;
		   else
			    _nGroupSumGain	= 6400 / g_tmp_group_struct.law_info.Elem_qty;
		   g_tmp_group_struct. sum_gain = _nGroupSumGain ;
		}//*/
	}

	return TRUE;
}

int wizardFocallawHandler_first_element(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;

		g_tmp_group_struct.law_info.First_tx_elem =  (guchar)para;
		if(g_tmp_group_struct.law_info.Focal_type == AZIMUTHAL_SCAN)//Azimuthal
			g_tmp_group_struct.law_info.Last_tx_elem =
					(guchar) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1;
		else//Linear
		{
			if( g_tmp_group_struct.law_info.Last_tx_elem
					< ((guchar) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1) )
			{
				g_tmp_group_struct.law_info.Last_tx_elem =
						(guchar) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1;
			}
		}//*/
	}

	return TRUE;
}


int wizardFocallawHandler_last_element(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;
		int temp_value ;

		if( g_tmp_group_struct.law_info.Last_tx_elem <
				((unsigned char) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1) )
		{
			temp_value = (unsigned char) (g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty) - 1;
		}else{
			temp_value =  (unsigned char)para;
		}

		g_tmp_group_struct.law_info.Last_tx_elem = g_tmp_group_struct.law_info.Elem_step * ((temp_value + 1 - g_tmp_group_struct.law_info.Elem_qty-
				g_tmp_group_struct.law_info.First_tx_elem)/g_tmp_group_struct.law_info.Elem_step)
				+ g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty - 1;
	}
	return TRUE;
}

int wizardFocallawHandler_element_step(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;

	    g_tmp_group_struct.law_info.Elem_step =  (unsigned char)para;
		int _nStep = (int)para ;
		int _nProbeQty = MIN(gMaxElementRecieveQty , g_tmp_group_struct.probe.Elem_qty) ;
		int _nLastElemnt = _nStep * ((g_tmp_group_struct.law_info.Last_tx_elem + 1 - g_tmp_group_struct.law_info.Elem_qty-
				g_tmp_group_struct.law_info.First_tx_elem)/_nStep + 1) + g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty - 1;

		if(_nLastElemnt >  _nProbeQty)
		{
			_nLastElemnt = _nStep * ((_nProbeQty + 1 - g_tmp_group_struct.law_info.Elem_qty-
					g_tmp_group_struct.law_info.First_tx_elem)/_nStep) + g_tmp_group_struct.law_info.First_tx_elem + g_tmp_group_struct.law_info.Elem_qty - 1;
		}
		g_tmp_group_struct.law_info.Last_tx_elem = _nLastElemnt  ;
	}

	return TRUE;
}

int wizardFocallawHandler_min_angle(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;

	    short temp_value ;
	    int beam_num = 0; //
	    temp_value = (gint)(para * 100.0) ;

		if(gtk_widget_get_sensitive(pp->eventbox30[1]))
		{
			if(temp_value >= 0)
			     temp_value = ((temp_value + 5) / 10) * 10 ;
			else temp_value = ((temp_value - 5) / 10) * 10 ;
			beam_num = (int)((g_tmp_group_struct.law_info.Angle_max - temp_value) / g_tmp_group_struct.law_info.Angle_step) ;
			temp_value = g_tmp_group_struct.law_info.Angle_max - beam_num * g_tmp_group_struct.law_info.Angle_step ;
			//temp_value = rounding(LAW_VAL(Angle_max), temp_value, (LAW_VAL(Angle_step) + 5) / 10 * 10) ;
		}
		if(beam_num >= setup_MAX_GROUP_LAW_QTY)     return TRUE;
		g_tmp_group_struct.law_info.Angle_min = temp_value;
	    if(temp_value > g_tmp_group_struct.law_info.Angle_max)
	    	g_tmp_group_struct.law_info.Angle_max = temp_value ;
	}
	return TRUE;
}

int wizardFocallawHandler_max_angle(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;
	    short temp_value ;
	    temp_value =  (short)(para * 100.0);
		int beam_num ;

	    if (g_tmp_group_struct.law_info.Focal_type == AZIMUTHAL_SCAN)
		{
			if(temp_value >= 0)
			     temp_value = ((temp_value + 5) / 10) * 10 ;
			else temp_value = ((temp_value - 5) / 10) * 10 ;
			beam_num = (int)(( temp_value - g_tmp_group_struct.law_info.Angle_min) / g_tmp_group_struct.law_info.Angle_step) ;
	    	temp_value = g_tmp_group_struct.law_info.Angle_min + beam_num * g_tmp_group_struct.law_info.Angle_step;

	    	if(beam_num >= setup_MAX_GROUP_LAW_QTY)  return TRUE;

	    	g_tmp_group_struct.law_info.Angle_max = temp_value ;
			if(temp_value < g_tmp_group_struct.law_info.Angle_min)
				g_tmp_group_struct.law_info.Angle_min  = temp_value  ;
		}
	}
	return TRUE;
}

int wizardFocallawHandler_angle_step(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;
		short temp_value ;
		int beam_no ;

		if (g_tmp_group_struct.law_info.Focal_type == AZIMUTHAL_SCAN)
		{
			temp_value = (gushort) (para * 100.0);
			if(temp_value >= 0)
			     temp_value = ((temp_value + 5) / 10) * 10 ;
			else
				temp_value = ((temp_value - 5) / 10) * 10 ;

			beam_no = (int)((g_tmp_group_struct.law_info.Angle_max - g_tmp_group_struct.law_info.Angle_min) / temp_value);

			if(beam_no >= setup_MAX_LAW_QTY)  return TRUE;

			g_tmp_group_struct.law_info.Angle_step = temp_value ;
			g_tmp_group_struct.law_info.Angle_max = g_tmp_group_struct.law_info.Angle_min + beam_no * temp_value ;
		}
	}
	return TRUE;
}

int wizardFocallawHandler_focus_depth(void* p_para)
{
	return TRUE;
}
