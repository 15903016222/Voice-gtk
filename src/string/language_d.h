/*
 * language_d.h
 *
 *  Created on: 2012-7-12
 *      Author: lzk
 */

#ifndef LANGUAGE_D_H_
#define LANGUAGE_D_H_





typedef enum
{
	_STRING_NULL,//{NULL ,NULL},	//#
	_STRING_STICK,//{"-----------" ,"-----------"},	//#
	_STRING_HTML_SPACE,//{"&nbsp;" ,"&nbsp;"},	//#

	_STRING_Min_Angle,//{"Min.Angle","最小角度"},	//#
	_STRING_Max_Angle,//{"Max.Angle","最大角度"},	//#
	_STRING_Group_S,//{"Group","组选择"},		//#
	_STRING_Group_Select,//{"Group Select","组选择"},		//#
	_STRING_Group_Mode,//{"Group Mode","组模式"},		//#
	_STRING_Group,//{"Group","组"},	//#
	_STRING_LW__5890_0m_s,//{"LW  5890.0m/s","纵波  5890.0m/s"},	//#
	_STRING_LW,//{"LW","纵波"},	//#
	_STRING_Auto_Detect,//{"Auto Detect","自动识别"},	//#
	_STRING_Automatic,//{"Automatic","自动"},	//#
	_STRING_Define_Wedge,//{"Define Wedge","自定义楔块"},	//#
	_STRING_Define_Probe,//{"Define Probe","自定义探头"},	//#
	_STRING_Defines,//{"Defines","自定义"},	//#
	_STRING_Skew,//{"Skew","转向角"},	//#
	_STRING_Note,//{"Note","注释"},	//#
	_STRING_Axis,//{"Axis","轴"},	//#
	_STRING_Rename,//{"Rename","重命名"},		//#
	_STRING_PRF,//{"PRF","重复频率"},	//#
	_STRING_Stop_Angle,//{"Stop Angle","终止角度"},	//#
	_STRING_Position_End,//{"Position End","终点位置"},	//#
	_STRING_Center_Frequency,//{"Center Frequency","中心频率"},	//#
	_STRING_Element_Qty_,//{"Element Qty.","阵元数"},		//#
	_STRING_Used_Element_Qty_,//{"Used Element Qty.","阵元数"},	//#
	_STRING_Element_Step,//{"Element Step","阵元步进"},	//#
	_STRING_Resolution,//{"Resolution","阵元步进"},	//#
	_STRING_True_Depth,//{"True Depth","真实深度"},	//#
	_STRING_Gate_I,//{"Gate I","闸门 I"},	//#
	_STRING_Gate_B,//{"Gate B","闸门 B"},	//#
	_STRING_Gate_A,//{"Gate A","闸门 A"},	//#
	_STRING_Gate,//{"Gate","闸门"},	//#
	_STRING_Gain,//{"Gain","增益"},	//#
	_STRING_Add_Entry,//{"Add Entry","增加记录"},	//#
	_STRING_Add,//{"Add","增加"},	//#
    _STRING_No_CopySrc,//{"No Copy","不拷贝"},	//#
	_STRING_Source_Path,//{"Source Path","源路径"},		//#
	_STRING_Threshold,//{"Threshold","阈值"},	//#
	_STRING_AND,//{"AND","与"},	//#
	_STRING_Hardware_Update,//{"HW Update","硬件升级"},	//#
	_STRING_Silver,//{"Silver","银"},	//#
	_STRING_Move,//{"Move","移动"},	//#
	_STRING_Doppler_Type,//{"Doppler Type","仪器型号"},	//#
	_STRING_AL_OXIDE,//{"AL OXIDE","氧化铝"},	//#
	_STRING_Wedge_Delay,//{"Wedge Delay","延迟"},	//#
	_STRING_Scale_Factor,//{"Scale Factor","压缩系数"},	//#
	_STRING_Select_Probe,//{"Select Probe","选择探头"},	//#
    _STRING_Select_Wedge,//{"Select Wedge","选择楔块"},	//#
	_STRING_Select_Entry,//{"Select Entry","选择记录"},	//#
	_STRING_Select,//{"Select","选择"},	//#
	_STRING_Probe_Model,//{"Probe Model","型号"},	//#
	_STRING_Geometry,//{"Geometry","形状"},	//#
	_STRING_Zinc,//{"Zinc","锌"},	//#
	_STRING_Wedge_Model,//{"Wedge Model","楔块型号"},	//#
	_STRING_Wedge_Angle,//{"Wedge Angle","楔块角度"},	//#
	_STRING_Wedge,//{"Wedge","楔块"},	//#
	_STRING_Linear_sector_,//{"Linear          Linear sector","线性扫查"},	//#
	_STRING_Linear,//{"Linear","线性"},	//#
	_STRING_Display_Table,//{"Display Table","显示报表"},	//#
	_STRING_Probe_Serial__,//{"Probe Serial #","系列号"},		//#
	_STRING_Tin,//{"Tin","锡"},	//#
	_STRING_None,//{"None","无"},	//#
	_STRING_File_Preview,//{"File Preview","文件预览"},	//#
	_STRING_File_Exist_Overwrite,//{"File Exist, Overwrite","文件已存在, 覆盖"},	//#
	_STRING_File_Name,//{"File Name","文件名"},	//#
	_STRING_listed_Files,//{"listed Files","文件列表"},	//#
	_STRING_File_List,//{"File List","文件列表"},	//#
	_STRING_File_Type,//{"File Type","文件类型"},	//#
	_STRING_File_Manager,//{"File Manager","文件管理"},	//#
	_STRING_Position,//{"Position","位置"},	//#
	_STRING_Finish,//{"Finish","完成"},	//#
	_STRING_Image_,//{"Image","图片"},	//#
    _STRING_CAD,//{"CAD", "CAD“}, //#
    _STRING_TMPL, //{"Template", "报表模板"},   //#
	_STRING_OD,//{"OD","凸面"},	//#
	_STRING_Projection,//{"Projection","投影"},	//#
	_STRING_Copper,//{"Copper","铜"},	//#
	_STRING_Synchro,//{"Synchro","同步"},	//#
	_STRING_Iron,//{"Iron","铁"},	//#
	_STRING_Probe_Elements,//{"Probe Elements","探头阵元"},	//#
	_STRING_Probe,//{"Probe","探头型号"},	//#
	_STRING_Probe_Aperture,//{"Probe Aperture","探头孔径"},	//#
	_STRING_Probe_Characterization,//{"Probe Characterization","探头表征"},	//#
	_STRING_Titanium,//{"Titanium","钛"},	//#
	_STRING_Water,//{"Water","水"},	//#
	_STRING_Data_File_Name,//{"Data File Name","数据文件名"},	//#
	_STRING_Plexiglass,//{"Plexiglass","树脂玻璃"},	//#
	_STRING_Output,//{"Output","输出"},	//#
	_STRING_Tx_Rx_Mode,//{"Tx/Rx Mode","收发模式"},	//#
	_STRING_Mode,//{"Mode","收发模式"},	//#
	_STRING_Select_Tx_Rx,//{"Select Tx/Rx","收发"},	//#
	_STRING_Video_Filter,//{"Video Filter","视频滤波"},	//#
	_STRING_Time,//{"Time","时间"},	//#
	_STRING_Quartz_glass,//{"Quartz glass","石英玻璃"},	//#
	_STRING_Velocity_And_Delay,//{"Velocity And Delay","声速和延迟"},	//#
	_STRING_Velocity_,//{"Velocity","声速"},	//#
	_STRING_Sound_Velocity,//{"Sound Velocity","声速"},	//#
	_STRING_Beam_Delay,//{"Beam Delay","声束延迟"},	//#
	_STRING_Beam_Angle,//{"Beam Angle","声束角度"},	//#
	_STRING_Generate,//{"Generate","生成法则"},	//#
	_STRING_Generate_,//{"Generate","生成"},	//#
	_STRING_Contractor,//{"Contractor","审核"},	//#
	_STRING_Depth_sector,//{"Depth          Depth sector","深度扫查"},	//#
	_STRING_Depth,//{"Depth","深度"},	//#
	_STRING_Azimuthal,//{"Azimuthal","扇形"},	//#
	_STRING_Delete_Entry,//{"Delete Entry","删除记录"},	//#
	_STRING_Delete,//{"Delete","删除"},	//#
	_STRING_Scan_End,//{"Scan End","扫查终点"},	//#
	_STRING_Scan_Speed_,//{"Scan Speed","扫查速度"},	//#
	_STRING_Scan_Area,//{"Scan Area","扫查区域"},	//#
	_STRING_Scan_Start,//{"Scan Start","扫查起点"},	//#
	_STRING_Scan_Offset,//{"Scan Offset","扫查偏置"},	//#
	_STRING_Scan_Resolution,//{"Scan Resolution","扫查分辨率"},	//#
//	_STRING_Software_Update,//{"SW Update","软件升级"},	//#
    _STRING_Calibrate_Touch_Screen,//{"Calibrate Touch Screen","屏幕校准"}
    _STRING_Date,//{"Date","日期"},	//#
	_STRING_Focal_Plane,//{"Focal Plane","任意面"},	//#
	_STRING_Confirm,//{"Confirm","确认"},	//#
	_STRING_Table,//{"Table","缺陷报表"},	//#
	_STRING_Select_All,//{"Select All","全选"},	//#
	_STRING_Unselect_All,//{"Unselect All","全部取消选择"},	//#
	_STRING_All_,//{"All","全部"},	//#
	_STRING_Unselect,//{"Unselect","取消选择"},	//#
	_STRING_Cancel,//{"Cancel","取消"},	//#
	_STRING_Please_Enter_File_Name,//{"Please Enter File Name","请输入文件名"},	//#
	_STRING_Technician_Signature,//{"Technician Signature","签名"},	//#
	_STRING_Lead,//{"Lead","铅"},	//#
	_STRING_First_RX_Element,//{"First RX Element","起始接收阵元"},	//#
	_STRING_Start_Angle,//{"Start Angle","起始角度"},	//#
	_STRING_First_TX_Element,//{"First TX Element","起始发射阵元 "},	//#
	_STRING_Start__Half_Path_,//{"Start (Half Path)","起始点"},	//#
	_STRING_Position_Start,//{"Focal depth","聚焦深度"},	//#
	_STRING_Focus_Half,//{"Focal Radius","聚焦半径"},	//#
	_STRING_Start___,//{"Start","起点"},	//#
	_STRING_Steel_common,//{"Steel common","普通钢"},	//#
	_STRING_Averaging_Factor,//{"Averaging Factor","平均"},	//#
	_STRING_Probe_Frequency,//{"Probe Frequency","频率"},	//#
	_STRING_Setup_File_Name,//{"Setup File Name","配置文件"},	//#
	_STRING_Setup_,//{"Setup","配置"},	//#
	_STRING_Law_Config,//{"Law Config","配置"},	//#
	_STRING_Nickel,//{"Nickel","镍"},	//#
	_STRING_Nylon,//{"Nylon","尼龙"},	//#
	_STRING_Target_Path,//{"Target Path","目标路径"},	//#
	_STRING_Last_Element_,//{"Last Element","末阵元"},	//#
	_STRING_Last_RX_Element,//{"Last RX Element","末接收阵元"},	//#
	_STRING_Last_TX_Element,//{"Last TX Element","末发射阵元"},	//#
	_STRING_Analog_2___On_,//{"Analog 2  [On]","模拟 2  [开]"},	//#
	_STRING_Analog_2,//{"Analog 2","模拟 2"},	//#
	_STRING_Analog_1___On_,//{"Analog 1  [On]","模拟 1  [开]"},	//#
	_STRING_Analog_1,//{"Analog 1","模拟 1"},	//#
	_STRING_Pulse_Width,//{"Pulse Width","脉宽"},	//#
	_STRING_Aluminum,//{"Aluminum","铝"},	//#
	_STRING_Band_Pass_Filter,//{"Band-Pass Filter","滤波"},	//#
	_STRING_Rubber_vulcanized,//{"Rubber vulcanized","硫化橡胶"},	//#
	_STRING_Law_Type,//{"Law Type","聚焦法则类型"},	//#
	_STRING_Width_,//{"Width","宽度"},	//#
	_STRING_Source__,//{"Source","拷贝源选择"},	//#
	_STRING_Depth_Start,//{"Depth Start","开始深度"},	//#
	_STRING_Offset_Start,//{"Offset Start","开始偏移"},	//#
	_STRING_Start_s,//{"Start","开始"},	//#
	_STRING_Switch,//{"Switch","开关状态"},	//#
	_STRING_On,//{"On","开"},	//#
	_STRING_Teflon,//{"Teflon","聚四氟乙烯"},	//#
	_STRING_PVC,//{"PVC","聚氯乙烯"},	//#
	_STRING_Focus_Depth,//{"Focus_Depth","聚焦深度"},	//#
	_STRING_Focal_Type,//{"Focal Type","聚焦类型"},	//#
	_STRING_Law_Configuration,//{"Law Configuration","聚焦法则类型"},	//#
	_STRING_Polystyrene,//{"Polystyrene","聚苯乙烯"},	//#
	_STRING_Static_sector_,//{"Static          Static sector","静态扫查"},	//#
	_STRING_Static,//{"Static","静态"},	//#
	_STRING_Gold,//{"Gold","金"},	//#
	_STRING_Depth_End,//{"Depth End","结束深度"},	//#
	_STRING_Offset_End,//{"Offset End","结束偏移"},	//#
	_STRING_Receiver_,//{"Receiver","接收"},	//#
	_STRING_Select_Rx,//{"Select Rx","接收"},	//#
	_STRING_Inspection_Date,//{"Inspection Date","检测日期 "},	//#
	_STRING_Type_m,//{"Type","检测模式"},	//#
	_STRING_Inspection_Version,//{"Inspection Version","检测版本"},	//#
	_STRING_Rectification_,//{"Rectification","检波"},	//#
	_STRING_Technician_Name,//{"Technician Name","技术员"},	//#
	_STRING_Entry_Image,//{"Entry Image","记录图像"},	//#
	_STRING_Calculator,//{"Calculator","计算器"},	//#
	_STRING_Polarity,//{"Polarity","极性"},	//#
	_STRING_OR,//{"OR","或"},	//#
	_STRING_Brass,//{"Brass","黄铜"},	//#
	_STRING_Epoxy_resin,//{"Epoxy resin","环氧树脂"},	//#
	_STRING_Thickness_,//{"Thickness","厚度"},	//#
	_STRING_SW__3240_0m_s,//{"SW  3240.0m/s","横波  3240.0m/s"},	//#
	_STRING_SW,//{"SW","横波"},	//#
	_STRING_Off__No_Sound,//{"Off  No Sound","关  静音"},	//#
	_STRING_Off,//{"Off","关"},	//#
	_STRING_Procedure,//{"Procedure","工序"},	//#
	_STRING_Geometry_,//{"Geometry","工件形状"},	//#
	_STRING_Part,//{"Part","工件"},	//#
	_STRING_Higher_Frequency,//{"Higher Frequency","高端频率"},	//#
	_STRING_Copy,//{"Copy","复制"},	//#
	_STRING_Sound,//{"Sound","蜂鸣"},	//#
	_STRING_Encoder_Resolution,//{"Encoder Resolution","分辨率"},	//#
	_STRING_Not_Gate_I,//{"Not Gate I","非闸门I"},	//#
	_STRING_Not_Gate_B,//{"Not Gate B","非闸门B"},	//#
	_STRING_Not_Gate_A,//{"Not Gate A","非闸门A"},	//#
	_STRING_Range_is_too_large_nOr_Not_RF_Mode,//{"Range is too large\nOr Not RF Mode","范围太大或者\n不是射频模式"},	//#
	_STRING_Range__Half_Path_,//{"Range (Half Path)","范围"},	//#
	_STRING_Back,//{"Back","返回"},	//#
	_STRING_Law_Wizard,//{"Law Wizard","法则向导"},	//#
	_STRING_Connection_P,//{"Connection P","发射起始阵元"},	//#
	_STRING_Voltage,//{"Voltage","发射电压"},	//#
	_STRING_Pulser_,//{"Pulser","发射"},	//#
	_STRING_Select_Tx,//{"Select Tx","发射"},	//#
	_STRING_Multi,//{"Multi","多选"},	//#
	_STRING_read_setup_file_,//{"read setup file?","读取配置文件?"},	//#
	_STRING_read_data_file_,//{"read data file?","读取波型数据?"},		//#
	_STRING_Group_9_,//{"Group 9   √","第9 组    √"},	//#
	_STRING_Group_9,//{"Group 9","第9 组"},	//#
	_STRING_Group_8_,//{"Group 8   √","第8 组    √"},	//#
	_STRING_Group_8,//{"Group 8","第8 组"},	//#
	_STRING_Group_7_,//{"Group 7   √","第7 组    √"},	//#
	_STRING_Group_7,//{"Group 7","第7 组"},	//#
	_STRING_Group_6_,//{"Group 6   √","第6 组    √"},	//#
	_STRING_Group_6,//{"Group 6","第6 组"},	//#
	_STRING_Group_5_,//{"Group 5   √","第5 组    √"},	//#
	_STRING_Group_5,//{"Group 5","第5 组"},	//#
	_STRING_Group_4_,//{"Group 4   √","第4 组    √ "},	//#
	_STRING_Group_4,//{"Group 4","第4 组"},	//#
	_STRING_Group_3_,//{"Group 3   √","第3 组    √"},	//#
	_STRING_Group_3,//{"Group 3","第3 组"},	//#
	_STRING_Group_2_,//{"Group 2   √","第2 组    √"},	//#
	_STRING_Group_2,//{"Group 2","第2 组"},	//#
	_STRING_Group_16_,//{"Group 16  √","第16组    √"},	//#
	_STRING_Group_16,//{"Group 16","第16组"},	//#
	_STRING_Group_15_,//{"Group 15  √","第15组    √"},	//#
	_STRING_Group_15,//{"Group 15","第15组"},	//#
	_STRING_Group_14_,//{"Group 14  √","第14组    √"},	//#
	_STRING_Group_14,//{"Group 14","第14组"},	//#
	_STRING_Group_13_,//{"Group 13  √","第13组    √"},	//#
	_STRING_Group_13,//{"Group 13","第13组"},	//#
	_STRING_Group_12_,//{"Group 12  √","第12组    √"},	//#
	_STRING_Group_12,//{"Group 12","第12组"},	//#
	_STRING_Group_11_,//{"Group 11  √","第11组    √"},	//#
	_STRING_Group_11,//{"Group 11","第11组"},	//#
	_STRING_Group_10_,//{"Group 10  √","第10组    √"},	//#
	_STRING_Group_10,//{"Group 10","第10组"},	//#
	_STRING_Group_1_,//{"Group 1   √","第1 组    √"},	//#
	_STRING_Group_1,//{"Group 1","第1 组"},	//#
	_STRING_Lower_Frequency,//{"Lower Frequency","低端频率"},	//#
	_STRING_Current_View,//{"Current View","当前布局"},	//#
	_STRING_Bandwidth__MHz_,//{"Bandwidth (MHz)","带宽 (MHz)"},	//#
	_STRING_Bandwidth____,//{"Bandwidth (%)","带宽 (%)"},	//#
	_STRING_Open,//{"Open","打开"},	//#
	_STRING_Porcelain,//{"Porcelain","瓷器"},	//#
	_STRING_Count,//{"Count","触发次数"},	//#
	_STRING_Hold_Time,//{"Hold Time","持续时间"},	//#
	_STRING_Material,//{"Material","材料"},	//#
	_STRING_Index_Length,//{"Index Length","步进终点"},	//#
	_STRING_Position_Step,//{"Position Step","步进位置"},	//#
	_STRING_Index_Start,//{"Index Start","步进起点"},	//#
	_STRING_Index_Offset,//{"Index Offset","步进偏置"},	//#
	_STRING_Index_Resolution,//{"Index Resolution","步进分辨率"},	//#
	_STRING_Angle_Resolution,//{"Angle Resolution","步进"},	//#
	_STRING_Steel_stainless,//{"Steel stainless","不锈钢"},	//#
	_STRING_Wave_Type,//{"Wave Type","波型"},	//#
	_STRING_Ice,//{"Ice","冰"},	//#
	_STRING_Encoder_Type,//{"Encoder Type","编码器类型"},	//#
	_STRING_Encoder,//{"Encoder","编码器"},	//#
	_STRING_Edit_Comments,//{"Edit Comments","备注"},	//#
	_STRING_Data_,//{"Data","报警源"},	//#
	_STRING_Delay_,//{"Delay","报警延迟"},	//#
	_STRING_Condition2,//{"Condition2","报警条件2"},	//#
	_STRING_Condition1,//{"Condition1","报警条件1"},	//#
	_STRING_Operator,//{"Operator","报警逻辑"},	//#
	_STRING_Alarm__3___On_,//{"Alarm  3  [On]","报警 3  [开]"},	//#
	_STRING_Alarm__3,//{"Alarm  3","报警 3"},	//#
	_STRING_Alarm__2___On_,//{"Alarm  2  [On]","报警 2  [开]"},	//#
	_STRING_Alarm__2,//{"Alarm  2","报警 2"},	//#
	_STRING_Alarm__1___On_,//{"Alarm  1  [On]","报警 1  [开]"},	//#
	_STRING_Alarm__1,//{"Alarm  1","报警 1"},	//#
	_STRING_Alarm,//{"Alarm","报警"},	//#
	_STRING_Report_Date,//{"Report Date","报告日期"},	//#
	_STRING_Report_Version,//{"Report Version","报告版本"},	//#
	_STRING_Report,//{"Report","报告"},	//#
	_STRING_Save_Mode,//{"Save Mode","保存模式"},	//#
	_STRING_Save,//{"Save","保存 "},	//#
	_STRING_Half_Path,//{"Half Path","半声程"},	//#
	_STRING_Plate,//{"Plate","板"},	//#
	_STRING_ID,//{"ID","凹面"},	//#
	_STRING_UT2,//{"UT2","UT2"},	//#
	_STRING_UT1_Conventional_UT_,//{"UT1(Conventional UT)","UT1(独立通道1)"},	//#
	_STRING_UT1,//{"UT1","UT1"},	//#
	_STRING_UT__PA_Connector_,//{"UT (PA Connector)","UT (常规超声)"},	//#
	_STRING_UT,//{"UT","UT"},	//#
	_STRING_TT_Through_Transmission,//{"TT Through-Transmission","TT  穿透式"},	//#
	_STRING_TT,//{"TT","TT"},	//#
	_STRING_TOFD_Time_of_Flight_Diffraction,//{"TOFD Time-of-Flight Diffraction","TOFD 衍射声时"},	//#
	_STRING_TOFD,//{"TOFD","TOFD"},	//#
    _STRING_PhaScan_Report,//{"Phascan Report","Phascan 报告"},	//#
	_STRING_PE_Pulse_Echo,//{"PE Pulse-Echo","PE 脉冲回波"},	//#
	_STRING_PE,//{"PE","PE"},	//#
	_STRING_PC_Pitch_and_Catch,//{"PC Pitch-and-Catch","PC 一发一收"},	//#
	_STRING_PC,//{"PC","PC"},	//#
	_STRING_PA_Phassed_Array,//{"PA (Phassed Array)","PA (相控阵)"},	//#
	_STRING_PA,//{"PA","PA"},	//#
	_STRING_Characterization_Gain,//{"Characterization Gain","Characterization Gain"},	//#
	_STRING_Calibration_Block,//{"Calibration Block","Calibration Block"},	//#
	_STRING_BERYLIUM,//{"BERYLIUM","BERYLIUM铍"},	//#
	_STRING_B_,//{"B%","B%"},	//#
	_STRING_A_33,//{"A%","A%"},	//#
	_STRING_600Hz__Audio_output_at_600_Hz,//{"600Hz  Audio output at 600 Hz","600Hz  音频输出"},	//#
	_STRING_600Hz,//{"600Hz","600Hz"},	//#
	_STRING_5000Hz__Audio_output_at_5000_Hz,//{"5000Hz  Audio output at 5000 Hz","5000Hz  音频输出"},	//#
	_STRING_5000Hz,//{"5000Hz","5000Hz"},	//#
	_STRING_300Hz__Audio_output_at_300_Hz,//{"300Hz  Audio output at 300 Hz","300Hz  音频输出"},	//#
	_STRING_300Hz,//{"300Hz","300Hz"},	//#
	_STRING_1000Hz__Audio_output_at_1000_Hz,//{"1000Hz  Audio output at 1000 Hz","1000Hz  音频输出"},	//#
	_STRING_1000Hz,//{"1000Hz","1000Hz"},	//#
	_STRING_Max_Thickness_,//{">Max.Thickness",">最大厚度"},	//#
	_STRING_Min_Thickness_,//{"<Min.Thickness","<最小厚度"},	//#
	_STRING__n_Calibration_results_is_not_reasonable__n,//{"\n Calibration results is not reasonable \n","\n 校准结果不合理! \n"},	//#
	_STRING__n_velocity_is_not_reasonable__n,//{"\n velocity is not reasonable \n","\n 声速校准结果不合理 \n"},	//#
	_STRING__n__Copy_configure_of_Group___1__,//{"\n  Copy configure of Group   1 ?","\n  复制第一组的配置信息  ?"},	//#
	_STRING_Finish_Focallaw_Setting,//{"Finish Focallaw Setting","生成聚焦法则"},
	_STRING_U_Storage,//{"U Storage","U盘"},
	_STRING_Setup,//{"Setup","设置"},
	_STRING_Data,//{"Data","数据"},
	_STRING_Close,//{"Close","关闭"},
	_STRING_Setup_Lock,//{"Setup Lock","设置锁定"},
	_STRING_Remove,//{"Remove","删除"},
	_STRING_UT2_Conventional_UT_,//{"UT2(Conventional UT)","UT2(独立通道2)"},
	_STRING_Azimuthal_sector,//{"Azimuthal    Angular sector ","扇形扫查"},
	_STRING_Manual_Weld,//{"Manual Weld","手动焊缝"},
	_STRING_Auto_Weld,//{"Auto Weld","自动焊缝"},
	_STRING_User,//{"User","用户"},
	_STRING_Sample,//{"Sample","模板"},
	_STRING_Peak_Frequency,//{"Peak Frequency","峰值频率"},
	_STRING_Scan_Length,//{"Scan Length","扫查终点"},
	_STRING_Next,//{"Next","下一步"},
	_STRING_Diameter,//{"Diameter","直径"},
	_STRING_Connection_R,//{"Connection R","接收起始阵元"},
	_STRING_Continue,//{"Continue","继续"},
	_STRING_Focal_Type_,//{"Focal Type","聚焦方式"},
	_STRING_First_Element,//{"First Element","起始阵元"},
	_STRING_Angle_Step,//{"Angle Step","角度步进"},

	_STRING_Entry,//{"Entry","记录"},
	_STRING_Scan_t,//{"Scan","扫查轴"},
	_STRING_Index_t,//{"Index","步进轴"},
	_STRING_Channel,//{"Channel","通道"},
	_STRING_Comments,//{"Comments","注释"},
    _STRING_ABOUT,//{"About","关于"},
	_STRING_Reset_Config,//{"Reset Config","清除设置"},

	//////////////////////////////////////////////////////////////////////////////////////////////
	_STRING_CLB_VELOCITY_NOGO,//{"\n velocity is not reasonable \n" ,"\n 声速校准结果不合理 \n"},
	_STRING_CLBWEDGEDELAY_QUEST,//{"you should calibration velocity first \n\n\n  Continue ?" ,"请先进行声速校准 \n\n\n 继续  ?"},
	_STRING_CLB_DELAY_PA_NOGO,//{"At least one context has no amplitude peak in \n gate A. Wedge delay calibration cannot be performed. "
								//		,"闸门A采集不到所有聚焦法则的位置信息.\n 楔块延时不能校准！. "},
	_STRING_CLB_DELAY_UT_NOGO,//{"Wedge delay is not reasonable!\n\nWedge delay calibration\n cannot be performed. "
							//	,"楔块延时校准结果不合理!\n\n楔块延时不能校准！ "},
	_STRING_CLB_SENSATION_REJECT,//{"Calibration Result is not in tolerance!\n\nSensation calibration can not be accept！ "
								//			,"校准结果不在接受区间!\n\n灵敏度校准不能接受！ "},
	_STRING_CLB_UT_NOGO,//{"\n Calibration results is not reasonable \n" ,"\n 校准结果不合理! \n"},
	_STRING_COPY_GROUP_QUEST,//{"\n  Copy configure of Group:   1 ?" ,"\n  复制第一组的配置信息  ?"},
	_STRING_GROUP_ADD_NOT_AVAILABEL,//{"Group add invalidate!" ,"组添加失败！"} ,
	_STRING_FOCALLAW_NOT_AVAILABEL,//{"Focallaw settings invalidate!" ,"聚焦法则参数超出允许范围！"} ,
	_STRING_CLB_SENSATION_NOGO,//{"At least one beam has max amplitude smaller than Gate A Height! Calibration cannot be performed."
							  //,"存在闸门A采集的幅度小于闸门高度.\n 灵敏度不能校准！."},
	_STRING_CLB_SENSATION_AMP_NOGO,//{"Max amplitude should not bigger than 100!" ,"最在幅度应该小于100％！"},
	_STRING_GROUP_MODE_CHANGE,//{"Current group setting will lose! \n\n\n  Continue ?" ,"当前组配置信息将丢失! \n\n\n  继续 ?"},
	_STRING_FILE_SAVE_FINISHED,// {"File Save Finished！" ,"文件保存成功！"},
	_STRING_FILE_SAVE_ERROR,//    {"File Save Failed ！" ,"文件保存失败！"},
	_STRING_DELETE_WARNING,//{"Delete file" ,"删除文件"},

	_STRING_read_part_file_error,//{"read part file error? ","读取工件文件错误?"},
	_STRING_APPLY_EDIT_TO_ALL_LAWS_DAC,//{"Apply current law DAC parameters to all laws ?" , "是否将当前聚焦法则DAC参数更新到全部聚焦法则 ？"}
	_STRING_APPLY_EDIT_TO_ALL_LAWS_TCG,//{"Apply current law TCG parameters to all laws ?" , "是否将当前聚焦法则TCG参数更新到全部聚焦法则 ？"}
	_STRING_TCG_CLB_ERROR1,//{"At least one beam has max amplitude larger than 150%! Calibration cannot be performed." ,"存在闸门A采集的幅度高于150%.\n TCG不能校准！."},
	_STRING_TCG_CLB_ERROR2,//{"At least one beam has larger amplitude than Reference! Please turn Gain down." ,"存在闸门A采集的幅度大于参考幅度.\n 请调小增益！."},

	////////////////////////////////////////////////////////////////////////////////////////////
	_STRING_A100,//{		"A%  Peak amplitude in gate A",				"A% 闸门A内峰值幅度",	},
	_STRING_AdBA,//{		"AdBA  Signal in gate minus gate level for gate A",				"AdBA  闸门A内峰值幅度与闸门阈值幅度之差（dB）",	},
	_STRING_AdBr,//{		"AdBr  Diff. between signal and reference in gate A",				"AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）",	},
	_STRING_B100,//{		"B%  Peak amplitude in gate B",				"B%  闸门B内峰值幅度",	},
	_STRING_BdBB,//{		"BdBB Signal in gate minus gate level for gate B",				"BdBB闸门B内峰值幅度与闸门阈值幅度之差（dB）",	},
	_STRING_BdBr,//{		"BdBr  Diff. between signal and reference in gate B",				"BdBr  闸门B内峰值幅度与参考信号幅度之差（dB）",	},
	_STRING_APeak,//{		"A^  Position in gate A",				"A^ 闸门A内峰值位置",	},
	_STRING_AEdge,//{		"A/  Position in gate A",				"A/ 闸门A内前沿位置",	},
	_STRING_BPeak,//{		"B^  Position in gate B",				"B^ 闸门B内峰值位置",	},
	_STRING_BEdge,//{		"B/  Position in gate B",				"B/ 闸门B内前沿位置",	},
	_STRING_IEdge,//{		"I/  Position in gate I",				"I/ 闸门I的前沿位置",	},
	_STRING_IEdgeInWater,//{		"I(w)/  Position in gate I in water",				"I(w)/ 闸门I的前沿位置(水)",	},
	_STRING_ThicknessAPeak,//{		"T(A^)  Thickness",				"T(A/)  厚度",	},
	_STRING_ML,//{		"ML  Percentage of material loss",				"ML  材料损失百分比",	},
	_STRING_r100,//{		"%(r)  Amplitude value for the reference cursor",				"%(r)  参考光标位置的幅度值",	},
	_STRING_m100,//{		"%(m)  Amplitude value of the measurement cursor",				"%(m)  测量光标位置的幅度值",	},
	_STRING_mr100,//{		"%(m-r)  Amplitude (measurement cursor - reference cursor)",				"%(m-r) 测量光标的幅度值减去参考信号的幅度值",	},
	_STRING_Ur,//{		"U(r)  Ultrasonic position of the reference cursor",				"U(r)  参考光标在超声轴上的位置",	},
	_STRING_Um,//{		"U(m)  Ultrasonic position of the measurement cursor",				"U(m)  测量光标在超声轴上的位置",	},
	_STRING_Umr,//{		"U(m-r)  Ultrasonic axis (measurement cursor - reference cursor)",				"U(m-r)  超声轴上测量光标位置与参考光标位置之差",	},
	_STRING_Pr,//{		"P(r)  Probe position of the reference cursor",				"P(r)  相对于参考光标的探头位置",	},
	_STRING_Pm,//{		"P(m)  Probe position of the measurement cursor",				"P(m)  相对于测量光标的探头位置",	},
	_STRING_Pmr,//{		"P(m-r)  Probe (measurement cursor - reference cursor)",				"P(m-r)  探头相对于测量光标与参考光标差值的位置",	},
	_STRING_Sr,//{		"S(r)  Scan position of the reference cursor",				"S(r)  扫查轴上参考光标的位置",	},
	_STRING_Sm,//{		"S(m)  Scan position of the measurement cursor",				"S(m)  扫查轴上测量光标的位置",	},
	_STRING_Smr,//{		"S(m-r) Scan axis (measurement cursor - reference cursor)",				"S(m-r) 扫查轴上测量光标与参考光标的距离",	},
	_STRING_Ir,//{		"I(r)  Index position of the reference cursor",				"I(r)  步进轴上参考光标的位置",	},
	_STRING_Im,//{		"I(m)  Index position of the measurement cursor",				"I(m)  步进轴上测量光标的位置",	},
	_STRING_Imr,//{		"I(m-r)  Index axis (measurement cursor - reference cursor)",				"I(m-r)  步进轴上测量光标与参考光标的距离",	},
	_STRING_RAPeak,//{		"RA^  Reference point to the indication in gate A",				"RA^  声束出射点与闸门A内检测到的缺陷之间的距离",	},
	_STRING_RAEdge,//{		"RA/  Reference point to the indication in gate A",				"RA/  声束出射点与闸门A内检测到的缺陷之间的距离",	},
	_STRING_RBPeak,//{		"RB^  Reference point to the indication in gate B",				"RB^  声束出射点与闸门B内检测到的缺陷之间的距离",	},
	_STRING_RBEdge,//{		"RB/  Reference point to the indication in gate B",				"RB/  声束出射点与闸门B内检测到的缺陷之间的距离",	},
	_STRING_PAPeak,//{		"PA^  Probe front face to the indication in gate A",				"PA^  探头前表面与闸门A内检测到的缺陷的距离",	},
	_STRING_PAEdge,//{		"PA/  Probe front face to the indication in gate A",				"PA/  探头前表面与闸门A内检测到的缺陷的距离",	},
	_STRING_PBPeak,//{		"PB^  Probe front face to the indication in gate B",				"PB^  探头前表面与闸门B内检测到的缺陷的距离", 	},
	_STRING_PBEdge,//{		"PB/  Probe front face to the indication in gate B",				"PB/  探头前表面与闸门B内检测到的缺陷的距离",	},
	_STRING_DAPeak,//{		"DA^  Depth of the indication in gate A",				"DA^  闸门A内检测到的缺陷在工件中的深度",	},
	_STRING_DAEdge,//{		"DA/  Depth of the indication in gate A",				"DA/  闸门A内检测到的缺陷在工件中的深度",	},
	_STRING_DBPeak,//{		"DB^  Depth of the indication in gate B",				"DB^  闸门B内检测到的缺陷在工件中的深度", 	},
	_STRING_DBEdge,//{		"DB/  Depth of the indication in gate B",				"DB/  闸门B内检测到的缺陷在工件中的深度",	},
	_STRING_SAPeak,//{		"SA^  Sound path of the indication in gate A",				"SA^  声束出射点到闸门A内检测到的缺陷之间的声程",	},
	_STRING_SAEdge,//{		"SA/  Sound path of the indication in gate A",				"SA/  声束出射点到闸门A内检测到的缺陷之间的声程",	},
	_STRING_SBPeak,//{		"SB^  Sound path of the indication in gate B",				"SB^  声束出射点到闸门B内检测到的缺陷之间的声程",	},
	_STRING_SBEdge,//{		"SB/  Sound path of the indication in gate B",				"SB/  声束出射点到闸门B内检测到的缺陷之间的声程",	},
	_STRING_ViAPeak,//{		"ViA^  Volumetric position in gate A on the index axis",				"ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位",	},
	_STRING_ViAEdge,//{		"ViA/  Volumetric position in gate A on the index axis",				"ViA/  闸门A内检测到的缺陷相对于步进轴的体积定位",	},
	_STRING_ViBPeak,//{		"ViB^  Volumetric position in gate B on the index axis",				"ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位",	},
	_STRING_ViBEdge,//{		"ViB/  Volumetric position in gate B on the index axis",				"ViB/  闸门B内检测到的缺陷相对于步进轴的体积定位",	},
	_STRING_VsAPeak,//{		"VsA^  Volumetric position in gate A on the scan axis",				"VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位",	},
	_STRING_VsAEdge,//{		"VsA/  Volumetric position in gate A on the scan axis",				"VsA/  闸门A内检测到的缺陷相对于扫查轴的体积定位",	},
	_STRING_VsBPeak,//{		"VsB^  Volumetric position in gate B on the scan axis",				"VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位",	},
	_STRING_VsBEdge,//{		"VsB/  Volumetric position in gate B on the scan axis",				"VsB/  闸门B内检测到的缺陷相对于扫查轴的体积定位",	},
	_STRING_LAPeak,//{		"LA^  Leg (skip) of the indication in gate A",				"LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数",	},
	_STRING_LAEdge,//{		"LA/  Leg (skip) of the indication in gate A",				"LA/  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数",	},
	_STRING_LBPeak,//{		"LB^  Leg (skip) of the indication in gate B",				"LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数",	},
	_STRING_LBEdge,//{		"LB/  Leg (skip) of the indication in gate B",				"LB/  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数",	},
	_STRING_AWSDA,//{		"AWS-D A  Indication level for AWS-D1.5 A",				"AWS-D A  根据AWS-D1.5 A标准的指示电平",	},
	_STRING_AWSDB,//{		"AWS-D B  Zero reference level for AWS-D1.5 B",				"AWS-D B  根据AWS-D1.5 B标准的零点参考电平",	},
	_STRING_AWSDC,//{		"AWS-D C  Attenuation factor for AWS-D1.5 C",				"AWS-D C  根据AWS-D1.5C标准的衰减系数",	},
	_STRING_AWSDD,//{		"AWS-D D  Indication rating for AWS-D1.5 D",				"AWS-D D  根据AWS-D1.5 D标准的指征率",	},
	_STRING_AWSD45,//{		"AWS-D 45  Discontinuity Severity Class for AWS-D1.5 45",				"AWS-D 45  根据AWS-D1.5 45标准的不连续性严重等级",	},
	_STRING_AWSD60,//{		"AWS-D 60  Discontinuity Severity Class for AWS-D1.5 60",				"AWS-D 60  根据AWS-D1.5 60标准的不连续性严重等级",	},
	_STRING_AWSD70,//{		"AWS-D 70  Discontinuity Severity Class for AWS-D1.5 70",				"AWS-D 70  根据AWS-D1.5 70标准的不连续性严重等级",	},
	_STRING_AWSDCL,//{		"AWS-D CL  Discontinuity Severity Class for AWS-D1.5",				"AWS-D CL  根据AWS-D1.5标准的不连续性严重等级",	},
	_STRING_E100,//{		"E%  Peak amplitude of the envelope in gate A",				"E%  闸门A中包络线的峰值幅度",	},
	_STRING_EPeak,//{		"E^  Position of envelop in gate A",				"E^  闸门A内包络线的峰值位置",	},
	_STRING_E3dB,//{		"E-3dB  Envelop signal at -3 dB in gate A",				"E-3dB  闸门A内信号在-3 dB处包络线的宽度",	},
	_STRING_E6dB,//{		"E-6dB  Envelop signal at -6 dB in gate A",				"E-6dB  闸门A内信号在-6dB处包络线的宽度",	},
	_STRING_E12dB,//{		"E-12dB  Envelop signal at -12 dB in gate A",				"E-12dB  闸门A内信号在-12dB处包络线的宽度",	},
	_STRING_E20dB,//{		"E-20dB  Envelop signal at -20 dB in gate A",				"E-20dB  闸门A内信号在-20dB处包络线的宽度",	},
	_STRING_APIDL,//{		"API-DL  Defect length extension according to API code",				"API-DL根据API标准规定的缺陷长度",	},
	_STRING_APIHW,//{		"API-HW  Half-wave width according to API code",				"API-HW  根据API 标准规定的半波宽度",	},
	_STRING_Scale,//{		"Scale  Scale factor",				"比例  比例系数",	},
	_STRING_Ur100,//{		"%(U(r))  Signal amplitude at reference ultrasonic cursor position",				"%(U(r))  参考光标在超声轴上对应的信号幅度", 	},
	_STRING_Um100,//{		"%(U(m))  Signal amplitude at measurement ultrasonic cursor position",				"%(U(m))  测量光标在超声轴上对应的信号幅度",	},
	_STRING_DaDi,//{		"DA - DI",				"DA - DI",	},
	_STRING_TOFD_Depth1,   //{"Depth","深度"},	//#
	_STRING_TOFD_Depth2,   //{"Depth","深度"},	//#
	_STRING_TOFD_Length,//{"TOFD Length" ,"TOFD 长度"},
	_STRING_TOFD_Height,//{"TOFD Height" ,"TOFD 高度"},
	_STRING_Straightening,//{"Straightening" ,"直通波拉直"},
	_STRING_Remove_Lateral_Wave,//{"Remove Lateral" ,"去除直通波"},
	_STRING_Cancel__,//{"Cancel" ,"清除处理"},
	_STRING_RefLine,//{"Ref. Line" ,"参考线"},
    _STRING_RefLine_Depth,//{"Ref. Position", "参考位置"},
    _STRING_DepthCal,//{"Depth Cal.", "深度校准"},
	_STRING_WedgeSeparation,//{"Wedge Sep." ,"楔块间距"},
	_STRING_PCS,//{"PCS","探头中心距"},
	_STRING_Select_Average,//{"Please Select Average AScan with Ref Line" ,"请用参考线选择目标波形"},
	_STRING_Not_Tofd,//{"Not TOFD Mode!" ,"非TOFD模式"},
	_STRING_CLB_ENCODER_BEGIN,//{"Please move to origin position !" ,"请移动到起点位置 !"},
	_STRING_CLB_ENCODER_END,//{"Please move to stop position !" ,"请移动到终点位置 !"},
	_STRING_DISK_VOLUME_LITTLE,//    {"Disk volume no enough, Save Failed!" ,"空间不足，保存失败！"},
	_STRING_DISK_VOLUME_THRESHOLD,// {"File Save Finished, Not enough space, Delete some files." ,"文件保存成功, 但空间不足，请清理数据"},
	_STRING_CMD_APPLY_ALL,// {"Apply to all.", "应用至所有!"},
	_STRING_NOTICE_DELETE_GROUP,//{"Delete ", "确定删除"},
#if 0
	_STRING_Straight__Right,//{"Straight  Right","左单边坡口"},
	_STRING_Min,//{"Min","最小值"},
	_STRING_Min_Beamskew,//{"Min.Beamskew","最小声束转向角"},
	_STRING_Min_,//{"Min.","最小厚度"},
	_STRING_Min_Thickness,//{"Min.Thickness","最小厚度"},
	_STRING_Thinnest,//{"Thinnest","最小厚度"},
	_STRING_Optimum,//{"Optimum","最佳"},
	_STRING_Last_Element,//{"Last Element","最后一个阵元"},
	_STRING_Last_Angle,//{"Last Angle","最后一个角度"},
	_STRING_Last_VPA,//{"Last VPA","最后一个波束"},
	_STRING_LAST_VPA,//{"LAST VPA","最后VPA "},
	_STRING_Max,//{"Max","最大值"},
	_STRING_Max_Beamskew,//{"Max.Beamskew","最大声束转向角"},
	_STRING_Max___,//{"Max.  ","最大厚度"},
	_STRING_Max_Thickness,//{"Max.Thickness","最大厚度"},
	_STRING_Auto_Max,//{"Auto Max","最大"},
	_STRING_Group_A,//{"Group A","组 A"},
	_STRING_Group_B,//{"Group B","组 B"},
	_STRING_Group_8__,//{"Group 8","组 8"},
	_STRING_Group_7__,//{"Group 7","组 7"},
	_STRING_Group_6__,//{"Group 6","组 6"},
	_STRING_Group_5__,//{"Group 5","组 5"},
	_STRING_Group_4__,//{"Group 4","组 4"},
	_STRING_Group_3__,//{"Group 3","组 3"},
	_STRING_Group_2__,//{"Group 2","组 2"},
	_STRING_Group_1__,//{"Group 1","组 1"},
	_STRING_Source,//{"Source","组"},
	_STRING_Damping,//{"Damping","阻尼"},
	_STRING_Sum_Gain,//{"Sum Gain","总增益"},
	_STRING_Auto_Program,//{"Auto Program","自动计算"},
	_STRING_Auto,//{"Auto","自动"},
	_STRING_UserDef,//{"UserDef","自定义"},
	_STRING_Subnet_Mask,//{"Subnet Mask","子网掩码"},
	_STRING_BeamskewStep,//{"BeamskewStep","转向角步进"},
	_STRING_Skew___,//{"Skew(°)","转向角(°)"},
	_STRING_Notes,//{"Notes","注释"},
	_STRING_Properties,//{"Properties","属性"},
	_STRING_Label,//{"Label","主题"},
	_STRING_Preset_Encoder_2,//{"Preset Encoder 2","重置编码器 2"},
	_STRING_Reset_Encoder,//{"Reset Encoder","重置编码器"},
	_STRING_Reset_All,//{"Reset All","重新设置"},
	_STRING_Restart,//{"Restart","重新开始"},
	_STRING_Max_Angle_,//{"Max.Angle_","终止角度"},
	_STRING_End,//{"End","终止"},
	_STRING_Chinese,//{"Chinese","中文"},
	_STRING_Hollow_2_Colors,//{"Hollow 2 Colors","中空 双色"},
	_STRING_Hollow,//{"Hollow","中空"},
	_STRING_Paper_Size,//{"Paper Size","纸张类型"},
	_STRING_Quad_Down,//{"Quad Down","正交向下"},
	_STRING_Quad_Up,//{"Quad Up","正交向上"},
	_STRING_Quad,//{"Quad","正交"},
	_STRING_HW_I_,//{"HW+","正半波"},
	_STRING_Raster_Scan,//{"Raster Scan","栅格扫查"},
	_STRING_Grid,//{"Grid","栅格"},
	_STRING_Start,//{"Start","闸门起点"},
	_STRING_Width,//{"Width","闸门宽度"},
	_STRING_Gate_Alarm,//{"Gate/Alarm","闸门/报警"},
	_STRING_Comp_Gain_,//{"Comp.Gain ","增益补偿"},
	_STRING_Gain_Offset,//{"Gain Offset","增益补偿"},
	_STRING_Gain_Ref__,//{"Gain(Ref.)","增益(参考)"},
	_STRING_Add_,//{"Add","增加组"},
	_STRING_Add_Point,//{"Add Point","增加点"},
	_STRING_Pause_Resume,//{"Pause/Resume","暂停/重新开始"},
	_STRING_Pause,//{"Pause","暂停"},
	_STRING_Preset,//{"Preset","预置编码器"},
	_STRING_Language,//{"Language","语言"},
	_STRING_Straight__Left,//{"Straight  Left","右单边坡口"},
	_STRING_User_field,//{"User field","用户栏"},
	_STRING_Inches,//{"Inches","英寸"},
	_STRING_Apply_,//{"Apply ","应用"},
	_STRING_Reject,//{"Reject","抑制"},
	_STRING_Color,//{"Color","颜色"},
	_STRING_Delay,//{"Delay","延迟时间"},
	_STRING_Compression,//{"Compression","压缩"},
	_STRING_Select_Tx_Rx_,//{"Select Tx/Rx ","选择系列 "},
	_STRING_Select_Code,//{"Select Code","选择标准"},
	_STRING_Selection,//{"Selection","选择"},
	_STRING_NAME,//{"NAME","姓名"},
	_STRING_Wedge_delay_is_not_reasonable,//{"Wedge delay is not reasonable!\n\nWedge delay calibration\n cannot be performed.","楔块延时校准结果不合理!\n\n楔块延时不能校准！"},
	_STRING_Wedge_Delay_,//{"Wedge Delay","楔块延迟"},
	_STRING_Calibrate,//{"Calibrate","校准"},
	_STRING_Calibration,//{"Calibration","校准"},
	_STRING_Project,//{"Project","项目"},
	_STRING_Down,//{"Down","向下"},
	_STRING_Up,//{"Up","向上"},
	_STRING_Wizard,//{"Wizard","向导"},
	_STRING_Linear_sector,//{"Linear sector","线性扫查"},
	_STRING_Linear_DAC,//{"Linear DAC","线性DAC"},
	_STRING_Selection_,//{"Selection","显示模式"},
	_STRING_Display,//{"Display","显示"},
	_STRING_Next_Section,//{"Next Section","下一区间"},
	_STRING_System_Info,//{"System Info","系统信息"},
	_STRING_System,//{"System","系统设置"},
	_STRING_Infinite,//{"Infinite","极限值"},
	_STRING_None__0_5_21_0_MHz,//{"None  0.5-21.0 MHz","无  0.5-21.0 MHz"},
	_STRING_File,//{"File","文件"},
	_STRING_Uncorrected_C_Scan,//{"Uncorrected C-Scan","未校正的C-扫描"},
	_STRING_Network,//{"Network","网络设置"},
	_STRING_Complete___Standard_,//{"Complete  (Standard)","完整  (标准)"},
	_STRING_Complete,//{"Complete","完整"},
	_STRING_Appearance,//{"Appearance","外观"},
	_STRING_Image,//{"Image","图像"},
	_STRING_Synchro_,//{"Synchro.","同步"},
	_STRING_Fill_zone_angle,//{"Fill zone angle","填充区角度"},
	_STRING_Fill_zone_radius,//{"Fill zone radius","填充区弧度"},
	_STRING_Fill_zone_height,//{"Fill zone height","填充区高度"},
	_STRING_Fill_2_Colors,//{"Fill 2 Colors","填充 双色"},
	_STRING_Fill,//{"Fill","填充"},
	_STRING_Characterize,//{"Characterize","特性"},
	_STRING_Probe_Part,//{"Probe/Part","探头/工件"},
	_STRING_Probe_,//{"Probe","探头"},
	_STRING_All,//{"All","所有组"},
	_STRING_Zoom,//{"Zoom","缩放"},
	_STRING_Velocity,//{"Velocity","速度"},
	_STRING_Horizontal,//{"Horizontal","水平"},
	_STRING_Double_V,//{"Double V","双V型"},
	_STRING_Mat_Attenuatior,//{"Mat.Attenuatior","衰减系数"},
	_STRING_Reset_Data,//{"Reset Data","数据清零"},
	_STRING_Source_,//{"Source","数据读取"},
	_STRING_Enter,//{"Enter","输入"},
	_STRING_Export,//{"Export","输出报表"},
	_STRING_Export_Table,//{"Export Table","输出报表"},
	_STRING_Output_3,//{"Output 3","输出 3"},
	_STRING_Output_2,//{"Output 2","输出 2"},
	_STRING_Output_1,//{"Output 1","输出 1"},
	_STRING_1_Manual_Weld_1__A___DA___PA___SA_,//{"1 Manual Weld 1: A%. DA^. PA^. SA^","手动焊缝 1: A%. DA^. PA^. SA^"},
	_STRING_Indoor,//{"Indoor","室内 "},
	_STRING_Finish_Group_Setting,//{"Finish Group Setting","使组参数生效"},
	_STRING_Clock_Dir_Down,//{"Clock/Dir Down","时钟方向 向下"},
	_STRING_Clock_Dir_Up,//{"Clock/Dir Up","时钟方向 向上"},
	_STRING_Clock_Dir,//{"Clock/Dir","时钟方向"},
	_STRING_Clock_Set__n_HH_MM_SS_,//{"Clock Set \n(HH:MM:SS)","时间"},
	_STRING_Beam,//{"Beam","声束"},
	_STRING_Sound_Path,//{"Sound Path","声程"},
	_STRING_Finish_Focallaw_Setting,//{"Finish Focallaw Setting","生成聚焦法则"},
	_STRING_GenerateNULL,//{"GenerateNULL","生成"},
	_STRING_Update,//{"Update","升级"},
	_STRING_Depth_2,//{"Depth 2","深度2"},
	_STRING_Depth_1,//{"Depth 1","深度1"},
	_STRING_Depth_A,//{"Depth A","深度 A"},
	_STRING_RF,//{"RF","射频"},
	_STRING_Save_Setup_As,//{"Save Setup As","设置另存为"},
	_STRING_Preferences,//{"Preferences","设置"},
	_STRING_Set_80_,//{"Set 80%","设为80%"},
	_STRING_Set_Ref_,//{"Set Ref.","设定参考"},
	_STRING_Scan,//{"Scan","扫描模式"},
	_STRING_Max_Scan_Speed,//{"Max.Scan Speed","扫查轴最大速度"},
	_STRING_Scan_Speed,//{"Scan Speed","扫查轴速度"},
	_STRING_Scan_,//{"Scan","扫查轴数据源"},
	_STRING_Synchro__,//{"Synchro","扫查轴数据源"},
	_STRING_Inspection,//{"Inspection","扫查设置"},
	_STRING_Area,//{"Area","扫查区域"},
	_STRING_Type,//{"Type","扫查方式"},
	_STRING_Scan__,//{"Scan","扫查"},
	_STRING_Date_Set,//{"Date Set","日期"},
	_STRING_Indication_Table,//{"Indication Table","缺陷记录表"},
	_STRING_Reset_All_,//{"Reset All","全部重置"},
	_STRING_All_Laws,//{"All Laws","全部法则"},
	_STRING_All_C_scans,//{"All C scans","全部C扫描"},
	_STRING_All_A___C_scans,//{"All A & C scans","全部 A & C 扫描"},
	_STRING_FW,//{"FW","全波"},
	_STRING_Curve_Step,//{"Curve Step","曲线增益差"},
	_STRING_Curve,//{"Curve","曲线"},
	_STRING_you_should_calibration_velocity_first____Continue__,//{"you should calibration velocity first \n\n\n  Continue ?","请先进行声速校准 \n\n\n 继续  ?"},
	_STRING_Clear_Calib,//{"Clear Calib","清除校准"},
	_STRING_Clear_All,//{"Clear All","清除全部"},
	_STRING_Clear_Env_,//{"Clear Env.","清除包络"},
	_STRING_Edge___,//{"Edge(/)","前沿(/)"},
	_STRING_Ref_Point,//{"Ref Point","前沿"},
	_STRING_Min_Angle_,//{"Min.Angle","起始角度"},
	_STRING_Start_,//{"Start","起始点"},
	_STRING_Start__,//{"Start","起始"},
	_STRING_Enable,//{"Enable","启用"},
	_STRING_Screen,//{"Screen","屏幕"},
	_STRING_Averaging,//{"Averaging","平均"},
	_STRING_Freq__MHz_,//{"Freq.(MHz)","频率(MHz)"},
	_STRING_Pref_,//{"Pref.","配置"},
	_STRING_Configuration_,//{"Configuration ","配置"},
	_STRING_Content,//{"Content","内容"},
	_STRING_Internal_Memory___,//{"Internal Memory \\","内存  \\"},
	_STRING_Internal_Memory,//{"Internal Memory","内存"},
	_STRING_Mode_,//{"Mode","模式"},
	_STRING_Template,//{"Template","模板"},
	_STRING_PW_ns_,//{"PW(ns)","脉宽(ns)"},
	_STRING_PRF_,//{"PRF","脉冲重复频率"},
	_STRING_Pulse,//{"Pulse","脉冲"},
	_STRING_Helicoidal_Scan,//{"Helicoidal Scan","螺旋扫查"},
	_STRING_Green,//{"Green","绿色"},
	_STRING_Filter,//{"Filter","滤波"},
	_STRING_Sensitivity,//{"Sensitivity","灵敏度"},
	_STRING_List,//{"List","列表"},
	_STRING_Bright,//{"Bright","亮度"},
	_STRING_Brightness,//{"Brightness","亮度"},
	_STRING_Type_,//{"Type","类型"},
	_STRING_Blue,//{"Blue","蓝色"},
	_STRING_Exclusion,//{"Exclusion","扩展"},
	_STRING_Aperture,//{"Aperture","孔径"},
	_STRING_Customer,//{"Customer","客户"},
	_STRING_Start_Mode,//{"Start Mode","开始模式"},
	_STRING_Start_Stop,//{"Start/Stop","开始/停止"},
	_STRING_Type__,//{"Type.","聚焦类型"},
	_STRING_Law_Type_,//{"Law Type","聚焦法则类型"},
	_STRING_Focal_Law,//{"Focal Law","聚焦法则"},
	_STRING_Distance,//{"Distance","距离"},
	_STRING_Static_sector,//{"Static sector","静态扫查"},
	_STRING_Field_4,//{"Field 4","结果4"},
	_STRING_Field_3,//{"Field 3","结果3"},
	_STRING_Field_2,//{"Field 2","结果2"},
	_STRING_Field_1,//{"Field 1","结果1"},
	_STRING_Accept,//{"Accept","接受"},
	_STRING_Receiver,//{"Receiver","接收起始阵元"},
	_STRING_Angle___,//{"Angle(°)","角度(°)"},
	_STRING_Angle,//{"Angle","角度"},
	_STRING_Beam_Angle_,//{"Beam Angle","角度"},
	_STRING_Focal_Point,//{"Focal Point","焦点"},
	_STRING_keyboard,//{"keyboard","键盘"},
	_STRING_Inspec_Data,//{"Inspec.Data","检测数据"},
	_STRING_Inspection_Data,//{"Inspection Data","检测数据"},
	_STRING_Group_Mode_,//{"Group Mode","检测模式"},
	_STRING_Reading,//{"Reading","检测结果"},
	_STRING_Rectifier,//{"Rectifier","检波"},
	_STRING_Load_Law_File,//{"Load Law File","加载法则文件"},
	_STRING_Load,//{"Load","加载"},
	_STRING_Technician04,//{"Technician04","技术员"},
	_STRING_Cal_Focallaw,//{"Cal Focallaw","计算聚焦法则"},
	_STRING_Laws,//{"Laws","计算"},
	_STRING_Echo_Type,//{"Echo Type","回波类型"},
	_STRING_Echo_Qty_,//{"Echo Qty.","回波次数"},
	_STRING_Yellow,//{"Yellow","黄色"},
	_STRING_Outdoor,//{"Outdoor","户外"},
	_STRING_Thickness,//{"Thickness","厚度设置"},
	_STRING_Thickness_A,//{"Thickness A","厚度 A"},
	_STRING_Thickness_2,//{"Thickness 2","厚度 2"},
	_STRING_Thickness_1,//{"Thickness 1","厚度 1"},
	_STRING_Red,//{"Red","红色"},
	_STRING_Black,//{"Black","黑色"},
	_STRING_Millimeters,//{"Millimeters","毫米"},
	_STRING_Weld_land_offset,//{"Weld land offset","焊接区偏移"},
	_STRING_Weld_land_height,//{"Weld land height","焊接区高度"},
	_STRING_Weld,//{"Weld","焊缝图示"},
	_STRING_Weld_Type,//{"Weld Type","焊缝类型"},
	_STRING_Weld_,//{"Weld","焊缝"},
	_STRING_Cursor,//{"Cursor","光标"},
	_STRING_Cursors,//{"Cursors","光标"},
	_STRING_Tolerance,//{"Tolerance","公差"},
	_STRING_Part_Number,//{"Part Number","工件编号"},
	_STRING_Parts,//{"Parts","工件"},
	_STRING_Modify,//{"Modify","更改"},
	_STRING_CADMIUM,//{"CADMIUM","镉"},
	_STRING_Format,//{"Format","格式"},
	_STRING_Advanced,//{"Advanced","高级"},
	_STRING_HW_,//{"HW-","负半波"},
	_STRING_Overlay,//{"Overlay","辅助线"},
	_STRING_Amplitude,//{"Amplitude","幅度"},
	_STRING_Service,//{"Service","服务"},
	_STRING_Buzzer,//{"Buzzer","蜂鸣器"},
	_STRING_Peak___,//{"Peak(^)","峰值(^)"},
	_STRING_Highest___,//{"Highest(%)","峰值(%)"},
	_STRING_Peak,//{"Peak","峰值"},
	_STRING_Resolution_,//{"Resolution","分辨率"},
	_STRING_Orientation,//{"Orientation","方向"},
	_STRING_Range,//{"Range","范围"},
	_STRING_Inverse,//{"Inverse","反转"},
	_STRING_Threshold_,//{"Threshold ","阀值"},
	_STRING_Pulser,//{"Pulser","发射起始阵元"},
	_STRING_Voltage_V_,//{"Voltage(V)","发射电压(V)"},
	_STRING_Symmetry,//{"Symmetry","对称性"},
	_STRING_Symmetric,//{"Symmetric","对称"},
	_STRING_Contrast,//{"Contrast","对比度"},
	_STRING_Get_Position2,//{"Get Position2","读取位置2"},
	_STRING_Get_Position1,//{"Get Position1","读取位置1"},
	_STRING_Get_Position,//{"Get Position","读取位置"},
	_STRING_Point,//{"Point","点"},
	_STRING_First_Element_,//{"First Element","第一个阵元"},
	_STRING_First_Angle,//{"First Angle","第一个角度"},
	_STRING_First_VPA,//{"First VPA","第一个波束"},
	_STRING_Site,//{"Site","地点"},
	_STRING_STEEL_MILD,//{"STEEL.MILD","低碳钢"},
	_STRING_Current_group_setting_will_lose__Continue_,//{"Current group setting will lose! \n\n\n  Continue ?","当前组配置信息将丢失! \n\n\n  继续 ?"},
	_STRING_Current,//{"Current","当前组"},
	_STRING_Current_Layout,//{"Current Layout","当前布局"},
	_STRING_One_Line_Scan,//{"One-Line Scan","单线扫查"},
	_STRING_Units,//{"Units","单位"},
	_STRING_Storage_Card____,//{"Storage Card   \\","存储卡   \\"},
	_STRING_Storage_Card,//{"Storage Card","存储卡"},
	_STRING_Storage,//{"Storage","存储"},
	_STRING_Vertical,//{"Vertical","垂直"},
	_STRING_UT_Unit,//{"UT Unit","超声轴单位"},
	_STRING_UT_Settings,//{"UT Settings","超声检测设置"},
	_STRING_Ultrasound,//{"Ultrasound","超声"},
	_STRING_Normal,//{"Normal","常规"},
	_STRING_General,//{"General","常规"},
	_STRING_View,//{"View","查看"},
	_STRING_Interpolation,//{"Interpolation","插值"},
	_STRING_Group_,//{"Group","测量结果"},
	_STRING_Measure_,//{"Measure","测量方式"},
	_STRING_Measure,//{"Measure","测量"},
	_STRING_Measurements,//{"Measurements","测量"},
	_STRING_Mode__,//{"Mode","操作模式"},
	_STRING_Parameters,//{"Parameters","参数"},
	_STRING_dB_Ref_,//{"dB Ref.","参考增益"},
	_STRING_Gain_r,//{"Gain-r","参考增益"},
	_STRING_Reference_Gain,//{"Reference Gain","参考增益"},
	_STRING_Overlay_,//{"Overlay","参考线"},
	_STRING_Ref_Ampl_Offset,//{"Ref.Ampl.Offset","参考幅度补偿"},
	_STRING_Ref_Amplitude,//{"Ref.Amplitude","参考幅度"},
	_STRING_Origin,//{"Origin","参考点"},
	_STRING_Reference,//{"Reference","参考"},
	_STRING_Points_Qty_,//{"Points Qty.","采样点"},
	_STRING_Max_Index_Speed,//{"Max.Index Speed","步进轴最大速度"},
	_STRING_Index_Speed,//{"Index Speed","步进轴速度"},
	_STRING_Index,//{"Index","步进轴数据源"},
	_STRING_Index_End,//{"Index End","步进终点"},
	_STRING_Angle_Step_,//{"Angle Step","步进"},
	_STRING_Index_,//{"Index","步进"},
	_STRING_Wave_Type_,//{"Wave Type","波形类型"},
	_STRING_Overlay__,//{"Overlay ","波形对比"},
	_STRING_Code,//{"Code","标准"},
	_STRING_Origin_,//{"Origin","编码器起点"},
	_STRING_Encoder_2,//{"Encoder 2","编码器 2"},
	_STRING_Encoder_1,//{"Encoder 1","编码器 1"},
	_STRING_Edit_Notes,//{"Edit Notes","编辑注释"},
	_STRING_Edit_Header,//{"Edit Header","编辑页眉"},
	_STRING_Edit,//{"Edit","编辑"},
	_STRING_Ratio_1_1,//{"Ratio 1:1","比率 1:1"},
	_STRING_Scale__Scale_factor_,//{"Scale  Scale factor ","比例  比例系数"},
	_STRING_Commnets,//{"Commnets","备注"},
	_STRING_Alarm__,//{"Alarm #","报警组"},
	_STRING_Condition,//{"Condition","报警条件"},
	_STRING_Output_,//{"Output","报警输出"},
	_STRING_Alarm_,//{"Alarm","报警设置"},
	_STRING_Build,//{"Build","报告预览"},
	_STRING_Table_,//{"Table","报表"},
	_STRING_Save_Data,//{"Save Data","保存数据"},
	_STRING_Save_Law_File,//{"Save Law File","保存法则文件"},
	_STRING_Envelope,//{"Envelope","包络"},
	_STRING_Radius_2,//{"Radius 2","半径2"},
	_STRING_Radius_1,//{"Radius 1","半径1"},
	_STRING_Radius_A,//{"Radius A","半径 A"},
	_STRING_Radius,//{"Radius","半径"},
	_STRING_White,//{"White","白色"},
	_STRING_wave_type_dict,//{"wave_type_dict","wave_type_dict"},
	_STRING_V,//{"V","V型"},
	_STRING_VsB_1,//{"VsB/  Volumetric position in gate B on the scan axis","VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位"},
	_STRING_VsB_2,//{"VsB^  Volumetric position in gate B on the scan axis","VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位"},
	_STRING_VsA_1,//{"VsA^  Volumetric position in gate A on the scan axis","VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位"},
	_STRING_VsA_2,//{"VsA/  Volumetric position in gate A on the scan axis","VsA/  闸门A内检测到的缺陷相对于扫查轴的体积定位"},
	_STRING_ViB_1,//{"ViB/  Volumetric position in gate B on the index axis","ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位"},
	_STRING_ViB_2,//{"ViB^  Volumetric position in gate B on the index axis","ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位"},
	_STRING_ViA_1,//{"ViA^  Volumetric position in gate A on the index axis","ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位 "},
	_STRING_ViA_2,//{"ViA/  Volumetric position in gate A on the index axis ","ViA/  闸门A内检测到的缺陷相对于步进轴的体积定位"},
	_STRING_U,//{"U","U型"},
	_STRING_USB_Storage____,//{"USB Storage   \\","U盘  \\"},
	_STRING_USB_Storage,//{"USB Storage","U盘"},
	_STRING_Unknown,//{"Unknown","Unknown"},
	_STRING_U_r___Ultrasonic_position_of_the_reference_cursor,//{"U(r)  Ultrasonic position of the reference cursor","U(r)  参考光标在超声轴上的位置"},
	_STRING_U_r_,//{"U(r)","U(r)"},
	_STRING_U_m_r___Ultrasonic_axis__measurement_cursor___reference_cursor_,//{"U(m-r)  Ultrasonic axis (measurement cursor - reference cursor)","U(m-r)  超声轴上测量光标位置与参考光标位置之差"},
	_STRING_U_m___Ultrasonic_position_of_the_measurement_cursor,//{"U(m)  Ultrasonic position of the measurement cursor","U(m)  测量光标在超声轴上的位置"},
	_STRING_U_m_,//{"U(m)","U(m)"},
	_STRING_tx_rx_mode_dict,//{"tx_rx_mode_dict","tx_rx_mode_dict"},
	_STRING_title,//{"title","title"},
	_STRING_T_A_1,//{"T(A^)  Thickness","T(A^)  厚度"},
	_STRING_T_A_2,//{"T(A/)  Thickness","T(A/)  厚度"},
	_STRING_S_Scan,//{"S-Scan","S-扫描"},
	_STRING_switch_dict,//{"switch_dict","switch_dict"},
	_STRING_Strip_Chart__A___Strip_Chart_A_Scan_,//{"Strip Chart-[A]  Strip Chart[A-Scan]","Strip Chart-[A]  Strip Chart[A-Scan]"},
	_STRING_Strip_Chart__A_,//{"Strip Chart-[A]","Strip Chart-[A]"},
	_STRING_SB_1,//{"SB/  Sound path of the indication in gate B","SB^  声束出射点到闸门B内检测到的缺陷之间的声程"},
	_STRING_SB_2,//{"SB^  Sound path of the indication in gate B","SB^  声束出射点到闸门B内检测到的缺陷之间的声程"},
	_STRING_SA_1,//{"SA^  Sound path of the indication in gate A","SA^  声束出射点到闸门A内检测到的缺陷之间的声程"},
	_STRING_SA_2,//{"SA/  Sound path of the indication in gate A","SA/  声束出射点到闸门A内检测到的缺陷之间的声程"},
	_STRING_S_r_1,//{"S(r)  Scan position of the reference cursor","S(r)  扫查轴上参考光标的位置"},
	_STRING_S_r_,//{"S(r)","S(r)"},
	_STRING_S_r_m_,//{"S(r&m)","S(r&m)"},
	_STRING_S_m_r__Scan_axis__measurement_cursor___reference_cursor_,//{"S(m-r) Scan axis (measurement cursor - reference cursor)","S(m-r) 扫查轴上测量光标与参考光标的距离"},
	_STRING_S_m___Scan_position_of_the_measurement_cursor_,//{"S(m)  Scan position of the measurement cursor ","S(m)  扫查轴上测量光标的位置"},
	_STRING_S_m_,//{"S(m)","S(m)"},
	_STRING_S__S_Scan,//{"S  S-Scan","S  S-扫描"},
	_STRING_Remote_Pc,//{"Remote Pc","Remote Pc"},
	_STRING_RB_1,//{"RB/  Reference point to the indication in gate B","RB^  声束出射点与闸门B内检测到的缺陷之间的距离"},
	_STRING_RB_2,//{"RB^  Reference point to the indication in gate B","RB^  声束出射点与闸门B内检测到的缺陷之间的距离"},
	_STRING_RA_1,//{"RA^  Reference point to the indication in gate A","RA^  声束出射点与闸门A内检测到的缺陷之间的距离"},
	_STRING_RA_2,//{"RA/  Reference point to the indication in gate A","RA/  声束出射点与闸门A内检测到的缺陷之间的距离"},
	_STRING_probe4,//{"probe4","probe4"},
	_STRING_probe3,//{"probe3","probe3"},
	_STRING_probe2,//{"probe2","probe2"},
	_STRING_probe1,//{"probe1","probe1"},
	_STRING_Pretrig_,//{"Pretrig.","Pretrig."},
	_STRING_PB_1,//{"PB^  Probe front face to the indication in gate B","PB^  探头前表面与闸门B内检测到的缺陷的距离"},
	_STRING_PB_2,//{"PB/  Probe front face to the indication in gate B","PB^  探头前表面与闸门B内检测到的缺陷的距离"},
	_STRING_PA_TOFD__PA_TOFD,//{"PA-TOFD  PA-TOFD","PA-TOFD  PA-TOFD"},
	_STRING_PA_TOFD,//{"PA-TOFD","PA-TOFD"},
	_STRING_part,//{"part","part"},
	_STRING_PA_1,//{"PA^  Probe front face to the indication in gate A","PA^  探头前表面与闸门A内检测到的缺陷的距离"},
	_STRING_PA_2,//{"PA/  Probe front face to the indication in gate A","PA/  探头前表面与闸门A内检测到的缺陷的距离"},
	_STRING_P_r___Probe_position_of_the_reference_cursor,//{"P(r)  Probe position of the reference cursor","P(r)  相对于参考光标的探头位置"},
	_STRING_P_m_r___Probe__measurement_cursor___reference_cursor_,//{"P(m-r)  Probe (measurement cursor - reference cursor)","P(m-r)  探头相对于测量光标与参考光标差值的位置"},
	_STRING_P_m___Probe_position_of_the_measurement_cursor,//{"P(m)  Probe position of the measurement cursor","P(m)  相对于测量光标的探头位置"},
	_STRING_output_dict,//{"output_dict","output_dict"},
	_STRING_Output_3___On_,//{"Output 3  [On]","Output 3  [开]"},
	_STRING_Output_2___On_,//{"Output 2  [On]","Output 2  [开]"},
	_STRING_Output_1___On_,//{"Output 1  [On]","Output 1  [开]"},
	_STRING_Operation,//{"Operation","Operation选择？？"},
	_STRING_Max_2,//{"Max/2","最大/2"},
	_STRING_LB_1,//{"LB/  Leg (skip) of the indication in gate B","LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数"},
	_STRING_LB_2,//{"LB^  Leg (skip) of the indication in gate B","LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数"},
	_STRING_LA_1,//{"LA^  Leg (skip) of the indication in gate A","LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数"},
	_STRING_LA_2,//{"LA/  Leg (skip) of the indication in gate A","LA/  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数"},
	_STRING_J,//{"J","J型"},
	_STRING_I,//{"I","I型"},
	_STRING_IP_Address,//{"IP Address","IP地址"},
	_STRING_I_1,//{"I/  Position in gate I","I/ 闸门I的前沿位置"},
	_STRING_I_2,//{"I/  Position in gate I","I/  穿过闸门I时信号的位置"},
	_STRING_I_,//{"I/","I/"},
	_STRING_I_w____Position_in_gate_I_in_water,//{"I(w)/  Position in gate I in water","I(w)/  Position in gate I in water"},
	_STRING_I_r___Index_position_of_the_reference_cursor,//{"I(r)  Index position of the reference cursor","I(r)  步进轴上参考光标的位置"},
	_STRING_I_r_,//{"I(r)","I(r)"},
	_STRING_I_m_r___Index_axis__measurement_cursor___reference_cursor_,//{"I(m-r)  Index axis (measurement cursor - reference cursor)","I(m-r)  步进轴上测量光标与参考光标的距离"},
	_STRING_I_m___Index_position_of_the_measurement_cursor,//{"I(m)  Index position of the measurement cursor","I(m)  步进轴上测量光标的位置"},
	_STRING_I_m_,//{"I(m)","I(m)"},
	_STRING_E_6dB__Envelop_signal_at__6_dB_in_gate_A,//{"E-6dB  Envelop signal at -6 dB in gate A","E-6dB  闸门A内信号在-6dB处包络线的宽度"},
	_STRING_E_3dB__Envelop_signal_at__3_dB_in_gate_A,//{"E-3dB  Envelop signal at -3 dB in gate A","E-3dB  闸门A内信号在-3 dB处包络线的宽度"},
	_STRING_E_20dB__Envelop_signal_at__20_dB_in_gate_A_,//{"E-20dB  Envelop signal at -20 dB in gate A ","E-20dB  闸门A内信号在-20dB处包络线的宽度"},
	_STRING_E_12dB__Envelop_signal_at__12_dB_in_gate_A_,//{"E-12dB  Envelop signal at -12 dB in gate A ","E-12dB  闸门A内信号在-12dB处包络线的宽度"},
	_STRING_E___Position_of_envelop_in_gate_A,//{"E^  Position of envelop in gate A","E^  闸门A内包络线的峰值位置"},
	_STRING_E___Peak_amplitude_of_the_envelope_in_gate_A,//{"E%  Peak amplitude of the envelope in gate A","E%  闸门A中包络线的峰值幅度"},
	_STRING_Doppler_Serial,//{"Doppler Serial","Doppler Serial"},
	_STRING_DIN4,//{"DIN4","DIN4"},
	_STRING_DIN3,//{"DIN3","DIN3"},
	_STRING_DIN2,//{"DIN2","DIN2"},
	_STRING_DIN1,//{"DIN1","DIN1"},
	_STRING_DB_1,//{"DB/  Depth of the indication in gate B","DB^  闸门B内检测到的缺陷在工件中的深度 "},
	_STRING_DB_2,//{"DB^  Depth of the indication in gate B","DB^  闸门B内检测到的缺陷在工件中的深度 "},
	_STRING_Data_2,//{"Data 2","Data 2"},
	_STRING_Data_1,//{"Data 1","Data 1"},
	_STRING_Sizing_Curves,//{"Sizing Curves","DAC/TCG"},
	_STRING_DAC,//{"DAC","DAC"},
	_STRING_DA_1,//{"DA^  Depth of the indication in gate A","DA^  闸门A内检测到的缺陷在工件中的深度"},
	_STRING_DA_2,//{"DA/  Depth of the indication in gate A","DA/  闸门A内检测到的缺陷在工件中的深度"},
	_STRING_C_scan,//{"C-scan","C-扫描"},
	_STRING_Current___A_Scan,//{"Current & A-Scan","Current & A-Scan"},
	_STRING_C_Scan_2,//{"C-Scan 2","C-Scan 2"},
	_STRING_C_Scan_1,//{"C-Scan 1","C-Scan 1"},
	_STRING_calculator2,//{"calculator2","calculator2"},
	_STRING_calculator1,//{"calculator1","calculator1"},
	_STRING_Cailbration_Due,//{"Cailbration Due","Cailbration Due"},
	_STRING_C__C_Scan,//{"C  C-Scan","C  C-扫描"},
	_STRING_B_scan,//{"B-scan","B-扫描"},
	_STRING_BdBr__Diff__between_signal_and_reference_in_gate_B,//{"BdBr  Diff. between signal and reference in gate B","BdBr  闸门B内峰值幅度与参考信号幅度之差（dB）"},
	_STRING_BdBB_1,//{"BdBB Signal in gate minus gate level for gate B","BdBB闸门B内峰值幅度与闸门阈值幅度之差（dB）"},
	_STRING_BdBB_2,//{"BdBB Signal in gate minus gate level for gate B","BdBB 闸门A内信号幅度与阈值之差（dB）"},
	_STRING_B_1,//{"B^  Position in gate B","B^ 闸门B内信号峰值位置"},
	_STRING_B_2,//{"B/  Position in gate B","B^ 闸门B内峰值位置"},
	_STRING_B_1_,//{"B%  Peak amplitude in gate B","B%  闸门B内信号峰值幅度"},
	_STRING_B_2_,//{"B%  Peak amplitude in gate B","B%  闸门B内峰值幅度"},
	_STRING_B__B_Scan,//{"B  B-Scan","B  B-扫描"},
	_STRING_A_scan,//{"A-scan","A-扫描"},
	_STRING_AWS_D1_5,//{"AWS-D1.5","AWS-D1.5"},
	_STRING_AWS_D_D__Indication_rating_for_AWS_D1_5_D,//{"AWS-D D  Indication rating for AWS-D1.5 D","AWS-D D  根据AWS-D1.5 D标准的指征率"},
	_STRING_AWS_D_CL__Discontinuity_Severity_Class_for_AWS_D1_5,//{"AWS-D CL  Discontinuity Severity Class for AWS-D1.5","AWS-D CL  根据AWS-D1.5标准的不连续性严重等级"},
	_STRING_AWS_D_C__Attenuation_factor_for_AWS_D1_5_C,//{"AWS-D C  Attenuation factor for AWS-D1.5 C","AWS-D C  根据AWS-D1.5C标准的衰减系数"},
	_STRING_AWS_D_B__Zero_reference_level_for_AWS_D1_5_B,//{"AWS-D B  Zero reference level for AWS-D1.5 B","AWS-D B  根据AWS-D1.5 B标准的零点参考电平"},
	_STRING_AWS_D_A__Indication_level_for_AWS_D1_5_A,//{"AWS-D A  Indication level for AWS-D1.5 A","AWS-D A  根据AWS-D1.5 A标准的指示电平"},
	_STRING_AWS_D_70__Discontinuity_Severity_Class_for_AWS_D1_5_70,//{"AWS-D 70  Discontinuity Severity Class for AWS-D1.5 70","AWS-D 70  根据AWS-D1.5 70标准的不连续性严重等级"},
	_STRING_AWS_D_60__Discontinuity_Severity_Class_for_AWS_D1_5_60,//{"AWS-D 60  Discontinuity Severity Class for AWS-D1.5 60","AWS-D 60  根据AWS-D1.5 60标准的不连续性严重等级"},
	_STRING_AWS_D_45__Discontinuity_Severity_Class_for_AWS_D1_5_45,//{"AWS-D 45  Discontinuity Severity Class for AWS-D1.5 45","AWS-D 45  根据AWS-D1.5 45标准的不连续性严重等级"},
	_STRING_A_S__C___A_Scan_S_Scan__C_Scan_,//{"A-S-[C]  A-Scan S-Scan [C-Scan]","A-S-[C]  A扫描-S扫描- C扫描[C-扫描]"},
	_STRING_A_S__C_,//{"A-S-[C]","A-S-[C]"},
	_STRING_API_HW__Half_wave_width_according_to_API_code,//{"API-HW  Half-wave width according to API code","API-HW  根据API 标准规定的半波宽度 "},
	_STRING_API_DL__Defect_length_extension_according_to_API_code,//{"API-DL  Defect length extension according to API code","API-DL根据API标准规定的缺陷长度 "},
	_STRING_API_DL,//{"API-DL","API-DL "},
	_STRING_All___A_Scan,//{"All & A-Scan","All & A-Scan"},
	_STRING_AdBr__Diff__between_signal_and_reference_in_gate_A,//{"AdBr  Diff. between signal and reference in gate A","AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）"},
	_STRING_AdBA_1,//{"AdBA  Signal in gate minus gate level for gate A","AdBA  闸门A内信号幅度与阈值之差（dB）"},
	_STRING_AdBA_2,//{"AdBA  Signal in gate minus gate level for gate A","AdBA  闸门A内峰值幅度与闸门阈值幅度之差（dB）"},
	_STRING_A_C__C__A_Scan_C_Scan__C_Scan_,//{"A-C-[C] A-Scan C-Scan [C-Scan]","A-C-[C] A-扫描C-扫描[C-扫描]"},
	_STRING_A_C__C_,//{"A-C-[C]","A-C-[C]"},
	_STRING_A_B_S__A_Scan_B_Scan_S_Scan,//{"A-B-S  A-Scan B-Scan S-Scan","A-B-S  A-扫描B-扫描S-扫描"},
	_STRING_A_B_S,//{"A-B-S","A-B-S"},
	_STRING_A_B_C__A_Scan_B_Scan_C_Scan,//{"A-B-C  A-Scan B-Scan C-Scan","A-B-C  A-扫描B-扫描C-扫描"},
	_STRING_A_B_C,//{"A-B-C","A-B-C"},
	_STRING_A_B__A_Scan_B_Scan,//{"A-B  A-Scan B-Scan","A-B  A -扫描B-扫描"},
	_STRING_A_B,//{"A-B","A-B"},
	_STRING_A__I_1,//{"A^-I^","A^-I^"},
	_STRING_A__I_2,//{"A^-I/","A^-I/"},
	_STRING_A_1,//{"A^  Position in gate A","A^ 闸门A内信号峰值位置"},
	_STRING_A_11,//{"A^","A^"},
	_STRING_A_2,//{"A/  Position in gate A","A/ 闸门A内峰值位置"},
	_STRING_A_22,//{"A/","A/"},
	_STRING_A_3,//{"A%  Peak amplitude in gate A","A% 闸门A内信号峰值幅度"},
	_STRING_A__A_Scan,//{"A  A-Scan","A  A-扫描"},
	_STRING_9__On_,//{"9 [On]","9 [开]"},
	_STRING_8__On_,//{"8 [On]","8 [开]"},
	_STRING_7__On_,//{"7 [On]","7 [开]"},
	_STRING_6__On_,//{"6 [On]","6 [开]"},
	_STRING_50_Low,//{"50 Low","50 低"},
	_STRING_5__On_,//{"5 [On]","5 [开]"},
	_STRING_4__On_,//{"4 [On]","4 [开]"},
	_STRING_3__On_,//{"3 [On]","3 [开]"},
	_STRING_200_High,//{"200 High","200 高"},
	_STRING_2_Auto__Weld_1__A___DA___ViA___VsA_,//{"2 Auto. Weld 1: A%. DA^. ViA^. VsA^","2 自动焊綘 1: A%. DA^. ViA^. VsA^"},
	_STRING_2_Auto__Weld_1,//{"2 Auto. Weld 1","2 自动焊綘 1"},
	_STRING_2__On_,//{"2 [On]","2 [开]"},
	_STRING_16__On_,//{"16 [On]","16 [开]"},
	_STRING_15__On_,//{"15 [On]","15 [开]"},
	_STRING_14__On_,//{"14 [On]","14 [开]"},
	_STRING_13__On_,//{"13 [On]","13 [开]"},
	_STRING_12__On_,//{"12 [On]","12 [开]"},
	_STRING_11__On_,//{"11 [On]","11 [开]"},
	_STRING_100_Middle,//{"100 Middle","100 中"},
	_STRING_100_High,//{"100 High","100 高"},
	_STRING_10__On_,//{"10 [On]","10 [开]"},
	_STRING_1_Manual_Weld_1,//{"1 Manual Weld 1","1 手动焊缝 1"},
	_STRING_1__On_,//{"1 [On]","1 [开]"},
	_STRING___Storage_Card_,//{"\\Storage Card\\","\\存储卡\\"},
	_STRING___Hard_Disk_,//{"\\Hard Disk\\","\\U盘\\"},
	_STRING___U_r____Signal_amplitude_at_reference_ultrasonic_cursor_position,//{"%(U(r))  Signal amplitude at reference ultrasonic cursor position","%(U(r))  参考光标在超声轴上对应的信号幅度"},
	_STRING___U_m____Signal_amplitude_at_measurement_ultrasonic_cursor_position,//{"%(U(m))  Signal amplitude at measurement ultrasonic cursor position","%(U(m))  测量光标在超声轴上对应的信号幅度"},
	_STRING___r___Amplitude_value_for_the_reference_cursor,//{"%(r)  Amplitude value for the reference cursor","%(r)  参考光标位置的幅度值"},
	_STRING___r_,//{"%(r)","%(r)"},
	_STRING___m_r___Amplitude__measurement_cursor___reference_cursor_,//{"%(m-r)  Amplitude (measurement cursor - reference cursor)","%(m-r) 测量光标的幅度值减去参考信号的幅度值)"},
	_STRING___m___Amplitude_value_of_the_measurement_cursor,//{"%(m)  Amplitude value of the measurement cursor","%(m)  测量光标位置的幅度值"},
	_STRING___m_,//{"%(m)","%(m)"},
	_STRING_Angle_Beam,//{"Angle Beam",""},
	_STRING_Avg_Scan_Speed,//{"Avg.Scan Speed",""},
	_STRING_Block_Name,//{"Block Name",""},
	_STRING_By_Context,//{"By Context",""},
	_STRING_By_Group_,//{"By Group ",""},
	_STRING_Center,//{"Center",""},
	_STRING_Clicker,//{"Clicker",""},
	_STRING_Compressive,//{"Compressive",""},
	_STRING_Connect,//{"Connect",""},
	_STRING_Context_Select,//{"Context Select",""},
	_STRING_Control_Area,//{"Control Area",""},
	_STRING_Custom,//{"Custom",""},
	_STRING_Data_Link,//{"Data Link",""},
	_STRING_Display_Range,//{"Display Range",""},
	_STRING_Double_V_Offset,//{"Double V Offset",""},
	_STRING_Immersion,//{"Immersion",""},
	_STRING_Last,//{"Last","最新"},
	_STRING_Legal,//{"Legal",""},
	_STRING_Letter,//{"Letter",""},
	_STRING_Mouse,//{"Mouse",""},
	_STRING_Name,//{"Name",""},
	_STRING_Strip_Chart,//{"Strip Chart","带状图"},
	_STRING_Tensile,//{"Tensile",""},
	_STRING_Top_Turn,//{"Top Turn",""},
	_STRING_V_Offset,//{"V Offset",""},
	_STRING_B__A_,//{"B^-A^",""},
	_STRING_B__I_1,//{"B^-I/",""},
	_STRING_B__I_2,//{"B^-I^",""},
	_STRING_max_laws_is_256_,//{"max laws is 256!",""},
	_STRING_Measurement,//{"Measurement",""},
	_STRING_Negative,//{"Negative",""},
	_STRING_Option_Key,//{"Option Key",""},
	_STRING_Positive,//{"Positive",""},
	_STRING_R_and_M,//{"R and M",""},
	_STRING_Rectification_TOFD,//{"Rectification-TOFD",""},
	_STRING_Remote_Desktop,//{"Remote Desktop",""},
	_STRING_S_scan,//{"S-scan",""},
#endif
	_STRING_MAX,//
}
	_STRING_ENUM;

typedef const _STRING_ENUM CONST_STRING_ENUM;
#endif /* LANGUAGE_D_H_ */
