/*
 * dialog.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include <gtk/gtk.h>
#include "ui.h"
#include <string.h>
#include <unistd.h>
#include <gdk/gdkkeysyms.h>

static GdkColor	g_ColorBlack     = {0x0, 0x0, 0x0, 0x0};

#include "../string/_string.h"
GtkWidget* dialogNew(GtkWidget* fatherWidget)
{
	GtkWidget* dialog;
	//主窗体
	dialog = gtk_dialog_new_with_buttons (" ", GTK_WINDOW(fatherWidget),
				GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
				NULL, NULL,
				NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			//不可以装饰
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &g_ColorBlack);	//黑色背景

	return dialog;
}


static gboolean dialogWarningPress(GtkWidget* pWidget ,GdkEventKey* pEvent ,gpointer data)
{
	switch(pEvent->keyval)
	{
	case GDK_Up:
	case GDK_Down:
		return TRUE;
	//case GDK_Return:
	//	return TRUE;
		break;
	case GDK_Escape:
		gtk_dialog_response (GTK_DIALOG(pWidget), GTK_RESPONSE_NO);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

int dialogWarning(GtkWidget* fatherWidget ,const char* warningString)
{

	GtkWidget* dialog;
	dialog = gtk_message_dialog_new( GTK_WINDOW(fatherWidget),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_WARNING,
			GTK_BUTTONS_YES_NO,
			"%s" ,warningString);
	gtk_widget_set_size_request(dialog , 400 , 120);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);
	g_signal_connect(G_OBJECT (dialog), "key-press-event", G_CALLBACK(dialogWarningPress), NULL);

	int response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if(response == GTK_RESPONSE_YES)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int dialogFileWarning(GtkWidget* fatherWidget ,const char* warningString)
{
    int ret, response;
    GtkWidget *dialog;
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *table;
    GtkWidget *check = NULL;

    dialog = gtk_dialog_new_with_buttons(NULL,
            GTK_WINDOW(fatherWidget),
            GTK_DIALOG_MODAL,
            GTK_STOCK_NO, GTK_RESPONSE_NO,
            GTK_STOCK_YES, GTK_RESPONSE_YES,
            NULL);
	gtk_window_set_decorated(GTK_WINDOW (dialog), FALSE);
    gtk_dialog_set_has_separator(GTK_DIALOG(dialog),FALSE);
    label = gtk_label_new(warningString);

    image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_DIALOG);

    check = gtk_check_button_new_with_label(getDictString(_STRING_CMD_APPLY_ALL));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), FALSE);
    GTK_WIDGET_UNSET_FLAGS(check, GTK_CAN_FOCUS);

    table = gtk_table_new(2, 2, FALSE);
    //gtk_table_set_row_spacing(GTK_TABLE(table), 1, 0);
    //gtk_table_set_col_spacing(GTK_TABLE(table), 0, 0);
    gtk_table_attach_defaults(GTK_TABLE(table), image, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), check, 1, 2, 1, 2);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);

    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
	gtk_widget_set_size_request(dialog , 400 , 120);
	gtk_widget_set_size_request(image, 50 , 50);
    gtk_widget_show_all(dialog);  
  
    ret = gtk_dialog_run(GTK_DIALOG(dialog));
	if(ret == GTK_RESPONSE_YES) {
        ret = 1;
    } else {
        ret = 0;
    }
    response = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check));

    gtk_widget_destroy(check);
    gtk_widget_destroy(label);
    gtk_widget_destroy(image);
    gtk_widget_destroy(dialog);

    return ret+response*2;
}

GtkWidget* dialogMessageWnd = NULL ;
static int destroyDilogMessage(gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
	dialogMessageWnd = NULL ;
	return 0 ;
}

void dialogMessage(GtkWidget* fatherWidget ,const char* msg , GdkColor* pBgColor_)
{
	char* markup ;
	int _nStringLenghtPixels ;
	PangoLayout *layout = NULL;
	GtkWidget* Label ;// = gtk_label_new("");

	if(dialogMessageWnd) gtk_widget_destroy(dialogMessageWnd);
	dialogMessageWnd = NULL ;

	dialogMessageWnd = gtk_window_new(GTK_WINDOW_POPUP) ;
	Label  = gtk_label_new("");
	// get string length in pixel
	layout = gtk_widget_create_pango_layout (Label, msg);
	pango_layout_get_pixel_size(layout,   &_nStringLenghtPixels,   NULL);
	_nStringLenghtPixels = _nStringLenghtPixels * 2 + 70;

	markup = g_markup_printf_escaped("<span foreground='white' font_desc='20'>%s</span>", msg);
	gtk_label_set_markup(GTK_LABEL(Label) , markup);
	gtk_window_set_decorated (GTK_WINDOW (dialogMessageWnd), FALSE);
	gtk_widget_modify_bg(dialogMessageWnd , GTK_STATE_NORMAL , pBgColor_ ) ;
	gtk_widget_set_size_request(dialogMessageWnd , _nStringLenghtPixels  , 60) ;
	gtk_window_set_position (GTK_WINDOW (dialogMessageWnd),GTK_WIN_POS_CENTER);

	gtk_container_add(GTK_CONTAINER(dialogMessageWnd) , Label);
	gtk_window_set_transient_for (GTK_WINDOW (dialogMessageWnd), GTK_WINDOW (fatherWidget));
	gtk_widget_show_all(dialogMessageWnd);

	gtk_window_set_modal(GTK_WINDOW(dialogMessageWnd) , TRUE);
	g_timeout_add(1000 , destroyDilogMessage , dialogMessageWnd);

	g_free(markup);
}

void dialogMessagePix(GtkWidget* fatherWidget ,const char* msg , GdkColor* pBgColor_ , int pixels)
{
	char* markup ;
	int _nStringLenghtPixels ;
	PangoLayout *layout = NULL;
	GtkWidget* Label ;// = gtk_label_new("");

	if(dialogMessageWnd) gtk_widget_destroy(dialogMessageWnd);
	dialogMessageWnd = NULL ;

	dialogMessageWnd = gtk_window_new(GTK_WINDOW_POPUP) ;
	Label  = gtk_label_new("");
	// get string length in pixel
	layout = gtk_widget_create_pango_layout (Label, msg);
	pango_layout_get_pixel_size(layout,   &_nStringLenghtPixels,   NULL);
	_nStringLenghtPixels = _nStringLenghtPixels*2+pixels;

	markup = g_markup_printf_escaped("<span foreground='white' font_desc='18'>%s</span>", msg);
	gtk_label_set_markup(GTK_LABEL(Label) , markup);
	gtk_window_set_decorated (GTK_WINDOW (dialogMessageWnd), FALSE);
	gtk_widget_modify_bg(dialogMessageWnd , GTK_STATE_NORMAL , pBgColor_ ) ;
	gtk_widget_set_size_request(dialogMessageWnd , _nStringLenghtPixels  , 60) ;
	gtk_window_set_position (GTK_WINDOW (dialogMessageWnd),GTK_WIN_POS_CENTER);

	gtk_container_add(GTK_CONTAINER(dialogMessageWnd) , Label);
	gtk_window_set_transient_for (GTK_WINDOW (dialogMessageWnd), GTK_WINDOW (fatherWidget));
	gtk_widget_show_all(dialogMessageWnd);

	gtk_window_set_modal(GTK_WINDOW(dialogMessageWnd) , TRUE);
	g_timeout_add(2000, destroyDilogMessage , dialogMessageWnd);

	g_free(markup);
}

static gboolean dialogErrorPress(GtkWidget* pWidget ,GdkEventKey* pEvent ,gpointer data)
{
	switch(pEvent->keyval)
	{
	case GDK_Up:
	case GDK_Down:
	case GDK_Right:
	case GDK_Left :
		return TRUE ;
		break;
	default:
		break;
	}
	return FALSE ;
}

void dialogError(GtkWidget* fatherWidget ,const char* warningString)
{
	GtkWidget* dialog;
	dialog = gtk_message_dialog_new( GTK_WINDOW(fatherWidget),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
			"%s" ,warningString);
	gtk_widget_set_size_request(dialog , 400 , 120);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);
	g_signal_connect (G_OBJECT (dialog), "key-press-event", G_CALLBACK(dialogErrorPress), NULL);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


int dialogFileConfirm(GtkWidget* dialog ,const char* filename ,const char* path)
{
	const char* str = strrchr(filename ,'/');
	char filenameTarget[256];
	memset(filenameTarget ,0 ,256);
	if(path)
	{
		if(str)
		{
			sprintf(filenameTarget ,"%s%s" ,path ,str);
		}
		else
		{
			sprintf(filenameTarget ,"%s/%s" ,path ,filename);
		}
	}
	else
	{
		sprintf(filenameTarget ,"%s" ,filename);
	}
	if(0 == access(filenameTarget ,F_OK))
	{
		memset(filenameTarget ,0 ,256);
		if(str)
			sprintf(filenameTarget ,"\n\"%s\"\n\t%s?" ,str + 1 ,getDictString(_STRING_File_Exist_Overwrite));
		else
			sprintf(filenameTarget ,"\n\"%s\"\n\t%s?" ,filename ,getDictString(_STRING_File_Exist_Overwrite));
		return dialogFileWarning(dialog ,filenameTarget);
	}
	return TRUE;
}

char* dialogGetAString(GtkWidget* fatherWidget ,const char* title ,const char* initStr)
{
	char* str = NULL;
	GtkWidget* dialog;
	GtkWidget* vbox;
	GtkWidget *boxKeyboard;
	GtkWidget* label ,*textView ,*scroll;
	GtkTextBuffer* textBuffer;
	dialog = gtk_dialog_new_with_buttons (title, GTK_WINDOW(fatherWidget),
				GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
				getDictString(_STRING_Confirm) ,GTK_RESPONSE_OK,
				getDictString(_STRING_Cancel) ,GTK_RESPONSE_CANCEL,
				NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			//不可以装饰
//	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	//黑色背景

	label = gtk_label_new(title);

	textView = gtk_text_view_new();
	textBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textView));
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textView), GTK_WRAP_WORD_CHAR);
	if(initStr)
	{
		gtk_text_buffer_set_text (textBuffer, initStr, -1);
	}

	scroll = gtk_scrolled_window_new(NULL ,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scroll)
									,GTK_POLICY_AUTOMATIC ,GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scroll),textView);

	vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);

	gtk_box_pack_start(GTK_BOX(vbox),scroll,FALSE,FALSE,5);

	boxKeyboard = keyBoardNew();
	gtk_widget_set_size_request(GTK_WIDGET(boxKeyboard) ,650 ,200);
	gtk_box_pack_start(GTK_BOX(vbox),boxKeyboard,TRUE,TRUE,10);


	gtk_widget_show_all(vbox);

	GtkTextIter start, end;
	int response;
	switch(response = gtk_dialog_run(GTK_DIALOG(dialog)))
	{
	case GTK_RESPONSE_OK:
		gtk_text_buffer_get_start_iter(textBuffer, &start);
		gtk_text_buffer_get_end_iter(textBuffer, &end);

		str = gtk_text_buffer_get_text(textBuffer, &start, &end, FALSE);
		break;
	case GTK_RESPONSE_CANCEL:
		break;
	}
	gtk_widget_destroy (dialog);
	return str;
}

struct ProgressData {
    GtkWidget *window;
    GtkWidget *pbar;
    int timer;
    int work;
    int count;  //计数器
    gboolean activity_mode;
};

gint progressTimeout(gpointer data)
{
    struct ProgressData *pbar = (struct ProgressData*)data;
    if (pbar->work == 0) {
        gtk_dialog_response(GTK_DIALOG(pbar->window), GTK_RESPONSE_CLOSE);
        return FALSE;
    } else {
        pbar->count++;
        if (pbar->count %10) {
            gtk_progress_bar_pulse(GTK_PROGRESS_BAR(pbar->pbar));
        }
    }
    return TRUE;  
}

void CmdOperate(const char **args)
{
    struct ProgressData* pdata = (struct ProgressData *)args[0];

    FileCommand(args[1]);
    system("sync");

    pdata->work = 0;
}

int fileCopyProgressBar(GtkWidget* fatherWidget,
        const char* cmd,
        const char* fileName,
        const char* path,
        const char* commandOption)
{
	int ret;
	pthread_t tid3;
    struct ProgressData *parData = g_malloc(sizeof(struct ProgressData));  
    const char *note = NULL;
    const void *args[3];
    char command[256];
    memset(command, 0, sizeof(command));
    memset(parData, 0, sizeof(struct ProgressData));
    strcpy(command, cmd);
    strcat(command, " ");
    if (commandOption != NULL && strlen(commandOption)) {
        strcat(command, commandOption);
        strcat(command, " ");
    }
    if (strcmp(cmd, "cp") == 0) {
        note = getDictString(_STRING_Copy);
        strcat(command, "\"");
        strcat(command, fileName);
        strcat(command, "\" ");
        strcat(command, path);
    } else if (strcmp(cmd, "mv") == 0) {
        note = getDictString(_STRING_Move);
        strcat(command, "\"");
        strcat(command, fileName);
        strcat(command, "\" ");
        strcat(command, path);
    } else if (strcmp(cmd, "rm") == 0) {
        note = getDictString(_STRING_Delete);
        strcat(command, "\"");
        strcat(command, fileName);
        strcat(command, "\"");
    }
    args[0] = parData;
    args[1] = command;
    args[2] = NULL;
    parData->window = gtk_message_dialog_new(GTK_WINDOW(fatherWidget),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_NONE,
            "%s", note);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 3);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(parData->window)->vbox), vbox);
    parData->pbar = gtk_progress_bar_new();
    gtk_container_add(GTK_CONTAINER(vbox), parData->pbar);
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(parData->pbar));  
    parData->work = 1;
    parData->timer = gtk_timeout_add (10, progressTimeout, parData);
	gtk_widget_set_size_request(parData->window, 400 , 120);
	gtk_window_set_decorated (GTK_WINDOW(parData->window), FALSE);
    gtk_widget_show_all(parData->window);

	ret = pthread_create (&tid3, NULL, (void*)CmdOperate, &args);
    gtk_dialog_run(GTK_DIALOG(parData->window));

    gtk_timeout_remove(parData->timer);
    gtk_widget_destroy(parData->pbar);
    gtk_widget_destroy(vbox);
    gtk_widget_destroy(parData->window);
    pthread_join(tid3, NULL);
    g_free(parData);

    return 0;
}
