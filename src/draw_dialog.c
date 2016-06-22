

/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 *
 *
 */

#include "core/core.h"
#include "dev/dev.h"
#include "base.h"
#include "base_config.h"
#include "draw_dialog.h"
#include "drawui.h"
#include "callback.h"
#include "file_op.h"
#include "main.h"
#include "string/dialog_str.h"
#include <glib/gprintf.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <linux/rtc.h>
#include <gdk/gdkkeysyms.h>
#include "globalData.h"
#include "ui/authorization.h"

GtkWidget *g_entry_name;
GtkWidget *g_law_ebox_save;
GtkWidget *g_law_ebox_read;
GtkWidget * g_source_list = NULL;
MY_SIGNAL g_my_signal;
_my_ip_set entry_ip;
_my_mask_set entry_mask;
PROBE g_tmp_probe;
WEDGE g_tmp_wedge;


void cd_source_dir_path (GtkTreeView *tree_view,GtkTreePath *path,GtkTreeViewColumn *column,gpointer user_data);
void cd_target_dir_path (GtkTreeView *tree_view,GtkTreePath *path,GtkTreeViewColumn *column,gpointer user_data);
gboolean law_save (GtkWidget *widget, GdkEventButton *event, gpointer data);
void draw_menu3_pop (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos );
enum
{
	LIST_ITEM = 0,
	N_COLUMNS
};

void VerifyElementSelection(GROUP* p_grp)
{
	p_grp->law_info.Elem_qty = MIN(p_grp->law_info.Elem_qty , p_grp->probe.Elem_qty);
	p_grp->law_info.Elem_qty = MIN(p_grp->law_info.Elem_qty , gMaxElementTriggerQty);
	if(p_grp->law_info.Focal_type == AZIMUTHAL_SCAN)
	{
		if(p_grp->law_info.First_tx_elem + p_grp->law_info.Elem_qty >= p_grp->probe.Elem_qty )
		{
			p_grp->law_info.First_tx_elem = 1 ;
			p_grp->law_info.Last_tx_elem  = p_grp->law_info.Elem_qty ;
		}
		else
		{
			p_grp->law_info.Last_tx_elem  = p_grp->law_info.First_tx_elem + p_grp->law_info.Elem_qty - 1 ;
		}
	}
	else  // LINEASCAN
	{
		p_grp->law_info.First_tx_elem = MIN(p_grp->law_info.First_tx_elem , p_grp->probe.Elem_qty - p_grp->law_info.Elem_qty + 1);
		p_grp->law_info.Elem_step     = 1 ;
		p_grp->law_info.Last_tx_elem = MIN(p_grp->law_info.Last_tx_elem , p_grp->probe.Elem_qty);
	}
}

void dialog_destroy(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	if ( pp->sub_window == GTK_WIDGET(data))
	{
		pp->sub_window = NULL;
	}

	gtk_widget_destroy (GTK_WIDGET(data));
	change_keypress_event (KEYPRESS_MAIN);
}
/*
void EnableKeyResponse(int _bEnable)
{
	gtk_widget_set_sensitive(pp->menubar,_bEnable);
	gtk_widget_set_sensitive(pp->eventbox2[1],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox2[2],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox2[3],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox30[0],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox31[0],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox30[1],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox31[1],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox30[2],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox31[2],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox30[4],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox31[4],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox30[5],_bEnable);
	gtk_widget_set_sensitive(pp->eventbox31[5],_bEnable);
}
*/
void DisplayInputKeyBoard()
{
	if (pp->win_keyboard)
	{
		return ;
	}
    draw_keyboard(NULL, NULL, (void*)FULL_KEYBOARD)  ;
}


void set_dialog_menu_position(GtkMenu *menu, gint *x, gint *y,
		gboolean *push_in, gpointer status_icon)
{
	*x = pp->x_pos;
	*y = pp->y_pos;
	return;
}

/* Probe 选择探头2个按键的处理 一个是确认 一个是取消 */
static void da_call_probe (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gint grp;
	GROUP *p_grp;
	grp = get_current_group(pp->p_config);
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else 
	{
		//其实要指明一个临时的group
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}

	if (GTK_RESPONSE_OK == response_id)  /* 确认 */
	{

		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection1), &model, &iter)) /* 选中探头型号时 */
		{
			gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
			if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
				file_path = g_strdup_printf ("%s%s/%s", PA_PROBE_PATH, pp->p_type, value);
			else// if (GROUP_VAL(group_mode) == UT_SCAN )
				file_path = g_strdup_printf ("%s%s/%s", UT_PROBE_PATH, pp->p_type, value);
			strcpy(pTmpGroupData->probeType ,pp->p_type);
			strcpy(pTmpGroupData->probeFileName ,value);
			free(value);//add by lzk
			if(!read_probe_file (file_path, &p_grp->probe))
			{
				g_free (file_path);
				return ;
			}
			set_group_val (p_grp, GROUP_FREQ_VAL, p_grp->probe.Frequency);				/* 频率 */
			//if (!get_group_val (p_grp, GROUP_PW_POS))
			set_group_val (p_grp, GROUP_PW_VAL, get_pw() * PW_DIV);

			VerifyElementSelection(p_grp) ;

			g_free (file_path);
			gtk_widget_destroy(GTK_WIDGET (dialog));
			change_keypress_event (KEYPRESS_MAIN);
			if (pp->pos != 0)
			{
				set_filter(grp);
				GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
			    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
			    generate_focallaw (grp);
				RefreshBeamDelay(grp);
				RefreshGainOffset(grp);
				MultiGroupSendGroupSpi();
				MultiGroupSendFocalSpi();
				gtk_widget_queue_draw (pp->status_area);
				RefreshPrfMark();
				draw_area_all() ;
				RefreshScanInfor() ;
				pp->bRefreshDraw = TRUE ;
			}
		 }
		else /* 未选中探头型号时 */
		{
			if (pp->tag == 0)/*探头大类选择Unknow时*/
			{
				if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
					read_probe_file (PA_UNKNOWN_PROBE, &p_grp->probe);
				else// if (GROUP_VAL(group_mode) == UT12_SCAN )
					read_probe_file (UT_UNKNOWN_PROBE, &p_grp->probe);

				set_group_val (p_grp, GROUP_FREQ_VAL, p_grp->probe.Frequency);				/* 频率 */
				set_group_val (p_grp, GROUP_PW_VAL, get_pw() * PW_DIV);
				gtk_label_set_text (GTK_LABEL (pp->data3[3]), p_grp->probe.Model);

				gtk_widget_destroy(GTK_WIDGET (dialog));
				change_keypress_event (KEYPRESS_MAIN);

				strcpy(pTmpGroupData->probeType ,pp->p_type);
				strcpy(pTmpGroupData->probeFileName ,"");
			}
			else 
			{
				gtk_widget_child_focus(GTK_WIDGET (dialog),GTK_DIR_TAB_FORWARD);	
				if (gtk_tree_model_get_iter_first (model, &iter))/*用model中的第一项初始化iter*/
				{
					gtk_tree_selection_select_iter(pp->selection1, &iter);/*选择&iter指定的那项*/
				}
				else 
				{
					gtk_widget_destroy(GTK_WIDGET (dialog));
					change_keypress_event (KEYPRESS_MAIN);
				}
			}
		}
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection1), &model, &iter)) /* 选中探头型号时 */
		{
			gtk_widget_child_focus(GTK_WIDGET (dialog),GTK_DIR_TAB_BACKWARD);
			gtk_tree_selection_unselect_all(pp->selection1);
		}
		else
		{
			gtk_widget_destroy(GTK_WIDGET (dialog));
			change_keypress_event (KEYPRESS_MAIN);
		}
	}
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

/* Wedge 选择楔块2个按键的处理 一个是确认 一个是取消 */
static void da_call_wedge (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value = NULL;
	char *file_path = NULL;
	int grp;
	GROUP *p_grp;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		grp = get_current_group(pp->p_config);
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}

	if (GTK_RESPONSE_OK == response_id)  /* 确认 */
	{
		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection1), &model, &iter)) /* 选中楔块型号时 */
		{
			gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		//	if (GROUP_VAL_POS(grp , group_mode) == PA_SCAN )
			if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)//edit by lzk
				file_path = g_strdup_printf ("%s%s/%s", PA_WEDGE_PATH, pp->p_type, value);
			else// if (GROUP_VAL(group_mode) == UT_SCAN )
				file_path = g_strdup_printf ("%s%s/%s", UT_WEDGE_PATH, pp->p_type, value);
			strcpy(pTmpGroupData->wedgeType ,pp->p_type);
			strcpy(pTmpGroupData->wedgeFileName ,value);
			free(value);//add by lzk
			if(!read_wedge_file (file_path, &p_grp->wedge))
			{
				g_free(file_path);
				return ;
			}
			g_free(file_path);

			gtk_widget_destroy (GTK_WIDGET (dialog));
			change_keypress_event (KEYPRESS_MAIN);

			if (pp->pos)
			{
				GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
			    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
			    generate_focallaw (grp);
				RefreshBeamDelay(grp);
				RefreshGainOffset(grp);
				MultiGroupSendGroupSpi();
				MultiGroupSendFocalSpi();
				gtk_widget_queue_draw (pp->status_area);
				RefreshPrfMark();
				draw_area_all() ;
				RefreshScanInfor() ;
				pp->bRefreshDraw = TRUE ;
			}
		}
		else
		{
			if (pp->tag == 0)/* 楔块大类选择Unknow时*/
			{
			//	strcpy(GROUP_VAL_POS(grp , wedge.Model), " Unknown");
				strcpy(p_grp->wedge.Model, " Unknown");//edit by lzk
				gtk_label_set_text (GTK_LABEL (pp->data3[4]), p_grp->wedge.Model);
				gtk_widget_destroy (GTK_WIDGET (dialog));
				change_keypress_event (KEYPRESS_MAIN);

				strcpy(pTmpGroupData->wedgeType ,pp->p_type);
				strcpy(pTmpGroupData->wedgeFileName ,"");
			}
			else
			{
				gtk_widget_child_focus(GTK_WIDGET (dialog),GTK_DIR_TAB_FORWARD);	
				if (gtk_tree_model_get_iter_first (model, &iter))/*用model中的第一项初始化iter*/
					gtk_tree_selection_select_iter(pp->selection1, &iter);/*选择&iter指定的那项*/
				else
					gtk_widget_destroy (GTK_WIDGET (dialog));
					change_keypress_event (KEYPRESS_MAIN);
			}
		}
	}

	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection1), &model, &iter)) /* 选中楔块型号时 */
		{
				gtk_widget_child_focus(GTK_WIDGET (dialog),GTK_DIR_TAB_BACKWARD);
				gtk_tree_selection_unselect_all(pp->selection1);
		}
		else
		{
			gtk_widget_destroy (GTK_WIDGET (dialog));
			change_keypress_event (KEYPRESS_MAIN);
		}
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

/* 调色板 选择2个按键的处理 一个是确认 一个是取消 */
static void da_call_palette (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gint i;
	int grp = get_current_group (pp->p_config) ;

	if (GTK_RESPONSE_OK == response_id)  /* 确认 */
	{
		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection), &model, &iter)) /* 选中探头型号时 */
		{
			gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
			file_path = g_strdup_printf ("%s%s", PALETTE_PATH, value);

			read_palette_file (file_path, TMP(t_special_col), TMP(t_color));  /*   */
			switch (GROUP_VAL_POS(grp , col_select_pos))
			{
				case 0:
					for (i = 0; i < 256; i ++)
						TMP(color_amp[i]) = COL_24_TO_16(TMP(t_color)[i]);
					strcpy(pp->p_config->szPalettePathAmplitude , value) ;
					break;
				case 1:
					for (i = 0; i < 256; i ++)
						TMP(color_tofd[i]) = COL_24_TO_16(TMP(t_color)[i]);
					strcpy(pp->p_config->szPalettePathTofd , value) ;
					break;
				case 2:
					for (i = 0; i < 256; i ++)
						TMP(color_depth[i]) = COL_24_TO_16(TMP(t_color)[i]);
					strcpy(pp->p_config->szPalettePathCorrosion , value) ;
					break;
				default:break;
			}
			free(value);//add by lzk
			g_free (file_path);
			gtk_widget_destroy (GTK_WIDGET (dialog));
			change_keypress_event (KEYPRESS_MAIN);
		}
		gtk_widget_queue_draw (pp->vboxtable);
		pp->bRefreshDraw = TRUE ;
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
}

/* 向列表里添加东西 */
void add_to_list(GtkWidget *list, const gchar *str, guint count)
{
	GtkListStore *store;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *str_data;
	guint tt = count, pos;
	store = GTK_LIST_STORE(gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));
	model = (gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));

	if (!count) 
	{
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
	}
	else
	{
		gtk_tree_model_get_iter_first (model, &iter);
		gtk_tree_model_get (model, &iter, LIST_ITEM, &str_data, -1);

		while ( count-- )
		{
			if (strcmp(str, str_data) < 0)	
			{
				pos = tt - count - 1;
				break;
			}
			else 
				pos = tt - count ;
			if (count)
			{
				g_free (str_data);
				gtk_tree_model_iter_next (model, &iter);
				gtk_tree_model_get (model, &iter, LIST_ITEM, &str_data, -1);
			}
		}
		g_free (str_data);

		gtk_list_store_insert (store, &iter, pos);
		gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
	}
}


static void
AppendItemToListView(GtkWidget *list, const gchar *str)
{
	GtkListStore *store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	store = GTK_LIST_STORE(gtk_tree_view_get_model
	  (GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, str, -1);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	gtk_tree_selection_select_iter (selection, &iter);
}


/* 初始化列表 */
static void init_file_list (GtkWidget *list,
		GtkTreeSelection *selection,
		const gchar *path, guint file_type)
{ 
	DIR* dir = opendir(path);
	GtkListStore *store;
	struct dirent* enump = NULL;
	size_t name_len;
	GtkTreeIter iter;
	GtkTreeModel *model;
	guint i = 0;

	if (NULL == dir)
	{
		g_print("Open directory failed:%s \n", path);
		return ;
	}

	store = GTK_LIST_STORE(gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));
	gtk_list_store_clear(store);

	while ((enump = readdir(dir)))
	{
		name_len = strlen(enump->d_name);
		if ((name_len == 1 && enump->d_name[0] == '.')
				|| (name_len == 2 && !strncmp(enump->d_name, "..", 2)))
			continue;

		if (file_type == enump->d_type)
		{
			add_to_list(list, enump->d_name, i);
			i++;
		}
	}
	if (selection)
	{
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (list));	
		if (gtk_tree_model_get_iter_first (model, &iter))
		{
			gtk_tree_selection_select_iter(selection, &iter);
			pp->tag = 0;
		}
	}

	closedir(dir);
}

