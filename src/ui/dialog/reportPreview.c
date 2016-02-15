/*
 * reportPreview.c
 *
 *  Created on: 2012-6-12
 *      Author: lzk
 */

#include <gtk/gtk.h>
struct mmsghdr;
#include <webkit/webkit.h>

#include "../ui.h"
#include "../../string/_string.h"
#include "../../drawui.h"
#include "../../lzk/fileHandler.h"
extern GtkWidget* baseDialogNew(GtkWidget* fatherWidget);
extern void SaveReportFile(const char* filename ,int isTmp);

#define BUTTON_SAVE 	"ButtonF1"
//#define BUTTON_FILENAME "ButtonF2"
#define BUTTON_CLOSE 	"ButtonF2"

static void responseDialog(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	switch(response_id)
	{
	case GTK_RESPONSE_OK:
		break;
	case GTK_RESPONSE_CANCEL:
		gtk_widget_destroy(GTK_WIDGET(dialog));
		break;

	default:
		break;
	}
}

GtkWidget* webkitScrollNew(GtkWidget* fatherWidget);
void webkitFullScreenNew(GtkWidget* fatherWidget ,const char* url)
{
    GtkWidget* dialog;
    GtkWidget* vbox;
    GtkWidget* scroll;
	WebKitWebView* webview;
	dialog = dialogNew(fatherWidget);
	vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	scroll = webkitScrollNew(NULL);
	gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

	webview = g_object_get_data(G_OBJECT(scroll) ,"webkit");
	g_object_set_data(G_OBJECT(webview) ,"dialog" ,dialog);
	g_signal_connect (G_OBJECT(dialog), "response",G_CALLBACK(responseDialog), NULL);/*发送退出信号*/

    gtk_widget_show_all(dialog) ;
    gtk_widget_hide(gtk_dialog_get_action_area(GTK_DIALOG(dialog)));
	webkit_web_view_load_uri(webview ,url);
}

static gboolean callbackWebkitPress(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    if(event->type == GDK_2BUTTON_PRESS)
    {
    	if(data)
    	{
    		const char* uri = webkit_web_view_get_uri((WebKitWebView*)widget);
    		webkitFullScreenNew(GTK_WIDGET(data) ,uri);
    	}
    	else
    	{
    		GtkWidget* dialog = g_object_get_data(G_OBJECT(widget) ,"dialog");
    		if(dialog)
    		{
    			gtk_widget_destroy(GTK_WIDGET(dialog));
    		    return TRUE;
    		}
    	}
    }
    return FALSE;
}

GtkWidget* webkitScrollNew(GtkWidget* fullScreenFatherWidget)
{
	GtkWidget* scroll;
	WebKitWebView* webview;
	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*)(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	webview = WEBKIT_WEB_VIEW (webkit_web_view_new());
	webkit_web_view_set_custom_encoding (webview ,"UTF-8");
	gtk_container_add(GTK_CONTAINER(scroll), GTK_WIDGET(webview));
	//gtk_scrolled_window_add_with_viewport((GtkScrolledWindow*)scroll, GTK_WIDGET(webview));

	g_signal_connect(G_OBJECT(webview) ,"button-press-event" ,G_CALLBACK(callbackWebkitPress) ,fullScreenFatherWidget);
	g_object_set_data(G_OBJECT(scroll) ,"webkit" ,webview);
	return scroll;
}

static void SaveHtmlFiles(const char* filename ,const char* newDirName ,const char* imageDirName)
{
	char cmd[256];
	memset(cmd ,0 ,256);
	sprintf(cmd ,"cp %s%s %s" ,gTmpReportPath ,filename ,newDirName);
	system(cmd);

	sprintf(cmd ,"cp %s/ %s -rf" ,imageDirName ,newDirName);
	system(cmd);
	system("sync");
}

