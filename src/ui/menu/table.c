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
#include "../../globalData.h"

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

static void filling_report_defects(Report *r)
{
    ReportDefects *defects = report_defects_new();
    gint i = 0;

    for ( i = COLUMN_FIELD1; i <= COLUMN_FIELD8; ++i) {
        report_defects_set_field_name(defects, i-COLUMN_FIELD1, getTableTitle(i));
    }

    GtkTreeView *treeView = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(scrollTable), "treeview"));
    GtkTreeModel *model = gtk_tree_view_get_model(treeView);
    gtk_tree_model_foreach(model, treeModelForeach, defects);

    report_set_defects(r, defects);
}

static inline void filling_report_header(Report *r, const gchar *outputFile)
{
    ReportHeader *hdr = report_header_new();
    report_header_set_device_type(hdr, "Phascan");
    report_header_set_report_file(hdr, outputFile);
    report_header_set_setup_file(hdr, gData->file.setupfile);
    report_header_set_save_mode(hdr, menu_content[SAVE_MODE+get_file_save_mode (pp->p_config)] );
    report_set_header(r, hdr);
}

static void filling_report_users(Report *r)
{
    gint i = 0;
    ReportUsers *users = report_users_new();
    ReportUser *user = NULL;
    for ( ; i < 10; ++i) {
        if (get_report_userfield_enable(pp->p_config, i)) {
            user = report_user_new();
            report_user_set_name(user, get_report_userfield_label(pp->p_config, i) );
            report_user_set_content(user, get_report_userfield_content(pp->p_config, i) );
            report_users_add_user(users, user);
        }
    }
    report_set_users(r, users);
}

static void filling_report_group_probe(ReportGroup *reportGroup, PROBE *probe, gint groupNo)
{
    ReportProbe *reportProbe = report_probe_new();

    report_probe_set_model(reportProbe, probe->Model);
    report_probe_set_serial(reportProbe, probe->Serial);
    report_probe_set_freq(reportProbe, probe->Frequency*0.001);
    report_probe_set_aperture(reportProbe, LAW_VAL_POS(groupNo, Elem_qty));

    report_group_set_probe(reportGroup, reportProbe);
}

static void filling_report_group_wedge(ReportGroup *reportGroup, WEDGE *w)
{
    ReportWedge *reportWedge = report_wedge_new();

    report_wedge_set_model(reportWedge, w->Model);
    report_wedge_set_angle(reportWedge, w->Angle*0.1);

    report_group_set_wedge(reportGroup, reportWedge);
}

static void filling_report_group_ttf(ReportGroup *reportGroup, struct fftStruct *fft)
{
    ReportFFT *reportFFT = report_fft_new();

    report_fft_set_peak_freq(reportFFT, fft->peakPoint * fft->hzPerPoint);

    /* 6dB */
    report_fft_set_lower_freq_6dB(reportFFT, fft->db6minPoint * fft->hzPerPoint);
    report_fft_set_higher_freq_6dB(reportFFT, fft->db6maxPoint * fft->hzPerPoint);
    report_fft_set_center_freq_6dB(reportFFT, FftGetCenterFreq(fft, 6));
    report_fft_set_bandwidth_6dB(reportFFT, (fft->db6maxPoint - fft->db6minPoint) * fft->hzPerPoint);
    report_fft_set_bandwidth_percent_6dB(reportFFT, (fft->db6maxPoint > fft->db6minPoint) ? (fft->peakPoint * 100.0 / (fft->db6maxPoint - fft->db6minPoint)):0);

    /* 20dB */
    report_fft_set_lower_freq_20dB(reportFFT, fft->db20minPoint * fft->hzPerPoint);
    report_fft_set_higher_freq_20dB(reportFFT, fft->db20maxPoint * fft->hzPerPoint);
    report_fft_set_center_freq_20dB(reportFFT, FftGetCenterFreq(fft, 20));
    report_fft_set_bandwidth_20dB(reportFFT, (fft->db20maxPoint - fft->db20minPoint) * fft->hzPerPoint);
    report_fft_set_bandwidth_percent_20dB(reportFFT, (fft->db20maxPoint > fft->db20minPoint) ? (fft->peakPoint * 100.0 / (fft->db20maxPoint - fft->db20minPoint)):0);

    report_group_set_fft(reportGroup, reportFFT);
}

