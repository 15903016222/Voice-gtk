/*
 * getFieldString.c
 *
 *  Created on: 2012-7-16
 *      Author: wolflord
 */
#include "../drawui.h"
#include "../file_op.h"
#include "../drawfb.h"
#include "../draw_dialog.h"
#include "../callback.h"
#include "../main.h"
#include "../calibration.h"
#include <stdlib.h>
#include <linux/rtc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "_string.h"
struct measureStringStruct
{
	const char* simpleStr;
	int strIndex;
};

static const struct measureStringStruct measureStr[] =
{
		{		"A%",		 _STRING_A100,		},
		{		"AdBA",		 _STRING_AdBA,		},
		{		"AdBr",		 _STRING_AdBr,		},
		{		"B%",		 _STRING_B100,		},
		{		"BdBB",		 _STRING_BdBB,		},
		{		"BdBr",		 _STRING_BdBr,		},
		{		"A^",		 _STRING_APeak,		},
		{		"A/",		 _STRING_AEdge,		},
		{		"B^",		 _STRING_BPeak,		},
		{		"B/",		 _STRING_BEdge,		},
		{		"I/",		 _STRING_IEdge,		},
		{		"I(w)/",		 _STRING_IEdgeInWater,		},
		{		"T(A^)",		 _STRING_ThicknessAPeak,		},
		{		"ML",		 _STRING_ML,		},
		{		"%(r)",		 _STRING_r100,		},
		{		"%(m)",		 _STRING_m100,		},
		{		"%(m-r)",		 _STRING_mr100,		},
		{		"U(r)",		 _STRING_Ur,		},
		{		"U(m)",		 _STRING_Um,		},
		{		"U(m-r)",		 _STRING_Umr,		},
		{		"P(r)",		 _STRING_Pr,		},
		{		"P(m)",		 _STRING_Pm,		},
		{		"P(m-r)",		 _STRING_Pmr,		},
		{		"S(r)",		 _STRING_Sr,		},
		{		"S(m)",		 _STRING_Sm,		},
		{		"S(m-r)",		 _STRING_Smr,		},
		{		"I(r)",		 _STRING_Ir,		},
		{		"I(m)",		 _STRING_Im,		},
		{		"I(m-r)",		 _STRING_Imr,		},
		{		"RA^",		 _STRING_RAPeak,		},
		{		"RA/",		 _STRING_RAEdge,		},
		{		"RB^",		 _STRING_RBPeak,		},
		{		"RB/",		 _STRING_RBEdge,		},
		{		"PA^",		 _STRING_PAPeak,		},
		{		"PA/",		 _STRING_PAEdge,		},
		{		"PB^",		 _STRING_PBPeak,		},
		{		"PB/",		 _STRING_PBEdge,		},
		{		"DA^",		 _STRING_DAPeak,		},
		{		"DA/",		 _STRING_DAEdge,		},
		{		"DB^",		 _STRING_DBPeak,		},
		{		"DB/",		 _STRING_DBEdge,		},
		{		"SA^",		 _STRING_SAPeak,		},
		{		"SA/",		 _STRING_SAEdge,		},
		{		"SB^",		 _STRING_SBPeak,		},
		{		"SB/",		 _STRING_SBEdge,		},
		{		"ViA^",		 _STRING_ViAPeak,		},
		{		"ViA/",		 _STRING_ViAEdge,		},
		{		"ViB^",		 _STRING_ViBPeak,		},
		{		"ViB/",		 _STRING_ViBEdge,		},
		{		"VsA^",		 _STRING_VsAPeak,		},
		{		"VsA/",		 _STRING_VsAEdge,		},
		{		"VsB^",		 _STRING_VsBPeak,		},
		{		"VsB/",		 _STRING_VsBEdge,		},
		{		"LA^",		 _STRING_LAPeak,		},
		{		"LA/",		 _STRING_LAEdge,		},
		{		"LB^",		 _STRING_LBPeak,		},
		{		"LB/",		 _STRING_LBEdge,		},
		{		"AWS-D A",		 _STRING_AWSDA,		},
		{		"AWS-D B",		 _STRING_AWSDB,		},
		{		"AWS-D C",		 _STRING_AWSDC,		},
		{		"AWS-D D",		 _STRING_AWSDD,		},
		{		"AWS-D 45",		 _STRING_AWSD45,	},
		{		"AWS-D 60",		 _STRING_AWSD60,	},
		{		"AWS-D 70",		 _STRING_AWSD70,	},
		{		"AWS-D CL",		 _STRING_AWSDCL,	},
		{		"E%",		 _STRING_E100,			},
		{		"E/",		 _STRING_EPeak,			},
		{		"E-3dB",		 _STRING_E3dB,		},
		{		"E-6dB",		 _STRING_E6dB,		},
		{		"E-12dB",		 _STRING_E12dB,		},
		{		"E-20dB",		 _STRING_E20dB,		},
		{		"API-DL",		 _STRING_APIDL,		},
		{		"API-HW",		 _STRING_APIHW,		},
		{		"Scale",		 _STRING_Scale,		},
		{		"%(U(r))",		 _STRING_Ur100,		},
		{		"%(U(m))",		 _STRING_Um100,		},
		{		"DA-DI" ,		_STRING_DaDi,		},

		{"T-D(r)" , _STRING_TOFD_Depth1},
		{"T-D(m)" , _STRING_TOFD_Depth2},
		{"T-S(m-r)" ,_STRING_TOFD_Length},
		{"T-D(m-r)" ,_STRING_TOFD_Height},

};

