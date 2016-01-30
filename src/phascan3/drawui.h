
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"
#include "main.h"
#include "base_config.h"
#include "language.h"

#define MEASURE_DATA_BASE  987654000
#define MEASURE_DATA_ND    987654321.123
#define MEASURE_DATA_NS    987654322

#define MEASURE_DATA_CHAR_BASE  987654323
#define MEASURE_DATA_A     		987654323
#define MEASURE_DATA_B     		987654324
#define MEASURE_DATA_C     		987654325
#define MEASURE_DATA_D     		987654326

#define MEASURE_DATA_NULL  987654327

/* 当前位置信息  */
#define MENU1_STOP    4
#define MENU2_STOP    0 
#define MENU2_PRESSED 1
#define MENU3_STOP    2
#define MENU3_PRESSED 3
/* 单位 */
#define UNIT_MM		0
#define UNIT_INCH	1
#define UNIT_US		2
#define UNIT_NULL	3
#define UNIT_M_S	4
#define UNIT_IN_US	5
#define UNIT_DB 	6
#define UNIT_TO 	7
#define UNIT_BFH 	8
#define UNIT_MS 	9
#define UNIT_MM_S 	10
#define UNIT_TO1 	11
#define UNIT_DB_MM      12
#define UNIT_S		13
#define UNIT_DEG	14
#define UNIT_STEP_MM	15
#define UNIT_RPM	16
#define UNIT_DB_INCH	17
#define UNIT_DB_US	18
#define UNIT_INCH_S	19
#define UNIT_VPA	20
#define UNIT_NONE	21
#define UNIT_MHZ	22

/* UT_UNIT */
#define UT_UNIT_SOUNDPATH	0
#define UT_UNIT_TIME		1
#define UT_UNIT_TRUE_DEPTH	2

#define AUTO_SET	0
/* 发射模式 P111 */
#define PITCH_CATCH		0
#define	PULSE_ECHO		1
#define	THROUGH_TRAN	2
#define TOFD			3
/* 电压 */
#define VOL_LOW		0
#define VOL_HIGH	1

/* 检波 P122 */
#define RF_WAVE			0
#define POSITIVE_WAVE	1
#define NAGATIVE_WAVE	2
#define FULL_WAVE		3
/* 闸门 */
#define GATE_A	0
#define GATE_B  1
#define GATE_I  2

#define GATE_POSITION 0
#define GATE_MODE     1

#define GATE_PULSER   0
#define GATE_II       1
#define GATE_AA	      2

/* focallaw beam 类型定义*/
#define	AZIMUTHAL_SCAN	0
#define LINEAR_SCAN	1
#define	DEPTH_SCAN	2
#define	STATIC_SCAN	3
/* focalpoint 聚焦点计算方法 */
#define HALFPATH_P		0
#define DEPTH_P			1
#define	PROJECTION_P	2
#define FOCALPLANE_P	3


/* 0纵波 1横波 */
#define LONGITUDINAL_WAVE	0
#define SHEAR_WAVE	1

#define GAIN_MAX	80.0
#define GAINR_OFF	0
#define GAINR_ON	1
#define AUTO_FOCAL_OFF	0
#define AUTO_FOCAL_ON	1
#define VIDEO_FILTER_OFF 0
#define VIDEO_FILTER_ON	 1

/*  */
#define UT_CHANNEL	3

/*上方数值显示信息定义 */
#define GAIN_LABEL	0
#define GAIN_VALUE	1

/* 最大测量范围  扫描延时+扫描范围 */
#define MAX_RANGE_US	9899.89