void testTreeView(GtkWidget* treeview ,const char* filename ,int column);
/* Probe 大类(左边treeview)选择的处理函数 */
static void on_changed_probe(GtkTreeSelection *selection, gpointer p) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;
	int grp = get_current_group (pp->p_config) ;
	int _nLanguage = get_language(pp->p_config);
	GROUP *p_grp;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		if (strcmp(value, " Unknown") == 0)
			pp->tag = 0;
		else if (strcmp(value, "user") == 0)
			pp->tag =2;
		else
			pp->tag =1;

		
		strcpy(pp->p_type, value);

		if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
			file_path = g_strdup_printf ("%s%s/", PA_PROBE_PATH , value);	
		else
			file_path = g_strdup_printf ("%s%s/", UT_PROBE_PATH , value);	
		init_file_list (GTK_WIDGET (p), NULL, file_path, DT_REG);
		g_free(file_path);
		if(0 == strcmp(pTmpGroupData->probeType ,value))
		{
			testTreeView(GTK_WIDGET(p) ,pTmpGroupData->probeFileName ,LIST_ITEM);
		}
		g_free(value);
	}
	gtk_tree_model_unref_node (model, &iter);
	if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
	{
		if (pp->tag == 0)
			gtk_label_set_text (GTK_LABEL (pp->label_probe), strProbeSelectChange[_nLanguage][0]);
		else if (pp->tag == 2)
			gtk_label_set_text (GTK_LABEL (pp->label_probe), strProbeSelectChange[_nLanguage][1]);
		else
			gtk_label_set_text (GTK_LABEL (pp->label_probe), strProbeSelectChange[_nLanguage][2]);
	}

}

/* Wedge 大类(左边treeview)选择的处理函数 */
static void on_changed_wedge(GtkTreeSelection *selection, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;
	int grp = get_current_group (pp->p_config) ;
	int _nLanguage = get_language(pp->p_config);
	GROUP *p_grp;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		if (strcmp(value, " Unknown") == 0)
			pp->tag = 0;
		else if (strcmp(value, "user") == 0)
			pp->tag =2;
		else
			pp->tag =1;
		strcpy(pp->p_type, value);

		if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
			file_path = g_strdup_printf ("%s%s/", PA_WEDGE_PATH , value);	
		else
			file_path = g_strdup_printf ("%s%s/", UT_WEDGE_PATH , value);	
		init_file_list (GTK_WIDGET (label), NULL, file_path, DT_REG);
		//printf("file_path = %s\n", file_path);
		g_free(file_path);
		if(0 == strcmp(pTmpGroupData->wedgeType ,value))
		{
			testTreeView(GTK_WIDGET(label) ,pTmpGroupData->wedgeFileName ,LIST_ITEM);
		}
		g_free(value);
	}
	gtk_tree_model_unref_node (model, &iter);

	if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
	{
		if (pp->tag == 0)
			gtk_label_set_text (GTK_LABEL (pp->label_probe), strWedgeSelectChange[_nLanguage][0]);
		else if (pp->tag == 2)
			gtk_label_set_text (GTK_LABEL (pp->label_probe), strWedgeSelectChange[_nLanguage][1]);
		else
			gtk_label_set_text (GTK_LABEL (pp->label_probe), strWedgeSelectChange[_nLanguage][2]);
	}

}

/* 画调色板 */
static gboolean draw_palette(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gint i;
	double color_r, color_g, color_b;

	cairo_t *cr;        //声明一支画笔
	cr=gdk_cairo_create(gtk_widget_get_window(widget));//创建画笔
	cairo_set_line_width(cr, 2);

	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*设置画笔颜色为黑色*/
	cairo_move_to(cr,10,15);
	cairo_save(cr);
	cairo_set_font_size(cr, 20);		/*设置字体大小*/
	cairo_show_text(cr,"Palette");   	/*最顶端标签 Palette*/
	cairo_restore(cr);

	cairo_rectangle(cr,10,30,265,70);	/*special colors 外框*/
	cairo_move_to(cr,13,45);
	cairo_show_text(cr,"Special colors");	/*标签  Special colors*/

	if (TMP(t_special_col[0]) != 0x12345678)  
	{
		cairo_move_to(cr,13,60);
		cairo_show_text(cr,"No data");		/*标签  No data*/
		cairo_move_to(cr,13,75);
		cairo_show_text(cr,"No detection");	/*标签  No detection*/
		cairo_move_to(cr,13,90);
		cairo_show_text(cr,"No synchro");	/*标签  No synchro*/
	}
	cairo_rectangle(cr,10,102,265,306);	/*Main colors 外框*/
	cairo_move_to(cr,13,120);
	cairo_show_text(cr,"Main colors");	/*标签  Main colors*/
	cairo_move_to(cr,120,120);
	cairo_show_text(cr,"Quantity");		/*标签  Quantity*/
	cairo_move_to(cr,120,135);
	cairo_show_text(cr,"256");		/*标签  256 */
	cairo_stroke(cr);
	/*
	   cairo_rectangle(cr,20,140,245,256);	
	   cairo_stroke(cr);
	   */
	if (TMP(t_special_col[0]) != 0x12345678)  
	{
		for (i = 0; i < 3; i++)
		{
			color_r = (TMP(t_special_col[i]) >> 16) / 256.0;
			color_g = ((TMP(t_special_col[i]) & 0xff00) >> 8) / 256.0;
			color_b = ((TMP(t_special_col[i]) & 0xff)) /  256.0;
			//			color_r = ((TMP(t_special_col[i]) >> 16) >> 3) / 32.0;
			//			color_g = (((TMP(t_special_col[i]) & 0xff00) >> 8) >> 2) / 64.0;
			//			color_b = (((TMP(t_special_col[i]) & 0xff)) >> 3) /  32.0;
			cairo_set_source_rgba(cr, color_r, color_g, color_b, 1.0);
			cairo_rectangle(cr,85,50 + 15 * i,185,10);
			cairo_fill(cr);
			cairo_stroke(cr);
		}
	}

	for (i = 0; i < 256; i++)
	{
		color_r = (TMP(t_color[i]) >> 16) / 256.0;
		color_g = ((TMP(t_color[i]) & 0xff00) >> 8) / 256.0;
		color_b = ((TMP(t_color[i]) & 0xff)) /  256.0;
		cairo_set_source_rgba(cr,color_r,color_g,color_b,1.0);
		cairo_set_line_width(cr, 1);
		cairo_move_to(cr,20,140+i);
		cairo_line_to(cr,265,140+i);
		cairo_stroke(cr);
	}

	cairo_destroy(cr);//销毁画笔

	return TRUE;
}

/* Palette 大类(左边treeview)选择的处理函数 */
static void on_changed_palette(GtkTreeSelection *selection, gpointer drawing_area) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		file_path = g_strdup_printf ("%s%s", PALETTE_PATH , value);	
		read_palette_file (file_path, TMP(t_special_col), TMP(t_color));  /*   */
		g_free(file_path);
		g_free(value);
	}

	gtk_tree_model_unref_node (model, &iter);

	gtk_widget_queue_draw(GTK_WIDGET (drawing_area));

}

/* 简单获取探头的信息 */
static gchar* get_probe_info(const gchar *file_path)
{
	PROBE p1;
	char *probe_info = NULL;
	int _nLanguage = get_language(pp->p_config);

	if(!read_probe_file (file_path, &p1))
	{
		probe_info =  g_strdup_printf("%s", strProbeFileInfo[_nLanguage][2]);
		return probe_info  ;
	}

	int grp = get_current_group (pp->p_config) ;
	GROUP *p_grp;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
	}
	else
	{
		p_grp = &g_tmp_group_struct;
	}


	if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
		probe_info = g_strdup_printf (strProbeFileInfo[_nLanguage][0],
				p1.Model, p1.Frequency/1000.0,p1.Elem_qty, p1.Pitch/1000.0, p1.Reference_Point/1000.0 );
	else
		probe_info = g_strdup_printf ( strProbeFileInfo[_nLanguage][1],
				p1.Model, p1.Frequency/1000.0, p1.Pitch / 1000.0 );

	return probe_info;
}

/* 简单获取楔块的信息 */
static gchar* get_wedge_info(const gchar *file_path)
{
	WEDGE w1;
	int _nLanguage = get_language(pp->p_config);
	char *wedge_info = NULL;
	if(!read_wedge_file (file_path, &w1))
	{
		wedge_info = g_strdup_printf("%s", strWedgeFileInfo[_nLanguage][6]);
		return wedge_info  ;
	}
	int grp = get_current_group (pp->p_config) ;
	GROUP *p_grp;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
	}
	else
	{
		p_grp = &g_tmp_group_struct;
	}


	if (p_grp->group_mode != PA_SCAN && p_grp->group_mode != UT_SCAN)
		wedge_info = g_strdup_printf (strWedgeFileInfo[_nLanguage][0] ,
				w1.Model, w1.Angle/10.0, ((w1.Wave_type == 1) ? strWedgeFileInfo[_nLanguage][3] : strWedgeFileInfo[_nLanguage][4]), w1.Probe_delay / 1000.0 , w1.Ref_point / 1000.0 );
	else
		wedge_info = g_strdup_printf (strWedgeFileInfo[_nLanguage][1],
				w1.Model, w1.Angle/10.0, ((w1.Orientation == 1) ? strWedgeFileInfo[_nLanguage][5] : strWedgeFileInfo[_nLanguage][6]), w1.Height/1000.0, w1.Velocity_PA / 1000.0, w1.Primary_offset/1000.0, w1.Secondary_offset/1000.0, w1.length/1000.0);

	return wedge_info;
}


/* Probe 具体型号选择的处理函数 */
static void on_changed1_probe(GtkTreeSelection *selection, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gchar *probe_info = NULL;
    int grp = get_current_group (pp->p_config) ;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		GROUP *p_grp;
		if (pp->pos != 0)
		{
			p_grp = get_group_by_id (pp->p_config, grp);
		}
		else
		{
			p_grp = &g_tmp_group_struct;
		}
		if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
		{	
			file_path = g_strdup_printf ("%s%s/%s", PA_PROBE_PATH, pp->p_type, value);
		}
		else
		{	
			file_path = g_strdup_printf ("%s%s/%s", UT_PROBE_PATH, pp->p_type, value);
		}
		g_free(value);
		probe_info = get_probe_info(file_path);
		gtk_label_set_text (GTK_LABEL (pp->label_probe), probe_info);
	}
	gtk_tree_model_unref_node (model, &iter);

	if (file_path)
		g_free(file_path);
	if (probe_info)
		g_free(probe_info);
}

/* Wedge 具体型号选择的处理函数 */
static void on_changed1_wedge(GtkTreeSelection *selection, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gchar *wedge_info = NULL;
	int grp = get_current_group (pp->p_config) ;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		GROUP *p_grp;
		if (pp->pos != 0)
		{
			p_grp = get_group_by_id (pp->p_config, grp);
		}
		else
		{
			p_grp = &g_tmp_group_struct;
		}
		if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
			file_path = g_strdup_printf ("%s%s/%s", PA_WEDGE_PATH, pp->p_type, value);	
		else
			file_path = g_strdup_printf ("%s%s/%s", UT_WEDGE_PATH, pp->p_type, value);	
		g_free(value);
		wedge_info = get_wedge_info(file_path);
		gtk_label_set_text (GTK_LABEL (pp->label_probe), wedge_info);
	}
	gtk_tree_model_unref_node (model, &iter);

	if (file_path)
		g_free(file_path);
	if (wedge_info)
		g_free(wedge_info);
}
#if 0
/* 1 探头 */
static GtkWidget* ProbeDialogNew(GtkWidget* fatherWidget)
{
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */
	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */
	GtkWidget *list;	/* 2个treeview 用来放置 探头大类和名称 楔块也一样 */
	GtkWidget *list1;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkCellRenderer *renderer1;
	GtkTreeViewColumn *column1;
	GtkListStore *store1;
	GtkWidget *hpaned;
	int grp = get_current_group (pp->p_config) ;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Probe", GTK_WINDOW(fatherWidget),
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 400, 370);

	vbox1 = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

	sw = gtk_scrolled_window_new (NULL, NULL);
	sw1 = gtk_scrolled_window_new ( NULL, NULL);
	pp->sw11 = sw;
	pp->sw1 = sw1;

	/* 目录名字 探头大类 */
	gtk_widget_set_size_request (sw, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);
	/* 探头名字 小类 */
	gtk_widget_set_size_request (sw1, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	/* 2个treeview 放置探头大类和名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	list1 = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list1), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list),
			GTK_TREE_MODEL(store));
	g_object_unref(store);
	/* 初始化list1 */
	renderer1 = gtk_cell_renderer_text_new();
	column1 = gtk_tree_view_column_new_with_attributes("List Items",
			renderer1, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list1), column1);

	store1 = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list1),
			GTK_TREE_MODEL(store1));
	g_object_unref(store1);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add(GTK_CONTAINER (sw), list);
	gtk_container_add(GTK_CONTAINER (sw1), list1);

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	pp->selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(list1));

	gtk_box_pack_start (GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->label_probe = gtk_label_new("\n");
	gtk_box_pack_start (GTK_BOX(vbox1), pp->label_probe, TRUE, TRUE, 5);

	GROUP *p_grp;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
	}
	else
	{
		p_grp = &g_tmp_group_struct;
	}
	if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
		init_file_list (list, pp->selection, UT_PROBE_PATH ,DT_DIR);
	else
		init_file_list (list, pp->selection, PA_PROBE_PATH ,DT_DIR);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_probe), NULL);/*确定 or 取消*/

	g_signal_connect (G_OBJECT (pp->selection), "changed",
			G_CALLBACK(on_changed_probe), (gpointer) (list1));

	g_signal_connect (G_OBJECT (pp->selection1), "changed",
			G_CALLBACK(on_changed1_probe), (gpointer) NULL);

	gtk_widget_show_all(dialog);

	return dialog;
}

static GtkWidget* gProbeDialog = NULL;
static void draw_probe ()
{
	if(NULL == gProbeDialog)
	{
		gProbeDialog = ProbeDialogNew(GTK_WIDGET((pp->window)));
	}
	gtk_widget_show(gProbeDialog);
}
#endif

