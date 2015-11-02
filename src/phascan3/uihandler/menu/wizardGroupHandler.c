/*
 * wizardGroupHandler.c
 *
 *  Created on: 2012-5-23
 *      Author: lzk
 */
#include "menuHandler.h"
#include "../../lzk/common.h"
#include "../../lzk/output_spi.h"
#include "../../map/menuMap.h"
#include "../../ui/menu/menuFunc.h"

#include <gtk/gtk.h>
#include "../../drawui.h"
#include "../../draw_dialog.h"
#include "../../callback.h"
#include "../../main.h"
#include "../../base_const.h"
#include <assert.h>

#include "../../ui/ui.h"
#include "../../string/_string.h"
#include "../../globalData.h"
#include "../../calculation/sizingcurve.h"
void set_filter (int group) ;
void generate_focallaw(int grp);
void MultiGroupSendGroupSpi();
void MultiGroupSendFocalSpi();
int RefreshScanInfor();
extern void UTgroupConfigSetting(unsigned char grp);

void b2_fun0(DRAW_UI_P p, guint pos);
int wizardGroup_getProbeMode();
void wizardFocallawHandler_setStep(int step);

void wizardGroupHandler_setStep(int step)
{
	MENU_UNION menu_step = {{0 ,0 ,0 ,0}};
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_step);
	if(pMenuData)
	{
		pMenuData->data = step;
		gCurrentMenuPos.pos.menu4 = step;
	}
}

int wizardGroupHandler_back(void* pPara)//back
{
	if(pPara)
	{
		menuDataStruct* pMenuData = pPara;
		switch(pMenuData->data)
		{
		case WIZARD_GROUP_STEP_GROUP_SELECT:
			pp->help_yn = FALSE;
			showHelp(gCurrentMenuPos ,pp->help_yn);
			break;

		case WIZARD_GROUP_STEP_START:
		case WIZARD_GROUP_STEP_ADD:
		case WIZARD_GROUP_STEP_FINISH:
		case WIZARD_GROUP_STEP_MATERIAL:
		case WIZARD_GROUP_STEP_GROUP_MODE:
		case WIZARD_GROUP_STEP_TX_RX_MODE:
		case WIZARD_GROUP_STEP_PROBE_PE:
		case WIZARD_GROUP_STEP_WEDGE_PE:
		case WIZARD_GROUP_STEP_POSITION:
		case WIZARD_GROUP_STEP_MAX:
		default:
			break;
		}
	}
	return TRUE;
}

int wizardGroupHandler_group_select(void* pPara)
{
	MENU_UNION menu_group_select = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_GROUP_SELECT}};
	//选组时如果修改数据则把数据导到临时变量g_tmp_group_struct里
	//增加组时增加完就可以了
	if(get_group_qty(pp->p_config) != popGetLastPos(menu_group_select))//不是add,跳过第2步
	{//modify
		memcpy( &g_tmp_group_struct//获得临时数据
				,&(pp->p_config->group[popGetLastPos(menu_group_select)])
				,sizeof(struct _Group));
		memcpy(&(gData->tmpGroupData) ,&(gGroupData[popGetLastPos(menu_group_select)]) ,sizeof(struct groupStruct));
	}
	pp->wgroup_pos = popGetLastPos(menu_group_select);//选中的组
	return TRUE;
}