static void filling_report_group_setup(ReportGroup *reportGroup, gint groupNo)
{
    GROUP *grp = get_group_by_id(pp->p_config, groupNo);
    ReportSetup *setup = report_setup_new();

    gint velocity = get_group_val(grp, GROUP_VELOCITY);
    gdouble dbVelocity = velocity / 200000.0;

    report_setup_set_beam_delay(setup, GROUP_VAL_POS(groupNo , beam_delay[TMP(beam_num[groupNo])]) * 0.001 );
    report_setup_set_half_path_start( setup, group_get_start (groupNo) * 0.001 * dbVelocity );
    report_setup_set_half_path_range( setup, group_get_range (groupNo) * 0.001 * dbVelocity );
    report_setup_set_prf(setup, GROUP_VAL_POS(0 , prf1) / 10);

    report_setup_set_inspection_type(setup, menu_content[ GROUP_MODE_P +(GROUP_VAL_POS(groupNo, group_mode))]);

    report_setup_set_averaging_factor(setup, 1 << get_group_val (get_group_by_id (pp->p_config, groupNo), GROUP_AVERAGING));

    if (PA_SCAN == group_get_mode(groupNo)
            || UT_SCAN == group_get_mode(groupNo)) {
        report_setup_set_voltage(setup, menu_content[PA_VOLTAGE + 2 + get_voltage (pp->p_config, groupNo)]);
    } else {
        report_setup_set_voltage(setup, menu_content[UT_VOLTAGE + 3 + get_voltage (pp->p_config, groupNo)]);
    }

    /* Gain */
    report_setup_set_gain(setup, group_get_gain(groupNo)/100.0);

    /* Tx/Rx Mode */
    report_setup_set_rx_tx_mode(setup, menu_content[ TX_RX_MODE + group_get_rx_tx_mode(groupNo)]);

    /* Wave Type */
    gint material = get_part_material(groupNo);
    gint sw = velocity > get_material_sw(material) ? velocity - get_material_sw(material) :get_material_sw(material) - velocity;
    gint lw = velocity > get_material_lw(material) ? velocity - get_material_lw(material) :get_material_lw(material) - velocity;
    if (sw < lw) {
        report_setup_set_wave_type(setup, getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,1));
    } else {
        report_setup_set_wave_type(setup, getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,0));
    }

    /* Sound Velocity */
    report_setup_set_sound_velocity(setup, get_group_val (grp, GROUP_VELOCITY) * 0.01);

    /* Pulse Width */
    report_setup_set_pulse_width(setup, get_group_val (grp, GROUP_PW_VAL) / (gdouble)PW_DIV);

    /* Scan Offset */
    report_setup_set_scan_offset(setup, GROUP_VAL_POS(groupNo, scan_offset) * 0.1);

    /* Index Offset */
    report_setup_set_index_offset(setup, GROUP_VAL_POS(groupNo,index_offset) * 0.1);

    /* Skew */
    report_setup_set_skew(setup, GROUP_VAL_POS(groupNo, skew) * 0.01);

    /* Gate A */
    ReportGate *gateA = report_gate_new();
    report_gate_set_start(gateA, pp->p_config->group[groupNo].gate[0].start * 0.001 * dbVelocity);
    report_gate_set_width(gateA, pp->p_config->group[groupNo].gate[0].width * 0.001 * dbVelocity);
    report_gate_set_threshold(gateA, pp->p_config->group[groupNo].gate[0].height);
    report_gate_set_synchro(gateA, menu_content[SYNCHRO + pp->p_config->group[groupNo].gate[0].synchro]);
    report_setup_set_gate_a(setup, gateA);

    /* Gate B */
    ReportGate *gateB = report_gate_new();
    report_gate_set_start(gateB, pp->p_config->group[groupNo].gate[1].start * 0.001 * dbVelocity);
    report_gate_set_width(gateB, pp->p_config->group[groupNo].gate[1].width * 0.001 * dbVelocity);
    report_gate_set_threshold(gateB, pp->p_config->group[groupNo].gate[1].height);
    report_gate_set_synchro(gateB, menu_content[SYNCHRO + pp->p_config->group[groupNo].gate[1].synchro]);
    report_setup_set_gate_b(setup, gateB);

    /* Gate I */
    ReportGate *gateI = report_gate_new();
    report_gate_set_start(gateI, pp->p_config->group[groupNo].gate[2].start * 0.001 * dbVelocity);
    report_gate_set_width(gateI, pp->p_config->group[groupNo].gate[2].width * 0.001 * dbVelocity);
    report_gate_set_threshold(gateI, pp->p_config->group[groupNo].gate[2].height);
    report_gate_set_synchro(gateI, menu_content[SYNCHRO + pp->p_config->group[groupNo].gate[2].synchro]);
    report_setup_set_gate_i(setup, gateI);


    report_group_set_setup(reportGroup, setup);
}

