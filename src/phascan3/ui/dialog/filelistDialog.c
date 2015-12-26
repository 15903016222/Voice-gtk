/*
 * filelistDialog.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */
#include <gtk/gtk.h>
#include <sys/stat.h>

#include "../../drawui.h"
#include "../../string/_string.h"
#include "../../uihandler/handler.h"
#include "../workpiece.h"
#include "gtk_dxf.h"
#include "../ui.h"
extern GtkWidget* webkitScrollNew(GtkWidget* fatherWidget);

void webkitRefresh(GtkWidget* dialog)
{
	GtkWidget* fileBrouser = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,"fileBrouser"));
	GtkWidget* scroll = GTK_WIDGET(g_object_get_data(G_OBJECT(fileBrouser) ,"scroll"));
	if(scroll)
	{
		gtk_widget_destroy(scroll);
		scroll = NULL;
		scroll = webkitScrollNew(dialog);
		g_object_set_data(G_OBJECT(fileBrouser) ,"scroll" ,scroll);
		gtk_box_pack_start(GTK_BOX(fileBrouser), scroll, TRUE, TRUE, 0);
		gtk_widget_show_all(scroll);
	}
}

GtkWidget* fileBrouserNew(GtkWidget* fatherWidget ,const char* str)
{
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);

	GtkWidget* label = gtk_label_new(str);
	gtk_widget_modify_fg (label, GTK_STATE_NORMAL, &color_white);

	GtkWidget* scroll = webkitScrollNew(fatherWidget);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	g_object_set_data(G_OBJECT(vbox) ,"label" ,label);
	g_object_set_data(G_OBJECT(vbox) ,"scroll" ,scroll);

	gtk_box_pack_start(GTK_BOX(vbox) ,label ,FALSE ,FALSE ,10);
	gtk_box_pack_start(GTK_BOX(vbox) ,scroll ,TRUE ,TRUE ,0);

	return vbox;
}

void webFilePreview(GtkWidget* dialog ,const char* filename)
{
	char webfile[256];
	memset(webfile ,0 ,256);
//	strcpy(webfile ,WEB_PREFIX);
//	strcat(webfile ,filename);
	getHttpFileName(webfile ,filename);
	webkitRefresh(dialog);

	GtkWidget* fileBrouser = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,"fileBrouser"));
	GtkWidget* scroll = GTK_WIDGET(g_object_get_data(G_OBJECT(fileBrouser) ,"scroll"));
	GtkWidget* webkit = GTK_WIDGET(g_object_get_data(G_OBJECT(scroll) ,"webkit"));
	webkit_web_view_load_uri((WebKitWebView*)webkit ,webfile);
}

static void setupFilePreview(GtkWidget* dialog ,const char* filename)
{
	struct stat buf;
	stat(filename ,&buf);
	tmpHtmlStruct tmpHtml;
	memset(&tmpHtml ,0 ,sizeof(tmpHtmlStruct));
	char* str = strrchr(filename ,'/');
	if(str && ((str - filename) != strlen(filename)))
		tmpHtml.filename = str + 1;
	else
		tmpHtml.filename = filename;

	tmpHtml.fileSize = buf.st_size;
	tmpHtml.fileSaveDate = ctime((time_t*)(&(buf.st_mtim)));
	tmpHtml.softwareVersion = "1.0";
//	tmpHtml.module = "222";
//	tmpHtml.note = "333";
//	tmpHtml.head = "444";
//	tmpHtml.channelQuantity = "3";
//	tmpHtml.maskOfUserFeild = 1;
//	tmpHtml.labelUserFeild[0] = "lzk";
//	tmpHtml.nameUserFeild[0] = "tcy";
	char tmpFileName[256];
	memset(tmpFileName ,0 ,256);
	sprintf(tmpFileName ,"%stmp.html" ,gTmpReportPath);
	if(0 == htmlStat(tmpFileName ,&tmpHtml))
	{
		webFilePreview(dialog ,tmpFileName);
	}
}

static void filePreview(GtkWidget* dialog ,const char* filename)
{
	if(0 == fileStat(filename))
	{
		setupFilePreview(dialog ,filename);
	}
	else
	{
		const char* str = strrchr(filename ,'.');
		if(str)
		{
			if(	(0 == strcmp(".cfg" ,str))
				||(0 == strcmp(".data" ,str))
				)
			{
				setupFilePreview(dialog ,filename);
			}
			else
			{
				webFilePreview(dialog ,filename);
			}
		}
		else
		{
			webFilePreview(dialog ,filename);
		}
	}
}