extern int IsDisplayModalAvailable(unsigned char nDisplayType_);
extern int CheckGroupConfigureAvailable();
int wizardGroupHandler_addGroup(void* pPara)
{
	int _nGroupQty, index, flage=0;
	_nGroupQty = get_group_qty(pp->p_config);
	MENU_UNION menu_group_source = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_ADD}};
	assert(_nGroupQty < 8);
	index = popGetLastPos(menu_group_source);
	assert(index <= _nGroupQty);
    if (index == _nGroupQty) {
        index = 0;
        flage = 1;
		ClearOneGroupParameters(_nGroupQty);
		SettingGroupConfigure (_nGroupQty);
    } else {
        memcpy(&(pp->p_config->group[_nGroupQty])//复制数据到新增组
			,&(pp->p_config->group[index])//选择数据源
			,sizeof(struct _Group));
    }

	memcpy(&(gGroupData[_nGroupQty]) ,&(gGroupData[index]) ,sizeof(struct groupStruct));
	//工作组数量更新
	set_group_qty (pp->p_config, _nGroupQty + 1);
	//新加组校验
	if(CheckGroupConfigureAvailable())
	{
		set_group_qty (pp->p_config, _nGroupQty);
		dialogError(pp->window, getDictString(_STRING_GROUP_ADD_NOT_AVAILABEL));
		return TRUE;
	}
	//新组附加设置
	GROUP_VAL_POS(_nGroupQty , WedgeDelayCalibrated) = 0 ;
	GROUP_VAL_POS(_nGroupQty , SensationCalibrated) = 0 ;
	//参数计算
	set_filter(_nGroupQty);
	generate_focallaw (_nGroupQty);
	RefreshBeamDelay(_nGroupQty);
	RefreshGainOffset(_nGroupQty);
	//硬件更新
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	//显示更新
	RefreshPrfMark() ;
	gtk_widget_queue_draw (pp->status_area);
	if(!IsDisplayModalAvailable(get_display_pos(pp->p_config)))
	{
		set_display_pos(pp->p_config, A_SCAN) ;
	}
	pp->p_config->groupId  = _nGroupQty    ;
	request_refresh(REFRESH_MULTI_GROUP_ACTION) ;

	memcpy(&g_tmp_group_struct, //获得临时数据
            &(pp->p_config->group[_nGroupQty]), sizeof(struct _Group));
    memcpy(&(gData->tmpGroupData) ,&(gGroupData[_nGroupQty]) ,sizeof(struct groupStruct));
    pp->wgroup_pos = _nGroupQty;//选中的组
	return TRUE;
}

int wizardGroupHandler_DeleteGroup(void* pPara)
{
	if(pPara) {
		int *grp = pPara;
	    int i, _nGroupQty;
    	_nGroupQty = get_group_qty(pp->p_config);

        if (_nGroupQty == 1) {
            return -1;
        }

        for (i=*grp; i<_nGroupQty-1; i++) {
	        memcpy (&pp->p_config->group[i], &pp->p_config->group[i+1] , sizeof(GROUP));

	        set_filter(i);
	        generate_focallaw(i);
	        RefreshBeamDelay(i);
        }

	    _nGroupQty -= 1 ;
	    set_group_qty (pp->p_config, _nGroupQty);
        pp->wgroup_pos = 0;
	    pp->p_config->groupId = 0;

	    MultiGroupSendGroupSpi();
	    MultiGroupSendFocalSpi();
	    RefreshScanInfor() ;

    	RefreshPrfMark() ;
	    gtk_widget_queue_draw (pp->status_area);
    	if(!IsDisplayModalAvailable(get_display_pos(pp->p_config)))
	    {
		    set_display_pos(pp->p_config, A_SCAN) ;
    	}
    	request_refresh(REFRESH_MULTI_GROUP_ACTION) ;
    }
}