static void filling_report_group_law(ReportGroup *reportGroup, gint groupNo)
{
    ReportLaw *law = report_law_new();

    gint rxTxMode = group_get_rx_tx_mode(groupNo);

    /* Elemnt Qty */
    report_law_set_element_qty(law, LAW_VAL_POS(groupNo, Elem_qty));

    /* Tx Element */
    report_law_set_first_tx_element(law, LAW_VAL_POS(groupNo,First_tx_elem));
    report_law_set_last_tx_element(law, LAW_VAL_POS(groupNo, Last_tx_elem));

    /* Rx Element */
    switch( rxTxMode ) {
    case 0: //PC
    case 2: //TT
        report_law_set_first_rx_element(law, LAW_VAL_POS(groupNo,First_rx_elem));
        report_law_set_last_rx_element(law, LAW_VAL_POS(groupNo, Last_rx_elem));
        break;
    case 1: //PE
    case 3: //TOFD
    default:
        break;
    }

    /* Element Resolution */
    if (LINEAR_SCAN == LAW_VAL_POS(groupNo, Focal_type)) {
        report_law_set_element_resolution(law, LAW_VAL_POS(groupNo,Elem_step));
    }

    /* Angle */
    report_law_set_start_angle(law, LAW_VAL_POS(groupNo, Angle_min) * 0.01);
    if(AZIMUTHAL_SCAN == LAW_VAL_POS(groupNo , Focal_type)) {
        report_law_set_stop_angle(law, LAW_VAL_POS(groupNo, Angle_max) * 0.01);
        report_law_set_angle_resolution(law, LAW_VAL_POS(groupNo, Angle_step)  * 0.01);
    }

    /* Law Type */
    report_law_set_type(law, menu_content[L_CONFIG + LAW_VAL_POS(groupNo, Focal_type)]);

    /* Focal Type */
    report_law_set_focal_type(law, menu_content[FOCAL_POINT_TYPE1 + LAW_VAL_POS(groupNo, Focal_point_type)]);

    /* FocalFiledNames */
    REPORTSTRINGDICT_ENUM strIndex;
    switch(LAW_VAL_POS(groupNo, Focal_point_type)) {
    case 2:
        strIndex = REPORTSTRINGDICT_FOCALTYPE2;
        break;
    case 3:
        strIndex = REPORTSTRINGDICT_FOCALTYPE3;
        break;
    case 0:
    case 1:
    default:
        strIndex = REPORTSTRINGDICT_FOCALTYPE1;
        break;
    }
    int i;
    for(i = 1 ;i < getReportDictSize(strIndex) - 1 ;++i) {
        report_law_set_focal_field_name(law, i-1, getReportDictString(strIndex ,i));
    }

    /* FocalFiledValues */
    switch( LAW_VAL_POS(groupNo, Focal_point_type) ) {
    case 0:
    case 1:
        report_law_set_focal_filed_value(law, 0, LAW_VAL_POS(groupNo, Position_start) * 0.001);//Position Start
        break;
    case 2:
        switch(rxTxMode) {
        case 0://pc
        case 2://tt
            report_law_set_focal_filed_value(law, 0, LAW_VAL_POS(groupNo, Offset_start) * 0.001);//Offset Start
            break;
        case 1://pe
        default://tofd
            break;
        }
        break;
    case 3:
        switch(rxTxMode) {
        case 0://pc
        case 2://tt
            report_law_set_focal_filed_value(law, 0, LAW_VAL_POS(groupNo, Offset_start) * 0.001);    //Offset Start
            report_law_set_focal_filed_value(law, 1, LAW_VAL_POS(groupNo, Offset_end) * 0.001);      //Offset End
            report_law_set_focal_filed_value(law, 2, LAW_VAL_POS(groupNo, Depth_start) * 0.001);     //Depth Start
            report_law_set_focal_filed_value(law, 3, LAW_VAL_POS(groupNo, Depth_end) * 0.001);       //Depth End
            break;
        case 1://pe
        default://tofd
            break;
        }
        break;
    default:
        break;
    }

    report_group_set_law(reportGroup, law);
}

static void filling_report_group_part(ReportGroup *reportGroup, gint groupNo)
{
    ReportPart *part = report_part_new();

    report_part_set_material(part, menu_content[MATERIAL + get_part_material(groupNo)]);
    report_part_set_geometry(part, menu_content[GEOMETRY + get_part_geometry(groupNo)]);
    report_part_set_thinckness(part, GROUP_VAL_POS(groupNo, part.Thickness) * 0.001);

    report_group_set_part(reportGroup, part);
}

static void filling_report_groups(Report *r)
{
    ReportGroups *reportGroups = report_groups_new();
    ReportGroup *reportGroup = NULL;
    GROUP *pGroup = NULL;
    gint i = 0;

    for ( ; i < get_group_qty(pp->p_config); ++i ) {
        pGroup = get_group_by_id(pp->p_config, i);
        reportGroup = report_group_new();

        filling_report_group_probe(reportGroup, &pGroup->probe, i);
        filling_report_group_wedge(reportGroup, &pGroup->wedge);
        filling_report_group_ttf(reportGroup, &gData->fft[i]);
        filling_report_group_setup(reportGroup, i);
        if (group_get_mode(i) == PA_SCAN || group_get_mode(i) == UT_SCAN) {
            filling_report_group_law(reportGroup, i);
        }
        filling_report_group_part(reportGroup, i);

        report_groups_add_group(reportGroups, reportGroup);
    }

    report_set_groups(r, reportGroups);
}

void filling_report(Report *r, const gchar *outputFile)
{
    g_return_if_fail( r != NULL );

    report_set_template(r, "/home/root/template.html");

    filling_report_header(r, outputFile);
    filling_report_users(r);
    filling_report_groups(r);
    filling_report_defects(r);
}