void CallbackPreview(GtkTreeSelection *treeselection ,gpointer user_data)
{
	GtkWidget* dialog = GTK_WIDGET(user_data);
	if(dialog)
	{
		GtkTreeModel* model ;
		GtkTreeIter iter;
		if(gtk_tree_selection_get_selected(treeselection ,&model ,&iter))
		{
			char* filename;
			gtk_tree_model_get(model ,&iter ,FILELIST_COLUMN_FILENAME ,&filename ,-1);
			GtkWidget* treeview = GTK_WIDGET(gtk_tree_selection_get_tree_view(treeselection));
			char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeview) ,"path");
			char fullName[256];
			memset(fullName ,0 ,256);
			strcpy(fullName ,fileListPath);
//			strcat(fullName ,"/");
			strcat(fullName ,filename);
			void (* func)(GtkWidget* ,const char*) =
					g_object_get_data(G_OBJECT(treeview) ,"filePreview");
			if(func)
				func(dialog ,fullName);
			free(filename);
		}
	}
}

static void callbackSelectionChanged(GtkTreeSelection *treeselection ,gpointer user_data)
{
	GtkWidget* treeview = GTK_WIDGET(gtk_tree_selection_get_tree_view(treeselection));
	void (* func)(GtkTreeSelection* ,gpointer) =
			g_object_get_data(G_OBJECT(treeview) ,"callbackSelectionChanged");
	if(func)
		func(treeselection ,user_data);

}

static gboolean callbackRowActivated(GtkTreeView *tree_view ,GtkTreePath *path,GtkTreeViewColumn *column,gpointer user_data)
{
	if(NULL != user_data)
	{
		GtkWidget* dialog = (GtkWidget*)user_data;

		GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
		GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
		GtkTreeIter iter;
		if(gtk_tree_selection_get_selected(selection ,&model ,&iter))
		{
			char* filename;
			gtk_tree_model_get(model ,&iter ,FILELIST_COLUMN_FILENAME ,&filename ,-1);

			char* fileListPath = (char*)g_object_get_data(G_OBJECT(tree_view) ,"path");
			char fullName[256];
			memset(fullName ,0 ,256);
			strcpy(fullName ,fileListPath);
			strcat(fullName ,"/");
			strcat(fullName ,filename);

			if(0 != fileStat(fullName))
			{
				void (* func)(GtkWidget* ,const char*) =
						(void (*)(GtkWidget* ,const char*))g_object_get_data(G_OBJECT(dialog) ,"callbackRowActivated");
				if(func)
					func(dialog ,fullName);
			}
			free(filename);
		}
	}
	return FALSE;
}

static GtkWidget* fileListNew(const char* str)
{
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);

	GtkWidget* label = gtk_label_new(str);
	gtk_widget_modify_fg (label, GTK_STATE_NORMAL, &color_white);

	GtkWidget* scroll = filelistScrollNew();
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	GtkWidget* treeview = g_object_get_data(G_OBJECT(scroll) ,"treeview");
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)), GTK_SELECTION_BROWSE);

	g_object_set_data(G_OBJECT(vbox) ,"label" ,label);
	g_object_set_data(G_OBJECT(vbox) ,"scroll" ,scroll);
	g_object_set_data(G_OBJECT(vbox) ,"treeview" ,treeview);

	gtk_box_pack_start(GTK_BOX(vbox) ,label ,FALSE ,FALSE ,10);
	gtk_box_pack_start(GTK_BOX(vbox) ,scroll ,TRUE ,TRUE ,0);

	return vbox;
}