extern void SetDefaultPCS(int grp);
int wizardGroupHandler_finish(void* pPara)
{
	//MENU_UNION menu_group_select = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_GROUP_SELECT}};
	//用户点击finish按钮后，把临时的group copy去
	memcpy(&(pp->p_config->group[pp->wgroup_pos]) ,&g_tmp_group_struct ,sizeof(struct _Group));//保存临时数据
	memcpy(&(gGroupData[pp->wgroup_pos]) ,&(gData->tmpGroupData) ,sizeof(struct groupStruct));
//add by lzk 121127
	pp->p_config->groupId = pp->wgroup_pos;
	GROUP *p_grp = get_group_by_id (pp->p_config, pp->p_config->groupId);
	set_group_val (p_grp, GROUP_PW_POS, 0);
	set_group_val (p_grp, GROUP_PW_VAL, get_pw() * PW_DIV);

	//*******************************************************************
	// 星号内代码为临时的，当前UT 双晶控制协议存在问题，
	int group = pp->p_config->groupId  ;
	int _nTmpModel = GROUP_VAL_POS(group , tx_rxmode1) ;
	if (_nTmpModel == PULSE_ECHO )
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,1 ,0);//双晶关
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,1 ,0);//双晶关
		}
	}
	else
	{
		//add by lzk 20130422
		if (_nTmpModel == TOFD)
		{
			GROUP *p_grp = get_group_by_id (pp->p_config, group);
			set_group_val (get_group_by_id(pp->p_config, group), GROUP_VIDEO_FILTER,  0);
			TMP(group_spi[group]).video_filter	= 0 ;
			set_group_val (p_grp, GROUP_RECTIFIER, 0);
			TMP(group_spi[group]).rectifier = 0 ;
		    DrawDisplayWindowFrame() ;
		}
		//end 20130422
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,0 ,0);//双晶关
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,0 ,0);//双晶关
		}
	}
	output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
	//*******************************************************************
	if(PA == g_tmp_group_struct.group_mode)
	{
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	}
	else
	{
	    UTgroupConfigSetting(pp->wgroup_pos);
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	}
	// setting filter ,
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
	// refresh display area
	//request_refresh(REFRESH_DRAW_AREA_ALL);
	draw_area_all();
	// reset encoder
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
	// clear framebuffer1
	//pp->bRefreshDraw = TRUE ;
	// refresh prf and scan velocity mark
	RefreshPrfMark() ;
	// reset calibration condition
	SetDefaultPCS(group);
	// redraw status area for calibration status
	//*******************************************************************
	// 星号内代码为临时的，当前UT 双晶控制协议存在问题，
	if (_nTmpModel == PULSE_ECHO )
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,0 ,0);//双晶关
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,0 ,0);//双晶关
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH ,0 ,0);
	}
	else
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,1 ,0);//双晶关
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,1 ,0);//双晶关
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH ,1 ,0);
	}
	output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
	//*******************************************************************
	return TRUE;
}

int wizardGroupHandler_next(void* pPara)
{
	int ret = TRUE;
	if(pPara)
	{
		menuDataStruct* pMenuData = pPara;
		switch(pMenuData->data)
		{
		case WIZARD_GROUP_STEP_GROUP_SELECT:
			ret = wizardGroupHandler_group_select(pPara);
			break;

		case WIZARD_GROUP_STEP_ADD:
			ret = wizardGroupHandler_addGroup(pPara);
			if(ret)
			{
				pp->help_yn = FALSE;
				showHelp(gCurrentMenuPos ,pp->help_yn);
			}
			break;

		case WIZARD_GROUP_STEP_FINISH:
			ret = wizardGroupHandler_finish(pPara);
			if(ret)
			{
				pp->help_yn = FALSE;
				showHelp(gCurrentMenuPos ,pp->help_yn);
			}
			break;

		case WIZARD_GROUP_STEP_START:
			pp->help_yn = TRUE;
			showHelp(gCurrentMenuPos ,pp->help_yn);
			break;

		case WIZARD_GROUP_STEP_MATERIAL:
		case WIZARD_GROUP_STEP_GROUP_MODE:
		case WIZARD_GROUP_STEP_TX_RX_MODE:
		case WIZARD_GROUP_STEP_PROBE_PE:
		case WIZARD_GROUP_STEP_WEDGE_PE:
		case WIZARD_GROUP_STEP_POSITION:
		case WIZARD_GROUP_STEP_MAX:
		default:
			break;
		}
	}
	return ret;
}

int wizardHandlerGroup_group_mode(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		if(para != g_tmp_group_struct.group_mode)
		{

			memset(&g_tmp_group_struct.probe ,0 ,sizeof(PROBE));
			memset(&g_tmp_group_struct.wedge ,0 ,sizeof(WEDGE));
			g_tmp_group_struct.probe.Model[0] =  32; /*  */

			if(para == PA_SCAN)
			{
				g_tmp_group_struct.probe.Elem_qty	=  16; /*  */
				g_tmp_group_struct.probe.Frequency	=  5000; /*  */
				g_tmp_group_struct.probe.Pitch	    =  500; /*  */
				g_tmp_group_struct.wedge.Orientation = 1 ;
			}

			g_tmp_group_struct.ut_unit                  =   UT_UNIT_SOUNDPATH  ;
			g_tmp_group_struct.law_info.Wave_type		=	LONGITUDINAL_WAVE;
			g_tmp_group_struct.law_info.Angle_min		=	0 ;
			g_tmp_group_struct.law_info.Angle_max		=	0 ;
			g_tmp_group_struct.law_info.Focal_type      =   0 ;

			g_tmp_group_struct.law_info.Elem_qty         =   1 ;
			g_tmp_group_struct.law_info.First_tx_elem    =   1 ;
			g_tmp_group_struct.law_info.First_rx_elem    =   1 ;
			g_tmp_group_struct.law_info.Last_tx_elem     =   1 ;
			g_tmp_group_struct.law_info.Last_rx_elem     =   1 ;
			g_tmp_group_struct.group_mode = para;
		    g_tmp_group_struct.tx_rxmode1 = 1 ; // PE Mode
		}
	}
	return TRUE;
}

