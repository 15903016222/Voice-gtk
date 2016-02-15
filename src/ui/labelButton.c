/*
 * labelButton.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */
#include <gtk/gtk.h>
#include <malloc.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>

#include "labelbutton.h"
#include "../drawui.h"
void update_widget_bg(GtkWidget *widget,int i);
static GdkColor	g_ColorWhite     = {0x0, 0xffff, 0xffff, 0xffff};

static void popup(GtkWidget* widget);
static void popdown(GtkWidget* widget);
static void setBackground(GtkWidget* widget ,LABELBUTTON_STATUS_ENUM status);

static void callbackDestroy(GtkWidget* widget)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	if(labelButtonPara->popPara)
	{
		free(labelButtonPara->popPara->label);
		free(labelButtonPara->popPara->data);
		free(labelButtonPara->popPara);
	}
	free(labelButtonPara);
}

void labelButtoncallbackPress(GtkWidget* widget)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	setBackground(widget ,LABELBUTTON_STATUS_PRESS);
	gtk_widget_grab_focus(widget);
	if(LABELBUTTON_FUNCTION_MODE_POP == labelButtonPara->mode)
		popup(widget);
	if(labelButtonPara->callbackPress)
		labelButtonPara->callbackPress(labelButtonPara->parent ,labelButtonPara->callbackPara);
}

void labelButtoncallbackRelease(GtkWidget* widget)
{
	if(G_IS_OBJECT(widget))
	{
		labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
		if(labelButtonPara)
		{
			if(LABELBUTTON_FUNCTION_MODE_NORMAL == labelButtonPara->mode)
				setBackground(widget ,LABELBUTTON_STATUS_RELEASE);
		}
	}
}

static gboolean callbackLabelButton(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
//	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");

	switch((int)data)
	{
	case 1://按下
		labelButtoncallbackPress(widget);
		break;

	case 0://放开
		labelButtoncallbackRelease(widget);
		break;

	case 2://退出
		callbackDestroy(widget);
		break;
	}
	return FALSE;
}

static void callbackLabelButtonFocus(GtkWidget *widget ,GdkEventFocus *event ,gpointer user_data)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	if(user_data)
	{
		setBackground(widget ,LABELBUTTON_STATUS_RELEASE);
	}
	else
	{
		if(labelButtonPara->menu && (gtk_widget_get_visible(labelButtonPara->menu)))
		{
			setBackground(widget ,LABELBUTTON_STATUS_PRESS);
		}
		else
		{
			setBackground(widget ,LABELBUTTON_STATUS_NORMAL);
		}
	}
}

static gboolean callbackLabelButtonKeyHandler(GtkWidget *widget ,GdkEventKey *event ,gpointer user_data)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	switch(event->keyval)
	{
	case GDK_Return:
		if(LABELBUTTON_FUNCTION_MODE_POP == labelButtonPara->mode)
		{
			if(FALSE == labelButtonPara->menu)
			{
				popup(widget);
			}
			else
			{
				popdown(widget);
			}
		}
		if(labelButtonPara->callbackPress)
			labelButtonPara->callbackPress(labelButtonPara->parent ,labelButtonPara->callbackPara);
		break;
	case GDK_Escape:
		break;
	case GDK_Left:
	case GDK_Up:
		break;
	case GDK_Right:
	case GDK_Down:
		break;
	default:
		break;
	}
	return FALSE;
}

