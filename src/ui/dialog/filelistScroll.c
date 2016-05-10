/*
 * filelistScroll.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "filelist.h"
#include "../../string/_string.h"
void listItemSet(GtkTreeModel* model ,GtkTreeIter* iter ,const char* filename,int isDir)
{
//	GtkWidget* image;
//	GtkWidget* hbox = gtk_hbox_new(FALSE, 5);
//	GtkWidget* label = gtk_label_new(filename);
    char* stock_id;
//    GtkIconSize iconSize = GTK_ICON_SIZE_LARGE_TOOLBAR;
    if(0 != isDir)
    {
//    	image = gtk_image_new_from_stock(GTK_STOCK_DIRECTORY, iconSize);
    	stock_id = GTK_STOCK_DIRECTORY;
    }else{
 //   	image = gtk_image_new_from_stock(GTK_STOCK_FILE, iconSize);
    	stock_id = GTK_STOCK_FILE;
    }
//    gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);
//    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

//	gtk_image_get_stock((GtkImage*)image ,&stock_id ,&iconSize);
    gtk_list_store_set((GtkListStore*)model ,iter
    		,FILELIST_COLUMN_IMAGE ,stock_id
    		,FILELIST_COLUMN_FILENAME ,filename
    		,-1);
}

int updateList(GtkTreeModel* model ,const char* path ,const char* suffix)
{
    DIR* dir = opendir(path);
    struct dirent* enump = NULL;
    GList* dlist = NULL;
    GList* flist = NULL;
    size_t name_len;
    const char* str;
    if(NULL == dir)
    {
        g_print("Open directory failed:%s.\n", path);
        return 1;
    }
    chdir(path);
    while((enump = readdir(dir)))
    {
        name_len = strlen(enump->d_name);

//       if( (name_len == 1 && enump->d_name[0] == '.') //不显示“.”
       if( enump->d_name[0] == '.') //不显示“.”开头
       {
    	   if(1 == name_len)
    		   continue;
    	   else if(!((NULL == suffix) || (0 == strcmp("", suffix))))
    	   {//屏蔽
    		   continue;
    	   }
       }

       if(name_len == 2 && !strncmp(enump->d_name, "..", 2))//不显示“..”
       {
            continue;
       }

        if(DT_DIR == enump->d_type) //文件夹
        {
        	if((NULL == suffix) || (0 == strcmp("", suffix)))
		   {//不屏蔽
				dlist = g_list_prepend(dlist, enump->d_name);
		   }
		   else
		   {
			   continue;
		   }
        }
        else if(DT_REG == enump->d_type)
        {
			if((NULL == suffix) || (0 == strcmp("", suffix)))
			{
				flist = g_list_prepend(flist, enump->d_name);
			}
			else
			{
				str = strrchr(enump->d_name,'.');
				if((NULL != str) && (0 == strcmp(str, suffix)))//匹配后缀
				{
					flist = g_list_prepend(flist, enump->d_name);
				}
				else
				{
					continue;
				}
			}
        }
    }
    dlist = g_list_sort(dlist ,(GCompareFunc)g_strcasecmp);//排序
    flist = g_list_sort(flist ,(GCompareFunc)g_strcasecmp);//排序

    gtk_list_store_clear(GTK_LIST_STORE(model));
	GtkTreeIter iter;

	GList* tempList = g_list_first(dlist);
	if(tempList)
	{
		do
		{
			gtk_list_store_append(GTK_LIST_STORE(model) ,&iter);
			listItemSet(model ,&iter ,tempList->data ,1);
		}
		while(NULL != (tempList = g_list_next(tempList)));
	}
	tempList = g_list_first(flist);
	if(tempList)
	{
		do
		{
			gtk_list_store_append(GTK_LIST_STORE(model) ,&iter);
			listItemSet(model ,&iter ,tempList->data ,0);
		}
		while(NULL != (tempList = g_list_next(tempList)));
	}
    return 0;
}

int filelistUpdate(GtkWidget* treeView)
{
	char* fileListSuffix = (char*)g_object_get_data(G_OBJECT(treeView) ,"suffix");
	char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeView) ,"path");
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeView));
	if(0 == updateList(model ,fileListPath ,fileListSuffix))
	{
		return 0;
	}
	return 1;
}

void comboUpdateEntry(GtkWidget* combo ,const char* path)
{

    if ( ! GTK_IS_COMBO_BOX(combo) ) {
        return;
    }

    GtkWidget* entry = gtk_bin_get_child(GTK_BIN(combo));

    if ( ! GTK_IS_ENTRY(entry) ) {
        return;
    }

    if(0 == strcmp(path ,gtk_entry_get_text(GTK_ENTRY(entry)))) {
        return;
    }

    gtk_entry_set_text(GTK_ENTRY(entry) ,path);
    int i;
    for(i = 0 ;i < DATA_PATH_MAX ;++i) {
        if(0 == strcmp(path ,getDataPath(i))) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo) ,i);
            break;
        }
    }
}

int filelistSetPath(GtkWidget* treeView ,const char* path)
{
	char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeView) ,"path");
	char* fileListSuffix = (char*)g_object_get_data(G_OBJECT(treeView) ,"suffix");

	GtkTreeModel* model = gtk_tree_view_get_model((GtkTreeView*)treeView);
	if(0 == updateList(model ,path ,fileListSuffix))
	{
		if(0 != strcmp(path ,fileListPath))
		{
			memset(fileListPath ,0 ,100);
			getcwd(fileListPath ,100);
			strcat(fileListPath ,"/");
		}
	}
	GtkWidget* combo = (GtkWidget*)g_object_get_data(G_OBJECT(treeView) ,"combo");
	comboUpdateEntry(combo ,fileListPath);
	return 0;
}

int filelistSetSuffix(GtkWidget* treeView ,const char* suffix)
{
	char* fileListSuffix = (char*)g_object_get_data(G_OBJECT(treeView) ,"suffix");
	char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeView) ,"path");
	GtkTreeModel* model = gtk_tree_view_get_model((GtkTreeView*)treeView);
	if(0 == updateList(model ,fileListPath ,suffix))
	{
		memset(fileListSuffix ,0 ,100);
		if(suffix)
			memcpy(fileListSuffix ,suffix ,strlen(suffix) + 1);
	}
	return 0;
}

int filelistSetPathAndSuffix(GtkWidget* treeView ,const char* path ,const char* suffix)
{
	char* fileListSuffix = (char*)g_object_get_data(G_OBJECT(treeView) ,"suffix");
	char* fileListPath = (char*)g_object_get_data(G_OBJECT(treeView) ,"path");
	GtkTreeModel* model = gtk_tree_view_get_model((GtkTreeView*)treeView);
	if(0 == updateList(model ,path ,suffix))
	{
		memset(fileListSuffix ,0 ,100);
		if(suffix)
			memcpy(fileListSuffix ,suffix ,strlen(suffix) + 1);

		if(0 != strcmp(path ,fileListPath))
		{
			memset(fileListPath ,0 ,100);
			getcwd(fileListPath ,100);
			strcat(fileListPath ,"/");
		}
	}
	GtkWidget* combo = (GtkWidget*)g_object_get_data(G_OBJECT(treeView) ,"combo");
	comboUpdateEntry(combo ,fileListPath);
	return 0;
}

void destroyCallback(GtkObject *object ,gpointer data)
{
	char* fileListSuffix = (char*)g_object_get_data(G_OBJECT(object) ,"suffix");
	char* fileListPath = (char*)g_object_get_data(G_OBJECT(object) ,"path");
	free(fileListSuffix);
	free(fileListPath);
}

int fileStat(const char* fileName);
static gboolean rowActivatedCallback(GtkTreeView *tree_view ,GtkTreePath *path,GtkTreeViewColumn *column,gpointer user_data)
{
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
    GtkTreeIter iter;
	gtk_tree_model_get_iter(model ,&iter ,path);
	char* filename;
	gtk_tree_model_get(model ,&iter ,FILELIST_COLUMN_FILENAME ,&filename ,-1);

	char* fileListPath = (char*)g_object_get_data(G_OBJECT(tree_view) ,"path");
	char fullName[256];
	memset(fullName ,0 ,256);
	strcpy(fullName ,fileListPath);
	strcat(fullName ,"/");
	strcat(fullName ,filename);
	free(filename);

	if(0 == fileStat(fullName))
	{
		filelistSetPath(GTK_WIDGET(tree_view) ,fullName);
	}
    return FALSE;
}

GtkWidget* filelistScrollNew()
{
	GtkWidget* scroll;
	scroll = gtk_scrolled_window_new (NULL, NULL);

	GtkListStore* listStore;
	listStore = gtk_list_store_new(FILELIST_COLUMN_MAX ,G_TYPE_STRING ,G_TYPE_STRING);

	GtkWidget* treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(listStore));
	g_object_unref(listStore);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeView), FALSE);

	GtkTreeViewColumn* column ;
	GtkCellRenderer* renderer ;
	renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new_with_attributes("file image",
			renderer, "stock_id", FILELIST_COLUMN_IMAGE, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("file name",
			renderer, "text", FILELIST_COLUMN_FILENAME, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);//*/

	gtk_container_add(GTK_CONTAINER(scroll),treeView);
	g_object_set_data(G_OBJECT(scroll) ,"treeview" ,(gpointer)treeView);

	char* fileListSuffix = (char*)malloc(100);
	char* fileListPath = (char*)malloc(100);
	g_object_set_data(G_OBJECT(treeView) ,"suffix" ,(gpointer)fileListSuffix);
	g_object_set_data(G_OBJECT(treeView) ,"path" ,(gpointer)fileListPath);
	const char* path = gThttpdPath;
	const char* suffix = "";
	filelistSetPathAndSuffix(treeView ,path ,suffix);

	g_object_set_data(G_OBJECT(treeView) ,"rowActivatedCallback" ,rowActivatedCallback);
	g_signal_connect(G_OBJECT(treeView) ,"row-activated" ,G_CALLBACK(rowActivatedCallback) ,NULL);
	g_signal_connect(G_OBJECT(treeView), "destroy", G_CALLBACK(destroyCallback) ,NULL);

	return scroll;
}