static void draw_probe ()
{
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */
	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */
	GtkWidget *list;	/* 2个treeview 用来放置 探头大类和名称 楔块也一样 */
	GtkWidget *list1;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkCellRenderer *renderer1;
	GtkTreeViewColumn *column1;
	GtkListStore *store1;
	GtkWidget *hpaned;
	int grp = get_current_group (pp->p_config) ;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Probe", GTK_WINDOW(pp->window),
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 400, 370);

	vbox1 = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

	sw = gtk_scrolled_window_new (NULL, NULL);
	sw1 = gtk_scrolled_window_new ( NULL, NULL);
	pp->sw11 = sw;
	pp->sw1 = sw1;

	/* 目录名字 探头大类 */
	gtk_widget_set_size_request (sw, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);
	/* 探头名字 小类 */
	gtk_widget_set_size_request (sw1, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	/* 2个treeview 放置探头大类和名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	list1 = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list1), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list),
			GTK_TREE_MODEL(store));
	g_object_unref(store);
	/* 初始化list1 */
	renderer1 = gtk_cell_renderer_text_new();
	column1 = gtk_tree_view_column_new_with_attributes("List Items",
			renderer1, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list1), column1);

	store1 = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list1),
			GTK_TREE_MODEL(store1));
	g_object_unref(store1);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add(GTK_CONTAINER (sw), list);
	gtk_container_add(GTK_CONTAINER (sw1), list1);

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	pp->selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(list1));

	gtk_box_pack_start (GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->label_probe = gtk_label_new("\n");
	gtk_box_pack_start (GTK_BOX(vbox1), pp->label_probe, TRUE, TRUE, 5);

	GROUP *p_grp;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}
	if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
		init_file_list (list, pp->selection, PA_PROBE_PATH ,DT_DIR);
	else
		init_file_list (list, pp->selection, UT_PROBE_PATH ,DT_DIR);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_probe), NULL);/*确定 or 取消*/

	g_signal_connect (G_OBJECT (pp->selection), "changed",
			G_CALLBACK(on_changed_probe), (gpointer) (list1));

	g_signal_connect (G_OBJECT (pp->selection1), "changed",
			G_CALLBACK(on_changed1_probe), (gpointer) NULL);

	testTreeView(GTK_WIDGET(list) ,pTmpGroupData->probeType ,LIST_ITEM);
	gtk_widget_show_all(dialog);

}


/* 2 楔块 */
static void draw_wedge ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */

	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */

	GtkWidget *list;	/* 2个treeview 用来放置 探头大类和名称 楔块也一样 */
	GtkWidget *list1;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	GtkCellRenderer *renderer1;
	GtkTreeViewColumn *column1;
	GtkListStore *store1;

	GtkWidget *hpaned;
	int grp = get_current_group (pp->p_config) ;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 400, 370);

	vbox1 = gtk_dialog_get_content_area(GTK_DIALOG(dialog));// GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	sw = gtk_scrolled_window_new (NULL, NULL);
	sw1 = gtk_scrolled_window_new ( NULL, NULL);
	/* 目录名字 楔块大类 */
	gtk_widget_set_size_request (sw, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	/* 楔块名字 小类 */
	gtk_widget_set_size_request (sw1, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	/* 2个treeview 放置楔块大类和名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	list1 = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list1), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list),
			GTK_TREE_MODEL(store));
	g_object_unref(store);
	/* 初始化list1 */
	renderer1 = gtk_cell_renderer_text_new();
	column1 = gtk_tree_view_column_new_with_attributes("List Items",
			renderer1, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list1), column1);

	store1 = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list1),
			GTK_TREE_MODEL(store1));
	g_object_unref(store1);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add(GTK_CONTAINER (sw), list);
	gtk_container_add(GTK_CONTAINER (sw1), list1);

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	pp->selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(list1));

	gtk_box_pack_start(GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->label_probe = gtk_label_new("\n");
	gtk_box_pack_start(GTK_BOX(vbox1), pp->label_probe, TRUE, TRUE, 5);

	GROUP *p_grp;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}
	if (p_grp->group_mode == PA_SCAN ||p_grp->group_mode == UT_SCAN)
		init_file_list (list, pp->selection, PA_WEDGE_PATH ,DT_DIR);
	else
		init_file_list (list, pp->selection, UT_WEDGE_PATH ,DT_DIR);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_wedge), NULL);/*确定 or 取消*/

	g_signal_connect (G_OBJECT (pp->selection), "changed",
			G_CALLBACK(on_changed_wedge), (gpointer) (list1));/*使用"changed"信号来与用户的选择信号进行关联起来*/

	g_signal_connect (G_OBJECT (pp->selection1), "changed",
			G_CALLBACK(on_changed1_wedge), (gpointer) NULL);

	testTreeView(GTK_WIDGET(list) ,pTmpGroupData->wedgeType ,LIST_ITEM);
	gtk_widget_show_all(dialog);

}

//作者 geniikid
//日期 2011-6-11
//函数 selection_file_type
//参数 char *dir_path
//参数 char *file_type
//参数 int show_father_dir
void selection_file_type(GtkWidget *list,char *dir_path,char *file_type,int show_father_dir)
{
	DIR *dir;
	int i;
	GtkListStore *store;
	struct dirent* enump = NULL;
	char *pos;
	int name_len;
	int suffix_len;

	assert(dir_path != NULL);
	assert(file_type != NULL);
	i = 0;
	store = GTK_LIST_STORE(gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));
	gtk_list_store_clear(store);
	dir = opendir(dir_path);
	if (dir == NULL)
	{
		g_print("Open directory failed:%s \n",dir_path);
		return ;
	}
	suffix_len = strlen(file_type);
	while((enump = readdir(dir)))
	{
		name_len = strlen(enump->d_name);
		if ( !( ( enump->d_type == DT_DIR ) || ( enump->d_type == DT_REG ) ) )
			continue;
		if (name_len == 1 && enump->d_name[0] == '.')
			continue;
	    if(name_len == 2 && !strncmp(enump->d_name, "..", 2))
		{
			if ( strcmp(dir_path,"/" ) == 0 )
			{
				continue;
			}
			if (show_father_dir == 0)
			{
				continue;
			}
		}
		if ( ( name_len <= suffix_len ) && ( enump->d_type == DT_REG ) && ( strcmp(file_type,"*.*") != 0 ) )
		{
			continue;
		}
		pos = strstr(enump->d_name,file_type);

		if ( ( pos == NULL ) && ( enump->d_type == DT_REG ) )
			continue;
		else
		{
			add_to_list(list, enump->d_name, i);
			i++;
		}

	}

	closedir(dir);
}


