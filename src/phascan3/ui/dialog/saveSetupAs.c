/*
 * saveSetupAs.c
 *
 *  Created on: 2012-5-3
 *      Author: gener
 */

#include <gtk/gtk.h>
#include <unistd.h>

#include "../ui.h"
#include "../../string/_string.h"
#include "../../uihandler/handler.h"
#define BUTTON_SAVE 	"ButtonF1"
#define BUTTON_FILENAME "ButtonF2"
#define BUTTON_LOCK 	"ButtonF3"
void callbackFileSave(GtkWidget* dialog ,void* p_para);
void callbackFileName(GtkWidget* dialog ,void* p_para);

#include "../../drawui.h"
void responseDialogFileName(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	GtkWidget* button = NULL;
	char key[20];
	switch(response_id)
	{
	case GTK_RESPONSE_OK:
		break;
	case GTK_RESPONSE_CANCEL:
		gtk_widget_destroy(GTK_WIDGET(dialog));
		break;
	case RESPONSE_LABELBUTTON1:
	case RESPONSE_LABELBUTTON2:
	case RESPONSE_LABELBUTTON7:
	case RESPONSE_LABELBUTTON8:
	case RESPONSE_LABELBUTTON9:
	case RESPONSE_LABELBUTTON10:
	case RESPONSE_LABELBUTTON11:
	case RESPONSE_LABELBUTTON12:
		memset(key ,0 ,20);
		sprintf(key ,"ButtonF%d" ,response_id + 1);
		button = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,key));
		if(button && gtk_widget_is_sensitive(button))
		{
			labelButtoncallbackPress(button);
			labelButtoncallbackRelease(button);
		}
		break;
	case RESPONSE_LABELBUTTON3:
	case RESPONSE_LABELBUTTON4:
		memset(key ,0 ,20);
		sprintf(key ,"ButtonF%d" ,RESPONSE_LABELBUTTON2 + 1);
		button = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,key));
		if(button && gtk_widget_is_sensitive(button))
		{
			labelButtoncallbackPress(button);
			labelButtoncallbackRelease(button);
		}
		break;
	case RESPONSE_LABELBUTTON5:
	case RESPONSE_LABELBUTTON6:
		memset(key ,0 ,20);
		sprintf(key ,"ButtonF%d" ,response_id - 1);
		button = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,key));
		if(button && gtk_widget_is_sensitive(button))
		{
			labelButtoncallbackPress(button);
			labelButtoncallbackRelease(button);
		}
		break;
	default:
		break;
	}
}
#if 0
static void updateLabelButtonSetupLock(GtkWidget* dialog ,int isLock)
{
	GtkWidget* buttonLock = g_object_get_data(G_OBJECT(dialog) ,BUTTON_LOCK);
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonLock) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->data) ,getMainDictString(MAINSTRINGDICT_SWITCH ,isLock));
}

static void callbackSetupLock(GtkWidget* dialog ,void* p_para)
{
//	if(NULL != p_para)
	{
//		GtkWidget* dialog = (GtkWidget*)p_para;
		int isLock = !g_object_get_data(G_OBJECT(dialog) ,"isLock");
		updateLabelButtonSetupLock(dialog ,isLock);
		g_object_set_data(G_OBJECT(dialog) ,"isLock" ,(gpointer)isLock);
	}
}
#endif

void filenameUpdate(GtkWidget* dialog ,const char* fullFileName)
{
	GtkWidget* buttonFileName = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILENAME);
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileName) ,"labelButtonPara");

	char fileName[100];
	memset(fileName ,0 ,100);
	GetFileName(fileName ,fullFileName ,FALSE);
	gtk_label_set_label(GTK_LABEL(labelButtonPara->data) ,fileName);
}

static void setupFileSave(GtkWidget* dialog ,const char* filename)
{
	char* filenameTrue = filenameCheck(filename ,".cfg");
	if(0 == access(filenameTrue ,F_OK))
	{
		if(dialogFileConfirm(dialog ,filenameTrue ,NULL))
		{
			saveConfigFile(filenameTrue);
		}
	}
	else
	{
		saveConfigFile(filenameTrue);
	}
	free(filenameTrue);
}

GtkWidget* saveSetupAsNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = filelistDialogNew(fatherWidget);

	void (*responseDialog)(GtkDialog *dialog, gint response_id, gpointer user_data) = g_object_get_data(G_OBJECT(dialog) ,"responseDialog");
	g_signal_handlers_disconnect_by_func(dialog ,responseDialog ,NULL);
	g_signal_connect (G_OBJECT(dialog), "response",G_CALLBACK(responseDialogFileName), NULL);/*发送退出信号*/

	labelButtonStruct* labelButtonPara;
	GtkWidget* buttonSave = g_object_get_data(G_OBJECT(dialog) ,BUTTON_SAVE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonSave) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Save));
	labelButtonInstallCallback(buttonSave ,callbackFileSave ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonSave ,LABELBUTTON_MODE_BIGMENU);

	GtkWidget* buttonFileName = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILENAME);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileName) ,"labelButtonPara");
	labelButtonSetSpecial(buttonFileName ,TRUE);
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_File_Name));
	labelButtonInstallCallback(buttonFileName ,callbackFileName ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonFileName ,LABELBUTTON_MODE_DATA3);
/*
	GtkWidget* buttonLock = g_object_get_data(G_OBJECT(dialog) ,BUTTON_LOCK);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonLock) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Setup_Lock));
	g_object_set_data(G_OBJECT(dialog) ,"isLock" ,FALSE);
	updateLabelButtonSetupLock(dialog ,FALSE);
	labelButtonInstallCallback(buttonLock ,callbackSetupLock ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonLock ,LABELBUTTON_MODE_DATA3);//*/

	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	filelistSetPathAndSuffix(treeview ,getDataPath(0) ,getDataSuffix(0));

	g_object_set_data(G_OBJECT(dialog) ,"callbackSave" ,setupFileSave);
	g_object_set_data(G_OBJECT(treeview) ,"filePreview" ,filenameUpdate);
	return dialog;
}
