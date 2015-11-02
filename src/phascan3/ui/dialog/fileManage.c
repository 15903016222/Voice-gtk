/*
 * fileManage.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../ui.h"
#include "../../string/_string.h"
#include "../../uihandler/handler.h"
static GdkColor	g_ColorWhite     = {0x0, 0xffff, 0xffff, 0xffff};


#define BUTTON_SELECT 		"ButtonF1"
#define BUTTON_SELECTALL 	"ButtonF2"
#define BUTTON_COPY 		"ButtonF3"
#define BUTTON_MOVE 		"ButtonF4"
#define BUTTON_DELETE 		"ButtonF5"
#define BUTTON_RENAME 		"ButtonF6"
#define BUTTON_FILETYPE 	"ButtonF10"
static int isFileListAllSelected(GtkTreeView* treeview)
{
	GtkTreeModel* model;
	GtkTreeIter iter;
	GtkTreeSelection* selection;
	int isAllSelected = TRUE;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

	if(gtk_tree_model_get_iter_first(model ,&iter))
	{
		do
		{
			if(!gtk_tree_selection_iter_is_selected(selection ,&iter))
			{
				isAllSelected = FALSE;
				break;
			}
		}
		while(gtk_tree_model_iter_next(model ,&iter));
	}
	else
	{
		isAllSelected = FALSE;
	}

	return isAllSelected;
}

static void callbackSelect(GtkWidget* dialog ,void* p_para)
{
//	if(NULL != p_para)
	{
//		GtkWidget* dialog = GTK_WIDGET(p_para);
		GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
		GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
		GtkTreePath* treePath;
		GtkTreeViewColumn* focusColumn;
		GtkTreeIter iter;
		GtkTreeModel* model;
		GtkTreeSelection* selection;
		model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		gtk_tree_view_get_cursor(GTK_TREE_VIEW(treeview) ,&treePath ,&focusColumn);
		if(treePath)
		{
			gtk_tree_model_get_iter(model ,&iter ,treePath);
			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
			if(gtk_tree_selection_iter_is_selected(selection ,&iter))
			{
				gtk_tree_selection_unselect_iter(selection ,&iter);
			}
			else
			{
				gtk_tree_selection_select_iter(selection ,&iter);
			}
		}
	}
}

static void callbackSelectAll(GtkWidget* dialog ,void* p_para)
{
//	if(NULL != p_para)
	{
//		GtkWidget* dialog = GTK_WIDGET(p_para);
		GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
		GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
		GtkTreeSelection* selection;
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

		if(isFileListAllSelected(GTK_TREE_VIEW(treeview)))
			gtk_tree_selection_unselect_all(selection);
		else
			gtk_tree_selection_select_all(selection);
	}
}

typedef enum{
	FILE_HANDLER_COPY,
	FILE_HANDLER_MOVE,
	FILE_HANDLER_DELETE,
	FILE_HANDLER_RENAME,
	FILE_HANDLER_MAX
}
	FILE_HANDLER_ENUM;

static void callbackFileHandler(GtkWidget* dialog ,void* p_para)
{
	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	GtkWidget* fileList2 = g_object_get_data(G_OBJECT(dialog) ,"fileBrouser");
	GtkWidget* treeview2 = g_object_get_data(G_OBJECT(fileList2) ,"treeview");
	FILE_HANDLER_ENUM fileHandler = (FILE_HANDLER_ENUM)p_para;
	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	GtkTreeIter iter;
	GList* list = gtk_tree_selection_get_selected_rows(selection ,&model);
	GList* tempList = g_list_first(list);
	char* pathSource = g_object_get_data(G_OBJECT(treeview) ,"path");
	char* pathTarget = g_object_get_data(G_OBJECT(treeview2) ,"path");
	char fileFullName[256];
	char* fileName;
	char newFileName[256];
	char* str;
	int hasOperate = FALSE;
	char dirName[256];
    int ret = -1;
    unsigned int len, len2;
	if(tempList)
	{
		do
		{
			memset(fileFullName ,0 ,256);
			memset(newFileName ,0 ,256);
			GtkTreePath* treePath = tempList->data;
			gtk_tree_model_get_iter(model ,&iter ,treePath);
			gtk_tree_model_get(model ,&iter ,FILELIST_COLUMN_FILENAME ,&fileName ,-1);
			strcpy(fileFullName ,pathSource);
			strcat(fileFullName ,"/");
			strcat(fileFullName ,fileName);

			memset(dirName ,0 ,256);
			str = strrchr(fileFullName ,'.');
			if((str) && !strcmp(".html" ,str))
			{
				memcpy(dirName ,fileFullName ,str - fileFullName);
				strcat(dirName ,"_files");
			}
			switch(fileHandler)
			{
			case FILE_HANDLER_COPY:
                if (ret < 2) {
                    ret = dialogFileConfirm(dialog ,fileName ,pathTarget);
                }
                if (ret<2?ret:ret-2)
				{
					//if(0 == fileCopy(fileFullName ,pathTarget ,"-rf"))
                    if (fileCopyProgressBar(dialog, "cp", fileFullName ,pathTarget ,"-rf") == 0)
					{
						hasOperate = TRUE;
						if(*dirName && (!fileStat(dirName)))//是一个目录
						{
							fileCopy(dirName ,pathTarget ,"-rf");
						}
					}
				}
				break;
			case FILE_HANDLER_MOVE:
                if (ret < 2) {
				    ret = dialogFileConfirm(dialog ,fileName ,pathTarget);
                }
                if (ret<2?ret:ret-2)
				{
					//if(0 == fileMove(fileFullName ,pathTarget))
                    if (fileCopyProgressBar(dialog, "mv", fileFullName ,pathTarget, NULL) == 0)
					{
						hasOperate = TRUE;
						if(*dirName && (!fileStat(dirName)))
						{
							fileMove(dirName ,pathTarget);
						}
					}
				}
				break;
			case FILE_HANDLER_DELETE:
                if (ret < 2) {
				    str = g_strdup_printf("%s\n%s?" ,getDictString(_STRING_DELETE_WARNING) ,fileName);
                    ret = dialogFileWarning(dialog ,str);
				    g_free(str);
                }
                if (ret<2?ret:ret-2)
				//if(dialogWarning(dialog ,str))
				{
					//if (0 == fileDelete(fileFullName ,"-rf"))
                    if (fileCopyProgressBar(dialog, "rm", fileFullName , NULL, "-rf") == 0)
					{
						hasOperate = TRUE;
						if(*dirName && (!fileStat(dirName)))
						{
							fileDelete(dirName ,"-rf");
						}
					}
				}
				break;
			case FILE_HANDLER_RENAME:
				str = dialogGetAString(dialog ,getDictString(_STRING_Rename) ,fileName);
				if(str)
				{
                    len2 = 0;
                    len = 0;
                    while (str[len] != '\0') {
                        if (str[len] != '/') {
                            str[len2] = str[len];
                            len2++;
                        }
                        len++;
                    }
                    str[len2] = '\0';
					char newFileFullName[256];
					memset(newFileFullName ,0 ,256);
					strcpy(newFileFullName ,pathSource);
					strcat(newFileFullName ,"/");
					strcat(newFileFullName ,str);
					if(dialogFileConfirm(dialog ,str ,pathTarget))
					{
						//if(fileRename(fileFullName ,newFileFullName))
                        if (fileCopyProgressBar(dialog, "mv", fileFullName, newFileFullName, NULL) == 0)
							hasOperate = TRUE;
					}
					free(str);
				}
				break;
			default:
				break;
			}
			free(fileName);
			if(hasOperate)
			{
				filelistUpdate(treeview2);
				hasOperate = FALSE;
			}
		}
		while(NULL != (tempList = g_list_next(tempList)));
	}
	g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
	g_list_free (list);
	filelistUpdate(treeview);
}

static void callbackSelectionChanged(GtkTreeSelection *treeselection ,gpointer user_data)
{
	GtkWidget* dialog = GTK_WIDGET(user_data);
	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	GtkWidget* buttonSelect = g_object_get_data(G_OBJECT(dialog) ,BUTTON_SELECT);
	GtkWidget* buttonSelectAll = g_object_get_data(G_OBJECT(dialog) ,BUTTON_SELECTALL);
	GtkTreePath* path = NULL;
	GtkTreeViewColumn* focusColumn;
	labelButtonStruct* labelButtonPara;
	gtk_tree_view_get_cursor(GTK_TREE_VIEW(treeview) ,&path ,&focusColumn);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonSelect) ,"labelButtonPara");
	if(path)
	{
		if(gtk_tree_selection_path_is_selected(treeselection ,path))
		{
			gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Unselect));
		}
		else
		{
			gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Select));
		}
		gtk_tree_path_free(path);
	}
	else
	{
		gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Select));
	}

	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(buttonSelectAll) ,"labelButtonPara");
	if(isFileListAllSelected(GTK_TREE_VIEW(treeview)))
	{
		gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Unselect_All));
	}
	else
	{
		gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Select_All));
	}
}

static void callbackFileType(GtkWidget* widget ,void* p_para)
{
	if(NULL != p_para)
	{
		if(GTK_WIDGET(widget))
		{
			GtkWidget* fileList = g_object_get_data(G_OBJECT(widget) ,"fileList");
			GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
			GtkWidget* fileList2 = g_object_get_data(G_OBJECT(widget) ,"fileBrouser");
			GtkWidget* treeview2 = g_object_get_data(G_OBJECT(fileList2) ,"treeview");
			int para = *(int*)p_para;
			const char* suffix =getDataSuffix(para);
			const char* path = getDataPath(para);

            if(DATA_PATH_USB != para)
			{
				filelistSetPathAndSuffix(treeview ,path ,suffix);
				if(0 == fileDetectUSB())
				{
                    path = getDataPath(DATA_PATH_USB);
				}
				filelistSetPathAndSuffix(treeview2 ,path ,suffix);
			}
			else//all 还是原来的PATH
			{
				filelistSetSuffix(treeview ,suffix);
				if(0 == fileDetectUSB())
				{
                    path = getDataPath(DATA_PATH_USB);
					filelistSetPathAndSuffix(treeview2 ,path ,suffix);
				}
				else
				{
					filelistSetSuffix(treeview2 ,suffix);
				}
			}
		}

	}
}

static void callbackComboNotify(GtkComboBox* widget ,gpointer data)
{
	//动态添加USB菜单
	GtkTreeModel* model;
	GtkTreeIter iter;
	model = gtk_combo_box_get_model(widget);

    if(gtk_tree_model_get_iter_from_string(model ,&iter ,"5"))
	{
		if(0 != fileDetectUSB())
            gtk_combo_box_remove_text(GTK_COMBO_BOX(widget) , DATA_PATH_USB);
	}
	else
	{
		if(0 == fileDetectUSB())
			gtk_combo_box_append_text(GTK_COMBO_BOX(widget) ,getDictString(_STRING_U_Storage));
	}
}

static void callbackCombo(GtkComboBox *widget ,gpointer data)
{
	GtkWidget* treeview = g_object_get_data(G_OBJECT(data) ,"treeview");
	int para = gtk_combo_box_get_active(widget);
	if(para >= 0)
	{
		const char* path = getDataPath(para);
		char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeview) ,"path");
		if(0 != strcmp(path ,fileListPath))
		{
			filelistSetPath(treeview ,path);
		}
	}
	callbackComboNotify(widget ,data);
}

static GtkWidget* fileListNew(const char* str)
{
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	GtkWidget* hbox = gtk_hbox_new(FALSE, 0);

	GtkWidget* label = gtk_label_new(str);
	gtk_widget_modify_fg (label, GTK_STATE_NORMAL, &g_ColorWhite);

	GtkWidget* combo = gtk_combo_box_entry_new_text();
	GtkWidget* entry = gtk_bin_get_child(GTK_BIN(combo));
	gtk_widget_set_sensitive(GTK_WIDGET(entry) ,FALSE);
	g_signal_connect (G_OBJECT(combo), "changed",	G_CALLBACK (callbackCombo), vbox);
	g_signal_connect (G_OBJECT(combo), "grab-notify",	G_CALLBACK (callbackComboNotify), vbox);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo) ,getDictString(_STRING_Setup));
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo) ,getDictString(_STRING_Data));
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo) ,getDictString(_STRING_Report));
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo) ,getDictString(_STRING_Image_));
    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), getDictString(_STRING_CAD));

	GtkWidget* scroll = filelistScrollNew();
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	GtkWidget* treeview = g_object_get_data(G_OBJECT(scroll) ,"treeview");
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)), GTK_SELECTION_BROWSE);

	g_object_set_data(G_OBJECT(vbox) ,"label" ,label);
	g_object_set_data(G_OBJECT(vbox) ,"combo" ,combo);
	g_object_set_data(G_OBJECT(vbox) ,"scroll" ,scroll);
	g_object_set_data(G_OBJECT(vbox) ,"treeview" ,treeview);

	g_object_set_data(G_OBJECT(treeview) ,"combo" ,combo);

	gtk_box_pack_start(GTK_BOX(hbox) ,label ,FALSE ,FALSE ,0);
	gtk_box_pack_start(GTK_BOX(hbox) ,combo ,TRUE ,TRUE ,0);

	gtk_box_pack_start(GTK_BOX(vbox) ,hbox ,FALSE ,FALSE ,0);
	gtk_box_pack_start(GTK_BOX(vbox) ,scroll ,TRUE ,TRUE ,5);

	gtk_widget_show_all(vbox);
	return vbox;
}

#include <gdk/gdkkeysyms.h>
GtkWidget* fileManageNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = filelistDialogNew(fatherWidget);

	GtkWidget* boxLeft = g_object_get_data(G_OBJECT(dialog) ,"boxLeft");
	gtk_widget_set_size_request(GTK_WIDGET(boxLeft), 330, -1);
	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	gtk_widget_destroy(fileList);

	char str[20];
	memset(str ,0 ,20);
	sprintf(str ,"%s:" ,getDictString(_STRING_Source_Path));
	fileList = fileListNew(str);
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);
	g_signal_connect(G_OBJECT(selection), "changed" ,G_CALLBACK(callbackSelectionChanged) ,dialog);

	gtk_box_pack_start(GTK_BOX(boxLeft), fileList, TRUE, TRUE, 0);
	g_object_set_data(G_OBJECT(dialog) ,"fileList" ,fileList);

	GtkWidget* boxRight = g_object_get_data(G_OBJECT(dialog) ,"boxRight");
	GtkWidget* fileBrouser = g_object_get_data(G_OBJECT(dialog) ,"fileBrouser");
	gtk_widget_destroy(fileBrouser);
	memset(str ,0 ,20);
	sprintf(str ,"%s:" ,getDictString(_STRING_Target_Path));
	fileBrouser = fileListNew(str);
	gtk_box_pack_start(GTK_BOX(boxRight), fileBrouser, TRUE, TRUE, 0);
	GtkWidget* scroll = g_object_get_data(G_OBJECT(fileBrouser) ,"scroll");
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	g_object_set_data(G_OBJECT(dialog) ,"fileBrouser" ,fileBrouser);

	labelButtonStruct* labelButtonPara;
	GtkWidget* labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_SELECT);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Select));
	labelButtonInstallCallback(labelButton ,callbackSelect ,dialog ,NULL);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_BIGMENU);

	labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_SELECTALL);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Select_All));
	labelButtonInstallCallback(labelButton ,callbackSelectAll ,dialog ,NULL);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_BIGMENU);

	labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_COPY);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Copy));
	labelButtonInstallCallback(labelButton ,callbackFileHandler ,dialog ,(void*)FILE_HANDLER_COPY);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_BIGMENU);

	labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_MOVE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Move));
	labelButtonInstallCallback(labelButton ,callbackFileHandler ,dialog ,(void*)FILE_HANDLER_MOVE);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_BIGMENU);

	labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_DELETE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Delete));
	labelButtonInstallCallback(labelButton ,callbackFileHandler ,dialog ,(void*)FILE_HANDLER_DELETE);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_BIGMENU);

	labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_RENAME);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_Rename));
	labelButtonInstallCallback(labelButton ,callbackFileHandler ,dialog ,(void*)FILE_HANDLER_RENAME);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_BIGMENU);

	labelButton = g_object_get_data(G_OBJECT(dialog) ,BUTTON_FILETYPE);
	labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(labelButton) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,getDictString(_STRING_File_Type));
//	labelButtonInstallCallback(labelButton ,callbackOpen ,dialog ,NULL);
	labelButtonSetDisplayMode(labelButton ,LABELBUTTON_MODE_DATA3);
	labelButtonPara->parent = dialog;
	popStruct para;
	para.allocation.x = -1;
	para.allocation.y = -1;
	para.current_pos = 0;
	para.item_status = 0;
    para.num_of_item = 6;

	int size = getMainDictSize(MAINSTRINGDICT_FILE_TYPE);
	const char* strDict[size];
	getMainDictStrings(MAINSTRINGDICT_FILE_TYPE ,strDict ,size);

	para.label = strDict;
	para.data = strDict;
	para.push_in = 0;
	para.callbackMenuItem = callbackFileType;
	para.popKey = GDK_F10;
	labelButtonSetFunctionModeAndPara(labelButton ,LABELBUTTON_FUNCTION_MODE_POP ,&para);
	callbackFileType(dialog ,&para.current_pos);
	return dialog;
}