static void  response_system_info(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	if (GTK_RESPONSE_OK == response_id)  /* 保存信息 */
	{
		//g_print ("OK_Pressed\n");
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		//g_print ("CANCEL_Pressed\n");
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
	else if (RESPONSE_CLOSE == response_id)
	{
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
	else if (RESPONSE_SAVE_AND_CLOSE == response_id)
	{
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
}

/* 系统信息的显示 */
static void draw_system_info ()
{
	GtkWindow *window = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	WebKitWebView* web_view;
	gchar *file_path = "http://127.0.0.1/home/tt/TT/source/system_info.htm";

	GtkWidget *sw;
	GtkWidget *full_screen;
	GtkWidget *top_box;
	GtkWidget *bottom_box;
	GtkWidget *hbox_menu[7];
	GtkWidget *label_menu[7];
	char *char_menu[7];
	int i;
	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", window,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			NULL, NULL,
			NULL, NULL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	full_screen = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/

    switch (get_language(pp->p_config)) {
		case ENGLISH_:
			char_menu[0] = "";
			char_menu[1] = "";
			char_menu[2] = "";
			char_menu[3] = "";
			char_menu[4] = "Print";
            char_menu[5] = "Save";
			char_menu[6] = "Close";
			break;
		case CHINESE_:
			char_menu[0] = "";
			char_menu[1] = "";
			char_menu[2] = "";
			char_menu[3] = "";
			char_menu[4] = "打印";
            char_menu[5] = "保存";
			char_menu[6] = "关闭";
			break;
		default:
			char_menu[0] = "";
			char_menu[1] = "";
			char_menu[2] = "";
			char_menu[3] = "";
			char_menu[4] = "Print";
            char_menu[5] = "Save";
			char_menu[6] = "Close";
			break;
	}

	top_box = gtk_vbox_new(FALSE,0);
	bottom_box = gtk_hbox_new(FALSE,0);
    for ( i=0; i<7; ++i ) {
		hbox_menu[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(hbox_menu[i]),114,85);
		update_widget_bg(hbox_menu[i], /*backpic[1]*/1);
		label_menu[i] = gtk_label_new(char_menu[i]);
		gtk_widget_modify_fg (label_menu[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_menu[i]), GTK_JUSTIFY_CENTER);
		gtk_container_add(GTK_CONTAINER(hbox_menu[i]), label_menu[i]);
		gtk_box_pack_start(GTK_BOX(bottom_box),hbox_menu[i], FALSE, FALSE, 0);
	}
	sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(GTK_WIDGET(sw),800,515);
	//
	web_view = WEBKIT_WEB_VIEW (webkit_web_view_new());
	//web的编码方式
	webkit_web_view_set_custom_encoding (web_view, "UTF-8");

    gchar *certMode = dev_get_cert_mode();
    gchar *certData = dev_get_cert_data();
    gchar *content = g_strdup_printf("<html>"
                                     "<head><title>System Infomation</title>"
                                     "<style>"
                                     "body {background-color: #CCCCCC;}"
                                     "table, th, td {"
                                     "padding:5px;"
                                         "border: 1px solid black;"
                                         "border-collapse: collapse;"
                                     "}"
                                     "table {"
                                     "margin:5px;"
                                     "}"
                                     "th {"
                                     "text-align:left;"
                                     "background:#CCCCFF;"
                                     "}"
                                     "td {"
                                     "text-align:left;"
                                     "}"
                                     "h3 {"
                                     "text-align:center;"
                                     "}"
                                     "</style>"
                                     "</head>"
                                     "<body>"
                                     "<h3>System Infomation</h3>"
                                     "<table border=1 >"
                                     "<tr><th>Serial Number</th><td>%s</td></tr>"
                                     "<tr><th>Type</th><td>%s</td></tr>"
                                     "<tr><th>FPGA</th><td>%d</td></tr>"
                                     "<tr><th>Run Time(s)</th><td>%d</td></tr>"
                                     "<tr><th>Run Count</th><td>%d</td></tr>"
                                     "<tr><th>Phascan Version</th><td>%d.%d.%d</td></tr>"
                                     "<tr><th>Phascan Commit</th><td>%s</td></tr>"
                                     "<tr><th>Authentication Mode</th><td>%s</td></tr>"
                                     "<tr><th>Authentication Expire</th><td>%s</td></tr>"
                                     "</table>"
                                     "</body>"
                                     "</html>",
                                     dev_serial_number(),
                                     dev_type_str(),
                                     dev_fpga_version(),
                                     dev_run_time(),
                                     dev_run_count(),
                                     APP_MAJOR, APP_MINOR, APP_MICRO, GIT_COMMIT,
                                     certMode, certData);
    g_file_set_contents("/home/tt/TT/source/system_info.htm", content, strlen(content), NULL);
    g_free(content);
    g_free(certData);
    g_free(certMode);

	webkit_web_view_load_uri (web_view, file_path);
	g_signal_connect(G_OBJECT (hbox_menu[5]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);
	g_signal_connect(G_OBJECT (hbox_menu[6]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);
	gtk_container_add(GTK_CONTAINER(sw),(GtkWidget *)web_view);
	gtk_box_pack_start(GTK_BOX(top_box),sw, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(full_screen),top_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(full_screen),bottom_box, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(response_system_info), NULL);/*发送退出信号*/
	gtk_widget_show_all(dialog);
	return ;
}

/* 11 调色板 */
static void draw_color_palette ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */
	GtkWidget *sw;		/* 第一个scroll */
	GtkWidget *drawarea;
	GtkWidget *hbox_sw;	/* 用来装调色板信息*/
	GtkWidget *list;	/* 1个treeview 用来放置调色板名称 */
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkWidget *hpaned;
	dialog = gtk_dialog_new_with_buttons ("Dialog_Palette", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 480, 460);

	vbox1 = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

	sw = gtk_scrolled_window_new (NULL, NULL);
	drawarea = gtk_drawing_area_new ();
	/* 调色板名字 */
	gtk_widget_set_size_request (sw, 165, 410);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);
	/* treeview 放置调色板名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);
	hbox_sw = gtk_hbox_new(FALSE,0);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), drawarea);
	gtk_container_add(GTK_CONTAINER (sw), list);

	gtk_box_pack_start(GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	init_file_list (list, pp->selection, PALETTE_PATH, DT_REG);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_palette), NULL);

	g_signal_connect (G_OBJECT(pp->selection), "changed",
			G_CALLBACK(on_changed_palette), (gpointer)(drawarea));

	g_signal_connect (G_OBJECT (drawarea), "expose_event",
			G_CALLBACK (draw_palette), NULL);

	gtk_widget_show_all(dialog);
}

//函数说明:找到
//函数名称:get_list_check_name
//函数参数:tree_view代表在那个tree_view那里发生这件事
//返回值  :file_name代表我们在那个文件上面双击的文件名
char *get_list_check_name(GtkTreeView *tree_view)
{
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
    char *file_name = NULL;
	int value = 0;
	model = gtk_tree_view_get_model(tree_view);
	selection = gtk_tree_view_get_selection(tree_view);
	value = gtk_tree_model_get_iter_from_string(model,&iter,"0");
	//循环查找整个目录
	while(value)
	{
		//如果找到
		//理论上来说，双击了，就是选中那个东西。所以直接找选中的那个就可以了。
		//不过，这样会有一个问题，假设用户故意按ctrl键，再去双击，会有问题。
		if (gtk_tree_selection_iter_is_selected(selection,&iter))
		{
			//读取文件名
			gtk_tree_model_get(model,&iter,0,&file_name,-1);
            //返回文件名
			return file_name;
		}
		else
		{
			//下一个列表项
			value = gtk_tree_model_iter_next(model,&iter);
		}
	}
	//理论上来说，不会跳到这里。
	//因为跳到这里，代表我们找了整个列表，都没有找到是那个进行了双击的。
    return file_name;
}

//函数说明：判断某一个文件夹下面的某个文件，究竟是一个文件，还是一个目录
//函数名称：is_dir
//函数参数：path文件夹的路径名称
//          file_name文件名
//返回值  ：1代表是一个目录，0代表不是一个目录，是一个文件,-1代表查找出错。
int is_dir(char *path,char *file_name)
{
	DIR *dir;
	struct dirent *dir_name;
	//打开目录
    dir = opendir(path);
	//理论上不会出现这种现象，这个路径是用pwd所产生的。不过为了程序的健壮性，还是这么写
	if ( dir == NULL )
		return FALSE;
    //循环搜索文件夹下面的
    while ( ( dir_name = readdir(dir) ) != NULL )
	{
		//找到那个文件
        if( strcmp(dir_name->d_name,file_name ) == 0 )
		{
			//是文件夹
			if( dir_name->d_type == DT_DIR )
			{
				closedir(dir);
				return 1;
			}
			//是文件
			else
			{
				closedir(dir);
				return 0;
			}
		}
	}

	//理论上不会产生这个，
	g_print("I am sorry,i can't find %s file in %s path\n",file_name,path);
	return -1;
}

void change_source_dir(char *dir_name)
{
    char old_dir[PATH_MAX];
	char new_dir[PATH_MAX];
	int tmp;
	tmp = (gint)getcwd( old_dir, PATH_MAX);
    tmp = chdir(Get_Source_File_Path());
    tmp = chdir(dir_name);
    tmp = (gint)getcwd(new_dir,PATH_MAX);
	Set_Source_File_Path(new_dir);
	tmp = chdir(old_dir);
}

void change_target_dir(char *dir_name)
{
    char old_dir[PATH_MAX];
	char new_dir[PATH_MAX];
	int tmp;
	tmp = (gint)getcwd(old_dir,PATH_MAX);
    tmp = chdir(Get_Target_File_Path());
    tmp = chdir(dir_name);
    tmp = (gint)getcwd(new_dir,PATH_MAX);
	Set_Target_File_Path(new_dir);
	tmp = chdir(old_dir);
}

void cd_source_dir_path (GtkTreeView *tree_view,GtkTreePath *path,GtkTreeViewColumn *column,gpointer user_data)
{
    _dir_path_label_p dir_path_label_p_struct = (_dir_path_label_p) user_data;
	char *file_type = dir_path_label_p_struct->file_type;
	GtkWidget *source_label = dir_path_label_p_struct->source_label;
	char *file_name = NULL;
	char *pwd_path = Get_Source_File_Path();
	int return_value;
	g_print("Source_File_Path is %s\n",pwd_path);
	file_name = get_list_check_name(tree_view);
	//没有查出选中是那个文件，直接退出
	if(file_name == NULL)
		return ;
	//直接判断是不是文件夹
    return_value = is_dir(pwd_path,file_name);
	//不是文件夹或者查找出错，直接退出
	if ( ( return_value == -1 ) || ( return_value == 0 ) )
	{
		return ;
	}
	//
	change_source_dir(file_name);
	//
	gtk_label_set_text(GTK_LABEL (source_label), pwd_path);
    //
	selection_file_type(GTK_WIDGET (tree_view),pwd_path,file_type,TRUE);
	//
	g_free(file_name);
}

void cd_target_dir_path (GtkTreeView *tree_view,GtkTreePath *path,GtkTreeViewColumn *column,gpointer user_data)
{ 
	_dir_path_label_p dir_path_label_p_struct = (_dir_path_label_p) user_data;
	char *file_type = dir_path_label_p_struct->file_type;
	GtkWidget *target_label = dir_path_label_p_struct->target_label;
	char *file_name = NULL;
	char *pwd_path = Get_Target_File_Path();
	int return_value;
	g_print("Source_File_Path is %s\n",pwd_path);
	file_name = get_list_check_name(tree_view);
	//没有查出选中是那个文件，直接退出
	if(file_name == NULL)
		return ;
	//直接判断是不是文件夹
    return_value = is_dir(pwd_path,file_name);
	//不是文件夹或者查找出错，直接退出
	if ( ( return_value == -1 ) || ( return_value == 0 ) )
	{
		return ;
	}
	change_target_dir(file_name);
	gtk_label_set_text(GTK_LABEL (target_label), pwd_path);
	selection_file_type(GTK_WIDGET (tree_view),pwd_path,file_type,TRUE);
	g_free(file_name);
}


static void response_law_save (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	if (GTK_RESPONSE_OK == response_id)  /* 保存信息 */
	{
		law_save(g_law_ebox_save,NULL,g_entry_name);
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		g_print ("CANCEL_Pressed");
	}
	else if (RESPONSE_LAW_SAVE == response_id)
	{
		law_save(g_law_ebox_save,NULL,g_entry_name);
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
	change_keypress_event (KEYPRESS_MAIN);
}

void  on_changed_law_save(GtkWidget *widget, gpointer label)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(widget), &model, &iter)) {

		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		file_path =
			(gchar*)malloc(strlen("file://") + strlen(label) + strlen(value) + 1);
		strcpy(file_path, "file://");
		strcat(file_path, label);
		strcat(file_path, value);
		webkit_web_view_load_uri (pp->web_view_tmp, file_path);   /* 本地文件路径前面加上file:// */
		g_debug ("file_path = %s\n", file_path);
		g_free(file_path);
		g_free(value);
	}
	gtk_tree_model_unref_node (model, &iter);

}

gboolean law_save (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	gchar *file_path;

	file_path = g_strdup_printf ("%s%s.law",
			USER_LAW_PATH, gtk_entry_get_text (GTK_ENTRY (data)));

	save_law_file(file_path, get_current_group(pp->p_config));
	g_free (file_path);
	//	widget_window_class->key_press_event = my_keypress_event;
	gtk_widget_destroy (gtk_widget_get_parent
			(gtk_widget_get_parent
			 (gtk_widget_get_parent (widget))));
	return TRUE;
}

gboolean law_close (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	//	widget_window_class->key_press_event = my_keypress_event;
	gtk_widget_destroy (GTK_WIDGET (data));
	change_keypress_event (KEYPRESS_MAIN);
	return TRUE;
}

gboolean law_name (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	gtk_widget_grab_focus (GTK_WIDGET (data));
	return 0;
}

/* 13 保存聚焦法则 */
static void draw_law_save ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	//	GtkWidgetClass *widget_window_class1;
	GtkWidget *dialog;
	GtkWidget *vbox1;
	GtkWidget *ebox_save;
	GtkWidget *label_save;
	GtkWidget *ebox_close;
	GtkWidget *label_close;
	GtkWidget *vbox_name;
	GtkWidget *entry_name;
	GtkWidget *ebox_name;
	GtkWidget *label_name;
	GtkWidget *sw;
	GtkWidget *sw1;
	GtkWidget *table;
	GtkWidget *hpaned;
	GtkWidget *list;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	dialog = gtk_dialog_new_with_buttons("Dialog_Law_Save", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg (GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/

	switch(get_language(pp->p_config))
	{
		case ENGLISH_:
			label_save	= gtk_label_new("Save");
			label_close	= gtk_label_new("Close");
			label_name	= gtk_label_new("Name");
			break;
		case CHINESE_:
			label_save	= gtk_label_new("保存");
			label_close	= gtk_label_new("关闭");
			label_name	= gtk_label_new("名字");
			break;
		default:
			label_save	= gtk_label_new("Save");
			label_close	= gtk_label_new("Close");
			label_name	= gtk_label_new("Name");
			break;
	}

	vbox1	= GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	ebox_save	= gtk_event_box_new();
	update_widget_bg(ebox_save, /*backpic[7]*/7);
	ebox_close	= gtk_event_box_new();
	update_widget_bg(ebox_close, /*backpic[7]*/7);
	entry_name	= gtk_entry_new();
	vbox_name	= gtk_vbox_new(FALSE, 0);
	ebox_name	= gtk_event_box_new();

	gtk_container_add (GTK_CONTAINER (ebox_save), label_save);
	gtk_container_add (GTK_CONTAINER (ebox_close), label_close);
	gtk_container_add (GTK_CONTAINER (ebox_name), label_name);
	gtk_box_pack_start (GTK_BOX(vbox_name), entry_name, FALSE, FALSE, 5);
	gtk_box_pack_start (GTK_BOX(vbox_name), ebox_name, TRUE, TRUE, 5);

	g_law_ebox_save = ebox_save;
	g_entry_name = entry_name;

	g_signal_connect (G_OBJECT(ebox_save), "button-press-event",
			G_CALLBACK(law_save), (gpointer) (entry_name));

	g_signal_connect (G_OBJECT(ebox_close), "button-press-event",
			G_CALLBACK(law_close), (gpointer) (dialog));

	g_signal_connect (G_OBJECT(ebox_name), "button-press-event",
			G_CALLBACK(law_name), (gpointer) (entry_name));

	sw		= gtk_scrolled_window_new(NULL, NULL);
	sw1		= gtk_scrolled_window_new(NULL, NULL);
	/* 文件的名字 */
	gtk_widget_set_size_request(sw, 200, 400);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list),
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	/* 文件的内容 */
	gtk_widget_set_size_request(sw1, 400, 400);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	pp->web_view_tmp = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	webkit_web_view_set_custom_encoding (pp->web_view_tmp, "UTF-8");
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add (GTK_CONTAINER (sw), list);
	gtk_container_add (GTK_CONTAINER (sw1), GTK_WIDGET (pp->web_view_tmp));

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	init_file_list (list, pp->selection, USER_LAW_PATH, DT_REG);
	//	on_changed(GTK_WIDGET (pp->selection), NULL) ;/* 初始化 */

	g_signal_connect (G_OBJECT (pp->selection), "changed",
			G_CALLBACK(on_changed_law_save), USER_LAW_PATH);

	table = gtk_table_new(6, 7, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0);

	gtk_table_attach(GTK_TABLE(table), hpaned, 0, 6, 0, 5,
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);
	gtk_table_attach(GTK_TABLE(table), ebox_save, 6, 7, 1, 2,		/* 0 5   */
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */
	gtk_table_attach(GTK_TABLE(table), ebox_close, 6, 7, 2, 3,
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */
	gtk_table_attach(GTK_TABLE(table), vbox_name, 0, 7, 5, 6,
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 下方栏 */
	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(response_law_save), NULL);/*发送退出信号*/

	gtk_box_pack_start (GTK_BOX(vbox1), table, TRUE, TRUE, 5);
	gtk_widget_show_all (dialog);
	gtk_widget_hide (gtk_dialog_get_action_area(GTK_DIALOG(dialog)));
}

gboolean law_read (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	//gint offset, k;
	gchar *value = NULL;
	gchar *file_path = NULL;
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(pp->selection), &model, &iter))
	{
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		file_path = g_strdup_printf ("%s%s", USER_LAW_PATH, value);
		free(value);//add by lzk
		read_law_file(file_path, grp);
		set_group_val (p_grp, GROUP_PW_VAL  ,TMP(focal_law_all_elem[grp][0][0]).P_width);				/* 频率 */
	}

	g_free (file_path);
	gtk_widget_destroy (gtk_widget_get_parent
			(gtk_widget_get_parent
			 (gtk_widget_get_parent (widget))));
	return TRUE;
}

static void response_law_read (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	if (GTK_RESPONSE_OK == response_id)  /* 保存信息 */
	{
		g_print ("OK_Pressed");
		law_read(g_law_ebox_read,NULL,NULL);
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		g_print ("CANCEL_Pressed");
	}
	else if (RESPONSE_LAW_READ ==  response_id)
	{
		law_read(g_law_ebox_read,NULL,NULL);
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
	change_keypress_event (KEYPRESS_MAIN);
}

/* 14 读取聚焦法则 */
static void draw_law_read ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	//	GtkWidgetClass *widget_window_class1;
	GtkWidget *dialog;
	GtkWidget *vbox1;
	GtkWidget *ebox_read;
	GtkWidget *label_read;
	GtkWidget *ebox_close;
	GtkWidget *label_close;
	GtkWidget *sw;
	GtkWidget *sw1;
	GtkWidget *table;
	GtkWidget *hpaned;
	GtkWidget *list;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	dialog = gtk_dialog_new_with_buttons("Dialog_Law_Save", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg (GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	//	widget_window_class->key_press_event = window_keypress_event_orig;
	//	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(dialog)));
	//	widget_window_class1->key_press_event = gtk_entry_full_screen_keypress_event;
	/* 这里处理按键 */

	switch(get_language(pp->p_config))
	{
		case ENGLISH_:
			label_close	= gtk_label_new("Close");
			label_read	= gtk_label_new("Read");
			break;
		case CHINESE_:
			label_close	= gtk_label_new("关闭");
			label_read	= gtk_label_new("读");
			break;
		default:
			label_close	= gtk_label_new("Close");
			label_read	= gtk_label_new("Read");
			break;
	}

	vbox1	= GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	ebox_read	= gtk_event_box_new();
	gtk_widget_set_size_request(GTK_WIDGET (ebox_read), 115, 85);
	//gtk_widget_modify_bg(GTK_WIDGET (ebox_read), GTK_STATE_NORMAL, &color_blue);
	update_widget_bg(ebox_read, /*backpic[7]*/7);
	ebox_close	= gtk_event_box_new();
	gtk_widget_set_size_request(GTK_WIDGET (ebox_close), 115, 85);
	//gtk_widget_modify_bg(GTK_WIDGET (ebox_close), GTK_STATE_NORMAL, &color_blue);
	update_widget_bg(ebox_close, /*backpic[7]*/7);

	gtk_container_add (GTK_CONTAINER (ebox_read), label_read);
	gtk_container_add (GTK_CONTAINER (ebox_close), label_close);

	g_law_ebox_read = ebox_read;

	g_signal_connect (G_OBJECT(ebox_read), "button-press-event",
			G_CALLBACK(law_read), (gpointer) (NULL));

	g_signal_connect (G_OBJECT(ebox_close), "button-press-event",
			G_CALLBACK(law_close), (gpointer) (dialog));

	sw		= gtk_scrolled_window_new(NULL, NULL);
	sw1		= gtk_scrolled_window_new(NULL, NULL);
	/* 文件的名字 */
	gtk_widget_set_size_request(sw, 200, 600);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list),
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	/* 文件的内容 */
	gtk_widget_set_size_request(sw1, 400, 600);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	pp->web_view_tmp = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	webkit_web_view_set_custom_encoding (pp->web_view_tmp, "UTF-8");
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add (GTK_CONTAINER (sw), list);
	gtk_container_add (GTK_CONTAINER (sw1), GTK_WIDGET (pp->web_view_tmp));

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	init_file_list (list, pp->selection, USER_LAW_PATH, DT_REG);
	//	on_changed(GTK_WIDGET (pp->selection), NULL) ;/* 初始化 */

	g_signal_connect (G_OBJECT (pp->selection), "changed",
			G_CALLBACK(on_changed_law_save), USER_LAW_PATH);

	table = gtk_table_new(6, 7, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0);


	gtk_table_attach(GTK_TABLE(table), hpaned, 0, 6, 0, 6,
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);

	gtk_table_attach(GTK_TABLE(table), ebox_read, 6, 7, 1, 2,		/* 0 5   */
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */

	gtk_table_attach(GTK_TABLE(table), ebox_close, 6, 7, 2, 3,
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(response_law_read), NULL);/*发送退出信号*/

	gtk_box_pack_start (GTK_BOX(vbox1), table, TRUE, TRUE, 5);
	gtk_widget_show_all (dialog);
	gtk_widget_hide (gtk_dialog_get_action_area(GTK_DIALOG(dialog)));
}

static void da_call_ip (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	char ifconfig_buf[64] = "ifconfig usb0 ";
	_my_ip_get tmp_ip;
	_my_ip_set *entry_ip_p = (_my_ip_set *)user_data;
	int i;
	unsigned char *tmp;
	char ip_string[256] = {0};
	struct in_addr addr;
	int inet_sock;
	struct ifreq ifr;
	int system_value;

	//用户点击了确认按钮
	if (GTK_RESPONSE_OK == response_id)
	{
		tmp = (unsigned char *)&tmp_ip;

		//读取输入框里面的数据
		for(i=0;i<4;i++)
		{
			*tmp = gtk_spin_button_get_value((GtkSpinButton *)entry_ip_p->entry[i]);
			tmp++;
		}

		//设置ip地址
		memcpy(&addr,&tmp_ip,sizeof(tmp_ip));
		sprintf(ip_string,"%s\n", inet_ntoa(addr));

		strcat(ifconfig_buf,ip_string);
		system_value = system(ifconfig_buf);

		//重新读取ip地址，并显示出来，因为有可能设置失败，所以要重新读取
		inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
		strcpy(ifr.ifr_name, "usb0");
		if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
			perror("ioctl");

		memcpy(&tmp_ip,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_ip));

		memcpy(&addr,&tmp_ip,sizeof(tmp_ip));
		sprintf(ip_string,"%s\n", inet_ntoa(addr));

		gtk_label_set_text (GTK_LABEL (pp->data3[0]), ip_string);

		//重算子网掩码，并重画
		draw3_data1(pp);

		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
	else if(GTK_RESPONSE_CANCEL == response_id)
	{
		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}

}

static void draw_ip()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;

	GtkObject *adjustment[5];
	GtkWidget *label[5];
	char *char_label[5];

	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;

	GtkWidget *vbox_first;	/* 指向dialog的vbox */

	int inet_sock;
	struct ifreq ifr;
	_my_ip_get tmp_ip;
	//_my_ip_set entry_ip;
	unsigned char *tmp;
	int i;

	//读取usb0 ip地址
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, "usb0");
	if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
		perror("ioctl");

	memcpy(&tmp_ip,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_ip));

	//新建对话框
	dialog = gtk_dialog_new_with_buttons ("Dialog_Ip", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 140);


	switch(get_language(pp->p_config))
	{
		case ENGLISH_:
			char_label[0] = "IP Address";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			char_label[4] = "";
			break;
		case CHINESE_:
			char_label[0] = "IP 地址";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			char_label[4] = "";
			break;
		default:
			char_label[0] = "IP Address";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			char_label[4] = "";
			break;
	}

	vbox_first = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

	hbox1 = gtk_hbox_new(TRUE, 0);
	hbox2 = gtk_hbox_new(FALSE, 0);
	hbox3 = gtk_hbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_first), hbox3, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox2, FALSE, FALSE, 5);

	label[0] = gtk_label_new(char_label[0]);
	gtk_box_pack_start(GTK_BOX(hbox3), label[0], TRUE, TRUE, 5);

	tmp = (unsigned char *)&tmp_ip;

	//把ip在输入框那里显示出来
	for (i=1;i<5;i++)
	{
		label[i] = gtk_label_new(char_label[i]);
		gtk_box_pack_start(GTK_BOX(hbox1), label[i], FALSE, FALSE, 15);
		adjustment[i] = gtk_adjustment_new(0.0,0.0,255.0,1.0,0.0,0.0);
		entry_ip.entry[i-1] = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment[i]),0.01,0);
		gtk_spin_button_set_value((GtkSpinButton *)entry_ip.entry[i-1],*tmp);
		gtk_box_pack_start(GTK_BOX(hbox2), entry_ip.entry[i-1], TRUE, TRUE, 1);
		tmp++;
	}

	//用户点击对话上面的按钮
	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_ip), (gpointer)&entry_ip);/*确定 or 取消*/

	gtk_widget_show_all(dialog);
}