#define BUTTON_CLOSE "ButtonF11"
extern GtkWidget* baseDialogNew(GtkWidget* fatherWidget);
extern void callbackButtonClose(GtkWidget* dialog ,void* p_para);
GtkWidget* filelistDialogNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = baseDialogNew(fatherWidget);

	GtkWidget* dirList = fileListNew("dir");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(dirList) ,"treeview");
	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(callbackSelectionChanged), dialog);
	g_object_set_data(G_OBJECT(treeview) ,"callbackSelectionChanged" ,CallbackPreview);
	g_object_set_data(G_OBJECT(dialog) ,"dirList" ,dirList);
	gboolean (*rowActivatedCallback)(GtkTreeView* ,GtkTreePath*,GtkTreeViewColumn*,gpointer) =
			g_object_get_data(G_OBJECT(treeview) ,"rowActivatedCallback");
	g_signal_handlers_disconnect_by_func(GTK_OBJECT(treeview) ,G_CALLBACK(rowActivatedCallback) ,NULL);


	GtkWidget* fileList = fileListNew(getDictString(_STRING_File_List));
	treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(callbackSelectionChanged), dialog);
	g_signal_connect(G_OBJECT(treeview) ,"row-activated" ,G_CALLBACK(callbackRowActivated) ,dialog);
	g_object_set_data(G_OBJECT(treeview) ,"callbackSelectionChanged" ,CallbackPreview);
	g_object_set_data(G_OBJECT(treeview) ,"filePreview" ,filePreview);
	g_object_set_data(G_OBJECT(dialog) ,"fileList" ,fileList);

	GtkWidget* fileBrouser = fileBrouserNew(dialog ,getDictString(_STRING_File_Preview));
	g_object_set_data(G_OBJECT(dialog) ,"fileBrouser" ,fileBrouser);

	GtkWidget* hbox = g_object_get_data(G_OBJECT(dialog) ,"hbox211");
	GtkWidget* vbox1 = gtk_vbox_new(FALSE, 0);//左列表
	GtkWidget* vbox2 = gtk_vbox_new(FALSE, 0);//右列表

	gtk_box_pack_start(GTK_BOX(vbox1), dirList, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox1), fileList, TRUE, TRUE, 0);
	gtk_widget_set_size_request(GTK_WIDGET(vbox1), 250, -1);
	g_object_set_data(G_OBJECT(dialog) ,"boxLeft" ,vbox1);

	gtk_box_pack_start(GTK_BOX(vbox2), fileBrouser, TRUE, TRUE, 0);
	g_object_set_data(G_OBJECT(dialog) ,"boxRight" ,vbox2);

	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);

	gtk_widget_show_all(hbox);
	gtk_widget_hide(dirList);

	labelButtonStruct* labelButtonPara;
	GtkWidget* buttonClose = g_object_get_data(G_OBJECT(dialog) ,BUTTON_CLOSE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonClose) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Close));
	labelButtonInstallCallback(buttonClose ,callbackButtonClose ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonClose ,LABELBUTTON_MODE_BIGMENU);
	return dialog;
}

/**
 * workpiece
 */
static gboolean timerFlag = FALSE;  /*定时器标志*/
static GTimeVal changeTime;         /*treeSelect产生事件的时间*/
G_LOCK_DEFINE(timerMutex);          /*定时器互斥量*/
#define INTERVAL_TIME   300         /*单位毫秒*/
static gboolean time_handle(gpointer data)
{
    GTimeVal curtime;
    g_get_current_time(&curtime);

    G_LOCK(timerMutex);
    if ((curtime.tv_sec*1000+curtime.tv_usec/1000) - (changeTime.tv_sec*1000+changeTime.tv_usec/1000) < INTERVAL_TIME) {
        G_UNLOCK(timerMutex);
        return TRUE;
    }

    GtkTreeSelection *treeselection = GTK_TREE_SELECTION(data);
    GtkDxf *gdxf = GTK_DXF(g_object_get_data(G_OBJECT(treeselection), "gdxf"));
    GtkWidget *treeview = NULL;
    GtkTreeModel *model = NULL;
    GtkTreeIter iter;
    gchar *filename = NULL;         /*保存文件名*/
    GString *fullname = NULL;       /*文件绝对路径*/

    if( ! gtk_tree_selection_get_selected(treeselection ,&model ,&iter) ) {
        goto TIME_HANDLE_END;
    }

    gtk_tree_model_get(model ,&iter ,FILELIST_COLUMN_FILENAME ,&filename ,-1);
    if (filename == NULL) {
        goto TIME_HANDLE_END;
    }

    if ( g_ascii_strncasecmp(filename + strlen(filename)-4, ".dxf", 4) ) {
        g_free(filename);
        goto TIME_HANDLE_END;
    }

    treeview = GTK_WIDGET(gtk_tree_selection_get_tree_view(treeselection));
    fullname = g_string_sized_new(0);
    g_string_append(fullname,
                    (gchar*)g_object_get_data(G_OBJECT(treeview) ,"path"));
    g_string_append(fullname, filename);

    gtk_dxf_load_file(gdxf, fullname->str, DXF_SECTION_HEADER|DXF_SECTION_ENTITIES);

    g_free(filename);
    g_string_free(fullname, TRUE);

TIME_HANDLE_END:
    timerFlag = FALSE;
    G_UNLOCK(timerMutex);
    return FALSE;
}

