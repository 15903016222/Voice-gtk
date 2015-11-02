/*
 * tableExport.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include "../ui.h"

#include "../../string/_string.h"
#include "../../uihandler/handler.h"
#define BUTTON_SAVE 	"ButtonF1"
#define BUTTON_FILENAME "ButtonF2"
#define BUTTON_LOCK 	"ButtonF3"

//void filenameUpdate(GtkWidget* dialog ,const char* filename);

void callbackFileSave(GtkWidget* dialog ,void* p_para)
{
//	if(NULL != p_para)
	{
//		GtkWidget* dialog = (GtkWidget*)p_para;
		GtkWidget* buttonFileName = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILENAME);
		labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileName) ,"labelButtonPara");
		GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
		GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
		int (*func)(GtkWidget* ,const char*) = g_object_get_data(G_OBJECT(dialog) ,"callbackSave");
		const char* filename;
		filename = gtk_label_get_label(GTK_LABEL(labelButtonPara->data));
		if((NULL == filename) || (0 == strcmp("" , filename)))
		{
			dialogWarning(dialog ,getDictString(_STRING_Please_Enter_File_Name));
			return;
		}
		if(func)
		{
			func(dialog ,filename);
			filelistUpdate(treeview);
		}
	}
}

#include "../../drawui.h"
static void tableFileSave(GtkWidget* dialog ,const char* filename)
{
	char fullfilename[100];
	strcpy(fullfilename ,gReportPath);
	if (STORAGE_USB1 == get_file_storage(pp->p_config))
	{
		if(0 == fileDetectUSB())
		{
#if ARM
			strcpy(fullfilename ,"/opt/usbStorage/" );
#endif
		}
	}
	strcat(fullfilename ,filename);
	char* filenameTrue = filenameCheck(fullfilename ,".html");
	if(0 == access(filenameTrue ,F_OK))
	{
		if(dialogFileConfirm(dialog ,filenameTrue ,NULL))
		{
			SaveTableFile(filenameTrue ,FALSE);
		}
	}
	else
	{
		SaveTableFile(filenameTrue ,FALSE);
	}
	free(filenameTrue);
}

void callbackFileName(GtkWidget* dialog ,void* p_para)
{
//	if(NULL != p_para)
	{
//		GtkWidget* dialog = (GtkWidget*)p_para;
		GtkWidget* buttonFileName = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILENAME);
		labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileName) ,"labelButtonPara");
		char* str = NULL;
		str = dialogGetAString( dialog
							    ,getDictString(_STRING_File_Name)
		                        ,gtk_label_get_label(GTK_LABEL(labelButtonPara->data)));
		if(str)
		{
			gtk_label_set_label(GTK_LABEL(labelButtonPara->data),str);
			free(str);
		}
	}
}

extern void responseDialogFileName(GtkDialog *dialog, gint response_id, gpointer user_data);
extern void webFilePreview(GtkWidget* dialog ,const char* filename);
GtkWidget* tableExportNew(GtkWidget* fatherWidget)
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

//	GtkWidget* buttonLock = g_object_get_data(G_OBJECT(dialog) ,BUTTON_LOCK);
//	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonLock) ,"labelButtonPara");
//	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Setup_Lock));
//	labelButtonSetDisplayMode(buttonLock ,LABELBUTTON_MODE_HIDE);

//	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
//	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
//	filelistSetPathAndSuffix(treeview ,gReportPath ,".html");

	GtkWidget* boxLeft = g_object_get_data(G_OBJECT(dialog) ,"boxLeft");
	gtk_widget_hide(boxLeft);

	g_object_set_data(G_OBJECT(dialog) ,"callbackSave" ,tableFileSave);
//	g_object_set_data(G_OBJECT(dialog) ,"filePreview" ,filenameUpdate);

	webFilePreview(dialog ,gTmpTableFileName);
	return dialog;
}