static void da_call_mask (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	char ifconfig_buf[64] = "ifconfig usb0 netmask ";
	_my_mask_get tmp_mask;
	_my_mask_set *entry_mask_p = (_my_mask_set *)user_data;
	int i;
	unsigned char *tmp;
	char mask_string[256] = {0};
	struct in_addr addr;
	int inet_sock;
	struct ifreq ifr;
	int system_value;
	//用户点击了确认按钮
	if (GTK_RESPONSE_OK == response_id)
	{
		tmp = (unsigned char *)&tmp_mask;

		//读取输入框里面的数据
		for(i=0;i<4;i++)
		{
			*tmp = gtk_spin_button_get_value((GtkSpinButton *)entry_mask_p->entry[i]);
			tmp++;
		}

		//设置mask地址
		memcpy(&addr,&tmp_mask,sizeof(tmp_mask));
		sprintf(mask_string,"%s\n", inet_ntoa(addr));

		strcat(ifconfig_buf,mask_string);
		system_value = system(ifconfig_buf);

		//重新读取mask地址，并显示出来，因为有可能设置失败，所以要重新读取
		inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
		strcpy(ifr.ifr_name, "usb0");
		if (ioctl(inet_sock, SIOCGIFNETMASK, &ifr) < 0)
			perror("ioctl");

		memcpy(&tmp_mask,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_mask));

		memcpy(&addr,&tmp_mask,sizeof(tmp_mask));
		sprintf(mask_string,"%s\n", inet_ntoa(addr));

		gtk_label_set_text (GTK_LABEL (pp->data3[1]), mask_string);

		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
	else if(GTK_RESPONSE_CANCEL == response_id)
	{
		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}
}

static void draw_mask()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;

	GtkObject *adjustment[5];
	GtkWidget *label[5];
	char *char_label[5];

	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;

	GtkWidget *vbox_first;	/* 指向dialog的vbox */

	int inet_sock;
	struct ifreq ifr;
	_my_mask_get tmp_mask;
	//_my_mask_set entry_mask;
	unsigned char *tmp;
	int i;

	//读取usb0 mask 地址
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, "usb0");
	if (ioctl(inet_sock, SIOCGIFNETMASK, &ifr) < 0)
		perror("ioctl");

	memcpy(&tmp_mask,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_mask));

	//新建对话框
	dialog = gtk_dialog_new_with_buttons ("Dialog_MASK", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 140);

	switch(get_language(pp->p_config))
	{
		case ENGLISH_:
			char_label[0] = "MASK Address";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			char_label[4] = "";
			break;
		case CHINESE_:
			char_label[0] = "子网掩码";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			char_label[4] = "";
			break;
		default:
			char_label[0] = "MASK Address";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			char_label[4] = "";
			break;
	}

	vbox_first = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

	hbox1 = gtk_hbox_new(TRUE, 0);
	hbox2 = gtk_hbox_new(FALSE, 0);
	hbox3 = gtk_hbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_first), hbox3, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox2, FALSE, FALSE, 5);

	label[0] = gtk_label_new(char_label[0]);
	gtk_box_pack_start(GTK_BOX(hbox3), label[0], TRUE, TRUE, 5);

	tmp = (unsigned char *)&tmp_mask;

	//把ip在输入框那里显示出来
	for (i=1;i<5;i++)
	{
		label[i] = gtk_label_new(char_label[i]);
		gtk_box_pack_start(GTK_BOX(hbox1), label[i], FALSE, FALSE, 15);
		adjustment[i] = gtk_adjustment_new(0.0,0.0,255.0,1.0,0.0,0.0);
		entry_mask.entry[i-1] = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment[i]),0.01,0);
		gtk_spin_button_set_value((GtkSpinButton *)entry_mask.entry[i-1],*tmp);
		gtk_box_pack_start(GTK_BOX(hbox2), entry_mask.entry[i-1], TRUE, TRUE, 1);
		tmp++;
	}

	//用户点击对话上面的按钮
	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_mask), (gpointer)&entry_mask);/*确定 or 取消*/

	gtk_widget_show_all(dialog);
}

static void da_call_time (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    _my_time_set_p entry_time_p = (struct __my_time_set *)user_data;
    if (GTK_RESPONSE_OK == response_id) {
        /* 点击了确认 */
        time_t t = core_time();
        struct tm *tm = localtime(&t);

		//读取输入框里面的数据
        tm->tm_hour = gtk_spin_button_get_value( (GtkSpinButton *)entry_time_p->entry[0] );
        tm->tm_min = gtk_spin_button_get_value( (GtkSpinButton *)entry_time_p->entry[1] );
        tm->tm_sec = gtk_spin_button_get_value( (GtkSpinButton *)entry_time_p->entry[2] );

        core_set_time(mktime(tm));

        //关闭对话框
        gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
		pp->pos_pos = MENU3_STOP  ;
		draw_menu3(0, NULL);
    } else if (GTK_RESPONSE_CANCEL == response_id) {
        //关闭对话框
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
    }
}

void draw_time()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox_first;	/* 指向dialog的vbox */
    static _my_time_set entry_time;
	time_t timep;
	struct tm *q;
    int time_tmp[3];
    int i;
	GtkObject *adjustment[4];
	GtkWidget *label[4];
	char *char_label[4];
	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;
    //新建对话框
	dialog = gtk_dialog_new_with_buttons ("Dialog_TIME", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 140);

	switch(get_language(pp->p_config))
	{
		case ENGLISH_:
			char_label[0] = "Time";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			break;
		case CHINESE_:
			char_label[0] = "时间";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			break;
		default:
			char_label[0] = "Time";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			break;
	}

	vbox_first = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

	hbox1 = gtk_hbox_new(TRUE, 0);
	hbox2 = gtk_hbox_new(FALSE, 0);
	hbox3 = gtk_hbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_first), hbox3, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox2, FALSE, FALSE, 5);

	label[0] = gtk_label_new(char_label[0]);
	gtk_box_pack_start(GTK_BOX(hbox3), label[0], TRUE, TRUE, 5);

    timep = core_time();
	q = localtime(&timep);
	time_tmp[0] = q->tm_hour;
	time_tmp[1] = q->tm_min;
	time_tmp[2] = q->tm_sec;

	for (i=1;i<4;i++)
	{
		label[i] = gtk_label_new(char_label[i]);
		gtk_box_pack_start(GTK_BOX(hbox1), label[i], FALSE, FALSE, 15);

        if ( i == 1 )
		    adjustment[i] = gtk_adjustment_new(0.0,0.0,23.0,1.0,0.0,0.0);
        else
		    adjustment[i] = gtk_adjustment_new(0.0,0.0,59.0,1.0,0.0,0.0);

		entry_time.entry[i-1] = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment[i]),0.01,0);
		gtk_spin_button_set_value((GtkSpinButton *)entry_time.entry[i-1],time_tmp[i-1]);
		gtk_box_pack_start(GTK_BOX(hbox2), entry_time.entry[i-1], TRUE, TRUE, 1);
	}

    g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_time), (gpointer)&entry_time);/*确定 or 取消*/

    gtk_widget_show_all(dialog);
}
/*闰年函数*/
int is_leap_year(int year)
{
    //计算是否指定的年份是否闰年,返回值为1则为闰年，返回值为0则为非闰年
    if (year % 4 == 0)
    {
        if (year % 100 != 0)
        {
            return 1;
        }
        else
        {
            if (year % 400 ==0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
}

void year_mon_changed(GtkAdjustment *adjustment,gpointer user_data)
{
    int mon = 0;
    int upper;
    int tmp_mday;
    int year;
    _year_mon_day_change_p change_information_p;
    gdouble mday_leap_year[12] = {31.0,29.0,31.0,30.0,31.0,30.0,31.0,31.0,30.0,31.0,30.0,31.0};
    gdouble mday_non_leap_year[12] = {31.0,28.0,31.0,30.0,31.0,30.0,31.0,31.0,30.0,31.0,30.0,31.0};
    change_information_p = (_year_mon_day_change_p)user_data;
    year = gtk_adjustment_get_value( change_information_p->year );
    mon = gtk_adjustment_get_value( change_information_p->mon );
    tmp_mday = gtk_adjustment_get_value( change_information_p->day );
    if (0 == is_leap_year(year))
        upper = mday_non_leap_year[mon-1];
    else
        upper = mday_leap_year[mon-1];
    gtk_adjustment_configure(change_information_p->day,0.0,1.0,upper,1.0,0.0,0.0);

    if (tmp_mday > upper)
        gtk_adjustment_set_value(change_information_p->day,upper);
    else
        gtk_adjustment_set_value(change_information_p->day,tmp_mday);
}

static void da_call_date (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    _my_date_set_p entry_date_p = (struct __my_date_set *)user_data;
    if (GTK_RESPONSE_OK == response_id) {
        time_t t = core_time();
        struct tm *tm = localtime(&t);

		//读取输入框里面的数据，就是年月日
        tm->tm_year = gtk_spin_button_get_value( (GtkSpinButton *)entry_date_p->entry[0]) - 1900;
        tm->tm_mon = gtk_spin_button_get_value( (GtkSpinButton *)entry_date_p->entry[1]) - 1;
        tm->tm_mday = gtk_spin_button_get_value( (GtkSpinButton *)entry_date_p->entry[2] );

        core_set_time(mktime(tm));

	    //关闭对话框
        gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
		pp->pos_pos = MENU3_STOP  ;
		draw_menu3(0, NULL);
    } else if (GTK_RESPONSE_CANCEL == response_id) {
        //关闭对话框
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
    }
}

//设置日期的对话框
void draw_date()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox_first;	/* 指向dialog的vbox */
    static _my_date_set entry_date;
	time_t timep;
	struct tm *q;
    int date_tmp[3];
    int i;
	GtkObject *adjustment[4];
	GtkWidget *label[4];
	char *char_label[4];
	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;
    static _year_mon_day_change change_information;
    //新建对话框
	dialog = gtk_dialog_new_with_buttons ("Dialog_DATE", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 140);

	switch(get_language(pp->p_config))
	{
		case ENGLISH_:
			char_label[0] = "DATE";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			break;
		case CHINESE_:
			char_label[0] = "日期";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			break;
		default:
			char_label[0] = "DATE";
			char_label[1] = "";
			char_label[2] = "";
			char_label[3] = "";
			break;
	}

	vbox_first = GTK_WIDGET (gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	hbox1 = gtk_hbox_new(TRUE, 0);
	hbox2 = gtk_hbox_new(FALSE, 0);
	hbox3 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox3, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox2, FALSE, FALSE, 5);
	label[0] = gtk_label_new(char_label[0]);
	gtk_box_pack_start(GTK_BOX(hbox3), label[0], TRUE, TRUE, 5);
    timep = core_time();
	q = localtime(&timep);
	date_tmp[0] = q->tm_year + 1900;
	date_tmp[1] = q->tm_mon + 1;
	date_tmp[2] = q->tm_mday;

	for (i=1;i<4;i++)
	{
		label[i] = gtk_label_new(char_label[i]);
		gtk_box_pack_start(GTK_BOX(hbox1), label[i], FALSE, FALSE, 15);

        if ( i == 1 ) //年
		    adjustment[i] = gtk_adjustment_new(0.0,2011.0,2099.0,1.0,0.0,0.0);
        else if (i == 2)  //月
		    adjustment[i] = gtk_adjustment_new(0.0,1.0,12.0,1.0,0.0,0.0);
        else if (i == 3)  //日
		    adjustment[i] = gtk_adjustment_new(0.0,1.0,31.0,1.0,0.0,0.0);

		entry_date.entry[i-1] = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment[i]),0.01,0);
		gtk_spin_button_set_value((GtkSpinButton *)entry_date.entry[i-1],date_tmp[i-1]);
		gtk_box_pack_start(GTK_BOX(hbox2), entry_date.entry[i-1], TRUE, TRUE, 1);
	}

    change_information.year = GTK_ADJUSTMENT(adjustment[1]);
    change_information.mon = GTK_ADJUSTMENT(adjustment[2]);
    change_information.day = GTK_ADJUSTMENT(adjustment[3]);

    g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_date), (gpointer)&entry_date);/*确定 or 取消*/

    g_signal_connect (G_OBJECT(adjustment[2]), "value_changed",
			G_CALLBACK(year_mon_changed), (gpointer)(&change_information));/**/

    g_signal_connect (G_OBJECT(adjustment[1]), "value_changed",
			G_CALLBACK(year_mon_changed), (gpointer)(&change_information));/*调节*/

    gtk_widget_show_all(dialog);
}


