#ifndef ____CALLBACK___H____________
#define ____CALLBACK___H____________
#include "base.h"


extern int    PrfLimitToGroupSampleRange(int GroupId_) ;
extern int    StoreBuffLimitToGroupPointQty(int GroupId_) ;
extern void   generate_focallaw(int grp);
extern int    RefreshScanInfor() ;
extern int    GetGroupMaxBeamDelay(int GroupId_) ;
extern int    GetGroupRxTime(int GroupId_) ;
extern void   MultiGroupRefreshIdelTime() ;
extern double GetGainLimit(int nGroupId_) ;
extern void   set_filter (int group) ;
extern void   data_234_add_point (DRAW_UI_P p) ;
extern void   data_235_del_point  (DRAW_UI_P p) ;
extern void   request_refresh(int mark)  ;
extern void   UpdateGateForSpiSending(int nGroupId_) ;
extern int    DrawAreaButtonClick (GtkWidget* widget, GdkEvent* event, gpointer data);
extern int    IsGateSynchro();
extern void   cal_focal_law (guint group);
extern int	  get_beam_qty() ;
extern int    GetGroupBeamQty(int grp) ;

extern void data_root_angle_callback (GtkSpinButton *spinbutton, gpointer data);/*root angle*/
extern void data_root_height_callback (GtkSpinButton *spinbutton, gpointer data); /*root height*/

extern void data_0021 (GtkMenuItem *menuitem, gpointer data);
extern void data_0022 (GtkMenuItem *menuitem, gpointer data);
extern void data_0023 (GtkMenuItem *menuitem, gpointer data);
extern void data_00241 (GtkSpinButton *spinbutton, gpointer data);
extern void data_00242 (GtkMenuItem *menuitem, gpointer data);
extern void data_0025 (GtkMenuItem *menuitem, gpointer data); /*  */
extern void data_0027 (GtkSpinButton *spinbutton, gpointer data); /**/
extern void data_00341 (GtkSpinButton *spinbutton, gpointer data);
extern void data_00342 (GtkSpinButton *spinbutton, gpointer data); /* Pulser 发射 P110 */
extern void data_0037 (GtkSpinButton *spinbutton, gpointer data); /**/
extern void data_0044 (GtkSpinButton *spinbutton, gpointer data);
extern void data_00471 (GtkSpinButton *spinbutton, gpointer data);
extern void data_00472 (GtkMenuItem *menuitem, gpointer data); /* Skew (deg) */
extern void data_022 (GtkMenuItem *menuitem, gpointer data);
extern void data_0221 (GtkMenuItem *menuitem, gpointer data);
extern void data_0222 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0223 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0224 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0225 (GtkMenuItem *menuitem, gpointer data);
extern void data_0226 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0227 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0228 (GtkSpinButton *spinbutton, gpointer data);
extern void data_023  (GtkMenuItem *menuitem, gpointer data);
extern void data_0231 (GtkMenuItem *menuitem, gpointer data);
extern void data_0232 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0233 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0234 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0238 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0239 (GtkSpinButton *spinbutton, gpointer data);
extern void data_024 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0241 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0242 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0243 (GtkSpinButton *spinbutton, gpointer data);
extern void data_032 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0321 (GtkSpinButton *spinbutton, gpointer data);
extern void data_033 (GtkMenuItem *menuitem, gpointer data);
extern void data_0331 (GtkSpinButton *spinbutton, gpointer data);
extern void data_0332 (GtkSpinButton *spinbutton, gpointer data);
extern void data_034 (GtkSpinButton *spinbutton, gpointer data);

extern void data_100 (GtkSpinButton *spinbutton, gpointer data);	/* 100 gain增益 */
extern void data_101 (GtkSpinButton *spinbutton, gpointer data);	/* 101 start 扫描延时 */
extern void data_102 (GtkSpinButton *spinbutton, gpointer data);	/* 102 range 范围 */
extern void data_103 (GtkSpinButton *spinbutton, gpointer data);	/* 103 wedget delay 楔款延时 */
extern void data_104 (GtkSpinButton *spinbutton, gpointer data);	/* 104 velocity 声速 */