const int measureFeild[] =
{
		FEILD_A100,	//"A% 闸门A内峰值幅度",
		FEILD_AdBA,	//"AdBA  闸门A内峰值幅度与闸门阈值幅度之差（dB）",
		FEILD_AdBr,	//"AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）",
		FEILD_B100,	//"B%  闸门B内峰值幅度",
		FEILD_BdBB,	//"BdBB闸门B内峰值幅度与闸门阈值幅度之差（dB）",
		FEILD_BdBr,	//"BdBr  闸门B内峰值幅度与参考信号幅度之差（dB）",
		FEILD_APeak,	//"A^ 闸门A内峰值位置",
		FEILD_BPeak,	//"B^ 闸门B内峰值位置",
		FEILD_IEdge,	//"I/ 闸门I的前沿位置",
		FEILD_IEdgeInWater,	//"I(w)/  Position in gate I in water",
		FEILD_ThicknessAPeak,	//"T(A^)  厚度",
#ifndef HIGH_POWER
		FEILD_ML,		//"ML  材料损失百分比",
		FEILD_r100,//"%(r)  参考光标位置的幅度值",
		FEILD_m100,//"%(m)  测量光标位置的幅度值",
		FEILD_mr100,//"%(m-r) 测量光标的幅度值减去参考信号的幅度值)",
		FEILD_Ur,//"U(r)  参考光标在超声轴上的位置",
		FEILD_Um,//"U(m)  测量光标在超声轴上的位置",
		FEILD_Umr,//"U(m-r)  超声轴上测量光标位置与参考光标位置之差",
		FEILD_Pr,//"P(r)  相对于参考光标的探头位置",
		FEILD_Pm,//"P(m)  相对于测量光标的探头位置",
		FEILD_Pmr,//"P(m-r)  探头相对于测量光标与参考光标差值的位置",
#endif
		FEILD_Sr,//"S(r)  扫查轴上参考光标的位置",
		FEILD_Sm,//"S(m)  扫查轴上测量光标的位置",
		FEILD_Smr,//"S(m-r) 扫查轴上测量光标与参考光标的距离",
		FEILD_Ir,//"I(r)  步进轴上参考光标的位置",
		FEILD_Im,//"I(m)  步进轴上测量光标的位置",
		FEILD_Imr,//"I(m-r)  步进轴上测量光标与参考光标的距离",
		FEILD_RAPeak,//"RA^  声束出射点与闸门A内检测到的缺陷之间的距离",
		FEILD_RBPeak,//"RB^  声束出射点与闸门B内检测到的缺陷之间的距离",
		FEILD_PAPeak,//"PA^  探头前表面与闸门A内检测到的缺陷的距离",
		FEILD_PBPeak,//"PB^  探头前表面与闸门B内检测到的缺陷的距离",
		FEILD_DAPeak,//"DA^  闸门A内检测到的缺陷在工件中的深度",
		FEILD_DBPeak,//"DB^  闸门B内检测到的缺陷在工件中的深度",
		FEILD_SAPeak,//"SA^  声束出射点到闸门A内检测到的缺陷之间的声程",
		FEILD_SBPeak,//"SB^  声束出射点到闸门B内检测到的缺陷之间的声程",
		FEILD_ViAPeak,//"ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位",
		FEILD_ViBPeak,//"ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位",
		FEILD_VsAPeak,//"VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位",
		FEILD_VsBPeak,//"VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位",
		FEILD_LAPeak,//"LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数",
		FEILD_LBPeak,//"LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数",
#ifndef HIGH_POWER
		FEILD_AWSDA,//"AWS-D A  根据AWS-D1.5 A标准的指示电平",
		FEILD_AWSDB,//"AWS-D B  根据AWS-D1.5 B标准的零点参考电平",
		FEILD_AWSDC,//"AWS-D C  根据AWS-D1.5C标准的衰减系数",
		FEILD_AWSDD,//"AWS-D D  根据AWS-D1.5 D标准的指征率",
		FEILD_AWSD45,//"AWS-D 45  根据AWS-D1.5 45标准的不连续性严重等级",
		FEILD_AWSD60,//"AWS-D 60  根据AWS-D1.5 60标准的不连续性严重等级",
		FEILD_AWSD70,//"AWS-D 70  根据AWS-D1.5 70标准的不连续性严重等级",
		FEILD_AWSDCL,//"AWS-D CL  根据AWS-D1.5标准的不连续性严重等级",
#endif
//		FEILD_E100,//"E%  闸门A中包络线的峰值幅度",
//		FEILD_EPeak,//"E^  闸门A内包络线的峰值位置",
//		FEILD_E3dB,//"E-3dB  闸门A内信号在-3 dB处包络线的宽度",
//		FEILD_E6dB,//"E-6dB  闸门A内信号在-6dB处包络线的宽度",
//		FEILD_E12dB,//"E-12dB  闸门A内信号在-12dB处包络线的宽度",
//		FEILD_E20dB,//"E-20dB  闸门A内信号在-20dB处包络线的宽度",
//		FEILD_APIDL,//"API-DL根据API标准规定的缺陷长度",
//		FEILD_APIHW,//"API-HW  根据API 标准规定的半波宽度",
//		FEILD_Scale,//"比例  比例系数",
//		FEILD_Ur100,//"%(U(r))  参考光标在超声轴上对应的信号幅度",
//		FEILD_Um100,//"%(U(m))  测量光标在超声轴上对应的信号幅度",	  /* 203 Field */
		FEILD_DaDi,

		FEILD_Depth1,
		FEILD_Depth2,
		FEILD_Length,
		FEILD_Height,
};