/* 各个menu 弹出菜单的偏移量 */
#define TX_RX_MODE	0
#define FREQUENCY	(TX_RX_MODE + 8)
#define PA_VOLTAGE	(FREQUENCY + 14)
#define UT_VOLTAGE  (PA_VOLTAGE + 4)
#define PULSER_WIDTH    (UT_VOLTAGE + 8)
#define PRF         (PULSER_WIDTH + 2)
#define FILTER		(PRF + 4)
#define RECTIFIER	(FILTER + 16)
#define AVERAGING	(RECTIFIER + 4)
#define OFF_ON		(AVERAGING + 5)
#define POINT_QTY	(OFF_ON + 2)
#define SUM_GAIN	(POINT_QTY + 5)
#define GATE_POS	(SUM_GAIN + 2)
#define PARAMETERS	(GATE_POS + 3)
#define SYNCHRO 	(PARAMETERS + 2)
#define SYNCHRO_ 	(SYNCHRO + 3)
#define MEASURE 	(SYNCHRO_ + 3)
#define RECTIFIER_FREQ 	(MEASURE + 2)
#define ALARM_POS 	(RECTIFIER_FREQ + 3)
#define ALARM_ON_POS 	(ALARM_POS + 16)
#define GROUPA	 	(ALARM_ON_POS + 16)
#define CONDITIONA 	(GROUPA + 8)
#define OPERAT 		(CONDITIONA + 9)
#define OUTPUT_POS	(OPERAT + 2)
#define ALARM1		(OUTPUT_POS+10)
#define DATA		(ALARM1+37)
#define SOUND		(DATA+4)
#define MODE_POS	(SOUND+10)
#define CURVE_POS	(MODE_POS + 2)
#define LIST		(CURVE_POS + 4 )
#define FIELD		(LIST + 2)
#define SELECTION	(FIELD + 61)
#define DATA_LINK	(SELECTION+9)
#define SOURCE		(DATA_LINK+4)
#define DISPLAY		(SOURCE + 9)
#define DISPL		(DISPLAY + 12)
#define GROUP_POS	(DISPL +12)
#define C_SCAN1		(GROUP_POS +2)
#define DATA2		(C_SCAN1 + 5)
#define DIS_MODE	(DATA2 + 4)
#define UTUNIT		(DIS_MODE + 4)
#define GRID		(UTUNIT + 3)
#define ZOOM_DISPLAY	(GRID + 6)
#define TYPE		(ZOOM_DISPLAY + 6)
#define COL_SELECT	(TYPE + 6 )
#define COL_MODE	(COL_SELECT + 3)
#define PROP_SCAN	(COL_MODE + 2)
#define PROP_COLOR	(PROP_SCAN + 6)
#define ENVELOPE	(PROP_COLOR + 6)
#define PROP_SOURCE	(ENVELOPE + 2)
#define PROP_APP	(PROP_SOURCE + 4)
#define PROP_OVERLAY	(PROP_APP + 4)
#define FFT_COLOR	(PROP_OVERLAY + 3)
#define ORIENTATION	(FFT_COLOR + 4)
#define GROUP_P		(ORIENTATION + 2)
#define GROUP_MODE	(GROUP_P + 10)
#define GROUP_MODE_P	(GROUP_MODE + 4)
#define PROB_SELECT	(GROUP_MODE_P + 4)
#define PROB_SKEW	(PROB_SELECT + 2 )
#define GEOMETRY	(PROB_SKEW + 5)
#define MATERIAL	(GEOMETRY + 3)
#define LAW_CONFIG	(MATERIAL + 24)
#define L_CONFIG	(LAW_CONFIG + 4)
#define FOCAL_POINT_TYPE	(L_CONFIG + 4)
#define FOCAL_POINT_TYPE1	(FOCAL_POINT_TYPE + 5)
#define WAVE_TYPE	(FOCAL_POINT_TYPE1 + 5)
#define W_TYPE		(WAVE_TYPE + 3)
#define ENCODER		(W_TYPE + 3)
#define POLARITY	(ENCODER + 2)
#define E_TYPE		(POLARITY + 2)
#define I_TYPE		(E_TYPE + 9)
#define I_SCAN		(I_TYPE + 3)
#define I_INDEX		(I_SCAN + 3)
#define START_MODE	(I_INDEX + 5)
#define STORAGE		(START_MODE + 3)
#define INSPEC_DATA	(STORAGE + 4)
#define F_STORAGE	(INSPEC_DATA + 2)
#define F_STORAGE_P	(F_STORAGE + 3)
#define SAVE_MODE	(F_STORAGE_P + 3)
#define TEMPLA		(SAVE_MODE + 4)
#define TEMPLA_P	(TEMPLA + 1)
#define PAPER_SIZE	(TEMPLA_P + 1)
#define VIEW		(PAPER_SIZE + 4)
#define F_SELECT	(VIEW + 3)
#define U_LABEL		(F_SELECT + 10)
#define U_CONTENT	(U_LABEL + 10)
#define P_UNITS		(U_CONTENT + 10)
#define SCHEME		(P_UNITS + 2 )
#define GATE_M		(SCHEME + 2)
#define SELECT_K	(GATE_M + 2)
#define ASSIGN_K	(SELECT_K + 9)
#define ASSIGN_K_P	(ASSIGN_K + 16)
#define MOUSE		(ASSIGN_K_P + 5)
#define OPERATION	(MOUSE + 3)
#define WGROUP		(OPERATION + 2)
#define CTYPE		(WGROUP + 8)
#define CMODE		(CTYPE + 3)
#define SCODE		(CMODE + 4)
#define ECHOTYPE	(SCODE + 2)
#define WELDTYPE	(ECHOTYPE + 3)
#define PROBETYPE	(WELDTYPE + 2)
#define DAMPING		(PROBETYPE + 4)
#define WELD		(DAMPING + 2)
#define SYMMETRY	(WELD + 6)
#define LANGUAGE	(SYMMETRY + 3)
#define FILETYPE	(LANGUAGE + 2)
#define LOADPART	(FILETYPE + 4)
//#define LAW_CONFIG	(WGROUP + 3)
//#define L_CONFIG	(LAW_CONFIG + 4)