extern void data_110 (GtkSpinButton *spinbutton, gpointer data);	/* 110 pulser 脉冲发射器 */
extern void data_120 (GtkSpinButton *spinbutton, gpointer data);	/* 120 receiver 脉冲接收器 */
extern void data_111 (GtkMenuItem *menuitem, gpointer data);            /* 111 Tx/Rx Mode 收发模式 */
extern void data_1121 (GtkSpinButton *spinbutton, gpointer data);       /* 112 freq 频率 */
extern void data_112 (GtkMenuItem *menuitem, gpointer data);            /* 112 Freq 频率 */
extern void data_113 (GtkMenuItem *menuitem, gpointer data);            /* 113 Voltage */
extern void data_1141 (GtkSpinButton *spinbutton, gpointer data);       /* 114 PW 脉宽*/
extern void data_114 (GtkMenuItem *menuitem, gpointer data);            /* 114 PW 脉宽 */
extern void data_1151 (GtkSpinButton *spinbutton, gpointer data);       /* 115 PRF  重复频率*/
extern void data_115 (GtkMenuItem *menuitem, gpointer data);            /* 115 PRF  重复频率 */

extern void data_1201 (GtkMenuItem *menuitem, gpointer data);
extern void data_121 (GtkMenuItem *menuitem, gpointer data);            /* 121 Filter */
extern void data_122 (GtkMenuItem *menuitem, gpointer data);            /* 122 Rectifier */
extern void data_124 (GtkMenuItem *menuitem, gpointer data);            /* 124 Averaging */
extern void data_125 (GtkSpinButton *spinbutton, gpointer data);	/* 125 */

extern void data_130 (GtkSpinButton *spinbutton, gpointer data);	/* 130 */
extern void data_131 (GtkSpinButton *spinbutton, gpointer data);	/* 131 */
extern void data_132 (GtkSpinButton *spinbutton, gpointer data);	/* 132 */
extern void data_134 (GtkSpinButton *spinbutton, gpointer data);	/* 134 */
extern void data_135 (GtkSpinButton *spinbutton, gpointer data);	/* 135 */

extern void data_140 (GtkSpinButton *spinbutton, gpointer data);
extern void data_1431(GtkSpinButton *spinbutton, gpointer data);        /* 143 Points Qty */
extern void data_143 (GtkMenuItem *menuitem, gpointer data);            /* 143 Points Qty */
extern void data_1451(GtkSpinButton *spinbutton, gpointer data);        /* 145 Sum Gain */
extern void data_145 (GtkMenuItem *menuitem, gpointer data);            /* 145 Sum Gain */

extern void data_200 (GtkMenuItem *menuitem, gpointer data);        /*  200 Gate */
extern void data_201 (GtkMenuItem *menuitem, gpointer data);        /*  201 parameter */
extern void data_202 (GtkSpinButton *spinbutton, gpointer data);	/* 202 */
extern void data_2021 (GtkMenuItem *menuitem, gpointer data);         /*  2021 synchro */
extern void data_203 (GtkSpinButton *spinbutton, gpointer data);	/* 203 */
extern void data_2031 (GtkMenuItem *menuitem, gpointer data);         /*  2031 measure */
extern void data_204 (GtkSpinButton *spinbutton, gpointer data);	/* 204 */
extern void data_2041 (GtkMenuItem *menuitem, gpointer data);         /*  2041 rf */

extern void data_210 (GtkMenuItem *menuitem, gpointer data);         /*  210 Alarm */
extern void data_211 (GtkMenuItem *menuitem, gpointer data);         /*  211 Group A */
extern void data_212 (GtkMenuItem *menuitem, gpointer data);         /*  212 condition A */
extern void data_213 (GtkMenuItem *menuitem, gpointer data);         /*  213 Operator */
extern void data_214 (GtkMenuItem *menuitem, gpointer data);         /*  214 Group B */
extern void data_215 (GtkMenuItem *menuitem, gpointer data);         /*  215 condition B */

