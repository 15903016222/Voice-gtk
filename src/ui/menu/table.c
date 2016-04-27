/*
 * table.c
 *
 *  Created on: 2012-4-11
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include <assert.h>
#include "../../drawui.h"
#include "../../main.h"
#include "../../lzk/fileHandler.h"
#include "../../string/_string.h"
#include "../ui.h"
#include "../../calculation/getMeasureData.h"

static GtkWidget* scrollTable;
#if ARM
extern void saveDrawAreaToImage(const char* filename ,GtkAllocation allocation);
#endif
static const char* defaultComments = "-";
static const char* stringND = "ND";
static char* tableComments;

typedef enum
{
  COLUMN_ENTRY,
  COLUMN_SCAN,
  COLUMN_INDEX,
  COLUMN_GROUP,
  COLUMN_CHANNEL,
  COLUMN_FIELD1,
  COLUMN_FIELD2,
  COLUMN_FIELD3,
  COLUMN_FIELD4,
  COLUMN_FIELD5,
  COLUMN_FIELD6,
  COLUMN_FIELD7,
  COLUMN_FIELD8,
  COLUMN_COMMENTS,
  COLUMN_SHOW_MAX,
  COLUMN_DATA_SELF_DEFINE = COLUMN_SHOW_MAX,
  COLUMN_MAX
}
	COLUMN_ENUM;

void setFieldIndex(int list ,int fieldNO ,unsigned char value)
{
	pp->p_config->fieldIndex[list][fieldNO] = value;
}

unsigned char getFieldIndex(int list ,int fieldNO)
{
	return pp->p_config->fieldIndex[list][fieldNO];
}

char* getTableTitle(int i)
{
	assert(i < COLUMN_SHOW_MAX);

	if((COLUMN_SCAN == i)||(COLUMN_INDEX == i))
	{
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			return g_strdup_printf("%s\n%s" ,getMainDictString(MAINSTRINGDICT_TABLE_TITLE ,i) ,getUnit(1));
		}
		else
		{
			return g_strdup_printf("%s\n%s" ,getMainDictString(MAINSTRINGDICT_TABLE_TITLE ,i) ,getUnit(2));
		}
	}
	if(i < COLUMN_FIELD1)
	{
		return g_strdup_printf("%s" ,getMainDictString(MAINSTRINGDICT_TABLE_TITLE ,i));
	}
	else if(i <= COLUMN_FIELD8)
	{
		int feildNum = GetMesureFeildNum();
        g_message("%s[%d] num(%d)", __func__, __LINE__, feildNum);
		const char* pSimpleStr[feildNum];
		GetMesureFeildString(pSimpleStr ,NULL);
		unsigned char fieldIndex = getFieldIndex((i - COLUMN_FIELD1) / 4 ,(i - COLUMN_FIELD1) % 4);
		return g_strdup_printf("%s\n(%s)" ,pSimpleStr[fieldIndex] ,getMeasureUnit(pp->p_config->groupId ,fieldIndex));
	}
	else
	{
		return g_strdup_printf("%s" ,getMainDictString(MAINSTRINGDICT_TABLE_TITLE ,i - 8));
	}
}

static void callbackSelectionChanged(GtkTreeSelection *treeselection ,gpointer user_data)
{

}

GtkWidget* tableScrollNew()
{
	GtkWidget* scroll;
	scroll = gtk_scrolled_window_new (NULL, NULL);

	GtkListStore* listStore;
	listStore = gtk_list_store_new(COLUMN_MAX
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_UINT
									,G_TYPE_UINT
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_STRING
									,G_TYPE_POINTER
									);

	GtkWidget* treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(listStore));
	g_object_unref(listStore);

	gtk_container_add(GTK_CONTAINER(scroll) ,treeView);
	g_object_set_data(G_OBJECT(scroll) ,"treeview" ,(gpointer)treeView);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll) ,GTK_POLICY_AUTOMATIC ,GTK_POLICY_AUTOMATIC);
	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);


	GtkTreeViewColumn* column ;
	GtkCellRenderer* renderer ;
	renderer = gtk_cell_renderer_text_new();

	int i;
	for(i = COLUMN_ENTRY ;i < COLUMN_SHOW_MAX ;++i)
	{
		char* title = getTableTitle(i);
		column = gtk_tree_view_column_new_with_attributes(title,
				renderer, "text", i, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);
		g_free(title);
	}

	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(callbackSelectionChanged), NULL);

	gtk_widget_set_can_focus(scroll ,FALSE);
	return scroll;
}

void tableInit()
{
	fileInit();
	scrollTable = tableScrollNew();
	gtk_box_pack_start (GTK_BOX(pp->vbox_draw_area) ,scrollTable ,TRUE ,TRUE ,0);
	gtk_widget_show_all(scrollTable);
	gtk_widget_hide(scrollTable);

	tableComments = (char*)defaultComments;
}

void tableDisplay(int show)
{
	if(show)
	{
		if(pp->vboxtable)
		{
			gtk_widget_hide(pp->vboxtable);
		}
		gtk_widget_show(scrollTable);
	}
	else
	{
		if(pp->vboxtable)
		{
			gtk_widget_show(pp->vboxtable);
		}
		gtk_widget_hide(scrollTable);
	}
}

int IsTableDisplay()
{
	return gtk_widget_get_visible(scrollTable);
}

typedef struct
{
	int imageBOOL;
	int field[2][4];
	double fieldValue[2][4];
	double scan;
	double index;
	char* imageFileName;
}
	tableDataSelfDefineStruct;

typedef struct
{
	char* entry;
	char* scan;
	char* index;
	int group;
	int channel;
	char* field[2][4];
	char* comments;
	tableDataSelfDefineStruct* dataSelfDefine;
}
	tableDataStruct;

gboolean treeModelForeachFunc(GtkTreeModel *model ,GtkTreePath *path ,GtkTreeIter *iter ,gpointer data)
{
	tableDataStruct tableData;
	tableData.dataSelfDefine = NULL;
	gtk_tree_model_get(model ,iter
					  ,COLUMN_DATA_SELF_DEFINE		,&tableData.dataSelfDefine
					  ,-1);
	if(tableData.dataSelfDefine)
	{
		int i ;
		int j;
		unsigned char fieldIndex ;

		double factor = 1.0;
		if(get_unit(pp->p_config) == UNIT_INCH)
		{
			factor = 0.03937;
		}
		switch((int)data)
		{
		case COLUMN_ENTRY:
			if(tableData.dataSelfDefine->imageBOOL)
			{
				tableData.entry = g_strdup_printf("%d *" ,*(gtk_tree_path_get_indices(path)) + 1);
			}
			else
			{
				tableData.entry = g_strdup_printf("%d  " ,*(gtk_tree_path_get_indices(path)) + 1);
			}
			gtk_list_store_set (GTK_LIST_STORE(model) ,iter
							  ,data		,tableData.entry
							  ,-1);
			free(tableData.entry);
			break;
		case COLUMN_SCAN:
			tableData.scan = g_strdup_printf("%0.1f" ,tableData.dataSelfDefine->scan * factor);
			gtk_list_store_set (GTK_LIST_STORE(model) ,iter
							  ,data		,&tableData.scan
							  ,-1);
			free(tableData.scan);
			break;
		case COLUMN_INDEX:
			tableData.index = g_strdup_printf("%0.1f" ,tableData.dataSelfDefine->index * factor);
			gtk_list_store_set (GTK_LIST_STORE(model) ,iter
							  ,data		,tableData.index
							  ,-1);
			free(tableData.index);
			break;
		case COLUMN_GROUP:
		case COLUMN_CHANNEL:
			break;
		case COLUMN_FIELD1:
		case COLUMN_FIELD2:
		case COLUMN_FIELD3:
		case COLUMN_FIELD4:
		case COLUMN_FIELD5:
		case COLUMN_FIELD6:
		case COLUMN_FIELD7:
		case COLUMN_FIELD8:
			i = ((int)data - COLUMN_FIELD1) / 4;
			j = ((int)data - COLUMN_FIELD1) % 4;
			fieldIndex = getFieldIndex(i ,j);
			tableData.field[i][j] = g_strdup_printf("%s" ,stringND);
			int k,l;
			for(k = 0 ;k < 2 ;++k)
			{
				for(l = 0;l < 4; ++l)
				{
					if(fieldIndex == tableData.dataSelfDefine->field[k][l])
					{
						free(tableData.field[i][j]);
						if(MEASURE_DATA_ND == tableData.dataSelfDefine->fieldValue[i][j])
						{
							tableData.field[i][j] = g_strdup_printf("%s" ,stringND);
						}
						else
						{
							tableData.field[i][j] = g_strdup_printf("%0.1f" ,tableData.dataSelfDefine->fieldValue[i][j]);
						}
						goto FOUND_OUT;
					}
				}
			}
FOUND_OUT:	gtk_list_store_set (GTK_LIST_STORE(model) ,iter
							  ,data		,tableData.field[i][j]
							  ,-1);
			free(tableData.field[i][j]);
			break;
		case COLUMN_COMMENTS:
		default:
			break;
		}
	}
	return FALSE;
}

void updateColumn(int columnEnum)
{
	assert(columnEnum < COLUMN_SHOW_MAX);
	GtkTreeView* treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable) ,"treeview"));
	GtkTreeViewColumn* column = gtk_tree_view_get_column(treeView ,columnEnum);
	GtkTreeModel* model = gtk_tree_view_get_model(treeView);
	char* title;
	unsigned char fieldIndex;
	switch(columnEnum)
	{
	case COLUMN_ENTRY:
		gtk_tree_model_foreach(model ,treeModelForeachFunc ,(gpointer)columnEnum);
		break;
	case COLUMN_SCAN:
	case COLUMN_INDEX:
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			title = g_strdup_printf("%s\n%s" ,getMainDictString(MAINSTRINGDICT_TABLE_TITLE ,columnEnum) ,getUnit(1));
		}
		else
		{
			title = g_strdup_printf("%s\n%s" ,getMainDictString(MAINSTRINGDICT_TABLE_TITLE ,columnEnum) ,getUnit(2));
		}
		gtk_tree_view_column_set_title(column ,title);
		g_free(title);

		gtk_tree_model_foreach(model ,treeModelForeachFunc ,(gpointer)columnEnum);
		break;
	case COLUMN_GROUP:
	case COLUMN_CHANNEL:
		break;
	case COLUMN_FIELD1:
	case COLUMN_FIELD2:
	case COLUMN_FIELD3:
	case COLUMN_FIELD4:
	case COLUMN_FIELD5:
	case COLUMN_FIELD6:
	case COLUMN_FIELD7:
	case COLUMN_FIELD8:
		fieldIndex = getFieldIndex((columnEnum - COLUMN_FIELD1) / 4 ,(columnEnum - COLUMN_FIELD1) % 4);
		title = g_strdup_printf("%s\n(%s)" ,pp->field[fieldIndex] ,pp->field_unit[fieldIndex]);
		gtk_tree_view_column_set_title(column ,title);
		g_free(title);

		gtk_tree_model_foreach(model ,treeModelForeachFunc ,(gpointer)columnEnum);
		break;
	case COLUMN_COMMENTS:
	default:
		break;
	}
}

void getTableComments()
{
	char* tmpComments;
	MENU_UNION menu_union = {{3 ,2 ,5 ,0}};
	tmpComments = dialogGetAString(pp->window ,getMainLabelString(menu_union) ,tableComments);
	if(tmpComments)
	{
		tableComments = tmpComments;
	}
}

static void deletePath(GtkTreeModel* model ,GtkTreeIter* iter ,tableDataStruct* pTableData)
{
	pTableData->dataSelfDefine = NULL;
	gtk_tree_model_get(GTK_TREE_MODEL(model) ,iter
					  ,COLUMN_DATA_SELF_DEFINE 		,&pTableData->dataSelfDefine
					  ,-1);
	if(pTableData->dataSelfDefine)
	{
		if(pTableData->dataSelfDefine->imageFileName)
		{
			char imageFullFileName[256];
			memset(imageFullFileName ,0 ,256);
			sprintf(imageFullFileName ,"%s%s" ,gTmpReportImagePath ,pTableData->dataSelfDefine->imageFileName);
			fileDelete(imageFullFileName ,"-f");
			free(pTableData->dataSelfDefine->imageFileName);
		}
		free(pTableData->dataSelfDefine);
	}
	gtk_list_store_remove(GTK_LIST_STORE(model) ,iter);
}

int tableHasEntry()
{
	GtkTreeView* treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable) ,"treeview"));
	GtkTreeModel* model = gtk_tree_view_get_model(treeView);
	GtkTreeIter iter;
	return gtk_tree_model_get_iter_first(model ,&iter);
}

void getTableEntryData(tableDataStruct* pTableData ,int enableIamge)
{
	double factor = 1.0;
	if(get_unit(pp->p_config) != UNIT_MM)
	{
		factor = 0.03937;
	}

	int i ,j;
	pTableData->group = pp->p_config->groupId + 1;
	pTableData->channel = TMP(beam_num[pp->p_config->groupId]) + 1;

	pTableData->dataSelfDefine = malloc(sizeof(tableDataSelfDefineStruct));

	pTableData->dataSelfDefine->scan = TMP(scan) / 100.0;
	pTableData->dataSelfDefine->index = TMP(index) / 100.0;

	pTableData->scan = g_strdup_printf("%0.1f" ,pTableData->dataSelfDefine->scan * factor);
	pTableData->index = g_strdup_printf("%0.1f" ,pTableData->dataSelfDefine->index * factor);

	for(i = 0 ;i < 2 ;++i)
	{
		for(j = 0 ;j < 4 ;++j)
		{
			unsigned char fieldIndex = getFieldIndex(i ,j);
			pTableData->dataSelfDefine->fieldValue[i][j] = TMP(field[i][j]);
			if(MEASURE_DATA_ND == pTableData->dataSelfDefine->fieldValue[i][j])
			{
				pTableData->field[i][j] = g_strdup_printf("%s" ,stringND);
			}
			else
			{
				pTableData->field[i][j] = g_strdup_printf("%0.1f" ,pTableData->dataSelfDefine->fieldValue[i][j]);
			}
			pTableData->dataSelfDefine->field[i][j] = fieldIndex;
		}
	}

//有图片生成图片，保存图片路径
	pTableData->dataSelfDefine->imageBOOL = enableIamge;
	pTableData->dataSelfDefine->imageFileName = NULL;
	if(pTableData->dataSelfDefine->imageBOOL)
	{
		GtkAllocation allocation;
		gtk_widget_get_allocation(pp->vbox_draw_area ,&allocation);
		int i;
		char imageFullFileName[256];
		for(i = 0 ; ;++i)
		{
			pTableData->dataSelfDefine->imageFileName = g_strdup_printf("table%d.jpg" ,i);
			memset(imageFullFileName ,0 ,256);
			sprintf(imageFullFileName ,"%s%s" ,gTmpReportImagePath ,pTableData->dataSelfDefine->imageFileName);
			if(0 == access(imageFullFileName ,F_OK))
			{
				free(pTableData->dataSelfDefine->imageFileName);
				continue;
			}
			else
			{
				break;
			}
		}

#if ARM
		saveDrawAreaToImage(imageFullFileName ,allocation);
#else
		char cmd[256];
		memset(cmd ,0 ,256);
		sprintf(cmd ,"echo \"Hello World\" > %s" ,imageFullFileName);
		system(cmd);
#endif
	}
}

void deleteTableTmpData(tableDataStruct* pTableData)
{
	int i ,j;
	//删除临时数据
	if(pTableData->entry)
	{
		free(pTableData->entry);
		pTableData->entry = NULL;
	}
	if(pTableData->scan)
	{
		free(pTableData->scan);
		pTableData->scan = NULL;
	}
	if(pTableData->index)
	{
		free(pTableData->index);
		pTableData->index = NULL;
	}

	for(i = 0 ;i < 2 ;++i)
	{
		for(j = 0 ;j < 4 ;++j)
		{
			if(pTableData->field[i][j])
			{
				g_free(pTableData->field[i][j]);
				pTableData->field[i][j] = NULL;
			}
		}
	}

	//g_debug("%s" ,tableComments);
	if(tableComments != defaultComments)
	{
		free(tableComments);
		tableComments = (char*)defaultComments;
	}
//	if((pTableData->comments) && (pTableData->comments != defaultComments))
//	{
//		free(pTableData->comments);
//		pTableData->comments = NULL;
//	}
}

void tableAddEntry()
{
	GtkTreeView* treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable) ,"treeview"));
	GtkListStore* store = GTK_LIST_STORE(gtk_tree_view_get_model(treeView));
	GtkTreeIter iter;

	gtk_list_store_prepend(store, &iter);
	GtkTreePath* path = gtk_tree_model_get_path(GTK_TREE_MODEL(store) ,&iter);
	tableDataStruct tableData ,*pTableData = &tableData;
	memset(pTableData ,0 ,sizeof(tableDataStruct));

	getTableEntryData(pTableData ,get_entry_image(pp->p_config));

	if(pTableData->dataSelfDefine->imageBOOL)
	{
		pTableData->entry = g_strdup_printf("%d *" ,*(gtk_tree_path_get_indices(path)) + 1);
	}
	else
	{
		pTableData->entry = g_strdup_printf("%d  " ,*(gtk_tree_path_get_indices(path)) + 1);
	}
	pTableData->comments = tableComments;

	gtk_list_store_set (store, &iter
					  ,COLUMN_ENTRY				,tableData.entry
					  ,COLUMN_SCAN				,tableData.scan
					  ,COLUMN_INDEX				,tableData.index
					  ,COLUMN_GROUP				,tableData.group
					  ,COLUMN_CHANNEL			,tableData.channel
					  ,COLUMN_FIELD1    		,tableData.field[0][0]
					  ,COLUMN_FIELD2    		,tableData.field[0][1]
					  ,COLUMN_FIELD3    		,tableData.field[0][2]
					  ,COLUMN_FIELD4    		,tableData.field[0][3]
					  ,COLUMN_FIELD5    		,tableData.field[1][0]
					  ,COLUMN_FIELD6    		,tableData.field[1][1]
					  ,COLUMN_FIELD7    		,tableData.field[1][2]
					  ,COLUMN_FIELD8    		,tableData.field[1][3]
					  ,COLUMN_COMMENTS			,tableData.comments
					  ,COLUMN_DATA_SELF_DEFINE 	,tableData.dataSelfDefine
					,-1);
	updateColumn(COLUMN_ENTRY);
	GtkTreeSelection* selection = gtk_tree_view_get_selection(treeView);
	gtk_tree_selection_select_iter(selection ,&iter);

//删除临时数据
	deleteTableTmpData(pTableData);

//多余10条记录，删除记录和保存的数据
	if(gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(store) ,&iter ,"10"))
	{
		deletePath(GTK_TREE_MODEL(store) ,&iter ,&tableData);
	}
}

void tableDeleteEntry()
{
	GtkTreeView* treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable) ,"treeview"));
	GtkTreeSelection* selection = gtk_tree_view_get_selection(treeView);
	GtkTreeModel* model = gtk_tree_view_get_model(treeView);
	GtkTreeIter iter;
	tableDataStruct tableData;
	tableData.dataSelfDefine = NULL;
	if(gtk_tree_selection_get_selected(selection ,&model ,&iter))
	{
		deletePath(model ,&iter ,&tableData);

		gtk_tree_model_foreach(model ,treeModelForeachFunc ,NULL);
		if(gtk_tree_model_get_iter_first(model ,&iter))
			gtk_tree_selection_select_iter(selection ,&iter);
	}

}

void tableSelect(int entry)
{
	assert(entry > 0);
	GtkTreeView* treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable) ,"treeview"));
	GtkTreeSelection* selection = gtk_tree_view_get_selection(treeView);
	GtkTreeModel* model = gtk_tree_view_get_model(treeView);
	GtkTreeIter iter;
	char path[10];
	memset(path ,0 ,10);
	sprintf(path ,"%d" ,entry - 1);
	if(gtk_tree_model_get_iter_from_string(model ,&iter ,path))
	{
		gtk_tree_selection_select_iter(selection ,&iter);
	}
}

void fprintfTableTitle(FILE* fp ,int enableBit)
{
	int i;
	char str[100];
	char* title;
	fprintf(fp ,"\t\t\t<tr>\n");
	for(i = COLUMN_ENTRY ;i < COLUMN_SHOW_MAX ;++i)
	{
		if(0x01 == ((enableBit >> i) & 0x01))
		{
			memset(str ,0 ,100);
			title = getTableTitle(i);
            g_message("%s[%d] title(%s)", __func__, __LINE__, title);
			changeNewLine(str ,title);
			g_free(title);
			if(COLUMN_COMMENTS == i)
			{

				fprintf(fp ,"\t\t\t<th colspan=14 align=\"left\">%s</th>\n" ,str);
			}
			else
			{
				fprintf(fp ,"\t\t\t<th>%s</th>\n" ,str);
			}

		}
	}

	fprintf(fp ,"\t\t\t</tr>\n");
}

void fprintfEntry(FILE* fp ,int enableBit ,tableDataStruct* pTableData)
{
	int i ,j ,k;
//    const char* titleFormat = "b";
	const char* bodyFormat = "i";
	const char* newLineFormat = " align=\"center\" style=\"word-break:break-all; word-wrap:break-word;\"";

	fprintf(fp ,"\t\t\t<tr>\n");

	for(i = COLUMN_ENTRY ;i < COLUMN_SHOW_MAX ;++i)
	{
		if(0x01 == ((enableBit >> i) & 0x01))
		{
			switch(i)
			{
			case COLUMN_ENTRY:
				fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
						,newLineFormat ,bodyFormat ,pTableData->entry,bodyFormat);
				break;
			case COLUMN_SCAN:
				fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
						,newLineFormat ,bodyFormat ,pTableData->scan,bodyFormat);
				break;
			case COLUMN_INDEX:
				fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
						,newLineFormat ,bodyFormat ,pTableData->index,bodyFormat);
				break;
			case COLUMN_GROUP:
				fprintf(fp ,"\t\t\t<td%s><%s>%d</%s></td>\n"
						,newLineFormat ,bodyFormat ,pTableData->group,bodyFormat);
				break;
			case COLUMN_CHANNEL:
				fprintf(fp ,"\t\t\t<td%s><%s>%d</%s></td>\n"
						,newLineFormat ,bodyFormat ,pTableData->channel,bodyFormat);
				break;
			case COLUMN_FIELD1:
			case COLUMN_FIELD2:
			case COLUMN_FIELD3:
			case COLUMN_FIELD4:
			case COLUMN_FIELD5:
			case COLUMN_FIELD6:
			case COLUMN_FIELD7:
			case COLUMN_FIELD8:
				j = (i - COLUMN_FIELD1) / 4;
				k = (i - COLUMN_FIELD1) % 4;
				fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
						,newLineFormat ,bodyFormat ,pTableData->field[j][k] ,bodyFormat);
				break;
			case COLUMN_COMMENTS:
				fprintf(fp ,"\t\t\t<td colspan=14 align=\"left\" style=\"word-break:break-all; word-wrap:break-word;\"><%s>%s</%s></td>\n"
						,bodyFormat ,pTableData->comments,bodyFormat);
				break;
			default:
				break;
			}
		}
	}
	fprintf(fp ,"\t\t\t</tr>\n");
}

gboolean treeModelForeachBody(GtkTreeModel *model ,GtkTreePath *path ,GtkTreeIter *iter ,gpointer data)
{
	tableDataStruct tableData;
	memset(&tableData ,0 ,sizeof(tableDataStruct));
	gtk_tree_model_get(model ,iter
					  ,COLUMN_ENTRY		,&tableData.entry
					  ,COLUMN_SCAN		,&tableData.scan
					  ,COLUMN_INDEX		,&tableData.index
					  ,COLUMN_GROUP		,&tableData.group
					  ,COLUMN_CHANNEL	,&tableData.channel
					  ,COLUMN_FIELD1    ,&tableData.field[0][0]
					  ,COLUMN_FIELD2    ,&tableData.field[0][1]
					  ,COLUMN_FIELD3    ,&tableData.field[0][2]
					  ,COLUMN_FIELD4    ,&tableData.field[0][3]
					  ,COLUMN_FIELD5    ,&tableData.field[1][0]
					  ,COLUMN_FIELD6    ,&tableData.field[1][1]
					  ,COLUMN_FIELD7    ,&tableData.field[1][2]
					  ,COLUMN_FIELD8    ,&tableData.field[1][3]
					  ,COLUMN_COMMENTS	,&tableData.comments
					  ,-1);

    FILE* fp = (FILE*)data;
    int enableBit = 0;
    int i;
    for(i = COLUMN_ENTRY ;i < COLUMN_SHOW_MAX ;++i)
    {
    	if(COLUMN_COMMENTS != i)
    		enableBit |= 1 << i;
    }
    fprintfEntry(fp ,enableBit ,&tableData);
    deleteTableTmpData(&tableData);
	return FALSE;
}

extern const char* tableWidth;
extern const char* sonTableStyle;
gboolean treeModelForeachImage(GtkTreeModel *model ,GtkTreePath *path ,GtkTreeIter *iter ,gpointer data)
{
	tableDataStruct tableData;
	memset(&tableData ,0 ,sizeof(tableDataStruct));
	gtk_tree_model_get(model ,iter
					  ,COLUMN_ENTRY				,&tableData.entry
					  ,COLUMN_SCAN				,&tableData.scan
					  ,COLUMN_INDEX				,&tableData.index
					  ,COLUMN_GROUP				,&tableData.group
					  ,COLUMN_CHANNEL			,&tableData.channel
					  ,COLUMN_FIELD1    		,&tableData.field[0][0]
					  ,COLUMN_FIELD2    		,&tableData.field[0][1]
					  ,COLUMN_FIELD3    		,&tableData.field[0][2]
					  ,COLUMN_FIELD4    		,&tableData.field[0][3]
					  ,COLUMN_FIELD5    		,&tableData.field[1][0]
					  ,COLUMN_FIELD6    		,&tableData.field[1][1]
					  ,COLUMN_FIELD7    		,&tableData.field[1][2]
					  ,COLUMN_FIELD8    		,&tableData.field[1][3]
					  ,COLUMN_COMMENTS			,&tableData.comments
					  ,COLUMN_DATA_SELF_DEFINE 	,&tableData.dataSelfDefine
					  ,-1);

	if(tableData.dataSelfDefine->imageBOOL)
	{
		reportParaStruct* pPara = (reportParaStruct*)data;
		FILE* fp = pPara->fp;
		fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

	    int enableBit = 0;
	    int i;
	    for(i = COLUMN_ENTRY ;i < COLUMN_SHOW_MAX ;++i)
	    {
	    	if(COLUMN_COMMENTS != i)
	    		enableBit |= 1 << i;
	    }
	    fprintfTableTitle(fp ,enableBit);
	    fprintfEntry(fp ,enableBit ,&tableData);

		enableBit = 1 << COLUMN_COMMENTS;
	    fprintfTableTitle(fp ,enableBit);
	    fprintfEntry(fp ,enableBit ,&tableData);

		fprintf(fp,"</table></td></tr>\n</table>\n\n");//table

		char imageFileName[100];
		memset(imageFileName ,0 ,100);
		GetDirName(imageFileName ,pPara->imageDirName);
		fprintf(fp ,"\t\t<img src=\"%s/%s\" width=690>\n" ,imageFileName ,tableData.dataSelfDefine->imageFileName);

		sprintf(imageFileName ,"%s/%s" ,pPara->imageDirName ,tableData.dataSelfDefine->imageFileName);
//		if(!pPara->isTmp)
		{
			char cmd[256];
			memset(cmd ,0 ,256);
			sprintf(cmd ,"cp %s%s %s" ,gTmpReportImagePath ,tableData.dataSelfDefine->imageFileName ,pPara->imageDirName);
			system(cmd);
			system("sync");
		}
	}
	deleteTableTmpData(&tableData);
	return FALSE;
}

void TableReport(reportParaStruct* pPara)
{
	GtkTreeView* treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable) ,"treeview"));
	GtkTreeModel* model = gtk_tree_view_get_model(treeView);
//	GtkTreeIter iter;
//	if(gtk_tree_model_get_iter_first(model ,&iter))
//	if(tableHasEntry())
	{
		FILE* fp = pPara->fp;

		fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

	    int enableBit = 0;
	    int i;
	    for(i = COLUMN_ENTRY ;i < COLUMN_SHOW_MAX ;++i)
	    {
	    	if(COLUMN_COMMENTS != i)
	    		enableBit |= 1 << i;
	    }

        fprintfTableTitle(fp ,enableBit);

		gtk_tree_model_foreach(model ,treeModelForeachBody ,fp);

		fprintf(fp,"</table></td></tr>\n</table>\n\n");//table

		fprintf(fp ,"\t\t</br>\n");

		gtk_tree_model_foreach(model ,treeModelForeachImage ,pPara);
	}

}

void CurrentViewReport(reportParaStruct* pPara)
{
	FILE* fp = pPara->fp;

	tableDataStruct tableData;
	memset(&tableData ,0 ,sizeof(tableDataStruct));

	getTableEntryData(&tableData ,TRUE);

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    int enableBit = 0;
    int i;
    for(i = COLUMN_FIELD1 ;i <= COLUMN_FIELD8 ;++i)
    {
    	enableBit |= 1 << i;
    }
    fprintfTableTitle(fp ,enableBit);
    fprintfEntry(fp ,enableBit ,&tableData);

	deleteTableTmpData(&tableData);

	fprintf(fp,"</table></td></tr>\n</table>\n\n");//table

	char imageFileName[100];
	memset(imageFileName ,0 ,100);
	GetDirName(imageFileName ,pPara->imageDirName);
	fprintf(fp ,"\t\t<img src=\"%s/%s\" width=690>\n" ,imageFileName ,tableData.dataSelfDefine->imageFileName);
//	if(!pPara->isTmp)
	{
		char cmd[256];
		memset(cmd ,0 ,256);
		sprintf(cmd ,"cp %s%s %s" ,gTmpReportImagePath ,tableData.dataSelfDefine->imageFileName ,pPara->imageDirName);
		system(cmd);
		system("sync");
	}
}

void fprintfReportStart(FILE* fp);
void fprintfReportEnd(FILE* fp);
int SaveTableFile(const char* filename ,int isTmp)
{

    FILE *fp = NULL;

    fp = fopen(filename ,"w+");

    if(fp == NULL)
    {
        return 1;
    }

    reportParaStruct para;
    reportParaStruct* pPara = &para;
	char imageDirName[100];
	memset(imageDirName ,0 ,100);
	pPara->fp = fp;
	MkImageDirName(imageDirName ,filename);
	pPara->imageDirName = imageDirName;
	pPara->isTmp = isTmp;

	fprintfReportStart(fp);

    TableReport(pPara);

    fprintfReportEnd(fp);

    fclose(fp);
    system("sync");
    system("sync");
    return 0;
}

GtkWidget* tableExportNew(GtkWidget* fatherWidget);
void tableExport()
{
	SaveTableFile(gTmpTableFileName ,TRUE);
	GtkWidget* dialogSave = tableExportNew(pp->window);
	gtk_widget_show(dialogSave);
}

static gboolean treeModelForeach(GtkTreeModel *model ,GtkTreePath *path ,GtkTreeIter *iter ,gpointer data)
{
    ReportDefect *defect = report_defect_new();
    gint group = 0;
    gint channel = 0;

    gtk_tree_model_get(model ,iter,
                       COLUMN_SCAN,     &defect->scan,
                       COLUMN_INDEX,    &defect->index,
                       COLUMN_GROUP,    &group,
                       COLUMN_CHANNEL,  &channel,
                       COLUMN_FIELD1,   &defect->fieldValues[0],
                       COLUMN_FIELD2,   &defect->fieldValues[1],
                       COLUMN_FIELD3,   &defect->fieldValues[2],
                       COLUMN_FIELD4,   &defect->fieldValues[3],
                       COLUMN_FIELD5,   &defect->fieldValues[4],
                       COLUMN_FIELD6,   &defect->fieldValues[5],
                       COLUMN_FIELD7,	&defect->fieldValues[6],
                       COLUMN_FIELD8,   &defect->fieldValues[7],
                       COLUMN_COMMENTS, &defect->comments,
                       -1);
    report_defect_set_group(defect, group);
    report_defect_set_channel(defect, channel);

    report_defects_add_defect((ReportDefects *)data, defect);

    return FALSE;
}

void filling_report_defects(Report *r)
{
    ReportDefects *defects = report_defects_new();
    gint i = 0;

    for ( i = COLUMN_FIELD1; i <= COLUMN_FIELD8; ++i) {
        report_defects_set_field_name(defects, i-COLUMN_FIELD1, getTableTitle(i));
    }

    GtkTreeView *treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable), "treeview"));
    GtkTreeModel *model = gtk_tree_view_get_model(treeView);
    gtk_tree_model_foreach(model, treeModelForeach, defects);

    r->defects = defects;
}
