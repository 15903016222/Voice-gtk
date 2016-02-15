/*
 * keyboard.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <fakekey/fakekey.h>
#include <gdk/gdkkeysyms.h>
#include <X11/keysym.h>
static Display* display = NULL;
static FakeKey* fakekey = NULL;
//static int capslock = FALSE;
#define capslock pp->key_flag

#include "../drawui.h"
static const char* upper_letter[] =
{
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",	"-", "=", "BackSpace",
	"Tab", "Q", "W", "E", "R", "T",	"Y", "U", "I", "O", "P", "[", "]",
	"Caps Lock","A", "S",	"D", "F", "G","H", "J", "K", "L", ";","'","Enter",
	"Shift", "Z", "X", "C", "V", "B","N", "M", ",",	".", "/", "↑","Del",
	"Ctrl" ,"Alt",	"Space","Alt","Ctrl", "←", "↓", "→"
};

static const char* lower_letter[] =
{
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0","-", "=", "BackSpace",
	"Tab", "q", "w", "e", "r", "t",	"y", "u", "i", "o", "p", "[", "]",
	"Caps Lock","a", "s",	"d", "f", "g","h", "j", "k", "l", ";","'","Enter",
	"Shift", "z", "x", "c", "v", "b","n", "m", ",",".",  "/", "↑","Del",
	"Ctrl","Alt",	"Space","Alt","Ctrl", "←", "↓", "→"
};//*/

static const gushort key_code[] =
{
	XK_1, XK_2, XK_3, XK_4, XK_5, XK_6,XK_7, XK_8, XK_9, XK_0, XK_minus,	XK_equal, XK_BackSpace,
	XK_Tab, XK_q, XK_w, XK_e, XK_r, XK_t, XK_y,	XK_u, XK_i, XK_o, XK_p, XK_bracketleft, XK_bracketright,
	XK_Caps_Lock,	XK_a, XK_s, XK_d,XK_f, XK_g, XK_h, XK_j, XK_k, XK_l,XK_semicolon,	XK_apostrophe,XK_Return,
	XK_Shift_L, XK_z, XK_x, XK_c, XK_v, XK_b,XK_n,	XK_m, XK_comma, XK_period, XK_slash, XK_Up,XK_Delete,
	XK_Control_L, XK_Alt_L,	XK_space,XK_Alt_R,XK_Control_R,XK_Left, XK_Down, XK_Right
};

void keyboardInit()
{
	display = XOpenDisplay(NULL);
	if (display)
		fakekey= fakekey_init(display);
}

void buttonForeach(gpointer data,gpointer user_data)
{
	if(GTK_IS_BUTTON(data))
	{
		int i = (int)g_object_get_data(G_OBJECT(data) ,"button");
		if(capslock)
			gtk_button_set_label(GTK_BUTTON(data), upper_letter[i]);
		else
			gtk_button_set_label(GTK_BUTTON(data), lower_letter[i]);
	}
}

gboolean buttonCallback(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	int i = (int)g_object_get_data(G_OBJECT(widget) ,"button");
	fakekey_press_keysym(pp->fk, key_code[i], 0);
	fakekey_release(pp->fk);

	if( i == 26)//CAPSLOCK键切换
	{
		capslock = !capslock;
		GtkWidget* table = (GtkWidget*)user_data;
		if(GTK_IS_TABLE(table))
		{
			GList* list = gtk_container_get_children(GTK_CONTAINER(table));
			g_list_foreach(list ,buttonForeach ,NULL);
		}//*/
	}
	return TRUE;
}

GtkWidget* keyBoardNew()
{
	GtkWidget* vbox;
	GtkWidget* button[60];

	vbox = gtk_vbox_new(TRUE ,0);

	GtkWidget* table;
	table = gtk_table_new(5 ,28 ,TRUE);
	gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,0);

	int i;
	for(i = 0 ;i < 60 ;++i)
	{
		if(capslock)
			button[i] = gtk_button_new_with_label (upper_letter[i]);
		else
			button[i] = gtk_button_new_with_label (lower_letter[i]);
		gtk_button_set_focus_on_click(GTK_BUTTON(button[i]),FALSE);
		g_signal_connect(G_OBJECT(button[i]), "button-release-event",
				G_CALLBACK(buttonCallback), table);
		g_object_set_data(G_OBJECT(button[i]) ,"button" ,GUINT_TO_POINTER (i));
	}
//row 1
	for(i=0;i<12;i++)
	{
		gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[i]) ,2 * i ,2 * (i+ 1) ,0 ,1);
	}
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[12]) ,24 ,28 ,0 ,1);
//row 2
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[13]) ,0 ,4 ,1 ,2);
	for(i=14;i<26;i++)
	{
		gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[i]) ,2 * (i - 14) + 4 ,2 * (i - 14) + 6 ,1 ,2);
	}
//row 3
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[26]) ,0 ,4 ,2 ,3);
	for(i=27;i<38;i++)
	{
		gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[i]) ,2 * (i - 25) ,2 * (i - 24) ,2 ,3);
	}
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[38]) ,26 ,28 ,2 ,3);
//row 4
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[39]) ,0 ,4 ,3 ,4);
	for(i=40;i<51;i++)
	{
		gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[i]) ,2 * (i - 40) + 4 ,2 * (i - 40) + 6 ,3 ,4);
	}
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[51]) ,26 ,28 ,3 ,4);
//row 5
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[52]) ,0 ,3 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[53]) ,3 ,6 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[54]) ,6 ,16 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[55]) ,16 ,19 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[56]) ,19 ,22 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[57]) ,22 ,24 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[58]) ,24 ,26 ,4 ,5);
	gtk_table_attach_defaults(GTK_TABLE(table) ,GTK_WIDGET(button[59]) ,26 ,28 ,4 ,5);

	return vbox;
}
