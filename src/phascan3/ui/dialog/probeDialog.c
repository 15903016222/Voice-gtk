/*
 * probeDialog.c
 * 未完，待续，预留
 *  Created on: 2012-8-10
 *      Author: lzk
 */
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <string.h>
#include "../ui.h"
#include "../../string/_string.h"
#include "../../uihandler/handler.h"
#include "../../file_op.h"
#include "../../globalData.h"

extern void webFilePreview(GtkWidget* dialog ,const char* filename);

int getTmpHtmlFile(const char* tmpFileName ,const char* label)
{
    FILE *fp = NULL;

    fp = fopen(tmpFileName,"w+");

    if(fp == NULL)
    {
        return 1;
    }

//    const char* titleFormat = "b";
//    const char* bodyFormat = "i";
//    const char* newLineFormat = " style= \"word-break:break-all \"";

    fprintf(fp,"<html>\n");

    fprintf(fp ,"\t<body>\n");

    fprintf(fp ,"</br>");

    fprintf(fp ,"<p>%s</p>" ,label);

    fprintf(fp ,"\t</body>\n");

    fprintf(fp ,"</html>\n");

    fclose(fp);

    return 0;
}

extern const char** strProbeSelectChange[];
static void ProbeFilePreview(GtkWidget* dialog ,const char* filename)
{
	char tmpFileName[256];
	memset(tmpFileName ,0 ,256);
	sprintf(tmpFileName ,"%s.tmp.html" ,gReportPath);
	if(0 == getTmpHtmlFile(tmpFileName ,strProbeSelectChange[SYSTEM(language)][0]))
	{
		webFilePreview(dialog ,tmpFileName);
	}
}

typedef struct testFile_
{
	const char* filename;
	GtkTreeSelection* selection;
	GtkTreeView* treeView;
	int column;
}
	testFileStruct;

gboolean  testFileName(GtkTreeModel *model,
                       GtkTreePath *path,
                       GtkTreeIter *iter,
                       gpointer data)
{
	testFileStruct* pTt = (testFileStruct*)data;
	if(pTt)
	{
		char* filename;
		gtk_tree_model_get(model ,iter ,pTt->column ,&filename ,-1);
		if(0 == strcmp(pTt->filename ,filename))
		{
			gtk_tree_selection_select_iter(pTt->selection ,iter);
			gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(pTt->treeView) ,path
					,gtk_tree_view_get_column(GTK_TREE_VIEW(pTt->treeView) ,pTt->column) ,FALSE ,0 ,0);
			return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
	return FALSE;
}

void testTreeView(GtkWidget* treeview ,const char* filename ,int column)
{
	testFileStruct tt;
	tt.filename = filename;
	tt.selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	tt.column = column;
	tt.treeView = GTK_TREE_VIEW(treeview);
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_unselect_all(tt.selection);
	gtk_tree_model_foreach(model ,testFileName ,&tt);
}

static void DirPreview(GtkWidget* dialog ,const char* filename)
{
	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	filelistSetPath(treeview ,filename);

	testTreeView(treeview ,"B1S.oup" ,FILELIST_COLUMN_FILENAME);
}

extern GtkWidget* filelistDialogNew(GtkWidget* fatherWidget);
GtkWidget* ProbeDialogNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog = filelistDialogNew(fatherWidget);

	GtkWidget* dirList = g_object_get_data(G_OBJECT(dialog) ,"dirList");
	GtkWidget* treeview = g_object_get_data(G_OBJECT(dirList) ,"treeview");
	filelistSetPathAndSuffix(treeview ,UT_PROBE_PATH ,NULL);
	g_object_set_data(G_OBJECT(treeview) ,"filePreview" ,DirPreview);
	gtk_widget_show(dirList);
	testTreeView(treeview ,"Contact" ,FILELIST_COLUMN_FILENAME);

	GtkWidget* fileList = g_object_get_data(G_OBJECT(dialog) ,"fileList");
	treeview = g_object_get_data(G_OBJECT(fileList) ,"treeview");
	filelistSetPathAndSuffix(treeview ,UT_PROBE_PATH ,".oup");
	g_object_set_data(G_OBJECT(treeview) ,"filePreview" ,ProbeFilePreview);

	return dialog;
}
//*/