int GetMesureFeildNum()
{
	return sizeof(measureFeild) / sizeof(int);
}

int GetMesureFeildIndex(int group ,int nIndex_)
{
	int numOfStr = sizeof(measureFeild) / sizeof(measureFeild[0]);
	if(nIndex_ >= numOfStr)
		nIndex_ = 0;
	int measureStrIndex;
	measureStrIndex = measureFeild[nIndex_];
	switch(measureStrIndex)
	{
	case FEILD_APeak:
	case FEILD_RAPeak:
	case FEILD_PAPeak:
	case FEILD_DAPeak:
	case FEILD_SAPeak:
	case FEILD_ViAPeak:
	case FEILD_VsAPeak:
	case FEILD_LAPeak:
		if (GROUP_VAL_POS(group , gate[GATE_A].measure) == 1)
		{
			measureStrIndex ++;
		}
		break;
	case FEILD_BPeak:
	case FEILD_RBPeak:
	case FEILD_PBPeak:
	case FEILD_DBPeak:
	case FEILD_SBPeak:
	case FEILD_ViBPeak:
	case FEILD_VsBPeak:
	case FEILD_LBPeak:
		if (GROUP_VAL_POS(group , gate[GATE_B].measure) == 1)
		{
			measureStrIndex ++;
		}
		break;
	default:
		break;
	}
	return measureStrIndex;
}

void GetMesureFeildString(const char** pSimpleStr ,const char** pStr)
{
	int i;
	int numOfStr = sizeof(measureFeild) / sizeof(int);
	int strIndex;
	int measureStrIndex;
    int group = get_current_group(pp->p_config);
	for(i = 0; i < numOfStr ;i++)
	{
		measureStrIndex = GetMesureFeildIndex(group ,i);
		pSimpleStr[i] = measureStr[measureStrIndex].simpleStr;
		if(pStr)
		{
			strIndex = measureStr[measureStrIndex].strIndex;
			pStr[i] = getDictString(strIndex);
		}
	}
}

