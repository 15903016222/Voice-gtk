/*
 * fileOpen.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>

#include "../ui.h"
#include "../workpiece.h"
#include "../../globalData.h"
#include "../../string/_string.h"
#include "../../drawui.h"
#include "../../callback.h"
#include "../../uihandler/handler.h"
#include "../../lzk/fileHandler.h"
#include "dxf.h"

void webkitFullScreenNew(GtkWidget* fatherWidget ,const char* url);

#define BUTTON_OPEN 		"ButtonF1"
#define BUTTON_FILETYPE 	"ButtonF10"
#define BUTTON_LISTFILES 	"ButtonF9"
static void openSetupFile(GtkWidget* dialog ,const char* filename)
{
	char* markup ;
	if(dialogWarning(dialog ,getDictString(_STRING_read_setup_file_)))
	{
		if(0 == handlerOpenSetupFile(filename))
		{

			gtk_widget_destroy(dialog);

			pp->pos_pos = MENU3_STOP;
			change_language (get_language(pp->p_config), pp);
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);

			const char* str = strrchr(filename ,'/');
			memset(gData->file.setupfile ,0 ,FILE_SIZE);
			if(str)
			{
				strcpy(gData->file.setupfile ,str + 1);
			}
			else
			{
				strcpy(gData->file.setupfile ,filename);
			}
			markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>" ,gData->file.setupfile);
				gtk_label_set_markup (GTK_LABEL (pp->label[2]), markup);
			g_free(markup);

			markup=g_markup_printf_escaped(
						"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>", GROUP_VAL_POS (0, prf1) / 10, GROUP_VAL_POS(0 , prf1) * get_beam_qty() / 10);

			gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
			g_free(markup);
			markup = g_markup_printf_escaped (
					"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(double)(GROUP_VAL_POS(0 , prf1)/ 10.0));
			gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup);
			g_free(markup);
		}

	}
}

static void openDataFile(GtkWidget* dialog ,const char* filename)
{
	if(dialogWarning(dialog ,getDictString(_STRING_read_data_file_)))
	{
        if(checkDataFile(filename)) {
            return ;
        }
		request_refresh(REFRESH_SCANDATA_LOAD) ;
		HandlerOpenDataFile(filename);
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_widget_destroy(dialog);
		setKeyInvalidateWhenDataRecalling(FALSE);
		TMP(scan)   = 0 ;
		TMP(index)  = 0 ;
        SetDefaultTOFTCal();
	}
}

static void filePreviewFullScreen(GtkWidget* dialog ,const char* filename)
{
	char webfile[256];
	memset(webfile ,0 ,256);
	getHttpFileName(webfile ,filename);
//	GtkWidget* fileBrouser = GTK_WIDGET(g_object_get_data(G_OBJECT(dialog) ,"fileBrouser"));
//	GtkWidget* scroll = GTK_WIDGET(g_object_get_data(G_OBJECT(fileBrouser) ,"scroll"));
//	GtkWidget* webview = GTK_WIDGET(g_object_get_data(G_OBJECT(scroll) ,"webkit"));
//	webkit_web_view_load_uri((WebKitWebView*)webview ,webfile);
	webkitFullScreenNew(dialog ,webfile);
}


static void openPartFile(GtkWidget* dialog ,const char* filename)
{
    if(access(filename, F_OK)) {
        dialogError(dialog ,getDictString(_STRING_read_part_file_error));
        return;
    }

    //draw_menu2(0);
    if( ! workpiece_load_file(filename , get_current_group(pp->p_config)) ) {
        dialogError(dialog ,getDictString(_STRING_read_part_file_error));
        return;
    }

    pp->p_config->bPartLoaded = TRUE ;
    pp->pos_pos = MENU3_STOP;
    draw_menu3(0, NULL);
    gtk_widget_destroy(dialog);
    gtk_widget_queue_draw (pp->vboxtable);

}

static void openFile(GtkWidget* dialog ,const char* filename)
{
    const char* str = strrchr(filename ,'.');
    if(str) {
        if(0 == strcmp(".cfg" ,str)) {
            openSetupFile(dialog ,filename);
        } else if(0 == strcmp(".data" ,str)) {
            openDataFile(dialog ,filename);
        } else if(0 == g_strcmp0(".dxf" ,str)) {
            openPartFile(dialog ,filename);
        } else {
            filePreviewFullScreen(dialog ,filename);
        }
    } else {
        filePreviewFullScreen(dialog ,filename);
    }

}

static void callbackOpen(GtkWidget* dialog ,void* p_para)
{

		GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
		GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
		GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		GtkTreeIter iter;
		if(gtk_tree_selection_get_selected(selection ,&model ,&iter))
		{
			char* filename;
			gtk_tree_model_get(model ,&iter ,FILELIST_COLUMN_FILENAME ,&filename ,-1);

			char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeview) ,"path");
			char fullName[256];
			memset(fullName ,0 ,256);
			strcpy(fullName ,fileListPath);
		//	strcat(fullName ,"/");
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

static void updateLabelButtonListedFiles(GtkWidget* widget ,int isSample)
{
	GtkWidget* buttonListFiles = g_object_get_data(G_OBJECT(widget) ,BUTTON_LISTFILES);
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonListFiles) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->data) ,getMainDictString(MAINSTRINGDICT_FILE_LIST ,isSample));
}

static void callbackFileType(GtkWidget* widget ,void* p_para)
{
	if(NULL != p_para)
	{
		if(widget)
		{
			int para = *(int*)p_para;
			GtkWidget* buttonListFiles = g_object_get_data(G_OBJECT(widget) ,BUTTON_LISTFILES);
			int isSample = (int)g_object_get_data(G_OBJECT(widget) ,"isSample");
			int offset = 0;
			switch(para)
			{
			case 0:
			case 1:
				labelButtonSetDisplayMode(buttonListFiles ,LABELBUTTON_MODE_DATA3);
				if(isSample)
					offset = 5;
				break;
			case 2:
			case 3:
			default:
				isSample = FALSE;
				labelButtonSetDisplayMode(buttonListFiles ,LABELBUTTON_MODE_DISABLE);
				updateLabelButtonListedFiles(widget ,isSample);
				g_object_set_data(G_OBJECT(widget) ,"isSample" ,(gpointer)isSample);
				break;
			}
			const char* suffix = getDataSuffix(para);
			const char* path = getDataPath(para + offset);

			GtkWidget* fileList = g_object_get_data(G_OBJECT(widget) ,"fileList");
			GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
			filelistSetPathAndSuffix(treeview ,path ,suffix);
		}
	}
}

static void callbackListedFiles(GtkWidget* dialog ,void* p_para)
{
//	if(NULL != p_para)
	{
//		GtkWidget* dialog = (GtkWidget*)p_para;
		int isSample = !g_object_get_data(G_OBJECT(dialog) ,"isSample");
		GtkWidget* buttonFileType = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILETYPE);
		labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileType) ,"labelButtonPara");
		callbackFileType(dialog ,&(labelButtonPara->popPara->current_pos));
		updateLabelButtonListedFiles(dialog ,isSample);
		g_object_set_data(G_OBJECT(dialog) ,"isSample" ,(gpointer)isSample);
	}
}

#include <gdk/gdkkeysyms.h>
GtkWidget* fileOpenNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = filelistDialogNew(fatherWidget);

	labelButtonStruct* labelButtonPara;
	GtkWidget* buttonOpen = g_object_get_data(G_OBJECT(dialog) ,BUTTON_OPEN);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonOpen) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Open));
	labelButtonInstallCallback(buttonOpen ,callbackOpen ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonOpen ,LABELBUTTON_MODE_BIGMENU);

	GtkWidget* buttonFileType = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILETYPE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonFileType) ,"labelButtonPara");
	labelButtonPara->parent = dialog;
	popStruct para;
	para.allocation.x = -1;
	para.allocation.y = -1;
	para.current_pos = 0;
	para.item_status = 0;
	para.num_of_item = 4;

	int size = getMainDictSize(MAINSTRINGDICT_FILE_TYPE);
	const char* strDict[size];
	getMainDictStrings(MAINSTRINGDICT_FILE_TYPE ,strDict ,size);

	para.label = strDict;
	para.data = strDict;
	para.push_in = 0;
	para.callbackMenuItem = callbackFileType;
	para.popKey = GDK_F10;
	labelButtonSetFunctionModeAndPara(buttonFileType ,LABELBUTTON_FUNCTION_MODE_POP ,&para);
	callbackFileType(dialog ,&(para.current_pos));
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_File_Type));
	labelButtonSetDisplayMode(buttonFileType ,LABELBUTTON_MODE_DATA3);

	GtkWidget* buttonListFiles = g_object_get_data(G_OBJECT(dialog) ,BUTTON_LISTFILES);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonListFiles) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_listed_Files));
	updateLabelButtonListedFiles(dialog ,FALSE);
	g_object_set_data(G_OBJECT(dialog) ,"isSample" ,FALSE);
	labelButtonInstallCallback(buttonListFiles ,callbackListedFiles ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonListFiles ,LABELBUTTON_MODE_DATA3);

	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	filelistSetPathAndSuffix(treeview ,getDataPath(0) ,getDataSuffix(0));

	g_object_set_data(G_OBJECT(dialog) ,"callbackRowActivated" ,openFile);
	return dialog;
}


GtkWidget* PartFileOpenNew(GtkWidget* fatherWidget)
{

    GtkWidget* dialog = workpiece_chooser_dialog_new(fatherWidget);
	labelButtonStruct* labelButtonPara;
	GtkWidget* buttonOpen = g_object_get_data(G_OBJECT(dialog) ,BUTTON_OPEN);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonOpen) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Open));
	labelButtonInstallCallback(buttonOpen ,callbackOpen ,dialog ,NULL);
	labelButtonSetDisplayMode(buttonOpen , LABELBUTTON_MODE_BIGMENU);

	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
    filelistSetPathAndSuffix(treeview ,"/home/tt/TT/data/Part/", ".dxf");

	g_object_set_data(G_OBJECT(dialog) ,"callbackRowActivated" ,openFile);
	return dialog;
}