static void on_treeSelect_changed(GtkTreeSelection *treeselection ,gpointer user_data)
{
    G_LOCK(timerMutex);
    g_get_current_time(&changeTime);
    if (!timerFlag) {
        g_timeout_add(INTERVAL_TIME, time_handle, treeselection);
        timerFlag = TRUE;
    }
    G_UNLOCK(timerMutex);
}

static void on_dxf_mouse_event(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    static gdouble _x = 0;
    static gdouble _y = 0;

    if (event->type == GDK_BUTTON_PRESS) {
        _x = event->x;
        _y = event->y;
    } else if (event->type == GDK_MOTION_NOTIFY) {
        gtk_dxf_set_basepoint(GTK_DXF(widget),
                              GTK_DXF(widget)->basePointX + (event->x-_x),
                              GTK_DXF(widget)->basePointY + (event->y-_y));
        _x = event->x;
        _y = event->y;
    }
}


GtkWidget* workpiece_chooser_dialog_new(GtkWidget* parent)
{
    GtkWidget* dialog = baseDialogNew(parent);
    GtkWidget *previewVbox = NULL;
    GtkWidget *previewLabel = NULL;
    GtkWidget *gdxf = NULL;

    GtkWidget* dirList = fileListNew("dir");
    GtkWidget* treeview = g_object_get_data(G_OBJECT(dirList) ,"treeview");
    GtkTreeSelection* treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    GtkWidget* fileList = fileListNew(getDictString(_STRING_File_List));
    treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
    treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    g_signal_connect(G_OBJECT(treeSelection), "changed", G_CALLBACK(callbackSelectionChanged), dialog);
    g_signal_connect(G_OBJECT(treeview) ,"row-activated" ,G_CALLBACK(callbackRowActivated) ,dialog);
    g_object_set_data(G_OBJECT(dialog) ,"fileList" ,fileList);

    /* previewLabel */
    previewLabel = gtk_label_new(getDictString(_STRING_File_Preview));
    gtk_widget_modify_fg(GTK_WIDGET(previewLabel), GTK_STATE_NORMAL, &color_white);

    /* gdxf */
    gdxf = gtk_dxf_new();
    gtk_dxf_set_bg(GTK_DXF(gdxf), &color_black);
    gtk_dxf_set_fg(GTK_DXF(gdxf), &color_green);
    gtk_widget_set_size_request(gdxf, 500, 454);
    g_signal_connect(G_OBJECT(gdxf), "motion-notify-event",
                     G_CALLBACK(on_dxf_mouse_event), NULL);  /*添加事件移动事件*/
    g_signal_connect(G_OBJECT(gdxf), "button-press-event",
                     G_CALLBACK(on_dxf_mouse_event), NULL); /*点击事件*/
    g_signal_connect(G_OBJECT(gdxf), "button-release-event",
                     G_CALLBACK(on_dxf_mouse_event), NULL); /*释放事件*/


    /* previewVBox */
    previewVbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(previewVbox), previewLabel, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(previewVbox), gdxf, TRUE, TRUE, 0);

    g_object_set_data(G_OBJECT(treeSelection), "gdxf", gdxf);
    g_signal_connect(G_OBJECT(treeSelection), "changed",
                     G_CALLBACK(on_treeSelect_changed), NULL);

    GtkWidget* hbox = g_object_get_data(G_OBJECT(dialog) ,"hbox211");
    GtkWidget* vbox1 = gtk_vbox_new(FALSE, 0);//左列表
    GtkWidget* vbox2 = gtk_vbox_new(FALSE, 0);//右列表

    gtk_box_pack_start(GTK_BOX(vbox1), dirList, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox1), fileList, TRUE, TRUE, 0);
    gtk_widget_set_size_request(GTK_WIDGET(vbox1), 250, -1);
    g_object_set_data(G_OBJECT(dialog) ,"boxLeft" ,vbox1);

    gtk_box_pack_start(GTK_BOX(vbox2), previewVbox, TRUE, TRUE, 0);
    g_object_set_data(G_OBJECT(dialog) ,"boxRight" ,vbox2);

    gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);

    gtk_widget_show_all(hbox);
    gtk_widget_hide(dirList);

    labelButtonStruct* labelButtonPara;
    GtkWidget* buttonClose = g_object_get_data(G_OBJECT(dialog) ,BUTTON_CLOSE);
    labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonClose) ,"labelButtonPara");
    gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Close));
    labelButtonInstallCallback(buttonClose ,callbackButtonClose ,dialog ,NULL);
    labelButtonSetDisplayMode(buttonClose ,LABELBUTTON_MODE_BIGMENU);
    return dialog;
}