GtkWidget* labelButtonNew()
{
	GtkWidget* label ,*data;
	GtkWidget* eventLabel;
	GtkWidget* eventData ;
	GtkWidget* event;
	GtkWidget* box;
	labelButtonStruct* labelButtonPara;

	event = gtk_event_box_new();

	box = gtk_vbox_new(FALSE, 0);

	eventLabel = gtk_event_box_new();
	label = gtk_label_new(NULL);
	gtk_widget_modify_fg (label, GTK_STATE_NORMAL, &g_ColorWhite);
	gtk_container_add(GTK_CONTAINER(eventLabel), label);

	eventData = gtk_event_box_new();
	data = gtk_label_new("");
	gtk_widget_set_size_request(GTK_WIDGET(data), -1, 25);
	gtk_widget_modify_fg (data, GTK_STATE_NORMAL, &g_ColorWhite);
	gtk_container_add(GTK_CONTAINER(eventData), data);

	gtk_box_pack_start(GTK_BOX(box), eventLabel ,TRUE ,TRUE ,0);
	gtk_box_pack_start(GTK_BOX(box), eventData ,FALSE ,FALSE ,0);

	gtk_container_add(GTK_CONTAINER(event), box);
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), event ,TRUE ,TRUE ,0);

	labelButtonPara = (labelButtonStruct*)malloc(sizeof(labelButtonStruct));
	labelButtonPara->parent = NULL;
	labelButtonPara->event = event;
	labelButtonPara->data = data;
	labelButtonPara->eventData = eventData;
	labelButtonPara->eventLabel = eventLabel;
	labelButtonPara->label = label;
	labelButtonPara->menu = NULL;
	labelButtonPara->spin = NULL;
	labelButtonPara->mode = LABELBUTTON_FUNCTION_MODE_NORMAL;
	labelButtonPara->popPara = NULL;
	labelButtonPara->isSpecialButton = FALSE;
	labelButtonPara->callbackPress = NULL;
	labelButtonPara->callbackPara = NULL;
	labelButtonPara->menuPopStatus = FALSE;

	g_signal_connect(G_OBJECT(vbox), "button-press-event", G_CALLBACK(callbackLabelButton), (gpointer)1);
	g_signal_connect(G_OBJECT(vbox), "button-release-event", G_CALLBACK(callbackLabelButton), (gpointer)0);
	g_signal_connect(G_OBJECT(vbox), "destroy", G_CALLBACK(callbackLabelButton), (gpointer)2);
	g_signal_connect(G_OBJECT(vbox), "focus-in-event", G_CALLBACK(callbackLabelButtonFocus), (gpointer)1);
	g_signal_connect(G_OBJECT(vbox), "focus-out-event", G_CALLBACK(callbackLabelButtonFocus), (gpointer)0);
//	g_signal_connect(G_OBJECT(vbox), "key-press-event", G_CALLBACK(callbackLabelButtonKeyHandler), (gpointer)1);
	g_signal_connect(G_OBJECT(vbox), "key-release-event", G_CALLBACK(callbackLabelButtonKeyHandler), (gpointer)0);

	gtk_widget_set_size_request(GTK_WIDGET(vbox), 114, 85);
	g_object_set_data(G_OBJECT(vbox) ,"labelButtonPara" ,labelButtonPara);
	setBackground(vbox ,LABELBUTTON_STATUS_NORMAL);
	return vbox;
}

void labelButtonSetSpecial(GtkWidget* widget ,int isSpecialButton)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	if(isSpecialButton)
		gtk_widget_set_size_request(GTK_WIDGET(widget), 342, 85);
	else
		gtk_widget_set_size_request(GTK_WIDGET(widget), 114, 85);
	labelButtonPara->isSpecialButton = isSpecialButton;
	g_object_set_data(G_OBJECT(widget) ,"labelButtonPara" ,labelButtonPara);

	setBackground(widget ,LABELBUTTON_STATUS_NORMAL);
}

void labelButtonInstallCallback(GtkWidget* widget ,void(*func)(GtkWidget* ,void*) ,GtkWidget* dialog ,void* para)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	labelButtonPara->callbackPress = func;
	labelButtonPara->callbackPara = para;
	labelButtonPara->parent = dialog;
	g_object_set_data(G_OBJECT(widget) ,"labelButtonPara" ,(gpointer)labelButtonPara);
}

void labelButtonSetDisplayMode(GtkWidget* widget ,LABELBUTTON_MODE_ENUM mode)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	gtk_widget_set_sensitive(widget ,TRUE);
	switch(mode)
	{
	case LABELBUTTON_MODE_NONE:
	case LABELBUTTON_MODE_HIDE:
		gtk_widget_hide(labelButtonPara->event);
		gtk_widget_set_can_focus(widget ,FALSE);
		break;
	case LABELBUTTON_MODE_DATA3:
		gtk_widget_show_all(widget);
		//gtk_widget_hide(labelButtonPara->spin);
		gtk_widget_set_can_focus(widget ,TRUE);
		break;
	case LABELBUTTON_MODE_BIGMENU:
		gtk_widget_show_all(widget);
		//gtk_widget_hide(labelButtonPara->spin);
		gtk_widget_hide(labelButtonPara->data);
		gtk_widget_set_can_focus(widget ,TRUE);
		break;
	case LABELBUTTON_MODE_SPINBUTTON:
		//gtk_widget_show_all(widget);
		//gtk_widget_hide(labelButtonPara->spin);
		//gtk_widget_hide(labelButtonPara->data);
		//gtk_widget_set_can_focus(widget ,TRUE);
		break;
	case LABELBUTTON_MODE_DISABLE:
		gtk_widget_set_sensitive(widget ,FALSE);
		gtk_widget_set_can_focus(widget ,FALSE);
		break;
	default:
		break;
	}
}

void labelButtonSetData(GtkWidget* widget ,const char* str)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->data) ,str);
}