char* getMeasureUnit(int grp ,int measureStrIndex)
{
	char* unit;
 	switch(measureStrIndex)//field1
 	{
	case FEILD_A100://case 0://A%
	case FEILD_B100://case 3://B%
	case FEILD_ML://case 11://ML
	case FEILD_r100://case 12://%(r)
	case FEILD_m100://case 13://%(m)
	case FEILD_mr100://case 14://%(m-r)
		unit = "%";
		break;
	case FEILD_LAPeak://case 39://LA
	case FEILD_LAEdge://case 39://LA
	case FEILD_LBPeak://case 40://LB
	case FEILD_LBEdge://case 40://LB
		unit = "times";
		break;
	case FEILD_AdBA://case 1://AdBA
	case FEILD_AdBr://case 2://AdBr
	case FEILD_BdBB://case 4://BdBB
	case FEILD_BdBr://case 5://BdBr
	case FEILD_AWSDA://case 41://AWS-D A
	case FEILD_AWSDB://case 42://AWS-D B
	case FEILD_AWSDC://case 43://AWS-D C
	case FEILD_AWSDD://case 44://AWS-D D
		unit = "dB";
		break;
	case FEILD_Ur://case 15://U(r)
	case FEILD_Um://case 16://U(m)
	case FEILD_Umr://case 17://U(m-r)
	case FEILD_APeak://case 6://A^
	case FEILD_AEdge://case 6://A^
	case FEILD_BPeak://case 7://B^
	case FEILD_BEdge://case 7://B^
	case FEILD_IEdge://case 8://I/
	case FEILD_IEdgeInWater://case 9://I(w)

	case FEILD_Depth1:
	case FEILD_Depth2:
	case FEILD_Length:
	case FEILD_Height:
		if  (UT_UNIT_TIME == GROUP_VAL_POS( grp , ut_unit))
		{
			unit = "us"  ;
		}
		else
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				unit =   "mm" ;
			}
			else
			{
				unit = "in" ;
			}
		}
		break;
	case FEILD_Pr://case 18://P(r)
	case FEILD_Pm://case 19://P(m)
	case FEILD_Pmr://case 20://P(m-r)
	case FEILD_Sr://case 21://S(r)
	case FEILD_Sm://case 22://S(m)
	case FEILD_Smr://case 23://S(m-r)
	case FEILD_Ir://case 24://I(r)
	case FEILD_Im://case 25://I(m)
	case FEILD_Imr://case 26://I(m-r)
	case FEILD_RAPeak://case 27://RA
	case FEILD_PAPeak://case 29://PA
	case FEILD_DAPeak://case 31://DA^
	case FEILD_SAPeak://case 33://SA^
	case FEILD_ViAPeak://case 35://ViA
	case FEILD_VsAPeak://case 37://VsA
	case FEILD_RAEdge://case 27://RA
	case FEILD_PAEdge://case 29://PA
	case FEILD_DAEdge://case 31://DA^
	case FEILD_SAEdge://case 33://SA^
	case FEILD_ViAEdge://case 35://ViA
	case FEILD_VsAEdge://case 37://VsA
	case FEILD_ThicknessAPeak://case 10://T(A^)
	case FEILD_RBPeak://case 28://RB
	case FEILD_PBPeak://case 30://PB
	case FEILD_DBPeak://case 32://DB^
	case FEILD_SBPeak://case 34://SB^
	case FEILD_ViBPeak://case 36://ViB
	case FEILD_VsBPeak://case 38://VsB
	case FEILD_RBEdge://case 28://RB
	case FEILD_PBEdge://case 30://PB
	case FEILD_DBEdge://case 32://DB^
	case FEILD_SBEdge://case 34://SB^
	case FEILD_ViBEdge://case 36://ViB
	case FEILD_VsBEdge://case 38://VsB
	case FEILD_DaDi:
		if (UNIT_MM == get_unit(pp->p_config))
		{
			unit =   "mm" ;
		}
		else
		{
			unit = "in" ;
		}
		break;
	case FEILD_AWSD45://case 45://AWS-D 45
	case FEILD_AWSD60://case 46://AWS-D 60
	case FEILD_AWSD70://case 47://AWS-D 75
	case FEILD_AWSDCL://case 48://AWS-D CL
	default:
		unit = "";
 		break;
 	}
	return unit;
}