static void callbackFileSave(GtkWidget* dialog ,void* p_para)
{
//	GtkWidget* buttonFileName = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILENAME);
//	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileName) ,"labelButtonPara");

	char* filename;
//	filename = filenameCheck(gtk_label_get_label(GTK_LABEL(labelButtonPara->data)) ,".html");
	filename = filenameCheck(get_file_name_info(pp->p_config) ,".html");
	char fullFileName[100];

	char imageDirName[100];
	memset(imageDirName ,0 ,100);
	sprintf(fullFileName ,"%s%s" ,gTmpReportPath ,filename);
	GetImageDirName(imageDirName ,fullFileName);

//	memset(fullFileName ,0 ,100);
	strcpy(fullFileName ,getDataPath(DATA_PATH_REPORT));
	if (STORAGE_USB1 == get_file_storage(pp->p_config))
	{
		if(0 == fileDetectUSB())
		{
#if ARM
			strcpy(fullFileName ,"/opt/usbStorage/" );
#endif
		}
	}
	char newDirName[100];
	strcpy(newDirName ,fullFileName);
//	strcat(fullFileName ,"/");
	strcat(fullFileName ,filename);


	char info[100];
	memset(info ,0 ,100);
	if(0 == access(fullFileName ,F_OK))
	{
		sprintf(info ,"\n \"%s\" \n%s?" ,filename ,getDictString(_STRING_File_Exist_Overwrite));
		if(dialogWarning(dialog ,info))
		{
			SaveHtmlFiles(filename ,newDirName ,imageDirName);
		}
	}
	else
	{
		SaveHtmlFiles(filename ,newDirName ,imageDirName);
	}
	free(filename);
}

extern void callbackButtonClose(GtkWidget* dialog ,void* p_para);
//extern void Save_Report_File(char *html_file_name);
//extern void responseDialogFileName(GtkDialog *dialog, gint response_id, gpointer user_data);
//extern void callbackFileName(GtkWidget* dialog ,void* p_para);
GtkWidget* reportPreviewNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = baseDialogNew(fatherWidget);
//	void (*responseDialog)(GtkDialog *dialog, gint response_id, gpointer user_data) = g_object_get_data(G_OBJECT(dialog) ,"responseDialog");
//	g_signal_handlers_disconnect_by_func(dialog ,responseDialog ,NULL);
//	g_signal_connect (G_OBJECT(dialog), "response",G_CALLBACK(responseDialogFileName), NULL);/*发送退出信号*/

	GtkWidget* webkitScroll = webkitScrollNew(dialog);

	GtkWidget* hbox = g_object_get_data(G_OBJECT(dialog) ,"hbox211");
	gtk_box_pack_start(GTK_BOX(hbox), webkitScroll, TRUE, TRUE, 0);
	gtk_widget_show_all(hbox);

	GtkWidget* vbox = g_object_get_data(G_OBJECT(dialog) ,"vbox22");//侧边，report预览隐藏
	gtk_widget_hide(vbox);

	labelButtonStruct* labelButtonPara;
	GtkWidget* buttonSave = g_object_get_data(G_OBJECT(dialog) ,BUTTON_SAVE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonSave) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Save));
	labelButtonInstallCallback(buttonSave ,callbackFileSave ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonSave ,LABELBUTTON_MODE_BIGMENU);
/*
	GtkWidget* buttonFileName = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILENAME);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileName) ,"labelButtonPara");
	labelButtonSetSpecial(buttonFileName ,TRUE);
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,labelString[1]);
	labelButtonInstallCallback(buttonFileName ,callbackFileName ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonFileName ,LABELBUTTON_MODE_DATA3);
	//*/
	GtkWidget* buttonClose = g_object_get_data(G_OBJECT(dialog) ,BUTTON_CLOSE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonClose) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Close));

	labelButtonInstallCallback(buttonClose ,callbackButtonClose ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonClose ,LABELBUTTON_MODE_BIGMENU);

	char* filename;
//	filename = filenameCheck(gtk_label_get_label(GTK_LABEL(labelButtonPara->data)) ,".html");
	filename = filenameCheck(get_file_name_info(pp->p_config) ,".html");
	char fullFileName[100];
	sprintf(fullFileName ,"%s%s" ,gTmpReportPath ,filename);
	free(filename);
	SaveReportFile(fullFileName ,TRUE);

	char tmpHttpPath[256];
	memset(tmpHttpPath ,0 ,256);
	getHttpFileName(tmpHttpPath ,fullFileName);
	GtkWidget* webview = g_object_get_data(G_OBJECT(webkitScroll) ,"webkit");
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview) ,tmpHttpPath);//*/
	return dialog;
}