void draw_menu3_pop (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos )
{
	gint i;
	GtkMenu *menu;

	//update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
	for (i = 0; i < 20; i++)
	{
		if (pp->menu_dialog[i])
		{
			gtk_widget_destroy(pp->menu_dialog[i]);
			pp->menu_dialog[i] = NULL;
		}
		else
			break;
	}

	for (i = 0; i < qty; i++ )
	{
		pp->menu_dialog[i] = gtk_menu_item_new_with_label (content[i]);
		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_dialog[i]);
		g_signal_connect (pp->menu_dialog[i], "activate",
				G_CALLBACK(fun), (GUINT_TO_POINTER (i)));

		gtk_widget_show (pp->menu_dialog[i]);
	}
	gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_dialog[pop_pos]));

	menu = GTK_MENU (pp->menu3);

	gtk_menu_popup (menu, NULL, NULL,
			(GtkMenuPositionFunc)set_dialog_menu_position,
			NULL,
			0,
			gtk_get_current_event_time());

	return ;
}


/* draw_define_probe 选择 函数 */
static void signal_define_probe_select (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	_Open_UT_PA_Probe_File_P  open_ut_pa_probe_file_p = (_Open_UT_PA_Probe_File_P)user_data;
//	GtkTreeIter source_iter;
	GtkTreeSelection *source_selection;
//	GtkTreeModel *source_model;
	GtkWidget *source_list;
//	int value;
//	int _bItemSelected = FALSE;
	source_list = open_ut_pa_probe_file_p->list;
//	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp ;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		p_grp = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		//其实要指明一个临时的group
		p_grp = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}

	GtkTreeModel* model ;
	GtkTreeIter iter;
	if(gtk_tree_selection_get_selected(source_selection ,&model ,&iter))
	{
		char* filename;
		gtk_tree_model_get(model ,&iter ,LIST_ITEM ,&filename ,-1);
		strcpy(pTmpGroupData->probeType ,"user");
		strcpy(pTmpGroupData->probeFileName ,filename);
		free(filename);

		memcpy(&(p_grp->probe),&g_tmp_probe,sizeof(struct _Probe));
		set_group_val (p_grp , GROUP_FREQ_VAL, g_tmp_probe.Frequency);
		set_group_val (p_grp, GROUP_PW_VAL, get_pw() * PW_DIV);

		VerifyElementSelection(p_grp) ;

		gtk_label_set_text (GTK_LABEL (pp->data3[3]) ,p_grp->probe.Model);
		if (pp->pos)
		{
			GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
		    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
		    generate_focallaw (grp);
			RefreshBeamDelay(grp);
			RefreshGainOffset(grp);
			MultiGroupSendGroupSpi();
			MultiGroupSendFocalSpi();
			gtk_widget_queue_draw (pp->status_area);
			RefreshPrfMark();
			draw_area_all() ;
			RefreshScanInfor() ;
			pp->bRefreshDraw = TRUE ;
		}
	}
	//关闭对话框
	if (pp->sub_window)
	{
		gtk_widget_destroy (GTK_WIDGET (pp->sub_window));
		pp->sub_window = NULL;
		change_keypress_event (KEYPRESS_MAIN);
		//EnableKeyResponse(TRUE)  ;
		FreezingFPGA(FALSE) ;
	}
}


/* draw_define_probe 保存 函数 */
static void signal_define_probe_save (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	_Open_UT_PA_Probe_File_P  open_ut_pa_probe_file_p = (_Open_UT_PA_Probe_File_P)user_data;
	const gchar *buf;
	char *file_path = NULL;
	char file_name[256];
    int pos;

    char pa_pos[4] = {1,3,5,6};
	struct _Group * pGroup = NULL;
	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->model_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_probe.Model,buf);

		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->serial_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_probe.Serial,buf);

		pos = gtk_combo_box_get_active(GTK_COMBO_BOX(open_ut_pa_probe_file_p->probe_type_combo));
		if (pos < 0)
			return ;
		g_tmp_probe.PA_probe_type =  pa_pos[pos];
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->frequency_entry));
		g_tmp_probe.Frequency = (unsigned short)( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->element_qty_entry));
		g_tmp_probe.Elem_qty = atoi(buf);
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->ref_point_entry));
		g_tmp_probe.Reference_Point = (unsigned short)( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->pitch_entry));
		g_tmp_probe.Pitch = (unsigned int) ( atof(buf) * 1000 );
	}
	else //if (pGroup->group_mode == UT12_SCAN)
	{
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->model_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_probe.Model,buf);

		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->serial_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_probe.Serial,buf);
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->frequency_entry));
		g_tmp_probe.Frequency = (unsigned short) ( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_probe_file_p->element_size_entry));
		g_tmp_probe.Pitch = (unsigned int) ( atof(buf) * 1000 );
		g_tmp_probe.UT_probe_type = 1;
	}

	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		file_path = PA_PROBE_USER_PATH;
	}
	else
	{
		file_path = UT_PROBE_USER_PATH;
	}

	strcpy(file_name,file_path);
	strcat(file_name,g_tmp_probe.Model);
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		strcat(file_name,".opp");
	}
	else// if ( pGroup->group_mode == UT12_SCAN )
	{
		strcat(file_name,".oup");
	}
    // save file
	save_probe_file(file_name,&g_tmp_probe);
	// add title to the list view
	AppendItemToListView(open_ut_pa_probe_file_p->list, file_name + strlen(file_path))  ;

}

/*draw_define_probe 取消函数 */
static void signal_define_probe_cancel (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (pp->sub_window)
	{
		gtk_widget_destroy (GTK_WIDGET (pp->sub_window));
		pp->sub_window = NULL;
		change_keypress_event (KEYPRESS_MAIN);
		//EnableKeyResponse(TRUE) ;
		FreezingFPGA(FALSE) ;
	}
	if (pp->win_keyboard)
	{
		gtk_widget_destroy (pp->win_keyboard);
		pp->win_keyboard = NULL;
	}
}

static void signal_define_probe_delete (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	char *file_path = NULL;
	char file_name[256];

	GtkTreeIter source_iter;
	GtkTreeSelection *source_selection;
	GtkTreeModel *source_model;
	GtkWidget *source_list;
	int value;
	char *value_name;
    char cmd[256];

	_Open_UT_PA_Probe_File_P  open_ut_pa_probe_file_p = (_Open_UT_PA_Probe_File_P)data;

	struct _Group * pGroup = NULL;
	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	source_list = open_ut_pa_probe_file_p->list;
	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		file_path = PA_PROBE_USER_PATH;
    }
    else// if ( pGroup->group_mode == UT_SCAN )
    {
		file_path = UT_PROBE_USER_PATH;
    }
	while(value)
	{
		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
		{
			gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);
			memset(file_name,0,sizeof(file_name));
			strcpy(file_name,file_path);
			strcat(file_name,value_name);
			sprintf(cmd ,"rm \"%s\"" , file_name);
			system(cmd) ;
			gtk_list_store_remove(GTK_LIST_STORE(source_model), &source_iter);
			g_free(value_name);
			return ;
		}
		else
		{
			value = gtk_tree_model_iter_next(source_model,&source_iter);
		}
	}

}

int on_changed_open_ut_pa_probe_file(GtkTreeSelection *selection,	gpointer       data)
{
	char *file_path = NULL;
	char file_name[256];

	GtkTreeIter source_iter;
	GtkTreeSelection *source_selection;
	GtkTreeModel *source_model;
	GtkWidget *source_list;
	int value;
	char *value_name;
    char buf[256];
	int pa_pos[7] ={0,0,0,1,0,2,3};
	_Open_UT_PA_Probe_File_P  open_ut_pa_probe_file_p = (_Open_UT_PA_Probe_File_P)data;
	struct _Group * pGroup = NULL;
	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	source_list = open_ut_pa_probe_file_p->list;
	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		file_path = PA_PROBE_USER_PATH;
    }
    else// if ( pGroup->group_mode == UT_SCAN )
    {
		file_path = UT_PROBE_USER_PATH;
    }
	while(value)
	{
		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
		{
			gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);
			memset(file_name,0,sizeof(file_name));
			strcpy(file_name,file_path);
			strcat(file_name,value_name);
			read_probe_file(file_name,&g_tmp_probe);

			if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
			{
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->model_entry),g_tmp_probe.Model);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->serial_entry),g_tmp_probe.Serial);
				gtk_combo_box_set_active(GTK_COMBO_BOX(open_ut_pa_probe_file_p->probe_type_combo),pa_pos[g_tmp_probe.PA_probe_type]);
				sprintf(buf,"%.3f", g_tmp_probe.Frequency / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->frequency_entry),buf);
				sprintf(buf,"%d", g_tmp_probe.Elem_qty);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->element_qty_entry),buf);
				sprintf(buf,"%.3f",  g_tmp_probe.Reference_Point / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->ref_point_entry),buf);
				sprintf(buf,"%.3f",  g_tmp_probe.Pitch / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->pitch_entry),buf);
			}
			else
			{
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->model_entry),g_tmp_probe.Model);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->serial_entry),g_tmp_probe.Serial);
				sprintf(buf,"%.3f", g_tmp_probe.Frequency / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->frequency_entry),buf);
				sprintf(buf,"%.3f", g_tmp_probe.Pitch / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_probe_file_p->element_size_entry),buf);
			}
			value = gtk_tree_model_iter_next(source_model,&source_iter);
			g_free(value_name);
		}
		else
		{
			value = gtk_tree_model_iter_next(source_model,&source_iter);
		}
	}

	return TRUE;
}


static gboolean ProbeDefineWindowKeyHandler (GtkWidget* pWidget ,GdkEventKey* pEvent ,gpointer data)
{
	switch(pEvent->keyval)
	{
	case GDK_Return:
		pEvent->keyval = 0 ;
		signal_define_probe_save(NULL , NULL , data);
		break;
	case GDK_Escape:
		if (pp->win_keyboard)
		{
			gtk_widget_destroy (pp->win_keyboard);
			pp->win_keyboard = NULL;
		}
		else
		{
			pEvent->keyval = 0 ;
			signal_define_probe_cancel(NULL , NULL , data);
		}
		break;
	}
    return FALSE ;
}