extern void data_220 (GtkMenuItem *menuitem, gpointer data);          /* 220 Output */
extern void data_221 (GtkMenuItem *menuitem, gpointer data);         /* Output -> alarm # */
extern void data_2211 (GtkMenuItem *menuitem, gpointer data);         /* Output -> group */
extern void data_222 (GtkSpinButton *spinbutton, gpointer data);	/*222 Output -> count */
extern void data_2221 (GtkMenuItem *menuitem, gpointer data);         /* Output -> data */
extern void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
extern void data_224 (GtkSpinButton *spinbutton, gpointer data);	/*224*/
extern void data_225 (GtkSpinButton *spinbutton, gpointer data);	/*225*/
extern void data_230 (GtkMenuItem *menuitem, gpointer data);         /* Gate/Alarm -> Sizing Curves -> Mode 230 */
extern void data_231 (GtkMenuItem *menuitem, gpointer data);         /* Gate/Alarm -> Sizing Curves -> Curve 231 */
extern void data_2311 (GtkMenuItem *menuitem, gpointer data);
extern void data_2312 (GtkSpinButton *spinbutton, gpointer data);
extern void data_232 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2321 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2322 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2323 (GtkSpinButton *spinbutton, gpointer data);
extern void data_233 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2331 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2332 (GtkSpinButton *spinbutton, gpointer data);
extern void data_234 (GtkSpinButton *spinbutton, gpointer data);
extern void data_235 (GtkSpinButton *spinbutton, gpointer data);

extern void data_300 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> list 300 */
extern void data_302 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field 302 */
extern void data_303 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field2 303 */
extern void data_304 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field3 304 */
extern void data_305 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field4 305 */

extern void data_310 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> cursors -> selection 310 */
extern void data_311 (GtkSpinButton *spinbutton, gpointer data);	/*311*/
extern void data_3111 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3112 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3113 (GtkSpinButton *spinbutton, gpointer data);
extern void data_312 (GtkSpinButton *spinbutton, gpointer data);	/*312*/
extern void data_3121 (GtkSpinButton *spinbutton, gpointer data);	/*312*/
extern void data_3122 (GtkSpinButton *spinbutton, gpointer data);
extern void data_313 (GtkSpinButton *spinbutton, gpointer data);	/*313*/
extern void data_3131 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3132 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3133 (GtkSpinButton *spinbutton, gpointer data);
extern void data_314 (GtkSpinButton *spinbutton, gpointer data);	/*314*/
extern void data_3141 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3142 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3143 (GtkSpinButton *spinbutton, gpointer data);
extern void data_315 (GtkSpinButton *spinbutton, gpointer data);
extern void data_324 (GtkSpinButton *spinbutton, gpointer data);        /*324*/
extern void data_330 (GtkMenuItem *menuitem, gpointer data);          /* Measurements -> Thickness -> source 330 */
extern void data_331 (GtkSpinButton *spinbutton, gpointer data);	/*331*/
extern void data_332 (GtkSpinButton *spinbutton, gpointer data);	/*332*/
extern void data_333 (GtkSpinButton *spinbutton, gpointer data);	/*333*/