/* 当前按键响应窗口类型 */
#define KEYPRESS_MAIN				0x00
#define KEYPRESS_MAIN_SPINBUTTON	0x01
#define KEYPRESS_MAIN_ENTRY			0x02
#define KEYPRESS_DIALOG				0x10
#define KEYPRESS_DIALOG_SPINBUTTON	0x11
#define KEYPRESS_DIALOG_ENTRY		0x12
#define KEYPRESS_WARNING			0x20

#define	KEYPRESS_MENUITEM			0x40

/* 按键是由那个window发出 */
#define MENU_POP		0
#define MENU_HIDE		1 /*disappear*/
#define MENU_CHANGE		2
#define MENU_UP			3
#define MENU_DOWN		4
#define MENU_ENTER		5

#define MENU3_TURN 	3
#define MENU3_ENTER	4
#define MENU3_UP	5
#define MENU3_DOWN	6
#define MENU3_HIDE	7
#define MENU3_SHOW	8

#define HELP_N		0
#define HELP_Y		1

#define	MAIN_MENU_PRESS	1
#define	MENU31_PRESS	2
#define	MENU32_PRESS	3
#define	MENU33_PRESS	4
#define	MENU34_PRESS	5
#define	MENU35_PRESS	6
#define	MENU36_PRESS	7

/* 发送给硬件数据类型 */
#define BEAM_DELAY_DSP	0x1000	/* 0x1000~0x10ff 最多256个聚焦法则 */
#define	RANGE_DSP		0x2000	/*  */	

#define ELEM_T_START 
#define ELEM_R_START 