void draw_define_probe()
{
	GtkWidget *vbox_first;
	GtkWidget *full_screen_box;
	GtkWidget *left_box;
	GtkWidget *right_box;
	GtkWidget *list_sw;
	GtkWidget *model_fixed;
    GtkWidget *model_entry;
	GtkWidget *model_label;
	GtkWidget *serial_fixed;
	GtkWidget *serial_entry;
	GtkWidget *serial_label;
	GtkWidget *probe_type_fixed;
	GtkWidget *probe_type_combo;
	GtkWidget *probe_type_label;
	GtkWidget *frequency_fixed;
	GtkWidget *frequency_entry;
	GtkWidget *frequency_label;
	GtkWidget *element_qty_fixed;
	GtkWidget *element_qty_entry;
	GtkWidget *element_qty_label;
	GtkWidget *ref_point_fixed;
	GtkWidget *ref_point_entry;
	GtkWidget *ref_point_label;
	GtkWidget *pitch_fixed;
	GtkWidget *pitch_entry;
	GtkWidget *pitch_label;
	GtkWidget *element_size_fixed;
	GtkWidget *element_size_entry;
	GtkWidget *element_size_label;
	GtkWidget *source_list;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkTreeSelection *source_selection;
	static _Open_UT_PA_Probe_File open_ut_pa_probe_file;
	struct _Group * pGroup = NULL;
	GtkWidget *button_select;
	GtkWidget *button_save;
	GtkWidget *button_cancel;
	GtkWidget *button_delete;

	GtkWidget *hbox;
	int _nLanguage = get_language(pp->p_config);
	pp->keyboard_xpos = 100;
	pp->keyboard_ypos = 350;

	if (pp->sub_window)
	{
		gtk_widget_destroy (GTK_WIDGET (pp->sub_window));
		pp->sub_window = NULL;
		//EnableKeyResponse(TRUE)  ;
		return ;
	}

	if (pp->pos != 0)
	{
		pGroup = get_group_by_id(pp->p_config,get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	pp->sub_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(GTK_WIDGET (pp->sub_window), 800, 600);
	gtk_window_set_decorated (GTK_WINDOW (pp->sub_window), FALSE);			/*不可以装饰*/
	gtk_window_set_transient_for(GTK_WINDOW(pp->sub_window), GTK_WINDOW(pp->window));//与主窗口关联
	gtk_window_set_resizable(GTK_WINDOW(pp->sub_window), FALSE);//禁止最大化
	vbox_first = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(pp->sub_window),vbox_first);
	//探头的类型的滚动窗口
	list_sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(GTK_WIDGET(list_sw),400, 550);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list_sw), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	//源文件列表
	source_list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(source_list), TRUE);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Probe Name", renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(source_list), column);
	store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(source_list), GTK_TREE_MODEL(store));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	full_screen_box = gtk_hbox_new(FALSE,0);
	left_box = gtk_vbox_new(FALSE,0);
	right_box = gtk_vbox_new(FALSE,0);
	// set label text
	model_label = gtk_label_new(strProbeDefineDialog[_nLanguage][0]);
	serial_label = gtk_label_new(strProbeDefineDialog[_nLanguage][1]);
	probe_type_label = gtk_label_new(strProbeDefineDialog[_nLanguage][2]);
	probe_type_combo = gtk_combo_box_new_text();
	gtk_widget_set_size_request(probe_type_combo , 150 , 30 );
	gtk_combo_box_append_text (GTK_COMBO_BOX (probe_type_combo), strProbeDefineDialog[_nLanguage][3]);
	gtk_combo_box_append_text (GTK_COMBO_BOX (probe_type_combo), strProbeDefineDialog[_nLanguage][4]);
	gtk_combo_box_append_text (GTK_COMBO_BOX (probe_type_combo), strProbeDefineDialog[_nLanguage][5]);
	gtk_combo_box_append_text (GTK_COMBO_BOX (probe_type_combo), strProbeDefineDialog[_nLanguage][6]);
	frequency_label = gtk_label_new(strProbeDefineDialog[_nLanguage][7]);
	element_qty_label = gtk_label_new(strProbeDefineDialog[_nLanguage][8]);
	ref_point_label = gtk_label_new(strProbeDefineDialog[_nLanguage][9]);
	pitch_label = gtk_label_new(strProbeDefineDialog[_nLanguage][10]);
	element_size_label = gtk_label_new(strProbeDefineDialog[_nLanguage][11]);
	button_select = gtk_button_new_with_label(strProbeDefineDialog[_nLanguage][12]);
	button_save = gtk_button_new_with_label(strProbeDefineDialog[_nLanguage][13])  ;
	button_cancel = gtk_button_new_with_label(strProbeDefineDialog[_nLanguage][14]);
	button_delete = gtk_button_new_with_label(strProbeDefineDialog[_nLanguage][15]);
	gtk_widget_set_size_request(button_select , 150 , 50 );
	gtk_widget_set_size_request(button_save ,   100 , 50 );
	gtk_widget_set_size_request(button_cancel , 100 , 50 );
	gtk_widget_set_size_request(button_delete , 150 , 50 );
	// create input widget
	model_fixed = gtk_fixed_new();
	model_entry = gtk_entry_new();

	gtk_fixed_put(GTK_FIXED(model_fixed),model_label,  5,0);
	gtk_fixed_put(GTK_FIXED(model_fixed),model_entry,150,0);
	serial_fixed = gtk_fixed_new();
	serial_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(serial_fixed),serial_label,5,0);
	gtk_fixed_put(GTK_FIXED(serial_fixed),serial_entry,150,0);
	probe_type_fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(probe_type_fixed),probe_type_label,5,0);
	gtk_fixed_put(GTK_FIXED(probe_type_fixed),probe_type_combo,150,0);
	frequency_fixed = gtk_fixed_new();
	frequency_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(frequency_fixed),frequency_label,5,0);
	gtk_fixed_put(GTK_FIXED(frequency_fixed),frequency_entry,150,0);
	element_qty_fixed = gtk_fixed_new();
	element_qty_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(element_qty_fixed),element_qty_label,5,0);
	gtk_fixed_put(GTK_FIXED(element_qty_fixed),element_qty_entry,150,0);
	ref_point_fixed = gtk_fixed_new();
	ref_point_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(ref_point_fixed),ref_point_label,5,0);
	gtk_fixed_put(GTK_FIXED(ref_point_fixed),ref_point_entry,150,0);
	pitch_fixed = gtk_fixed_new();
	pitch_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(pitch_fixed),pitch_label,5,0);
	gtk_fixed_put(GTK_FIXED(pitch_fixed),pitch_entry,150,0);
	element_size_fixed = gtk_fixed_new();
	element_size_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(element_size_fixed),element_size_label,5,0);
	gtk_fixed_put(GTK_FIXED(element_size_fixed),element_size_entry,150,0);
	open_ut_pa_probe_file.list = source_list;
	open_ut_pa_probe_file.model_entry = model_entry;
	open_ut_pa_probe_file.serial_entry = serial_entry;
	open_ut_pa_probe_file.probe_type_combo = probe_type_combo;
	open_ut_pa_probe_file.frequency_entry = frequency_entry;
	open_ut_pa_probe_file.element_qty_entry = element_qty_entry;
	open_ut_pa_probe_file.ref_point_entry = ref_point_entry;
	open_ut_pa_probe_file.pitch_entry = pitch_entry;
	open_ut_pa_probe_file.element_size_entry = element_size_entry;
	hbox = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox),button_select,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox),button_save,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox),button_cancel,FALSE,FALSE,5);
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		gtk_box_pack_start(GTK_BOX(right_box),model_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),serial_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),probe_type_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),frequency_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),element_qty_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),ref_point_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),pitch_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),hbox,FALSE,FALSE,5);

	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		gtk_box_pack_start(GTK_BOX(right_box),model_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),serial_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),frequency_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),element_size_fixed,FALSE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(right_box),hbox,FALSE,FALSE,5);
	}
	gtk_container_add(GTK_CONTAINER(list_sw), source_list);
	gtk_box_pack_start(GTK_BOX(left_box),list_sw,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(left_box),button_delete,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(full_screen_box),left_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(full_screen_box),right_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox_first),full_screen_box,FALSE,FALSE,0);
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		selection_file_type(source_list, PA_PROBE_USER_PATH,".opp",FALSE);
	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		selection_file_type(source_list, UT_PROBE_USER_PATH,".oup",FALSE);
	}

	g_signal_connect (G_OBJECT(button_select),"button-press-event",
				G_CALLBACK (signal_define_probe_select), (gpointer)&open_ut_pa_probe_file);
	g_signal_connect (G_OBJECT(button_save),"button-press-event",
				G_CALLBACK (signal_define_probe_save), (gpointer)&open_ut_pa_probe_file);
	g_signal_connect (G_OBJECT(button_cancel),"button-press-event",
				G_CALLBACK (signal_define_probe_cancel), (gpointer)&open_ut_pa_probe_file);
	g_signal_connect (G_OBJECT(button_delete),"button-press-event",
						G_CALLBACK (signal_define_probe_delete), (gpointer)&open_ut_pa_probe_file);
	g_signal_connect (G_OBJECT (source_selection), "changed", G_CALLBACK(on_changed_open_ut_pa_probe_file), (gpointer)&open_ut_pa_probe_file);
	g_signal_connect (G_OBJECT (pp->sub_window), "key-press-event", G_CALLBACK(ProbeDefineWindowKeyHandler), (gpointer)&open_ut_pa_probe_file);
	//显示窗口

	g_signal_connect_after (G_OBJECT(model_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(serial_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(frequency_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(element_qty_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(ref_point_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(pitch_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(element_size_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
#if ARM
	gtk_window_fullscreen(GTK_WINDOW(pp->sub_window));
#endif
	gtk_window_set_keep_above(GTK_WINDOW(pp->sub_window) , TRUE);
	gtk_widget_show_all(pp->sub_window);
	//EnableKeyResponse(TRUE)  ;
	FreezingFPGA(TRUE) ;

}

/*draw_define_wedge 选择 函数 */
static void signal_define_wedge_select (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	_Open_UT_PA_Probe_File_P  open_ut_pa_probe_file_p = (_Open_UT_PA_Probe_File_P)user_data;

//	GtkTreeIter source_iter;
	GtkTreeSelection *source_selection;
//	GtkTreeModel *source_model;
	GtkWidget *source_list;
//	int value;
//	int _bItemSelected = FALSE;
	source_list = open_ut_pa_probe_file_p->list;
//	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
//	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
//	while(value)
//	{
//		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
//		{
//			_bItemSelected = TRUE ;
//		}
//		value = gtk_tree_model_iter_next(source_model,&source_iter);
//	}
	struct _Group * pGroup = NULL;
	struct groupStruct* pTmpGroupData;
	if (pp->pos != 0)
	{
		int grp = get_current_group(pp->p_config);
		pGroup = get_group_by_id (pp->p_config, grp);
		pTmpGroupData = &gGroupData[grp];
	}
	else
	{
		//其实要指明一个临时的group
		pGroup = &g_tmp_group_struct;
		pTmpGroupData = &gData->tmpGroupData;
	}

	GtkTreeModel* model ;
	GtkTreeIter iter;
	if(gtk_tree_selection_get_selected(source_selection ,&model ,&iter))
//	if(_bItemSelected)
	{
		char* filename;
		gtk_tree_model_get(model ,&iter ,LIST_ITEM ,&filename ,-1);
		strcpy(pTmpGroupData->wedgeType ,"user");
		strcpy(pTmpGroupData->wedgeFileName ,filename);
		free(filename);

		memcpy(&(pGroup->wedge),&g_tmp_wedge,sizeof(struct _Wedge));
		//gtk_label_set_text (GTK_LABEL (pp->data3[4]),pGroup->wedge.Model);
		// refresh focallaw
		if (pp->pos != 0)
		{
			int grp = get_current_group(pp->p_config);
			GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
		    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
		    generate_focallaw (grp);
			RefreshBeamDelay(grp);
			RefreshGainOffset(grp);
			MultiGroupSendGroupSpi();
			MultiGroupSendFocalSpi();
			gtk_widget_queue_draw (pp->status_area);
			RefreshPrfMark();
			draw_area_all() ;
			RefreshScanInfor() ;
			pp->bRefreshDraw = TRUE ;
		}
	}
	if (pp->sub_window)
	{
		gtk_widget_destroy (GTK_WIDGET (pp->sub_window));
		pp->sub_window = NULL;
		change_keypress_event (KEYPRESS_MAIN);
		//EnableKeyResponse(TRUE);
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	FreezingFPGA(FALSE) ;
}

/*draw_define_wedge 保存 函数 */
static void signal_define_wedge_save (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{

	_Open_UT_PA_Wedge_File_P  open_ut_pa_wedge_file_p = (_Open_UT_PA_Wedge_File_P)user_data;
	const gchar *buf;
	char *file_path = NULL;
	char file_name[256];
	int pos;
	struct _Group * pGroup = NULL;
	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}

	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->model_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_wedge.Model,buf);
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->serial_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_wedge.Serial,buf);
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->angle_entry));
		g_tmp_wedge.Angle = (unsigned short) ( atof(buf) * 10 );
		pos =  gtk_combo_box_get_active(GTK_COMBO_BOX(open_ut_pa_wedge_file_p->orientation_combo));
		if (pos < 0)
			return ;
		g_tmp_wedge.Orientation =  pos;
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->velocity_entry));
		g_tmp_wedge.Velocity_PA = (unsigned int) ( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->pri_offset_entry));
		g_tmp_wedge.Primary_offset = (int) ( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->sec_offset_entry));
		g_tmp_wedge.Secondary_offset = (unsigned int) ( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->height_entry));
		g_tmp_wedge.Height = (unsigned int) ( atof(buf) * 1000 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->length_entry));
		g_tmp_wedge.length = (unsigned int) ( atof(buf) * 1000 );
	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->model_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_wedge.Model,buf);
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->serial_entry));
		if (strcmp(buf,"") == 0)
			return ;
		strcpy(g_tmp_wedge.Serial,buf);
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->angle_entry));
		g_tmp_wedge.Angle = (unsigned short) ( atof(buf) * 10 );
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->probe_delay_entry));
		g_tmp_wedge.Probe_delay = (unsigned short) ( atof(buf) * 1000 );
		pos = gtk_combo_box_get_active(GTK_COMBO_BOX(open_ut_pa_wedge_file_p->wave_type_combo)) + 1;
		if ( pos < 0 )
			return;
		g_tmp_wedge.Wave_type = pos;
		buf = gtk_entry_get_text(GTK_ENTRY(open_ut_pa_wedge_file_p->ref_point_entry));
		g_tmp_wedge.Ref_point = (int) ( atof(buf) * 1000 );
	}
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		file_path = PA_WEDGE_USER_PATH;
	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		file_path = UT_WEDGE_USER_PATH;
	}
	strcpy(file_name,file_path);
	strcat(file_name,g_tmp_wedge.Model);
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		strcat(file_name,".opw");
	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		strcat(file_name,".ouw");
	}
	save_wedge_file(file_name,&g_tmp_wedge);
	// add title to the list view
	AppendItemToListView(open_ut_pa_wedge_file_p->list, file_name + strlen(file_path))  ;
}

/*draw_define_wedge 取消 函数 */
static void signal_define_wedge_cancel (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
		if (pp->sub_window)
		{
			gtk_widget_destroy (GTK_WIDGET (pp->sub_window));
			pp->sub_window = NULL;
			change_keypress_event (KEYPRESS_MAIN);
			//EnableKeyResponse(TRUE);
		}
		if (pp->win_keyboard)
		{
			gtk_widget_destroy (pp->win_keyboard);
			pp->win_keyboard = NULL;
		}

		FreezingFPGA(FALSE) ;

}
static void signal_define_wedge_delete (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	char *file_path = NULL;
	char file_name[256];

	GtkTreeIter source_iter;
	GtkTreeSelection *source_selection;
	GtkTreeModel *source_model;
	GtkWidget *source_list;
	int value;
	char *value_name;
    char cmd[256];

    _Open_UT_PA_Wedge_File_P  open_ut_pa_wedge_file_p = (_Open_UT_PA_Wedge_File_P)data;

	struct _Group * pGroup = NULL;
	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	source_list = open_ut_pa_wedge_file_p->list;
	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		file_path = PA_WEDGE_USER_PATH;
    }
    else// if ( pGroup->group_mode == UT_SCAN )
    {
		file_path = UT_WEDGE_USER_PATH;
    }
	while(value)
	{
		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
		{
			gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);
			memset(file_name,0,sizeof(file_name));
			strcpy(file_name,file_path);
			strcat(file_name,value_name);
			sprintf(cmd ,"rm \"%s\"" , file_name);
			system(cmd) ;
			gtk_list_store_remove(GTK_LIST_STORE(source_model), &source_iter);
			g_free(value_name);
			return ;
		}
		else
		{
			value = gtk_tree_model_iter_next(source_model,&source_iter);
		}
	}

}

void on_changed_open_ut_pa_wedge_file(GtkTreeSelection *selection,	gpointer       data)
{
	char *file_path = NULL;
	char file_name[256];
	GtkTreeIter source_iter;
	GtkTreeSelection *source_selection;
	GtkTreeModel *source_model;
	GtkWidget *source_list;
	int value;
	char *value_name;
    char buf[256];
	_Open_UT_PA_Wedge_File_P  open_ut_pa_wedge_file_p = (_Open_UT_PA_Wedge_File_P)data;
	struct _Group * pGroup = NULL;
	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	source_list = open_ut_pa_wedge_file_p->list;
	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		file_path = PA_WEDGE_USER_PATH;
    }
    else// if ( pGroup->group_mode == UT_SCAN )
    {
		file_path = UT_WEDGE_USER_PATH;
    }
	while(value)
	{
		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
		{
			gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);
			memset(file_name,0,sizeof(file_name));
			strcpy(file_name,file_path);
			strcat(file_name,value_name);

			read_wedge_file(file_name,&g_tmp_wedge)  ;

            if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
			{
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->model_entry),g_tmp_wedge.Model);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->serial_entry),g_tmp_wedge.Serial);
				sprintf(buf,"%.1f",g_tmp_wedge.Angle / 10.0 );
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->angle_entry),buf);
				gtk_combo_box_set_active(GTK_COMBO_BOX(open_ut_pa_wedge_file_p->orientation_combo),g_tmp_wedge.Orientation);
				sprintf(buf,"%.3f",g_tmp_wedge.Velocity_PA / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->velocity_entry),buf);
				sprintf(buf,"%.3f",g_tmp_wedge.Primary_offset / 1000.0 );
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->pri_offset_entry),buf);
				sprintf(buf,"%.3f",g_tmp_wedge.Secondary_offset / 1000.0 );
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->sec_offset_entry),buf);
				sprintf(buf,"%.3f",g_tmp_wedge.Height / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->height_entry),buf);
				sprintf(buf,"%.3f",g_tmp_wedge.length / 1000.0);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->length_entry), buf);
			}
			else
			{
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->model_entry),g_tmp_wedge.Model);
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->serial_entry),g_tmp_wedge.Serial);
				sprintf(buf,"%.1f",g_tmp_wedge.Angle / 10.0 );
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->angle_entry),buf);
				sprintf(buf,"%.3f",g_tmp_wedge.Probe_delay / 1000.0 );
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->probe_delay_entry),buf);
				gtk_combo_box_set_active(GTK_COMBO_BOX(open_ut_pa_wedge_file_p->wave_type_combo),g_tmp_wedge.Wave_type - 1);
				sprintf(buf,"%.3f",g_tmp_wedge.Ref_point / 1000.0 );
				gtk_entry_set_text(GTK_ENTRY(open_ut_pa_wedge_file_p->ref_point_entry),buf);
			}
			value = gtk_tree_model_iter_next(source_model,&source_iter);
			g_free(value_name);
		}
		else
		{
			value = gtk_tree_model_iter_next(source_model,&source_iter);
		}
	}

}


