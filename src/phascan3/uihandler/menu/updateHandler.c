/*
 * updateHandler.c
 *
 *  Created on: 2012-10-17
 *      Author: lzk
 */


#include <gtk/gtk.h>
#include "../../lzk/fileHandler.h"
#include "../../ui/ui.h"
#include "../../string/_string.h"
#include "../../drawui.h"
#include "../../globalData.h"

extern void data_912 (GtkMenuItem *menuitem, gpointer data);
extern void HandlerResetCFG();
extern void HandlerResetPopData();

int UpdateHandler_ResetCFG(void* p_para)
{
	char str[30];
	memset(str ,0 ,30);
	sprintf(str ,"%s ?" ,getDictString(_STRING_Reset_Config));
	if(dialogWarning(pp->window ,str))
	{
		HandlerResetCFG();
		fileDelete("/home/tt/TT/source/user/setup/default.cfg" ,"-f");

		data_912(NULL ,(gpointer)SYSTEM(language));
        HandlerResetPopData();
/*		pp->pos_pos = MENU3_STOP;
		change_language (get_language(pp->p_config), pp);
		draw_menu1();
		draw_menu2(0);
		draw_menu3(0, NULL);
		if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
			tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
		else
			tt_label_show_string (pp->label[0], con2_p[1][0][0], "\n", "(dB)", "white", 10);//*/
	}
	return TRUE;
}