void labelButtonSetLabel(GtkWidget* widget ,const char* str)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	gtk_label_set_label(GTK_LABEL(labelButtonPara->label) ,str);
}

void setBackground(GtkWidget* widget ,LABELBUTTON_STATUS_ENUM status)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	switch(status)
	{
	case LABELBUTTON_STATUS_PRESS:
		if(labelButtonPara->isSpecialButton)
		{
			update_widget_bg (labelButtonPara->eventLabel, /*backpic[8]*/ 17);
			update_widget_bg (labelButtonPara->eventData, /*backpic[14]*/ 17);
		}
		else
		{
			update_widget_bg (labelButtonPara->eventLabel, /*backpic[8]*/ 6);
			update_widget_bg (labelButtonPara->eventData, /*backpic[14]*/ 11);
		}
		break;
	case LABELBUTTON_STATUS_RELEASE:
		if(labelButtonPara->isSpecialButton)
		{
			update_widget_bg (labelButtonPara->eventLabel, /*backpic[8]*/ 17);
			update_widget_bg (labelButtonPara->eventData, /*backpic[14]*/ 17);
		}
		else
		{
			update_widget_bg (labelButtonPara->eventLabel, /*backpic[8]*/ 8);
			update_widget_bg (labelButtonPara->eventData, /*backpic[14]*/ 11);
		}
		break;
	case LABELBUTTON_STATUS_NORMAL:
	default:
		if(labelButtonPara->isSpecialButton)
		{
			update_widget_bg (labelButtonPara->eventLabel, /*backpic[8]*/ 17);
			update_widget_bg (labelButtonPara->eventData, /*backpic[14]*/ 17);
		}
		else
		{
			update_widget_bg (labelButtonPara->eventLabel, /*backpic[7]*/ 7);
			update_widget_bg (labelButtonPara->eventData, /*backpic[13]*/ 10);
		}
		break;
	}
}

void labelButtonSetFunctionModeAndPara(GtkWidget* widget ,LABELBUTTON_FUNCTION_MODE_ENUM mode ,void* p_para)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	labelButtonPara->mode = mode;
	switch(mode)
	{
	case LABELBUTTON_FUNCTION_MODE_POP:
		if(p_para)
		{
			if(NULL != labelButtonPara->popPara)
			{
				free(labelButtonPara->popPara);
				labelButtonPara->popPara = NULL;
			}
			labelButtonPara->popPara = malloc(sizeof(popStruct));
			memcpy(labelButtonPara->popPara ,p_para ,sizeof(popStruct));

			int size = ((popStruct*)p_para)->num_of_item * sizeof(const char*);
			labelButtonPara->popPara->label = malloc(size);
			labelButtonPara->popPara->data = malloc(size);
			memcpy(labelButtonPara->popPara->label ,((popStruct*)p_para)->label ,size);
			memcpy(labelButtonPara->popPara->data ,((popStruct*)p_para)->data ,size);

			gtk_label_set_label(GTK_LABEL(labelButtonPara->data)
					,labelButtonPara->popPara->data[labelButtonPara->popPara->current_pos]);
		}
		break;

	case LABELBUTTON_FUNCTION_MODE_NORMAL:
	default:
		if(labelButtonPara->popPara)
		{
			free(labelButtonPara->popPara);
			labelButtonPara->popPara = NULL;
		}
		break;
	}
	g_object_set_data(G_OBJECT(widget) ,"labelButtonPara" ,(gpointer)labelButtonPara);
}

static void callbackMenuItem(GtkMenuItem *menuitem, gpointer data)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(menuitem) ,"labelButtonPara");
	GtkWidget* widget = gtk_widget_get_parent(gtk_widget_get_parent(labelButtonPara->eventLabel));
	labelButtonPara->popPara->current_pos = (int)data;

	if(NULL != labelButtonPara->menu)
	{
	//	gtk_widget_destroy(labelButtonPara->menu);
	//	labelButtonPara->menu = NULL;
		gtk_menu_popdown(GTK_MENU(labelButtonPara->menu));
		labelButtonPara->menuPopStatus = FALSE;
		g_object_set_data(G_OBJECT(widget) ,"labelButtonPara" ,(gpointer)labelButtonPara);
	}

	if(labelButtonPara->popPara->data)
	{
		gtk_label_set_label(GTK_LABEL(labelButtonPara->data) ,labelButtonPara->popPara->data[(int)data]);
	}
	//release
	setBackground(widget ,LABELBUTTON_STATUS_RELEASE);

	if(NULL != labelButtonPara->popPara->callbackMenuItem)
	{
		labelButtonPara->popPara->callbackMenuItem(labelButtonPara->parent ,&data);
	}
}