#define GATEA_DSP		0x3000
#define GATEB_DSP		0x3001
#define GATEI_DSP		0x3002
#define TX_RX_MODE_DSP		0x3003
#define FREQUENCY_DSP		0x3004
#define	VOLTAGE_DSP		0x3005
#define PW_DSP			0x3006
#define PRF_DSP			0x3007
#define FILTER_DSP		0x3008
#define RECTIFIER_DSP		0x3009
#define AVERAGING_DSP		0x300a
#define VIDEO_FILTER_DSP	0x300b
#define POINT_QTY_DSP		0x300c
#define SUM_GAIN_DSP		0x300d
#define GATE_POS_DSP		0x300e
#define PARAMETERS_DSP		0x300f
#define START_DSP		0x3010
#define SYNCHRO_DSP		0x3011
#define WIDTH_DSP		0x3012
#define MEASURE_DSP		0x3013
#define HEIGHT_DSP		0x3014
#define RECTIFIER_FREQ_DSP	0x3015
#define ALARM_POS_DSP		0x3016
#define GROUPA_DSP		0x3017
#define CONDITIONA_DSP		0x3018
#define OPERAT_DSP		0x3019
#define GROUPB_DSP		0x301a
#define CONDITIONB_DSP		0x301b
#define OUTPUT_POS_DSP		0x301c
#define ALARM1_DSP		0x301d
#define COUNT_DSP		0x301e
#define DATA_DSP		0x301f
#define SOUND_DSP		0x3020
#define DELAY_DSP		0x3021
#define HOLDTIME_DSP		0x3022
#define MODE_POS_DSP		0x3023
#define CURVE_POS_DSP		0x3024
#define POINT_POS_DSP		0x3025

enum
{
REFRESH_START_MARK   =      1,
REFRESH_RANGE_MARK         ,
REFRESH_WEDGE_DELAY_MARK   ,
REFRESH_BEAM_DELAY_MARK    ,
REFRESH_VELOCITY_MARK      ,
REFRESH_FREQUENCY_MARK     ,
REFRESH_PULSE_WIDTH_MARK   ,
REFRESH_POINT_MARK         ,
REFRESH_FOCALLAW_TYPE_MARK ,
REFRESH_ANGLE_MARK     ,
REFRESH_ANGLE_MARK_MIN     ,
REFRESH_ANGLE_MARK_MAX     ,
REFRESH_ANGLE_MARK_STEP    ,
REFRESH_FOCAL_DEPTH_MARK   ,
REFRESH_ELEMENT_MARK       ,
REFRESH_ELEMENT_MARK_QTY   ,
REFRESH_ELEMENT_MARK_START ,
REFRESH_ELEMENT_MARK_STOP  ,
REFRESH_ELEMENT_MARK_STEP  ,
REFRESH_KEY_ENTER_MARK     ,
REFRESH_GATE_POSITION      ,
REFRESH_MULTI_GROUP_ACTION ,
REFRESH_SETUP_CONFIG_LOAD  ,
REFRESH_PRF_CHANGED        ,
REFRESH_SCAN_RESOLUTION    ,
REFRESH_CALIBRATION_REDRAW ,
REFRESH_PART_POSITION      ,
REFRESH_THICKNESS_CHANGED  ,
REFRESH_SCANDATA_LOAD ,
REFRESH_PROBE_LOAD ,
REFRESH_CURRENT_LAW_CHANGED,
REFRESH_DRAW_AREA_ALL ,
};

#define CONFIG_OFFSET              0x00100000
#define SCAN_DATA_MARK_OFFSET      0x00200000
#define REGION_SIZE                0x00040000
#define DMA_STORE_BUFF_START_ADDR  0x90000000
#define DMA_DATA_OFFSET                     2

extern const gchar **menu_content;
extern _group_wizard g_group_wizard_struct;
extern GROUP g_tmp_group_struct;
extern const gchar *backpic[];
extern GdkPixbuf *g_pixbuf_[20];


extern gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
extern gint (*dialog_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
extern gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
extern GtkWidgetClass *widget_window_class;
extern GtkWidgetClass *dialog_window_class;									/* 主windows class */
extern void change_keypress_event(gint window_type);

extern guint get_freq ();
extern double get_pw ();
extern int   get_prf ();
extern int   MultiGroupGetMaxPrf() ;
extern guint get_max_point_qty();

extern void tt_label_show_string (GtkWidget *label, const gchar *s1, const gchar *s2,
		const gchar *s3, const gchar *color,  guint font_size);
extern void tt_label_show_float (GtkWidget *label, const gchar *s1, double value, guint digit,
		const gchar *s3, const gchar *color,  guint font_size);

extern void change_language();										/* 选择语言 */
extern void draw_menu1();
extern void draw_menu2(gint pa);									/* 更新二级菜单 */
extern void draw_menu3(gint pa, gpointer pt);					    /* 更新三级菜单 */
extern void init_ui(DRAW_UI_P p);									/* 初始化界面 */
extern void draw_area_all();

extern void send_dsp_data (guint data_type, guint value);
extern gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
extern gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data);