extern void data_400 (GtkMenuItem *menuitem, gpointer data);            /*400*/
extern void data_401 (GtkMenuItem *menuitem, gpointer data);            /*401*/
extern void data_4011 (GtkMenuItem *menuitem, gpointer data);
extern void data_4012 (GtkMenuItem *menuitem, gpointer data);
extern void data_4013 (GtkMenuItem *menuitem, gpointer data);
extern void data_402 (GtkMenuItem *menuitem, gpointer data);
extern void data_4021 (GtkMenuItem *menuitem, gpointer data);
extern void data_403 (GtkMenuItem *menuitem, gpointer data);
extern void data_404 (GtkSpinButton *spinbutton, gpointer data);
extern void data_405 (GtkSpinButton *spinbutton, gpointer data);
extern void data_410 (GtkMenuItem *menuitem, gpointer data);            /*410*/
extern void data_411 (GtkMenuItem *menuitem, gpointer data);            /*411*/
extern void data_420 (GtkMenuItem *menuitem, gpointer data);            /*420*/
extern void data_421 (GtkMenuItem *menuitem, gpointer data);
extern void data_422 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4221 (GtkSpinButton *spinbutton, gpointer data);
extern void data_423 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4231 (GtkSpinButton *spinbutton, gpointer data);
extern void data_424 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4241 (GtkSpinButton *spinbutton, gpointer data);
extern void data_425 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4251 (GtkSpinButton *spinbutton, gpointer data);
extern void data_430 (GtkMenuItem *menuitem, gpointer data);            /*430*/
extern void data_431 (GtkSpinButton *spinbutton, gpointer data);	/*431*/
extern void data_4311 (GtkSpinButton *spinbutton, gpointer data);       /*4311*/
extern void data_4312 (GtkSpinButton *spinbutton, gpointer data);       /*4311*/
extern void data_432 (GtkSpinButton *spinbutton, gpointer data);	/*432*/
extern void data_4321 (GtkSpinButton *spinbutton, gpointer data);	/*432*/
extern void data_4322 (GtkSpinButton *spinbutton, gpointer data);	/*432*/
extern void data_434 (GtkMenuItem *menuitem, gpointer data);            /*434*/
extern void data_440 (GtkMenuItem *menuitem, gpointer data);            /*440*/
extern void data_441 (GtkMenuItem *menuitem, gpointer data);            /*441*/
extern void data_4411 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4414 (GtkMenuItem *menuitem, gpointer data);
extern void data_4415 (GtkMenuItem *menuitem, gpointer data);
extern void data_442 (GtkMenuItem *menuitem, gpointer data);            /*442*/
//extern void data_443 (GtkMenuItem *menuitem, gpointer data);            /*443*/
extern void data_444 (GtkMenuItem *menuitem, gpointer data);            /*444*/
extern void data_445 (GtkMenuItem *menuitem, gpointer data);            /*445*/

extern void data_500 (GtkMenuItem *menuitem, gpointer data);
extern void data_501 (GtkMenuItem *menuitem, gpointer data);
extern void data_502 (GtkMenuItem *menuitem, gpointer data);
extern void data_510 (GtkSpinButton *spinbutton, gpointer data);	/*510*/
extern void data_511 (GtkSpinButton *spinbutton, gpointer data);	/*511*/
extern void data_512 (GtkMenuItem *menuitem, gpointer data);            /* 512 Skew (deg) */

void data_520();
extern void data_521 (GtkSpinButton *spinbutton, gpointer data);	/*521*/
extern void data_522 (GtkSpinButton *spinbutton, gpointer data);	/*522*/
extern void data_523 (GtkSpinButton *spinbutton, gpointer data);	/*523*/

extern void data_530 (GtkMenuItem *menuitem, gpointer data);
extern void data_531 (GtkSpinButton *spinbutton, gpointer data);	/*531*/
extern void data_532 (GtkSpinButton *spinbutton, gpointer data);
extern void data_533 (GtkMenuItem *menuitem, gpointer data);
extern void data_534 (GtkMenuItem *menuitem, gpointer data);

extern void data_600 (GtkMenuItem *menuitem, gpointer data);

extern void data_610 (GtkSpinButton *spinbutton, gpointer data);	/*610*/
extern void data_611 (GtkSpinButton *spinbutton, gpointer data);	/*611*/
extern void data_612 (GtkSpinButton *spinbutton, gpointer data);	/*612*/
extern void data_613 (GtkSpinButton *spinbutton, gpointer data);	/*613*/
extern void data_614 (GtkSpinButton *spinbutton, gpointer data);	/*614*/

