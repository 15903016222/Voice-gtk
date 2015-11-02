/*
 * dialog_str.h
 *
 *  Created on: 2012-6-19
 *      Author: wolflord
 */

#ifndef DIALOG_STR_H_
#define DIALOG_STR_H_

//********************************************************************************************
// ***************** Probe define dialog string
//********************************************************************************************
const char*  strProbeDefineDialog_en[] = {
		"Model",
		"Serial",
		"Probe_Type",
		"Custom",
		"angle beam",
		"Contact",
		"Immersion",
		"Frequency MHz",
		"Element_Qty",
		"Ref_Point(mm)",
		"Pitch(mm)",
		"Element_Size(mm)",
		"Select and Close",
		"Save",
		"Cancel",
		"Delete"
        } ;
const char* strProbeDefineDialog_ch[]  ={
		"型号",
		"系列",
		"探头类型",
		"自定义",
		"斜入射",
		"接触式",
		"水浸",
		"频率 MHz",
		"阵元数",
		"参考点(mm)",
		"阵元间距(mm)",
		"晶片尺寸(mm)",
		"选择并关闭",
		"保存",
		"取消",
		"删除"
		};
const char** strProbeDefineDialog[] = {strProbeDefineDialog_en , strProbeDefineDialog_ch}  ;

//********************************************************************************************
// ***************** Wedge define dialog string
//********************************************************************************************
const char*  strWedgeDefineDialog_en[] = {
		"Model",
		"Serial",
		"Angle (deg)",
		"Orientation",
		"Reverse",
		"Normal",
		"Velocity (m/s)",
		"Pri_Offset (mm)",
		"Sec_Offset (mm)",
		"Height (mm)",
		"Probe_Delay (us)",
		"Wave_Type",
		"LW",
		"SW",
		"Ref_Point (mm)",
		"Select and Close",
		"Save",
		"Cancel",
		"Delete",
        "Length (mm)",
		} ;
const char*  strWedgeDefineDialog_ch[] = {
		"型号",
		"系列",
		"角度",
		"方向",
		"反转",
		"正向",
		"声速",
		"主轴偏置 (mm)",
		"次轴偏置 (mm)",
		"第一阵元高度(mm)",
		"探头延时 (us)",
		"波型",
		"纵波",
		"横波",
		"参考点(mm)",
		"选择并关闭",
		"保存",
		"取消",
		"删除",
        "长度(mm)",
		};
const char** strWedgeDefineDialog[] = {strWedgeDefineDialog_en , strWedgeDefineDialog_ch}  ;


//********************************************************************************************
// *****************select change string
//********************************************************************************************
const char* strProbeSelectChange_en[] ={
		"Selecting the \"Unknown\" probe will require you \n to manually configure additional settings." ,
		"This group contains all user-defined ultrasonic \n phased array probes.",
		"Ultrasonic phased array probe family."
};
const char* strProbeSelectChange_ch[] ={
		"在探头菜单选择“未知”时，要求手动编辑额外的设置" ,
		"该组包含了所有用户自定义的超声相控阵探头" ,
		"超声相控阵探头系列"
};

const char** strProbeSelectChange[] = {strProbeSelectChange_en , strProbeSelectChange_ch}  ;



const char* strWedgeSelectChange_en[] ={
		"Selecting the \"Unknown\" wedge will require you \n to manually configure aditional settings." ,
		"This group contains all user-defined ultrasonic \n phased array wedges.",
		"Ultrasonic phased array wedge family."
};
const char* strWedgeSelectChange_ch[] ={
		"在楔块菜单选择“未知”时，要求手动编辑额外的设置" ,
		"该组包含了所有用户自定义的超声相控阵楔块",
		"超声相控阵楔块系列"
};

const char** strWedgeSelectChange[] = {strWedgeSelectChange_en , strWedgeSelectChange_ch}  ;

const char* strProbeFileInfo_en[] = {
		"Model:%s     Freq.:%.2fMHz\nElement Qty.:%d    Pitch:%.3f mm\nRef. Point:-%.2f mm" ,
		"Model:%s     Freq.:%.2fMHz\nElement size:%.2f",
		"Probe Parameters Error!"
} ;
const char* strProbeFileInfo_ch[] = {
		"型号:%s     频率:%.2fMHz\n阵元数量:%d     阵元间距:%.2f mm\n参考点:-%.2f mm" ,
		"型号:%s     频率:%.2fMHz\n阵元大小:%.3f",
		"探头参数异常!"
} ;

const char** strProbeFileInfo[] = {strProbeFileInfo_en , strProbeFileInfo_ch}  ;

const char* strWedgeFileInfo_en[] = {
		"Model:%s     Angle:%.1f°\nWave Type:%s   Probe Delay:%.2f us\nRef. Point:%.2f mm" ,
		"Model:%s     Angle:%.1f°\nOrientation:%s   Height:%.2f mm\n Velocity:%.0f m/s   Pri Offset:%.2f mm\nSec Offset:%.3f mm Length:%.2f mm",
		"SW" ,"LW",
		"reversal","Normal" ,
		"Wedge Parameters Error!"
} ;
const char* strWedgeFileInfo_ch[] = {
		"型号:%s     角度:%.1f°\n波型:%s     探头延时:%.2f us\n参考点:%.3f mm" ,
		"型号:%s     角度:%.1f°\n方向:%s     高度:%.2f mm\n速度:%.0f m/s     主偏移:%.2f mm\n次偏移:%.2f mm 长度:%.2f mm",
		"横波" ,"纵波",
		"反向","正向" ,
		"楔块参数异常!"
} ;

const char** strWedgeFileInfo[] = {strWedgeFileInfo_en , strWedgeFileInfo_ch}  ;

#endif /* DIALOG_STR_H_ */