int wizardHandlerGroup_geometry(void* p_para)
{
    double low, upper;
    int para = *(int*)p_para;
	if(NULL != p_para)
	{
		g_tmp_group_struct.part.Geometry = para  ;
        if (g_tmp_group_struct.part.Geometry != 0) {
            g_tmp_group_struct.part.Diameter = g_tmp_group_struct.part.Thickness *2;
		    if (g_tmp_group_struct.part.Diameter < 80000) {
                g_tmp_group_struct.part.Diameter = 80000;
            }
		    g_tmp_group_struct.law_info.Position_start = g_tmp_group_struct.part.Thickness;
            RefractAngleRange(&low, &upper, &g_tmp_group_struct, 0);
            if (low < 0 && upper > 0) {
                low = 0;
            } else if (low < 0 && upper < 0) {
                low = upper;
            }
            wizardFocallawHandler_min_angle(&low);
            wizardFocallawHandler_max_angle(&low);
        }
	}
	return TRUE;
}

int wizardHandlerGroup_thickness(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM != get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}

		float thickness = *(float*)p_para;
		g_tmp_group_struct.part.Thickness  = (unsigned int) (thickness * 1000.0 / multiplier);
		//set_part_thickness (pp->p_config, (GUINT_TO_POINTER
		//			((guint) (thickness * 1000.0 / multiplier))));
		gtk_widget_queue_draw (pp->vboxtable);
	}
	return TRUE;
}

int wizardHandlerGroup_diameter(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM == get_unit(pp->p_config))
		{
			multiplier = 1.0;
		}else{
			multiplier = 0.03937;
		}

		float diameter = *(float*)p_para;
		g_tmp_group_struct.part.Diameter = (unsigned int) (diameter * 1000.0 / multiplier);
		//set_part_diameter (pp->p_config, (GUINT_TO_POINTER
		//			((guint) (diameter * 1000.0 / multiplier))));
	}
	return TRUE;
}

int wizardHandlerGroup_pulse(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para + 0.5;
		set_group_val (&g_tmp_group_struct, GROUP_PULSER, (int)para);
		if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
			set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int)para);
	}
	return TRUE;
}

int wizardHandlerGroup_receive(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para + 0.5;
		set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int)para);
	}
	return TRUE;
}

int wizardHandlerGroup_tx_rx_mode(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		set_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE,  para);
		if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
			set_group_val (&g_tmp_group_struct,
					GROUP_RECEIVER, get_group_val (&g_tmp_group_struct, GROUP_PULSER));
	}
	return TRUE;
}

int wizardHandlerGroup_select_probe(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		int probe_mode = wizardGroup_getProbeMode();
		if(probe_mode)
		{
			if(para < 2)
			{
				set_probe_select(pp->p_config ,0);
			}
			else
			{
				set_probe_select(pp->p_config ,1);
			}
		}
		else
		{//probe_mode == 0;//PE
			set_probe_select(pp->p_config ,para);
		}
	}
	return TRUE;
}

int wizardHandlerGroup_probe_auto_detect(void* p_para)
{
	set_auto_detect (pp->p_config, !get_auto_detect(pp->p_config));
	if(get_auto_detect (pp->p_config))
	{
		RequestProbeAutoDetect();
	}
	return TRUE;
}


int wizardHandlerGroup_scan_off(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM == get_unit(pp->p_config))
		{
			multiplier = 1.0;
		}else{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.scan_offset =  (gint)(para * 10.0 / multiplier);
	}
	return TRUE;
}

int wizardHandlerGroup_skew(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;
		g_tmp_group_struct.skew =  (gushort) (para * 100.0);
	}
	return TRUE;
}