static gboolean WedgeDefineWindowKeyHandler (GtkWidget* pWidget ,GdkEventKey* pEvent ,gpointer data)
{
	switch(pEvent->keyval)
	{
	case GDK_Return:
		pEvent->keyval = 0 ;
		signal_define_wedge_save(NULL , NULL , data);
		break;
	case GDK_Escape:
		if (pp->win_keyboard)
		{
			gtk_widget_destroy (pp->win_keyboard);
			pp->win_keyboard = NULL;
		}
		else
		{
			pEvent->keyval = 0 ;
			signal_define_wedge_cancel(NULL , NULL , data);
		}
		break;
	}
    return FALSE ;
}



void draw_define_wedge()
{
	GtkWidget *vbox_first;
	GtkWidget *full_screen_box;
	GtkWidget *left_box;
	GtkWidget *right_box;
	GtkWidget *list_sw;
	GtkWidget *model_fixed;
    GtkWidget *model_entry;
	GtkWidget *model_label;
	GtkWidget *serial_fixed;
	GtkWidget *serial_entry;
	GtkWidget *serial_label;
	GtkWidget *angle_fixed;
	GtkWidget *angle_label;
	GtkWidget *angle_entry;
	GtkWidget *orientation_fixed;
	GtkWidget *orientation_label;
	GtkWidget *orientation_combo;
	GtkWidget *velocity_fixed;
	GtkWidget *velocity_label;
	GtkWidget *velocity_entry;
	GtkWidget *pri_offset_fixed;
	GtkWidget *pri_offset_label;
	GtkWidget *pri_offset_entry;
	GtkWidget *sec_offset_fixed;
	GtkWidget *sec_offset_label;
	GtkWidget *sec_offset_entry;
	GtkWidget *height_fixed;
	GtkWidget *height_label;
	GtkWidget *height_entry;
	GtkWidget *probe_delay_fixed;
	GtkWidget *probe_delay_label;
	GtkWidget *probe_delay_entry;
	GtkWidget *wave_type_fixed;
	GtkWidget *wave_type_label;
	GtkWidget *wave_type_combo;
	GtkWidget *ref_point_fixed;
	GtkWidget *ref_point_label;
	GtkWidget *ref_point_entry;
	GtkWidget *length_fixed;
	GtkWidget *length_label;
	GtkWidget *length_entry;
	GtkWidget *source_list;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkTreeSelection *source_selection;
	static _Open_UT_PA_Wedge_File open_ut_pa_wedge_file;
	struct _Group * pGroup = NULL;
	GtkWidget *button_select;
	GtkWidget *button_save;
	GtkWidget *button_cancel;
	GtkWidget *button_delete;
	GtkWidget *hbox;
	if (pp->sub_window)
	{
		gtk_widget_destroy (GTK_WIDGET (pp->sub_window));
		pp->sub_window = NULL;
		//EnableKeyResponse(TRUE);
		return ;
	}
	int _nLanguage = get_language(pp->p_config)  ;
	pp->keyboard_xpos = 100;
	pp->keyboard_ypos = 350;
	pp->sub_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(GTK_WIDGET (pp->sub_window), 800, 600);
	gtk_window_set_decorated (GTK_WINDOW (pp->sub_window), FALSE);			/*不可以装饰*/
	gtk_window_set_transient_for(GTK_WINDOW(pp->sub_window), GTK_WINDOW(pp->window));//与主窗口关联
	gtk_window_set_resizable(GTK_WINDOW(pp->sub_window), FALSE);//禁止最大化
	vbox_first = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(pp->sub_window), vbox_first);

	if (pp->pos != 0)
	{
		pGroup = get_group_by_id (pp->p_config, get_current_group(pp->p_config));
	}
	else
	{
		pGroup = &g_tmp_group_struct;
	}
	//契块的类型的滚动窗口
	list_sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(GTK_WIDGET(list_sw),400,550);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list_sw), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	//源文件列表
	source_list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(source_list), TRUE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Wedge Name",renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(source_list), column);

	store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(source_list), GTK_TREE_MODEL(store));
	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	full_screen_box = gtk_hbox_new(FALSE,0);
	left_box = gtk_vbox_new(FALSE,0);
	right_box = gtk_vbox_new(FALSE,0);

	model_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][0]);
	serial_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][1]);
	angle_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][2]);
	orientation_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][3]);
	orientation_combo = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (orientation_combo), strWedgeDefineDialog[_nLanguage][4]);
	gtk_combo_box_append_text (GTK_COMBO_BOX (orientation_combo), strWedgeDefineDialog[_nLanguage][5]);
	velocity_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][6]);
	pri_offset_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][7]);
	sec_offset_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][8]);
	height_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][9]);
	probe_delay_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][10]);
	wave_type_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][11]);
	wave_type_combo = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (wave_type_combo), strWedgeDefineDialog[_nLanguage][12]);
	gtk_combo_box_append_text (GTK_COMBO_BOX (wave_type_combo), strWedgeDefineDialog[_nLanguage][13]);
	ref_point_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][14]);
	button_select = gtk_button_new_with_label(strWedgeDefineDialog[_nLanguage][15]);
	button_save = gtk_button_new_with_label(strWedgeDefineDialog[_nLanguage][16]);
	button_cancel = gtk_button_new_with_label(strWedgeDefineDialog[_nLanguage][17]);
	button_delete = gtk_button_new_with_label(strWedgeDefineDialog[_nLanguage][18]);

	length_label = gtk_label_new(strWedgeDefineDialog[_nLanguage][19]);

	model_fixed = gtk_fixed_new();
	model_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(model_fixed),model_label,5,0);
	gtk_fixed_put(GTK_FIXED(model_fixed),model_entry,150,0);

	serial_fixed = gtk_fixed_new();
	serial_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(serial_fixed),serial_label,5,0);
	gtk_fixed_put(GTK_FIXED(serial_fixed),serial_entry,150,0);

	angle_fixed = gtk_fixed_new();
	angle_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(angle_fixed),angle_label,5,0);
	gtk_fixed_put(GTK_FIXED(angle_fixed),angle_entry,150,0);

	orientation_fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(orientation_fixed),orientation_label,5,0);
	gtk_fixed_put(GTK_FIXED(orientation_fixed),orientation_combo,150,0);

	velocity_fixed = gtk_fixed_new();
	velocity_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(velocity_fixed),velocity_label,5,0);
	gtk_fixed_put(GTK_FIXED(velocity_fixed),velocity_entry,150,0);

	pri_offset_fixed = gtk_fixed_new();
	pri_offset_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(pri_offset_fixed),pri_offset_label,5,0);
	gtk_fixed_put(GTK_FIXED(pri_offset_fixed),pri_offset_entry,150,0);

	sec_offset_fixed = gtk_fixed_new();
	sec_offset_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(sec_offset_fixed),sec_offset_label,5,0);
	gtk_fixed_put(GTK_FIXED(sec_offset_fixed),sec_offset_entry,150,0);

	height_fixed = gtk_fixed_new();
	height_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(height_fixed),height_label,5,0);
	gtk_fixed_put(GTK_FIXED(height_fixed),height_entry,150,0);

	probe_delay_fixed = gtk_fixed_new();
	probe_delay_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(probe_delay_fixed),probe_delay_label,5,0);
	gtk_fixed_put(GTK_FIXED(probe_delay_fixed),probe_delay_entry,150,0);

	wave_type_fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(wave_type_fixed),wave_type_label,5,0);
	gtk_fixed_put(GTK_FIXED(wave_type_fixed),wave_type_combo,150,0);

	ref_point_fixed = gtk_fixed_new();
	ref_point_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(ref_point_fixed),ref_point_label,5,0);
	gtk_fixed_put(GTK_FIXED(ref_point_fixed),ref_point_entry,150,0);

	length_fixed = gtk_fixed_new();
	length_entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(length_fixed), length_label, 5,0);
	gtk_fixed_put(GTK_FIXED(length_fixed), length_entry, 150,0);

	hbox = gtk_hbox_new(FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),button_select,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox),button_save,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox),button_cancel,FALSE,FALSE,5);
	gtk_widget_set_size_request(button_select , 150 , 30 );
	gtk_widget_set_size_request(button_save ,   100 , 30 );
	gtk_widget_set_size_request(button_cancel , 100 , 30 );
	gtk_widget_set_size_request(button_delete , 150 , 50 );

	open_ut_pa_wedge_file.list  = source_list;
	open_ut_pa_wedge_file.model_entry = model_entry;
	open_ut_pa_wedge_file.serial_entry = serial_entry;
	open_ut_pa_wedge_file.angle_entry = angle_entry;
	open_ut_pa_wedge_file.orientation_combo = orientation_combo;
	open_ut_pa_wedge_file.velocity_entry = velocity_entry;
	open_ut_pa_wedge_file.pri_offset_entry = pri_offset_entry;
	open_ut_pa_wedge_file.sec_offset_entry = sec_offset_entry;
	open_ut_pa_wedge_file.height_entry = height_entry;
	open_ut_pa_wedge_file.probe_delay_entry = probe_delay_entry;
	open_ut_pa_wedge_file.wave_type_combo = wave_type_combo;
	open_ut_pa_wedge_file.ref_point_entry = ref_point_entry;
	open_ut_pa_wedge_file.length_entry = length_entry;

	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		gtk_box_pack_start(GTK_BOX(right_box),model_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),serial_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),angle_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),orientation_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),velocity_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),pri_offset_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),sec_offset_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),height_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),length_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),hbox,FALSE,FALSE,3);
	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		gtk_box_pack_start(GTK_BOX(right_box),model_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),serial_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),angle_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),probe_delay_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),wave_type_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),ref_point_fixed,FALSE,FALSE,3);
		gtk_box_pack_start(GTK_BOX(right_box),hbox,FALSE,FALSE,3);
	}

	gtk_container_add(GTK_CONTAINER(list_sw), source_list);
	gtk_box_pack_start(GTK_BOX(left_box),list_sw,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(left_box),button_delete, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(full_screen_box),left_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(full_screen_box),right_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox_first),full_screen_box,FALSE,FALSE,0);

	if (pGroup->group_mode == PA_SCAN ||pGroup->group_mode == UT_SCAN)
	{
		selection_file_type(source_list, PA_WEDGE_USER_PATH,	".opw",FALSE);
	}
	else// if ( pGroup->group_mode == UT_SCAN )
	{
		selection_file_type(source_list, UT_WEDGE_USER_PATH,	".ouw",FALSE);
	}

	g_signal_connect (G_OBJECT(button_select),"button-press-event",
				G_CALLBACK (signal_define_wedge_select), (gpointer)&open_ut_pa_wedge_file);
	g_signal_connect (G_OBJECT(button_save),"button-press-event",
				G_CALLBACK (signal_define_wedge_save), (gpointer)&open_ut_pa_wedge_file);
	g_signal_connect (G_OBJECT(button_cancel),"button-press-event",
				G_CALLBACK (signal_define_wedge_cancel), (gpointer)&open_ut_pa_wedge_file);
	g_signal_connect (G_OBJECT(button_delete),"button-press-event",
					G_CALLBACK (signal_define_wedge_delete), (gpointer)&open_ut_pa_wedge_file);
	g_signal_connect (G_OBJECT (source_selection), "changed", G_CALLBACK(on_changed_open_ut_pa_wedge_file), (gpointer)&open_ut_pa_wedge_file);
	g_signal_connect (G_OBJECT (pp->sub_window), "key-press-event", G_CALLBACK(WedgeDefineWindowKeyHandler), (gpointer)&open_ut_pa_wedge_file);

	g_signal_connect_after (G_OBJECT(model_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(serial_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(angle_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(velocity_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(pri_offset_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(sec_offset_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(height_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(probe_delay_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(ref_point_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);
	g_signal_connect_after (G_OBJECT(length_entry),"focus-in-event",G_CALLBACK (DisplayInputKeyBoard), NULL);

	//显示窗口
#if ARM
	gtk_window_fullscreen(GTK_WINDOW(pp->sub_window));
#endif
	gtk_window_set_keep_above(GTK_WINDOW(pp->sub_window) , TRUE);
	gtk_widget_show_all(pp->sub_window);
	//EnableKeyResponse(FALSE);
	FreezingFPGA(TRUE) ;

}

/*
 * 弹出的dialog
 * 0 记事本 备注等等
 * 1 探头 
 * 2 楔块 
 * 3 自定义探头
 * 4 自定义楔块
 * 5 
 * 13 聚焦法则保存
 * 6 配置 数据 报告 图像 的读入
 * 7 保存 配置文件
 * 8 系统信息的显示 
 * 9 报告的显示
 * 10 Export Table
 * 11 调色板
 * 12 文件管理
 * 13 聚集法则的保存
 * 14 聚集法则的读入
 * 15 
 */
void draw_dialog_all (guint type)
{
	change_keypress_event (KEYPRESS_DIALOG);
	pp->dialog_pos = type;
	switch (type)
	{
	//	case DIALOG_EDIT_NOTES: draw_edit_notes(); break;
	//	case DIALOG_EDIT_HEADER: draw_edit_header(); break;
		case DIALOG_PROBE:  draw_probe(); break;
		case DIALOG_WEDGE:  draw_wedge(); break;
	//	case DIALOG_FILE_OPEN:  draw_file_open_main(); break;
	//	case DIALOG_SAVE_SETUP_AS:  draw_save_setup_as(); break;
		case DIALOG_SYSTEM_INFO:  draw_system_info(); break;
		case DIALOG_COLOR_PALETTE:  draw_color_palette(); break;
	//	case DIALOG_FILE_MANAGE:	draw_file_manage(); break;
		case DIALOG_LAW_SAVE:	draw_law_save();break;
		case DIALOG_LAW_READ:	draw_law_read();break;
		case DIALOG_IP:		draw_ip();break;
		case DIALOG_MASK:	draw_mask();break;
        case DIALOG_TIME:	draw_time();break;
        case DIALOG_DATE:	draw_date();break;
     //   case DIALOG_REPORT_BUILD:	draw_report_build();break;
	//	case DIALOG_FILE_NAME:	draw_file_name();break;
		case DIALOG_DEFINE_PROBE:	draw_define_probe();break;
	//	case DIALOG_USERFIELD_LABEL: draw_userfield_label(); break;
	//	case DIALOG_USERFIELD_CONTENT: draw_userfield_content(); break;
		case DIALOG_DEFINE_WEDGE:	draw_define_wedge();break;
	//	case DIALOG_FILE_NAME_ALL:	draw_file_name_all();break;
		default:break;
	}

}