char* getFieldStringName(int nIndex_)
{
    char* markup  = NULL ;
    const char* _strName = NULL ;
    const char* _strUnit = NULL ;
    int grp = get_current_group(pp->p_config);
  //  int _nTmp ;

    int measureStrIndex = GetMesureFeildIndex(grp ,nIndex_);
    _strName = measureStr[measureStrIndex].simpleStr;
    _strUnit = getMeasureUnit(grp ,measureStrIndex);
	markup = g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
				_strName, _strUnit);
     return markup ;
}
/*
char* getFieldStringName(int nIndex_ )
{
    char* markup  = NULL ;
    char* _strName = NULL ;
    char* _strUnit = NULL ;
    int grp = get_current_group(pp->p_config);
    int _nTmp ;

    nIndex_ =  GetMesureFeildIndex(nIndex_);
 	switch(nIndex_)//field1
 	{
 		case 0://A%
 		case 1://AdBA
 		case 2://AdBr
 		case 3://B%
 		case 4://BdBB
 		case 5://BdBr
 		case 11://ML
 		case 12://%(r)
 		case 13://%(m)
 		case 14://%(m-r)
 		case 39://LA
 		case 40://LB
 	    case 41://AWS-D A
 	 	case 42://AWS-D B
 	 	case 43://AWS-D C
 	 	case 44://AWS-D D
 	 	case 45://AWS-D 45
 	 	case 46://AWS-D 60
 	 	case 47://AWS-D 75
 	 	case 48://AWS-D CL
 		//case 41://%E//case 42://E^//case 43://E-3db//case 44://E-6db//case 45://E-12db
 		//case 46://E-20db//case 47://API-DL//case 48://API-HW//case 49://AWS-D A//case 50://AWS-D B
 	 	//case 51://AWS-D C//case 52://AWS-D D//case 53://AWS-D 45//case 54://AWS-D 60//case 55://AWS-D 70
 	 	//case 56://AWS-D CL//case 57://Scale//case 58://%(U(r))//case 59://%(U(m))
 			markup = g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
 						pp->field[nIndex_], pp->field_unit[nIndex_]);
 			break;
 		case 6://A^
 		case 7://B^
 		case 8://I/
 		case 9://I(w)
 			_nTmp = nIndex_ - 6  ;
 			if(_nTmp >= 3) _nTmp = 2 ;
 			if (GROUP_VAL_POS( grp , gate[_nTmp].measure) == 0)
 			{
 				_strName = (char*)field_en[nIndex_] ;
 			}
 			else
 			{
 				_strName = (char*)field_en_[nIndex_] ;
 			}
 			if  (UT_UNIT_TIME == GROUP_VAL_POS( grp , ut_unit))
 			{
 				_strUnit = "us"  ;
 			}
 			else
 			{
 				if (UNIT_MM == get_unit(pp->p_config))
 				{
 					_strUnit =   "mm" ;
 				}
 				else
 				{
 					_strUnit = "in" ;
 				}
 			}
 			markup = g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
 						_strName, _strUnit);
 			break;
 		case 15://U(r)
 		case 16://U(m)
 		case 17://U(m-r)
 		case 18://P(r)
 		case 19://P(m)
 		case 20://P(m-r)
 		case 21://S(r)
 		case 22://S(m)
 		case 23://S(m-r)
 		case 24://I(r)
 		case 25://I(m)
 		case 26://I(m-r)
			if (UNIT_MM == get_unit(pp->p_config))
			{
				_strUnit = "mm" ;
			}
			else
			{
				_strUnit = "in" ;
			}
			markup = g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
						pp->field[nIndex_], _strUnit);
 			break;
 		case 27://RA
 		case 29://PA
 		case 31://DA^
 		case 33://SA^
 		case 35://ViA
 		case 37://VsA
 		case 10://T(A^)
 			if (GROUP_VAL_POS( grp , gate[GATE_A].measure) == 0)
 			{
 				_strName = (char*)field_en[nIndex_] ;
 			}
 			else
 			{
 				_strName = (char*) field_en_[nIndex_] ;
 			}

			if (UNIT_MM == get_unit(pp->p_config))
			{
				_strUnit =   "mm" ;
			}
			else
			{
				_strUnit = "in" ;
			}
 			markup = g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
 						_strName, _strUnit);
 			break;
 		case 28://RB
 		case 30://PB
 		case 32://DB^
 		case 34://SB^
 		case 36://ViB
 		case 38://VsB
 			if (GROUP_VAL_POS( grp , gate[GATE_B].measure) == 0)
 			{
 				_strName = (char*)field_en[nIndex_] ;
 			}
 			else
 			{
 				_strName = (char*)field_en_[nIndex_] ;
 			}

			if (UNIT_MM == get_unit(pp->p_config))
			{
				_strUnit =   "mm" ;
			}
			else
			{
				_strUnit = "in" ;
			}
 			markup = g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
 						_strName, _strUnit);
 			break;
 		default :
 			break;
 	}

     return markup ;
}
//*/
//char* getFieldStringUnit(int nIndex )
//{
//	char* markup = NULL ;
//	return markup ;
//}
