#ifndef      __MAIN___H_______
#define     __MAIN___H_______

#include "base.h"

extern volatile DRAW_UI_P	pp;
extern void ExitApplication (GtkWidget *widget, GdkEventButton *event,	gpointer data) ;

extern void MultiGroupSendFocalSpi();
extern void MultiGroupSendGroupSpi();

extern void send_focal_spi (int grp , int reset);
extern void SettingGroupConfigure (int grp);

extern unsigned int GetChannelEnable() ;

extern void MainInit () ;

extern void RefreshGroupFocalLawSpi (int group) ;
extern void set_config(unsigned int grp);
extern void init_group_spi (int group)  ;


#endif