static void popupPosFunc(GtkMenu   *menu,
				     gint      *x,
				     gint      *y,
				     gboolean  *push_in,
				     gpointer	user_data)
{
	popStruct* para = (popStruct*) user_data;
	*x = ((GtkAllocation)para->allocation).x;
	*y = ((GtkAllocation)para->allocation).y;
	*push_in = para->push_in;
}

static void popdown(GtkWidget* widget)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	if(NULL != labelButtonPara->menu)
	{
		gtk_widget_destroy(labelButtonPara->menu);
		labelButtonPara->menu = NULL;
		labelButtonPara->menuPopStatus = FALSE;
		g_object_set_data(G_OBJECT(widget) ,"labelButtonPara" ,(gpointer)labelButtonPara);
	}
}

static gboolean keyHandler (GtkWidget* pWidget ,GdkEventKey* pEvent ,gpointer data)
{
	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(data) ,"labelButtonPara");
	if(labelButtonPara->popPara->popKey == pEvent->keyval)
	{
		popdown(data);
//		fakekey_press_keysym(pp->fk, XK_Down, 0);
//		fakekey_release(pp->fk);
		return TRUE;
	}
	switch(pEvent->keyval)
	{
	case GDK_Left:
		fakekey_press_keysym(pp->fk, XK_Up, 0);
		fakekey_release(pp->fk);
		return TRUE;

	case GDK_Right:
		fakekey_press_keysym(pp->fk, XK_Down, 0);
		fakekey_release(pp->fk);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

void popNext(popStruct* pPara)
{
	do
	{
		if(pPara->current_pos == pPara->num_of_item)
		{
			pPara->current_pos = 0;
		}
		else
		{
			pPara->current_pos ++;
		}
	}
	while(0x01 & (pPara->item_status >> pPara->current_pos));

}

static void popup(GtkWidget* widget)
{
	int i;
	GtkWidget* menu;
	GtkWidget* menu_item ,*current_menu_item = NULL;

	labelButtonStruct* labelButtonPara = (labelButtonStruct*)g_object_get_data(G_OBJECT(widget) ,"labelButtonPara");
	popStruct para = *(labelButtonPara->popPara);
	popStruct* p_para = &para;
	if(NULL != labelButtonPara->menu)
	{
		gtk_widget_destroy(labelButtonPara->menu);
		labelButtonPara->menu = NULL;
	}
	menu = gtk_menu_new();
	labelButtonPara->menu = menu;
	labelButtonPara->menuPopStatus = TRUE;
	g_object_set_data(G_OBJECT(widget) ,"labelButtonPara" ,(gpointer)labelButtonPara);
//生成菜单项
	for (i = 0 ;i < p_para->num_of_item ;++i)
	{
		menu_item = gtk_menu_item_new_with_label (p_para->label[i]);
		g_object_set_data(G_OBJECT(menu_item) ,"labelButtonPara" ,(gpointer)labelButtonPara);
		gtk_menu_shell_append (GTK_MENU_SHELL(menu), menu_item);
		g_signal_connect (menu_item, "activate",
				G_CALLBACK(callbackMenuItem), (GUINT_TO_POINTER (i)));
		if (0x01 & (p_para->item_status >> i))
			gtk_widget_set_sensitive (menu_item ,FALSE);
		gtk_widget_show (menu_item);
		if(i == p_para->current_pos)
		{
			current_menu_item = menu_item;
		}

	}
//调整位置
	//根据主父窗口调整坐标
	int x,y;
	GtkRequisition requisition;
	GtkAllocation allocation;
	gtk_widget_size_request(menu ,&requisition);
	gtk_widget_get_allocation(widget ,&allocation);
	gtk_window_get_position((GtkWindow *) pp->window, &x, &y);
	if(p_para->allocation.x < 0)
	{
		p_para->allocation.x = allocation.x;
	}
	p_para->allocation.x += x;
	p_para->allocation.x -= requisition.width;//pop up to the left

	if(p_para->allocation.y < 0)
	{
		p_para->allocation.y = allocation.y;
	}
	p_para->allocation.y += y;


//弹出菜单
	gtk_menu_popup (GTK_MENU(menu), NULL, NULL,
			(GtkMenuPositionFunc)popupPosFunc,
			p_para,
			0,
			gtk_get_current_event_time());
//默认选中位置
	if(NULL != current_menu_item)
	{
		gtk_menu_shell_select_item(GTK_MENU_SHELL(menu),current_menu_item);
	}
//额外按键处理
	g_signal_connect (menu, "key-press-event",
			G_CALLBACK(keyHandler) ,(gpointer)widget);
}

