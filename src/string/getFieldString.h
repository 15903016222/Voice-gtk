/*
 * getFieldString.h
 *
 *  Created on: 2012-7-16
 *      Author: wolflord
 */

#ifndef GETFIELDSTRING_H_
#define GETFIELDSTRING_H_

enum
{
	FEILD_A100,	//"A% 闸门A内峰值幅度",
	FEILD_AdBA,	//"AdBA  闸门A内峰值幅度与闸门阈值幅度之差（dB）",
	FEILD_AdBr,	//"AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）",
	FEILD_B100,	//"B%  闸门B内峰值幅度",
	FEILD_BdBB,	//"BdBB闸门B内峰值幅度与闸门阈值幅度之差（dB）",
	FEILD_BdBr,	//"BdBr  闸门B内峰值幅度与参考信号幅度之差（dB）",
	FEILD_APeak,	//"A^ 闸门A内峰值位置",
	FEILD_AEdge,
	FEILD_BPeak,	//"B^ 闸门B内峰值位置",
	FEILD_BEdge,
	FEILD_IEdge,	//"I/ 闸门I的前沿位置",
	FEILD_IEdgeInWater,	//"I(w)/  Position in gate I in water",
	FEILD_ThicknessAPeak,	//"T(A^)  厚度",
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
	FEILD_Sr,//"S(r)  扫查轴上参考光标的位置",
	FEILD_Sm,//"S(m)  扫查轴上测量光标的位置",
	FEILD_Smr,//"S(m-r) 扫查轴上测量光标与参考光标的距离",
	FEILD_Ir,//"I(r)  步进轴上参考光标的位置",
	FEILD_Im,//"I(m)  步进轴上测量光标的位置",
	FEILD_Imr,//"I(m-r)  步进轴上测量光标与参考光标的距离",
	FEILD_RAPeak,//"RA^  声束出射点与闸门A内检测到的缺陷之间的距离",
	FEILD_RAEdge,
	FEILD_RBPeak,//"RB^  声束出射点与闸门B内检测到的缺陷之间的距离",
	FEILD_RBEdge,//"RB^  声束出射点与闸门B内检测到的缺陷之间的距离",
	FEILD_PAPeak,//"PA^  探头前表面与闸门A内检测到的缺陷的距离",
	FEILD_PAEdge,//"PA^  探头前表面与闸门A内检测到的缺陷的距离",
	FEILD_PBPeak,//"PB^  探头前表面与闸门B内检测到的缺陷的距离",
	FEILD_PBEdge,//"PB^  探头前表面与闸门B内检测到的缺陷的距离",
	FEILD_DAPeak,//"DA^  闸门A内检测到的缺陷在工件中的深度",
	FEILD_DAEdge,//"DA^  闸门A内检测到的缺陷在工件中的深度",
	FEILD_DBPeak,//"DB^  闸门B内检测到的缺陷在工件中的深度",
	FEILD_DBEdge,//"DB^  闸门B内检测到的缺陷在工件中的深度",
	FEILD_SAPeak,//"SA^  声束出射点到闸门A内检测到的缺陷之间的声程",
	FEILD_SAEdge,//"SA^  声束出射点到闸门A内检测到的缺陷之间的声程",
	FEILD_SBPeak,//"SB^  声束出射点到闸门B内检测到的缺陷之间的声程",
	FEILD_SBEdge,//"SB^  声束出射点到闸门B内检测到的缺陷之间的声程",
	FEILD_ViAPeak,//"ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位",
	FEILD_ViAEdge,//"ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位",
	FEILD_ViBPeak,//"ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位",
	FEILD_ViBEdge,//"ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位",
	FEILD_VsAPeak,//"VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位",
	FEILD_VsAEdge,//"VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位",
	FEILD_VsBPeak,//"VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位",
	FEILD_VsBEdge,//"VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位",
	FEILD_LAPeak,//"LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数",
	FEILD_LAEdge,//"LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数",
	FEILD_LBPeak,//"LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数",
	FEILD_LBEdge,//"LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数",
	FEILD_AWSDA,//"AWS-D A  根据AWS-D1.5 A标准的指示电平",
	FEILD_AWSDB,//"AWS-D B  根据AWS-D1.5 B标准的零点参考电平",
	FEILD_AWSDC,//"AWS-D C  根据AWS-D1.5C标准的衰减系数",
	FEILD_AWSDD,//"AWS-D D  根据AWS-D1.5 D标准的指征率",
	FEILD_AWSD45,//"AWS-D 45  根据AWS-D1.5 45标准的不连续性严重等级",
	FEILD_AWSD60,//"AWS-D 60  根据AWS-D1.5 60标准的不连续性严重等级",
	FEILD_AWSD70,//"AWS-D 70  根据AWS-D1.5 70标准的不连续性严重等级",
	FEILD_AWSDCL,//"AWS-D CL  根据AWS-D1.5标准的不连续性严重等级",
	FEILD_E100,//"E%  闸门A中包络线的峰值幅度",
	FEILD_EPeak,//"E^  闸门A内包络线的峰值位置",
	FEILD_E3dB,//"E-3dB  闸门A内信号在-3 dB处包络线的宽度",
	FEILD_E6dB,//"E-6dB  闸门A内信号在-6dB处包络线的宽度",
	FEILD_E12dB,//"E-12dB  闸门A内信号在-12dB处包络线的宽度",
	FEILD_E20dB,//"E-20dB  闸门A内信号在-20dB处包络线的宽度",
	FEILD_APIDL,//"API-DL根据API标准规定的缺陷长度",
	FEILD_APIHW,//"API-HW  根据API 标准规定的半波宽度",
	FEILD_Scale,//"比例  比例系数",
	FEILD_Ur100,//"%(U(r))  参考光标在超声轴上对应的信号幅度",
	FEILD_Um100,//"%(U(m))  测量光标在超声轴上对应的信号幅度",	  /* 203 Field */
	FEILD_DaDi,

	FEILD_Depth1,
	FEILD_Depth2,
	FEILD_Length,
	FEILD_Height,
};


int GetMesureFeildNum();
void GetMesureFeildString(const char** pSimpleStr ,const char** pStr);
char* getFieldStringName(int nIndex_) ;
int GetMesureFeildIndex(int group ,int nIndex_);
char* getMeasureUnit(int grp ,int measureStrIndex);
#endif /* GETFIELDSTRING_H_ */
