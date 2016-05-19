/*
 * ui.h
 *
 *  Created on: 2012-5-3
 *
 */

#ifndef UI_H_
#define UI_H_

#include <gtk/gtk.h>
//module filelistScroll
#include "dialog/filelist.h"
#include <syslog.h>

#include "../uihandler/menu/menuDefine.h"
#include "menu/menu.h"
//module labelbutton
#include "labelbutton.h"

//module keyboard
void keyboardInit();
GtkWidget* keyBoardNew();

//module dialog
GtkWidget* dialogNew(GtkWidget* fatherWidget);
int   dialogWarning(GtkWidget* fatherWidget ,const char* warningString);
int   dialogFileConfirm(GtkWidget* dialog ,const char* filename ,const char* path);
char* dialogGetAString(GtkWidget* fatherWidget ,const char* title ,const char* initStr);
void  dialogError(GtkWidget* fatherWidget ,const char* warningString) ;
void dialogMessage(GtkWidget* fatherWidget ,const char* msg , GdkColor* pBgColor_) ;
void dialogMessagePix(GtkWidget* fatherWidget ,const char* msg , GdkColor* pBgColor_ , int pixels);
void showHelp(MENU_UNION menu_union ,int enable) ;
typedef enum
{
	RESPONSE_LABELBUTTON1,
	RESPONSE_LABELBUTTON2,
	RESPONSE_LABELBUTTON3,
	RESPONSE_LABELBUTTON4,
	RESPONSE_LABELBUTTON5,
	RESPONSE_LABELBUTTON6,
	RESPONSE_LABELBUTTON7,
	RESPONSE_LABELBUTTON8,
	RESPONSE_LABELBUTTON9,
	RESPONSE_LABELBUTTON10,
	RESPONSE_LABELBUTTON11,
	RESPONSE_LABELBUTTON12,
	RESPONSE_LABELBUTTON_MAX,
}
	RESPONSE_LABELBUTTON_ENUM;
gboolean keyHandler_dialog(GtkWidget* widget,	GdkEventKey* event, gpointer data);


static inline void ui_show_msg_dialog(GtkWindow *w, const gchar *msg, GtkMessageType type, GtkButtonsType btnType)
{
    GtkWidget *dialog = gtk_message_dialog_new(w,
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               type,
                                               btnType,
                                               msg, NULL);
    gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static inline void ui_show_warning(GtkWindow *w, const gchar *msg)
{
    ui_show_msg_dialog(w, msg, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE);
}

static inline void ui_show_info(GtkWindow *w, const gchar *msg)
{
    ui_show_msg_dialog(w, msg, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE);
}

static inline void ui_show_err(GtkWindow *w, const gchar *msg)
{
    ui_show_msg_dialog(w, msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE);
}

static inline gboolean ui_show_question(GtkWindow *w, const gchar *msg)
{
    ui_show_msg_dialog(w, msg, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL);
}


#endif /* UI_H_ */