extern void data_620 (GtkMenuItem *menuitem, gpointer data);		/*620*/
extern void data_621 (GtkSpinButton *spinbutton, gpointer data);
extern void data_6211 (GtkSpinButton *spinbutton, gpointer data);
extern void data_622 (GtkSpinButton *spinbutton, gpointer data);
extern void data_6221 (GtkSpinButton *spinbutton, gpointer data);
extern void data_623 (GtkSpinButton *spinbutton, gpointer data);	/*623*/
extern void data_6231 (GtkSpinButton *spinbutton, gpointer data);	/*623*/
extern void data_624 (GtkSpinButton *spinbutton, gpointer data);
extern void data_630 (GtkSpinButton *spinbutton, gpointer data);
extern void data_631 (GtkSpinButton *spinbutton, gpointer data);
extern void data_632 (GtkSpinButton *spinbutton, gpointer data);
extern void data_633 (GtkSpinButton *spinbutton, gpointer data);
extern void data_634 (GtkMenuItem *menuitem, gpointer data);

extern void data_700 (GtkMenuItem *menuitem, gpointer data);
extern void data_701 (GtkMenuItem *menuitem, gpointer data);
extern void data_702 (GtkMenuItem *menuitem, gpointer data);
extern void data_703 (GtkSpinButton *spinbutton, gpointer data);
extern void data_704 (GtkSpinButton *spinbutton, gpointer data);
extern void data_710 (GtkMenuItem *menuitem, gpointer data);
extern void data_711 (GtkMenuItem *menuitem, gpointer data);
extern void data_712 (GtkMenuItem *menuitem, gpointer data);
extern void data_713 (GtkSpinButton *spinbutton, gpointer data);	/*713*/
extern void data_714 (GtkSpinButton *spinbutton, gpointer data);
extern void data_715 (GtkSpinButton *spinbutton, gpointer data);

extern void data_720 (GtkSpinButton *spinbutton, gpointer data);	/*720*/
extern void data_721 (GtkSpinButton *spinbutton, gpointer data);	/*721*/
extern void data_722 (GtkSpinButton *spinbutton, gpointer data);	/*722*/
extern void data_723 (GtkSpinButton *spinbutton, gpointer data);	/*723*/
extern void data_724 (GtkSpinButton *spinbutton, gpointer data);	/*724*/
extern void data_725 (GtkSpinButton *spinbutton, gpointer data);	/*725*/
extern void data_730 (GtkMenuItem *menuitem, gpointer data);
extern void data_732 (GtkMenuItem *menuitem, gpointer data);
extern void data_740 (GtkMenuItem *menuitem, gpointer data);
extern void data_741 (GtkMenuItem *menuitem, gpointer data);

extern void data_800 (GtkMenuItem *menuitem, gpointer data);
extern void data_804 (GtkMenuItem *menuitem, gpointer data);
extern void data_810 (GtkMenuItem *menuitem, gpointer data);
extern void data_812 (GtkMenuItem *menuitem, gpointer data);
extern void data_824 (GtkMenuItem *menuitem, gpointer data);
extern void data_830 (GtkMenuItem *menuitem, gpointer data);

extern void data_900 (GtkMenuItem *menuitem, gpointer data);
extern void data_901 (GtkSpinButton *spinbutton, gpointer data);	/*901*/
extern void data_904 (GtkMenuItem *menuitem, gpointer data);
extern void data_903 (GtkMenuItem *menuitem, gpointer data);
extern void data_912 (GtkMenuItem *menuitem, gpointer data);
extern void data_913 (GtkMenuItem *menuitem, gpointer data);
extern void data_9131 (GtkMenuItem *menuitem, gpointer data);
extern void data_930 (GtkMenuItem *menuitem, gpointer data);
extern void b2_fun0(DRAW_UI_P p, guint pos);
extern void b3_fun0(gpointer data);
extern void b3_fun1(gpointer data);
extern void b3_fun2(gpointer data);
extern void b3_fun3(gpointer data);
extern void b3_fun4(gpointer data);
extern void b3_fun5(gpointer data);

extern void RefreshGainOffset(int grp) ;
extern void RefreshBeamDelay (int grp) ;
extern void ClearOneGroupParameters(int nGroupId_) ;
extern int    IsDisplayModalAvailable(unsigned char nDisplayType_) ;
extern int CheckGroupConfigureAvailable() ;
extern int IsDisplayModalAvailable(unsigned char nDisplayType_) ;
extern double tttmp;
#endif