int wizardHandlerGroup_index_offset(void* p_para)
{
	if(NULL != p_para)
	{
		float multiplier = 1.0;
		if(UNIT_MM == get_unit(pp->p_config))
		{
			multiplier = 1.0;
		}else{
			multiplier = 0.03937;
		}

		float para = *(float*)p_para;
		g_tmp_group_struct.index_offset =  (gint)(para * 10.0 / multiplier);
	}
	return TRUE;
}

int wizardHandlerGroup_define_probe(void* p_para)
{
	int probeSelect;

	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		MENU_UNION menu_probe = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_PROBE}};//获取当前菜单的索引
		switch((probeSelect = popGetLastPos(menu_probe)))//
		{
		case 0://select tx
			draw_dialog_all (DIALOG_PROBE);
			break;
		case 1://select rx
			break;
		case 2://define
			draw_dialog_all (DIALOG_DEFINE_PROBE);
			break;
		default:
			break;
		}
	}else{//probe_mode == 0 ;PE
		MENU_UNION menu_probe_pe = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_PROBE_PE}};//选择定义菜单的索引
		switch((probeSelect = popGetLastPos(menu_probe_pe)))//
//		switch((probeSelect = get_probe_select(pp->p_config)))//
		{
		case 0://select tx/rx
			draw_dialog_all (DIALOG_PROBE);
			break;

		case 1://define
			draw_dialog_all (DIALOG_DEFINE_PROBE);
			break;

		default:
			break;
		}
	}
	return TRUE;
}

int wizardHandlerGroup_define_wedge(void* p_para)
{
	int wedgeSelect;

	int probe_mode = wizardGroup_getProbeMode();
	if(probe_mode)
	{
		MENU_UNION menu_wedge = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_WEDGE}};//获取当前菜单的索引
		switch((wedgeSelect = popGetLastPos(menu_wedge)))//
		{
		case 0://select tx
			draw_dialog_all (DIALOG_WEDGE);
			break;
		case 1://select rx
			break;
		case 2://define
			draw_dialog_all (DIALOG_DEFINE_WEDGE);
			break;
		default:
			break;
		}
	}
	else
	{//probe_mode == 0;PE
		MENU_UNION menu_wedge_pe = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_WEDGE_PE}};//获取当前菜单的索引
		switch((wedgeSelect = popGetLastPos(menu_wedge_pe)))//
		{
		case 0://select tx/rx
			draw_dialog_all (DIALOG_WEDGE);
			break;
		case 1://define
			draw_dialog_all (DIALOG_DEFINE_WEDGE);
			break;
		default:
			break;
		}
	}
	return TRUE;
}

int wizardHandlerGroup_material(void* p_para)
{
	//int material = g_tmp_group_struct.part.Material_pos ;
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		//set_part_material (pp->p_config, (void*)para);
		g_tmp_group_struct.part.Material_pos  = para ;
		g_tmp_group_struct.velocity = get_material_lw (para);
	}
	return TRUE;
}

int wizardHandlerGroup_continue(void* p_para)
{
	if(PA == g_tmp_group_struct.group_mode)
	{
		//模拟按键，切换到foclaw
		b2_fun0(pp, 1);
		//把group向导设回开始处
//		MENU_UNION menu_group_step = {{0 ,0 ,0 ,0}};
//		menuSetData(menu_group_step ,WIZARD_GROUP_STEP_START);
		wizardGroupHandler_setStep(WIZARD_GROUP_STEP_START);
		//切换到foclaw的设置开始处
//		MENU_UNION menu_foclaw_step = {{0 ,1 ,0 ,0}};//获取当前菜单的索引
//		menuSetData(menu_foclaw_step ,WIZARD_FOCLAW_STEP_LAW_CONFIGURATION);
		wizardFocallawHandler_setStep(WIZARD_FOCLAW_STEP_LAW_CONFIGURATION);
		//切换到foclaw时设置的组就是之前的组
		MENU_UNION menu_group_select = {{0 ,0 ,2 ,WIZARD_GROUP_STEP_GROUP_SELECT}};
		MENU_UNION menu_foclaw_group_select = {{0 ,1 ,2 ,WIZARD_FOCLAW_STEP_GROUP_SELECT}};
		popSetLastPos(menu_foclaw_group_select ,popGetLastPos(menu_group_select));
	}
	return TRUE;
}
