
/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 *
 *
 */

#include <pango-1.0/pango/pango.h>
#include <cairo.h>
#include "drawui.h"
#include "file_op.h"
#include "drawfb.h"
#include "draw_dialog.h"
#include "callback.h"
#include "main.h"
#include "calibration.h"
#include "ui/workpiece.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glib/gprintf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/rtc.h>
#include <string.h>
#include <assert.h>
#include <gdk/gdkkeysyms.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <math.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#include "string/_string.h"
#include "ui/menu/drawmenu.h"
#include "ui/menu/menuFunc.h"
#include "ui/menu/menu.h"
#include "ui/dialog/reportPreview.h"
#include "lzk/output_spi.h"
#include "ui/ui.h"
#include "globalData.h"
#include "string/getFieldString.h"
#include "calculation/getMeasureData.h"
#include "calculation/sizingcurve.h"
#include "calculation/limit.h"
#include "ui/draw_overlay.h"
#include "ui/authorization.h"
#include "threads/threads.h"

void showHelp(MENU_UNION menu_union ,int enable);
void menu3FuncTrans(MENU_UNION);

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS((GtkObject*)(i))->x
#define	P_CFG	(pp->p_config)


GdkColor	color_black     = {0x0, 0x0, 0x0, 0x0};
GdkColor	color_black1    = {0x0, 0x0, 0x0, 0x0800};
GdkColor	color_white     = {0x0, 0xffff, 0xffff, 0xffff};
GdkColor	color_yellow    = {0x0, 0xffff, 0xffff, 0x0};
GdkColor	color_green     = {0x0, 0x0, 0xffff, 0x0};
GdkColor	color_blue      = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_red       = {0x0, 0xffff, 0x0, 0x0};
GdkColor	color_text_base = {0x0, 0x1300, 0x4900, 0x7600};
GdkColor	color_rule      = {0x0, 0xc300, 0xf000, 0x1d00};

GROUP g_tmp_group_struct;

pthread_mutex_t draw_thread_mutex = PTHREAD_MUTEX_INITIALIZER;

#if ARM
volatile int  *DMA_MARK       ;
volatile unsigned char *ScanDataMark   ;
#else
int DMA_MARK[10] ;
unsigned char ScanDataMark[256 * 1024] ;
#endif
volatile int REFRESH_REQUEST ;

static char *b_keyboard_display[] = 
{
	"1\0", "2\0", "3\0", "4\0", "5\0", "6\0", "7\0", "8\0", "9\0", "0\0", 
	"-\0", "=\0", "BackSpace\0", "Tab\0", "Q\0", "W\0", "E\0", "R\0", "T\0",
	"Y\0", "U\0", "I\0", "O\0", "P\0", "[\0", "]\0","Caps Lock\0","A\0", "S\0", 
	"D\0", "F\0", "G\0","H\0", "J\0", "K\0", "L\0", ";\0","'\0","Enter\0", 
	"Shift       \0", "Z\0", "X\0", "C\0", "V\0", "B\0","N\0", "M\0", ",\0", 
	".\0", "↑\0", "/\0", "Delete\0","SuperKey\0","Alt\0", 
	"          Space            \0", "←\0", "↓\0", "→\0"
};

static char *s_keyboard_display[] = 
{
	"1\0", "2\0", "3\0", "4\0", "5\0", "6\0", "7\0", "8\0", "9\0", "0\0", 
	"-\0", "=\0", "BackSpace\0", "Tab\0", "q\0", "w\0", "e\0", "r\0", "t\0",
	"y\0", "u\0", "i\0", "o\0", "p\0", "[\0", "]\0","Caps Lock\0","a\0", "s\0", 
	"d\0", "f\0", "g\0","h\0", "j\0", "k\0", "l\0", ";\0","'\0","Enter\0", 
	"Shift       \0", "z\0", "x\0", "c\0", "v\0", "b\0","n\0", "m\0", ",\0", 
	".\0", "↑\0", "/\0", "Delete\0","SuperKey\0","Alt\0", 
	"          Space            \0", "←\0", "↓\0", "→\0"
};
static gushort keyboard_send[] = 
{
	XK_1, XK_2, XK_3, XK_4, XK_5, XK_6,XK_7, XK_8, XK_9, XK_0, XK_minus, 
	XK_equal, XK_BackSpace, XK_Tab, XK_q, XK_w, XK_e, XK_r, XK_t, XK_y,
	XK_u, XK_i, XK_o, XK_p, XK_bracketleft, XK_bracketright, XK_Caps_Lock,
	XK_a, XK_s, XK_d,XK_f, XK_g, XK_h, XK_j, XK_k, XK_l,XK_semicolon,
	XK_apostrophe, XK_Return,  XK_Shift_L, XK_z, XK_x, XK_c, XK_v, XK_b,XK_n,
	XK_m, XK_comma, XK_period, XK_Up, XK_slash,XK_Delete, XK_Super_L, XK_Alt_L,
	XK_space,XK_Left, XK_Down, XK_Right
};

static char *numkeyboard_display[]=
{
	"1\0", "2\0", "3\0", "+","4\0", "5\0", "6\0","-", "7\0", "8\0", "9\0", ".", "0\0",  "←\0", "→\0", "Backspace\0", "Delete\0", "Enter\0"
};

static gushort numkeyboard_send[] = 
{
	XK_1, XK_2, XK_3, XK_plus, XK_4, XK_5, XK_6, XK_minus, XK_7, XK_8,  XK_9, XK_period, XK_0,  XK_Left, XK_Right, XK_BackSpace, XK_Delete, XK_Return
};


static DRAW_UI_P p_drawui_c;

gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);/* window 原始的按键处理 */
gint (*dialog_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);/* window 原始的按键处理 */
gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);			/* 自己的按键处理*/

GtkWidgetClass *widget_window_class;									/* 主windows class */
GtkWidgetClass *dialog_window_class;									/* 主windows class */

const gchar *backpic[] = 
{
	"pic/10.png", "pic/11.png", "pic/12.png",		/* 二级菜单 0按下 1未选中 2 停留 */
	"pic/00.png", "pic/01.png", "pic/02.png",	        /* 00上方数值 标识栏 01 数值背景 02 比较小的背景 */
	"pic/20.png", "pic/21.png", "pic/22.png",		/* 三级菜单名称 0按下 1未选中 2 停留 */
	"pic/30.png", "pic/31.png", "pic/32.png",		/* 三级菜单数值 0按下 1未选中 2 停留 */
	"pic/tt.png", "pic/311.png", "pic/322.png",      	/* 软键盘图标 */
	"pic/snow.png","pic/black_25.png","pic/file_name_40.png",
	"pic/clock.png" , "pic/gear.png"
};

GdkPixbuf *g_pixbuf_[20] = {0};

void change_language(guint lang, DRAW_UI_P p);
void draw_menu1();
void draw_menu2(gint pa);
void draw_menu3(gint pa, gpointer pt);
void init_ui(DRAW_UI_P p);				/* 初始化界面 */
void draw_area_all();

ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timout);
ssize_t treadn(int fd, void *buf, size_t nbytes, unsigned int timout);


void draw3_data0(DRAW_UI_P p);
void draw3_data1(DRAW_UI_P p);
void draw3_data2(DRAW_UI_P p);
void draw3_data3(DRAW_UI_P p);
void draw3_data4(DRAW_UI_P p);
void draw3_data5(DRAW_UI_P p);

void draw_field_name ();
void calc_measure_data();

int  DrawDynamicBar (GtkWidget* widget, GdkEventExpose *event, gpointer data);
int  RefreshSaveingDialog(gpointer data) ;
void SaveDataProcess(char* strTitle) ;
void DrawDisplayWindowFrame()  ;
void DrawFftInfo(GtkWidget *widget , gpointer data , cairo_t *cr) ;
void (*draw_data3[6])(DRAW_UI_P p) =
{
	draw3_data0,
	draw3_data1,
	draw3_data2,
	draw3_data3,
	draw3_data4,
	draw3_data5
};

/* 显示各个菜单图标的名称 */
const gchar **con0_p;
const gchar ***con1_p;
const gchar ****con2_p;
const gchar **units;
const gchar **menu_content;

const gchar **list;
const gchar **list1;
const gchar **field1;
const gchar **field;
const gchar **field_unit;



void FreezeScreen(int enable)
{
	if(enable)
	{
		gdk_threads_enter();
		update_widget_bg(pp->event[19], /*backpic[12]*/ 15);
		gdk_threads_leave();
	}
	else
	{
		gdk_threads_enter();
		update_widget_bg(pp->event[19], 16);
		gdk_threads_leave();
	}
	TMP(freeze) = enable;
	FreezingFPGA(enable);
}

//void menuitem0_function(GtkMenuItem *menuitem, gpointer data);

gint my_keypress_event(GtkWidget *widget, GdkEventKey *event)
{
	gpointer data = NULL;
	key_press_handler (widget, event, data);
	return 0;
}

void set_menu_position_tdh (GtkMenu *menu, gint *x, gint *y,
		gboolean *push_in, gpointer status_icon)
{
	*x = pp->x_pos ;
	*y = pp->y_pos ;
	return;
}
void set_menu_position_mainmenu (GtkMenu *menu, gint *x, gint *y,
		gboolean *push_in, gpointer status_icon)
{
	*x = 0;
	*y = 160;
	return;
}

extern const guint8* gPicInline[];
static void all_bg_pic_in_mem()
{   
	int i = 0;
	int count = sizeof(backpic) / sizeof(backpic[0]);
	for(i=0;i<count;i++)
	{
		g_pixbuf_[i] = gdk_pixbuf_new_from_inline(-1, gPicInline[i],TRUE, NULL);
	}
}

// two or three group is needed
// at least one UT  one PA
// UT group PR mode needs to be TOFD
int Is_PATOFD_DisplayValidate()
{
	int i;
	int _nPAGroupQty = 0 ;
	int _nUTGroupQty = 0 ;
	int _nTOFDGroupQty = 0 ;
	int _nGroupQty = get_group_qty(pp->p_config);

	if(_nGroupQty != 2 && _nGroupQty != 3)
    {
    	return FALSE ;
    }

    for(i = 0 ; i < _nGroupQty ; i ++)
    {
        if(GROUP_VAL_POS(i , group_mode) == PA_SCAN)
        {
        	_nPAGroupQty++ ;
        } else if(group_get_rx_tx_mode(i) == TOFD) {
            _nTOFDGroupQty++;
        } else {
        	_nUTGroupQty++ ;
        }
    }

    if (_nTOFDGroupQty && !_nUTGroupQty) {
        return TRUE;
    }

    if(!_nUTGroupQty)
    {
    	return FALSE ;
    }
    if(!_nPAGroupQty)
    {
    	return FALSE ;
    }

    if (!_nTOFDGroupQty) {
    	return FALSE ;
    }

    return TRUE ;
}


unsigned int DisplayModeValidateFlag()
{
	unsigned int menu_status = 0;
	if(!Is_PATOFD_DisplayValidate())
	{
		menu_status =0x600;
	}

	int _nGroupQty = get_group_qty(pp->p_config) ;
	// 1 是当前， 0  是全部
	unsigned char _nDisplayGroup = get_display_group (pp->p_config ) ; //, DISPLAY_CURRENT_GROUP);
	if(_nGroupQty > 1)
	{
		if(!_nDisplayGroup)
		{
			if(_nGroupQty == 2 || _nGroupQty == 3) menu_status = menu_status | 0x64 ;
			else     menu_status = menu_status | 0x1fe ;
		}
	}
	return menu_status  ;
}

/* 设置控件的背景图片 */
void update_widget_bg(GtkWidget *widget,int i)
{
    GtkStyle *style;
    GdkPixmap *pixmap; 
    GdkColormap *colormap;
    int alpha_threshold = 0;
    static gboolean flag = FALSE;
    
    colormap = gdk_colormap_get_system ();

    if ( i == 16 ) {
	    static GdkPixmap *_pixmap = NULL; 
	    if ( ! flag ) {
		    gdk_pixbuf_render_pixmap_and_mask_for_colormap(g_pixbuf_[16],
				    colormap, &_pixmap, NULL, alpha_threshold);
		    flag = TRUE;
	    }
        pixmap = gdk_pixmap_ref(_pixmap);
    } else {
	    gdk_pixbuf_render_pixmap_and_mask_for_colormap(g_pixbuf_[i],
			    colormap, &pixmap, NULL, alpha_threshold);
    }
    style = gtk_style_copy(gtk_widget_get_style(widget));

	if (style->bg_pixmap[GTK_STATE_NORMAL])      
	{
		g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);  
	}

	style->bg_pixmap[GTK_STATE_NORMAL] = (pixmap);
	gtk_widget_set_style( widget, style);
	g_object_unref(style);
}

void main_menu_pop(guint action)
{
	int i;
	static int _nPosSaved = 1;
	if (MENU_POP == action)	/*弹出主菜单*/
	{
		gtk_menu_popup ( GTK_MENU (pp->menu), NULL, NULL, 
				(GtkMenuPositionFunc)set_menu_position_mainmenu,
				NULL,
				0,
				gtk_get_current_event_time());
		for(i=0;i<10;i++)
			gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[i]));
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		_nPosSaved = pp->pos ;
	}
	else if (MENU_HIDE == action) /*收回主菜单*/
	{
		gtk_menu_popdown( GTK_MENU (pp->menu));
		pp->pos = _nPosSaved ;
	}
	else if (MENU_CHANGE == action) //||(MENU_DOWN == action) )	/*轮流切换主菜单*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 9)
		{
			pp->pos = 0;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
			{
				pp->pos++;
				if((pp->pos) > 9) pp->pos = 0 ;
			}

		}
		else
		{
			pp->pos++;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
			{
				pp->pos++;
				if((pp->pos) > 9) pp->pos = 0 ;
			}
		}
		_nPosSaved = pp->pos ;
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		gtk_menu_item_set_label(GTK_MENU_ITEM (pp->menuitem_main), pp->con0_p[pp->pos]);
		gtk_menu_item_activate (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		MENU_STATUS = MENU3_STOP;
		draw_menu2(0);
		draw_menu3(0, NULL);
	}
	else if (MENU_DOWN == action)
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 9)
		{
			pp->pos = 0;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
			{
				pp->pos++;
				if((pp->pos) > 9) pp->pos = 0 ;
			}
		}
		else
		{
			pp->pos++;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
			{
				pp->pos++;
				if((pp->pos) > 9) pp->pos = 0 ;
			}
		}
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
	}
	else if (MENU_UP == action)/*向上切换主菜单*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 0)
		{
			pp->pos = 9;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
			{
				if((pp->pos) == 0) pp->pos = 9 ;
				else               pp->pos --  ;
			}
		}
		else
		{
			pp->pos--;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
			{
				if((pp->pos) == 0) pp->pos = 9 ;
				else               pp->pos -- ;
			}
		}
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
	}
	else if (MENU_ENTER == action) /*选中当前菜单项，并收回主菜单*/
	{
		gtk_menu_popdown( GTK_MENU (pp->menu));
		gtk_menu_item_set_label(GTK_MENU_ITEM (pp->menuitem_main), pp->con0_p[pp->pos]);
		gtk_menu_item_activate (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		MENU_STATUS = MENU3_STOP;
		draw_menu2(0);
		draw_menu3(0, NULL);
	}

}

/* 按键 弹出三级菜单选项 的处理函数 */
void menu3_pop(guint action)
{
	int i;
	if (MENU3_TURN == action)	/* 轮流、向下切换三级菜单选项 */
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));

		pp->menu3_e= pp->menu3_qty - 1;
		while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_e]))
			pp->menu3_e--;		/* pp->menu3_e为最后一个可以选的菜单选项位置 */

		if( (pp->menu3_poppos) == (pp->menu3_e) )	/*当在三级菜单选项的可以选择的最后一个位置时*/
		{
			pp->menu3_poppos = 0;			/*选中的位置跳到最前去*/
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))/*判断这一个位置是否可选*/
				pp->menu3_poppos++;
		}
		else
		{
			pp->menu3_poppos++;
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))
				pp->menu3_poppos++;
		}
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));
	}
	else if (MENU3_UP == action) /*向上切换三级菜单选项*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));

		pp->menu3_s=0;
		while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_s]))
			pp->menu3_s++;		/* pp->menu3_s为第一个可以选的菜单选项位置 */

		if((pp->menu3_poppos) == pp->menu3_s )		/*当在三级菜单选项的可以选择的第一个位置时*/
		{
			pp->menu3_poppos = (pp->menu3_qty - 1);	/*选中的位置跳到最后去*/
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))/*判断这一个位置是否可选*/
				pp->menu3_poppos--;
		}
		else
		{
			pp->menu3_poppos--;
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos])) 
				pp->menu3_poppos--;
		}

		gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));		
	}
	else if (MENU3_ENTER == action) 	/* 选中当前菜单项，并收回三级菜单 */
	{
		gtk_menu_item_activate (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));
		for(i=0;i<6;i++)
			gtk_menu_popdown( GTK_MENU (pp->menu33[i]));
		draw_menu3(1, NULL);
	}
	else if (MENU3_HIDE == action) 		/* 收回三级菜单选项 */
	{
		for(i=0;i<6;i++)
			gtk_menu_popdown( GTK_MENU (pp->menu33[i]));
	}
}

gboolean main_menu_press (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	main_menu_pop(MENU_POP);
	return TRUE;
}

void add_click (GtkButton *button, gpointer data)
{
	gtk_adjustment_set_value (pp->adj, gtk_adjustment_get_value (pp->adj) + gtk_adjustment_get_step_increment (pp->adj));

	gtk_adjustment_value_changed (pp->adj);
	return ;
}

void sub_click (GtkButton *button, gpointer data)
{
	gtk_adjustment_set_value (pp->adj, gtk_adjustment_get_value (pp->adj) - gtk_adjustment_get_step_increment (pp->adj));

	gtk_adjustment_value_changed (pp->adj);
	return ;
}

/*1级菜单 回调函数*/
void menuitem_function (GtkMenuItem *menuitem, gpointer data)
{
    if(pp->pos_pos == MENU3_PRESSED)
    {
    	pp->pos_pos = MENU3_STOP ;
    	draw_menu3(0, NULL);
    }
	DRAW_UI_P p = p_drawui_c;
	guint num = GPOINTER_TO_UINT(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));	/*  */
	p->pos_last = pp->pos; /*  */
	p->pos = num;
	gLastMenuPos.pos.menu1 = gCurrentMenuPos.pos.menu1;
	gCurrentMenuPos.pos.menu1 = num;
	gCurrentMenuPos.pos.menu2 = p->pos1[p->pos];
	gCurrentMenuPos.pos.menu3 = CUR_POS_P(p);

	switch (num)
	{
		case 0:
			p->menu2_qty = 4;
			break;
		case 1:
		case 3:
		case 6:
		case 7:
		case 8:
		case 9:
			p->menu2_qty = 5;
			break;
		case 2:
		case 4:
		case 5:
			p->menu2_qty = 4;
			break;
		default:break;
	}

	showHelp(gCurrentMenuPos ,pp->help_yn);/*弹出帮助窗口*/
	MENU_STATUS = MENU3_STOP; /* */
	draw_menu2(1);
	draw_menu3(1, NULL);
}

/* 画一级菜单*/
void draw_menu1 ()
{
	gint i;
	DRAW_UI_P p = p_drawui_c;
	for (i = 0; i < 10; i++)
		gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem[i]), p->con0_p[i]);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), p->con0_p[p->pos]);
	return ;
}

/* 画二级菜单 pa的作用未曾体现，用来表示菜单名子的位置 6 6 6 6 6 */
void draw_menu2 (gint pa)
{
	GdkColor	yellow    = {0x0, 0xffff, 0xffff, 0x0};
	GdkColor	white     = {0x0, 0xffff, 0xffff, 0xffff};
	DRAW_UI_P p = p_drawui_c;
	gint i;


	if (p->pos_pos != MENU3_PRESSED)
		p->mark_pop_change = 0;
	for (i = 0 ; i < 5 ; i++) 
	{
		if (p->con1_p[p->pos][i])
		{
			gtk_label_set_text (GTK_LABEL (p->label2[i]), p->con1_p[p->pos][i]);
			update_widget_bg (p->eventbox2[i], /*backpic[1]*/1);
			gtk_widget_modify_fg (p->label2[i], GTK_STATE_NORMAL, &white);
			gtk_widget_show (p->eventbox2[i]);
		}
		else
		{
			gtk_label_set_text (GTK_LABEL (p->label2[i]), " ");
			gtk_widget_hide (p->eventbox2[i]);
		}
	}
	/* 当前二级菜单不是停留就是按下 */
	if (p->pos_pos == 0)
	{
		update_widget_bg(p->eventbox2[p->pos1[p->pos]], /*backpic[2]*/ 2);
	}
	else 
	{
		update_widget_bg(p->eventbox2[p->pos1[p->pos]], /*backpic[0]*/ 0);
	}


	gtk_widget_modify_fg(p->label2[p->pos1[p->pos]], GTK_STATE_NORMAL, &yellow);

	return ;
}

/* 画三级菜单*/
void draw_menu3 (gint pa, gpointer pt)
{
	DRAW_UI_P p = p_drawui_c;
	int i;

	for (i = 0 ; i < 6 ; i++) 
	{
		gtk_widget_set_sensitive (p->eventbox30[i], TRUE);
		gtk_widget_set_sensitive (p->eventbox31[i], TRUE);

		if (CUR_POS_P(p) != i)
		{
			draw_data3[i](p);
		}
		gtk_widget_hide (pp->sbutton[i]);//此时会响应value changed信号，何解？不过也是多余的---lzk
	}
	draw_data3[CUR_POS_P(p)](p);
	tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[CUR_POS_P(p)]));

	p->menu3_amount = 6;
	/* 计算当前3级菜单活动的数量 */
	while (!(gtk_widget_get_visible(p->eventbox30[p->menu3_amount - 1])))
		p->menu3_amount--;
	while (!(gtk_widget_get_sensitive(p->eventbox30[p->menu3_amount - 1])))
	{
		if (p->menu3_amount > 1)
			p->menu3_amount--;
		else
			p->menu3_amount = 0;
	}

	return ;
}

/*
 * 处理 三级菜单弹出菜单的画图
 * fun   为回调函数 
 * set_menu_position 用来指定弹出菜单的位置
 * cur_value 为菜单当前数值
 * content 弹出菜单的字符串数组
 * qty 多少个弹出菜单
 * pos 这个菜单的位置
 * pop_pos 弹出菜单选中的位置
 * menu_status 标志当前menu_item是否可以选择 0-31 位 0是第一个 1是第二个....
 */

void draw3_pop_tt (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status)
{
	gint i, x, y, z;
	GtkMenu *menu;
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	update_widget_bg(pp->eventbox30[z], 6);
	for (i = 0; i < 64; i++) 
	{
		if (pp->menu_item3[i]) 
		{
			gtk_widget_destroy(pp->menu_item3[i]);
			pp->menu_item3[i] = NULL;
		}
		else 
			break;
	}

	for (i = 0; i < qty; i++ )
	{
		pp->menu_item3[i] = gtk_menu_item_new_with_label (content[i]);
		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu33[pos]), pp->menu_item3[i]);
		g_signal_connect (pp->menu_item3[i], "activate",
				G_CALLBACK(fun), (GUINT_TO_POINTER (i)));

		if (0x01 & (menu_status >> i))
			gtk_widget_set_sensitive (pp->menu_item3[i], FALSE);
		gtk_widget_show (pp->menu_item3[i]);
	}
	gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pop_pos]));

	menu = GTK_MENU (pp->menu33[pos]);
	gtk_menu_popup (menu, NULL, NULL, 
			(GtkMenuPositionFunc)set_menu_position_tdh,
			NULL,
			0,
			gtk_get_current_event_time());

	if (x == 0 && y == 2 && z == 3 & pp->cstart_qty == 3 && pp->cmode_pos == 0 &&pp->ctype_pos ==1)
	{
		x = 4;
		y = 3;
		z = 2;	
	}
	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	if (x == 4 && y == 3 && z == 2 & pp->cstart_qty == 3 && pp->cmode_pos == 0&&pp->ctype_pos == 1)
	{
	        x = pp->pos;
       		y = pp->pos1[x];
        	z = pos;		
	}	
	gtk_label_set_text (GTK_LABEL (pp->data3[pos]), cur_value);
	gtk_label_set_text (GTK_LABEL (pp->label3[pos]), str);
	if (str) {
		g_free (str);
    }
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[pos]);
	gtk_widget_show (pp->eventbox31[pos]);
	gtk_widget_show (pp->data3[pos]);
	gtk_widget_hide (pp->sbutton[pos]);

	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);

	pp->menu3_qty = qty;
	pp->menu3_poppos = pop_pos;

	return ;
}

void draw3_pop_tt_shen (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status)
{
	int i;
	GtkWidget *menu;
    GtkWidget *items[100] ;
	menu=gtk_menu_new();
    for(i = 0 ; i < qty ; i++)
    {
    	 items[i]=gtk_menu_item_new_with_label(content[i]);
         g_signal_connect(GTK_OBJECT(items[i]),"activate",GTK_SIGNAL_FUNC(fun),GUINT_TO_POINTER(i));
         gtk_menu_shell_append(GTK_MENU_SHELL(menu),items[i]);
         gtk_widget_show (items[i]);
    }
    gtk_menu_item_select (GTK_MENU_ITEM (items[pop_pos]));
    gtk_menu_popup  (GTK_MENU(menu),
					 NULL,
					 NULL,
					(GtkMenuPositionFunc)set_menu_position_tdh,
					 NULL,
					 0,
					 gtk_get_current_event_time());

    update_widget_bg(pp->eventbox30[pos], 6);
	pp->menu3_poppos = pop_pos;

	return ;
}
/* 
 * 三级菜单只做显示时函数
 * cur_value 为当前值 
 * pos 表示第几个位置
 * big_menu 表示当前栏没有数值 只有菜单 像set Ref set 80%..
 *
 */
void draw3_popdown (const gchar *cur_value, guint pos, guint big_menu)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	if (x == 0 && y == 2 && z == 3 & pp->cstart_qty == 3 && pp->cmode_pos == 0&&pp->ctype_pos ==1)
	{
		x = 4;
		y = 3;
		z = 2;	
	}
	//	gtk_menu_popdown( GTK_MENU (pp->menu3));
	gtk_menu_popdown( GTK_MENU (pp->menu33[pos]));
	if (big_menu)
		str = g_strdup_printf ("\n\n%s", con2_p[x][y][z]);	
	else
		str = g_strdup_printf ("%s", con2_p[x][y][z]);
	if (x == 4 && y == 3 && z == 2 & pp->cstart_qty == 3 && pp->cmode_pos == 0&&pp->ctype_pos == 1)
	{
	        x = pp->pos;
       		y = pp->pos1[x];
        	z = pos;		
	}	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	g_free(str);

	if ((CUR_POS == z) && (pp->pos_pos == MENU3_PRESSED))
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}
	}
	else if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}	
	}
	else
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[7]*/ 7);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[13]*/ 13);
		}
		else 
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[10]*/ 10);
		}
	}
	gtk_label_set_text (GTK_LABEL (pp->data3[z]), cur_value);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	gtk_widget_hide (pp->dialog);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));
	change_keypress_event (KEYPRESS_MAIN);

	return ;
}

/* 
 * 三级菜单只做显示时函数
 * cur_value 为当前值 
 * pos 表示第几个位置
 * big_menu 表示当前栏没有数值 只有菜单 像set Ref set 80%..
 * content_pos 表示一个菜单有多种名字的情况 与 pos不是一个值 为零时候不起作用
 */

void draw3_popdown_offset (const gchar *cur_value, guint pos, guint big_menu, guint content_pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	//	gtk_menu_popdown( GTK_MENU (pp->menu3));
	gtk_menu_popdown( GTK_MENU (pp->menu33[pos]));
	if (big_menu)
		str = g_strdup_printf ("\n\n%s", con2_p[x][y][content_pos ? content_pos : pos]);	
	else
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos]);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	g_free(str);

	if ((CUR_POS == z) && (pp->pos_pos == MENU3_PRESSED))
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}
	}
	else if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}
	}
	else
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[7]*/ 7);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[13]*/ 13);
		}
		else 
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[10]*/ 10);
		}
	}
	gtk_label_set_text (GTK_LABEL (pp->data3[z]), cur_value);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	gtk_widget_hide (pp->dialog);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));


	change_keypress_event (KEYPRESS_MAIN);

	return ;
}


/*
 * 弹出scale触摸条的函数
 * 处理 三级菜单按下状态的画图，这个处理数值按键的部分
 * fun   为回调函数 
 * cur_value 为菜单当前数值
 * lower 为菜单最小值
 * upper 为菜单最大值
 * step  为菜单步进
 * digit 为保留小数点数
 * pos 为第几个3级菜单
 * content_pos 表示一个菜单有多种名字的情况 与 pos不是一个值 为零时候不起作用
 * p 预留
 * 
 */
void draw3_digit_pressed (void (*fun)(GtkSpinButton*, gpointer), const gchar *unit,
		double cur_value, double lower, double upper, double step,
		guint digit, gpointer p, guint pos, guint content_pos)
{
	guint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	if (!unit )
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos] );	
	else
		str = g_strdup_printf ("%s\n%s Δ%0.*f", 
				con2_p[x][y][content_pos ? content_pos : pos], unit, digit, step);	/* %*.*f 可以指点位数 */		

	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
	//	widget_window_class->key_press_event = window_keypress_event_orig;
	/* 一个信号能对应多个回调函数，所以先把对应的回调函数取消 */
	if (g_signal_handler_is_connected (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]))
		g_signal_handler_disconnect (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]);
	pp->signal_id[z] = g_signal_connect (G_OBJECT(pp->sbutton[z]), "value-changed", G_CALLBACK(fun), (gpointer) (pp));
	/* 设置值的范围 */
	pp->adj = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (pp->sbutton[z]));
	gtk_adjustment_configure (pp->adj, cur_value, lower, upper, step , 10.0, 0.0);
	gtk_spin_button_set_digits (GTK_SPIN_BUTTON (pp->sbutton[z]), digit);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_hide (pp->eventbox31[z]);
	gtk_widget_show (pp->sbutton[z]);
	gtk_widget_grab_focus (pp->sbutton[z]);
	/* 设置scale */
	gtk_range_set_adjustment (GTK_RANGE (pp->vscale), pp->adj);
	gtk_range_set_inverted (GTK_RANGE (pp->vscale), TRUE);
	gtk_scale_set_draw_value (GTK_SCALE (pp->vscale), FALSE);

	gtk_widget_hide (pp->scale_drawarea);
	gtk_widget_show (pp->button_add);
	gtk_widget_show (pp->button_sub);
	gtk_widget_show (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));

	if (str) {
		g_free(str);
    }

	change_keypress_event (KEYPRESS_MAIN_SPINBUTTON);
	//	pp->pos_pos=MENU3_STOP;

	return ;
}

/* 
 *
 */
void draw3_digit_stop(double cur_value, const gchar *unit,
		guint digit, guint pos, guint content_pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	if (g_signal_handler_is_connected (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]))
		g_signal_handler_disconnect (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]);
	if (!unit )
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos]);	
	else
		str = g_strdup_printf ("%s\n%s", con2_p[x][y][content_pos ? content_pos : pos], unit);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	g_free(str);

	gtk_label_set_justify (GTK_LABEL(pp->label3[z]),GTK_JUSTIFY_CENTER);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[8]*/ 8);
		update_widget_bg(pp->eventbox31[z], /*backpic[11]*/ 11);
	}
	else
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[7]*/ 7);
		update_widget_bg(pp->eventbox31[z], /*backpic[10]*/ 10);
	}
	/* 更新当前增益值显示 */
	str = g_strdup_printf ("%0.*f", digit, cur_value);

	gtk_label_set_text (GTK_LABEL (pp->data3[z]), str);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[z]), cur_value);
	/* 显示和隐藏控件 */
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[z]), cur_value);
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);

	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);

	change_keypress_event (KEYPRESS_MAIN);

	return ;
}

void RefreshPrfMark()
{
    char* markup ;
    char* markup1;
	int _nValue  ;
	_nValue = GROUP_VAL_POS (0, prf1) / 10  ;
    markup=g_markup_printf_escaped(
			"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>", _nValue,
			_nValue * get_beam_qty());
    double _nValue1 = get_area_scanresolution (pp->p_config)/ 1000.0  ;
    _nValue1 *= _nValue ;
	markup1 = g_markup_printf_escaped (
			"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>", _nValue1);
	//gdk_threads_enter();
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup1);
	//gdk_threads_leave();
	g_free(markup);
	g_free(markup1);

}

void DrawGate_a(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	gint w,h;
	int  j ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	gint grp = prule->group;//当前group
	gtk_widget_get_size_request (widget, &w, &h);
	/* mask = 二进制10000 则不显示 label  15 pixels*/
	/* mask = 二进制01000 则不显示 hruler1 20 pixels*/
	/* mask = 二进制00100 则不显示 调色条   10 pixels*/
	/* mask = 二进制00010 则不显示 hruler2 20 pixels*/
	/* mask = 二进制00001 则不显示 wruler1  20 pixels*/
	if(!(prule->mask & 0x01)) h  = h - 20 ;
	if(!(prule->mask & 0x02)) w  = w - 20  ;
	if(!(prule->mask & 0x04)) w  = w - 10  ;
	if(!(prule->mask & 0x08)) w  = w - 20  ;

	int offset;
	for (offset = 0, j = 0 ; j < grp; j++)
		offset += TMP(beam_qty[j]);
	int _nBeamNo =  TMP(beam_num[grp]);
	int index = offset + _nBeamNo;

	double draw_height ;
	double draw_start = 0.5 , draw_end = -0.5  ;

	double gate_height = GROUP_VAL_POS(grp , gate[0].height) ;
	double gate_width  = GROUP_VAL_POS(grp , gate[0].width) / 1000.0;
	double gate_start  = GROUP_VAL_POS(grp , gate[0].start) / 1000.0;

    double sample_start= group_get_start (grp)/ 1000.0 ;
    double sample_range= group_get_range(grp)/ 1000.0 ;
	// ***************************************************
	// this value is setting temporary
	// ***************************************************
	unsigned char UtUnit = GROUP_VAL_POS ( grp , ut_unit ) ;
	int    _nCurrentBeamDelay ;
	double _nGatePosition ;
	double min_angle ;
	double max_angle ;

	static const double dashed1[] ={2.0, 2.0 };
	cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A red */
	switch (prule->scan_type)
	{
		case	A_SCAN:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_start *= cos(TMP(current_angle[grp]) );
				sample_range *= cos(TMP(current_angle[grp]) );
			}

			if( GROUP_VAL_POS(grp ,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
			{
				draw_start  +=  (int)((gate_start - sample_start) / sample_range * w + 20 );
				draw_end    +=  (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}

			if(GROUP_VAL_POS(grp , rectifier1))
			{
				draw_height = (1 - gate_height / 100.0)* h ;
			}
			else
			{
				draw_height = h * gate_height / 200.0 ;
				h /= 2.0 ;
				draw_height = h - draw_height ;
			}
			cairo_set_line_width(cr, 2);
			cairo_move_to(cr, draw_start , draw_height );
			cairo_line_to(cr, draw_end   , draw_height );
			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);
			cairo_move_to(cr, draw_start , draw_height - 3 );
			cairo_line_to(cr, draw_start , draw_height + 3 );
			cairo_move_to(cr,  draw_end  , draw_height - 3 );
			cairo_line_to(cr,  draw_end  , draw_height + 3 );
			cairo_stroke(cr);
			prule->scanEx.gateDataA.height = draw_height;
			prule->scanEx.gateDataA.start = draw_start - 20;
			prule->scanEx.gateDataA.end = draw_end - 20;
			break ;
		case	A_SCAN_R:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;
			}

			if(  GROUP_VAL_POS(grp ,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
			{
				draw_start  += (int)((gate_start - sample_start) / sample_range * h );
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h );
			}
			else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay /10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  +=  (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
				draw_end    +=  (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h) ;
			}

			if(GROUP_VAL_POS(grp , rectifier1))
			{
				draw_height = (gate_height / 100.0) * w + 20 ;
			}
			else
			{
				draw_height = w * gate_height / 200.0 ;
				w = w / 2.0 + 20   ;
				draw_height += w  ;
			}
			cairo_set_line_width(cr, 2);
		    cairo_move_to(cr, draw_height , draw_start );
		    cairo_line_to(cr, draw_height , draw_end   );
			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);
		    cairo_move_to(cr, draw_height - 3 , draw_start );
		    cairo_line_to(cr, draw_height + 3 , draw_start );
		    cairo_move_to(cr, draw_height - 3 , draw_end   );
		    cairo_line_to(cr, draw_height + 3 , draw_end   );
		    prule->scanEx.gateDataA.height = draw_height - 20;
			prule->scanEx.gateDataA.start = draw_start;
			prule->scanEx.gateDataA.end = draw_end;
			break;
		case	B_SCAN:
		case	B_SCAN_VERT:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_start *= cos(TMP(current_angle[grp]) );
				sample_range *= cos(TMP(current_angle[grp]) );
			}
			if( GROUP_VAL_POS(grp , gate[0].synchro) == 0 )/* A闸门 pulse 时 */
			{
				draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			cairo_move_to (cr , draw_start , 0) ;
			cairo_line_to (cr , draw_start , h) ;
			cairo_move_to (cr , draw_end   , 0) ;
			cairo_line_to (cr , draw_end   , h) ;
			break;
		case    TOFD_B_SCAN:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if (UtUnit != UT_UNIT_SOUNDPATH )
			{
				min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				sample_range *= cos(min_angle) ;
				sample_start *= cos(min_angle);
			}
			if( GROUP_VAL_POS(grp , gate[0].synchro) == 0 )/* A闸门 pulse 时 */
			{
				draw_start  = (int)((gate_start - sample_start) / sample_range * h );
				draw_end    = (int)((gate_start - sample_start + gate_width )/ sample_range * h) ;
			}
			else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) +  GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h) ;
			}
			cairo_move_to (cr , 20 ,    draw_start ) ;
			cairo_line_to (cr , w + 20 ,draw_start ) ;
			cairo_move_to (cr , 20 ,    draw_end   ) ;
			cairo_line_to (cr , w + 20 ,draw_end   ) ;
			break ;
		case	S_SCAN:
		case	S_SCAN_A:
		case	S_SCAN_L:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if (UtUnit == UT_UNIT_SOUNDPATH || UtUnit == UT_UNIT_TIME)
			{
				if( GROUP_VAL_POS(grp , gate[0].synchro) == 0 )/* A闸门 pulse 时 */
				{
					draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
					draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);

				}
				else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
				{
					_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
					_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
					_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
					draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20 );
					draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20 );
				}
				cairo_move_to (cr , draw_start, 0) ;
				cairo_line_to (cr , draw_start, h) ;
				cairo_move_to (cr , draw_end  , 0) ;
				cairo_line_to (cr , draw_end  , h) ;
			}
			else	/* TRUE_DEPTH */
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;

				if( GROUP_VAL_POS(grp , gate[0].synchro) == 0 )/* A闸门 pulse 时 */
				{
					draw_start  +=  (int)((gate_start - sample_start) / sample_range * h );
					draw_end    +=  (int)((gate_start - sample_start + gate_width )/ sample_range * h) ;
				}
				else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
				{
					_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
					_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10 );
					_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
					draw_start  +=  (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
					draw_end    +=  (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h );
				}
				cairo_move_to (cr , 20 ,    draw_start ) ;
				cairo_line_to (cr , w + 20 ,draw_start ) ;
				cairo_move_to (cr , 20 ,    draw_end   ) ;
				cairo_line_to (cr , w + 20 ,draw_end   ) ;
			}
			break;
		case A_SCAN_CLB:
			cairo_set_line_width(cr, 2);
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;
			}
			if( GROUP_VAL_POS(grp , gate[0].synchro) == 0 )/* A闸门 pulse 时 */
			{
				draw_start  +=  (int)((gate_start - sample_start) / sample_range * w + 20);
				draw_end    +=  (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(grp , gate[0].synchro) == 1 )/* A闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			draw_height = (1 - gate_height / 100.0)* h ;

			cairo_move_to(cr, draw_start , draw_height );
			cairo_line_to(cr, draw_end   , draw_height );

			cairo_move_to(cr, draw_start , draw_height - 3 );
			cairo_line_to(cr, draw_start , draw_height + 3 );

			cairo_move_to(cr,  draw_end  , draw_height - 3 );
			cairo_line_to(cr,  draw_end  , draw_height + 3 );
			break ;
		default:
			break;
	}

	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawGate_b(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	int  j ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	int grp = prule->group ;//当前group
	gtk_widget_get_size_request (widget, &w, &h);
	/* mask = 二进制10000 则不显示 label  15 pixels*/
	/* mask = 二进制01000 则不显示 hruler1 20 pixels*/
	/* mask = 二进制00100 则不显示 调色条   10 pixels*/
	/* mask = 二进制00010 则不显示 hruler2 20 pixels*/
	/* mask = 二进制00001 则不显示 wruler1  20 pixels*/
	if(!(prule->mask & 0x01)) h  = h - 20 ;
	if(!(prule->mask & 0x02)) w = w - 20  ;
	if(!(prule->mask & 0x04)) w = w - 10  ;
	if(!(prule->mask & 0x08)) w = w - 20  ;

	int offset;
	for (offset = 0, j = 0 ; j < grp; j++)
		offset += TMP(beam_qty[j]);
	int _nBeamNo = TMP(beam_num[grp]);
	int index = offset + _nBeamNo    ;

	double draw_height ;
	double draw_start = 0.5, draw_end = -0.5   ;

	double gate_height = GROUP_VAL_POS(grp , gate[1].height) ;
	double gate_width  = GROUP_VAL_POS(grp , gate[1].width) / 1000.0;
	double gate_start  = GROUP_VAL_POS(grp , gate[1].start) / 1000.0;
    double sample_start= group_get_start(grp)/ 1000.0 ;
    double sample_range= group_get_range(grp)/ 1000.0 ;
	unsigned char UtUnit       = GROUP_VAL_POS(grp , ut_unit)  ;
	int _nCurrentBeamDelay ;
	double _nGatePosition ;
	double min_angle ;
	double max_angle ;
	cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色 */
	static const double dashed1[] ={2.0, 2.0 };
	switch (prule->scan_type)
	{
		case	A_SCAN:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_start *= cos(TMP(current_angle[grp]) );
				sample_range *= cos(TMP(current_angle[grp]) );
			}
			if( GROUP_VAL_POS(grp , gate[1].synchro) == 0 )/* B闸门 pulse 时 */
			{
				draw_start  +=  (int)((gate_start - sample_start) / sample_range * w + 20);
				draw_end    +=  (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 1 )/* B闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition     = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition     = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 2 )/* B闸门 ^A/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}

			if(GROUP_VAL_POS(grp ,rectifier1))
			{
				draw_height = (1 - gate_height / 100.0)* h ;
			}
			else
			{
				draw_height = h * gate_height / 200.0 ;
				h /= 2.0 ;
				draw_height = h - draw_height  ;
			}
			cairo_set_line_width(cr, 2);
			cairo_move_to(cr, draw_start , draw_height );
			cairo_line_to(cr, draw_end   , draw_height );
			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);
			cairo_move_to(cr, draw_start , draw_height - 3 );
			cairo_line_to(cr, draw_start , draw_height + 3 );
			cairo_move_to(cr,  draw_end  , draw_height - 3 );
			cairo_line_to(cr,  draw_end  , draw_height + 3 );
			cairo_stroke(cr);
			break ;
		case	A_SCAN_R:
			if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;
			}

			if(  GROUP_VAL_POS(grp , gate[1].synchro) == 0 )/* B闸门 pulse 时 */
			{
				draw_start  += (int)((gate_start - sample_start) / sample_range * h );
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h );
			}
			else if( GROUP_VAL_POS(grp ,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition     = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition     = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  +=  (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
				draw_end    +=  (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h );
			}
			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 2 )/* B闸门 ^A/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  +=  (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
				draw_end    +=  (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h );
			}

			if(GROUP_VAL_POS(grp ,rectifier1) == 0)
			{
				draw_height = w * gate_height / 200.0 ;
				w = w / 2.0 + 20   ;
				draw_height = w + draw_height ;
			}
			else
			{
				draw_height = (gate_height / 100.0)* w + 20 ;

			}
			cairo_set_line_width(cr, 2);
			cairo_move_to(cr, draw_height , draw_start );
			cairo_line_to(cr, draw_height , draw_end   );

			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);

			cairo_move_to(cr, draw_height - 3 , draw_start );
			cairo_line_to(cr, draw_height + 3 , draw_start );
			cairo_move_to(cr, draw_height - 3 , draw_end   );
			cairo_line_to(cr, draw_height + 3 , draw_end   );

			break;
		case	B_SCAN:
		case	B_SCAN_VERT:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_start *= cos(TMP(current_angle[grp]) );
				sample_range *= cos(TMP(current_angle[grp]) );
			}

			if( GROUP_VAL_POS(grp ,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
			{
				draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}

			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 1 )/* B闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition     = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition     = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 2 )/* B闸门 ^A/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			cairo_move_to (cr , draw_start , 0) ;
			cairo_line_to (cr , draw_start , h) ;
			cairo_move_to (cr , draw_end   , 0) ;
			cairo_line_to (cr , draw_end   , h) ;
			break;
		case    TOFD_B_SCAN:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if (UtUnit != UT_UNIT_SOUNDPATH )
			{
				min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				sample_range *= cos(min_angle) ;
				sample_start *= cos(min_angle);
			}
			if( GROUP_VAL_POS(grp , gate[1].synchro) == 0 )/* B闸门 pulse 时 */
			{
				draw_start  += (int)((gate_start - sample_start) / sample_range * h) ;
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h) ;
			}
			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 1 )/* B闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * h) ;
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h) ;
			}
			else if( GROUP_VAL_POS(grp , gate[1].synchro) == 2 )/* B闸门 ^A 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay / 10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h );
			}
			cairo_move_to (cr , 20 ,    draw_start ) ;
			cairo_line_to (cr , w + 20 ,draw_start ) ;
			cairo_move_to (cr , 20 ,    draw_end   ) ;
			cairo_line_to (cr , w + 20 ,draw_end   ) ;
			break ;
		case	S_SCAN:
		case	S_SCAN_A:
		case	S_SCAN_L:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if (UtUnit == UT_UNIT_SOUNDPATH || UtUnit == UT_UNIT_TIME)
			{
				if( GROUP_VAL_POS(grp , gate[1].synchro) == 0 )/* B闸门 pulse 时 */
				{
					draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
					draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);

				}
				else if( GROUP_VAL_POS(grp ,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
				{
					_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
					_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
					_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
					draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
					draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
				}
				else if( GROUP_VAL_POS(grp , gate[1].synchro) == 2 )/* B闸门 ^A/ 时 */
				{
					_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
					_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay /10);
					_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
					draw_start  = (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
					draw_end    = (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
				}

				cairo_move_to (cr , draw_start , 0) ;
				cairo_line_to (cr , draw_start , h) ;
				cairo_move_to (cr , draw_end   , 0) ;
				cairo_line_to (cr , draw_end   , h) ;
			}
			else	/* TRUE_DEPTH */
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;

				if( GROUP_VAL_POS(grp , gate[1].synchro) == 0 )/* B闸门 pulse 时 */
				{
					draw_start  += (int)((gate_start - sample_start) / sample_range * h) ;
					draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h) ;
				}
				else if( GROUP_VAL_POS(grp , gate[1].synchro) == 1 )/* B闸门 I/ 时 */
				{
					_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
					_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay / 10);
					_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
					draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
					draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h );
				}
				else if( GROUP_VAL_POS(grp , gate[1].synchro) == 2 )/* B闸门 ^A 时 */
				{
					_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) + GROUP_VAL_POS(grp , wedge_delay);
					_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay / 10);
					_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
					draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * h );
					draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * h );
				}
				cairo_move_to (cr , 20 ,    draw_start ) ;
				cairo_line_to (cr , w + 20 ,draw_start ) ;
				cairo_move_to (cr , 20 ,    draw_end   ) ;
				cairo_line_to (cr , w + 20 ,draw_end   ) ;
			}
			break;
		case A_SCAN_CLB:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;
			}
			if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
			{
				draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][3])) & 0xfffff) - _nCurrentBeamDelay/10);
				_nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 2 )/* B闸门 ^A/ 时 */
			{
				_nCurrentBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo])  + GROUP_VAL_POS(grp , wedge_delay);
				_nGatePosition    = (double)(((TMP(measure_data[index][1])) & 0xfffff) - _nCurrentBeamDelay / 10);
			    _nGatePosition    = _nGatePosition * cos(TMP(current_angle[grp])) / 100 ;
				draw_start  += (int)((_nGatePosition + gate_start - sample_start) / sample_range * w + 20);
				draw_end    += (int)((_nGatePosition + gate_start - sample_start + gate_width )/ sample_range * w + 20);
			}
			cairo_set_line_width(cr, 2);
			draw_height = (1 - gate_height / 100.0)* h ;
			cairo_move_to(cr, draw_start , draw_height );
			cairo_line_to(cr, draw_end   , draw_height );
			cairo_stroke (cr);
			cairo_set_line_width(cr, 1);
			cairo_move_to(cr, draw_start , draw_height - 3 );
			cairo_line_to(cr, draw_start , draw_height + 3 );
			cairo_move_to(cr,  draw_end  , draw_height - 3 );
			cairo_line_to(cr,  draw_end  , draw_height + 3 );
			cairo_stroke(cr);
			break;
		default:
			break;
	}
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawGate_i(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	int  j ;

	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	int grp = prule->group ;
	gtk_widget_get_size_request (widget, &w, &h);
	if(!(prule->mask & 0x01)) h  = h - 20 ;
	if(!(prule->mask & 0x02)) w = w - 20  ;
	if(!(prule->mask & 0x04)) w = w - 10  ;
	if(!(prule->mask & 0x08)) w = w - 20  ;

	int offset;
	for (offset = 0, j = 0 ; j < grp; j++)
		offset += TMP(beam_qty[j]);
	double draw_height ;
	double draw_start = 0.5 , draw_end = -0.5   ;
	double gate_height = GROUP_VAL_POS(grp , gate[2].height) ;
	double gate_width  = GROUP_VAL_POS(grp , gate[2].width) / 1000.0;
	double gate_start  = GROUP_VAL_POS(grp , gate[2].start) / 1000.0;
    double sample_start= group_get_start(grp)/ 1000.0 ;
    double sample_range= group_get_range(grp)/ 1000.0 ;
	unsigned char UtUnit       = GROUP_VAL_POS(grp , ut_unit)  ;
	//unsigned char DisplayGroup = get_display_group(pp->p_config) ;
	double min_angle ;
	double max_angle ;
	cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* I闸门为黄色 */
	static const double dashed1[] ={2.0, 2.0 };
	switch (prule->scan_type)
	{
		case	A_SCAN:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_start *= cos(TMP(current_angle[grp]) );
				sample_range *= cos(TMP(current_angle[grp]) );
			}
			draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
			draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);

			if(GROUP_VAL_POS(grp , rectifier1))
			{
				draw_height = (1 - gate_height / 100.0)* h ;

			}
			else
			{
				draw_height = h * gate_height / 200.0 ;
				h /= 2.0 ;
				draw_height = h - draw_height ;
			}
			cairo_set_line_width(cr, 2);
			cairo_move_to(cr,  draw_start , draw_height );
			cairo_line_to(cr,  draw_end   , draw_height );
			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);
			cairo_move_to(cr,  draw_start , draw_height - 3 );
			cairo_line_to(cr,  draw_start , draw_height + 3 );
			cairo_move_to(cr,  draw_end  , draw_height - 3 );
			cairo_line_to(cr,  draw_end  , draw_height + 3 );
			cairo_stroke (cr);
			break ;
		case	A_SCAN_R:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;
			}

			draw_start  += (int)((gate_start - sample_start) / sample_range * h );
			draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h );

			if(GROUP_VAL_POS(grp, rectifier1))
			{
				draw_height = (gate_height / 100.0)* w + 20 ;

			}
			else
			{
				draw_height = w * gate_height / 200.0 ;
				w = w / 2.0 + 20   ;
				draw_height += w ;
			}
			cairo_set_line_width(cr, 2);
			cairo_move_to(cr, draw_height , draw_start );
			cairo_line_to(cr, draw_height , draw_end   );
			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);
			cairo_move_to(cr, draw_height - 3 , draw_start );
			cairo_line_to(cr, draw_height + 3 , draw_start );
			cairo_move_to(cr, draw_height - 3 , draw_end   );
			cairo_line_to(cr, draw_height + 3 , draw_end   );
			cairo_stroke (cr);
			break;
		case	B_SCAN:
		case	B_SCAN_VERT:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if(GROUP_VAL_POS(grp, ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_start *= cos(TMP(current_angle[grp]) );
				sample_range *= cos(TMP(current_angle[grp]) );
			}
			draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
			draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);

			cairo_move_to (cr , draw_start , 0) ;
			cairo_line_to (cr , draw_start , h) ;
			cairo_move_to (cr , draw_end   , 0) ;
			cairo_line_to (cr , draw_end   , h) ;
			break;
		case    TOFD_B_SCAN:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if (UtUnit != UT_UNIT_SOUNDPATH )
			{
				min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				sample_range *= cos(min_angle) ;
				sample_start *= cos(min_angle);
			}
			draw_start  += (int)((gate_start - sample_start) / sample_range * h );
			draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h) ;
			cairo_move_to (cr , 20 ,    draw_start) ;
			cairo_line_to (cr , w + 20 ,draw_start) ;
			cairo_move_to (cr , 20 ,    draw_end  ) ;
			cairo_line_to (cr , w + 20 ,draw_end  ) ;
			break ;
		case	S_SCAN:
		case	S_SCAN_A:
		case	S_SCAN_L:
			cairo_set_dash (cr, dashed1, 2, 0);
			cairo_set_line_width(cr, 1);
			if (UtUnit == UT_UNIT_SOUNDPATH || UtUnit == UT_UNIT_TIME)
			{
				draw_start  = (int)((gate_start - sample_start) / sample_range * w + 20);
				draw_end    = (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);
				cairo_move_to (cr , draw_start + 0.5, 0) ;
				cairo_line_to (cr , draw_start + 0.5, h) ;
				cairo_move_to (cr , draw_end   + 0.5, 0) ;
				cairo_line_to (cr , draw_end   + 0.5, h) ;
			}
			else	/* TRUE_DEPTH */
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range -= sample_start ;
				draw_start  += (int)((gate_start - sample_start) / sample_range * h );
				draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * h );
				cairo_move_to (cr , 20 ,    draw_start ) ;
				cairo_line_to (cr , w + 20 ,draw_start ) ;
				cairo_move_to (cr , 20 ,    draw_end   ) ;
				cairo_line_to (cr , w + 20 ,draw_end   ) ;
			}
			break;
		case A_SCAN_CLB:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range  -= sample_start ;
			}
			draw_height = (1 - gate_height / 100.0)* h ;
			draw_start  += (int)((gate_start - sample_start) / sample_range * w + 20);
			draw_end    += (int)((gate_start - sample_start + gate_width )/ sample_range * w + 20);

			cairo_set_line_width(cr, 2);
			cairo_move_to(cr, draw_start , draw_height );
			cairo_line_to(cr, draw_end   , draw_height );
			cairo_stroke(cr);
			cairo_set_line_width(cr, 1);
			cairo_move_to(cr, draw_start , draw_height - 3 );
			cairo_line_to(cr, draw_start , draw_height + 3 );
			cairo_move_to(cr,  draw_end  , draw_height - 3 );
			cairo_line_to(cr,  draw_end  , draw_height + 3 );
			cairo_stroke(cr);
			break;
		default:
			break;
	}
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawGate(GtkWidget *widget,  gpointer data, cairo_t *cr)
{
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char mask = 1 << prule->group ;

    if (TMP(gateShow)[2] & mask) {
	    DrawGate_i(widget , data , cr) ;
    }
    if (TMP(gateShow)[1] & mask) {
	    DrawGate_b(widget , data , cr) ;
    }
    if (TMP(gateShow)[0] & mask) {
	    DrawGate_a(widget , data , cr) ;
    }
}


void DrawColorBar(GtkWidget* widget , gpointer data , cairo_t *cr)
{
	int w, h, i  ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	double color_r = 0, color_g = 0, color_b = 0;
	unsigned short* pColor;
	unsigned int grp = prule->group ;

	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark ;
	_nMark = prule->mask  ;
	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	// set color: RF model should be gray
	pColor = TMP(color_amp)  ;
	switch (prule->scan_type)
	{
		case	A_SCAN:
		case    A_SCAN_CLB:
		case	A_SCAN_R:
		case	B_SCAN:
		case	B_SCAN_VERT:
		case	S_SCAN:
		case	S_SCAN_A:
		case    S_SCAN_L:
		case WEDGE_DELAY:
		case SENSITIVITY:
		case         TCG:
		case TOFD_B_SCAN:
		case FFT_A_SCAN:
			if(pp->p_config->group[grp].rectifier1)
				pColor = TMP(color_amp)  ;
			else
				pColor = TMP(color_tofd) ;
			break;
		case	C_SCAN:
		case	CCC_SCAN:
			switch (get_cscan_source(pp->p_config, 0))
			{
				case C_SCAN_A_HEIGHT:
				case C_SCAN_B_HEIGHT:
					if(pp->p_config->group[grp].rectifier1)
						pColor = TMP(color_amp)  ;
					else
						pColor = TMP(color_tofd) ;
					break;
				case C_SCAN_THICKNESS:
				case C_SCAN_I_GATE:
					pColor = TMP(color_depth) ;
					break;
				default:
					break;
			}
			break;
		case	CC_SCAN:
			switch (get_cscan_source(pp->p_config, 1))
			{
				case C_SCAN_A_HEIGHT:
				case C_SCAN_B_HEIGHT:
					if(pp->p_config->group[grp].rectifier1)
						pColor = TMP(color_amp)  ;
					else
						pColor = TMP(color_tofd) ;
					break;
				case C_SCAN_THICKNESS:
				case C_SCAN_I_GATE:
					pColor = TMP(color_depth) ;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
	cairo_set_line_width(cr, 1);
	cairo_rectangle (cr, w-30, 0, 10, h-20);
	cairo_fill (cr);
    h = h - 20 ;
	for ( i = 0; i < h ; i++ )
	{
		color_r = ((pColor[i * 255 / h] >> 11)) / 32.0;
		color_g = ((pColor[i * 255 / h] & 0x07e0) >> 5) / 64.0;
		color_b = ((pColor[i * 255 / h] & 0x1f)) /  32.0;
		cairo_set_source_rgba (cr, color_r, color_g, color_b, 1.0);
		cairo_move_to (cr, w - 29, h - i - 0.5);
		cairo_line_to (cr, w - 21, h - i - 0.5);
		cairo_stroke (cr);
	}
}

void DrawOverLay(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	gint w,h;
	int i , j ;
	char* str= NULL ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;

	gtk_widget_get_size_request (widget, &w, &h);
	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;

	double min_angle ;
	double max_angle ;
	double current_pos ;
	double thickness = GROUP_VAL_POS(grp , part.Thickness) / 1000.0  ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0;
	double velocity = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) / 200000.0;
	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	cairo_set_line_width (cr, 0.5);
	switch (prule->scan_type)
	{

		case A_SCAN:
		case B_SCAN:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				sample_range *= cos_current_angle ;
				sample_start *= cos_current_angle ;
			}
			if((GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_SOUNDPATH)||(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH))
			{
				for(i = 1 ;  ((i * thickness - sample_start * velocity) < sample_range * velocity ) && (i <= 10 ) ; i++)
				{
					cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
					current_pos = 20 + (w - 50) * (i * thickness - sample_start * velocity) / (sample_range * velocity) ;
					current_pos = (int)current_pos + 0.5 ;
					if(current_pos <= 20) continue  ;
					for(j=0;j<h-20;j+=5)
					{
						cairo_move_to (cr, current_pos, j );
						cairo_line_to (cr, current_pos, j + 2.5);
					}
					cairo_stroke (cr);
					cairo_set_source_rgba(cr, 0.84, 0.67, 0.945, 1.0);//显示当前值的紫色框
					cairo_rectangle(cr, current_pos + 1 , 1 , 20 , 13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);//当前值
					cairo_move_to(cr,  current_pos + 3 , 11 );
					if(i%2)
						str=g_strdup_printf("B%d",i);
					else
						str=g_strdup_printf("T%d",i);
					cairo_show_text(cr,str);
					cairo_stroke(cr);
					g_free (str);
				}
			}
			break;
		case A_SCAN_R:
		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:
			if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_SOUNDPATH)
			{
				for(i=1; ((i * thickness - sample_start * velocity) < (sample_range * velocity)) && (i <= 10 ); i++)
				{
					current_pos = 20 + (w - 50) * (i * thickness - sample_start * velocity) / (sample_range * velocity) ;	
					current_pos = (int)current_pos + 0.5 ;
					if(current_pos <= 20) continue  ;
					for(j=0;j<h-20;j+=5)
					{
						cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
						cairo_move_to (cr, current_pos, j);
						cairo_line_to (cr, current_pos, j+2.5);
					}
					cairo_stroke (cr);
					cairo_set_source_rgba(cr, 0.84, 0.67, 0.945, 1.0);//显示当前值的紫色框
					cairo_rectangle(cr, current_pos + 1 , 1 , 20 , 13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);//当前值
					cairo_move_to(cr,  current_pos + 3 , 11 );
					if(i%2)
						str=g_strdup_printf("B%d",i);
					else
						str=g_strdup_printf("T%d",i);
					cairo_show_text(cr,str);
					cairo_stroke(cr);
					g_free (str);
				}
			}
			else if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
				}
				sample_range  = (sample_start + sample_range) * cos(min_angle);
				sample_start  *= cos(max_angle);
				sample_range  -= sample_start  ;

				for(i=1; ((i * thickness - sample_start * velocity) < sample_range * velocity) && (i <= 10) ; i++)
				{
					current_pos = (h - 20) * (i * thickness - sample_start * velocity) / (sample_range * velocity )  ;
					current_pos = (int)current_pos + 0.5 ;
					if(current_pos <= 0) continue  ;
					for(j=20;j<w-30;j+=5)
					{
						cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
						cairo_move_to (cr, j, current_pos);
						cairo_line_to (cr, j+2.5 ,current_pos);
					}
					cairo_stroke (cr);
					cairo_set_source_rgba(cr, 0.84, 0.67, 0.945, 1.0);//显示当前值的紫色框
					cairo_rectangle(cr, 21 , current_pos + 1 , 20 , 13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);//当前值
					cairo_move_to(cr,  23 , current_pos + 12 );
					if(i%2)
						str=g_strdup_printf("B%d",i);
					else
						str=g_strdup_printf("T%d",i);
					cairo_show_text(cr,str);
					cairo_stroke(cr);
					g_free (str);
				}
			}
			break;
		case A_SCAN_CLB:
			break ;
		default:
			break ;
	}
}

static void DrawWorkpiece(GtkWidget *widget, gpointer data, cairo_t *cr)
{
    DRAW_AREA_P prule = (DRAW_AREA_P)(data);
    gdouble beginX = 0.0;
    gdouble endX = 0.0;
    gdouble beginY = 0.0;
    gdouble endY = 0.0;
    gint width = 0;
    gint height = 0;

    /*获取部件长宽度*/
    gtk_widget_get_size_request(widget, &width, &height);

    if (G_UNLIKELY(prule->wmax1 == prule->wmin1)) {
        return;
    } else if (prule->wmax1 < prule->wmin1) {
        beginX = prule->wmax1;
        endX = prule->wmin1;
    } else {
        beginX = prule->wmin1;
        endX = prule->wmax1;
    }

    if (G_UNLIKELY(prule->hmax1 == prule->hmin1)) {
        return;
    } else if (prule->hmax1 < prule->hmin1) {
        beginY = prule->hmax1;
        endY = prule->hmin1;
    } else {
        beginY = prule->hmin1;
        endY = prule->hmax1;
    }

    workpiece_set_basepoint(prule->group,
                            -(width-20-20-10)/(endX-beginX) * beginX + 20, //这里的20为左右标尺的宽度,10为调色条宽度; 该数值参见DrawBottomRuler函数的
                            -(height-20)/(endY-beginY) * beginY);
    workpiece_paint(widget , prule , cr);
}

void DrawWeldShapeI(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h , i;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;

	gtk_widget_get_size_request (widget, &w, &h);
	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	w = w - 50 ;
	h = h - 20 ;

	double min_angle ;
	double max_angle ;
	double current_pos ;
	double thickness    = GROUP_VAL_POS(grp , part.Thickness) / 1000.0  ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double velocity     = GROUP_VAL_POS(grp  , velocity) / 200000.0;

	st_PART* p = &(GROUP_VAL_POS(grp , part)) ;
	int _nSymmetry = p->symmetry ;

	double _nWelandOffset  = p->weland_offset /1000.0 ;
	double _nWelandHeight  = p->weland_height /1000.0 ;

	double _nTmpX1 , _nTmpX2 ;
	double _nTmpY1 , _nTmpY2 ;
	double _nThickness2 ;

	double _nRulerStart = prule->wmin1   ;
	double _nRulerStop  = prule->wmax1   ;
	double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;

	cairo_set_line_width (cr, 0.5);
	static const double dashed1[] ={5.0, 2.0 , 5.0};
	cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
	current_pos = (int)_nZeroPosition + 0.5  ;
	if(current_pos >= 20 && current_pos <= w + 20)
	{

		cairo_set_dash (cr, dashed1, 3, 0);
		cairo_move_to (cr, current_pos  , 0  );
		cairo_line_to (cr, current_pos  , h  );
		cairo_stroke(cr);
		cairo_set_dash (cr, NULL, 0, 0);
		cairo_stroke(cr);
	}

	if(LAW_VAL_POS(grp, Focal_type) == 0)
	{
		if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
			min_angle = 0.0;
		else
			min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
	}
	else
	{
		max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
	}
	sample_range  = (sample_start + sample_range) * cos(min_angle);
	sample_start  *= cos(max_angle);
	sample_range  -= sample_start  ;
	sample_range  *= velocity  ;
	sample_start  *= velocity  ;

	_nTmpX1 = w * _nWelandOffset / (_nRulerStop - _nRulerStart) ;
	_nTmpY1 = h *  _nWelandHeight / sample_range  ;
	for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
	{
		current_pos = h * (i * thickness - sample_start ) / sample_range  ;
		_nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
		if(current_pos < 0 && _nThickness2 < 0) continue  ;
		switch(_nSymmetry)
		{
		case 0:
		case 1:
		case 2:
			_nTmpX2 = (int)(_nZeroPosition - _nTmpX1) + 0.5;
			_nTmpY2 = (int)(current_pos + _nTmpY1) + 0.5   ;
			current_pos = (int)current_pos + 0.5 ;
			cairo_move_to (cr, _nTmpX2, current_pos);
			cairo_line_to (cr, _nTmpX2 ,_nTmpY2  );
			cairo_move_to (cr, _nTmpX2, (int)(_nThickness2) + 0.5 );
			cairo_line_to (cr, _nTmpX2 ,(int)(_nThickness2 - _nTmpY1) + 0.5);
			_nTmpX2 = (int)(_nZeroPosition + _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX2, current_pos);
			cairo_line_to (cr, _nTmpX2 ,_nTmpY2  );
			cairo_move_to (cr, _nTmpX2, (int)(_nThickness2) + 0.5 );
			cairo_line_to (cr, _nTmpX2 ,(int)(_nThickness2 - _nTmpY1) + 0.5);

			break;
		default:
			break;
		}
		cairo_stroke (cr);
	}

}
void DrawWeldShapeV(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h , i;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;

	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	w = w - 50 ;
	h = h - 20 ;

	double min_angle ;
	double max_angle ;
	double current_pos ;
	double thickness    = GROUP_VAL_POS(grp , part.Thickness) / 1000.0  ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double velocity     = GROUP_VAL_POS(grp  , velocity) / 200000.0;

	st_PART* p = &(GROUP_VAL_POS(grp , part)) ;
	int _nSymmetry = p->symmetry ;

	double _nWelandOffset  = p->weland_offset /1000.0 ;
	double _nWelandHeight  = p->weland_height /1000.0 ;
	double _nFizoneHeight  = p->fizone_height /1000.0 ;
	double _nFizoneAngle   = p->fizone_angle  /1000.0 ;
	double _nFizoneWidth   ;
	if(_nFizoneAngle >= 90.0)
	{
		_nFizoneWidth = _nWelandOffset ;
	}
	else
	{
		_nFizoneAngle = _nFizoneAngle * G_PI/180.0 ;
		_nFizoneWidth = _nWelandOffset + _nFizoneHeight * tan(_nFizoneAngle) ;
	}

	double _nTmpX1 , _nTmpX2 , _nTmpX3 , _nTmpX4 ;
	double _nTmpY1 , _nTmpY2 , _nTmpY3 , _nTmpY4 ;
	double _nThickness2 ;

	double _nRulerStart = prule->wmin1   ;
	double _nRulerStop  = prule->wmax1   ;
	double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;


	cairo_set_line_width (cr, 0.5);
	static const double dashed1[] ={5.0, 2.0 , 5.0};

    current_pos = (int)_nZeroPosition + 0.5  ;
    cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
    if(current_pos >= 20 && current_pos <= w + 20)
    {
    	cairo_set_dash (cr, dashed1, 3, 0);
    	cairo_move_to (cr, current_pos  , 0  );
    	cairo_line_to (cr, current_pos  , h  );
    	cairo_stroke(cr);
    	cairo_set_dash (cr, NULL, 0, 0);
    	cairo_stroke(cr);
    }

	if(LAW_VAL_POS(grp, Focal_type) == 0)
	{
		if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
			min_angle = 0.0;
		else
			min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
	}
	else
	{
		max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
	}
	sample_range  = (sample_start + sample_range) * cos(min_angle);
	sample_start  *= cos(max_angle);
	sample_range  -= sample_start  ;
	sample_range  *= velocity  ;
	sample_start  *= velocity  ;


	_nTmpX1 = w * _nWelandOffset / (_nRulerStop - _nRulerStart) ;
	_nTmpY1 = h *  _nWelandHeight / sample_range  ;
	_nTmpY2 = h *  _nFizoneHeight / sample_range  ;
	_nTmpX2 = w * _nFizoneWidth /(_nRulerStop -  _nRulerStart) ;

	for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
	{
		current_pos = h * (i * thickness - sample_start ) / sample_range  ;
		_nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
		if(current_pos < 0 && _nThickness2 < 0) continue  ;
		switch(_nSymmetry)
		{
		case 0:
			// positive
			_nTmpY3 = (int)(current_pos + _nTmpY2) + 0.5  ;
			_nTmpY4 = (int)(current_pos + _nTmpY2 + _nTmpY1) + 0.5 ;
			// left
			_nTmpX3 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
			_nTmpX4 = (int)(_nZeroPosition - _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX3, current_pos);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY3 );
			cairo_move_to (cr, _nTmpX4 ,_nTmpY3);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY4 );
			// right
			_nTmpX3 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
			_nTmpX4 = (int)(_nZeroPosition + _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX3, current_pos);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY3 );
			cairo_move_to (cr, _nTmpX4 ,_nTmpY3);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY4 );
			// negative
			_nTmpY3 = (int)(_nThickness2 - _nTmpY2) + 0.5  ;
			_nTmpY4 = (int)(_nThickness2 - _nTmpY2 - _nTmpY1) + 0.5 ;
			// left
			_nTmpX3 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
			_nTmpX4 = (int)(_nZeroPosition - _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX3, _nThickness2);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY3 );
			cairo_move_to (cr, _nTmpX4 ,_nTmpY3);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY4 );
			// right
			_nTmpX3 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
			_nTmpX4 = (int)(_nZeroPosition + _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX3, _nThickness2);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY3 );
			cairo_move_to (cr, _nTmpX4 ,_nTmpY3);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY4 );


			break;
		case 1: // right
			// positive
			_nTmpY3 = (int)(current_pos + _nTmpY2) + 0.5  ;
			_nTmpY4 = (int)(current_pos + _nTmpY2 + _nTmpY1) + 0.5 ;

			_nTmpX3 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
			_nTmpX4 = (int)(_nZeroPosition + _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX3, current_pos);
			cairo_line_to (cr, _nTmpX4 ,(int)(current_pos + _nTmpY2) + 0.5 );
			cairo_move_to (cr, _nTmpX4 ,(int)(current_pos + _nTmpY2) + 0.5 );
			cairo_line_to (cr, _nTmpX4 ,(int)(current_pos + _nTmpY2 + _nTmpY1) + 0.5 );
			// negative
			_nTmpY3 = (int)(_nThickness2 - _nTmpY2) + 0.5  ;
			_nTmpY4 = (int)(_nThickness2 - _nTmpY2 - _nTmpY1) + 0.5 ;
			// left
			cairo_move_to (cr, _nTmpX3, _nThickness2);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY3 );
			cairo_move_to (cr, _nTmpX4 ,_nTmpY3);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY4 );

			break;
		case 2: //  left
			_nTmpY3 = (int)(current_pos - _nTmpY2) + 0.5  ;
			_nTmpY4 = (int)(current_pos - _nTmpY2 + _nTmpY1) + 0.5 ;

			_nTmpX3 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
			_nTmpX4 = (int)(_nZeroPosition - _nTmpX1) + 0.5;
			cairo_move_to (cr, _nTmpX3, current_pos);
			cairo_line_to (cr, _nTmpX4 ,(int)(current_pos + _nTmpY2) + 0.5 );
			cairo_move_to (cr, _nTmpX4 ,(int)(current_pos + _nTmpY2) + 0.5 );
			cairo_line_to (cr, _nTmpX4 ,(int)(current_pos + _nTmpY2 + _nTmpY1) + 0.5 );
			// negative
			_nTmpY3 = (int)(_nThickness2 - _nTmpY2) + 0.5  ;
			_nTmpY4 = (int)(_nThickness2 - _nTmpY2 - _nTmpY1) + 0.5 ;
			// left
			cairo_move_to (cr, _nTmpX3, _nThickness2);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY3 );
			cairo_move_to (cr, _nTmpX4 ,_nTmpY3);
			cairo_line_to (cr, _nTmpX4 ,_nTmpY4 );
			break;
		default:
			break;
		}
		cairo_stroke (cr);
	}
}
void DrawWeldShapeDoubleV(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h , i;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;

	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	w -= 50 ;
	h -= 20 ;
	
	double min_angle ;
	double max_angle ;
	double current_pos ;
	double thickness    = GROUP_VAL_POS(grp , part.Thickness)/ 1000.0  ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double velocity     = GROUP_VAL_POS(grp  , velocity) / 200000.0;

	st_PART* p = &(GROUP_VAL_POS(grp , part)) ;
	int _nSymmetry = p->symmetry ;

	double _nWelandOffset  = p->weland_offset /1000.0 ;
	double _nWelandHeight  = p->weland_height /1000.0 ;
	double _nFizoneHeight  = p->fizone_height /1000.0 ;
	double _nFizoneAngle   = p->fizone_angle  /1000.0 ;
	double _nRootAngle     = p->fizone_radius /1000.0 ;
	double _nUpFizoneWidth;
	double _nDownFizoneWidth;


    _nFizoneAngle 	  = _nFizoneAngle * G_PI/180.0 ;
	_nRootAngle   	  = _nRootAngle * G_PI/180.0 ;
    _nUpFizoneWidth	  = _nWelandOffset + (thickness - _nFizoneHeight - _nWelandHeight) * tan(_nFizoneAngle) ;
	_nDownFizoneWidth = _nWelandOffset + (_nFizoneHeight * tan(_nRootAngle));


	double _nTmpX1 , _nTmpX2 , _nTmpX3 , _nTmpX4 , _nTmpX5 , _nTmpX6;
	double _nTmpY1 , _nTmpY2 , _nTmpY3 , _nTmpY4 , _nTmpY5 , _nTmpY6;
	double _nThickness2 ;
	
	double _nRulerStart = prule->wmin1	 ;
	double _nRulerStop	= prule->wmax1	 ;
	double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;
	cairo_set_line_width (cr, 0.5);
	static const double dashed1[] ={5.0, 2.0 , 5.0};
    current_pos = (int)_nZeroPosition + 0.5  ;
    cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
    if(current_pos >= 20 && current_pos <= w + 20)
    {
        if(!_nSymmetry)
        {
                        cairo_set_dash (cr, dashed1, 3, 0);
                        cairo_move_to (cr, current_pos  , 0  );
                        cairo_line_to (cr, current_pos  , h  );
                        cairo_stroke(cr);
                        cairo_set_dash (cr, NULL, 0, 0);
                        cairo_stroke(cr);

        }
        else
        {
                        cairo_move_to (cr, current_pos  , 0  );
                        cairo_line_to (cr, current_pos  , h  );
                        cairo_stroke(cr);
        }
    }
	if(LAW_VAL_POS(grp, Focal_type) == 0)
	{
			if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
					min_angle = 0.0;
			else
			{
					min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
	
			}
			max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
	}
	else
	{
			max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
	}
	sample_range  = (sample_start + sample_range) * cos(min_angle);
	sample_start  *= cos(max_angle);
	sample_range  -= sample_start  ;
	sample_range  *= velocity  ;
	sample_start  *= velocity  ;

	_nTmpX1 = w * _nWelandOffset / (_nRulerStop - _nRulerStart);
	_nTmpY1 = h * (thickness - _nFizoneHeight - _nWelandHeight) / sample_range  ;
	_nTmpX2 = w * _nUpFizoneWidth / (_nRulerStop - _nRulerStart);
	_nTmpY2 = h * _nWelandHeight / sample_range;
	_nTmpX3 = w * _nDownFizoneWidth / (_nRulerStop - _nRulerStart); 
	_nTmpY3 = h * _nFizoneHeight / sample_range; 

	
	for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
	{
		current_pos = h * (i * thickness - sample_start ) / sample_range  ; 
		_nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ; 
		if(current_pos < 0 && _nThickness2 < 0) continue  ;
		switch(_nSymmetry)
		{
		case 0:
			_nTmpX4 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
			_nTmpX5 = (int)(_nZeroPosition - _nTmpX1) + 0.5;
			_nTmpX6 = (int)(_nZeroPosition - _nTmpX3) + 0.5;

			_nTmpY4 = (int)(current_pos + _nTmpY1) + 0.5;
			_nTmpY5 = (int)(current_pos + _nTmpY1 + _nTmpY2) + 0.5;
			_nTmpY6 = (int)(current_pos +  _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;
			
			cairo_move_to(cr,_nTmpX4,current_pos);
			cairo_line_to(cr,_nTmpX5,_nTmpY4);
			cairo_move_to(cr,_nTmpX5,_nTmpY4);
			cairo_line_to(cr,_nTmpX5,_nTmpY5);
			cairo_move_to(cr,_nTmpX5,_nTmpY5);
			cairo_line_to(cr,_nTmpX6,_nTmpY6);


			_nTmpY4 = (int)(_nThickness2 - _nTmpY1) + 0.5;
			_nTmpY5 = (int)(_nThickness2 - _nTmpY1 - _nTmpY2) + 0.5;
			_nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;


			cairo_move_to(cr,_nTmpX4,_nThickness2);
			cairo_line_to(cr,_nTmpX5,_nTmpY4);
			cairo_move_to(cr,_nTmpX5,_nTmpY4);
			cairo_line_to(cr,_nTmpX5,_nTmpY5);
			cairo_move_to(cr,_nTmpX5,_nTmpY5);
			cairo_line_to(cr,_nTmpX6,_nTmpY6);
			break;
		case 1:
			break;
		case 2:
                        _nTmpX4 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
                        _nTmpX5 = (int)(_nZeroPosition - _nTmpX1) + 0.5;
                        _nTmpX6 = (int)(_nZeroPosition - _nTmpX3) + 0.5;

                        _nTmpY4 = (int)(current_pos + _nTmpY1) + 0.5;
                        _nTmpY5 = (int)(current_pos + _nTmpY1 + _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(current_pos +  _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,current_pos);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX5,_nTmpY5);
                        cairo_move_to(cr,_nTmpX5,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);


                        _nTmpY4 = (int)(_nThickness2 - _nTmpY1) + 0.5;
                        _nTmpY5 = (int)(_nThickness2 - _nTmpY1 - _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;


                        cairo_move_to(cr,_nTmpX4,_nThickness2);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX5,_nTmpY5);
                        cairo_move_to(cr,_nTmpX5,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);

			break;
		}
	}
	cairo_stroke(cr);

	for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
	{
		current_pos = h * (i * thickness - sample_start ) / sample_range  ;
		_nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
		if(current_pos < 0 && _nThickness2 < 0) continue  ;
		switch(_nSymmetry)

		{
		case 0:
			_nTmpX4 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
			_nTmpX5 = (int)(_nZeroPosition + _nTmpX1) + 0.5;
			_nTmpX6 = (int)(_nZeroPosition + _nTmpX3) + 0.5;

			_nTmpY4 = (int)(current_pos + _nTmpY1) + 0.5;
			_nTmpY5 = (int)(current_pos + _nTmpY1 + _nTmpY2) + 0.5;
			_nTmpY6 = (int)(current_pos + _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;

			cairo_move_to(cr,_nTmpX4,current_pos);
			cairo_line_to(cr,_nTmpX5,_nTmpY4);
			cairo_move_to(cr,_nTmpX5,_nTmpY4);
			cairo_line_to(cr,_nTmpX5,_nTmpY5);
			cairo_move_to(cr,_nTmpX5,_nTmpY5);
			cairo_line_to(cr,_nTmpX6,_nTmpY6);


			_nTmpY4 = (int)(_nThickness2 - _nTmpY1) + 0.5;
			_nTmpY5 = (int)(_nThickness2 - _nTmpY1 - _nTmpY2) + 0.5;
			_nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;
	
			cairo_move_to(cr,_nTmpX4,_nThickness2);
			cairo_line_to(cr,_nTmpX5,_nTmpY4);
			cairo_move_to(cr,_nTmpX5,_nTmpY4);
			cairo_line_to(cr,_nTmpX5,_nTmpY5);
			cairo_move_to(cr,_nTmpX5,_nTmpY5);
			cairo_line_to(cr,_nTmpX6,_nTmpY6);
			break;
		case 1:
                        _nTmpX4 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
                        _nTmpX5 = (int)(_nZeroPosition + _nTmpX1) + 0.5;
                        _nTmpX6 = (int)(_nZeroPosition + _nTmpX3) + 0.5;

                        _nTmpY4 = (int)(current_pos + _nTmpY1) + 0.5;
                        _nTmpY5 = (int)(current_pos + _nTmpY1 + _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(current_pos + _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,current_pos);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX5,_nTmpY5);
                        cairo_move_to(cr,_nTmpX5,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);


                        _nTmpY4 = (int)(_nThickness2 - _nTmpY1) + 0.5;
                        _nTmpY5 = (int)(_nThickness2 - _nTmpY1 - _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,_nThickness2);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX5,_nTmpY5);
                        cairo_move_to(cr,_nTmpX5,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);
			break;
		case 2:
			break;
		}	
	}		
	cairo_stroke(cr);
}

void DrawWeldShapeU(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h , i;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;

	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	w -= 50 ;
	h -= 20 ;

	double min_angle ;
	double max_angle ;
	double current_pos ;
	double thickness    = GROUP_VAL_POS(grp , part.Thickness)/ 1000.0  ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double velocity     = GROUP_VAL_POS(grp  , velocity) / 200000.0;

	st_PART* p = &(GROUP_VAL_POS(grp , part)) ;
	int _nSymmetry = p->symmetry ;

	double _nWelandOffset  = p->weland_offset /1000.0 ;
	double _nWelandHeight  = p->weland_height /1000.0 ;
	double _nFizoneHeight  = p->fizone_height /1000.0 ;
	double _nFizoneAngle  = p->fizone_angle   /1000.0 ;
	double _nFizoneRadius  = p->fizone_radius /1000.0 ;

	double _nTmpX1 , _nTmpX2 , _nTmpX3 , _nTmpX4 , _nTmpX5 , _nTmpX6;
	double _nTmpY1 , _nTmpY2 , _nTmpY3 , _nTmpY4 , _nTmpY5 , _nTmpY6 , _nTmpY7 , _nTmpY8;

	double _nArcx , _nArcy  , _nArcx1 , _nArcy1;
	double _nThickness2 ;

	double _nRulerStart = prule->wmin1   ;
	double _nRulerStop  = prule->wmax1   ;
	double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;


	cairo_set_line_width (cr, 0.5);
	static const double dashed1[] ={5.0, 2.0 , 5.0};

    current_pos = (int)_nZeroPosition + 0.5  ;
    cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
    if(current_pos >= 20 && current_pos <= w + 20)
    {
    	cairo_set_dash (cr, dashed1, 3, 0);
    	cairo_move_to (cr, current_pos  , 0  );
    	cairo_line_to (cr, current_pos  , h  );
    	cairo_stroke(cr);
    	cairo_set_dash (cr, NULL, 0, 0);
    	cairo_stroke(cr);
    }

	if(LAW_VAL_POS(grp, Focal_type) == 0)
	{
		if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
			min_angle = 0.0;
		else
			min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
	}
	else
	{
		max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
	}
	sample_range  = (sample_start + sample_range) * cos(min_angle);
	sample_start  *= cos(max_angle);
	sample_range  -= sample_start  ;
	sample_range  *= velocity  ;
	sample_start  *= velocity  ;

	if(_nFizoneRadius <= _nWelandOffset)
	{
		DrawWeldShapeV(widget , data , cr) ;
		return ;
	}
	else
	{
		_nTmpY1 = sqrt(_nFizoneRadius * _nFizoneRadius - _nWelandOffset * _nWelandOffset );
	}
	if(_nFizoneAngle < 0 || _nFizoneAngle >= 90)
	{
		_nFizoneAngle = 0 ;
	}
	else
	{
		_nFizoneAngle = _nFizoneAngle * G_PI / 180.0 ;
	}

	_nTmpY2 = sin(_nFizoneAngle) * _nFizoneRadius  ;
	//printf("curve %f , %f  \n" , _nTmpY2 , _nTmpY1 );

	if(_nTmpY2 > _nTmpY1)
	{
		//U段不满足相切条件时
		//_nTmpY2 = _nFizoneHeight - _nTmpY1 ;
		DrawWeldShapeV(widget , data , cr) ;
		return ;
	}
	else
	{
		//产满足相切条件时
		_nTmpY2 = _nFizoneHeight - _nTmpY1 + _nTmpY2   ;
	}

	_nTmpY1 = _nFizoneHeight - _nTmpY1 ;
	_nTmpX1 = cos(_nFizoneAngle) * _nFizoneRadius ;
	if(_nTmpX1 < _nWelandOffset )// _nTmpX1 = _nWelandOffset  ;//U段不满足相切条件时
	{
		//U段不满足相切条件时
		//_nTmpY2 = _nFizoneHeight - _nTmpY1 ;
		DrawWeldShapeV(widget , data , cr) ;
		return ;
	}
    _nTmpX2 = sin(_nFizoneAngle) * (_nFizoneHeight - _nTmpY1) + _nTmpX1 ;
	_nTmpX3 = w * _nWelandOffset / (_nRulerStop - _nRulerStart) ; 	// offset left
	_nTmpX2 = w * _nTmpX2 / (_nRulerStop - _nRulerStart) ;        	// left most
	_nTmpX1 = w * _nTmpX1 / (_nRulerStop - _nRulerStart) ;        	// junction left
	_nTmpY2 = h * _nTmpY2 / sample_range  ;                      	// junction Y1
	_nTmpY1 = h * _nTmpY1 / sample_range  ;                      	// center y
	_nTmpY3 = h * _nFizoneHeight / sample_range ;                	// junction Y2
	_nTmpY4 = h * (_nFizoneHeight + _nWelandHeight) / sample_range ; // bottom y

	_nArcx = (_nTmpX1 + _nTmpX3) / 2  ;
	_nArcy = _nTmpY2 / 4 + 3 * _nTmpY3 / 4  ;

	for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
	{
		current_pos = h * (i * thickness - sample_start ) / sample_range  ;
		_nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
		if(current_pos < 0 && _nThickness2 < 0) continue  ;
		switch(_nSymmetry)
		{
		case 0:
			_nTmpX4 = (int)(_nZeroPosition + _nTmpX2) + 0.5;  // left most
			_nTmpX5 = (int)(_nZeroPosition + _nTmpX1) + 0.5;  // junction left
			_nTmpX6 = (int)(_nZeroPosition + _nTmpX3) + 0.5;  // offset left
			_nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
			_nArcx1 = (int)(_nZeroPosition + _nArcx) + 0.5;   // arc middle point x
		    _nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, current_pos);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;

			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			_nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
		    _nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5   ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, _nThickness2);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			_nTmpX4 = (int)(_nZeroPosition - _nTmpX2) + 0.5;  // left most
			_nTmpX5 = (int)(_nZeroPosition - _nTmpX1) + 0.5;  // junction left
			_nTmpX6 = (int)(_nZeroPosition - _nTmpX3) + 0.5;  // offset left
			_nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
			_nArcx1 = (int)(_nZeroPosition - _nArcx) + 0.5;   // arc middle point x
		    _nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, current_pos);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			_nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
		    _nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5    ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, _nThickness2);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			break;
		case 1:
			_nTmpX4 = (int)(_nZeroPosition + _nTmpX2) + 0.5;  // left most
			_nTmpX5 = (int)(_nZeroPosition + _nTmpX1) + 0.5;  // junction left
			_nTmpX6 = (int)(_nZeroPosition + _nTmpX3) + 0.5;  // offset left
			_nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
			_nArcx1 = (int)(_nZeroPosition + _nArcx) + 0.5;   // arc middle point x
		    _nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, current_pos);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			_nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
		    _nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5   ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, _nThickness2);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			break;
		case 2:

			_nTmpX4 = (int)(_nZeroPosition - _nTmpX2) + 0.5;  // left most
			_nTmpX5 = (int)(_nZeroPosition - _nTmpX1) + 0.5;  // junction left
			_nTmpX6 = (int)(_nZeroPosition - _nTmpX3) + 0.5;  // offset left
			_nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
			_nArcx1 = (int)(_nZeroPosition - _nArcx) + 0.5;   // arc middle point x
		    _nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, current_pos);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			_nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
			_nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
			_nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
			_nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
		    _nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5    ;   // arc middle point y

			cairo_move_to (cr, _nTmpX4, _nThickness2);
			cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
			cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
			cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
			cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );

			break;
		default:
			break;
		}
		cairo_stroke (cr);
	}

}

void DrawWeldShapeVY(GtkWidget* widget , gpointer data , cairo_t* cr)
{
        int w,h , i;
        DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
        unsigned char grp = prule->group ;

        gtk_widget_get_size_request (widget, &w, &h);
        int _nMark = prule->mask ;
        if(_nMark & 0x1)  h += 20 ;
        if(_nMark & 0x2)  w += 20 ;
        if(_nMark & 0x4)  w += 10 ;
        if(_nMark & 0x8)  w += 20 ;
        if(_nMark & 0x10) h += 15 ;
        w -= 50 ;
        h -= 20 ;

        double min_angle ;
        double max_angle ;
        double current_pos ;
        double thickness    = GROUP_VAL_POS(grp , part.Thickness)/ 1000.0  ;
        double sample_start = group_get_start(grp) / 1000.0 ;
        double sample_range = group_get_range(grp) / 1000.0 ;
        double velocity     = GROUP_VAL_POS(grp  , velocity) / 200000.0;

        st_PART* p = &(GROUP_VAL_POS(grp , part)) ;
        int _nSymmetry = p->symmetry ;

        double _nWelandOffset  = p->weland_offset /1000.0 ;
        double _nWelandHeight  = p->weland_height /1000.0 ;
        double _nFizoneHeight  = p->fizone_height /1000.0 ;
        double _nFizoneAngle   = p->fizone_angle  /1000.0 ;
        double _nRootAngle     = p->fizone_radius /1000.0 ;
        double _nFizoneWidth;

	if(_nFizoneHeight > thickness)
	{
		_nFizoneHeight = thickness;
	}

        _nFizoneAngle = _nFizoneAngle * G_PI/180.0 ;
        _nRootAngle   = _nRootAngle * G_PI/180.0 ;
        _nFizoneWidth = _nWelandOffset + (_nFizoneHeight - _nWelandHeight)*tan(_nRootAngle) + (thickness - _nFizoneHeight) * tan(_nFizoneAngle) ;
        double _nTmpX1 , _nTmpX2 , _nTmpX3 , _nTmpX4 , _nTmpX5 , _nTmpX6;
        double _nTmpY1 , _nTmpY2 , _nTmpY3 , _nTmpY4 , _nTmpY5 , _nTmpY6;
        double _nThickness2 ;

        double _nRulerStart = prule->wmin1       ;
        double _nRulerStop      = prule->wmax1   ;
        double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;
        cairo_set_line_width (cr, 0.5);
        static const double dashed1[] ={5.0, 2.0 , 5.0};
    current_pos = (int)_nZeroPosition + 0.5  ;
    cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
    if(current_pos >= 20 && current_pos <= w + 20)
    {
        if(!_nSymmetry)
        {
                        cairo_set_dash (cr, dashed1, 3, 0);
                        cairo_move_to (cr, current_pos  , 0  );
                        cairo_line_to (cr, current_pos  , h  );
                        cairo_stroke(cr);
                        cairo_set_dash (cr, NULL, 0, 0);
                        cairo_stroke(cr);

        }
        else
        {
                        cairo_move_to (cr, current_pos  , 0  );
                        cairo_line_to (cr, current_pos  , h  );
                        cairo_stroke(cr);
        }
    }
        if(LAW_VAL_POS(grp, Focal_type) == 0)
        {
                        if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
                                        min_angle = 0.0;
                        else
                        {
                                        min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;

                        }
                        max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
        }
        else
        {
                        max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
        }
        sample_range  = (sample_start + sample_range) * cos(min_angle);
        sample_start  *= cos(max_angle);
        sample_range  -= sample_start  ;
        sample_range  *= velocity  ;
        sample_start  *= velocity  ;

        _nTmpX1 = w * _nWelandOffset / (_nRulerStop - _nRulerStart);
        _nTmpY1 = h * _nWelandHeight / sample_range  ;
        _nTmpX2 = w * ((_nFizoneHeight - (p->weland_height /1000.0)) * tan(_nRootAngle) + _nWelandOffset) / (_nRulerStop - _nRulerStart);

        _nTmpY2 = h * (_nFizoneHeight - _nWelandHeight) / sample_range;
        _nTmpX3 = w * _nFizoneWidth / (_nRulerStop - _nRulerStart);
        _nTmpY3 = h * (thickness - _nFizoneHeight) /sample_range;

        for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
        {
                current_pos = h * (i * thickness - sample_start ) / sample_range  ;
                _nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
                if(current_pos < 0 && _nThickness2 < 0) continue  ;
                switch(_nSymmetry)
                {
                case 0:
                        _nTmpX4 = (int)(_nZeroPosition - _nTmpX3) + 0.5;
                        _nTmpX5 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
                        _nTmpX6 = (int)(_nZeroPosition - _nTmpX1) + 0.5;

                        _nTmpY4 = (int)(current_pos + _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(current_pos + _nTmpY3 + _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(current_pos + _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,current_pos);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);


                        _nTmpY4 = (int)(_nThickness2 - _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,_nThickness2);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);

                        break;
		case 1:
			break;
		case 2:
			_nTmpX4 = (int)(_nZeroPosition - _nTmpX3) + 0.5;
                        _nTmpX5 = (int)(_nZeroPosition - _nTmpX2) + 0.5;
                        _nTmpX6 = (int)(_nZeroPosition - _nTmpX1) + 0.5;

                        _nTmpY4 = (int)(current_pos + _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(current_pos + _nTmpY3 + _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(current_pos + _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,current_pos);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);


                        _nTmpY4 = (int)(_nThickness2 - _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,_nThickness2);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);

			break;
                }
        }
        cairo_stroke(cr);

        for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
        {
                current_pos = h * (i * thickness - sample_start ) / sample_range  ;
                _nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
                if(current_pos < 0 && _nThickness2 < 0) continue  ;
                switch(_nSymmetry)

                {
                case 0:
                        _nTmpX4 = (int)(_nZeroPosition + _nTmpX3) + 0.5;
                        _nTmpX5 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
                        _nTmpX6 = (int)(_nZeroPosition + _nTmpX1) + 0.5;

                        _nTmpY4 = (int)(current_pos + _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(current_pos + _nTmpY3 + _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(current_pos + _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;
                        cairo_move_to(cr,_nTmpX4,current_pos);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);

                        _nTmpY4 = (int)(_nThickness2 - _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,_nThickness2);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);
                        break;
		case 1:
                        _nTmpX4 = (int)(_nZeroPosition + _nTmpX3) + 0.5;
                        _nTmpX5 = (int)(_nZeroPosition + _nTmpX2) + 0.5;
                        _nTmpX6 = (int)(_nZeroPosition + _nTmpX1) + 0.5;

                        _nTmpY4 = (int)(current_pos + _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(current_pos + _nTmpY3 + _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(current_pos + _nTmpY3 + _nTmpY2 + _nTmpY1) + 0.5;
                        cairo_move_to(cr,_nTmpX4,current_pos);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);

                        _nTmpY4 = (int)(_nThickness2 - _nTmpY3) + 0.5;
                        _nTmpY5 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2) + 0.5;
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3 - _nTmpY2 - _nTmpY1) + 0.5;

                        cairo_move_to(cr,_nTmpX4,_nThickness2);
                        cairo_line_to(cr,_nTmpX5,_nTmpY4);
                        cairo_move_to(cr,_nTmpX5,_nTmpY4);
                        cairo_line_to(cr,_nTmpX6,_nTmpY5);
                        cairo_move_to(cr,_nTmpX6,_nTmpY5);
                        cairo_line_to(cr,_nTmpX6,_nTmpY6);

			break;
		case 2:
			break;
                }
        }
        cairo_stroke(cr);
}

void DrawWeldShapeJ(GtkWidget* widget , gpointer data , cairo_t* cr)
{
        int w,h , i;
        DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
        unsigned char grp = prule->group ;

        gtk_widget_get_size_request (widget, &w, &h);
        int _nMark = prule->mask ;
        if(_nMark & 0x1)  h += 20 ;
        if(_nMark & 0x2)  w += 20 ;
        if(_nMark & 0x4)  w += 10 ;
        if(_nMark & 0x8)  w += 20 ;
        if(_nMark & 0x10) h += 15 ;
        w -= 50 ;
        h -= 20 ;

        double min_angle ;
        double max_angle ;
        double current_pos ;
        double thickness    = GROUP_VAL_POS(grp , part.Thickness)/ 1000.0  ;
        double sample_start = group_get_start(grp) / 1000.0 ;
        double sample_range = group_get_range(grp) / 1000.0 ;
        double velocity     = GROUP_VAL_POS(grp  , velocity) / 200000.0;

        st_PART* p = &(GROUP_VAL_POS(grp , part)) ;
        int _nSymmetry = p->symmetry ;

        double _nWelandOffset  = 0/*p->weland_offset /1000.0*/ ;
        double _nFizoneHeight  = p->fizone_height /1000.0 ;
        double _nFizoneAngle   = p->fizone_angle   /1000.0 ;
        double _nFizoneRadius  = p->fizone_radius /1000.0 ;
        double _nRootAngle     = p->weland_offset /1000.0;
        double _nRootHeight    = p->weland_height /1000.0;
	

        double _nTmpX1 , _nTmpX2 , _nTmpX3 , _nTmpX4 , _nTmpX5 , _nTmpX6,_nTmpX11,_nTmpX12;
        double _nTmpY1 , _nTmpY2 , _nTmpY3 , _nTmpY4 , _nTmpY5 , _nTmpY6 , _nTmpY7 , _nTmpY8,_nTmpY11,_nTmpY12;

        double _nArcx , _nArcy  , _nArcx1 , _nArcy1;
        double _nThickness2 ;

        double _nRulerStart = prule->wmin1   ;
        double _nRulerStop  = prule->wmax1   ;
        double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;


        cairo_set_line_width (cr, 0.5);
        static const double dashed1[] ={5.0, 2.0 , 5.0};

    current_pos = (int)_nZeroPosition + 0.5  ;
    cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
    if(current_pos >= 20 && current_pos <= w + 20)
    {
        if(!_nSymmetry)
        {
                        cairo_set_dash (cr, dashed1, 3, 0);
                        cairo_move_to (cr, current_pos  , 0  );
                        cairo_line_to (cr, current_pos  , h  );
                        cairo_stroke(cr);
                        cairo_set_dash (cr, NULL, 0, 0);
                        cairo_stroke(cr);
        }
        else
        {
                        cairo_move_to (cr, current_pos  , 0  );
                        cairo_line_to (cr, current_pos  , h  );
                        cairo_stroke(cr);
        }
    }

        if(LAW_VAL_POS(grp, Focal_type) == 0)
        {
                if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
                        min_angle = 0.0;
                else
                        min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
                max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
        }
        else
        {
                max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
        }
        sample_range  = (sample_start + sample_range) * cos(min_angle);
        sample_start  *= cos(max_angle);
        sample_range  -= sample_start  ;
        sample_range  *= velocity  ;
        sample_start  *= velocity  ;
	 _nTmpY1 = sqrt(_nFizoneRadius * _nFizoneRadius - _nWelandOffset * _nWelandOffset );
        if(_nFizoneAngle < 0 || _nFizoneAngle >= 90)
        {
                _nFizoneAngle = 0 ;
        }
        else
        {
                _nFizoneAngle = _nFizoneAngle * G_PI / 180.0 ;
                _nRootAngle       = _nRootAngle * G_PI / 180.0 ;
        }

        _nTmpY2 = sin(_nFizoneAngle) * _nFizoneRadius  ;
        if(_nTmpY2 > _nTmpY1)
        {
                //U段不满足相切条件时
                //_nTmpY2 = _nFizoneHeight - _nTmpY1 ;
                DrawWeldShapeV(widget , data , cr) ;
                return ;
        }
        else
        {
                //产满足相切条件时
                _nTmpY2 = _nFizoneHeight - _nTmpY1 + _nTmpY2   ;
        }

        _nTmpY1 = _nFizoneHeight - _nTmpY1 ;
        _nTmpX1 = cos(_nFizoneAngle) * _nFizoneRadius ;
        if(_nTmpX1 < _nWelandOffset )// _nTmpX1 = _nWelandOffset  ;//U段不满足相>切条件时
        {
                //U段不满足相切条件时
                //_nTmpY2 = _nFizoneHeight - _nTmpY1 ;
                DrawWeldShapeV(widget , data , cr) ;
                return ;
        }
	_nTmpX2 = sin(_nFizoneAngle) * (_nFizoneHeight - _nTmpY1) + _nTmpX1 ;
        _nTmpX3 = w * _nWelandOffset / (_nRulerStop - _nRulerStart) ;   // offset left
        _nTmpX2 = w * _nTmpX2 / (_nRulerStop - _nRulerStart) ;          // left most
        _nTmpX1 = w * _nTmpX1 / (_nRulerStop - _nRulerStart) ;          // junction left
        _nTmpY2 = h * _nTmpY2 / sample_range  ;                         // junction Y1
        _nTmpY1 = h * _nTmpY1 / sample_range  ;                         // center y
        _nTmpY3 = h * _nFizoneHeight / sample_range ;                   // junction Y2
        _nTmpY4 = h * (_nFizoneHeight + (thickness - _nFizoneHeight - _nRootHeight)) / sample_range ; // bottom y

        _nTmpX11 = w * (tan(_nRootAngle) * _nRootHeight)/(_nRulerStop - _nRulerStart);
        _nTmpY11 = h * thickness / sample_range ;

        _nArcx = (_nTmpX1 + _nTmpX3) / 2  ;
        _nArcy = _nTmpY2 / 4 + 3 * _nTmpY3 / 4  ;

        for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
        {
                current_pos = h * (i * thickness - sample_start ) / sample_range  ;
                _nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
                if(current_pos < 0 && _nThickness2 < 0) continue  ;
                switch(_nSymmetry)
                {
                case 0:
                        _nTmpX4 = (int)(_nZeroPosition - _nTmpX2) + 0.5;  // left most
                        _nTmpX5 = (int)(_nZeroPosition - _nTmpX1) + 0.5;  // junction left
                        _nTmpX6 = (int)(_nZeroPosition - _nTmpX3) + 0.5;  // offset left
                        _nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
                        _nArcx1 = (int)(_nZeroPosition - _nArcx) + 0.5;   // arc middle point x
			_nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y

                        _nTmpX12 = (int)(_nZeroPosition - _nTmpX11) + 0.5;
                        _nTmpX12 = (int)(_nZeroPosition - _nTmpX11) + 0.5;
                        _nTmpY12 = (int)(current_pos + _nTmpY11) + 0.5   ;

                        cairo_move_to (cr, _nTmpX4, current_pos);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5);
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6);
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7);
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);

                        _nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
			_nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5    ;   // arc middle point y

                        _nTmpY12 = (int)(_nThickness2 - _nTmpY11) + 0.5   ;


                        cairo_move_to (cr, _nTmpX4, _nThickness2);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);
                        break;
                case 1:
                        break;
                case 2:
                        _nTmpX4 = (int)(_nZeroPosition - _nTmpX2) + 0.5;  // left most
                        _nTmpX5 = (int)(_nZeroPosition - _nTmpX1) + 0.5;  // junction left
                        _nTmpX6 = (int)(_nZeroPosition - _nTmpX3) + 0.5;  // offset left
                        _nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
                        _nArcx1 = (int)(_nZeroPosition - _nArcx) + 0.5;   // arc middle point x
                        _nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y

                        _nTmpX12 = (int)(_nZeroPosition - _nTmpX11) + 0.5;
                        _nTmpX12 = (int)(_nZeroPosition - _nTmpX11) + 0.5;
                        _nTmpY12 = (int)(current_pos + _nTmpY11) + 0.5   ;

                        cairo_move_to (cr, _nTmpX4, current_pos);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5);
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6);
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7);
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);

                        _nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
                        _nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5    ;   // arc middle point y

                        _nTmpY12 = (int)(_nThickness2 - _nTmpY11) + 0.5   ;


                        cairo_move_to (cr, _nTmpX4, _nThickness2);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);

			break;
                }
        }
        cairo_stroke(cr);

        for(i = 0 ; ((i * thickness - sample_start) < sample_range) && (i <= 10); i+=2)
        {
                current_pos = h * (i * thickness - sample_start ) / sample_range  ;
                _nThickness2 = h * ((i+2) * thickness - sample_start ) / sample_range  ;
                if(current_pos < 0 && _nThickness2 < 0) continue  ;
                switch(_nSymmetry)
                {
                case 0:
                        _nTmpX4 = (int)(_nZeroPosition + _nTmpX2) + 0.5;  // left most
                        _nTmpX5 = (int)(_nZeroPosition + _nTmpX1) + 0.5;  // junction left
                        _nTmpX6 = (int)(_nZeroPosition + _nTmpX3) + 0.5;  // offset left
                        _nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
                        _nArcx1 = (int)(_nZeroPosition + _nArcx) + 0.5;   // arc middle point x
			_nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y
                        _nTmpX12 = (int)(_nZeroPosition + _nTmpX11) + 0.5;
                        _nTmpY12 = (int)(current_pos + _nTmpY11) + 0.5   ;

                        cairo_move_to (cr, _nTmpX4, current_pos);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);


                        _nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
			_nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5   ;   // arc middle point y
                        _nTmpY12 = (int)(_nThickness2 - _nTmpY11) + 0.5   ;

                        cairo_move_to (cr, _nTmpX4, _nThickness2);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);

                        break;
                case 1:
                        _nTmpX4 = (int)(_nZeroPosition + _nTmpX2) + 0.5;  // left most
                        _nTmpX5 = (int)(_nZeroPosition + _nTmpX1) + 0.5;  // junction left
                        _nTmpX6 = (int)(_nZeroPosition + _nTmpX3) + 0.5;  // offset left
                        _nTmpY5 = (int)(current_pos + _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(current_pos + _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(current_pos + _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(current_pos + _nTmpY1) + 0.5   ;  // center y
                        _nArcx1 = (int)(_nZeroPosition + _nArcx) + 0.5;   // arc middle point x
                        _nArcy1 = (int)(current_pos + _nArcy) + 0.5   ;   // arc middle point y
                        _nTmpX12 = (int)(_nZeroPosition + _nTmpX11) + 0.5;
                        _nTmpY12 = (int)(current_pos + _nTmpY11) + 0.5   ;

                        cairo_move_to (cr, _nTmpX4, current_pos);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);


                        _nTmpY5 = (int)(_nThickness2 - _nTmpY2) + 0.5   ;  // junction Y1
                        _nTmpY6 = (int)(_nThickness2 - _nTmpY3) + 0.5   ;  // junction Y2
                        _nTmpY7 = (int)(_nThickness2 - _nTmpY4) + 0.5   ;  // bottom y
                        _nTmpY8 = (int)(_nThickness2 - _nTmpY1) + 0.5   ;  // center y
                        _nArcy1 = (int)(_nThickness2 - _nArcy) + 0.5   ;   // arc middle point y
                        _nTmpY12 = (int)(_nThickness2 - _nTmpY11) + 0.5   ;

                        cairo_move_to (cr, _nTmpX4, _nThickness2);
                        cairo_line_to (cr, _nTmpX5 ,_nTmpY5 );
                        cairo_curve_to (cr , _nTmpX5 , _nTmpY5 , _nArcx1, _nArcy1 , _nTmpX6 , _nTmpY6)  ;
                        cairo_move_to (cr, _nTmpX6 ,_nTmpY6 );
                        cairo_line_to (cr, _nTmpX6 ,_nTmpY7 );
                        cairo_move_to(cr,_nTmpX6 ,_nTmpY7);
                        cairo_line_to (cr,_nTmpX12,_nTmpY12);

			break;
		case 2:
			break;
                }
        }
        cairo_stroke(cr);
}



void DrawWeld(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	st_PART* p = &(GROUP_VAL_POS(prule->group , part)) ;
	int _nWeldType = p->Weld  ;
	switch (prule->scan_type)
	{
		case S_SCAN_A:
		case S_SCAN_L:
				    switch(_nWeldType)
				    {
				    case 0:
				    	DrawWeldShapeI(widget , data , cr) ;
				    	break;
				    case 1:
				    	DrawWeldShapeV(widget , data , cr) ;
				    	break;
				    case 2:
				    	DrawWeldShapeDoubleV(widget , data , cr) ;
				    	break;
				    case 3:
				    	DrawWeldShapeU(widget , data , cr) ;
				    	break;
				    case 4:
					DrawWeldShapeVY(widget , data , cr);
					break;
				    case 5:
					DrawWeldShapeJ(widget , data , cr);
					break;
				    }
			break;
		default:
			break ;
	}
}

// dac   A SCAN  window
void draw_dac_curve_horizontal(cairo_t *cr, int width, int height , int grp)
{
	double point_x[18] ;
	double point_y[18] ;
	int i, j ;
	double _nStepPix1, _nStepPix2  , _nStepRate[5];
	double ref_ampl_offset;
	int _nBeamNo = TMP(beam_num[grp]);
    double sample_start = group_get_start (grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0;

	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	int point_pos   = GROUP_VAL_POS(grp , SizingCurves.point_pos) ;
	ref_ampl_offset = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) / 2000.0 ) ;

	point_y[0] = ( height * GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) / 1000.0 ) * ref_ampl_offset ;
	point_x[0] = 0 ;

	_nStepRate[0] = 1 ;
	_nStepRate[1] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 2000.0);
	_nStepRate[2] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 1000.0);
	_nStepRate[3] = 1 / _nStepRate[1] ;
	_nStepRate[4] = 1 / _nStepRate[2] ;

	for( i = 1; i < point_count+1; i++ )
	{
		point_x [i] =  width * (GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][i-1]) / 1000.0 - sample_start) / sample_range ;
		point_y [i] =  ref_ampl_offset * height * (GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][i-1] ) / 100000.0) ;
	}

	point_x[point_count+1] = width ;
	point_y[point_count+1] = point_y[point_count] ;

	cairo_set_source_rgb (cr, 0, 255 ,0);
	cairo_set_line_width (cr, 0.5);

	for(j = 0; j< 5 ; j++)
	{
		for(i = 0; i<= point_count; i++)
		{

			_nStepPix1 = height -  _nStepRate[j] * point_y[i] ;
			_nStepPix2 = height -  _nStepRate[j] * point_y[i+1] ;
			cairo_move_to(cr, 20 + point_x[i],  _nStepPix1 );
			cairo_line_to(cr, 20 + point_x[i+1], _nStepPix2);
		}
	}
	cairo_stroke(cr) ;
	// draw point rectangle
	cairo_set_source_rgb (cr, 0.3, 0.3, 0.3 );
	for(i = 1 ; i <= point_count ; i++)
	{
		if( (i-1) == point_pos) continue ;
		cairo_rectangle(cr , 17 + point_x[i], height - point_y[i] - 3, 6 ,6 );
		cairo_fill(cr);
	}
	cairo_stroke(cr);
	cairo_set_source_rgb (cr, 0.6, 0, 0 );
	cairo_rectangle(cr , 17 + point_x[point_pos + 1], height - point_y[point_pos + 1] - 3, 6 ,6 );
	cairo_fill(cr);
	cairo_stroke(cr);
}
// dac A SCAN R  window
void draw_dac_curve_vertical(cairo_t *cr, int width, int height , int grp)
{

	int i, j , draw_start   , draw_range;
	double ref_ampl_offset;
	double min_angle , max_angle , v_scale;
	double point_x[18] , point_y[18] ;

	int _nBeamNo = TMP(beam_num[grp])  ;
    double sample_start = group_get_start (grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	if( LAW_VAL_POS(grp, Focal_type) == AZIMUTHAL_SCAN )
	{
		if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
			min_angle = 0.0;
		else
			min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0     ;
		double temp_range = (sample_start + sample_range) * cos( min_angle ) - sample_start * cos( max_angle )   ;
		draw_start = height * sample_start *( cos_current_angle - cos(max_angle)) / temp_range                   ;
		v_scale =  (sample_range * cos_current_angle) / temp_range       ;
		draw_range = v_scale * height                                    ;
	}
	else
	{
		draw_start =  0 ;
		draw_range =  height ;
	}
	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	int point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos)            ;


	ref_ampl_offset = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) / 2000.0 ) ;

	point_x[0] = ( width * GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) / 1000.0 ) * ref_ampl_offset ;
	point_y[0] = draw_start ;

	double _nStepRate[5] ;
	_nStepRate[0] = 1 ;
	_nStepRate[1] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 2000.0);
	_nStepRate[2] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 1000.0);
	_nStepRate[3] = 1 / _nStepRate[1] ;
	_nStepRate[4] = 1 / _nStepRate[2] ;

	for( i = 1; i <= point_count; i++ )
	{
		point_y [i] =  draw_start + draw_range * (GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][i-1]) / 1000.0 - sample_start) / sample_range ;
		point_x [i] =  ref_ampl_offset * width * (GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][i-1] ) / 100000.0) ;
	}

	point_y[point_count+1] = draw_start + draw_range ;
	point_x[point_count+1] = point_x[point_count] ;

	cairo_set_source_rgb (cr, 0, 1 ,0);
	cairo_set_line_width (cr, 0.5);

	double _nStepPix1, _nStepPix2 ;
	for(j = 0; j< 5 ; j++)
	{
		for(i = 0; i<= point_count; i++)
		{

			_nStepPix1 = _nStepRate[j] * point_x[i] ;
			_nStepPix2 = _nStepRate[j] * point_x[i+1] ;
			cairo_move_to(cr, 20 + _nStepPix1,  point_y[i] );
			cairo_line_to(cr, 20 + _nStepPix2, point_y[i+1]);
		}
	}
	cairo_stroke(cr) ;
	// draw point rectangle
	cairo_set_source_rgb (cr, 0.3, 0.3, 0.3 );
	for(i = 1 ; i <= point_count ; i++)
	{
		if( (i-1) == point_pos) continue ;
		cairo_rectangle(cr , point_x[i] + 17 , point_y[i] - 3, 6 ,6 );
		cairo_fill(cr);
	}
	cairo_stroke(cr);
	cairo_set_source_rgb (cr, 0.6, 0, 0 );
	cairo_rectangle(cr , point_x[point_pos + 1] + 17 , point_y[point_pos + 1] - 3, 6 ,6 );
	cairo_fill(cr);
	cairo_stroke(cr);
}

// linear dac   A SCAN  window
void draw_linear_dac_curve_horizontal(cairo_t *cr, int width, int height , int grp )
{
	double point_x[18] , point_y[18] , ref_ampl_offset;
	int i, j ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0;

	int point_count = 10 ;
	ref_ampl_offset = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) / 2000.0 ) ;

	point_y[0] = ( height * GROUP_VAL_POS(grp , SizingCurves.linear_ref_ampl) / 1000.0 ) * ref_ampl_offset ;
	point_x[0] = 0 ;

    double _nStepRate[5] ;
	_nStepRate[0] = 1 ;
	_nStepRate[1] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 2000.0);
	_nStepRate[2] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 1000.0);
	_nStepRate[3] = 1 / _nStepRate[1] ;
	_nStepRate[4] = 1 / _nStepRate[2] ;

	for( i = 1; i < point_count+1; i++ )
	{
		point_x [i] =  width * (GROUP_VAL_POS(grp , SizingCurves.linearposition[i-1]) / 1000.0 - sample_start) / sample_range ;
		point_y [i] =  ref_ampl_offset * height * (GROUP_VAL_POS(grp ,  SizingCurves.linearamplitude[i-1] ) / 100000.0) ;
	}

	point_x[point_count+1] = width ;
	point_y[point_count+1] = point_y[point_count] ;

	cairo_set_source_rgb (cr, 0, 1 ,0);
	cairo_set_line_width (cr, 0.5);
	// draw lines
	double _nStepPix1 , _nStepPix2 ;
	for(j = 0; j< 5 ; j++)
	{
		for(i = 0; i<= point_count; i++)
		{

			_nStepPix1 = height -  _nStepRate[j] * point_y[i] ;
			_nStepPix2 = height -  _nStepRate[j] * point_y[i+1] ;
			cairo_move_to(cr, 20 + point_x[i],  _nStepPix1 );
			cairo_line_to(cr, 20 + point_x[i+1], _nStepPix2);
		}
	}
	cairo_stroke(cr) ;
}
// linear dac A SCAN R  window
void draw_linear_dac_curve_vertical(cairo_t *cr, int width, int height , int grp)
{
	double point_x[18] ,  point_y[18] ;
	int i, j ;
	double ref_ampl_offset , min_angle , max_angle , v_scale ;
	int draw_start  , draw_range   ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	if( LAW_VAL_POS(grp, Focal_type) == AZIMUTHAL_SCAN )
	{
		if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
			min_angle = 0.0;
		else
			min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0     ;
		double temp_range = (sample_start + sample_range) * cos( min_angle ) - sample_start * cos( max_angle )   ;
		draw_start = height * sample_start *( cos_current_angle - cos(max_angle)) / temp_range                   ;
		v_scale =  (sample_range * cos_current_angle) / temp_range       ;
		draw_range = v_scale * height                                    ;
	}
	else
	{
		draw_start =  0 ;
		draw_range =  height ;
	}
	int point_count = 10 ;

	ref_ampl_offset = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) / 2000.0 ) ;

	point_x[0] = ( width * GROUP_VAL_POS(grp , SizingCurves.linear_ref_ampl) / 1000.0 ) * ref_ampl_offset ;
	point_y[0] = draw_start ;
	double _nStepRate[5] ;
	_nStepRate[0] = 1 ;
	_nStepRate[1] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 2000.0);
	_nStepRate[2] = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 1000.0);
	_nStepRate[3] = 1 / _nStepRate[1] ;
	_nStepRate[4] = 1 / _nStepRate[2] ;

	for( i = 1; i <= point_count; i++ )
	{
		point_y [i] =  draw_start + draw_range * (GROUP_VAL_POS(grp , SizingCurves.linearposition[i-1]) / 1000.0 - sample_start) / sample_range ;
		point_x [i] =  ref_ampl_offset * width * (GROUP_VAL_POS(grp ,  SizingCurves.linearamplitude[i-1] ) / 100000.0) ;
	}

	point_y[point_count+1] = draw_start + draw_range ;
	point_x[point_count+1] = point_x[point_count] ;
	// some times the start point will smaller than draw start
	// so do the action below to avoid this situation
	if(point_y[0] > point_y[1])  point_y[0] = point_y[1] ;

	cairo_set_source_rgb (cr, 0, 1 ,0);
	cairo_set_line_width (cr, 0.5);
	// draw lines
	double _nStepPix1, _nStepPix2 ;
	for(j = 0; j< 5 ; j++)
	{
		for(i = 0; i<= point_count; i++)
		{

			_nStepPix1 = _nStepRate[j] * point_x[i] ;
			_nStepPix2 = _nStepRate[j] * point_x[i+1] ;
			cairo_move_to(cr, 20 + _nStepPix1,  point_y[i] );
			cairo_line_to(cr, 20 + _nStepPix2, point_y[i+1]);
		}
	}
	cairo_stroke(cr) ;
}


// tcg  A SCAN window
void draw_tcg_curve_horizontal(cairo_t *cr , int width , int height , int grp)
{
	double point_x[18] ;
	double point_y[18] ;

	int i ;
	int _nBeamNo = TMP(beam_num[grp]) ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0;

	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	double ref_ampl_offset = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) / 2000.0 ) ;
	int point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos)            ;
	point_y[0] = ref_ampl_offset * height * GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo])/1000.0 ;
	point_x[0] = 0 ;
	cairo_set_source_rgb (cr, 0, 255 , 0);
	cairo_set_line_width (cr, 0.5);
	for( i = 1; i < point_count+1; i++ )
	{
		point_x [i] =  width * (GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][i-1]) / 1000.0 - sample_start) / sample_range ;
		point_y [i] =  ref_ampl_offset * height * (GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][i-1] ) / 100000.0) ;
	}
	point_x[point_count+1] = width ;
	point_y[point_count+1] = point_y[point_count] ;

	for(i = point_count+1; i>0 ; i-- )
	{
		point_y[i] = fabs(point_y[i] - point_y[0]) ;
	}
	point_y[0] = 0;

	for(i = 0; i<= point_count; i++)
	{
		cairo_move_to(cr, 20 + point_x[i],   height - point_y[i] ) ;
		cairo_line_to(cr, 20 + point_x[i+1], height - point_y[i+1]);
		cairo_stroke(cr);
	}

	cairo_set_source_rgb (cr, 0.3, 0.3, 0.3 );
	for(i = 1 ; i <= point_count ; i++)
	{
		if((i-1) == point_pos) continue ;
		cairo_rectangle(cr , 17 + point_x[i], height - point_y[i] - 3, 6 ,6 );
		cairo_fill(cr);
	}
	cairo_stroke(cr);
	cairo_set_source_rgb (cr, 0.6, 0, 0 );
	cairo_rectangle(cr , 17 + point_x[point_pos + 1], height - point_y[point_pos + 1] - 3, 6 ,6 );
	cairo_fill(cr);
	cairo_stroke(cr);

}
// tcg A SCAN R window
void draw_tcg_curve_vertical(cairo_t *cr , int width , int height , int grp)
{
	double point_x[18] ;
	double point_y[18] ;
	int    i 		   ;
	double interval    ;
	double tmp         ;
	double ref_ampl_offset;

	double min_angle ;
	double max_angle ;
	int draw_start   ;
	int draw_range   ;
	double v_scale   ;
	int _nBeamNo = TMP(beam_num[grp]) ;
    double sample_start = group_get_start(grp) / 1000.0 ;
    double sample_range = group_get_range(grp) / 1000.0 ;
	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	int point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos)            ;
	if( LAW_VAL_POS(grp, Focal_type) == AZIMUTHAL_SCAN )
	{
		if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
			min_angle = 0.0;
		else
			min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0     ;
		double temp_range = (sample_start + sample_range) * cos( min_angle ) - sample_start * cos( max_angle )   ;
		draw_start = height * sample_start *( cos_current_angle - cos(max_angle)) / temp_range                   ;
		v_scale =  (sample_range * cos_current_angle) / temp_range       ;
		draw_range = v_scale * height                                    ;
	}
	else
	{
		draw_start =  0 ;
		draw_range =  height ;
	}
	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	ref_ampl_offset = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) / 2000.0 ) ;

	point_x[0] = 20 + ( width * GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) / 1000.0 ) * ref_ampl_offset ;
	point_y[0] = draw_start ;
	tmp = pow( 10.0, GROUP_VAL_POS(grp , SizingCurves.curve_step) / 2000.0);
	interval   = point_x[0] * (tmp-1) ;

	for( i = 1; i <= point_count; i++ )
	{
		point_y [i] =  draw_start + draw_range * (GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][i-1]) / 1000.0 - sample_start) / sample_range ;
		point_x [i] =  20 + ref_ampl_offset * width * (GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][i-1] ) / 100000.0) ;
	}

	point_y[point_count+1] = draw_start + draw_range ;
	point_x[point_count+1] = point_x[point_count] ;

	for(i = point_count+1; i>0 ; i-- )
	{
		point_x[i] = 20 + fabs(point_x[i] - point_x[0]) ;
	}
	point_x[0] = 20;

	cairo_set_source_rgb (cr, 0, 1 , 0);
	cairo_set_line_width (cr, 0.5);
	for(i = 0; i<= point_count; i++)
	{
		cairo_move_to(cr, point_x[i],   point_y[i] ) ;
		cairo_line_to(cr, point_x[i+1], point_y[i+1]);
		cairo_stroke(cr);
	}

	cairo_set_source_rgb (cr, 0.3, 0.3, 0.3 );
	for(i = 1 ; i <= point_count ; i++)
	{
		if((i - 1) == point_pos) continue ;
		cairo_rectangle(cr , point_x[i] - 3 , point_y[i] - 3, 6 ,6 );
		cairo_fill(cr);
	}
	cairo_stroke(cr);
	cairo_set_source_rgb (cr, 0.6, 0, 0 );
	cairo_rectangle(cr , point_x[point_pos + 1]-3, point_y[point_pos + 1] - 3, 6 ,6 );
	cairo_fill(cr);
	cairo_stroke(cr);

}


void draw_dac_curve (cairo_t *cr , int width , int height , gpointer data)
{
	DRAW_AREA_P ui = (DRAW_AREA_P)(data) ;

	switch (ui->scan_type)
	{
		case A_SCAN:
			draw_dac_curve_horizontal( cr , width, height , ui->group);
			break;
		case A_SCAN_R:
			draw_dac_curve_vertical(cr , width , height , ui->group ) ;
			break;
		default:break;
	}
}

void draw_tcg_curve (cairo_t *cr , int width , int height , gpointer data)
{
	DRAW_AREA_P ui = (DRAW_AREA_P)(data) ;

	switch (ui->scan_type)
	{
		case A_SCAN:
		case A_SCAN_CLB:
			draw_tcg_curve_horizontal( cr , width, height ,ui->group);
			break;
		case A_SCAN_R:
			draw_tcg_curve_vertical(cr , width , height , ui->group ) ;
			break;
		default:break;
	}
}

void draw_linear_dac_curve (cairo_t *cr , int width , int height , gpointer data)
{
	DRAW_AREA_P ui = (DRAW_AREA_P)(data) ;

	switch (ui->scan_type)
	{
		case A_SCAN:
			draw_linear_dac_curve_horizontal( cr , width, height ,ui->group );
			break;
		case A_SCAN_R:
			draw_linear_dac_curve_vertical(cr , width , height , ui->group ) ;
			break;
		default:break;
	}
}

void Draw_TCG_DAC(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h;
	gtk_widget_get_size_request (widget, &w, &h);

	DRAW_AREA_P ui = (DRAW_AREA_P)(data) ;
	if(!(ui->mask & 0x01)) h  = h - 20 ;
	if(!(ui->mask & 0x02)) w = w - 20  ;
	if(!(ui->mask & 0x04)) w = w - 10  ;
	if(!(ui->mask & 0x08)) w = w - 20  ;

	int i = GROUP_VAL_POS( ui->group , SizingCurves.curve_pos);
	switch(i)
	{
		case 1:// DAC
			draw_dac_curve ( cr, w, h , data)   ;
			break;
		case 2:// Linear DAC
			draw_linear_dac_curve ( cr, w, h , data)   ;
			break;
		case 3:// TCG
			draw_tcg_curve (cr , w , h , data)  ;
			break;
		default:
			break;
	}
}

void DrawWedgeDelayTolerance(GtkWidget* widget , gpointer data , cairo_t* cr)
{

	gint w,h;
	double y1  , y2;

	gtk_widget_get_size_request (widget, &w, &h);

	DRAW_AREA_P ui = (DRAW_AREA_P)(data) ;
	if(!(ui->mask & 0x01)) h  = h - 20 ;
	if(!(ui->mask & 0x02)) w = w - 20  ;
	if(!(ui->mask & 0x04)) w = w - 10  ;

	int grp = ui->group ;
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	double gate_width  = GROUP_VAL_POS(grp , gate[0].width) ;
	double gate_start  = GROUP_VAL_POS(grp , gate[0].start) ;
	double vel = (get_group_val (p_grp, GROUP_VELOCITY) / 100.0);// m/s
	double t1  = 2000000 * TMP_CBA(distance) / vel ;//ns

	double tolerance = pp->tolerance / 100.0 ;
	tolerance = 2000000 * tolerance / vel ;

	y1 = t1 - tolerance ;
	y2 = t1 + tolerance ;

	y1 = h - h * (y1 - gate_start) / gate_width ;
	y2 = h - h * (y2 - gate_start) / gate_width ;
	cairo_set_line_width(cr, 0.5);
	cairo_set_source_rgba(cr, 0 , 1.0 , 0 , 1.0);	/* I闸门为黄色 */

	y1 = (int)y1 + 0.5 ;
	y2 = (int)y2 + 0.5 ;
	cairo_move_to(cr, 30 , y1  );
	cairo_line_to(cr, w - 10   , y1 );

	cairo_move_to(cr, 30 , y2 + 0.5);
	cairo_line_to(cr, w - 10   , y2 );
	cairo_stroke (cr);

}

void DrawSensationTolerance(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	gint w,h;
	double y1  , y2;

	gtk_widget_get_size_request (widget, &w, &h);

	DRAW_AREA_P ui = (DRAW_AREA_P)(data) ;
	if(!(ui->mask & 0x01)) h  = h - 20 ;
	if(!(ui->mask & 0x02)) w = w - 20  ;
	if(!(ui->mask & 0x04)) w = w - 10  ;

	double tolerance = pp->tolerance_t / 100.0 ;
	double t1        = pp->ref_amplitude / 100.0    ;

	y1 = (t1 - tolerance)/100.0 ;
	y2 = (t1 + tolerance)/100.0 ;

	y1 = h - y1 * h ;
	y2 = h - y2 * h ;
	cairo_set_line_width(cr, 0.3);
	cairo_set_source_rgba(cr, 0 , 1.0 , 0 , 1.0);	/* I闸门为黄色 */

	y1 = (int)y1 + 0.5 ;
	y2 = (int)y2 + 0.5 ;
	cairo_move_to(cr, 30 , y1 );
	cairo_line_to(cr, w - 10   , y1);

	cairo_move_to(cr, 30 , y2 );
	cairo_line_to(cr, w - 10   , y2);
	cairo_stroke (cr);

}

// draw left ruler
void DrawLeftRuler(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	gchar *str = NULL;
    int i , _nStartIndex , _nPos;
    int w , h;
    int _nDirection ;
    double _nStart ;
    double _nStop  ;

    double _nInterval ;
    double _nPixelPerUnit ;
    double _nCurrentPos   ;
    int    _nDisplayDecimal  = 0;
    int    _nMarkQty ;
    DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	gtk_widget_get_size_request (widget, &w, &h);

	/* mask = 二进制10000 则不显示 label  15 pixels*/
	/* mask = 二进制01000 则不显示 hruler1 20 pixels*/
	/* mask = 二进制00100 则不显示 调色条   10 pixels*/
	/* mask = 二进制00010 则不显示 hruler2 20 pixels*/
	/* mask = 二进制00001 则不显示 wruler1  20 pixels*/
	if(!(prule->mask & 0x01)) h  = h - 20  ;
	// draw bottom ruler range
	cairo_set_source_rgba(cr,((prule->h1_color)>>16)/255.0,
	                         (((prule->h1_color)>>8)&0x00FF)/255.0,
	                         ((prule->h1_color)&0xFF)/255.0, 1.0);
	cairo_rectangle(cr,0.0,0.0,20.0,h);		/* hruler1 */
	cairo_fill (cr);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr , 8);
	cairo_set_line_width(cr, 0.5);

	// adjust the mark count to the width of display window
	if(h >= 300)                  _nMarkQty = 100;
	else if ((h>200)&&(h<300))    _nMarkQty = 60;
	else if ((h>100)&&(h<200))    _nMarkQty = 40;
	else                          _nMarkQty = 20;

	if(prule->hmax1 == prule->hmin1)
	{// avoid float exception
		return ;
	}
	if(prule->hmax1 < prule->hmin1)
	{
	   _nDirection = 1 ;
	   _nStart = prule->hmax1 ;
	   _nStop  = prule->hmin1 ;
	}
	else
	{
	   _nDirection = 0 ;
	   _nStart = prule->hmin1 ;
	   _nStop  = prule->hmax1 ;
	}
	_nPixelPerUnit = h / (_nStop - _nStart)  ;
	_nInterval = (_nStop - _nStart) / _nMarkQty  ;
	// get proper mark interval
	if      (_nInterval < 0.01 )                      {_nInterval = 0.01 ;   _nDisplayDecimal  = 1 ;}
	else if ( 0.01 <=_nInterval  && _nInterval <= 0.05) {_nInterval = ((int)(_nInterval * 100)) / 100.0 ; _nDisplayDecimal  = 1 ;}
	else if ( 0.05 <_nInterval  && _nInterval <= 0.1)  {_nInterval = 0.1 /*((int)(_nInterval * 100)) / 100.0 ; _nDisplayDecimal  = 1*/ ;}
	else if ( 0.1  <_nInterval  && _nInterval < 1.0)    _nInterval = ((int)(_nInterval * 10)) / 10.0 ;
	else if (_nInterval >= 1.0 && _nInterval <= 5.0)   _nInterval = (int)  _nInterval ;
	else if (_nInterval > 5    && _nInterval <= 10 )   _nInterval = 10 ;
	else if (_nInterval > 10   && _nInterval < 50)     _nInterval = 10 * (1 + (int)(_nInterval / 10)) ;
	else if (_nInterval >= 50  && _nInterval <= 100)   _nInterval = 100 ;
	else if (_nInterval > 100  && _nInterval <= 200)   _nInterval = 200 ;
	else if (_nInterval > 200)                         _nInterval = 100 * (1 + (int)(_nInterval / 100)) ;

    _nStartIndex = _nStart / _nInterval ;
    if(_nStart > 0)
    {
        _nStartIndex +=  1 ;
    }
    _nCurrentPos = _nStart  ;
    for(i = _nStartIndex ; _nCurrentPos < _nStop ; i++)
    {
        _nCurrentPos = i * _nInterval ;
        _nPos = (_nCurrentPos - _nStart) * _nPixelPerUnit ;
        if(_nDirection)
        {
           _nPos  = h - _nPos - 1 ;
        }
		if(i % 10 == 0)
		{
			cairo_save(cr);/* 保存当前画笔 */
			cairo_move_to(cr,  5, _nPos + 0.5);
			cairo_line_to(cr, 20, _nPos + 0.5);
			cairo_stroke(cr);
			cairo_move_to(cr,14,_nPos - 3.5);
			cairo_rotate(cr, G_PI * 3/2);		/*旋转270度*/
			str=g_strdup_printf("%0.*f", _nDisplayDecimal , _nCurrentPos );
			cairo_show_text(cr , str);
			cairo_restore(cr);/* 恢复当前画笔 */
			g_free (str);
		}
		else if(i%5 == 0)
		{
			cairo_move_to(cr , 10 , _nPos + 0.5);
			cairo_line_to(cr , 20 , _nPos + 0.5);
			cairo_stroke(cr);
		}
		else
		{
			cairo_move_to(cr , 15 , _nPos + 0.5);
			cairo_line_to(cr , 20 , _nPos + 0.5);
			cairo_stroke (cr);
		}
    }

    cairo_save(cr);/* 保存当前画笔 */
	cairo_move_to(cr , 6 , h-10);
	cairo_rotate(cr, G_PI*3/2);		/*旋转270度*/
	cairo_show_text(cr,units[prule->h1_unit]);   		/*标签*/
	cairo_restore(cr);/* 恢复当前画笔 */
	//cairo_stroke(cr);

}

void DrawBottomRuler(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	gchar *str = NULL;
    int i , _nStartIndex , _nPos;
    int w , h;
    int _nDirection ;
    double _nStart ;
    double _nStop  ;
    int    _nDisplayDecimal  = 0;

    double _nInterval ;
    double _nPixelPerUnit ;
    double _nCurrentPos   ;

    int    _nMarkQty ;
    DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	gtk_widget_get_size_request (widget, &w, &h);

	/* mask = 二进制10000 则不显示 label  15 pixels*/
	/* mask = 二进制01000 则不显示 hruler1 20 pixels*/
	/* mask = 二进制00100 则不显示 调色条   10 pixels*/
	/* mask = 二进制00010 则不显示 hruler2 20 pixels*/
	/* mask = 二进制00001 则不显示 wruler1  20 pixels*/
	if(!(prule->mask & 0x01)) h  = h - 20  ;
	if(!(prule->mask & 0x02)) w  = w - 20  ;
	if(!(prule->mask & 0x04)) w  = w - 10  ;
	if(!(prule->mask & 0x08)) w  = w - 20  ;

	// draw bottom ruler range
	cairo_set_line_width(cr, 0.5);
	cairo_set_source_rgba(cr,((prule->w_color)>>16)/255.0,
	                         (((prule->w_color)>>8)&0x00FF)/255.0,
	                         ((prule->w_color)&0xFF)/255.0, 1.0);
	cairo_rectangle(cr , 20.0, h , w , 20.0);	/* wruler */
	cairo_fill (cr) ;
	cairo_stroke(cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr , 8);
	// adjust the mark count to the width of display window
	if(w >= 400)                  _nMarkQty = 100;
	else if ((w>300)&&(w<400))    _nMarkQty = 80;
	else if ((w>200)&&(w<300))    _nMarkQty = 50;
	else if ((w>100)&&(w<200))    _nMarkQty = 40;
	else                          _nMarkQty = 20;

	if(prule->wmax1 == prule->wmin1)
	{// avoid float exception
		return ;
	}
	if(prule->wmax1 < prule->wmin1)
	{
	   _nDirection = 1 ;
	   _nStart = prule->wmax1 ;
	   _nStop  = prule->wmin1 ;
	}
	else
	{
	   _nDirection = 0 ;
	   _nStart = prule->wmin1 ;
	   _nStop  = prule->wmax1 ;
	}
	_nPixelPerUnit = w / (_nStop - _nStart)  ;
	_nInterval = (_nStop - _nStart) / _nMarkQty  ;
	// get proper mark interval
	// _nDisplayDecimal  = 0 ;
	if      (_nInterval < 0.01 )                      {_nInterval = 0.01 ;   _nDisplayDecimal  = 1 ;}
	else if ( 0.01 <=_nInterval  && _nInterval < 0.1)  {_nInterval = ((int)(_nInterval * 100)) / 100.0 ; _nDisplayDecimal  = 1 ;}
	else if ( 0.1 <=_nInterval  && _nInterval < 1.0)   _nInterval = ((int)(_nInterval * 10)) / 10.0 ;
	else if (_nInterval >= 1.0 && _nInterval <= 5.0)  _nInterval = (int)  _nInterval ;
	else if (_nInterval > 5    && _nInterval <= 10 )  _nInterval = 10 ;
	else if (_nInterval > 10   && _nInterval < 50)    _nInterval = 10 * (1 + (int)(_nInterval / 10)) ;
	else if (_nInterval >= 50  && _nInterval <= 100)  _nInterval = 100 ;
	else if (_nInterval > 100  && _nInterval <= 200)  _nInterval = 200 ;
	else if (_nInterval > 200)                        _nInterval = 100 * (1 + (int)(_nInterval / 100)) ;

	//if(prule->w_unit == UNIT_INCH && _nInterval == 0.1)   _nInterval = 0.01 ;
    _nStartIndex = _nStart / _nInterval ;
    if(_nStart > 0)
    {
        _nStartIndex +=  1 ;
    }
    _nCurrentPos = _nStart  ;
    for(i = _nStartIndex ; _nCurrentPos < _nStop ; i++)
    {
        _nCurrentPos = i * _nInterval ;
        _nPos = (_nCurrentPos - _nStart) * _nPixelPerUnit ;
        if(_nDirection)
        {
           _nPos  = w - _nPos ;
        }
        _nPos += 20 ;
		if(i%10 == 0)
		{
			cairo_move_to(cr , _nPos + 0.5 , h);
			cairo_line_to(cr , _nPos + 0.5 , h + 15);
			cairo_stroke (cr);
			cairo_move_to(cr , 3.5 + _nPos , h + 12);
			//if(prule->w_unit == UNIT_INCH)
			//	str = g_strdup_printf("%.1f", _nCurrentPos);
			//else
				str = g_strdup_printf("%0.*f",_nDisplayDecimal , _nCurrentPos);
			cairo_show_text(cr,str);   		/*标签*/
			cairo_stroke(cr);
			g_free (str);
		}
		else if(i%5 == 0)
		{
			cairo_move_to(cr, _nPos + 0.5 , h);
			cairo_line_to(cr, _nPos + 0.5 , h+10);
			cairo_stroke(cr);
		}
		else
		{
			cairo_move_to( cr, _nPos + 0.5, h);
			cairo_line_to( cr, _nPos + 0.5, h+5);
			cairo_stroke(cr);
		}
    }

    str=g_strdup_printf("%s", units[prule->w_unit]);
	cairo_move_to(cr , 30 , h + 20);
	cairo_show_text(cr,str);   		/*标签*/
	cairo_stroke(cr);
	g_free (str);

}

void DrawRightRuler(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	gchar *str = NULL;
    int i , _nStartIndex , _nPos;
    int w , h;
    int _nDirection ;
    double _nStart ;
    double _nStop  ;

    double _nInterval ;
    double _nPixelPerUnit ;
    double _nCurrentPos   ;

    int    _nMarkQty ;
    DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	gtk_widget_get_size_request (widget, &w, &h);

	/* mask = 二进制10000 则不显示 label  15 pixels*/
	/* mask = 二进制01000 则不显示 hruler1 20 pixels*/
	/* mask = 二进制00100 则不显示 调色条   10 pixels*/
	/* mask = 二进制00010 则不显示 hruler2 20 pixels*/
	/* mask = 二进制00001 则不显示 wruler1  20 pixels*/
	if(!(prule->mask & 0x01)) h  = h - 20  ;

	w = w - 20 ;
	// draw bottom ruler range
	cairo_set_source_rgba(cr,((prule->h2_color)>>16)/255.0, (((prule->h2_color)>>8)&0x00FF)/255.0, ((prule->h2_color)&0xFF)/255.0, 1.0);
	cairo_set_line_width(cr, 0.5);
	cairo_rectangle(cr, w , 0.0 , 20.0 , h);	/* hruler2 */
	cairo_fill (cr);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr,8);

	// adjust the mark count to the width of display window
	if(h >= 300)                  _nMarkQty = 100;
	else if ((h>100)&&(h<300))    _nMarkQty = 40;
	else                          _nMarkQty = 20;

	if(prule->hmax2 == prule->hmin2)
	{// avoid float exception
		return ;
	}
	if(prule->hmax2 < prule->hmin2)
	{
	   _nDirection = 1 ;
	   _nStart = prule->hmax2 ;
	   _nStop  = prule->hmin2 ;
	}
	else
	{
	   _nDirection = 0 ;
	   _nStart = prule->hmin2 ;
	   _nStop  = prule->hmax2 ;
	}
	_nPixelPerUnit = h / (_nStop - _nStart)  ;
	_nInterval = (_nStop - _nStart) / _nMarkQty  ;
	// get proper mark interval
	if      (_nInterval < 0.1 )                       _nInterval = 0.1 ;
	else if ( 0.1 <_nInterval  && _nInterval < 1.0)   _nInterval = ((int)(_nInterval * 10)) / 10.0 ;
	else if (_nInterval >= 1.0 && _nInterval <= 5.0)  _nInterval = (int)  _nInterval ;
	else if (_nInterval > 5    && _nInterval <= 10 )  _nInterval = 10 ;
	else if (_nInterval > 10   && _nInterval < 50)    _nInterval = 10 * (1 + (int)(_nInterval / 10)) ;
	else if (_nInterval >= 50  && _nInterval <= 100)  _nInterval = 100 ;
	else if (_nInterval > 100  && _nInterval <= 200)  _nInterval = 200 ;
	else if (_nInterval > 200)                        _nInterval = 100 * (1 + (int)(_nInterval / 100)) ;

    _nStartIndex = _nStart / _nInterval ;
    if(_nStart > 0)
    {
        _nStartIndex +=  1 ;
    }
    _nCurrentPos = _nStart  ;
    for(i = _nStartIndex ; _nCurrentPos < _nStop ; i++)
    {
        _nCurrentPos = i * _nInterval ;
        _nPos = (_nCurrentPos - _nStart) * _nPixelPerUnit ;
        if(_nDirection)
        {
           _nPos  = h - _nPos - 1 ;
        }
		if(i % 10 == 0)
		{
			cairo_save(cr);/* 保存当前画笔 */
			cairo_move_to(cr,  w, _nPos + 0.5);
			cairo_line_to(cr, w+15, _nPos + 0.5);
			cairo_stroke(cr);
			cairo_move_to(cr , w + 13, _nPos - 3.5);
			cairo_rotate(cr, G_PI * 3/2);		/*旋转270度*/
			str=g_strdup_printf("%.0f", _nCurrentPos );
			cairo_show_text(cr , str);
			cairo_restore(cr);/* 恢复当前画笔 */
			g_free (str);
		}
		else if(i%5 == 0)
		{
			cairo_move_to(cr , w  , _nPos + 0.5);
			cairo_line_to(cr , w + 10 , _nPos + 0.5);
			cairo_stroke(cr);
		}
		else
		{
			cairo_move_to(cr , w  , _nPos + 0.5);
			cairo_line_to(cr , w + 5 , _nPos + 0.5);
			cairo_stroke (cr);
		}
    }

    cairo_save(cr);/* 保存当前画笔 */
	cairo_move_to(cr , w+20 , h-10);
	cairo_rotate(cr, G_PI*3/2);		/*旋转270度*/
	cairo_show_text(cr,units[prule->h2_unit]);   		/*标签*/
	cairo_restore(cr);/* 恢复当前画笔 */
	//cairo_stroke(cr);

}

void DrawGrid(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	gint i,j,m,n,w,h;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;

	gtk_widget_get_size_request (widget, &w, &h);

	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	int _nMark = prule->mask ;
	if(_nMark & 0x1)
	{
		h += 20 ;
	}
	if(_nMark & 0x2)
	{
		w += 20 ;
	}
	if(_nMark & 0x4)
	{
		w += 10 ;
	}
	if(_nMark & 0x8)
	{
		w += 20 ;
	}
	if(_nMark & 0x10)
	{
		h += 15 ;
	}

	cairo_set_line_width(cr, 0.5);
	switch(get_overlay_grid(pp->p_config))
	{
		case 0:
			cairo_set_source_rgba(cr,0.0,0.5,0.5,1.0);break;
		case 1:
			cairo_set_source_rgba(cr,0.3,0.5,0.0,1.0);break;
		case 2:
			cairo_set_source_rgba(cr,0.5,0.0,0.0,1.0);break;
		case 3:
			cairo_set_source_rgba(cr,0.5,0.5,0.0,1.0);break;
		case 4:
			cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);break;
		default:break;
	}

	for (j = 0; j < (h-20); j += ((h-20) / 10.0))
	{
		for  (m = 20; m < (w-30); m += ((w-50) / 50.0))
		{
			cairo_move_to (cr, (int)(m) , (int)(j) + 0.5);
			cairo_line_to (cr, (int)(m) +1 , (int)(j) + 0.5);
			cairo_stroke (cr);
		}
	}
	for ( i = 20; i < (w-30); i += ((w-50) /10.0))
	{
		for (n = 0; n < (h-20); n += ((h-20) / 50.0))
		{
			cairo_move_to (cr, (int)(i) , (int)(n) + 0.5);
			cairo_line_to (cr, (int)(i) +1 , (int)(n) + 0.5);
			cairo_stroke(cr);
		}
	}
}

void DrawCurrentLineMarkSscanSoundPath(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w , h ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	gtk_widget_get_size_request (widget, &w, &h);
	int _nGroupId = prule->group ;
	if(GROUP_VAL_POS(_nGroupId ,  group_mode) != PA_SCAN)
	{
	    return ;
	}
	int _nBeamQty = TMP(beam_qty[_nGroupId]) ;
	int _nBeamNo  = TMP(beam_num[_nGroupId]) ;
	int _nXoffset = 20 ;
	int _nYoffset      ;

	if(!(prule->mask & 0x01)) h  = h - 20  ;
	if(!(prule->mask & 0x02)) w  = w - 20  ;
	if(!(prule->mask & 0x04)) w  = w - 10  ;
	if(!(prule->mask & 0x08)) w  = w - 20  ;

    _nYoffset = (int)((_nBeamNo + 0.5) * h / _nBeamQty) ;

	cairo_set_line_width(cr, 0.3);
	cairo_set_source_rgba(cr, 0 , 0, 1, 1.0);	/* 标尺及单位颜色为黑色 */
	cairo_move_to(cr, _nXoffset , _nYoffset + 0.5) ;
	cairo_line_to(cr, _nXoffset + w , _nYoffset + 0.5);
	cairo_stroke(cr);


}
// 当前聚焦法则光标显示。
void DrawCurrentLineMarkSscanTrueDepth(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int _nXoffset  = 20 ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	double _x1 , _x2 , _y1 , _y2 ;

	int _nGroupId = prule->group   ;
	int _nBeamNo = TMP(beam_num[_nGroupId]) ;
	if(GROUP_VAL_POS(_nGroupId ,  group_mode) != PA_SCAN)
	{
	    return ;
	}
	_x1 = CurrentLine_top[_nGroupId][_nBeamNo].x + _nXoffset + 0.5;
	_y1 = CurrentLine_top[_nGroupId][_nBeamNo].y  ;
	_x2 = CurrentLine_bottom[_nGroupId][_nBeamNo].x + _nXoffset + 0.5;
    _y2 = CurrentLine_bottom[_nGroupId][_nBeamNo].y ;

	cairo_set_line_width(cr, 0.3);
	cairo_set_source_rgba(cr, 0 , 0, 1 ,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_move_to(cr, _x1  , _y1 );
	cairo_line_to(cr, _x2  , _y2 );
	cairo_stroke(cr);

#ifdef X86
	int i;
	cairo_set_line_width(cr, 0.3);
	cairo_set_source_rgba(cr, 1 , 1, 1 ,1.0);
	for(i = 0 ; i< TMP(beam_qty[_nGroupId]) ; i++)
	{
		_x1 = CurrentLine_top[_nGroupId][i].x + _nXoffset;
		_y1 = CurrentLine_top[_nGroupId][i].y ;
		_x2 = CurrentLine_bottom[_nGroupId][i].x + _nXoffset;
	    _y2 = CurrentLine_bottom[_nGroupId][i].y ;
		cairo_move_to(cr, _x1 + 0.5 , _y1 );
		cairo_line_to(cr, _x2 + 0.5 , _y2 );
		cairo_stroke(cr);
	}
#endif
}
void DrawCurrentScanMarkBscan(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
    double _nTmpY  ;
	gtk_widget_get_size_request (widget, &w, &h);

	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	int _nMark = prule->mask ;
	if(!(_nMark & 0x01)) h  = h - 20  ;
	if(!(_nMark & 0x02)) w  = w - 20  ;
	if(!(_nMark & 0x04)) w  = w - 10  ;
	if(!(_nMark & 0x08)) w  = w - 20  ;
	int _nEncType = get_inspec_source(pp->p_config) ;
	int _nCurrentStartIndex = getCurrentBscanStart() ;
	int _nCurrentStopIndex  = getCurrentBscanStop()  ;
	int _nCurrentEncoderPos = TMP(scan) ;
	int _nCurrentIndex   ;

	if(pp->p_config->inspection_scan_resolution == 0)  return  ;
	if(_nEncType)
	{
		_nCurrentIndex = (_nCurrentEncoderPos * 10 - pp->p_config->inspection_scan_start) /  pp->p_config->inspection_scan_resolution  ;
	}
	else
	{
		_nCurrentIndex = _nCurrentEncoderPos * 10 /  pp->p_config->inspection_scan_resolution  ;
	}

	_nTmpY = (_nCurrentIndex - _nCurrentStartIndex) / ((double)(_nCurrentStopIndex - _nCurrentStartIndex)) ;
	_nTmpY = (1 - _nTmpY) * h ;
	_nTmpY = (int)_nTmpY + 0.5 ;
	static const double dashed1[] ={5.0, 2.0 , 5.0};
	cairo_set_source_rgba( cr, 1 , 0 , 0 , 1.0 );
	cairo_set_line_width(cr, 1);
	cairo_set_dash (cr, dashed1, 3, 0);
	cairo_move_to (cr, 20 , _nTmpY  );
	cairo_line_to (cr, 20 + w , _nTmpY  );
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawCurrentScanMarkTofdBscan(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
    double _nTmpX  ;
	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(!(_nMark & 0x01)) h  = h - 20  ;
	if(!(_nMark & 0x02)) w  = w - 20  ;
	if(!(_nMark & 0x04)) w  = w - 10  ;
	if(!(_nMark & 0x08)) w  = w - 20  ;
	int _nEncType = get_inspec_source(pp->p_config) ;
	int _nIndex = prule->nWindowIndex ;
	int _nCurrentStartIndex = getCurrentTofdBscanStart(_nIndex) ;
	int _nCurrentStopIndex  = getCurrentTofdBscanStop(_nIndex)  ;
	int _nCurrentEncoderPos = TMP(scan) ;
	int _nCurrentIndex   ;

	if(pp->p_config->inspection_scan_resolution == 0)  return  ;
	if(_nEncType)
	{
		_nCurrentIndex = (_nCurrentEncoderPos * 10 - pp->p_config->inspection_scan_start) /  pp->p_config->inspection_scan_resolution  ;
	}
	else
	{
		_nCurrentIndex = _nCurrentEncoderPos * 10 /  pp->p_config->inspection_scan_resolution  ;
	}

	_nTmpX = (_nCurrentIndex - _nCurrentStartIndex) / ((double)(_nCurrentStopIndex - _nCurrentStartIndex)) ;
	_nTmpX = _nTmpX * w ;
	_nTmpX = (int)_nTmpX + 20.5 ;
	static const double dashed1[] ={5.0, 2.0 , 5.0};
	cairo_set_source_rgba( cr, 1 , 0 , 0 , 1.0 );
	cairo_set_dash (cr, dashed1, 3, 0);
	cairo_set_line_width(cr, 1);
	cairo_move_to (cr, _nTmpX  , 0  );
	cairo_line_to (cr, _nTmpX ,  h  );
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawCurrentScanMarkTofdBscanVert(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
    double _nTmpY  ;
	gtk_widget_get_size_request (widget, &w, &h);

	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	int _nMark = prule->mask ;
	if(!(_nMark & 0x01)) h  = h - 20  ;
	if(!(_nMark & 0x02)) w  = w - 20  ;
	if(!(_nMark & 0x04)) w  = w - 10  ;
	if(!(_nMark & 0x08)) w  = w - 20  ;
	int _nEncType = get_inspec_source(pp->p_config) ;
	int _nIndex = prule->nWindowIndex ;
	int _nCurrentStartIndex = getCurrentTofdBscanStart(_nIndex) ;
	int _nCurrentStopIndex  = getCurrentTofdBscanStop(_nIndex)  ;
	int _nCurrentEncoderPos = TMP(scan) ;
	int _nCurrentIndex   ;

	if(pp->p_config->inspection_scan_resolution == 0)  return  ;
	if(_nEncType)
	{
		_nCurrentIndex = (_nCurrentEncoderPos * 10 - pp->p_config->inspection_scan_start) /  pp->p_config->inspection_scan_resolution  ;
	}
	else
	{
		_nCurrentIndex = _nCurrentEncoderPos * 10 /  pp->p_config->inspection_scan_resolution  ;
	}

	_nTmpY = (_nCurrentIndex - _nCurrentStartIndex) / ((double)(_nCurrentStopIndex - _nCurrentStartIndex)) ;
	_nTmpY = (1 - _nTmpY) * h + 0.5;
	static const double dashed1[] ={5.0, 2.0 , 5.0};
	cairo_set_source_rgba( cr, 1 , 0 , 0 , 1.0 );
	cairo_set_line_width(cr, 1);
	cairo_set_dash (cr, dashed1, 3, 0);
	cairo_move_to (cr, 20 , _nTmpY  );
	cairo_line_to (cr, 20 + w , _nTmpY  );
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawCurrentScanMarkCscan(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	int _nIndex = prule->nWindowIndex ;
    double _nTmpX  ;
	gtk_widget_get_size_request (widget, &w, &h);

	int _nMark = prule->mask ;
	if(!(_nMark & 0x01)) h  = h - 20  ;
	if(!(_nMark & 0x02)) w  = w - 20  ;
	if(!(_nMark & 0x04)) w  = w - 10  ;
	if(!(_nMark & 0x08)) w  = w - 20  ;
	int _nEncType = get_inspec_source(pp->p_config) ;
	int _nCurrentStartIndex = getCurrentCscanStart(_nIndex) ;
	int _nCurrentStopIndex  = getCurrentCscanStop(_nIndex)  ;
	int _nCurrentEncoderPos = TMP(scan) ;
	//printf("_nCurrentStartIndex   %d -  %d -  %d \n" , _nCurrentStartIndex , _nCurrentStopIndex , _nCurrentEncoderPos);
	int _nCurrentIndex   ;

	if(pp->p_config->inspection_scan_resolution == 0)  return  ;
	if(_nEncType)
	{
		_nCurrentIndex = (_nCurrentEncoderPos * 10 - pp->p_config->inspection_scan_start) /  pp->p_config->inspection_scan_resolution  ;
	}
	else
	{
		_nCurrentIndex = _nCurrentEncoderPos * 10 /  pp->p_config->inspection_scan_resolution  ;
	}
	//printf("_nCurrentIndex   %d \n" , _nCurrentIndex);
	_nTmpX = (_nCurrentIndex - _nCurrentStartIndex) / ((double)(_nCurrentStopIndex - _nCurrentStartIndex)) ;
	_nTmpX = _nTmpX * w ;
	_nTmpX = (int)_nTmpX + 20.5 ;
	//printf("_nTmpX   %f \n" , _nTmpX);
	static const double dashed1[] ={5.0, 2.0 , 5.0};
	cairo_set_source_rgba( cr, 1 , 0 , 0 , 1.0 );
	cairo_set_dash (cr, dashed1, 3, 0);
	cairo_set_line_width(cr, 1);
	cairo_move_to (cr, _nTmpX  , 0  );
	cairo_line_to (cr, _nTmpX ,  h  );
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawCurrentScanMarkCscanVert(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	int _nIndex = prule->nWindowIndex ;
    double _nTmpY  ;
	gtk_widget_get_size_request (widget, &w, &h);

	int _nMark = prule->mask ;
	if(!(_nMark & 0x01)) h  = h - 20  ;
	if(!(_nMark & 0x02)) w  = w - 20  ;
	if(!(_nMark & 0x04)) w  = w - 10  ;
	if(!(_nMark & 0x08)) w  = w - 20  ;
	int _nEncType = get_inspec_source(pp->p_config) ;
	int _nCurrentStartIndex = getCurrentCscanStart(_nIndex) ;
	int _nCurrentStopIndex  = getCurrentCscanStop(_nIndex)  ;
	int _nCurrentEncoderPos = TMP(scan) ;
	int _nCurrentIndex   ;

	if(pp->p_config->inspection_scan_resolution == 0)  return  ;
	if(_nEncType)
	{
		_nCurrentIndex = (_nCurrentEncoderPos * 10 - pp->p_config->inspection_scan_start) /  pp->p_config->inspection_scan_resolution  ;
	}
	else
	{
		_nCurrentIndex = _nCurrentEncoderPos * 10 /  pp->p_config->inspection_scan_resolution  ;
	}
	_nTmpY = (_nCurrentIndex - _nCurrentStartIndex) / ((double)(_nCurrentStopIndex - _nCurrentStartIndex)) ;
	_nTmpY = (1-_nTmpY)*h + 0.5;
	static const double dashed1[] ={5.0, 2.0 , 5.0};
	cairo_set_source_rgba( cr, 1 , 0 , 0 , 1.0 );
	cairo_set_dash (cr, dashed1, 3, 0);
	cairo_set_line_width(cr, 1);
	cairo_move_to (cr, 20, _nTmpY);
	cairo_line_to (cr, 20+w, _nTmpY);
	cairo_stroke(cr);
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

void DrawCurrentScanMark(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	switch (prule->scan_type)
	{
		case B_SCAN:
			DrawCurrentScanMarkBscan(widget ,  data ,  cr);
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
			DrawCurrentScanMarkCscan(widget ,  data ,  cr);
			break;
		case C_SCAN_VE:
			DrawCurrentScanMarkCscanVert(widget ,  data ,  cr);
			break;
		case TOFD_B_SCAN:
			DrawCurrentScanMarkTofdBscan(widget ,  data ,  cr);
			break;
		case B_SCAN_VERT:
			DrawCurrentScanMarkTofdBscanVert(widget ,  data ,  cr);
			break;
		default:
			break ;
	}
}

//单个显示窗口的实现
//画标尺，光标，闸门。。。。。。。。。
static gboolean draw_info(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	int _nDisplayType = prule->scan_type ;
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
	if(_nDisplayType == S_SCAN )
	{
         DrawCurrentLineMarkSscanSoundPath (widget , data , cr) ;
	}
	if( _nDisplayType == S_SCAN_L || _nDisplayType == S_SCAN_A)
	{
		 DrawCurrentLineMarkSscanTrueDepth (widget , data , cr) ;
	}
	/* 画网格 */
	if (get_overlay_grid(pp->p_config) != 5)
	{
		DrawGrid(widget , data , cr);
	}
	/* 画 overlay */
	if(get_weld_display())
	{
		DrawWeld(widget , data , cr);
	}
	if (get_overlay_overlay(pp->p_config) == 1)
	{

		DrawOverLay(widget , data , cr) ;
	}
	// 工件
    if(pp->p_config->bPartLoaded
            && (prule->scan_type == S_SCAN
                || prule->scan_type == S_SCAN_A
                || prule->scan_type == S_SCAN_L
                || prule->scan_type == A_B_S_SCAN
                || prule->scan_type == A_S_CC_SCAN)) {
        DrawWorkpiece(widget, data, cr);
	}
	/* 画cursor */
	if (get_overlay_cursor(pp->p_config) == 1)
	{
		DrawCursor(widget , data , cr) ;
	}
	/* 画闸门 gate */
	if(get_overlay_gate (pp->p_config) == 1)
	{
		DrawGate(widget,  data , cr) ;
	}
	/* 调色条信息 */
	if(_nDisplayType == WEDGE_DELAY)
	{
		DrawWedgeDelayTolerance(widget , data , cr ) ;
	}
	if(_nDisplayType == SENSITIVITY || _nDisplayType == TCG)
	{
		DrawSensationTolerance(widget , data , cr ) ;
	}
	// draw dac tcg curve
	if (GROUP_VAL_POS(prule->group , SizingCurves.curve_pos) && get_overlay_sizing_curves(pp->p_config))
	{
		Draw_TCG_DAC(widget , data , cr) ;
	}
	//current scan mark line
	if(TMP(dataRecalling))
	{
		DrawCurrentScanMark(widget , data , cr) ;
	}
	if(!(prule->mask & 0x04))
	{
		DrawColorBar(widget , data , cr ) ;
	}
	/* 画ruler */
	if (!(prule->mask & 0x08))//垂直尺左
	{
		DrawLeftRuler ( widget , data , cr ) ;
	}

	if (!(prule->mask & 0x01))//水平尺
	{
		DrawBottomRuler(widget , data , cr) ;
	}

	if (!(prule->mask & 0x02))//垂直尺右
	{
		DrawRightRuler(widget , data , cr) ;
	}

	if(_nDisplayType == FFT_A_SCAN)
	{
		DrawFftInfo(widget , data , cr) ;
	}

	cairo_destroy(cr);
	return TRUE;
}
//显示 REF DB  TCG DAC 等标识
static gboolean draw_status_info(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t* cr = gdk_cairo_create(gtk_widget_get_window(widget));//创建画笔
	gint grp = get_current_group(pp->p_config);//当前group
	// display if reference gain is enable
	cairo_select_font_face (cr, "Adobe Heiti Std",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD);
	if(get_group_db_ref (pp->p_config, grp))
	{
		cairo_set_font_size (cr, 12 );
		cairo_set_source_rgba (cr, 0, 1, 0, 1);
		cairo_move_to(cr , 2 , 16);
		cairo_show_text(cr , "Ref") ;
		cairo_stroke (cr);
	}
	cairo_set_source_rgba (cr, 0, 1, 1, 1);
	cairo_set_font_size (cr, 12 );
	switch(GROUP_VAL_POS(grp , SizingCurves.curve_pos))
	{
		case 1:
			cairo_move_to(cr , 32 , 18);
			cairo_show_text(cr , "DAC") ;
			break;
		case 2:
			cairo_move_to(cr , 32 , 5);
			cairo_line_to(cr , 59 , 7);
			cairo_stroke (cr)          ;
			cairo_move_to(cr , 32 , 18)  ;
			cairo_show_text(cr , "DAC")  ;
			break;
		case 3:
			cairo_move_to(cr , 32 , 18);
			cairo_show_text(cr , "TCG") ;
			break;
		default:
			break ;
	};
	cairo_stroke (cr);

	if(GROUP_VAL_POS(grp , SensationCalibrated))
		cairo_set_source_rgba (cr, 0, 1, 0, 1);
	else
		cairo_set_source_rgba (cr, 1 , 0 , 0, 1) ;
	cairo_move_to(cr , 65 , 16);
	cairo_show_text(cr, "S");
	cairo_stroke (cr);

	if(GROUP_VAL_POS(grp , VelocityCalibrated))
		cairo_set_source_rgba (cr, 0, 1, 0, 1);
	else
		cairo_set_source_rgba (cr, 1 , 0 , 0, 1) ;
	cairo_move_to(cr , 80 , 16);
	cairo_show_text(cr, "V");
	cairo_stroke (cr);

	if(GROUP_VAL_POS(grp , WedgeDelayCalibrated))
		cairo_set_source_rgba (cr, 0, 1, 0, 1);
	else
		cairo_set_source_rgba (cr, 1 , 0 , 0, 1) ;
	cairo_move_to(cr , 95 , 16);
	cairo_show_text(cr, "W");
	cairo_stroke (cr);
	cairo_destroy(cr);

	return TRUE ;
}

// 显示右上角的电池信息
void ShowBatteryStatus(cairo_t* cr)
{
	char  buff[9];
	static int ttt = 0;
	static int ttt1= 0;

	unsigned short status;
	cairo_set_line_width(cr, 2);
	/* 清空背景 */
	cairo_rectangle (cr , 0, 0, 115, 65);
	cairo_fill (cr);

	/* 第一个电池 需要根据电量来改变颜色 */
	cairo_set_source_rgba (cr, 1, 1, 1, 1);
	cairo_move_to (cr, 0, 3);
	cairo_line_to (cr, 52, 3);
	cairo_move_to (cr, 0, 3);
	cairo_line_to (cr, 0, 18);
	cairo_move_to (cr, 0, 18);
	cairo_line_to (cr, 52, 18);
	cairo_move_to (cr, 52, 3);
	cairo_line_to (cr, 52, 18);
	cairo_move_to (cr, 52, 8);
	cairo_line_to (cr, 57, 8);
	cairo_move_to (cr, 52, 13);
	cairo_line_to (cr, 57, 13);
	cairo_move_to (cr, 57, 8);
	cairo_line_to (cr, 57, 13);
	cairo_stroke (cr);

	/* 第二个电池 */
	cairo_move_to (cr, 0, 23);
	cairo_line_to (cr, 52,23);
	cairo_move_to (cr, 0, 23);
	cairo_line_to (cr, 0, 23 + 15);
	cairo_move_to (cr, 0, 23 +15);
	cairo_line_to (cr, 52, 23 + 15);
	cairo_move_to (cr, 52, 23);
	cairo_line_to (cr, 52, 23 +15);
	cairo_move_to (cr, 52, 23 + 5);
	cairo_line_to (cr, 57, 23 + 5);
	cairo_move_to (cr, 52, 23 + 10);
	cairo_line_to (cr, 57, 23 + 10);
	cairo_move_to (cr, 57, 23 + 5);
	cairo_line_to (cr, 57, 23 + 10);
	cairo_stroke (cr);

#ifdef X86
	pp->battery.power1  = 100 ;
	pp->battery.status1 = 0x00e0 ;
	pp->battery.power2  = 20 ;
	pp->battery.status2 = 0x0080 ;
#endif
	status = 0x0080 & pp->battery.status1 ;
	if (status && ((pp->battery.power1 >= 0 && pp->battery.power1 <=100) ))//电池1
	{
		status = 0x0060 & pp->battery.status1;
		if(status)
		{
			if(pp->battery.power1 > 15)
				cairo_set_source_rgba (cr, 0.3, 0.8, 0.3, 1);
			else
				cairo_set_source_rgba (cr, 1, 0, 0, 1);
			cairo_rectangle (cr , 1 , 3, 0.5*(pp->battery.power1), 15);
			cairo_fill (cr);
			cairo_set_source_rgba (cr, 1, 1, 0, 1);
			sprintf(buff, "%3d%%" , pp->battery.power1);
			cairo_move_to(cr, 13 , 3+12);
			cairo_show_text(cr, buff);
			cairo_stroke (cr);
		}
		else
		{

			if(pp->battery.power1 > 15)
				cairo_set_source_rgba (cr, 0.3, 0.8, 0.3, 1);
			else
				cairo_set_source_rgba (cr, 1, 0, 0, 1);
			cairo_rectangle (cr , 1, 3, 0.5*(pp->battery.power1), 15);
			cairo_fill (cr);
			cairo_set_source_rgba (cr, 1, 1, 0, 1);
			if(ttt)
			{
				ttt = 0 ;
				sprintf(buff, "%3d%%" , pp->battery.power1);
				cairo_move_to(cr, 13 , 3+12);
			}
			else
			{
				ttt = 1 ;
				sprintf(buff, "charging");
				cairo_move_to(cr, 2, 3 + 12);
			}
			cairo_show_text(cr, buff);
			cairo_stroke (cr);
		}
	}
	status = 0x0080 & pp->battery.status2 ;
	if (status && ((pp->battery.power2 >= 0 && pp->battery.power2 <=100) ))//电池1
	{
		status = 0x0060 & pp->battery.status2;
		if(status)
		{
			if(pp->battery.power2>15)
				cairo_set_source_rgba (cr, 0.3, 0.8, 0.3, 1);
			else
				cairo_set_source_rgba (cr, 1, 0, 0, 1);
			cairo_rectangle (cr , 1 , 23, 0.5*(pp->battery.power2), 15);//;50*(pp->battery.power1), 15);//
			//cairo_rectangle (cr , 0, y2, 50*0.85,15);//;50*(pp->battery.power1), 15);//
			cairo_fill (cr);
			cairo_set_source_rgba (cr, 1, 1, 0, 1);
			sprintf(buff, "%3d%%" , pp->battery.power2);
			cairo_move_to(cr, 13 , 23+12);
			cairo_show_text(cr, buff);
			cairo_stroke (cr);
		}
		else
		{
			if(pp->battery.power2>15)
				cairo_set_source_rgba (cr, 0.3, 0.8, 0.3, 1);
			else
				cairo_set_source_rgba (cr, 1, 0, 0, 1);
			cairo_rectangle (cr , 1 , 23, 0.5*(pp->battery.power2), 15);
			cairo_fill (cr);

			cairo_set_source_rgba (cr, 1, 1, 0, 1);
			if(ttt1)
			{
				ttt1 = 0 ;
				sprintf(buff, "%3d%%" , pp->battery.power2);
				cairo_move_to(cr, 13 , 23+12);
			}
			else
			{
				ttt1 = 1 ;
				sprintf(buff, "charging");
				cairo_move_to(cr, 2, 23+12);
			}
			cairo_show_text(cr, buff);
			cairo_stroke (cr);
		}
	}
}

void RefreshGainMark(int grp)
{
    char* markup ;

    if (get_group_db_ref (pp->p_config, grp)) {
		markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
                (group_get_gain(grp) - group_get_refgain(grp)) / 100.0,
                        group_get_refgain(grp) / 100.0);
    } else {
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
                group_get_gain(grp) / 100.0 );
	}
	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]), markup);
	g_free(markup);
}

void ShowTemperature(cairo_t* cr)
{
	// display max temperature
	short max_temperature  ; // = 0x1340;
    short* _nTmpBuffer   ;
	int       i ;
	char  buff[9];

	_nTmpBuffer = &(pp->battery.temp1) ;
	max_temperature = _nTmpBuffer[0] ;

	for(i = 1 ; i < 6 ; i ++)
	{
		if(max_temperature < _nTmpBuffer[i] )
		{
			max_temperature = _nTmpBuffer[i]  ;
		}
	}

	max_temperature = max_temperature>>7    ;
    if(max_temperature>=90  && max_temperature <= -40)
    {
    	return ;
    }
	sprintf(buff, "%3d℃" , max_temperature);
	cairo_set_font_size (cr, 12 );
	cairo_set_source_rgba (cr, 1, 0, 0, 1);
	cairo_move_to(cr, 60 , 3 + 15);
	cairo_show_text(cr, buff);
	cairo_stroke (cr);
}

void ShowSonicReflectTimes(cairo_t* cr)
{
	cairo_set_source_rgba (cr, 0, 1, 1, 1);
	cairo_set_line_width(cr, 1);
	cairo_set_font_size (cr, 10 );
	cairo_move_to(cr, 70 , 34);
	char buff[10] ;
	if(pp->thickness_refract_times < 0 || pp->thickness_refract_times > 100)
	{
	     cairo_show_text(cr, "0");
	}
	else
	{
	     sprintf(buff , "%2d" , pp->thickness_refract_times);
	     cairo_show_text(cr, buff);
	}
	cairo_move_to(cr, 68 , 23+10);
	cairo_line_to(cr, 76 , 23+16);
	cairo_move_to(cr, 76 , 23+16);
	cairo_line_to(cr, 91 , 23+ 6);
	cairo_move_to(cr, 91 , 23+ 6);
	cairo_line_to(cr, 86 , 23+ 7);
	cairo_move_to(cr, 91 , 23+ 6);
	cairo_line_to(cr, 89 , 23+ 11);
	cairo_stroke (cr);
	cairo_set_line_width(cr, 2);
	cairo_move_to(cr, 83 , 23+ 10);
	cairo_line_to(cr, 83 , 23+ 13);

	cairo_stroke (cr);
	cairo_destroy(cr);//销毁画笔
}


/* 画电池等信息 */
static gboolean draw_other_info (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;        //声明一支画笔

	cr = gdk_cairo_create(gtk_widget_get_window(widget));//创建画笔
	cairo_select_font_face (cr, "Adobe Heiti Std",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD);
	// draw battery
	ShowBatteryStatus(cr) ;
	// show current max inner temperature
	ShowTemperature(cr);
	// show sonic reflect times in the specimen
	ShowSonicReflectTimes(cr);

	return TRUE;
}
void DrawWindowTitle( DRAW_AREA *p)
{
	int _nGroupId = p->group;
	int _nCurrentGroupId = get_current_group(pp->p_config) ;
	char* markup ;
	if(_nGroupId == _nCurrentGroupId)
	{
		 markup = g_markup_printf_escaped ("<span foreground='red' font_desc='9'>%s</span>", p->title);
	}
	else
	{
		 markup = g_markup_printf_escaped ("<span foreground='white' font_desc='8'>%s</span>", p->title);
	}
	gtk_label_set_markup (GTK_LABEL (p->label), markup);
	g_free (markup);
}

// 画显示区域的单个窗口
static void draw_area_(GtkWidget *parent_box, int nIndex, guint width, guint height)
{
	DRAW_AREA* draw_area = &(pp->draw_area[nIndex]) ;
	draw_area->nWindowIndex = nIndex  ;
	GdkColor text_base	= {0x0, 0x1300, 0x4900, 0x7600};
	GdkColor _black	= {0x0, 0x0, 0x0, 0x0800};
	draw_area->width = width;
	draw_area->height = height;
	draw_area->vbox = gtk_vbox_new (FALSE, 0);
	/* 窗口名字 */
	if(!(draw_area->mask & 0x10))
	{
		draw_area->ebox = gtk_event_box_new ();

		draw_area->label = gtk_label_new (NULL);
		gtk_widget_modify_bg (draw_area->ebox, GTK_STATE_NORMAL, &text_base);

		gtk_container_add (GTK_CONTAINER (draw_area->ebox), draw_area->label);
		gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->ebox, FALSE, FALSE, 0);
		gtk_widget_set_size_request (GTK_WIDGET (draw_area->ebox), width, 15);
		draw_area->drawing_area = gtk_drawing_area_new();
		gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width, height-15);
		DrawWindowTitle(draw_area) ;
	}
	/* 在draw_info里面实现 3个刻度尺 1个画图区 */
	else
	{
		draw_area->drawing_area = gtk_drawing_area_new();
		gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width, height);
	}
	gtk_widget_modify_bg (draw_area->drawing_area, GTK_STATE_NORMAL, &_black);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->drawing_area, FALSE, FALSE, 0);
	/* 调用 draw_info 函数 */
	g_signal_connect (G_OBJECT (draw_area->drawing_area), "expose-event",
			G_CALLBACK (draw_info), (gpointer)(draw_area));

    gtk_widget_add_events ( draw_area->drawing_area , GDK_BUTTON_PRESS_MASK );
    g_signal_connect (G_OBJECT (draw_area->drawing_area), "button_press_event", G_CALLBACK (DrawAreaButtonClick),  draw_area);

	gtk_widget_show_all(draw_area->vbox);
	gtk_box_pack_start (GTK_BOX (parent_box), draw_area->vbox, FALSE, FALSE, 0);
	return ;
}

/*
 * WinIndex 第几个窗口0-15,
 * scan_type ABSC 
 * w h 宽高
 * xoff yoff 偏移
 * group 哪个组
 */
inline void SetDrawWindowBlackAreaConfig (guchar WinIndex,guchar scan_type, guint w, guint h,
		guint xoff, guint yoff, guchar group)
{

	TMP(scan_type[WinIndex])	=	scan_type;
	TMP(scan_xpos[WinIndex])	=	xoff ;
	TMP(scan_ypos[WinIndex])	=	yoff ;
	TMP(scan_width[WinIndex])   =   w    ;
	TMP(scan_height[WinIndex])  =   h    ;
	TMP(scan_group[WinIndex])	=	group;
	//TMP(a_scan_dot_qty)			=	aw   ;

	switch (scan_type)
	{

		case B_SCAN:
		case TOFD_B_SCAN:
		case B_SCAN_VERT:
			TMP(b_scan_width)	=	w;
			TMP(b_scan_height)	=	h;
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
		case C_SCAN_VE:
			TMP(c_scan_width)	=	w;
			TMP(c_scan_height)	=	h;
			break;
		default:break;
	}
	return ;
}

/*
 * 修改波形区的 刻度尺 标签的信息 
 * mask 后面3为是表示左右下3个方向的刻度尺 存在否
 *
 */

void SetDrawWindowTitle( DRAW_AREA *p)
{
	int grp = p->group ;
	double skew_angle       = GROUP_VAL_POS(grp , skew)  / 100.0 ;

	int num = TMP(beam_num[grp]);
	double angle ;
	switch (LAW_VAL_POS(grp , Focal_type))
	{
		case AZIMUTHAL_SCAN:
			angle = LAW_VAL_POS (grp , Angle_min) + LAW_VAL_POS (grp, Angle_step) * num ;
			break;
		case LINEAR_SCAN:
		case DEPTH_SCAN:
		case STATIC_SCAN:
			angle = LAW_VAL_POS (grp , Angle_min) ;
			break;
		default:break;
	}
	angle /= 100.0  ;
	int type = p->scan_type  ;
	switch (type)
	{
		case A_SCAN:
			g_sprintf (p->title, "A scan|Gr %d|CH %3.1f|SK%3.1f|L%2d",
					grp+1, angle , skew_angle , num + 1);
			break ;
		case A_SCAN_R: // only true depth situation
			g_sprintf (p->title, "A scan|Gr %d|CH %3.1f|SK%3.1f|L%2d",
					grp+1, angle , skew_angle , num + 1);
			break;
		case  A_SCAN_CLB :
			g_sprintf (p->title, "A scan|Gr %d|CH %3.1f|SK%3.1f|L%2d",
					grp+1, angle , skew_angle , num + 1);
			break;
		case B_SCAN:
			g_sprintf (p->title, "B scan|Gr %d|CH %3.1f|SK%3.1f|L%2d", grp+1, angle , skew_angle , num + 1);
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
		case C_SCAN_VE:
			g_sprintf (p->title, "C scan|Gr %d|CH %3.1f|SK%3.1f|L%2d", grp+1, angle , skew_angle , num + 1);
			break;
		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:
			g_sprintf (p->title, "S scan|Gr %d|CH %3.1f|SK%3.1f|L%2d",
					grp+1, angle , skew_angle , num + 1);
			break;
		case WEDGE_DELAY:
			g_sprintf (p->title, "Calibration:Wedge Delay");
			break;
		case SENSITIVITY:
			g_sprintf (p->title, "Calibration:Sensitivity");
			break ;
		case TCG:
			g_sprintf (p->title, "Calibration:TCG");
			break;
		case TOFD_B_SCAN:
		case B_SCAN_VERT:
			g_sprintf (p->title, "B scan|Gr %d|CH %3.1f|SK%3.1f|L%2d", grp+1, angle , skew_angle , num + 1);
			break;
		default:break;
	}
}

void UpdateWindowTitle()
{
	int i ;
	DRAW_AREA* _DrawArea;
	for (i = 0; ((i < 16) && (TMP(_scan_type[i]) != 0xff)); i++)
	{
		_DrawArea = &(pp->draw_area[i]) ;
		SetDrawWindowTitle(_DrawArea);
		DrawWindowTitle(_DrawArea);
	}
}

void SetDrawWindowConfigWnd( DRAW_AREA *p, int type, int mask)
{
	double middle = 0.0;
	double max_angle, min_angle ;
	int grp = p->group ;
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	p->scan_type = type;

	double gate_width  ;
	double gate_start  ;

    double start = group_get_start(grp)/1000.0 ;
    double range = group_get_range(grp)/1000.0 ;
	double scan_offset   = GROUP_VAL_POS(grp , scan_offset)  / 10.0;
	double index_offset  = GROUP_VAL_POS(grp , index_offset) / 10.0;
	int skew_angle       = GROUP_VAL_POS(grp , skew)  ;
	double velocity      = GROUP_VAL_POS(grp , velocity) / 100.0 ;
	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	p->mask = mask;

	switch (type)
	{
		case A_SCAN:
			if(pp->p_config->group[grp].rectifier1)
			{
		          p->hmax1 = 0 ;
		          p->hmax2 = 0 ;
			}
			else
			{
		         p->hmax1 = -100 ;
		         p->hmax2 = -100 ;
			}
			//p->hmin2 = 0;
			p->h1_unit = UNIT_BFH;
			p->h1_color = 0xEDF169;	/*黄色*/
			p->hmin1 = 100;
			p->hmin2 = 100;
			//p->hmax2 = 100;
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;	/*黄色*/
			if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)	/* wrule */	/*淡黄色*/
			{
				p->wmin1 = start;
				p->wmax1 = range + start;
				p->w_unit = UNIT_US;
				p->w_color = 0xF8EAC4;
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->wmin1 = start *  velocity / 2000.0 ;
					p->wmax1 = p->wmin1 + range * velocity / 2000.0;
					p->w_unit = UNIT_MM;
				}
				else
				{
					p->wmin1 = start * 0.03937 * velocity / 2000.0;
					p->wmax1 = p->wmin1 + range * 0.03937 * velocity / 2000.0;
					p->w_unit = UNIT_INCH;
				}
				if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_SOUNDPATH)
				{
					p->w_color = 0xF49CD6;	/* 粉色 */
				}
				else
				{
					p->wmin1 *= cos(TMP(current_angle[grp])) ;
					p->wmax1 *= cos(TMP(current_angle[grp])) ;
					p->w_color = 0xD6ABF1;	/* 紫色 */
				}
			}
			break ;
		case A_SCAN_R: // only true depth situation
			p->hmin2 = 100;
			if(pp->p_config->group[grp].rectifier1)
			{
		        p->wmin1 = 0 ;
		        p->hmax2 = 0 ;
			}
			else
			{
				p->wmin1 = -100;
				p->hmax2 = -100 ;
			}
			if(LAW_VAL_POS(grp, Focal_type) == 0)
			{
				if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0) {
					min_angle = 0.0;
                } else {
					min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
                }
				max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
			}
			else
			{
				max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
			}

			p->hmin1 = start * velocity / 2000.0 ;
			p->hmax1 = range * velocity / 2000.0 + p->hmin1;

			if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH) {
			    p->hmin1 *= cos(max_angle);
    			p->hmax1 *= cos(min_angle);
            }

			if(get_unit(pp->p_config) == UNIT_MM)
			{
				p->h1_unit = UNIT_MM;
			}
			else
			{
				p->h1_unit = UNIT_INCH;
				p->hmin1 *= 0.03937;
				p->hmax1 *= 0.03937;
			}
			p->h1_color = 0xD6ABF1;/*紫色*/
			p->wmax1 = 100;
			p->w_unit = UNIT_BFH;
			p->w_color = 0xEDF169;	/*黄色*/

			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;	/*黄色*/
			break;
		case  A_SCAN_CLB :
			if(LAW_VAL_POS(grp, Focal_type) == 0)
			{
				//current_angle = LAW_VAL_POS(grp , Angle_min)/100.0 + BeamNo * LAW_VAL_POS(grp , Angle_step)/100.0 ;
				if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
					min_angle = 0.0;
				else
					min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
				max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
			}
			else
			{
				max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
			}

			if(pp->p_config->group[grp].rectifier1)
			{
		          p->hmax1 = 0 ;
		          p->hmax2 = 0 ;
			}
			else
			{
		         p->hmax1 = -100 ;
		         p->hmax2 = -100 ;
			}
			p->h1_unit = UNIT_BFH;
			p->h1_color = 0xEDF169;	/*黄色*/
			p->hmin1 = 100;
			p->hmin2 = 100;
			//p->hmax2 = 100;
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;	/*黄色*/

			if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TIME)	/* wrule */	/*淡黄色*/
			{
				p->wmin1 = start;
				p->wmax1 = range;
				p->w_unit = UNIT_US;
				p->w_color = 0xF8EAC4;
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->wmin1 = start * velocity /2000.0 ;
					p->wmax1 = p->wmin1 + range * velocity / 2000.0;
					p->w_unit = UNIT_MM;
				}
				else
				{
					p->wmin1 = start * 0.03937 * velocity / 2000.0 ;
					p->wmax1 = p->wmin1 + range * 0.03937 * velocity / 2000.0;
					p->w_unit = UNIT_INCH;
				}
				if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					p->w_color = 0xD6ABF1;	/* 紫色 */
					p->wmin1 *= cos(max_angle) ;
					p->wmax1 *= cos(min_angle) ;
				}
				else
				{
					p->w_color = 0xF49CD6;	/* 粉色 */
				}
			}
			break;
		case B_SCAN:
		case B_SCAN_VERT:
			p->hmin2 = 100;
			if(pp->p_config->group[grp].rectifier1)
			{
		          p->hmax2 = 0 ;
			}
			else
			{
		          p->hmax2 = -100 ;
			}
			/* hrule1 */
			if(get_inspec_source (pp->p_config)==0)	/* scan -> time */
			{
				p->hmax1 = 0;
				p->hmin1 = TMP(b_scan_height) * 10.0 / get_prf();
				p->h1_unit = UNIT_S;
				p->h1_color = 0xCCD9D5;	/*浅灰色*/
			}
			else		/* scan -> Encoder1/Encoder2  */
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->hmax1 = scan_offset + get_area_scanstart (pp->p_config)/1000.0;
					p->hmin1 = scan_offset + get_area_scanend (pp->p_config)/1000.0;
					p->h1_unit = UNIT_MM;
					p->h1_color = 0x0AD5D3;
				}
				else
				{
					p->hmax1 = scan_offset*0.03937 + get_area_scanstart (pp->p_config)/1000.0*0.03937;
					p->hmin1 = scan_offset*0.03937 + get_area_scanend (pp->p_config)/1000.0*0.03937;
					p->h1_unit = UNIT_INCH;
					p->h1_color = 0x0AD5D3;	/*深绿色*/
				}
			}

			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;	/*黄色*/

			if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TIME)	/* wrule */
			{
				p->wmin1 = start;
				p->wmax1 = range + start ;
				p->w_unit = UNIT_US;
				p->w_color = 0xF8EAC4;	/*浅黄色*/
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->wmin1 = start * velocity / 2000.0 ;
					p->wmax1 = range * velocity / 2000.0 + p->wmin1 ;
					p->w_unit = UNIT_MM;
				}
				else
				{
					p->wmin1 = 0.03937 * start * velocity / 2000.0 ;
					p->wmax1 = 0.03937 * range * velocity / 2000.0 + p->wmin1 ;
					p->w_unit = UNIT_INCH;
				}

				if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					p->w_color = 0xD6ABF1;	/* 紫色 */
					p->wmin1 *= cos(TMP(current_angle[grp])) ;
					p->wmax1 *= cos(TMP(current_angle[grp])) ;
				}
				else
				{
					p->w_color = 0xF49CD6;	/* 粉色 */
				}
			}
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
			// C_SCAN 和 CCC_SCAN 数据源：0// CC_SCan 数据源：1
			if(type == CC_SCAN)
			{
				if((get_cscan_source(pp->p_config, 1)==0) || (get_cscan_source(pp->p_config, 1)==1))	/* hrule2 */
				{
					p->h2_unit  = UNIT_BFH;
					p->h2_color = 0xEDF169;	/*黄色*/
					p->hmin2 = 100;
					if(pp->p_config->group[grp].rectifier1)
					{
				          p->hmax2 = 0 ;
					}
					else
					{
				          p->hmax2 = -100 ;
					}
				}
				else
				{
					p->hmin2 = GROUP_VAL_POS(grp , max_thickness) /1000.0;
					p->hmax2 = GROUP_VAL_POS(grp , min_thickness) /1000.0;
					p->h2_unit  = UNIT_MM ;
					p->h2_color = 0xD6ABF1;	/*紫色*/
				}
			}
			else
			{
				if((get_cscan_source(pp->p_config, 0)==0) || (get_cscan_source(pp->p_config, 0)==1))	/* hrule2 */
				{
					p->h2_unit = UNIT_BFH;
					p->h2_color = 0xEDF169;	/*黄色*/
					p->hmin2 = 100;
					if(pp->p_config->group[grp].rectifier1)
					{
				          p->hmax2 = 0 ;
					}
					else
					{
				          p->hmax2 = -100 ;
					}
				}
				else
				{
					p->hmin2 = GROUP_VAL_POS(grp , max_thickness) /1000.0;
					p->hmax2 = GROUP_VAL_POS(grp , min_thickness) /1000.0;
					p->h2_unit  = UNIT_MM;
					p->h2_color = 0xD6ABF1;	/*紫色*/
				}
			}

			if( get_inspec_type(pp->p_config) == 0 )//one line scan 
			{
				if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
				{ 
					p->hmax1 = LAW_VAL_POS (grp, Angle_max) / 100.0 - 0.5;
					p->hmin1 = LAW_VAL_POS (grp, Angle_min) / 100.0 + 0.5;

					p->h1_color = 0xB2C1C1;
					p->h1_unit  = UNIT_DEG;
				}
				else if (LAW_VAL_POS(grp,Focal_type) == LINEAR_SCAN)
				{
					p->hmin1 =  0.5;
					p->hmax1 =  TMP(beam_qty[grp]) + 0.5;
					p->h1_color = 0xB2C1C1; /*浅绿色 */
					p->h1_unit  = UNIT_VPA;
				}
			}
			else //raster/helicoidal 待修改
			{
					if( get_inspec_type (pp->p_config) == 0)
					{
						p->hmax1 =  1.0;
						p->hmin1 =  0.0;
						set_area_indexstart (pp->p_config, 0.0);
						set_area_indexend   (pp->p_config, 1.0);
						p->h1_color = 0xADFBE3;
					}
					else if( get_inspec_type (pp->p_config) == 1)
					{
						p->hmax1 = get_area_indexend (pp->p_config)/1000.0;
						p->hmin1 = get_area_indexstart (pp->p_config)/1000.0;
						p->h1_color = 0xADFBE3;
					}
					else if( get_inspec_type (pp->p_config) == 2)
					{
						p->hmax1 = get_area_scanend (pp->p_config)/1000.0;
						p->hmin1 = get_area_scanstart (pp->p_config)/1000.0;
						p->h1_color = 0x0AD5D3;//					
					}
					p->h1_unit = UNIT_MM;
			}

			if(get_inspec_source (pp->p_config)==0)	/* scan -> time */
			{
				if( get_inspec_type (pp->p_config) == 2)
				{
					p->wmax1 = get_area_indexend (pp->p_config)/1000.0;
					p->wmin1 = get_area_indexstart (pp->p_config)/1000.0;
					p->w_color = 0xADFBE3;
					p->w_unit  = UNIT_MM;
				}
				else
				{
					p->wmin1 = 0;
					p->wmax1 = TMP(c_scan_width) * 10.0 / get_prf();//
					p->w_unit = UNIT_S;
					p->w_color =  0xCCD9D5;	/*浅灰色*/
				}
			}
			else		/* scan -> Encoder1/Encoder2  */
			{
				if( get_inspec_type (pp->p_config) == 2)
				{
					p->wmax1 = get_area_indexend (pp->p_config)/1000.0;
					p->wmin1 = get_area_indexstart (pp->p_config)/1000.0;
					p->w_color = 0xADFBE3;
					p->w_unit  = UNIT_MM;
				}
				else
				{
					if(get_unit(pp->p_config) == UNIT_MM)	/* wrule */		/* 深绿色 */
					{
						p->wmin1 = GROUP_VAL_POS(grp, scan_offset)/10.0 + get_area_scanstart (pp->p_config)/1000.0;
						p->wmax1 = GROUP_VAL_POS(grp, scan_offset)/10.0 + get_area_scanend (pp->p_config)/1000.0;
						p->w_unit = UNIT_MM;
						p->w_color = 0x0AD5D3;
					}
					else
					{
						p->wmin1 = GROUP_VAL_POS(grp, scan_offset)/10.0*0.03937 + get_area_scanstart (pp->p_config)/1000.0*0.03937;
						p->wmax1 = GROUP_VAL_POS(grp, scan_offset)/10.0*0.03937 + get_area_scanend (pp->p_config)/1000.0*0.03937;
						p->w_unit = UNIT_INCH;
						p->w_color = 0x0AD5D3;
					}
				}
			}	
			break;
		case C_SCAN_VE:
			if((get_cscan_source(pp->p_config, 1)==0) || (get_cscan_source(pp->p_config, 1)==1))	/* hrule2 */
			{
                p->h2_unit  = UNIT_BFH;
                p->h2_color = 0xEDF169;	/*黄色*/
                p->hmin2 = 100;
                if(pp->p_config->group[grp].rectifier1)	{
                    p->hmax2 = 0 ;
                } else {
                    p->hmax2 = -100 ;
                }
            } else {
                p->hmin2 = GROUP_VAL_POS(grp , max_thickness) /1000.0;
                p->hmax2 = GROUP_VAL_POS(grp , min_thickness) /1000.0;
                p->h2_unit  = UNIT_MM ;
                p->h2_color = 0xD6ABF1;	/*紫色*/
            }

			if( get_inspec_type(pp->p_config) == 0 )//one line scan 
			{
				if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
				{ 
					p->wmax1 = LAW_VAL_POS (grp, Angle_max) / 100.0 - 0.5;
					p->wmin1 = LAW_VAL_POS (grp, Angle_min) / 100.0 + 0.5;

					p->w_color = 0xB2C1C1;
					p->w_unit  = UNIT_DEG;
				}
				else if (LAW_VAL_POS(grp,Focal_type) == LINEAR_SCAN)
				{
					p->wmin1 =  0.5;
					p->wmax1 =  TMP(beam_qty[grp]) + 0.5;
					p->w_color = 0xB2C1C1; /*浅绿色 */
					p->w_unit  = UNIT_VPA;
				}
			}

			if(get_inspec_source (pp->p_config)==0)	/* scan -> time */
			{
				if( get_inspec_type (pp->p_config) == 2)
				{
					p->hmin1 = get_area_indexend (pp->p_config)/1000.0;
					p->hmax1 = get_area_indexstart (pp->p_config)/1000.0;
					p->h1_color = 0xADFBE3;
					p->h1_unit  = UNIT_MM;
				}
				else
				{
					p->hmax1 = 0;
					p->hmin1 = TMP(c_scan_height) * 10.0 / get_prf();//
					p->h1_unit = UNIT_S;
					p->h1_color =  0xCCD9D5;	/*浅灰色*/
				}
			}
			else		/* scan -> Encoder1/Encoder2  */
			{
				if( get_inspec_type (pp->p_config) == 2)
				{
					p->hmin1 = get_area_indexend (pp->p_config)/1000.0;
					p->hmax1 = get_area_indexstart (pp->p_config)/1000.0;
					p->h1_color = 0xADFBE3;
					p->h1_unit  = UNIT_MM;
				}
				else
				{
					if(get_unit(pp->p_config) == UNIT_MM)	/* wrule */		/* 深绿色 */
					{
						p->hmax1 = GROUP_VAL_POS(grp, scan_offset)/10.0 + get_area_scanstart (pp->p_config)/1000.0;
						p->hmin1 = GROUP_VAL_POS(grp, scan_offset)/10.0 + get_area_scanend (pp->p_config)/1000.0;
						p->h1_unit = UNIT_MM;
						p->h1_color = 0x0AD5D3;
					}
					else
					{
						p->hmax1 = GROUP_VAL_POS(grp, scan_offset)/10.0*0.03937 + get_area_scanstart (pp->p_config)/1000.0*0.03937;
						p->hmin1 = GROUP_VAL_POS(grp, scan_offset)/10.0*0.03937 + get_area_scanend (pp->p_config)/1000.0*0.03937;
						p->h1_unit = UNIT_INCH;
						p->h1_color = 0x0AD5D3;
					}
				}
			}
			break;
		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:
			p->hmin2 = 100;
			if(pp->p_config->group[grp].rectifier1)
			{
		          p->hmax2 = 0 ;
			}
			else
			{
		          p->hmax2 = -100 ;
			}

			if(GROUP_VAL_POS(grp, ut_unit)==UT_UNIT_SOUNDPATH)
			{
				if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
				{
					if(LAW_VAL_POS (grp,Angle_min) == LAW_VAL_POS(grp,Angle_max) )	/*hrule1*/	/* 灰色 */
					{
						p->hmax1 = LAW_VAL_POS (grp, Angle_min) / 100.0 + 1;
						p->hmin1 = LAW_VAL_POS (grp, Angle_max) / 100.0 ;
					}
					else
					{
						p->hmax1 = LAW_VAL_POS (grp, Angle_max) / 100.0 + 0.5;
						p->hmin1 = LAW_VAL_POS (grp, Angle_min) / 100.0 - 0.5;
					}
					p->h1_unit = UNIT_DEG;
				}
				else if (LAW_VAL_POS(grp, Focal_type) == LINEAR_SCAN)
				{
					p->hmin1 = 0.5;
					p->hmax1 = 1.5 + (double)((( LAW_VAL_POS (grp,Last_tx_elem)-LAW_VAL_POS(grp,First_tx_elem)
									- LAW_VAL_POS(grp,Elem_qty) + 1 ) / LAW_VAL_POS(grp,Elem_step))) ;
					p->h1_unit = UNIT_VPA;
				}
				p->h1_color = 0xB2C1C1;

				if(get_unit(pp->p_config) == UNIT_MM)	/*wrule*/
				{
					p->wmin1 = start * velocity / 2000.0 ;
					p->wmax1 = range * velocity / 2000.0 + start * velocity / 2000.0 ;
					p->w_unit = UNIT_MM  ;
					p->w_color = 0xF49CD6; /*粉色*/
				}
				else
				{
					p->wmin1   = start * 0.03937 * velocity / 2000.0 ;
					p->wmax1   = range * 0.03937 * velocity / 2000.0 + p->wmin1 ;
					p->w_unit  = UNIT_INCH;
					p->w_color = 0xF49CD6; /*粉色*/
				}
			}
			else if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TIME)
			{
				if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
				{
					if(LAW_VAL_POS (grp,Angle_min) == LAW_VAL_POS(grp,Angle_max) )	/*hrule1*/	/* 灰色 */
					{
						p->hmax1 = LAW_VAL_POS (grp, Angle_min) / 100.0 + 1;
						p->hmin1 = LAW_VAL_POS (grp, Angle_max) / 100.0 ;
					}
					else
					{
						p->hmax1 = LAW_VAL_POS (grp, Angle_max) / 100.0 + 0.5;
						p->hmin1 = LAW_VAL_POS (grp, Angle_min) / 100.0 - 0.5;
					}
					p->h1_unit = UNIT_DEG;
				}
				else if (LAW_VAL_POS(grp, Focal_type) == LINEAR_SCAN)
				{
					p->hmin1 = 0.5;
					p->hmax1 = 1.5 + (double)((( LAW_VAL_POS (grp,Last_tx_elem)-LAW_VAL_POS(grp,First_tx_elem)
									- LAW_VAL_POS(grp,Elem_qty) + 1 ) / LAW_VAL_POS(grp,Elem_step))) ;
					p->h1_unit = UNIT_VPA;
				}
				p->h1_color = 0xB2C1C1;

				p->wmin1 = start ;	/*wrule*/
				p->wmax1 = start + range ;
				p->w_unit = UNIT_US;
				p->w_color =0xF8EAC4; /*浅黄色*/
			}
			else if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if(LAW_VAL_POS(grp, Focal_type) == 0)
				{
					if(LAW_VAL_POS(grp, Angle_min)* LAW_VAL_POS(grp, Angle_max)<0)
						min_angle = 0.0;
					else
						min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
					p->hmax1 = (start + range) * velocity /2000.0 ;
					p->hmin1 = start * velocity /2000.0;
					p->hmin1 *= cos(max_angle);
					p->hmax1 *= cos(min_angle);

                    min_angle = LAW_VAL_POS(grp , Angle_min) * G_PI / 18000.0;
                    max_angle = LAW_VAL_POS(grp , Angle_max) * G_PI / 18000.0;
                    start = start * velocity /2000.0;
					range = range * velocity /2000.0;

                    middle = GROUP_VAL_POS(grp , field_distance[0]);
					if(min_angle < 0)
					{
						p->wmin1 = middle + (range + start) * sin(min_angle);
					}
					else
					{
						p->wmin1 = middle + start * sin(min_angle);
					}
					middle = GROUP_VAL_POS(grp , field_distance[TMP(beam_qty[grp]) -1]) ; //TMP(field_distance[grp][TMP(beam_qty[grp]) -1]);
					if(max_angle < 0)
					{
						p->wmax1 = middle + start * sin(max_angle);
					}
					else
					{
						p->wmax1 = middle + (start + range) * sin(max_angle) ;
					}
					if(p->wmin1 == p->wmax1) p->wmax1 = 1 + p->wmin1 ;
				}
				else
				{
					max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
                    start = start * velocity /2000.0;
					range = range * velocity /2000.0;
					float _hStart = GROUP_VAL_POS(grp , field_distance[0]);
					float _hStop  = GROUP_VAL_POS(grp , field_distance[TMP(beam_qty[grp]) -1]);
					if(_hStop < _hStart)
					{
						middle = _hStart ;
						_hStart= _hStop  ;
						_hStop = middle  ;
					}

					if(min_angle < 0)
					{
						p->wmin1 = _hStart + (range + start) * sin(min_angle);
					}
					else
					{
						p->wmin1 = _hStart + start * sin(min_angle);
					}

					if(max_angle < 0)
					{
						p->wmax1 = _hStop + start * sin(max_angle);
					}
					else
					{
						p->wmax1 = _hStop + (start + range) * sin(max_angle) ;
					}
					if(p->wmin1 == p->wmax1) p->wmax1 = 1 + p->wmin1 ;

					p->hmax1 = start + range;
					p->hmin1 = start;
					p->hmin1 *= cos(max_angle);
					p->hmax1 *= cos(min_angle);
				}

				if(get_unit(pp->p_config) == UNIT_MM) /* hrule1 */
				{
					p->h1_unit  = UNIT_MM ;
					p->h1_color = 0xD6ABF1;/*紫色*/
				}
				else
				{
					p->hmin1 *= 0.03937 ;
					p->hmax1 *= 0.03937 ;
					p->wmin1 *= 0.03937 ;
					p->wmax1 *= 0.03937 ;
					scan_offset  *= 0.03937 ;
					index_offset *= 0.03937 ;
					p->h1_unit  = UNIT_INCH ;
					p->h1_color = 0xD6ABF1  ; /*紫色*/
				}
                if(skew_angle >= 0 && skew_angle < 9000)
                {
					p->wmin1 += scan_offset ;
					p->wmax1 += scan_offset ;
					p->w_color =  0x0AD5D3;/* lan色 */
                }
                else if(skew_angle >= 9000 && skew_angle < 18000)
                {
					p->wmin1 += index_offset ;
					p->wmax1 += index_offset ;
					p->w_color = 0xADFBE3;/* 深绿色 */
                }
                else if(skew_angle >= 18000 && skew_angle < 27000)
				{
					middle   = -p->wmax1 ; // middle is used here temprerate
					p->wmax1 = -p->wmin1 ;
					p->wmin1 = middle   ;
					p->wmin1 += scan_offset ;
					p->wmax1 += scan_offset ;
					p->w_color =  0x0AD5D3 ;
				}
                else
                {
					middle   = -p->wmax1 ; // middle is used here temprerate
					p->wmax1 = -p->wmin1 ;
					p->wmin1 = middle   ;
					p->wmin1 += index_offset ;
					p->wmax1 += index_offset ;
					p->w_color = 0xADFBE3;
                }

				if(get_unit(pp->p_config) == UNIT_MM)
					p->w_unit = UNIT_MM;
				else if(get_unit(pp->p_config)== UNIT_INCH)
					p->w_unit = UNIT_INCH  ;
			}
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;
			break;
		case WEDGE_DELAY:
			p->hmin2 = 100;
		    p->hmax2 = 0 ;
			gate_width  = GROUP_VAL_POS(pp->p_config->groupId,gate[0].width) ;
			gate_start  = GROUP_VAL_POS(pp->p_config->groupId,gate[0].start) ;
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				p->hmax1 = gate_start * velocity / 2000000;
				p->hmin1 = (gate_start + gate_width) * velocity / 2000000 ;
				p->h1_unit = UNIT_MM;
			}
			else
			{
				p->hmax1 = 0.03937 * gate_start * velocity / 2000000 ;
				p->hmin1 = 0.03937 * (gate_start + gate_width) * velocity / 2000000 ;
				p->h1_unit = UNIT_INCH;
			}

			p->h1_color = 0xD6ABF1;/*紫色*/
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;
			if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
			{
				p->wmin1 = TMP_CBA(clb_start) / 100.0 - LAW_VAL_POS(grp , Angle_step) / 200.0;
				p->wmax1 = TMP_CBA(clb_end)   / 100.0 + LAW_VAL_POS(grp , Angle_step) / 200.0 ;
				p->w_unit = UNIT_DEG;
			}
			else if (LAW_VAL_POS(grp,Focal_type) == LINEAR_SCAN)
			{
				p->wmin1 = TMP_CBA(clb_start) / 100.0 - 0.5;
				p->wmax1 = TMP_CBA(clb_end) / 100.0 + 0.5 ;
				p->w_unit = UNIT_VPA;
			}
			p->w_color = 0xCCD9D5;	/*浅灰色*/;
			if(p->hmin1 == p->hmax1) p->hmin1 = p->hmax1 + 1 ;
			if(p->wmin1 == p->wmax1) p->wmax1 = p->wmin1 + 1 ;
			break;
		case SENSITIVITY:
		case TCG:
			p->hmin2 = 100;
		    p->hmax2 = 0 ;
			p->hmax1 = 0;
			p->hmin1 = 100;
			p->h1_unit = UNIT_BFH;
			p->h1_color = 0xEDF169;
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;
			if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
			{
				p->wmin1 = TMP_CBA(clb_start) / 100.0 - LAW_VAL_POS(grp , Angle_step) / 200.0;
				p->wmax1 = TMP_CBA(clb_end)   / 100.0 + LAW_VAL_POS(grp , Angle_step) / 200.0 ;
				p->w_unit = UNIT_DEG;
			}
			else if (LAW_VAL_POS(grp,Focal_type) == LINEAR_SCAN)
			{
				p->wmin1 = TMP_CBA(clb_start) / 100.0 - 0.5;
				p->wmax1 = TMP_CBA(clb_end) / 100.0 + 0.5 ;
				p->w_unit = UNIT_VPA;
			}

			if(p->wmin1 == p->wmax1) p->wmax1 = p->wmin1 + 1 ;
			p->w_color = 0xCCD9D5;	/*浅灰色*/;
			break;
		case TOFD_B_SCAN:
			p->hmin2 = 100;
			if(pp->p_config->group[grp].rectifier1)
			{
		          p->hmax2 = 0 ;
			}
			else
			{
		          p->hmax2 = -100 ;
			}
			if(get_inspec_source (pp->p_config)==0)	/* scan -> time */
			{
				p->wmin1 = 0;
				p->wmax1 = TMP(b_scan_width) * 10.0 / get_prf();
				p->w_unit = UNIT_S;
				p->w_color = 0xCCD9D5;	/*浅灰色*/
			}
			else/* scan -> Encoder1/Encoder2  */
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->wmin1 = GROUP_VAL_POS(grp, scan_offset)/10.0 + get_area_scanstart (pp->p_config)/1000.0;
					p->wmax1 = GROUP_VAL_POS(grp, scan_offset)/10.0 + get_area_scanend (pp->p_config)/1000.0;
					p->w_unit = UNIT_MM;
					p->w_color = 0x0AD5D3;
				}
				else
				{
					p->wmin1 = GROUP_VAL_POS(grp, scan_offset)/10.0*0.03937 + get_area_scanstart (pp->p_config)/1000.0*0.03937;
					p->wmax1 = GROUP_VAL_POS(grp, scan_offset)/10.0*0.03937 + get_area_scanend (pp->p_config)/1000.0*0.03937;
					p->w_unit = UNIT_INCH;
					p->w_color = 0x0AD5D3;	/*深绿色*/
				}
			}
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;	/*黄色*/
			if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TIME)	/* wrule */
			{
				p->hmin1 = start;
				p->hmax1 = range + start ;
				p->h1_unit = UNIT_US;
				p->h1_color = 0xF8EAC4;	/*浅黄色*/
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->hmin1 = start * velocity / 2000.0 ;
					p->hmax1 = range * velocity / 2000.0 + p->hmin1 ;
					p->h1_unit = UNIT_MM;
				}
				else
				{
					p->hmin1 = 0.03937 * start * velocity / 2000.0 ;
					p->hmax1 = 0.03937 * range * velocity / 2000.0 + p->hmin1 ;
					p->h1_unit = UNIT_INCH;
				}

				if(GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					p->h1_color = 0xD6ABF1;	/* 紫色 */
					p->hmin1 *= cos(TMP(current_angle[grp])) ;
					p->hmax1 *= cos(TMP(current_angle[grp])) ;
				}
				else
				{
					p->h1_color = 0xF49CD6;	/* 粉色 */
				}
			}
			break;
		default:break;
	}
}

void SetDrawWindowConfig( DRAW_AREA *p, int type, int mask)
{
	SetDrawWindowConfigWnd( p, type, mask)  ;
	SetDrawWindowTitle(p) ;
}


//****************************************
//  2011.08.26 hefan
//****************************************
void draw_area_calibration()
{
	int i;
	/* 把之前的区域释放 校准*/ 
	if(pp->vboxtable)
		gtk_widget_destroy(pp->vboxtable);
	pp->vboxtable = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_draw_area), pp->vboxtable, FALSE, FALSE, 0);
	gtk_widget_show(pp->vboxtable);
	for (i = 0; i < 4; i ++)
	{
		pp->vbox_area[i] = gtk_vbox_new(FALSE, 0);
		pp->hbox_area[i] = gtk_hbox_new(FALSE, 0);
		memset (TMP(scan_type), 0xff, 16);
	}
	int grp = get_current_group (pp->p_config) ;

	if ((get_display_group(pp->p_config) == DISPLAY_CURRENT_GROUP) || (get_group_qty(pp->p_config)==1))
	{
		switch(pp->ctype_pos)
		{
			case 0:
				break;
			case 1://Ultrasound
				if( !pp->cmode_pos )//Velocity
				{
					pp->draw_area[0].group	=	get_current_group(pp->p_config);
					pp->draw_area[1].group	=	get_current_group(pp->p_config);
					if ((GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_SOUNDPATH) ||
							(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME))
					{
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
						SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01);
						draw_area_ (pp->vbox_area[0], 0, 655, 130);
						SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN, 0);
						draw_area_ (pp->vbox_area[0], 1, 655, 295);
						gtk_widget_show (pp->vbox_area[0]);
						SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 115, 0, 0, grp );
						SetDrawWindowBlackAreaConfig (1, S_SCAN, 605, 260, 0, 130, grp );
					}
					else if (GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
					{
						if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
						{
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
							draw_area_ (pp->hbox_area[0], 0, 250, 425);
							SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_A, 0);
							draw_area_ (pp->hbox_area[0], 1, 405, 425);
							gtk_widget_show (pp->hbox_area[0]);
							SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 390, 0, 0, grp);
							SetDrawWindowBlackAreaConfig (1, S_SCAN_A, 355, 390, 250, 0, grp);
						}
						else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
						{
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
							draw_area_ (pp->hbox_area[0], 0, 250, 425);
							SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_L, 0x0);//modified by shensheng
							draw_area_ (pp->hbox_area[0], 1, 405, 425);
							gtk_widget_show (pp->hbox_area[0]);
							SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 390, 0, 0, get_current_group(pp->p_config));
							SetDrawWindowBlackAreaConfig (1, S_SCAN_L, 355, 390, 250, 0, get_current_group(pp->p_config));
						}
					}
				}
				else
				{
					pp->draw_area[0].group	=	get_current_group(pp->p_config);
					pp->draw_area[1].group	=	get_current_group(pp->p_config);
					pp->draw_area[2].group	=	get_current_group(pp->p_config);

					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
					gtk_widget_show (pp->hbox_area[0]);
					gtk_widget_show (pp->vbox_area[0]);

					if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
						SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_A, 0x06);
					else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
						SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_L, 0x06);

					draw_area_ (pp->hbox_area[0], 0, 327, 212);
					SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN_CLB, 0);
					draw_area_ (pp->hbox_area[0], 1, 327, 212);
					if( pp->cmode_pos == 1 )
					{
						SetDrawWindowConfig (&(pp->draw_area[2]), WEDGE_DELAY, 0);
						draw_area_ (pp->vbox_area[0], 2, 655, 213);
					}
					else if( pp->cmode_pos == 2 )
					{
						SetDrawWindowConfig (&(pp->draw_area[2]), SENSITIVITY, 0) ;
						draw_area_ (pp->vbox_area[0], 2, 655, 213);
					}
					else if( pp->cmode_pos == 3 )
					{
						SetDrawWindowConfig (&(pp->draw_area[2]), TCG, 0);
						draw_area_ (pp->vbox_area[0], 2, 655, 213);
					}

					if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
						SetDrawWindowBlackAreaConfig (0, S_SCAN_A, 307, 177, 0, 0, get_current_group(pp->p_config));
					else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
						SetDrawWindowBlackAreaConfig (0, S_SCAN_L, 307, 177, 0, 0, get_current_group(pp->p_config));
					SetDrawWindowBlackAreaConfig (1, A_SCAN_CLB, 278, 177, 327, 0, get_current_group(pp->p_config));
					if( pp->cmode_pos == 1 )
						SetDrawWindowBlackAreaConfig (2, WEDGE_DELAY, 605, 178, 0, 212, get_current_group(pp->p_config));
					else if( pp->cmode_pos == 2 )
						SetDrawWindowBlackAreaConfig (2, SENSITIVITY, 605, 178, 0, 212, get_current_group(pp->p_config));
					else if( pp->cmode_pos == 3 )
						SetDrawWindowBlackAreaConfig (2, TCG, 605, 178, 0, 212, get_current_group(pp->p_config));
				}
				break;
			case 2://Code
				break;
		}
	}

}

/* 当前group   */
void draw_cur_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 425);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_cur_area_b_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), B_SCAN_VERT, 0);
	draw_area_ (pp->vbox_area[0], 0 , 655, 425);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_cur_area_c_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), C_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 0 , 655, 425);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_cur_area_s_scan()
{
	int grp = get_current_group (pp->p_config) ;
	if ((GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_SOUNDPATH) ||
			(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME))
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN, 0);
		draw_area_ (pp->vbox_area[0], 0 , 655, 425);
		gtk_widget_show (pp->vbox_area[0]);
	} 
	else if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
	{
		if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
			SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_A, 0);
			draw_area_ (pp->vbox_area[0], 0 , 655, 425);
			gtk_widget_show (pp->vbox_area[0]);
		}
		else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
		{
			gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
			SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_L, 0);
			draw_area_ (pp->vbox_area[0], 0 , 655, 425);
			gtk_widget_show (pp->vbox_area[0]);
		}
	}
}

void draw_cur_area_a_b_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01)     ;
	draw_area_ (pp->vbox_area[0], 0 , 655, 130);
	SetDrawWindowConfig (&(pp->draw_area[1]), B_SCAN_VERT, 0);
	draw_area_ (pp->vbox_area[0], 1 , 655, 295);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_cur_area_a_b_c_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01);
	draw_area_ (pp->vbox_area[0], 0, 655, 125);
	SetDrawWindowConfig (&(pp->draw_area[1]), B_SCAN_VERT, 0);
	draw_area_ (pp->vbox_area[0], 1, 655, 150);
	SetDrawWindowConfig (&(pp->draw_area[2]), C_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 2, 655, 150);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_cur_area_a_b_s_scan()
{
	int grp = get_current_group (pp->p_config) ;
	if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
			SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_A, 0);
		else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
			SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_L, 0);
		draw_area_ (pp->vbox_area[0], 0, 300, 425);
		SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0x01);
		draw_area_ (pp->vbox_area[1], 1, 355, 210);
		SetDrawWindowConfig (&(pp->draw_area[2]), B_SCAN_VERT, 0);
		draw_area_ (pp->vbox_area[1], 2, 355, 215);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);

	}
	else
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01);
		draw_area_ (pp->vbox_area[0], 0 , 655, 125);
		SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN, 0x01);
		draw_area_ (pp->vbox_area[0], 1 , 655, 150);
		SetDrawWindowConfig (&(pp->draw_area[2]), B_SCAN_VERT, 0);
		draw_area_ (pp->vbox_area[0], 2 , 655, 150);
		gtk_widget_show (pp->vbox_area[0]);
	}
}

void draw_cur_area_a_c_cc_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
	{
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
		draw_area_ (pp->vbox_area[0], 0 , 655, 150);
		SetDrawWindowConfig (&(pp->draw_area[1]), C_SCAN, 0);
		draw_area_ (pp->vbox_area[0], 1 , 655, 275);

	}
	else if (get_cscan_source(pp->p_config, 1) != C_SCAN_OFF)
	{
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
		draw_area_ (pp->vbox_area[0], 0 , 655, 125);
		SetDrawWindowConfig (&(pp->draw_area[1]), C_SCAN, 0x01);
		draw_area_ (pp->vbox_area[0], 1 , 655, 150);
		SetDrawWindowConfig (&(pp->draw_area[2]), CC_SCAN, 0);
		draw_area_ (pp->vbox_area[0], 2 , 655, 150);
	}
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_cur_area_a_s_cc_scan()
{
	int grp = get_current_group (pp->p_config) ;
	if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
	{
		if ((GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_SOUNDPATH) ||
				(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME))
		{
			gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
			SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01);
			draw_area_ (pp->vbox_area[0], 0 , 655, 130);
			SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN, 0);
			draw_area_ (pp->vbox_area[0], 1 , 655, 295);
			gtk_widget_show (pp->vbox_area[0]);

		}
		else if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
		{
			if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
			{
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
				SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
				draw_area_ (pp->hbox_area[0], 0 , 250, 425);
				SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_A, 0);
				draw_area_ (pp->hbox_area[0], 1 , 405, 425);
				gtk_widget_show (pp->hbox_area[0]);
			} 
			else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
			{
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
				SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
				draw_area_ (pp->hbox_area[0], 0 , 250, 425);
				SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_L, 0x0);//modified by shengsheng
				draw_area_ (pp->hbox_area[0], 1 , 405, 425);
				gtk_widget_show (pp->hbox_area[0]);

			}
		}
	}
	else
	{
		if ((GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_SOUNDPATH) ||
				(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME))
		{
			gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
			SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01);
			draw_area_ (pp->vbox_area[0], 0 , 655, 105);
			SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN, 0);
			draw_area_ (pp->vbox_area[0], 1 , 655, 170);
			SetDrawWindowConfig (&(pp->draw_area[2]), CCC_SCAN, 0);
			draw_area_ (pp->vbox_area[0], 2 , 655, 150);
			gtk_widget_show (pp->vbox_area[0]);

		}
		else if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
		{
			gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
			gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[1], FALSE, FALSE, 0);
			if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
			{
				SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
				draw_area_ (pp->hbox_area[0], 0 , 250, 275);
				SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_A, 0);
				draw_area_ (pp->hbox_area[0], 1 , 405, 275);
				SetDrawWindowConfig (&(pp->draw_area[2]), CCC_SCAN, 0);
				draw_area_ (pp->vbox_area[1], 2 , 655, 150);
				gtk_widget_show (pp->hbox_area[0]);
				gtk_widget_show (pp->vbox_area[1]);

			} 
			else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
			{
				SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
				draw_area_ (pp->hbox_area[0], 0 , 250, 275);
				SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_L, 0);
				draw_area_ (pp->hbox_area[0], 1 , 405, 275);
				SetDrawWindowConfig (&(pp->draw_area[2]), CCC_SCAN, 0);
				draw_area_ (pp->vbox_area[1], 2 , 655, 150);
				gtk_widget_show (pp->hbox_area[0]);
				gtk_widget_show (pp->vbox_area[1]);

			}
		}
	}
}

void SetFftWindowConfig( DRAW_AREA *p, guint type, guint mask)
{
	gint grp = p->group ;
//	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	p->scan_type = type;

	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	p->mask = mask;

//左标尺
	p->hmin1 = 0;
	p->h1_unit = UNIT_BFH;
	p->h1_color = 0xEDF169;	/*黄色*/
	p->hmax1 = 100;
//右标尺
	p->hmax2 = 50;
	if(pp->p_config->group[grp].rectifier1)
	{
          p->hmin2 = 0 ;
	}
	else
	{
         p->hmin2 = -50 ;
	}
	p->h2_unit = UNIT_BFH;
	p->h2_color = 0xEDF169;	/*黄色*/
//底部标尺 //需改为频率标尺
	p->wmin1 = 0;
	p->wmax1 = gData->fft[grp].hzPerPoint * 640 ;//* (50 /	((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS( grp , point_qty)));
	p->w_unit = UNIT_MHZ;
	p->w_color = 0xF49CD6;	/* 粉色 */
//标题
	g_sprintf (p->title, "FFT");
}

void DrawFftInfo(GtkWidget *widget , gpointer data , cairo_t *cr)
{
//	int width ,height;

	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	int group = prule->group;//当前group
//	gtk_widget_get_size_request (widget, &width, &height);
	cairo_set_font_size(cr,8);
	cairo_set_source_rgba(cr,1.0,1.0,1,1.0);

	int originX = 30 ,originY = 15 ,intervalX = 80 ,intervalY = 15;
	char str[256];

	sprintf(str ,"Lower");
	cairo_move_to(cr ,originX + 1 * intervalX ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"Higher");
	cairo_move_to(cr ,originX + 2 * intervalX ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"Center");
	cairo_move_to(cr ,originX + 3 * intervalX ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"Bandwidth");
	cairo_move_to(cr ,originX + 4 * intervalX ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"Bandwidth");
	cairo_move_to(cr ,originX + 5 * intervalX ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"Max:");
	cairo_move_to(cr ,originX + 6 * intervalX ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz",gData->fft[group].hzPerPoint * gData->fft[group].peakPoint);
	cairo_move_to(cr ,originX + 7 * intervalX - 40 ,originY + 0 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"-6db");
	cairo_move_to(cr ,originX + 0 * intervalX + 40 ,originY + 1 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,gData->fft[group].hzPerPoint * gData->fft[group].db6minPoint);
	cairo_move_to(cr ,originX + 1 * intervalX ,originY + 1 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,gData->fft[group].hzPerPoint * gData->fft[group].db6maxPoint);
	cairo_move_to(cr ,originX + 2 * intervalX ,originY + 1 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,FftGetCenterFreq(&gData->fft[group] ,6));
	cairo_move_to(cr ,originX + 3 * intervalX ,originY + 1 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,gData->fft[group].hzPerPoint * (gData->fft[group].db6maxPoint - gData->fft[group].db6minPoint));
	cairo_move_to(cr ,originX + 4 * intervalX ,originY + 1 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2f%%" ,(gData->fft[group].db6maxPoint > gData->fft[group].db6minPoint) ?gData->fft[group].peakPoint * 100.0 / (gData->fft[group].db6maxPoint - gData->fft[group].db6minPoint) :0);
	cairo_move_to(cr ,originX + 5 * intervalX ,originY + 1 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"-20db");
	cairo_move_to(cr ,originX + 0 * intervalX + 40 ,originY + 2 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,gData->fft[group].hzPerPoint * gData->fft[group].db20minPoint);
	cairo_move_to(cr ,originX + 1 * intervalX ,originY + 2 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,gData->fft[group].hzPerPoint * gData->fft[group].db20maxPoint);
	cairo_move_to(cr ,originX + 2 * intervalX ,originY + 2 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" ,FftGetCenterFreq(&gData->fft[group] ,20));
	cairo_move_to(cr ,originX + 3 * intervalX ,originY + 2 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2fMHz" , gData->fft[group].hzPerPoint * (gData->fft[group].db20maxPoint - gData->fft[group].db20minPoint));
	cairo_move_to(cr ,originX + 4 * intervalX ,originY + 2 * intervalY);
	cairo_show_text(cr ,str);

	sprintf(str ,"%.2f%%" ,(gData->fft[group].db20maxPoint > gData->fft[group].db20minPoint) ?gData->fft[group].peakPoint * 100.0 / (gData->fft[group].db20maxPoint - gData->fft[group].db20minPoint) :0);
	cairo_move_to(cr ,originX + 5 * intervalX ,originY + 2 * intervalY);
	cairo_show_text(cr ,str);

}
void DrawFFT_WindowConfig()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 213);//212
	SetFftWindowConfig (&(pp->draw_area[1]), FFT_A_SCAN, 0x0);
	draw_area_ (pp->vbox_area[0], 1, 655, 212);//213

	gtk_widget_show (pp->vbox_area[0]);
}

void DrawFFT_FrameBufferConfig()
{
	int current_group = get_current_group(pp->p_config);
	pp->draw_area[0].group	=	current_group;
	pp->draw_area[1].group	=	current_group;
	SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 178, 0,   0,  current_group);//163
	SetDrawWindowBlackAreaConfig (1, FFT_A_SCAN, 605, 177, 0, 213,  current_group);//178 197

}

#if 0
void draw_fft()
{
	int current_group = get_current_group(pp->p_config);
//ASCAN RF模式
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);

	pp->draw_area[0].group	=	current_group;
	SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 178, 0,   0,  0);//163
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 213);//212
//FFT
	pp->draw_area[1].group	=	current_group;
	SetDrawWindowBlackAreaConfig (1, FFT_A_SCAN, 605, 177, 0, 213,  0);//178 197
	SetFftWindowConfig (&(pp->draw_area[1]), FFT_A_SCAN, 0x0);
	draw_area_ (pp->vbox_area[0], 1, 655, 212);//213

	gtk_widget_show (pp->vbox_area[0]);
}
#endif
/**************************************************************
          多个group
 *************************************************************/
void draw_2_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 213);//212
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0x0);
	draw_area_ (pp->vbox_area[0], 1, 655, 212);//213
	gtk_widget_show (pp->vbox_area[0]);
	/* 显示的位置 偏移等等 */
}

void draw_2_area_b_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), B_SCAN_VERT, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 213);
	SetDrawWindowConfig (&(pp->draw_area[1]), B_SCAN_VERT, 0x0);
	draw_area_ (pp->vbox_area[0], 1, 655, 212);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_2_area_s_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
	draw_area_(pp->hbox_area[0], 0 , 317, 425);
	SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0);
	draw_area_(pp->hbox_area[0], 1 , 338, 425);
	gtk_widget_show (pp->hbox_area[0]);
}

void draw_2_area_a_b_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
    SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
    draw_area_(pp->vbox_area[0], 0 , 317, 150);
    SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
    draw_area_(pp->vbox_area[0], 1 , 317, 275);
    SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0);
    draw_area_(pp->vbox_area[1], 2 , 338, 150);
    SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
    draw_area_(pp->vbox_area[1], 3 , 338, 275);

    gtk_widget_show (pp->hbox_area[0]);
    gtk_widget_show (pp->vbox_area[0]);
    gtk_widget_show (pp->vbox_area[1]);
}

void draw_2_area_a_c_cc_scan()
{
	if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable),    pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 0, 317, 150);
		SetDrawWindowConfig (&(pp->draw_area[1]), C_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 1, 317, 275);
		SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0);
		draw_area_(pp->vbox_area[1], 2, 338, 150);
		SetDrawWindowConfig (&(pp->draw_area[3]), C_SCAN, 0x0);
		draw_area_(pp->vbox_area[1], 3, 338, 275);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
	}
	else
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable)   , pp->hbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 0, 317, 145);
		SetDrawWindowConfig (&(pp->draw_area[1]), C_SCAN, 0x3);
		draw_area_(pp->vbox_area[0], 1, 317, 130);
		SetDrawWindowConfig (&(pp->draw_area[2]), CC_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 2, 317, 150);
		SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0);
		draw_area_(pp->vbox_area[1], 3, 338, 145);
		SetDrawWindowConfig (&(pp->draw_area[4]), C_SCAN, 0x1);
		draw_area_(pp->vbox_area[1], 4, 338, 130);
		SetDrawWindowConfig (&(pp->draw_area[5]), CC_SCAN, 0x0);
		draw_area_(pp->vbox_area[1], 5, 338, 150);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
	}
}

void draw_2_area_a_s_cc_scan()
{
	if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 0, 317, 150);
		SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 1, 317, 275);
		SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0);
		draw_area_(pp->vbox_area[1], 2, 338, 150);
		SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
		draw_area_(pp->vbox_area[1], 3, 338, 275);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);

	}
	else
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable)   , pp->hbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 0, 317, 135);
		SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 1, 317, 145);
		SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 2, 317, 145);
		SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
		draw_area_(pp->vbox_area[1], 3, 338, 135);
		SetDrawWindowConfig (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0);
		draw_area_(pp->vbox_area[1], 4, 338, 145);
		SetDrawWindowConfig (&(pp->draw_area[5]), pp->draw_area[5].scan_type, 0);
		draw_area_(pp->vbox_area[1], 5, 338, 145);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
	}
}

void draw_3_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 141);
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0x0);
	draw_area_ (pp->vbox_area[0], 1, 655, 142);
	SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x0);
	draw_area_ (pp->vbox_area[0], 2, 655, 142);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_3_area_b_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), B_SCAN_VERT, 0);
	draw_area_ (pp->vbox_area[0], 0, 655, 141);
	SetDrawWindowConfig (&(pp->draw_area[1]), B_SCAN_VERT, 0x0);
	draw_area_ (pp->vbox_area[0], 1, 655, 142);
	SetDrawWindowConfig (&(pp->draw_area[2]), B_SCAN_VERT, 0x0);
	draw_area_ (pp->vbox_area[0], 2, 655, 142);
	gtk_widget_show (pp->vbox_area[0]);
}

void draw_3_area_s_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
	draw_area_ (pp->hbox_area[0], 0, 211, 425);
	SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
	draw_area_ (pp->hbox_area[0], 1, 212, 425);
	SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0);
	draw_area_ (pp->hbox_area[0], 2, 232, 425);
	gtk_widget_show (pp->hbox_area[0]);

}

void draw_3_area_a_b_scan()
{
    gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
    gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);

    SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
    draw_area_(pp->vbox_area[0], 0, 211, 150);
    SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
    draw_area_(pp->vbox_area[0], 1, 211, 275);
    SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0x2);
    draw_area_(pp->vbox_area[1], 2, 212, 150);
    SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0x2);
    draw_area_(pp->vbox_area[1], 3, 212, 275);
    SetDrawWindowConfig (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x0);
    draw_area_(pp->vbox_area[2], 4, 232, 150);
    SetDrawWindowConfig (&(pp->draw_area[5]), pp->draw_area[5].scan_type, 0x0);
    draw_area_(pp->vbox_area[2], 5, 232, 275);
    gtk_widget_show (pp->hbox_area[0]);
    gtk_widget_show (pp->vbox_area[0]);
    gtk_widget_show (pp->vbox_area[1]);
    gtk_widget_show (pp->vbox_area[2]);
}

void draw_3_area_a_c_cc_scan()
{
	if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 0, 211, 150);
		SetDrawWindowConfig (&(pp->draw_area[1]), C_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 1, 211, 275);
		SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x2);
		draw_area_(pp->vbox_area[1], 2, 212, 150);
		SetDrawWindowConfig (&(pp->draw_area[3]), C_SCAN, 0x2);
		draw_area_(pp->vbox_area[1], 3, 212, 275);
		SetDrawWindowConfig (&(pp->draw_area[4]), A_SCAN, 0x0);
		draw_area_(pp->vbox_area[2], 4, 232, 150);
		SetDrawWindowConfig (&(pp->draw_area[5]), C_SCAN, 0x0);
		draw_area_(pp->vbox_area[2], 5, 232, 275);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
		gtk_widget_show (pp->vbox_area[2]);

	}
	else
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 0, 211, 145);
		SetDrawWindowConfig (&(pp->draw_area[1]), C_SCAN, 0x3);
		draw_area_(pp->vbox_area[0], 1, 211, 130);
		SetDrawWindowConfig (&(pp->draw_area[2]), CC_SCAN, 0x2);
		draw_area_(pp->vbox_area[0], 2, 211, 150);
		SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0x2);
		draw_area_(pp->vbox_area[1], 3, 212, 145);
		SetDrawWindowConfig (&(pp->draw_area[4]), C_SCAN, 0x3);
		draw_area_(pp->vbox_area[1], 4, 212, 130);
		SetDrawWindowConfig (&(pp->draw_area[5]), CC_SCAN, 0x2);
		draw_area_(pp->vbox_area[1], 5, 212, 150);
		SetDrawWindowConfig (&(pp->draw_area[6]), A_SCAN, 0);
		draw_area_(pp->vbox_area[2], 6, 232, 145);
		SetDrawWindowConfig (&(pp->draw_area[7]), C_SCAN, 0x1);
		draw_area_(pp->vbox_area[2], 7, 232, 130);
		SetDrawWindowConfig (&(pp->draw_area[8]), CC_SCAN, 0x0);
		draw_area_(pp->vbox_area[2], 8, 232, 150);

		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
		gtk_widget_show (pp->vbox_area[2]);

	}
}

void draw_3_area_a_s_cc_scan()
{
	if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 0 , 211, 150);
		SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 1 , 211, 275);
		SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 3 , 212, 150);
		SetDrawWindowConfig (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 4 , 212, 275);
		SetDrawWindowConfig (&(pp->draw_area[6]), pp->draw_area[6].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 6 , 232, 150);
		SetDrawWindowConfig (&(pp->draw_area[7]), pp->draw_area[7].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 7 , 232, 275);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
		gtk_widget_show (pp->vbox_area[2]);

	}
	else
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 0 , 211, 135);
		SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 1 , 211, 145);
		SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 2 , 211, 145);

		SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 3 , 212, 135);
		SetDrawWindowConfig (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 4 , 212, 145);
		SetDrawWindowConfig (&(pp->draw_area[5]), pp->draw_area[5].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 5 , 212, 145);

		SetDrawWindowConfig (&(pp->draw_area[6]), pp->draw_area[6].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 6 , 232, 135);
		SetDrawWindowConfig (&(pp->draw_area[7]), pp->draw_area[7].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 7 , 232, 145);
		SetDrawWindowConfig (&(pp->draw_area[8]), pp->draw_area[8].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 8 , 232, 145);

		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
		gtk_widget_show (pp->vbox_area[2]);

	}
}

void draw_4_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable),    pp->vbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[0], 0 , 317, 212);
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[0], 1 , 338, 212);
	SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[1], 2 , 317, 213);
	SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[1], 3 , 338, 213);
	gtk_widget_show (pp->vbox_area[0]);
	gtk_widget_show (pp->hbox_area[0]);
	gtk_widget_show (pp->hbox_area[1]);

}

void draw_5_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[0], 0 , 317, 141);
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0);
	draw_area_ (pp->hbox_area[0], 1 , 338, 141);
	SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[1], 2 , 317, 142);
	SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[1], 3 , 338, 142);
	SetDrawWindowConfig (&(pp->draw_area[4]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[2], 4 , 337, 142);
	gtk_widget_show (pp->vbox_area[0]);
	gtk_widget_show (pp->hbox_area[0]);
	gtk_widget_show (pp->hbox_area[1]);
	gtk_widget_show (pp->hbox_area[2]);

}

void draw_6_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[0], 0 , 317, 141);
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0);
	draw_area_ (pp->hbox_area[0], 1 , 338, 141);
	SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[1], 2 , 317, 142);
	SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[1], 3 , 338, 142);
	SetDrawWindowConfig (&(pp->draw_area[4]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[2], 4 , 317, 142);
	SetDrawWindowConfig (&(pp->draw_area[5]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[2], 5 , 338, 142);
	gtk_widget_show (pp->vbox_area[0]);
	gtk_widget_show (pp->hbox_area[0]);
	gtk_widget_show (pp->hbox_area[1]);
	gtk_widget_show (pp->hbox_area[2]);
}

void draw_7_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[3], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[0], 0 , 317, 106);
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[0], 1 , 338, 106);
	SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[1], 2 , 317, 106);
	SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[1], 3 , 338, 106);
	SetDrawWindowConfig (&(pp->draw_area[4]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[2], 4 , 317, 106);
	SetDrawWindowConfig (&(pp->draw_area[5]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[2], 5 , 338, 106);
	SetDrawWindowConfig (&(pp->draw_area[6]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[3], 6 , 337, 107);
	gtk_widget_show (pp->vbox_area[0]);
	gtk_widget_show (pp->hbox_area[0]);
	gtk_widget_show (pp->hbox_area[1]);
	gtk_widget_show (pp->hbox_area[2]);
	gtk_widget_show (pp->hbox_area[3]);

}

void draw_8_area_a_scan()
{
	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[3], FALSE, FALSE, 0);
	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[0], 0 , 317, 106);
	SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[0], 1 , 338, 106);
	SetDrawWindowConfig (&(pp->draw_area[2]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[1], 2 , 317, 106);
	SetDrawWindowConfig (&(pp->draw_area[3]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[1], 3 , 338, 106);
	SetDrawWindowConfig (&(pp->draw_area[4]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[2], 4 , 317, 106);
	SetDrawWindowConfig (&(pp->draw_area[5]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[2], 5 , 338, 106);
	SetDrawWindowConfig (&(pp->draw_area[6]), A_SCAN, 0x2);
	draw_area_ (pp->hbox_area[3], 6 , 317, 107);
	SetDrawWindowConfig (&(pp->draw_area[7]), A_SCAN, 0x0);
	draw_area_ (pp->hbox_area[3], 7 , 338, 107);
	gtk_widget_show (pp->vbox_area[0]);
	gtk_widget_show (pp->hbox_area[0]);
	gtk_widget_show (pp->hbox_area[1]);
	gtk_widget_show (pp->hbox_area[2]);
	gtk_widget_show (pp->hbox_area[3]);
}


void DrawCurrentGroupDisplayArea()
{
	//printf("DrawTwoGroupDisplayArea\n");
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_cur_area_a_scan();
			break;
		case B_SCAN:
			draw_cur_area_b_scan();
			break;
		case C_SCAN:
			draw_cur_area_c_scan();
			break;
		case S_SCAN:
			draw_cur_area_s_scan();
			break;
		case A_B_SCAN:
			draw_cur_area_a_b_scan();
			break;
		case A_B_C_SCAN:
			draw_cur_area_a_b_c_scan();
			break;
		case A_B_S_SCAN:
			draw_cur_area_a_b_s_scan();
			break;
		case A_C_CC_SCAN:
			draw_cur_area_a_c_cc_scan();
			break;
		case A_S_CC_SCAN:
			draw_cur_area_a_s_cc_scan();
			break;
		//case Strip_Chart_AA:
		//	break;
		default:
			break;
	}
}
void Draw_PATOFD_DisplayArea()
{
	int _nGroupQty = get_group_qty( pp->p_config ) ;
	if(_nGroupQty == 2)
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);

		if ((GROUP_VAL_POS(0 , group_mode) != PA_SCAN) &&
                (GROUP_VAL_POS(1 , group_mode) != PA_SCAN)) {
            SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 6);
            draw_area_(pp->vbox_area[0], 0 , 230, 212);
            SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0);
            draw_area_(pp->vbox_area[1], 1 , 425, 212);
            
            SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 6);
            draw_area_(pp->vbox_area[0], 2, 230, 212);
            SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
            draw_area_(pp->vbox_area[1], 3, 425, 212);
        } else {
            SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
            draw_area_(pp->vbox_area[0], 0 , 317, 150);
            SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
            draw_area_(pp->vbox_area[0], 1 , 317, 275);
            SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0);
            draw_area_(pp->vbox_area[1], 2 , 338, 150);
            SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
            draw_area_(pp->vbox_area[1], 3 , 338, 275);
        }

		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
	}
	if(_nGroupQty == 3)
	{
		gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
		SetDrawWindowConfig (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 0 , 211, 150);
		SetDrawWindowConfig (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x2);
		draw_area_(pp->vbox_area[0], 1 , 211, 275);
		SetDrawWindowConfig (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 2 , 212, 150);
		SetDrawWindowConfig (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0x2);
		draw_area_(pp->vbox_area[1], 3 , 212, 275);
		SetDrawWindowConfig (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 4 , 232, 150);
		SetDrawWindowConfig (&(pp->draw_area[5]), pp->draw_area[5].scan_type, 0x0);
		draw_area_(pp->vbox_area[2], 5 , 232, 275);
		gtk_widget_show (pp->hbox_area[0]);
		gtk_widget_show (pp->vbox_area[0]);
		gtk_widget_show (pp->vbox_area[1]);
		gtk_widget_show (pp->vbox_area[2]);
	}
}

void DrawTwoGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_2_area_a_scan();
			break;
		case B_SCAN:
			draw_2_area_b_scan();
			break;
		case S_SCAN:
			draw_2_area_s_scan();
			break;
        case A_B_SCAN:
			draw_2_area_a_b_scan();
			break;
		case A_C_CC_SCAN:
			draw_2_area_a_c_cc_scan();
			break;
		case A_S_CC_SCAN:
			draw_2_area_a_s_cc_scan();
			break;
		case PA_TOFD:
		case PA_TOFD_CSCAN:
			Draw_PATOFD_DisplayArea() ;
			break;
		default:
			break;
	}
}

void DrawThreeGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_3_area_a_scan();
			break;
		case B_SCAN:
			draw_3_area_b_scan();
			break;
		case S_SCAN:
			draw_3_area_s_scan();
			break;
		case A_B_SCAN:
			draw_3_area_a_b_scan();
			break;
		case A_C_CC_SCAN:
			draw_3_area_a_c_cc_scan();
			break;
		case A_S_CC_SCAN:
			draw_3_area_a_s_cc_scan();
			break;
		case PA_TOFD:
		case PA_TOFD_CSCAN:
			Draw_PATOFD_DisplayArea();
			break;
		default:break;
	}
}

void DrawFourGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_4_area_a_scan();
			break;
		default:break;
	}
}

void DrawFiveGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_5_area_a_scan();
			break;
		default:break;
	}
}

void DrawSixGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_6_area_a_scan();
			break;
		default:break;
	}
}

void DrawSevenGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_7_area_a_scan();
			break;
		default:break;
	}
}

void DrawEightGroupDisplayArea()
{
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			draw_8_area_a_scan();
			break;
		default:break;
	}
}

void SetCurrentGroupFrameBufferConfig()
{
	int grp = get_current_group (pp->p_config) ;
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case B_SCAN:
            _nDisplayModal = B_SCAN_VERT;
		case A_SCAN:
		case C_SCAN:
			pp->draw_area[0].group	=	grp;
			SetDrawWindowBlackAreaConfig (0, _nDisplayModal, 605, 390, 0, 0, grp);
			break ;
		case S_SCAN:
			pp->draw_area[0].group	=	grp;
			if ( GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH )
			{
				if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_A;
					SetDrawWindowBlackAreaConfig (0, S_SCAN_A, 605, 390, 0, 0, grp );
				}
				else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_L;
					SetDrawWindowBlackAreaConfig (0, S_SCAN_L, 605, 390, 0, 0, grp );
				}
			}
			else
			{
				pp->draw_area[0].scan_type	=	S_SCAN;
				SetDrawWindowBlackAreaConfig (0, S_SCAN, 605, 390, 0, 0, grp);
			}
			break;
		case A_B_SCAN:
			pp->draw_area[0].group	=	grp;
			pp->draw_area[1].group	=	grp;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 115, 0,   0, grp);
			SetDrawWindowBlackAreaConfig (1, B_SCAN_VERT, 605, 260, 0, 130, grp);
			break;
		case A_B_C_SCAN:
			pp->draw_area[0].group	=	grp;
			pp->draw_area[1].group	=	grp;
			pp->draw_area[2].group	=	grp;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 110, 0,   0, grp);
			SetDrawWindowBlackAreaConfig (1, B_SCAN_VERT, 605, 115, 0, 125, grp);
			SetDrawWindowBlackAreaConfig (2, C_SCAN, 605, 115, 0, 275, grp);
			break;
		case A_B_S_SCAN:
			pp->draw_area[0].group	=	grp;
			pp->draw_area[1].group	=	grp;
			pp->draw_area[2].group	=	grp;
			if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
					SetDrawWindowBlackAreaConfig (0, S_SCAN_A, 250, 390, 0, 0, grp);
				else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
					SetDrawWindowBlackAreaConfig (0, S_SCAN_L, 250, 390, 0, 0, grp);
				SetDrawWindowBlackAreaConfig (1, A_SCAN, 305, 195, 300, 0, grp);
				SetDrawWindowBlackAreaConfig (2, B_SCAN_VERT, 305, 180, 300, 210, grp);
			}
			else
			{
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 110, 0, 0, grp);
				SetDrawWindowBlackAreaConfig (1, S_SCAN, 605, 135, 0, 125, grp);
				SetDrawWindowBlackAreaConfig (2, B_SCAN_VERT, 605, 115, 0, 275, grp);
			}
			break;
		case A_C_CC_SCAN:
			pp->draw_area[0].group	=	grp;
			pp->draw_area[1].group	=	grp;
			pp->draw_area[2].group  =   grp;
			if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
			{
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 115, 0, 0, grp);
				SetDrawWindowBlackAreaConfig (1, C_SCAN, 605, 240, 0, 150, grp);
			}
			else
			{
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 90, 0, 0, grp);
				SetDrawWindowBlackAreaConfig (1, C_SCAN, 605, 135, 0, 125, grp);
				SetDrawWindowBlackAreaConfig (2, CC_SCAN, 605, 115, 0, 275, grp);
			}
			break;
		case A_S_CC_SCAN:
			pp->draw_area[0].group	=	grp;
			pp->draw_area[1].group	=	grp;
			pp->draw_area[2].group	=	grp;
			if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
			{
				if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
					{
						SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 390, 0, 0, grp);
						SetDrawWindowBlackAreaConfig (1, S_SCAN_A, 355, 390, 250, 0, grp);
					}
					else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
					{
						SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 390, 0, 0, grp);
						SetDrawWindowBlackAreaConfig (1, S_SCAN_L, 355, 390, 250, 0, grp);
					}
				}
				else
				{
					SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 115, 0, 0, grp);
					SetDrawWindowBlackAreaConfig (1, S_SCAN, 605, 260, 0, 130, grp);
				}
			}
			else
			{
			    if (GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
					{
						SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 240, 0, 0, grp);
						SetDrawWindowBlackAreaConfig (1, S_SCAN_A, 355, 240, 250, 0, grp);
						SetDrawWindowBlackAreaConfig (2, CCC_SCAN, 605, 115, 0, 275, grp);
					}
					else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
					{
						SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 240, 0, 0, grp);
						SetDrawWindowBlackAreaConfig (1, S_SCAN_L, 355, 240, 250, 0, grp);
						SetDrawWindowBlackAreaConfig (2, CCC_SCAN, 605, 115, 0, 275, grp);
					}
				}
			    else
			    {
					SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 90, 0, 0, grp);
					SetDrawWindowBlackAreaConfig (1, S_SCAN, 605, 135, 0, 105, grp);
					SetDrawWindowBlackAreaConfig (2, CCC_SCAN, 605, 115, 0, 275, grp);
				}
			}
			break;
		default:
			break;
	}
}

void Setting_PATOFD_FrameBufferConfigure(int mode)
{
	int _nGroupQty = get_group_qty( pp->p_config ) ;
	if( _nGroupQty == 2 )
	{
        pp->draw_area[0].group	=	0;
        pp->draw_area[1].group	=	0;
        pp->draw_area[2].group	=	1;
        pp->draw_area[3].group	=	1;

		if ((GROUP_VAL_POS(0 , group_mode) != PA_SCAN) &&
                (GROUP_VAL_POS(1 , group_mode) != PA_SCAN)) {
    		pp->draw_area[0].scan_type	=   A_SCAN_R;
    		pp->draw_area[2].scan_type	=	A_SCAN_R;
    	    pp->draw_area[1].scan_type	=  TOFD_B_SCAN  ;
    		pp->draw_area[3].scan_type	=  TOFD_B_SCAN  ;
    		SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 210, 177, 0, 0, 0);
    		SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 375, 177, 230, 0, 0);
    		SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 210, 177, 0, 212,  1);
    		SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 375, 177, 230, 212, 1);
        } else {
    		pp->draw_area[0].scan_type	=	A_SCAN;
    		pp->draw_area[2].scan_type	=	A_SCAN;

    		if(GROUP_VAL_POS(0 , group_mode) != PA_SCAN) {
	    		pp->draw_area[1].scan_type	=  B_SCAN_VERT  ;
    		} else {
                if (mode == PA_TOFD_CSCAN) {
                    pp->draw_area[1].scan_type	=	C_SCAN_VE;
                } else if ((GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_SOUNDPATH) ||
                        (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TIME)) {
                        pp->draw_area[1].scan_type	=	S_SCAN;
                } else if (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH) {
                    if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN) {
                        pp->draw_area[1].scan_type	=	S_SCAN_A;
                    } else if (LAW_VAL_POS(0 , Focal_type) == LINEAR_SCAN) {
                        pp->draw_area[1].scan_type	=	S_SCAN_L;
                    }
                }
            }

            if(GROUP_VAL_POS(1 , group_mode) != PA_SCAN) {
                pp->draw_area[3].scan_type	=  B_SCAN_VERT  ;
            } else {
                if (mode == PA_TOFD_CSCAN) {
                    pp->draw_area[3].scan_type	=	C_SCAN_VE;
                } else if ((GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_SOUNDPATH) ||
                        (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TIME)) {
                    pp->draw_area[3].scan_type	=	S_SCAN;
                } else if (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH) {
                    if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN) {
                        pp->draw_area[3].scan_type	=	S_SCAN_A;
                    } else if (LAW_VAL_POS(1 , Focal_type) == LINEAR_SCAN) {
                        pp->draw_area[3].scan_type	=	S_SCAN_L;
                    }
                }
            }
            SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 287, 115, 0, 0,  0);
            SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 287, 240, 0, 150, 0);
            SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 288, 115, 317, 0,  1);
            SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 288, 240, 317,150, 1);
        }
	}
	if(_nGroupQty == 3)
	{
		pp->draw_area[0].group	=	0;
		pp->draw_area[1].group	=	0;
		pp->draw_area[2].group	=	1;
		pp->draw_area[3].group	=	1;
		pp->draw_area[4].group	=	2;
		pp->draw_area[5].group	=	2;
		pp->draw_area[0].scan_type	=	A_SCAN;
		pp->draw_area[2].scan_type	=	A_SCAN;
		pp->draw_area[4].scan_type	=	A_SCAN;

		//************************************************
		//if(GROUP_VAL_POS(0 , group_mode) == UT1_SCAN || GROUP_VAL_POS(0 , group_mode) == UT2_SCAN)
		if(GROUP_VAL_POS(0 , group_mode) != PA_SCAN)
		{
			pp->draw_area[1].scan_type	=  B_SCAN_VERT  ;
		}
		else
		{
            if (mode == PA_TOFD_CSCAN) {
                pp->draw_area[1].scan_type	=	C_SCAN_VE;
            } else if ((GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_SOUNDPATH) ||
                    (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[1].scan_type	=	S_SCAN;
			}
			else if (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[1].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(0 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[1].scan_type	=	S_SCAN_L;
				}
			}
		}
        //************************************************
		//if(GROUP_VAL_POS(1 , group_mode) == UT1_SCAN || GROUP_VAL_POS(1 , group_mode) == UT2_SCAN)
		if(GROUP_VAL_POS(1 , group_mode) != PA_SCAN)
		{
			pp->draw_area[3].scan_type	=  B_SCAN_VERT  ;
		}
		else
		{
            if (mode == PA_TOFD_CSCAN) {
                pp->draw_area[3].scan_type	=	C_SCAN_VE;
            } else if ((GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[3].scan_type	=	S_SCAN;
			}
			else if (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[3].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(1 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[3].scan_type	=	S_SCAN_L;
				}
			}
		}
		//************************************************
		//if(GROUP_VAL_POS(2 , group_mode) == UT1_SCAN || GROUP_VAL_POS(2 , group_mode) == UT2_SCAN)
		if(GROUP_VAL_POS(2 , group_mode) != PA_SCAN)
		{
			pp->draw_area[5].scan_type	=  B_SCAN_VERT  ;
		}
		else
		{
            if (mode == PA_TOFD_CSCAN) {
                pp->draw_area[5].scan_type	=	C_SCAN_VE;
            } else if ((GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[5].scan_type	=	S_SCAN;
			}
			else if (GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(2 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[5].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(2 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[5].scan_type	=	S_SCAN_L;
				}
			}
		}
		//************************************************
		SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 181, 115, 0, 0,  0);
		SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 181, 240, 0, 150, 0);
		SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 182, 115, 211, 0,  1);
		SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 182, 240, 211, 150, 1);
		SetDrawWindowBlackAreaConfig (4, pp->draw_area[4].scan_type, 182, 115, 423, 0,  2);
		SetDrawWindowBlackAreaConfig (5, pp->draw_area[5].scan_type, 182, 240, 423, 150, 2);
	}
}

void SetTwoGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 178, 0,   0,  0);//163
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 605, 177, 0, 213,  1);//178 197
			break;
		case B_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			SetDrawWindowBlackAreaConfig (0, B_SCAN_VERT, 605, 178, 0,   0,  0);//163
			SetDrawWindowBlackAreaConfig (1, B_SCAN_VERT, 605, 177, 0, 213,  1);//178 197
			break;
		case S_SCAN:
			if(GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(0 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_L;
				}
			}
			else
			{
				pp->draw_area[0].scan_type	= S_SCAN;
			}
			if(GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[1].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(1 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[1].scan_type	=	S_SCAN_L;
				}
			}
			else
			{
				pp->draw_area[1].scan_type	= S_SCAN;
			}
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 287, 390, 0, 0,  0) ;
			SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 288, 390, 317, 0, 1);
			break;
		case A_B_SCAN:
            pp->draw_area[0].group	=	0;
            pp->draw_area[1].group	=	0;
            pp->draw_area[2].group	=	1;
            pp->draw_area[3].group	=	1;
    		pp->draw_area[0].scan_type	=	A_SCAN;
    		pp->draw_area[2].scan_type	=	A_SCAN;
            pp->draw_area[1].scan_type	=  B_SCAN_VERT;
            pp->draw_area[3].scan_type	=  B_SCAN_VERT;
            SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 287, 115, 0, 0,  0);
            SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 287, 240, 0, 150, 0);
            SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 288, 115, 317, 0,  1);
            SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 288, 240, 317,150, 1);
			break;
		case A_C_CC_SCAN:
			if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
			{
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	1;
				pp->draw_area[3].group	=	1;
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 115, 0, 0,  0);
				SetDrawWindowBlackAreaConfig (1, C_SCAN, 287, 255, 0, 150, 0);
				SetDrawWindowBlackAreaConfig (2, A_SCAN, 288, 115, 317, 0,  1);
				SetDrawWindowBlackAreaConfig (3, C_SCAN, 288, 255, 317,150, 1);
			}
			else
			{
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	0;
				pp->draw_area[3].group	=	1;
				pp->draw_area[4].group	=	1;
				pp->draw_area[5].group	=	1;
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 110, 0,    0,  0);
				SetDrawWindowBlackAreaConfig (1, C_SCAN, 287, 115, 0,  145,  0);
				SetDrawWindowBlackAreaConfig (2, CC_SCAN, 287, 115, 0,  275,  0);
				SetDrawWindowBlackAreaConfig (3, A_SCAN, 288, 110, 317,  0,  1);
				SetDrawWindowBlackAreaConfig (4, C_SCAN, 288, 115, 317, 145, 1);
				SetDrawWindowBlackAreaConfig (5, CC_SCAN, 288, 115, 317, 275, 1);
			}
			break;
		case A_S_CC_SCAN:
			if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
			{
				if ((GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_SOUNDPATH) ||
						(GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TIME))
				{
					pp->draw_area[0].scan_type	=	A_SCAN;
					pp->draw_area[1].scan_type	=	S_SCAN;
				}
				else if (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN)
					{
						pp->draw_area[0].scan_type	=	A_SCAN;
						pp->draw_area[1].scan_type	=	S_SCAN_A;
					}
					else if (LAW_VAL_POS(0 , Focal_type) == LINEAR_SCAN)
					{
						pp->draw_area[0].scan_type	=	A_SCAN;
						pp->draw_area[1].scan_type	=	S_SCAN_L;
					}
				}

				if ((GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_SOUNDPATH) ||
						(GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TIME))
				{
					pp->draw_area[2].scan_type	=	A_SCAN;
					pp->draw_area[3].scan_type	=	S_SCAN;
				}
				else if (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN)
					{
						pp->draw_area[2].scan_type	=	A_SCAN;
						pp->draw_area[3].scan_type	=	S_SCAN_A;
					}
					else if (LAW_VAL_POS(1 , Focal_type) == LINEAR_SCAN)
					{
						pp->draw_area[2].scan_type	=	A_SCAN;
						pp->draw_area[3].scan_type	=	S_SCAN_L;
					}
				}
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	1;
				pp->draw_area[3].group	=	1;
				SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 287, 115, 0, 0,  0);
				SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 287, 240, 0, 150, 0);
				SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 288, 115, 317, 0,  1);
				SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 288, 240, 317,150, 1);
			}
			else
			{
				if ((GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_SOUNDPATH) ||
						(GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TIME))
				{
					pp->draw_area[0].scan_type	=	A_SCAN;
					pp->draw_area[1].scan_type	=	S_SCAN;
					pp->draw_area[2].scan_type	=	CCC_SCAN;
				}
				else if (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN)
					{
						pp->draw_area[0].scan_type	=	A_SCAN;
						pp->draw_area[1].scan_type	=	S_SCAN_A;
						pp->draw_area[2].scan_type	=	CCC_SCAN;
					}
					else if (LAW_VAL_POS(0 , Focal_type) == LINEAR_SCAN)
					{
						pp->draw_area[0].scan_type	=	A_SCAN;
						pp->draw_area[1].scan_type	=	S_SCAN_L;
						pp->draw_area[2].scan_type	=	CCC_SCAN;
					}
				}

				if ((GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_SOUNDPATH) ||
						(GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TIME))
				{
					pp->draw_area[3].scan_type	=	A_SCAN;
					pp->draw_area[4].scan_type	=	S_SCAN;
					pp->draw_area[5].scan_type	=	CCC_SCAN;
				}
				else if (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN)
					{
						pp->draw_area[3].scan_type	=	A_SCAN;
						pp->draw_area[4].scan_type	=	S_SCAN_A;
						pp->draw_area[5].scan_type	=	CCC_SCAN;
					}
					else if (LAW_VAL_POS(1 , Focal_type) == LINEAR_SCAN)
					{
						pp->draw_area[3].scan_type	=	A_SCAN;
						pp->draw_area[4].scan_type	=	S_SCAN_L;
						pp->draw_area[5].scan_type	=	CCC_SCAN;
					}
				}
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	0;
				pp->draw_area[3].group	=	1;
				pp->draw_area[4].group	=	1;
				pp->draw_area[5].group	=	1;
				SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 287, 100, 0,    0,  0);
				SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 287, 110, 0,  135,  0);
				SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 287, 110, 0,  280,  0);
				SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 288, 100, 317,  0,  1);
				SetDrawWindowBlackAreaConfig (4, pp->draw_area[4].scan_type, 288, 110, 317, 135, 1);
				SetDrawWindowBlackAreaConfig (5, pp->draw_area[5].scan_type, 288, 110, 317, 280, 1);
			}
			break;
		case PA_TOFD:
		case PA_TOFD_CSCAN:
			Setting_PATOFD_FrameBufferConfigure(_nDisplayModal);
			break;
		default:
			break;
	}
}

void SetThreeGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 615, 106, 0, 0,  0);
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 615, 107, 0, 141, 1);
			SetDrawWindowBlackAreaConfig (2, A_SCAN, 615, 107, 0, 283, 2);
			break;
		case B_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			SetDrawWindowBlackAreaConfig (0, B_SCAN_VERT, 615, 106, 0, 0,  0);
			SetDrawWindowBlackAreaConfig (1, B_SCAN_VERT, 615, 107, 0, 141, 1);
			SetDrawWindowBlackAreaConfig (2, B_SCAN_VERT, 615, 107, 0, 283, 2);
			break;
		case S_SCAN:
			if ((GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[0].scan_type	=	S_SCAN;
			}
			else if (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(0 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_L;
				}
			}

			if ((GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[1].scan_type	=	S_SCAN;
			}
			else if (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[1].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(1 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[1].scan_type	=	S_SCAN_L;
				}
			}

			if ((GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[2].scan_type	=	S_SCAN;
			}
			else if (GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(2 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[2].scan_type	=	S_SCAN_A;
				}
				else if (LAW_VAL_POS(2 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[2].scan_type	=	S_SCAN_L;
				}
			}
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 181, 390, 0, 0,  0);
			SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 182, 390, 211, 0, 1);
			SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 182, 390, 423, 0, 2);
			break;
        case A_B_SCAN:
            pp->draw_area[0].group	=	0;
            pp->draw_area[1].group	=	0;
            pp->draw_area[2].group	=	1;
            pp->draw_area[3].group	=	1;
            pp->draw_area[4].group	=	2;
            pp->draw_area[5].group	=	2;
            pp->draw_area[0].scan_type = A_SCAN;
            pp->draw_area[1].scan_type = B_SCAN_VERT;
            pp->draw_area[2].scan_type = A_SCAN;
            pp->draw_area[3].scan_type = B_SCAN_VERT;
            pp->draw_area[4].scan_type = A_SCAN;
            pp->draw_area[5].scan_type = B_SCAN_VERT;
            SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 181, 115,   0,   0,  0);
            SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 181, 240,   0, 150,  0);
            SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 182, 115, 211,   0,  1);
            SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 182, 240, 211, 150,  1);
            SetDrawWindowBlackAreaConfig (4, pp->draw_area[4].scan_type, 182, 115, 423,   0,  2);
            SetDrawWindowBlackAreaConfig (5, pp->draw_area[5].scan_type, 182, 240, 423, 150,  2);
			break;
		case A_C_CC_SCAN:
			if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
			{
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	1;
				pp->draw_area[3].group	=	1;
				pp->draw_area[4].group	=	2;
				pp->draw_area[5].group	=	2;
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 181, 115,   0,   0,  0);
				SetDrawWindowBlackAreaConfig (1, C_SCAN, 181, 240,   0, 150,  0);
				SetDrawWindowBlackAreaConfig (2, A_SCAN, 182, 115, 211,   0,  1);
				SetDrawWindowBlackAreaConfig (3, C_SCAN, 182, 240, 211, 150,  1);
				SetDrawWindowBlackAreaConfig (4, A_SCAN, 182, 115, 423,   0,  2);
				SetDrawWindowBlackAreaConfig (5, C_SCAN, 182, 240, 423, 150,  2);
			}
			else
			{
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	0;
				pp->draw_area[3].group	=	1;
				pp->draw_area[4].group	=	1;
				pp->draw_area[5].group	=	1;
				pp->draw_area[6].group	=	2;
				pp->draw_area[7].group	=	2;
				pp->draw_area[8].group	=	2;
				SetDrawWindowBlackAreaConfig (0, A_SCAN, 181, 110, 0,     0, 0);
				SetDrawWindowBlackAreaConfig (1, C_SCAN, 181, 115, 0,   145, 0);
				SetDrawWindowBlackAreaConfig (2, CC_SCAN, 181, 115, 0,   275, 0);
				SetDrawWindowBlackAreaConfig (3, A_SCAN, 182, 110, 211,  0,  1);
				SetDrawWindowBlackAreaConfig (4, C_SCAN, 182, 115, 211, 145, 1);
				SetDrawWindowBlackAreaConfig (5, CC_SCAN, 182, 115, 211, 275, 1);
				SetDrawWindowBlackAreaConfig (6, A_SCAN, 182, 110, 423,  0,  2);
				SetDrawWindowBlackAreaConfig (7, C_SCAN, 182, 115, 423, 145, 2);
				SetDrawWindowBlackAreaConfig (8, CC_SCAN, 182, 115, 423, 275, 2);
			}
			break;
		case A_S_CC_SCAN:
			if ((GROUP_VAL_POS(0 ,ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[0].scan_type	=	A_SCAN;
				pp->draw_area[1].scan_type	=	S_SCAN;
				pp->draw_area[2].scan_type	=	CCC_SCAN;
			}
			else if (GROUP_VAL_POS(0 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(0 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[0].scan_type	=	A_SCAN;
					pp->draw_area[1].scan_type	=	S_SCAN_A;
					pp->draw_area[2].scan_type	=	CCC_SCAN;
				}
				else if (LAW_VAL_POS( 0 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[0].scan_type	=	A_SCAN;
					pp->draw_area[1].scan_type	=	S_SCAN_L;
					pp->draw_area[2].scan_type	=	CCC_SCAN;
				}
			}

			if ((GROUP_VAL_POS(1 ,ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[3].scan_type	=	A_SCAN;
				pp->draw_area[4].scan_type	=	S_SCAN;
				pp->draw_area[5].scan_type	=	CCC_SCAN;
			}
			else if (GROUP_VAL_POS(1 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(1 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[3].scan_type	=	A_SCAN;
					pp->draw_area[4].scan_type	=	S_SCAN_A;
					pp->draw_area[5].scan_type	=	CCC_SCAN;
				}
				else if (LAW_VAL_POS( 1 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[3].scan_type	=	A_SCAN;
					pp->draw_area[4].scan_type	=	S_SCAN_L;
					pp->draw_area[5].scan_type	=	CCC_SCAN;
				}
			}
			if ((GROUP_VAL_POS(2 ,ut_unit) == UT_UNIT_SOUNDPATH) ||
					(GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_TIME))
			{
				pp->draw_area[6].scan_type	=	A_SCAN;
				pp->draw_area[7].scan_type	=	S_SCAN;
				pp->draw_area[8].scan_type	=	CCC_SCAN;
			}
			else if (GROUP_VAL_POS(2 , ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if (LAW_VAL_POS(2 , Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[6].scan_type	=	A_SCAN;
					pp->draw_area[7].scan_type	=	S_SCAN_A;
					pp->draw_area[8].scan_type	=	CCC_SCAN;
				}
				else if (LAW_VAL_POS( 2 , Focal_type) == LINEAR_SCAN)
				{
					pp->draw_area[6].scan_type	=	A_SCAN;
					pp->draw_area[7].scan_type	=	S_SCAN_L;
					pp->draw_area[8].scan_type	=	CCC_SCAN;
				}
			}
			if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
			{
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[3].group	=	1;
				pp->draw_area[4].group	=	1;
				pp->draw_area[6].group	=	2;
				pp->draw_area[7].group	=	2;
				SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 181, 115, 0, 0,  0);
				SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 181, 240, 0, 150, 0);
				SetDrawWindowBlackAreaConfig (2, pp->draw_area[3].scan_type, 182, 115, 211, 0,  1);
				SetDrawWindowBlackAreaConfig (3, pp->draw_area[4].scan_type, 182, 240, 211, 150, 1);
				SetDrawWindowBlackAreaConfig (4, pp->draw_area[6].scan_type, 182, 115, 423, 0,  2);
				SetDrawWindowBlackAreaConfig (5, pp->draw_area[7].scan_type, 182, 240, 423, 150, 2);
			}
			else
			{
				pp->draw_area[0].group	=	0;
				pp->draw_area[1].group	=	0;
				pp->draw_area[2].group	=	0;
				pp->draw_area[3].group	=	1;
				pp->draw_area[4].group	=	1;
				pp->draw_area[5].group	=	1;
				pp->draw_area[6].group	=	2;
				pp->draw_area[7].group	=	2;
				pp->draw_area[8].group	=	2;
				SetDrawWindowBlackAreaConfig (0, pp->draw_area[0].scan_type, 181, 100,   0,   0,  0);
				SetDrawWindowBlackAreaConfig (1, pp->draw_area[1].scan_type, 181, 110,   0, 135, 0);
				SetDrawWindowBlackAreaConfig (2, pp->draw_area[2].scan_type, 181, 110,   0, 280, 0);
				SetDrawWindowBlackAreaConfig (3, pp->draw_area[3].scan_type, 182, 100, 211,   0, 1);
				SetDrawWindowBlackAreaConfig (4, pp->draw_area[4].scan_type, 182, 110, 211, 135,  1);
				SetDrawWindowBlackAreaConfig (5, pp->draw_area[5].scan_type, 182, 110, 211, 280, 1);
				SetDrawWindowBlackAreaConfig (6, pp->draw_area[6].scan_type, 182, 100, 423,   0, 2);
				SetDrawWindowBlackAreaConfig (7, pp->draw_area[7].scan_type, 182, 110, 423, 135,  2);
				SetDrawWindowBlackAreaConfig (8, pp->draw_area[8].scan_type, 182, 110, 423, 280, 2);
			}
			break;
		case PA_TOFD:
		case PA_TOFD_CSCAN:
			Setting_PATOFD_FrameBufferConfigure(_nDisplayModal);
			break;
		default:break;
	}
}

void SetFourGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			pp->draw_area[3].group	=	3;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 177,   0,   0, 0);
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 288, 177, 317,   0, 1);
			SetDrawWindowBlackAreaConfig (2, A_SCAN, 287, 178,   0, 212, 2);
			SetDrawWindowBlackAreaConfig (3, A_SCAN, 288, 178, 317, 212, 3);
			break;
		default:break;
	}
}

void SetFiveGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			pp->draw_area[3].group	=	3;
			pp->draw_area[4].group	=	4;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 106, 0,     0,  0);
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 288, 106, 317,   0, 1);
			SetDrawWindowBlackAreaConfig (2, A_SCAN, 287, 107, 0,   141, 2);
			SetDrawWindowBlackAreaConfig (3, A_SCAN, 288, 107, 317, 141, 3);
			SetDrawWindowBlackAreaConfig (4, A_SCAN, 287, 107, 0,   283, 4);
			break;
		default:break;
	}
}

void SetSixGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			pp->draw_area[3].group	=	3;
			pp->draw_area[4].group	=	4;
			pp->draw_area[5].group	=	5;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 106, 0,     0,  0);
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 288, 106, 317,   0, 1);
			SetDrawWindowBlackAreaConfig (2, A_SCAN, 287, 107, 0,   141, 2);
			SetDrawWindowBlackAreaConfig (3, A_SCAN, 288, 107, 317, 141, 3);
			SetDrawWindowBlackAreaConfig (4, A_SCAN, 287, 107, 0,   283, 4);
			SetDrawWindowBlackAreaConfig (5, A_SCAN, 288, 107, 317, 283, 5);
			break;
		default:break;
	}
}

void SetSevenGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			pp->draw_area[3].group	=	3;
			pp->draw_area[4].group	=	4;
			pp->draw_area[5].group	=	5;
			pp->draw_area[6].group	=	6;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 71, 0,     0, 0);
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 288, 71, 317,   0, 1);
			SetDrawWindowBlackAreaConfig (2, A_SCAN, 287, 71, 0,   106, 2);
			SetDrawWindowBlackAreaConfig (3, A_SCAN, 288, 71, 317, 106, 3);
			SetDrawWindowBlackAreaConfig (4, A_SCAN, 287, 71, 0,   212, 4);
			SetDrawWindowBlackAreaConfig (5, A_SCAN, 288, 71, 317, 212, 5);
			SetDrawWindowBlackAreaConfig (6, A_SCAN, 287, 72, 0,   318, 6);
			break;
		default:break;
	}
}

void SetEightGroupFrameBufferConfig()
{
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	switch (_nDisplayModal)
	{
		case A_SCAN:
			pp->draw_area[0].group	=	0;
			pp->draw_area[1].group	=	1;
			pp->draw_area[2].group	=	2;
			pp->draw_area[3].group	=	3;
			pp->draw_area[4].group	=	4;
			pp->draw_area[5].group	=	5;
			pp->draw_area[6].group	=	6;
			pp->draw_area[7].group	=	7;
			SetDrawWindowBlackAreaConfig (0, A_SCAN, 287, 71, 0,     0, 0);
			SetDrawWindowBlackAreaConfig (1, A_SCAN, 288, 71, 317,   0, 1);
			SetDrawWindowBlackAreaConfig (2, A_SCAN, 287, 71, 0,   106, 2);
			SetDrawWindowBlackAreaConfig (3, A_SCAN, 288, 71, 317, 106, 3);
			SetDrawWindowBlackAreaConfig (4, A_SCAN, 287, 71, 0,   212, 4);
			SetDrawWindowBlackAreaConfig (5, A_SCAN, 288, 71, 317, 212, 5);
			SetDrawWindowBlackAreaConfig (6, A_SCAN, 287, 72, 0,   318, 6);
			SetDrawWindowBlackAreaConfig (7, A_SCAN, 288, 72, 317, 318, 7);
			break;
		default:break;
	}
}

void SettingFrameBufferWindowConfigCalibration()
{
	memset (TMP(scan_type), 0xff, 16) ;
	unsigned char grp = get_current_group(pp->p_config)  ;
	//unsigned char _nDisplayModal = get_display_group(pp->p_config) ;
	switch(pp->ctype_pos)
	{
		case 0:
			break;
		case 1://Ultrasound
			if( !pp->cmode_pos )//Velocity
			{
				if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
				{
					pp->draw_area[0].group	=	grp;
					pp->draw_area[1].group	=	grp;
					if ((GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_SOUNDPATH) ||
							(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME))
					{
						SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 115, 0, 0, grp );
						SetDrawWindowBlackAreaConfig (1, S_SCAN, 605, 260, 0, 130, grp );
					}
					else if (GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
					{
						if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
						{
							SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 390, 0, 0, grp);
							SetDrawWindowBlackAreaConfig (1, S_SCAN_A, 355, 390, 250, 0, grp);
						}
						else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
						{
							SetDrawWindowBlackAreaConfig (0, A_SCAN_R, 230, 390, 0, 0, grp);
							SetDrawWindowBlackAreaConfig (1, S_SCAN_L, 355, 390, 250, 0, grp);
						}
					}
				}
				else
				{
					pp->draw_area[0].group	=	grp;
					SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 390, 0, 0, grp);
				}
			}
			else
			{
				if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
				{
					pp->draw_area[0].group	=	grp;
					pp->draw_area[1].group	=	grp;
					pp->draw_area[2].group	=	grp;

					if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_SOUNDPATH)
					{
						SetDrawWindowBlackAreaConfig (0, S_SCAN, 293, 177, 0, 0, grp);
						SetDrawWindowBlackAreaConfig (1, A_SCAN, 292, 177,  313, 0, grp);
					}
					else
					{
						if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
							SetDrawWindowBlackAreaConfig (0, S_SCAN_A, 293, 177, 0, 0, grp);
						else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
							SetDrawWindowBlackAreaConfig (0, S_SCAN_L, 293, 177, 0, 0, grp);
						if ( pp->cmode_pos == 3 ) //tcg
							SetDrawWindowBlackAreaConfig (1, A_SCAN    , 292, 177,  313, 0, grp);
						else
							SetDrawWindowBlackAreaConfig (1, A_SCAN_CLB, 292, 177,  313, 0, grp);
					}
					if( pp->cmode_pos == 1 )
						SetDrawWindowBlackAreaConfig (2, WEDGE_DELAY,   605, 178, 0, 212, grp);
					else if( pp->cmode_pos == 2 )
						SetDrawWindowBlackAreaConfig (2, SENSITIVITY,   605, 178, 0, 212, grp);
					else if( pp->cmode_pos == 3 )
						SetDrawWindowBlackAreaConfig (2, TCG,           605, 178, 0, 212, grp);
				}
				else
				{
					pp->draw_area[0].group	=	grp;
					SetDrawWindowBlackAreaConfig (0, A_SCAN, 605, 390, 0, 0, grp);
				}
			}
			break;
		case 2://Code
			SettingFrameBufferWindowConfig() ;
			break;
	}
}
/* 画波形数据显示区 */
void SettingFrameBufferWindowConfig()
{
	memset (TMP(scan_type), 0xff, 16) ;
	unsigned char _nGroupQty = get_group_qty(pp->p_config)  ;
	unsigned char _nDisplayModal = get_display_group (pp->p_config) ;

//	if(get_probe_fft (pp->p_config))
	int group = get_current_group(pp->p_config);
	if(gData->fft[group].enableFft)
	{
		DrawFFT_FrameBufferConfig() ; //draw_fft();
		return ;
	}

	if(!IsDisplayModalAvailable(get_display_pos(pp->p_config)))
	{
		set_display_pos(pp->p_config, A_SCAN) ;
	}

	if ( _nDisplayModal == DISPLAY_CURRENT_GROUP || _nGroupQty == 1)
	{
		SetCurrentGroupFrameBufferConfig();
	}
	else
	{
		switch (_nGroupQty)
		{
			case 2:
				SetTwoGroupFrameBufferConfig() ;
				break;
			case 3:
				SetThreeGroupFrameBufferConfig() ;
				break;
			case 4:
				SetFourGroupFrameBufferConfig() ;
				break;
			case 5:
				SetFiveGroupFrameBufferConfig() ;
				break;
			case 6:
				SetSixGroupFrameBufferConfig() ;
				break;
			case 7:
				SetSevenGroupFrameBufferConfig() ;
				break;
			case 8:
				SetEightGroupFrameBufferConfig() ;
				break;
			default:break;
		}
	}

}

void DrawDisplayWindowFrameCalibration()
{
	int i;
	/* 把之前的区域释放 */
	if(pp->vboxtable)
		gtk_widget_destroy(pp->vboxtable);
	pp->vboxtable = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_draw_area), pp->vboxtable, FALSE, FALSE, 0);
	gtk_widget_show(pp->vboxtable);

	for (i = 0; i < 4; i ++)
	{
		pp->vbox_area[i] = gtk_vbox_new(FALSE, 0);
		pp->hbox_area[i] = gtk_hbox_new(FALSE, 0);
	}
	int grp = get_current_group(pp->p_config) ;
	switch(pp->ctype_pos)
	{
		case 0:
			break;
		case 1://Ultrasound
			// UT display A scan
			if( !pp->cmode_pos )//Velocity
			{
                if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
                {
    				if ((GROUP_VAL_POS(grp, ut_unit) == UT_UNIT_SOUNDPATH) ||
    						(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME))
    				{
    					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
    					SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0x01);
    					draw_area_ (pp->vbox_area[0], 0 , 655, 130);
    					SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN, 0);
    					draw_area_ (pp->vbox_area[0], 1 , 655, 295);
    					gtk_widget_show (pp->vbox_area[0]);
    				}
    				else if (GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
    				{
    					if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
    					{
    						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
    						SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
    						draw_area_ (pp->hbox_area[0], 0 , 250, 425);
    						SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_A, 0);
    						draw_area_ (pp->hbox_area[0], 1 , 405, 425);
    						gtk_widget_show (pp->hbox_area[0]);
    					}
    					else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
    					{
    						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
    						SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN_R, 0x06);
    						draw_area_ (pp->hbox_area[0], 0 , 250, 425);
    						SetDrawWindowConfig (&(pp->draw_area[1]), S_SCAN_L, 0x0);
    						draw_area_ (pp->hbox_area[0], 1 , 405, 425);
    						gtk_widget_show (pp->hbox_area[0]);
    					}
    				}
                }
                else
                {
                	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
                	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
                	draw_area_ (pp->vbox_area[0], 0 , 655, 425);
                	gtk_widget_show (pp->vbox_area[0]);
                }

			}
			else
			{
                if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
                {
    				pp->draw_area[0].group	=	get_current_group(pp->p_config);
    				pp->draw_area[1].group	=	get_current_group(pp->p_config);
    				pp->draw_area[2].group	=	get_current_group(pp->p_config);

    				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
    				gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
    				gtk_widget_show (pp->hbox_area[0]);
    				gtk_widget_show (pp->vbox_area[0]);

    				if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_SOUNDPATH)
    				{
    					SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN, 0x06);
    					SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0);
    				}
    				else
    				{
    					if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
    						SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_A, 0x06);
    					else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
    						SetDrawWindowConfig (&(pp->draw_area[0]), S_SCAN_L, 0x06);
    					if( pp->cmode_pos == 3 )
    						SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN, 0);
    					else
    						SetDrawWindowConfig (&(pp->draw_area[1]), A_SCAN_CLB, 0);
    				}

    				draw_area_ (pp->hbox_area[0], 0, 313, 212);

    				draw_area_ (pp->hbox_area[0], 1, 342, 212);
    				if( pp->cmode_pos == 1 )
    				{
    					SetDrawWindowConfig (&(pp->draw_area[2]), WEDGE_DELAY, 0);
    					draw_area_ (pp->vbox_area[0], 2, 655, 213);
    				}
    				else if( pp->cmode_pos == 2 )
    				{
    					SetDrawWindowConfig (&(pp->draw_area[2]), SENSITIVITY, 0) ;
    					draw_area_ (pp->vbox_area[0], 2, 655, 213);
    				}
    				else if( pp->cmode_pos == 3 )
    				{
    					SetDrawWindowConfig (&(pp->draw_area[2]), TCG, 0);
    					draw_area_ (pp->vbox_area[0], 2, 655, 213);
    				}
                }
                else
                {
                	gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
                	SetDrawWindowConfig (&(pp->draw_area[0]), A_SCAN, 0);
                	draw_area_ (pp->vbox_area[0], 0 , 655, 425);
                	gtk_widget_show (pp->vbox_area[0]);
                }
			}
			break;
		case 2://Code
			DrawDisplayWindowFrame() ;
			break;
	}
	gtk_widget_queue_draw (pp->status_area) ;
}


void DrawDisplayWindowFrame()
{
	int i;
	/* 把之前的区域释放 */
	if(pp->vboxtable)
		gtk_widget_destroy(pp->vboxtable);
	pp->vboxtable = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vbox_draw_area), pp->vboxtable, FALSE, FALSE, 0);
	gtk_widget_show(pp->vboxtable);

	for (i = 0; i < 4; i ++)
	{
		pp->vbox_area[i] = gtk_vbox_new(FALSE, 0);
		pp->hbox_area[i] = gtk_hbox_new(FALSE, 0);
	}

//	if(get_probe_fft (pp->p_config))
	int group = get_current_group(pp->p_config);
	if(gData->fft[group].enableFft)
	{
		DrawFFT_WindowConfig() ;//draw_fft();
		return ;
	}

	if ((get_display_group(pp->p_config) == DISPLAY_CURRENT_GROUP) || (get_group_qty(pp->p_config)==1))
	{
		DrawCurrentGroupDisplayArea() ;
	}
	else
	{
		switch (get_group_qty(pp->p_config))
		{
			case 2:
				DrawTwoGroupDisplayArea() ;
				break;
			case 3:
				DrawThreeGroupDisplayArea() ;
				break;
			case 4:
				DrawFourGroupDisplayArea() ;
				break;
			case 5:
				DrawFiveGroupDisplayArea() ;
				break;
			case 6:
				DrawSixGroupDisplayArea() ;
				break;
			case 7:
				DrawSevenGroupDisplayArea() ;
				break;
			case 8:
				DrawEightGroupDisplayArea() ;
				break;
			default:break;
		}
	}


	//gtk_widget_queue_draw (pp->status_area) ;
}
/* 画波形数据显示区 */
void InitializeSscanMatrix()
{
	int k ;
	for (k = 0; ((k < 16) && (TMP(scan_type[k]) != 0xff)); k++)
	{
		RefreshDisplayMatrix(k)  ;
	}
}
//画 整个显示区域
void draw_area_all()
{
	if(pp->clb_flag && pp->ctype_pos == 1)
	{	// set black area configure
		SettingFrameBufferWindowConfigCalibration() ;
	}
	else //正常显示时
	{
		SettingFrameBufferWindowConfig() ;
	}
	//计算 S ， A 扫 FRAMEBUFFER 显示所需要的信息
	// calc matrix(mostly s scan matrix) needed by draw frame buffer
	InitializeSscanMatrix();
	// 画FB0上的显示区域（窗口，尺子，颜色条）
	if(pp->clb_flag && pp->ctype_pos == 1)
	{	// set black area configure
		DrawDisplayWindowFrameCalibration() ;
	}
	else
	{
		DrawDisplayWindowFrame() ;
	}
}

/* 三级菜单第一个 */
void draw3_data0(DRAW_UI_P p) 
{
	MENU_UNION menu_union = {{0 ,0 ,0 ,0}};
	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:/*Wizard -> Group -> Back P000 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 0;
					menu_union.pos.menu3 = 0;
					menu3FuncTrans(menu_union);
					break;
				case 1:/*Wizard -> Focal Law -> Back p010 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 0;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Wizard -> Calibration -> Back P020 */
					DrawMenu020();
					break;
				case 3:/*Wizard -> Weld -> Back p030 */
					DrawMenu030();
					break;
				case 4:
					DrawMenu040();
					break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0: /* 增益 Gain P100 */
					DrawMenu100();
					break;
				case 1: /* 发射 Pulser P110  TAN1 和 focal connection P一样的 */
					DrawMenu110();
					break;
				case 2: /* Reveiver 接收器  P120 TAN1 */
					DrawMenu120();
					break;
				case 3:/* Scan Offset P130 TAN1 */
					DrawMenu130();
					break;
				case 4:/* Set 80% P140 */
					DrawMenu140();
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/* Gate 选择调节哪个闸门 P200 TAN1 */
                    DrawMenu200();
					break;
				case 1:/* Alarm 选择哪个报警项 P210 TAN1 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 0;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Output  P220  */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 2;
					menu_union.pos.menu3 = 0;
					menu3FuncTrans(menu_union);
					break;
				case 3:/* Sizing Curves -> Setup  p230 */
					DrawMenu230();
					break;
				case 4:
					DrawMenu240();
					break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 0:/* Measurements -> Reading -> List P300 */
					DrawMenu300();
					break;
				case 1:/*Measurements -> Cursors -> Selection p310 */
					DrawMenu310();
					break;
				case 2:/*Measurements -> Table -> Display Table  p320 */
					DrawMenu320();
					break;
				case 3:/*Measurements -> Thickness -> Source  p330 */
					DrawMenu330();
					break;
				case 4:/*Measurements -> Export -> Export Table p340 */
					DrawMenu340();
					break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{ 
				case 0:/* 选择显示模式ABSC P400 */
					DrawMenu400();
					break;
				case 1:/*Display -> Overlay -> UT Unit  P410 */
					DrawMenu410();
					break;
				case 2:/*Display -> Color -> Select  p420 */
					DrawMenu420();
					break;
				case 3:/*Display -> Properties -> Scan P430 */
					DrawMenu430();
					break;
				case 4:
					DrawMenu440();
					break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/* 组设置 添加删除选择组 P500 */
					DrawMenu500();
					break;
				case 1:/* Scan Offset  P510 */
					DrawMenu510();
					break;
				case 2:/*Probe/Part -> characterize -> FFT p520 */
					DrawMenu520();
					break;
				case 3: /* 被检则工件的几何形状  P530 */
					DrawMenu530();
					break;
				case 4:
					 DrawMenu540();
					break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:/* 聚焦法则类型 线性扫查 角度(扇形)扫查深度扫查 静态扫查  P600 TAN1 */
					DrawMenu600();
					break;
				case 1:/* beam的最小角度 线性扫描就只有一个角度角度扫差时候需可设置多个角度  P610*/
					DrawMenu610();
					break;
				case 2:/* 聚焦点的计算方式 0halfpath 1truedepth 2projection 3focalplane 4automatic P620 */
					DrawMenu620();
					break;
				case 3:/* 聚焦 阵元数量 P630 */
					DrawMenu630();
					break;
				case 4:/* 自动计算聚焦法则 P640 */
					DrawMenu640();
					break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encoder -> Encoder p700 */
					DrawMenu700();
					break;
				case 1:/*Scan -> Inspection -> type  p710 */
					DrawMenu710();
					break;
				case 2:/*Scan -> Area -> scan start p720 *//* 当前步进 */
					DrawMenu720();
					break;
				case 3:/*Scan -> start -> start mode  p730 */
					DrawMenu730();
					break;
				case 4:/*Scan -> data -> storage  p740 */
					DrawMenu740();
					break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> Storage  p800 */
					DrawMenu800();
					break;
				case 1:/*File -> report -> template p810 */
					DrawMenu810();
					break;
				case 2:/*File -> format -> user field  p820 */
					DrawMenu820();
					break;
				case 3:/*File -> user field -> select  p830 */
					DrawMenu830();
					break;
				case 4:/*File -> notes-> edit notes  p840 */
					DrawMenu840();
					break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*Preferences -> pref.-> units p900 */
					DrawMenu900();
					break;
				case 1:/* Preferences -> system -> clock set 设置时间 p910 */
					DrawMenu910();
					break;
				case 2:/* 系统信息  P920*/
					DrawMenu920();
					break;
				case 3:/*Preferences -> network -> IP Address  p930*/
					DrawMenu930();
					break;
				case 4:/*Preferences -> options -> mouse  p940 */
					menu_union.pos.menu1 = 9;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 0;
					menu3FuncTrans(menu_union);
					break;
				default:break;
			}
			break;
		default:break;
	}
}


void draw3_data1(DRAW_UI_P p) 
{
	MENU_UNION menu_union = {{0 ,0 ,1 ,0}};
	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> start  p001 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 0;
					menu_union.pos.menu3 = 1;
					menu3FuncTrans(menu_union);
					break;
				case 1:/*Wizard -> Focal Law -> start p011 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 1;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Wizard -> Calibration -> start p021 */
					DrawMenu021()  ;
					break;
				case 3:/*Wizard -> Weld -> start p031 */
					DrawMenu031()  ;
					break;
				case 4:
					DrawMenu041()  ;
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* start 显示延时 P101 TAN1 等硬件 */
					DrawMenu101()  ;
					break;
				case 1: /* 收发模式 Tx/Rx Mode P111 TAN1 */
					DrawMenu111()  ;
					break;
				case 2: /* 接收滤波 P121 TAN1 */
					DrawMenu121()  ;
					break;
				case 3:/* Index Offset P131 TAN1 */
					DrawMenu131()  ;
					break;
				case 4:/*Set Ref P141*/
					DrawMenu141()  ;
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* 闸门调节 Mode还是Position  P201 TAN1 */
					DrawMenu201()  ;
					break;
				case 1:/* Group A 第一组报警 P211 TAN1 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 1;
					menu3FuncTrans(menu_union);
					break;
				case 2: /* Gate -> Output -> Alarm or Group #  P221 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 2;
					menu_union.pos.menu3 = 1;
					menu3FuncTrans(menu_union);
					break;
				case 3:/*Sizing Curves -> Curve  p231 */
					DrawMenu231()  ;
					break;
				case 4:
					DrawMenu241()  ;
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Group p301 */
					DrawMenu301()  ;
					break;
				case 1:/*Measurements -> Cursors -> Angle p311 */
					DrawMenu311()  ;
					break;
				case 2:/*Measurements -> Table -> Entry Image p321*/
					DrawMenu321();
					break;
				case 3:/*Measurements -> Thickness -> Min  p331 */
					DrawMenu331()  ;
					break;
				case 4:
					menu_union.pos.menu1 = 3;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 1;
					menu3FuncTrans(menu_union);
				//	DrawMenu341()  ;
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/* Ascan Sscan 时候是Group  P401 */
					DrawMenu401()  ;
					break;
				case 1:/* 窗口的栅格线 P411 */
					DrawMenu411()  ;
					break;
				case 2:/*Display -> Color -> start p421 */
					DrawMenu421();
					break;
				case 3:/* Ascan->Color  p431 */
					DrawMenu431();
					break;
				case 4:
					DrawMenu441();
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Group Mode  p501 */
					DrawMenu501();
					break;
				case 1:/* Index Offset P511 */
					DrawMenu511();
					break;
				case 2:/*probe/part -> characterize -> gain P521 */
					DrawMenu521();
					break;
				case 3:/*probe/part -> parts -> thickness p531 */
					DrawMenu531();
					break;
				case 4:
					DrawMenu541();
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0: /* 第一个发射阵元在连接器上面的位置 与UT设置的pulser一样 P601  TAN1 */
					DrawMenu601();
					break;
				case 1:/* 角度扫查的最大角度 出射角度 P611 */
					DrawMenu611();
					break;
				case 2:
					DrawMenu621();
					break;
				case 3:/* 聚焦法则的第一个阵元编号 P631 */
					DrawMenu631();
					break;
				case 4:/* 读取聚集法则 P641*/
					DrawMenu641();
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> polarity  p701 */
					DrawMenu701() ;
					break;
				case 1:/*Scan -> Inspection -> scan  p711 */
					DrawMenu711() ;
					break;
				case 2:/*Scan -> Area -> scan end  p721 */
					DrawMenu721();
					break;
				case 3:/*Scan -> start -> pause  p731 */
					DrawMenu731();
					break;
				case 4:/*Scan -> data -> inspec.data  p741 */
					DrawMenu741();
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/* 打开文件 P801 */
					DrawMenu801();
					break;
				case 1:/*File -> report -> file name p811 */
					DrawMenu811();
					break;
				case 2:/*File -> format -> probe  p821 */
					DrawMenu821();
					break;
				case 3:/*File -> user field -> enable  p831 */
					DrawMenu831();
					break;
				case 4:/*File -> notes-> edit header p841 */
					DrawMenu841();
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> bright p901 */
					DrawMenu901();
					break;
				case 1: /* Preferences -> system -> data set 设置日期  p911*/
					DrawMenu911();
					break;
				case 2:/*Preferences -> service -> File Manager  P921 */
					DrawMenu921();
					break;
				case 3:/*Preferences -> Network -> IP Address  p931*/
					DrawMenu931();
					break;
				case 4:/*Preferences -> options -> Option Key  p941 */
					menu_union.pos.menu1 = 9;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 1;
					menu3FuncTrans(menu_union);
					break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data2(DRAW_UI_P p) 
{
	MENU_UNION menu_union = {{0 ,0 ,2 ,0}};
	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:	/* p002 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 0;
					menu_union.pos.menu3 = 2;
					menu3FuncTrans(menu_union);
					break;
				case 1: /* p012 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 2;
					menu3FuncTrans(menu_union);
					break;
				case 2:/*Wizard -> Calibration -> Type  p022 */
					DrawMenu022();
					break;
				case 3:/*p032*/
					DrawMenu032();
					break;
				case 4:
					DrawMenu042();
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* range范围 P102 TAN1 */
					DrawMenu102();
					break;
				case 1: /* Freq频带(Mhz)  P112 TAN1 */
					DrawMenu112();
					break;
				case 2:/* Rectifier 检波 P122 */
					DrawMenu122();
					break;
				case 3:/* Angle (deg.) PA 当前Beam的角度 UT 当前探头的角度  P132 TAN1 */
					DrawMenu132();
					break;
				case 4:/* dB Ref 开关 P142 */
					DrawMenu142();
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* 闸门起点 或者 同步模式 P202 TAN1 */
					DrawMenu202();
					break;
				case 1:/* Condition GroupA P212 TAN1 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 2;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Output->Count or Data P222 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 2;
					menu_union.pos.menu3 = 2;
					menu3FuncTrans(menu_union);
					break;
				case 3:/* Gate/Alarm -> Sizing Curves-> ref.amplitude  p232 */
					DrawMenu232();
					break;
				case 4:
					DrawMenu242();
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/* Measurements -> Reading -> Field 1 P302 */
					DrawMenu302();
					break;
				case 1:/*Measurements -> Cursors -> Scan p312 */
					DrawMenu312();
					break;
				case 2:/*Measurements -> Table -> Add Entry p322 */
					DrawMenu322();
					break;
				case 3:/*Measurements -> Thickness -> Max p332 */
					DrawMenu332();
					break;
				case 4:
					menu_union.pos.menu1 = 3;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 2;
					menu3FuncTrans(menu_union);
				//	DrawMenu342();
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/* C-Scan2  P402 */
					DrawMenu402();
					break;
				case 1:/*Display -> Overlay -> sizing curves  p412 */
					DrawMenu412();
					break;
				case 2:/*Display -> Color -> end p422 */
					DrawMenu422();
					break;
				case 3:/* Display->Properties-> Ascan Envelope  Bscan optimum p432 */
					DrawMenu432();
					break;
				case 4:
					DrawMenu442();
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Select  p502 */
					DrawMenu502();
					break;
				case 1:/* skew angle P512 */
					DrawMenu512();
					break;
				case 2:/*Probe/Part -> Characterize -> start p522 */
					DrawMenu522();
					break;
				case 3:/*Probe/Part -> Parts -> diameter p532 */
					DrawMenu532();
					break; 
				case 4:
					DrawMenu542();
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0: /* 第一个接收阵元在连接器上面的位置 与UT设置的receiver一样 P602  TAN1 */
					DrawMenu602();
					break;
				case 1:/* 角度扫查的步进 P612 */
					DrawMenu612();
					break;
				case 2:/* 角度扫查的步进 P622 */
					DrawMenu622();
					break;
				case 3:/* 最后一个发射阵元 P632 */
					DrawMenu632();
					break;
				case 4:/* 保存聚焦法则 P642 */
					DrawMenu642();
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> type p702 */
                    DrawMenu702() ;
					break;
				case 1:/*Scan -> Inspection -> Index p712 */
					DrawMenu712();
					break;
				case 2:/*Scan -> Area -> scan resolution p722 */
					DrawMenu722();
					break;
				case 3:/*Scan -> start -> start  p732 */
					DrawMenu732();
					break;
				case 4:
					DrawMenu742();
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/* File -> File -> save setup as  p802 */
					DrawMenu802();
					break;
				case 1:/*File -> report -> paper size  p812 */
					DrawMenu812();
					break;
				case 2:/* File -> format -> setup  p822 */
					DrawMenu822();
					break;
				case 3:/*File -> user field -> label p832 */
					DrawMenu832();
					break;
				case 4:
					DrawMenu842();
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> scheme  p902 */
					DrawMenu902();
					break;
				case 1:/*Preferences -> system -> select key  p912 */
					DrawMenu912();
					break;
				case 2:/*Preferences -> service -> Import/Export p922 */
					DrawMenu922();
					break;
				case 3:/*Preferences -> Options -> Remote Desktop p932 */
					DrawMenu932();
					break;
				case 4:/*Preferences -> network -> Apply p942 */
					menu_union.pos.menu1 = 9;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 2;
					menu3FuncTrans(menu_union);
				//	DrawMenu942();
					break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data3(DRAW_UI_P p) 
{
	MENU_UNION menu_union = {{0 ,0 ,3 ,0}};
	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/* p003 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 0;
					menu_union.pos.menu3 = 3;
					menu3FuncTrans(menu_union);
					break;
				case 1:/*p013*/
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 3;
					menu3FuncTrans(menu_union);
					break;
				case 2:/*Wizard -> Calibration -> Mode  p023 */
					DrawMenu023();
					break;
				case 3:/* p033 */
					DrawMenu033();
					break;
				case 4:
					DrawMenu043();
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* wedge delay P103 TAN1 */
					DrawMenu103();
					break;
				case 1: /* 发射电压高低  P113 TAN1 */
					DrawMenu113();
					break;
				case 2: /* Video Filter 视频滤波 P123 TAN1 */
					DrawMenu123();
					break;
				case 3: /* Skew (deg.) UT 无 P133 TAN1 */
					DrawMenu133();
					break;
				case 4:/* Points Qty.  P143 TAN1 */
					DrawMenu143();
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/* 闸门宽度 或者 测量(Measure) P203 TAN1 */
					DrawMenu203();
					break;

				case 1:/*Operator  P213*/
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 3;
					menu3FuncTrans(menu_union);
					break;

				case 2:/* Sound  p223 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 2;
					menu_union.pos.menu3 = 3;
					menu3FuncTrans(menu_union);
					break;

				case 3:/* Gate/Alarm->Sizing Curves->Ref.Ampl.Offset  p233*/
					DrawMenu233();
					break;

				case 4:
					DrawMenu243();
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3]) 
			{
				case 0:/* Measurements -> reading -> field2  P303*/
					DrawMenu303();
					break;
				case 1:/*Measurements -> Cursors -> Index p313 */
					DrawMenu313();
					break;
				case 2:/*Measurements -> Table -> Delete Entry  p323 */
					DrawMenu323();
					break;
				case 3:/*Measurements -> Thickness -> Echo Qty.  p333*/
					DrawMenu333();
					break;
				case 4:
					menu_union.pos.menu1 = 3;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 3;
					menu3FuncTrans(menu_union);
			//		DrawMenu343();
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/*p403*/
					DrawMenu403();
					break;
				case 1:/*Display -> Overlay -> gate  p413 */
					DrawMenu413();
					break;
				case 2:/* Load color 调色板 P423 */
					DrawMenu423();
					break;
				case 3:/* A_SCAN -> Source  P433 */
					DrawMenu433();
					break;
				case 4:
					DrawMenu443();
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* Probe/Part -> Select -> Probe 探头选择  P503 */
					DrawMenu503();
					break;
				case 1:
					DrawMenu513();
					break;
				case 2:/*Probe/Part -> Characterize -> width p523 */
					DrawMenu523();
					break;
				case 3:/* 选择被测材料类型 P533 */
					DrawMenu533();
					break;
				case 4:
					DrawMenu543();
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:
					DrawMenu603();
					break;
				case 1:/* beam skew angle min 2D时候可以用 P613 */
					DrawMenu613();
					break;
				case 2:/* 聚焦深度 线扫   聚焦声程 角度扫查 P623 */
					DrawMenu623();
					break;
				case 3:/* 阵元步进 线扫时候用 P633 */
					DrawMenu633();
					break;
				case 4: /* 计算聚焦法则 P643 */
					DrawMenu643();
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> resolution  p703 */
					DrawMenu703();
					break;
				case 1:/* 扫查移动速度 P713 */
					DrawMenu713();
					break;
				case 2:/*Scan -> area -> index start p723 */
					DrawMenu723();
					break;
				case 3:
					DrawMenu733();
					break;
				case 4:
					DrawMenu743();
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:  /*File -> File -> save data p803 */
					DrawMenu803();
					break;/*File -> File -> save data p803 */
				case 1:/*File -> report -> build p813 */
					DrawMenu813();
					break;
				case 2:/*File -> format -> Note  p823 */
					DrawMenu823();
					break;
				case 3:/*File -> user field -> content  p833*/
					DrawMenu833();
					break;
				case 4:
					DrawMenu843();
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> scheme  p903 */
					DrawMenu903();
					break;
				case 1:/*Preferences -> system -> assign key  p913 */
					DrawMenu913();
					break;
				case 2:/*Preferences -> service -> Import/Export  p923 */
					DrawMenu923();
					break;
				case 3:/* 933 */
					DrawMenu933();
					break;
				case 4:/*  943 */
					menu_union.pos.menu1 = 9;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 3;
					menu3FuncTrans(menu_union);
				//	DrawMenu943();
					break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data4(DRAW_UI_P p) 
{
	MENU_UNION menu_union = {{0 ,0 ,4 ,0}};
	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0: /* p004 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 0;
					menu_union.pos.menu3 = 4;
					menu3FuncTrans(menu_union);
					break;
				case 1:	 /* p014 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 4;
					menu3FuncTrans(menu_union);
					break;
				case 2:/*p024*/
					DrawMenu024();
					break;
				case 3: /*p034*/
					DrawMenu034();
					break;
				case 4:
					DrawMenu044();
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* velocity 声速 P104 TAN1 */
					DrawMenu104();
					break;
				case 1: /* 脉冲宽度 pulser width  P114 TAN1 */
					DrawMenu114();
					break;
				case 2:/* Averaging  P124 TAN1 */
					DrawMenu124();
					break;
				case 3:/* Beam Delay 波束延时 UT无 P134 */
					DrawMenu134();
					break;
				case 4:/* Scale Factor P144 OK */
					DrawMenu144();
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* 闸门高度 或者 RF P204 TAN1 */
					DrawMenu204();
					break;
				case 1:/*Group B   p214 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 4;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Output->delay P224 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 2;
					menu_union.pos.menu3 = 4;
					menu3FuncTrans(menu_union);
					break;
				case 3:/*Gate/Alarm -> Sizing Curves -> Curve Step  p234 */
					DrawMenu234();
					break;
				case 4:
					DrawMenu244();
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 3 P304 */
					DrawMenu304();
					break;
				case 1:/*Measurements -> Cursors -> Add Entry p314 */
					DrawMenu314();
					break;
				case 2:/* Measurements -> Table -> Select Entry  P324*/
					DrawMenu324();
					break;
				case 3:
					DrawMenu334();
					break;
				case 4:
					menu_union.pos.menu1 = 3;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 4;
					menu3FuncTrans(menu_union);
				//	DrawMenu344();
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/* p404 */
					DrawMenu404();
					break;
				case 1:/*Display -> Overlay -> cursor  p414 */
					DrawMenu414();
					break;
				case 2:/*Display -> color -> Mode  p424 */
					DrawMenu424();
					break;
				case 3:/*Display -> properties -> appearence  p434 */
					DrawMenu434();
					break;
				case 4:
					DrawMenu444();
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* Probe/Part -> Select -> Wedge P504*/
					DrawMenu504();
					break;
				case 1:/* p514 */
					DrawMenu514();
					break;
				case 2:/*Probe/Part -> characterize -> procedure  p524 */
					DrawMenu524();
					break;
				case 3:/* p534 */
					DrawMenu534();
					break;
				case 4:
					DrawMenu544();
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:
					DrawMenu604();
					break;
				case 1: /* beam skew angle max 2D时候可以用 P614 */
					DrawMenu614();
					break;
				case 2:	/* P624 */
					DrawMenu624();
					break;
				case 3:	/* P634 */
					DrawMenu634();
					break;
				case 4:
					DrawMenu644();
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encodr -> Origin  p704 */
					DrawMenu704();
					break;
				case 1: /* Max.Scan Speed p714 */
					DrawMenu714();
					break;
				case 2:/*Scan -> area -> index end  p724 */
					DrawMenu724();
					break;
				case 3:
					DrawMenu734();
					break;
				case 4:
					DrawMenu744();
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/*File -> File -> save mode  p804 */
					DrawMenu804();
					break;
				case 1:
					DrawMenu814();
					break;
				case 2:/*File -> format -> view  p824 */
					DrawMenu824();
					break;
				case 3:
					DrawMenu834();
					break;
				case 4:
					DrawMenu844();
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> gate mode   p904 */
					DrawMenu904();
					break;
				case 1:
					DrawMenu914();
					break;
				case 2:/*  924*/
					DrawMenu924();
					break;
				case 3:
					DrawMenu934();
					break;
				case 4:/*Preferences -> Network -> Remote PC*/
					DrawMenu944();
					break;
				default:break;
			}
			break;
		default:break;
	}


}

void draw3_data5(DRAW_UI_P p) 
{

	MENU_UNION menu_union = {{0 ,0 ,5 ,0}};
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 0;
					menu_union.pos.menu3 = 5;
					menu3FuncTrans(menu_union);
					break;
				case 1: /* p015 */
					menu_union.pos.menu1 = 0;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 5;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Wizard -> Calibration -> clear calib p025*/
					DrawMenu025();
					break;
				case 3:
					DrawMenu035();
					break;
				case 4:
					DrawMenu045();
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* NULL 空 P105*/
					DrawMenu105();
					break;
				case 1: /* 重复频率 PRF P115 TAN1  */
					DrawMenu115();
					break;
				case 2:/* Reject 抑制 P125 TAN1 */
					DrawMenu125();
					break;
				case 3:/* Gain Offset UT 无 P135 */
					DrawMenu135();
					break;
				case 4:/* Sum Gain  P145 */
					DrawMenu145();
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/*NULL*/
					DrawMenu205();
					break;
				case 1:/*Condition   p215 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 1;
					menu_union.pos.menu3 = 5;
					menu3FuncTrans(menu_union);
					break;
				case 2:/* Hold Time P225 */
					menu_union.pos.menu1 = 2;
					menu_union.pos.menu2 = 2;
					menu_union.pos.menu3 = 5;
					menu3FuncTrans(menu_union);
					break;
				case 3:/* p235 */
					DrawMenu235();
					break;
				case 4:
					DrawMenu245();
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 4 p305 */
					DrawMenu305();
					break;
				case 1:/* p315 */
					DrawMenu315();
					break;
				case 2:/*Measurements -> Table -> Edit Comments p325 */
					DrawMenu325();
					break;
				case 3:
					DrawMenu335();
					break;
				case 4:
					menu_union.pos.menu1 = 3;
					menu_union.pos.menu2 = 4;
					menu_union.pos.menu3 = 5;
					menu3FuncTrans(menu_union);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:		/*  */
					DrawMenu405();
					break;
				case 1:/*Display -> Overlay -> overlay  p415 */
					DrawMenu415();
					break;
				case 2:
					DrawMenu425();
					break;
				case 3:/* Display -> properties -> overlay  p435 */
					DrawMenu435();
					break;
				case 4:
					DrawMenu445();
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Auto Detect  p505 */
					DrawMenu505();
					break;
				case 1:
					DrawMenu515();
					break;
				case 2:/*Probe/Part -> characterize -> procedure  p525 */
					DrawMenu525();
					break;
				case 3:
					DrawMenu535();
					break;
				case 4:
					DrawMenu545();
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:
					DrawMenu605();
					break;
				case 1:/* beam skew angle step P615 */
					DrawMenu615();
					break;
				case 2:
					DrawMenu625();
					break;
				case 3:
					DrawMenu635();
					break;
				case 4:
					DrawMenu645();
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encodr -> preset p705 */
					DrawMenu705();
					break;
				case 1: /* Max.Index  Speed  p715 */
					DrawMenu715();
					break;
				case 2:/*Scan -> area -> index resolution p725 */
					DrawMenu725();
					break;
				case 3:
					DrawMenu735();
					break;
				case 4:
					DrawMenu745();
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0: /*File -> File -> file name  p805 */
					DrawMenu805();
					break;
				case 1:
					DrawMenu815();
					break;
				case 2:
					DrawMenu825();
					break;
				case 3:
					DrawMenu835();
					break;
				case 4:
					DrawMenu845();
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:
					DrawMenu905();
					break;
				case 1:
					DrawMenu915();
					break;
				case 2:
					DrawMenu925();
					break;
				case 3:
					DrawMenu935();
					break;
				case 4:/*Preferences -> Network -> connect*/
					DrawMenu945();
					break;
				default:break;
			}
			break;
		default:break;
	}


}


gboolean bt_release (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	int j;
	int i = GPOINTER_TO_UINT(user_data);
	if( i == 26)//TAB键切换
	{
		pp->key_flag = !pp->key_flag;
		for (j=0; j<58; j++)
		{
			if(!pp->key_flag)
				gtk_button_set_label(GTK_BUTTON(pp->softkey_button[j]), s_keyboard_display[j]);
			else
				gtk_button_set_label(GTK_BUTTON(pp->softkey_button[j]), b_keyboard_display[j]);
		}
	}
	fakekey_press_keysym(pp->fk, keyboard_send[i], 0);
	fakekey_release(pp->fk);
	return TRUE;
}

gboolean numbt_release (GtkWidget *widget, GdkEventButton *event,
		gpointer user_data) 
{

	int i = GPOINTER_TO_UINT(user_data);
	fakekey_press_keysym(pp->fk, numkeyboard_send[i], 0);
	fakekey_release(pp->fk);
	if(i == 17)//点击enter收起软键盘
	{
		gtk_widget_destroy (pp->win_keyboard);
		pp->win_keyboard = NULL;
	}
	return TRUE;
}

void draw_keyboard (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	if (pp->win_keyboard) 
	{
		gtk_widget_destroy (pp->win_keyboard);
		pp->win_keyboard = NULL;
	}

	else
	{
		if ((GPOINTER_TO_UINT (data) == FULL_KEYBOARD)||(pp->pos == 5 && pp->pos1[5] == 0 && (pp->pos2[5][0]==3||pp->pos2[5][0]==4)))
			/*弹出全键盘*/
		{
			gint j;
			GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
			GtkWidget *hbox[5];

			pp->win_keyboard = gtk_window_new (GTK_WINDOW_POPUP);

			hbox[0] = gtk_hbox_new(FALSE, 5);
			for (j = 0 ; j < 13; j++ ) {
				if(!pp->key_flag)
					pp->softkey_button[j] = gtk_button_new_with_label (s_keyboard_display[j]);
				else
					pp->softkey_button[j] = gtk_button_new_with_label (b_keyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[0]), pp->softkey_button[j]);
				g_signal_connect(G_OBJECT(pp->softkey_button[j]), "button-release-event",
						G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[0]);

			hbox[1] = gtk_hbox_new(FALSE, 5);
			for (j = 13 ; j < 26; j++ ) {
				if(!pp->key_flag)
					pp->softkey_button[j] = gtk_button_new_with_label (s_keyboard_display[j]);
				else
					pp->softkey_button[j] = gtk_button_new_with_label (b_keyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[1]), pp->softkey_button[j]);
				g_signal_connect(G_OBJECT(pp->softkey_button[j]), "button-release-event",
						G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[1]);

			hbox[2] = gtk_hbox_new(FALSE, 5);
			for (j = 26 ; j < 39; j++ ) {
				if(!pp->key_flag)
					pp->softkey_button[j] = gtk_button_new_with_label (s_keyboard_display[j]);
				else
					pp->softkey_button[j] = gtk_button_new_with_label (b_keyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[2]), pp->softkey_button[j]);
				g_signal_connect(G_OBJECT(pp->softkey_button[j]), "button-release-event",
						G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[2]);

			hbox[3] = gtk_hbox_new(FALSE, 5);
			for (j = 39 ; j < 51; j++ ) {
				if(!pp->key_flag)
					pp->softkey_button[j] = gtk_button_new_with_label (s_keyboard_display[j]);
				else
					pp->softkey_button[j] = gtk_button_new_with_label (b_keyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[3]), pp->softkey_button[j]);
				g_signal_connect(G_OBJECT(pp->softkey_button[j]), "button-release-event",
						G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[3]);

			hbox[4] = gtk_hbox_new(FALSE, 5);
			for (j = 51 ; j < 58; j++ ) {
				if(!pp->key_flag)
					pp->softkey_button[j] = gtk_button_new_with_label (s_keyboard_display[j]);
				else
					pp->softkey_button[j] = gtk_button_new_with_label (b_keyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[4]), pp->softkey_button[j]);
				g_signal_connect(G_OBJECT(pp->softkey_button[j]), "button-release-event",
						G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[4]);

			gtk_container_add(GTK_CONTAINER(pp->win_keyboard), vbox);
			gtk_window_move (GTK_WINDOW (pp->win_keyboard), pp->keyboard_xpos,pp->keyboard_ypos);//215, 155); /* 设置窗口位置 */
			gtk_widget_set_size_request (GTK_WIDGET (pp->win_keyboard), 600, 250); /* 设置窗口大小 */
			gtk_widget_show_all(pp->win_keyboard);

		}

		else /*弹出数字键盘*/
		{
			gint j;
			GtkWidget *button[5];
			GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
			GtkWidget *hbox[5];

			pp->win_keyboard = gtk_window_new (GTK_WINDOW_POPUP);
			hbox[0] = gtk_hbox_new(TRUE, 5);
			for (j = 0 ; j < 4; j++ ) {
				button[0] = gtk_button_new_with_label (numkeyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[0]), button[0]);
				g_signal_connect(G_OBJECT(button[0]), "button-release-event",
						G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[0]);

			hbox[1] = gtk_hbox_new(TRUE, 5);
			for (j = 4 ; j < 8; j++ ) {
				button[1] = gtk_button_new_with_label (numkeyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[1]), button[1]);
				g_signal_connect(G_OBJECT(button[1]), "button-release-event",
						G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[1]);

			hbox[2] = gtk_hbox_new(TRUE, 5);
			for (j = 8 ; j < 12; j++ ) {
				button[2] = gtk_button_new_with_label (numkeyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[2]), button[2]);
				g_signal_connect(G_OBJECT(button[2]), "button-release-event",
						G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[2]);

			hbox[3] = gtk_hbox_new(TRUE, 5);
			for (j = 12 ; j < 15; j++ ) {
				button[3] = gtk_button_new_with_label (numkeyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[3]), button[3]);
				g_signal_connect(G_OBJECT(button[3]), "button-release-event",
						G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[3]);

			hbox[4] = gtk_hbox_new(TRUE, 5);
			for (j = 15 ; j < 18; j++ ) {
				button[4] = gtk_button_new_with_label (numkeyboard_display[j]);
				gtk_box_pack_start_defaults(GTK_BOX(hbox[4]), button[4]);
				g_signal_connect(G_OBJECT(button[4]), "button-release-event",
						G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
			}
			gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[4]);

			gtk_container_add(GTK_CONTAINER(pp->win_keyboard), vbox);
			gtk_window_move (GTK_WINDOW (pp->win_keyboard), 450, 155); /* 设置窗口位置 */
			gtk_widget_set_size_request (GTK_WIDGET (pp->win_keyboard), 200, 200); /* 设置窗口大小 */
			gtk_widget_show_all(pp->win_keyboard);
		}
	}
}


void change_language (guint lang, DRAW_UI_P p)
{
	int grp = get_current_group(pp->p_config);
	switch (lang)
	{ 
		case ENGLISH_:
			p->con0_p	= content_en10;
			p->con1_p	= content1_en;
			p->con2_p	= content2_en;

			p->units	= units_en;
			p->menu_content	= all_menu_content_en;

			p->list		= list_en;
			p->list1	= list1_en;

			if (GROUP_VAL_POS( grp , gate[GROUP_VAL_POS(grp , gate_pos)].measure) == 0)
			{
				p->field1	= field1_en;
				p->field	= field_en;
			}
			else
			{
				p->field1	= field2_en;
				p->field	= field_en_;
			}

			if(GROUP_VAL_POS(grp , ut_unit)==1)//Time
				p->field_unit	= field_unit_en;
			else//
				p->field_unit	= field_unit_en_mm;

			con0_p	= content_en10;
			con1_p	= content1_en;
			con2_p	= content2_en;

			units	= units_en;
			menu_content	= all_menu_content_en;

			list		= list_en;
			list1	= list1_en;
			if (GROUP_VAL_POS(grp , gate[GROUP_VAL_POS(grp , gate_pos)].measure) == 0)
			{
				field1	= field1_en;
				field	= field_en;
			}
			else
			{
				field1	= field2_en;
				field	= field_en_;
			}
			break;
		case CHINESE_:
			p->con0_p	= content_ch10;
			p->con1_p	= content1_ch;
			p->con2_p	= content2_ch;

			p->units	= units_ch;
			p->menu_content	= all_menu_content_ch;

			p->list		= list_ch;
			p->list1	= list1_ch;
			if (GROUP_VAL_POS( grp , gate[GROUP_VAL_POS(grp , gate_pos)].measure) == 0)
			{
				p->field1	= field1_ch;
				p->field	= field_ch;
			}
			else
			{
				p->field1	= field2_ch;
				p->field	= field_ch_;
			}
			if(GROUP_VAL_POS( 0 , ut_unit)==1)//Time
				p->field_unit	= field_unit_ch;
			else//
				p->field_unit	= field_unit_ch_mm;

			con0_p	= content_ch10;
			con1_p	= content1_ch;
			con2_p	= content2_ch;

			units	= units_ch;
			menu_content	= all_menu_content_ch;

			list	= list_ch;
			list1	= list1_ch;
			if (GROUP_VAL_POS(grp , gate[GROUP_VAL_POS(grp , gate_pos)].measure) == 0)
			{
				field1	= field1_ch;
				field	= field_ch;
			}
			else
			{
				field1	= field2_ch;
				field	= field_ch_;
			}
			break;
		default:break;
	}

	SYSTEM(language) = lang;
	g_debug("language:%s\n" ,menu_content[LANGUAGE + SYSTEM(language)]);
}

void draw_field_name ()
{
	char	*markup;
	/* 4个测量值名字显示 */
	markup = getFieldStringName( pp->p_config->field[0]) ;
	gtk_label_set_markup (GTK_LABEL(pp->label[8]),markup);
	g_free (markup);

	markup = getFieldStringName( pp->p_config->field[1]) ;
	gtk_label_set_markup (GTK_LABEL(pp->label[10]),markup);
	g_free (markup);

	markup = getFieldStringName( pp->p_config->field[2]) ;
	gtk_label_set_markup (GTK_LABEL(pp->label[12]),markup);
	g_free (markup);

	markup = getFieldStringName( pp->p_config->field[3]) ;
	gtk_label_set_markup (GTK_LABEL(pp->label[14]),markup);
	g_free (markup);
}

gboolean on_finish(gpointer p)
{
	gtk_label_set_markup (GTK_LABEL(pp->label[9]), p);
	return FALSE;
}


void init_box(DRAW_UI_P p)
{
	int i;
	p->pos_pos		= MENU3_STOP;
	p->menu2_qty	= 5;
	p->pos			= 1;
	p->pos1[0]		= 0;
	p->pos2[0][0]	= 0;

	p->vbox			= gtk_vbox_new(FALSE, 0);	
	p->hbox1		= gtk_hbox_new(FALSE, 0);	
	p->vbox11		= gtk_vbox_new(FALSE, 0);
	p->vbox12		= gtk_vbox_new(FALSE, 0);
	p->hbox121		= gtk_hbox_new(FALSE, 0);	
	p->hbox111		= gtk_hbox_new(FALSE, 0);	
	p->vbox1111[0]	= gtk_vbox_new(FALSE, 0);	
	p->vbox1111[1]  = gtk_vbox_new(FALSE, 0);	
	p->vbox1111[2]  = gtk_vbox_new(FALSE, 0);	
	p->hbox112		= gtk_hbox_new(FALSE, 0);	

	p->hbox2		= gtk_hbox_new(FALSE, 0);	
	p->vbox21		= gtk_vbox_new(FALSE, 0);	
	p->hbox211		= gtk_hbox_new(FALSE, 0);
	p->vbox_draw_area	= gtk_vbox_new(FALSE, 0);
	p->sw			= gtk_scrolled_window_new(NULL, NULL);
	p->hbox212		= gtk_hbox_new(FALSE, 0);	
	p->vbox22		= gtk_vbox_new(FALSE, 0);	
	for (i = 0; i < 6; i++)
		p->vbox221[i]	= gtk_vbox_new(FALSE, 0);

	for (i = 0; i < 20; i++) 
	{
		pp->label[i]	= gtk_label_new ("");
		gtk_widget_modify_fg(pp->label[i], GTK_STATE_NORMAL, &color_white);	/* 字体颜色白色 */
		pp->event[i]	= gtk_event_box_new();
		gtk_container_add(GTK_CONTAINER(p->event[i]), p->label[i]);
		gtk_container_set_border_width( GTK_CONTAINER(pp->event[i]), 0);     /*设置边框大小，这个地方使用图片*/
	}
}

/*初始化界面*/
void init_menu1(DRAW_UI_P p)
{
	int	i;
	int grp = get_current_group (pp->p_config) ;
	/* 一级菜单的初始化 */
	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content_en10[1]);
	gtk_widget_set_size_request(GTK_WIDGET(p->menuitem_main), 114, 83);        
	gtk_menu_bar_append(GTK_MENU_BAR(p->menubar), p->menuitem_main);
	for (i = 0; i < 10; i++)
	{
		p->menuitem[i]	= gtk_menu_item_new_with_label(content_en10[i]);
		gtk_widget_set_size_request(GTK_WIDGET(p->menuitem[i]), 120, 35);
		g_signal_connect(G_OBJECT(p->menuitem[i]), "activate", 
				G_CALLBACK(menuitem_function), GUINT_TO_POINTER (i));
		gtk_menu_shell_append(GTK_MENU_SHELL(p->menu), p->menuitem[i]);
		gtk_widget_show(p->menuitem[i]);
		if ((i == 6) && GROUP_VAL_POS(grp , group_mode) != PA_SCAN)
			gtk_widget_set_sensitive(pp->menuitem[i] ,FALSE);
	}
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(p->menuitem_main), p->menu);
	gtk_box_pack_start(GTK_BOX(p->hbox212), p->menubar, FALSE, FALSE, 0);

	g_signal_connect (pp->menu, "key-press-event", 
			G_CALLBACK(key_press_handler), (gpointer) (MAIN_MENU_PRESS));

	g_signal_connect (pp->menuitem_main, "button-press-event", 
			G_CALLBACK(main_menu_press), NULL);
	gtk_widget_show(p->menubar);
	gtk_widget_show(p->menu);
	gtk_widget_show(p->menuitem_main);
}

void init_menu2(DRAW_UI_P p)
{
	gint	i;
	/* 二级菜单的初始化 */
	for (i = 0; i < 5; i++) 
	{
		p->eventbox2[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox2[i]), 113, 85);
		p->label2[i] = gtk_label_new("");
		gtk_container_add(GTK_CONTAINER(p->eventbox2[i]), p->label2[i]);
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox2[i]), 0);     
		update_widget_bg(p->eventbox2[i], /*backpic[1]*/ 1);
		gtk_widget_modify_fg(p->label2[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox2[i]), "button-press-event", 
				G_CALLBACK(eventbox2_function0), GUINT_TO_POINTER (i));

		gtk_box_pack_start(GTK_BOX(p->hbox212), p->eventbox2[i], FALSE, FALSE, 0);
		gtk_widget_show(p->eventbox2[i]);
		gtk_widget_show(p->label2[i]);
	}
}

void init_menu3(DRAW_UI_P p)
{
	int	i;
	/*三级菜单的初始化*/
	for (i = 0; i < 6; i++)
	{
		p->eventbox30[i] = gtk_event_box_new();
		p->eventbox31[i] = gtk_event_box_new();
		p->label3[i]     = gtk_label_new("");
		p->data3[i]      = gtk_label_new("");
		pp->adj = (GtkAdjustment *) gtk_adjustment_new (10.0, 0.0, 74.0, 0.1, 10.0, 0.0);
		pp->sbutton[i] = gtk_spin_button_new (pp->adj, 0, 1);
		gtk_spin_button_set_numeric((GtkSpinButton*)pp->sbutton[i], 1);

		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox30[i]), 114, 60);
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox31[i]), 114, 25);       
		gtk_widget_set_size_request(GTK_WIDGET(p->sbutton[i]), 114, 25);
		update_widget_bg(pp->eventbox30[i], /*backpic[7]*/ 7);
		update_widget_bg(pp->eventbox31[i], /*backpic[10]*/ 10);
		gtk_widget_modify_base(GTK_WIDGET (pp->sbutton[i]), GTK_STATE_NORMAL, &color_text_base);
		gtk_widget_modify_text(GTK_WIDGET (pp->sbutton[i]), GTK_STATE_NORMAL, &color_yellow);

		gtk_container_set_border_width(GTK_CONTAINER(p->eventbox30[i]), 0);   
		gtk_container_set_border_width(GTK_CONTAINER(p->eventbox31[i]), 0);
		gtk_container_add(GTK_CONTAINER(p->eventbox30[i]), p->label3[i]);
		gtk_container_add(GTK_CONTAINER(p->eventbox31[i]), p->data3[i]);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox30[i], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox31[i], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->sbutton[i], FALSE, FALSE, 0);
		gtk_widget_modify_fg(p->label3[i], GTK_STATE_NORMAL, &color_white);
		gtk_widget_modify_fg(p->data3[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox30[i]), "button-press-event", 
				G_CALLBACK(data_function0), (GUINT_TO_POINTER (i)));
		g_signal_connect(G_OBJECT(p->eventbox31[i]), "button-press-event", 
				G_CALLBACK(data_function0), (GUINT_TO_POINTER (i)));
		gtk_widget_show(p->eventbox30[i]);
		gtk_widget_show(p->eventbox31[i]);
		gtk_widget_show(p->label3[i]);
	}
}


void init_ui(DRAW_UI_P p)				
{
	int	i;
	char	*markup;
	p_drawui_c = p;
	all_bg_pic_in_mem();
	change_language (get_language(pp->p_config), p);//初始化语言 language init
	init_box(p);
	init_menu1(p);
	init_menu2(p);
	init_menu3(p);
	/* 3级菜单的显示 */
	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}
	/* 三级菜单 弹出的初始化 */
	pp->menu3 = gtk_menu_new();
	for ( i = 0; i < 6; i++)
	{
		pp->menu33[i] = gtk_menu_new();
		g_signal_connect (pp->menu33[i], "key-press-event", 
				G_CALLBACK(key_press_handler) ,(gpointer)(i + 2));
	}
	for ( i = 0; i < 64; i++)
	pp->menu_item3[i] = NULL;
	/* 各box的包含关系*/
	gtk_container_add(GTK_CONTAINER(p->window), p->vbox);
	gtk_widget_show(p->vbox);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox1, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox1);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox2, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox2);
	/* 上方数据显示  */
	gtk_box_pack_start (GTK_BOX (p->hbox1), pp->vbox11,   FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox1), pp->vbox12,   FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox111, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox112, FALSE, FALSE, 0);
	/*软键盘显示*/
	pp->drawing_area = gtk_drawing_area_new();
	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->drawing_area, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->drawing_area), 115, 40);
	g_signal_connect (G_OBJECT (pp->drawing_area), "expose_event",
			G_CALLBACK (draw_other_info), NULL);		/* 电池信息 */

	pp->status_area = gtk_drawing_area_new();
	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->status_area, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->status_area), 115, 25);
	gtk_widget_modify_bg(pp->status_area, GTK_STATE_NORMAL, &color_black);
	g_signal_connect (G_OBJECT (pp->status_area), "expose_event",
			G_CALLBACK (draw_status_info), NULL);


	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->hbox121, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[17], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[18], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[19], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[17]), 62 , 25 );
	update_widget_bg(pp->event[17], /*backpic[12]*/ 12); 
	g_signal_connect (G_OBJECT (pp->event[17]), "button-press-event", 
			G_CALLBACK (draw_keyboard), NULL);		/* 虚拟键盘 */

	gtk_widget_set_size_request (GTK_WIDGET(pp->event[18]), 26, 25);

	if(!get_inspec_source (pp->p_config))
		update_widget_bg(pp->event[18], /*backpic[12]*/ 18);
	else
		update_widget_bg(pp->event[18], /*backpic[12]*/ 19);

	gtk_widget_set_size_request (GTK_WIDGET(pp->event[19]), 26, 25);
	update_widget_bg(pp->event[19], /*backpic[12]*/ 16);
	/* 增益显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[0], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[0]), 60, 45);
	update_widget_bg(pp->event[0], /*backpic[3]*/ 3);
	//****************
	if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	else
		tt_label_show_string (pp->label[0], con2_p[1][0][0], "\n", "(dB)", "white", 10);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[1], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[1]), 112, 45);  
	update_widget_bg(pp->event[1], /*backpic[4]*/ 4);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>", 
            group_get_gain(get_current_group(pp->p_config)) / 100.0);
	gtk_label_set_markup (GTK_LABEL (pp->label[1]), markup);      /* 增益数值 */
	g_free (markup);
	/* 小状态栏  */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[2], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[2], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[2]), 172, 22);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>", "default.cfg");
	gtk_label_set_markup (GTK_LABEL (pp->label[2]), markup); 
	g_free (markup);
	update_widget_bg(pp->event[2], /*backpic[5]*/ 5);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[3], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[3]), 172, 22);
	gtk_label_set_justify (GTK_LABEL (pp->label[3]), PANGO_ELLIPSIZE_START);
	markup=g_markup_printf_escaped(
		"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>", GROUP_VAL_POS (0, prf1) / 10, (GROUP_VAL_POS(0 , prf1) * get_beam_qty() / 10) * 1);
	gtk_label_set_markup (GTK_LABEL (pp->label[3]), markup); 
	g_free (markup);
	update_widget_bg(pp->event[3], /*backpic[5]*/ 5);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[4], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[4]), 172, 22);
	time_handler1(NULL);
	update_widget_bg(pp->event[4], /*backpic[5]*/ 5);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[5], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[5]), 172, 22);
	update_widget_bg(pp->event[5], /*backpic[5]*/ 5);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[6], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[6]), 172, 22);

    gchar *devVersion = NULL;
    gsize len;
    g_file_get_contents("/etc/version", &devVersion, &len, NULL);
    if (len > 0) {
        devVersion[len-1] = 0;
    }
    gchar *version = g_strdup_printf("DPL PA %s  %s", devVersion, g_pVersion);
    markup = g_markup_printf_escaped ("<span foreground='red' font_desc='10'>%s</span>", version);
    g_free(version);
    g_free(devVersion);

	gtk_label_set_markup (GTK_LABEL (pp->label[6]), markup); 
	g_free (markup);
#ifdef X86
	g_signal_connect(G_OBJECT(pp->event[6]), "button-press-event",
			G_CALLBACK(ExitApplication), 0);
#endif
	update_widget_bg(pp->event[6], /*backpic[5]*/ 5);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",
			(double)(GROUP_VAL_POS(0 , prf1) * pp->p_config->inspection_scan_resolution / 10000));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;
	g_free (markup);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[7], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[7]), 172, 22);
	update_widget_bg(pp->event[7], /*backpic[5]*/ 5);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f   </span>",0.0);
	gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 
	g_free (markup);
	/* 4个测量值显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[8], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>%%A\n(%%)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[8]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[8]), 60, 45);
	update_widget_bg(pp->event[8], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[9], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[9]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[9]), 112, 45);
	update_widget_bg(pp->event[9], /*backpic[4]*/ 4);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[10], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>DA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[10]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[10]), 60, 34);
	update_widget_bg(pp->event[10], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[11], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[11]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[11]), 112, 34);
	update_widget_bg(pp->event[11], /*backpic[4]*/ 4);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[12], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>PA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[12]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[12]), 60, 34);
	update_widget_bg(pp->event[12], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[13], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[13]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[13]), 112, 34);
	update_widget_bg(pp->event[13], /*backpic[4]*/ 4);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[14], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>SA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[14]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[14]), 60, 34);
	update_widget_bg(pp->event[14], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[15], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[15]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[15]), 112, 34);
	update_widget_bg(pp->event[15], /*backpic[4]*/ 4);
	draw_field_name ();
	gtk_widget_show_all (pp->hbox1);   /* 上方显示信息  */
	/* 画图区域及各button */
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox21, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX (p->hbox2), p->vbox22, FALSE, FALSE, 2);
	gtk_widget_show(p->vbox21);
	gtk_widget_show(p->vbox22);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox211, FALSE, FALSE, 0);
	gtk_widget_set_size_request(p->sw,685,425);	/* 显示帮助文档 */
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->sw, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);
	/*帮助文档*/
	pp->web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());	
	webkit_web_view_set_custom_encoding (pp->web_view, "UTF-8");
	gtk_container_add(GTK_CONTAINER (pp->sw), GTK_WIDGET (pp->web_view));
	memset(pp->file_path,0,128);
	strcpy(pp->file_path, HELP_UT_SETTING_PATH);
	webkit_web_view_load_uri (pp->web_view, pp->file_path);
	gtk_box_pack_start (GTK_BOX (p->hbox211), p->vbox_draw_area, TRUE, TRUE, 0);
	gtk_widget_show(p->vbox_draw_area);
	draw_area_all ();
	gtk_widget_show_all (p->hbox2); /* 画图区域 及 button 的显示 */
	widget_window_class = GTK_WIDGET_GET_CLASS ((GtkObject*)(pp->window)); 
	window_keypress_event_orig = widget_window_class->key_press_event; 
	pp->dialog = gtk_dialog_new_with_buttons("TanDenghua", GTK_WINDOW (pp->window),
			GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	dialog_window_class = GTK_WIDGET_GET_CLASS ((GtkObject*)(pp->dialog)); 
	dialog_keypress_event_orig = dialog_window_class->key_press_event; 
	gtk_window_set_decorated (GTK_WINDOW (pp->dialog), FALSE);			/*不可以装饰*/
	gtk_container_set_border_width( GTK_CONTAINER (gtk_dialog_get_content_area(GTK_DIALOG(pp->dialog))), 0);     /* */
	gtk_widget_show (gtk_dialog_get_content_area(GTK_DIALOG(pp->dialog)));
	gtk_widget_hide (gtk_dialog_get_action_area(GTK_DIALOG(pp->dialog)));
	/* scale 上面的透明条 */
	pp->scale_drawarea = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(pp->scale_drawarea), 30, 425);
	gtk_widget_modify_bg(pp->scale_drawarea, GTK_STATE_NORMAL, &color_black);
	/* scale 快速调节数值 */
	pp->button_add = gtk_button_new_with_label ("+");    /* 加减数值这里功能数值加减会出现不准的时候 */
	g_signal_connect (pp->button_add, "clicked", 
			G_CALLBACK(add_click), NULL);
	pp->button_sub = gtk_button_new_with_label ("-");    
	g_signal_connect (pp->button_sub, "clicked", 
			G_CALLBACK(sub_click), NULL);
	pp->vscale = gtk_vscale_new_with_range(1.0, 100.0, 1.0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscale), 30, 366);
	/* vscalebox */
	pp->vscalebox = gtk_vbox_new (FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscalebox), 30, 425);
	gtk_widget_show (pp->vscalebox);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->scale_drawarea, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->button_add, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->vscale, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->button_sub, FALSE, FALSE, 0);
	gtk_widget_show (pp->scale_drawarea); 
	gtk_widget_show (pp->button_add);
	gtk_widget_show (pp->button_sub);
	gtk_widget_show (pp->vscale);
	gtk_widget_show (pp->vscalebox);
	gtk_widget_hide (pp->sw);
	gtk_widget_set_can_focus (pp->button_add, FALSE);
	gtk_widget_set_can_focus (pp->vscale, FALSE);
	gtk_widget_set_can_focus (pp->button_sub, FALSE);
	gtk_box_pack_start (GTK_BOX (p->hbox211), pp->vscalebox, FALSE, FALSE, 0);
	change_keypress_event (KEYPRESS_MAIN);
	draw_menu1 ();
	draw_menu2 (1);
	draw_menu3 (1, NULL);
	if (!g_thread_supported()) 
	{
		g_thread_init (NULL);
	}
	pp->mark3 = 1;
	pp->mark2 = 1;
	pp->scan_count = 1;

#if ARM
	DMA_MARK = (int*) (pp->p_beam_data + CONFIG_OFFSET);
	*DMA_MARK = 0 ;
	ScanDataMark = (unsigned char*)(pp->p_beam_data + SCAN_DATA_MARK_OFFSET) ;
	g_timeout_add (5000, (GSourceFunc) key_read_delay, NULL);
#endif
	g_timeout_add (1000, (GSourceFunc) time_handler1, NULL);
    int ret;
	pthread_mutex_init(&draw_thread_mutex , NULL);
	pthread_t tid2 ;
	ret = pthread_create (&tid2, NULL, (void*)signal_scan_thread, NULL);
	if(ret){
		perror("in1:");
		return;
	}
	pthread_t tid1 ;
	ret = pthread_create (&tid1 , NULL, (void*)RefressFocalLaw , NULL) ;
	if(ret){
		perror("in1:");
		return;
	}

    pthread_t thid3 ;
	ret = pthread_create (&thid3, NULL, (void*)DopplerPhyscanTcpServer, NULL);
	if(ret){
			perror("in1:");
	}

	menuLabelInit();
	tableInit();

    /* report preview */
    report_preview_set_tmpl(DEFAULT_REPORT_TEMPLATE);

    TMP(gateShow)[0] = 0xff;
    TMP(gateShow)[1] = 0;
    TMP(gateShow)[2] = 0;
    TMP(gateShow)[3] = 0;
}

ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timout)
{

	int                         nfds;
	fd_set                   readfds;
	struct timeval  tv;

	tv.tv_sec  = timout;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	nfds = select(fd+1, &readfds, NULL, NULL, &tv);
	if (nfds <= 0) 
	{
		if (nfds == 0)
			errno = ETIME;
		return(-1);
	}
	else
		return(read(fd, buf, nbytes));
}

ssize_t treadn(int fd, void *buf, size_t nbytes, unsigned int timout)
{
	size_t      nleft;
	ssize_t     nread;

	nleft = nbytes;
	while (nleft > 0) 
	{
		if ((nread = tread(fd, buf, nleft, timout)) < 0) 
		{
			if (nleft == nbytes)
				return(-1); /* error, return -1 */
			else
				break;      /* error, return amount read so far */
		} 
		else if (nread == 0) 
		{
			break;          /* EOF */
		}
		nleft -= nread;
		buf += nread;
	}
	return(nbytes - nleft);      /* return >= 0 */
}



double ColorTable[10] = {0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.4 , 0.3 , 0.2 , 0.1 , 0} ;

typedef struct _SAVE_DATA_DIALOG_DATA
{
	GtkWidget* Win;
	GtkWidget* DrawArea;
	int DrawStatus ;
}SAVE_DATA_DIALOG_DATA , *SAVE_DATA_DIALOG_DATA_P  ;


SAVE_DATA_DIALOG_DATA  DataSaveInfor ;

int DrawDynamicBar (GtkWidget* widget, GdkEventExpose *event, gpointer data)
{
    SAVE_DATA_DIALOG_DATA_P pData = (SAVE_DATA_DIALOG_DATA_P)data ;
    cairo_t *cr;
    cr = gdk_cairo_create(gtk_widget_get_window(widget));

    int width , height ;
    int _nPosition ;
    width = event->area.width ;
    height= event->area.height;
    int i ;
    double _nColor  ;
    _nPosition = pData->DrawStatus  % 10 ;

    for(i = 0 ; i < 10 ; i++)
    {
	    _nColor = ColorTable[(i + _nPosition)%10] ;
        cairo_set_source_rgba(cr,  0, _nColor, 1, 1);
        cairo_rectangle(cr , i * 30 , 0 , (i + 1) * 30 , height);
	    cairo_fill ( cr) ;
	    cairo_stroke(cr);
    }
    return TRUE;
}

int RefreshSaveingDialog(gpointer data)
{
	//printf("RefreshSaveingDialog\n");
	SAVE_DATA_DIALOG_DATA_P pData = (SAVE_DATA_DIALOG_DATA_P)data ;
	if(!pp->bSaveDataProcessing)
	{
	    gtk_widget_destroy((GtkWidget*)pData->Win);
	    memset(&DataSaveInfor , 0 , sizeof(SAVE_DATA_DIALOG_DATA)) ;

	    return 0;
	}
	pData->DrawStatus++ ;
	gtk_widget_queue_draw (pData->DrawArea) ;

	return TRUE ;
}

void SaveDataProcess(char* strTitle)
{
	if(pp->bSaveDataProcessing) return ;
	//printf("SaveDataProcess\n");
	int _nFileSaveModel = get_file_save_mode(pp->p_config);
	if(_nFileSaveModel == SAVE_MODE_SCREEN)
	{
		pp->bSaveDataProcessing = TRUE ;
		return ;
	}
	if(DataSaveInfor.Win || DataSaveInfor.DrawArea)
	{
		return ;
	}
	pp->bSaveDataProcessing = TRUE ;
	GtkWidget* Win = gtk_window_new(GTK_WINDOW_POPUP) ;
	GtkWidget* _vBox  = gtk_vbox_new( 0  , 0 );
	GtkWidget* DrawArea = gtk_drawing_area_new();
	GtkWidget* Label  = gtk_label_new(strTitle);

	DataSaveInfor.Win = Win ;
	DataSaveInfor.DrawArea = DrawArea ;
	DataSaveInfor.DrawStatus = 0 ;

	gtk_window_set_decorated (GTK_WINDOW (Win), FALSE);

	gtk_widget_set_size_request(Win , 300 , 60) ;
	gtk_window_set_position (GTK_WINDOW (Win),GTK_WIN_POS_CENTER);

	gtk_widget_set_size_request(DrawArea, 300, 40) ;
        g_signal_connect (G_OBJECT (DrawArea), "expose-event",
			G_CALLBACK (DrawDynamicBar), (gpointer)(&DataSaveInfor));
	gtk_box_pack_start(GTK_BOX(_vBox) , DrawArea, 0 , 0 , 0);
	gtk_box_pack_start(GTK_BOX(_vBox) , Label , 0 , 0 , 0);

	gtk_container_add(GTK_CONTAINER(Win) , _vBox);
	gtk_window_set_transient_for (GTK_WINDOW (Win), GTK_WINDOW (pp->window));
	gtk_widget_show_all(Win);

	gtk_window_set_modal(GTK_WINDOW(Win) , TRUE);

	g_timeout_add(100 , RefreshSaveingDialog , &DataSaveInfor);
}


void setKeyInvalidateWhenDataRecalling(gboolean enable)
{
    if(enable) {
		TMP(dataRecalling) = 0;
		gtk_widget_set_sensitive(pp->menuitem[0],TRUE);
		gtk_widget_set_sensitive(pp->menuitem[1],TRUE);
		gtk_widget_set_sensitive(pp->menuitem[2],TRUE);
		gtk_widget_set_sensitive(pp->menuitem[5],TRUE);
		gtk_widget_set_sensitive(pp->menuitem[7],TRUE);
        if (GROUP_VAL_POS(get_current_group(pp->p_config) , group_mode) == PA_SCAN) {
            /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
			gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
        } else {
			gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
		}
    } else {
		TMP(dataRecalling) = 1;
		gtk_widget_set_sensitive(pp->menuitem[0],FALSE);
		gtk_widget_set_sensitive(pp->menuitem[1],FALSE);
		gtk_widget_set_sensitive(pp->menuitem[2],FALSE);
		gtk_widget_set_sensitive(pp->menuitem[5],FALSE);
        gtk_widget_set_sensitive(pp->menuitem[7],FALSE);
        if (GROUP_VAL_POS(get_current_group(pp->p_config) , group_mode) != PA_SCAN) {
            /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
            gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
        }
	}
}

