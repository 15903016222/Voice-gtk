/*
 * baseDialog.c
 *
 *  Created on: 2012-6-12
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "../workpiece.h"
#include "../../drawui.h"
#include "../ui.h"
static gboolean keyHandlerDialog(GtkWidget* widget ,GdkEventKey* event ,gpointer data)
{
	int retvar = FALSE;
	if (event->type == GDK_KEY_PRESS)
	{
		switch(event->keyval)
		{
		case GDK_Super_L:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON1);
			retvar = TRUE;
			break;
		case GDK_F2:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON2);
			retvar = TRUE;
			break;
		case GDK_F3:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON3);
			retvar = TRUE;
			break;
		case GDK_F4:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON4);
			retvar = TRUE;
			break;
		case GDK_F5:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON5);
			retvar = TRUE;
			break;
		case GDK_F6:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON6);
			retvar = TRUE;
			break;
		case GDK_F7:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON7);
			retvar = TRUE;
			break;
		case GDK_F8:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON8);
			retvar = TRUE;
			break;
		case GDK_F9:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON9);
			retvar = TRUE;
			break;
		case GDK_F10:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON10);
			retvar = TRUE;
			break;
		case GDK_F11:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON11);
			retvar = TRUE;
			break;
		case GDK_F12:
			gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LABELBUTTON12);
			retvar = TRUE;
			break;
		default:
			break;
		}
	}
	return retvar;
}

void callbackButtonClose(GtkWidget* dialog ,void* p_para)
{
    change_keypress_event (KEYPRESS_MAIN);
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void responseDialog(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	GtkWidget* button = NULL;
	char key[20];
	switch(response_id)
	{
	case GTK_RESPONSE_OK:
		break;
	case GTK_RESPONSE_CANCEL:
		change_keypress_event (KEYPRESS_MAIN);
		gtk_widget_destroy(GTK_WIDGET(dialog));
		break;
	case RESPONSE_LABELBUTTON1:
	case RESPONSE_LABELBUTTON2:
	case RESPONSE_LABELBUTTON3:
	case RESPONSE_LABELBUTTON4:
	case RESPONSE_LABELBUTTON5:
	case RESPONSE_LABELBUTTON6:
	case RESPONSE_LABELBUTTON7:
	case RESPONSE_LABELBUTTON8:
	case RESPONSE_LABELBUTTON9:
	case RESPONSE_LABELBUTTON10:
	case RESPONSE_LABELBUTTON11:
	case RESPONSE_LABELBUTTON12:
		memset(key ,0 ,20);
		sprintf(key ,"ButtonF%d" ,response_id + 1 - RESPONSE_LABELBUTTON1);
		button = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,key));
		if(button && gtk_widget_is_sensitive(button))
		{
			//g_signal_emit_by_name(G_OBJECT(button) ,"button-press-event" ,NULL);
			labelButtoncallbackPress(button);
			//
			labelButtoncallbackRelease(button);
		}
		break;
	default:
		break;
	}
}

GtkWidget* baseDialogNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = dialogNew(fatherWidget);
	change_keypress_event (KEYPRESS_WARNING);//noting
	g_signal_connect (G_OBJECT(dialog), "key-press-event",G_CALLBACK(keyHandlerDialog), NULL);
	g_signal_connect (G_OBJECT(dialog), "response",G_CALLBACK(responseDialog), NULL);
	g_object_set_data(G_OBJECT(dialog) ,"responseDialog" ,(gpointer)responseDialog);


	GtkWidget* labelButton[NUM_OF_BUTTON];
	int i;
	char key[20];
	for(i = 0 ;i < NUM_OF_BUTTON ;++i)
	{
		labelButton[i] = labelButtonNew();

		memset(key ,0 ,20);
		sprintf(key ,"ButtonF%d" ,i + 1);
		g_object_set_data(G_OBJECT(dialog) ,key ,labelButton[i]);
	}

	GtkWidget* vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget* hbox1 = gtk_hbox_new(FALSE, 0);
	g_object_set_data(G_OBJECT(dialog) ,"hbox1" ,hbox1);//最顶部，主界面才有，其他隐藏

	GtkWidget* hbox2 = gtk_hbox_new(FALSE, 0);

	GtkWidget* vbox21 = gtk_vbox_new(FALSE, 0);
	GtkWidget* vbox22 = gtk_vbox_new(FALSE, 0);//侧边按键
	g_object_set_data(G_OBJECT(dialog) ,"vbox22" ,vbox22);//侧边，report预览隐藏

	GtkWidget* hbox211 = gtk_hbox_new(FALSE, 0);
	GtkWidget* hbox212 = gtk_hbox_new(FALSE, 0);//底边按键
	g_object_set_data(G_OBJECT(dialog) ,"hbox211" ,hbox211);//主画图区域
	for(i = NUM_OF_BUTTON_BOTTOM ;i < NUM_OF_BUTTON ;++i)
	{
			gtk_box_pack_end(GTK_BOX(vbox22), labelButton[i], FALSE, FALSE, 0);
	}

	for(i = 0 ;i < NUM_OF_BUTTON_BOTTOM ;++i)
	{
		gtk_box_pack_start(GTK_BOX(hbox212), labelButton[i], FALSE, FALSE, 0);
	}

	gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), vbox21, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), vbox22, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox21), hbox211, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(vbox21), hbox212, FALSE, FALSE, 0);

	gtk_widget_show_all(gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	gtk_widget_hide (gtk_dialog_get_action_area(GTK_DIALOG(dialog)));

	//默认隐藏的控件
	gtk_widget_hide(hbox1);
	for(i = 0 ;i < NUM_OF_BUTTON ;++i)
	{
		labelButtonSetDisplayMode(labelButton[i] ,LABELBUTTON_MODE_HIDE);
	}
	return dialog;
}