extern void data_dialog (GtkMenuItem *menuitem, gpointer data);

extern gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

extern gboolean key_press_handler (GtkWidget* pWidget, GdkEventKey* pEvent, gpointer data);
extern void main_menu_pop();
extern void menu3_pop();

//extern void draw_warning(guint btn_qty, const gchar *warn_info);

extern void draw3_pressed1(double step, guint digit);

extern void da_call_remark (GtkDialog *dialog, gint response_id, gpointer user_data);


//extern int  on_changed_open_config_file(GtkTreeSelection *selection,	gpointer       data);
//extern int  on_changed_save_config_file(GtkTreeSelection *selection,	gpointer       data);
extern void add_to_list(GtkWidget *list, const gchar *str, guint count);
extern void draw_field_name ();
extern void update_widget_bg(GtkWidget *widget, int i);
extern void draw3_data1(DRAW_UI_P p) ;


extern void draw3_pop_tt (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status);
extern void draw3_pop_tt_shen (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status) ;

extern void draw_keyboard (GtkWidget *widget, GdkEventButton *event,	gpointer data);

extern void data_file_type(GtkMenuItem *menuitem, gpointer data);

extern void draw3_digit_pressed (void (*fun)(GtkSpinButton*, gpointer), const gchar *unit,
		double cur_value, double lower, double upper, double step,
		guint digit, gpointer p, guint pos, guint content_pos) ;
extern void draw3_digit_stop(double cur_value, const gchar *unit,
		guint digit, guint pos, guint content_pos) ;
extern void draw3_popdown (const gchar *cur_value, guint pos, guint big_menu) ;
extern void draw3_popdown_offset (const gchar *cur_value, guint pos, guint big_menu, guint content_pos) ;
extern void SaveDataProcess(char* strTitle)  ;
extern int  Is_PATOFD_DisplayValidate() ;
extern unsigned int DisplayModeValidateFlag() ;
extern void RefreshPrfMark() ;
extern void SettingFrameBufferWindowConfig() ;
extern void InitializeSscanMatrix();
extern void DrawDisplayWindowFrame() ;
extern void DrawDisplayWindowFrameCalibration() ;
extern void SaveDataProcess(char* strTitle) ;
extern void setKeyInvalidateWhenDataRecalling(int bEnable_);
extern void UpdateWindowTitle() ;
extern void FreezeScreen(int enable) ;
extern void RefreshGainMark(int grp);
/* 显示各个菜单图标的名称 */
extern const gchar **con0_p;
extern const gchar ***con1_p;
extern const gchar ****con2_p;
extern const gchar **units;
extern const gchar **menu_content;

extern const gchar **list;
extern const gchar **list1;
extern const gchar **field1;
extern const gchar **field;
extern const gchar **field_unit;


extern GdkColor	color_black;
extern GdkColor	color_black1;
extern GdkColor	color_white;
extern GdkColor	color_yellow;
extern GdkColor	color_green;
extern GdkColor	color_blue;
extern GdkColor	color_red;
extern GdkColor	color_text_base;
extern GdkColor color_rule;


extern volatile int REFRESH_REQUEST ;
#if ARM
extern volatile int  *DMA_MARK       ;
extern volatile unsigned char *ScanDataMark   ;
#else
extern int DMA_MARK[10] ;
extern unsigned char ScanDataMark[256 * 1024] ;
#endif


extern pthread_mutex_t draw_thread_mutex  ;
#endif

