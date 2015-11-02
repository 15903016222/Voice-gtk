/*
 * labelbutton.h
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#ifndef LABELBUTTON_H_
#define LABELBUTTON_H_

#include <gtk/gtk.h>
typedef enum{
	LABELBUTTON_FUNCTION_MODE_NORMAL ,
	LABELBUTTON_FUNCTION_MODE_POP ,
	LABELBUTTON_FUNCTION_MODE_SPIN ,
	LABELBUTTON_FUNCTION_MODE_MAX
}
	LABELBUTTON_FUNCTION_MODE_ENUM;

typedef struct{
	const char** label;//弹出菜单显示的字符串数组
	const char** data;//data显示的字符串
	int num_of_item;//弹出的项数
	int item_status;//每一项的使能状态，1位表示，低位表示第0项
	int current_pos;//弹出时显示的选中的位置
	GtkAllocation allocation;//手动设置弹出位置，如果为负数，则用父窗口的坐标计算
	gboolean push_in;
	int popKey;
	void (*callbackMenuItem)(GtkWidget* widget ,void* para);
}
	popStruct;

typedef struct _labelButtonStruct{
	GtkWidget* parent;
	GtkWidget* event;
	GtkWidget* label;
	GtkWidget* data;
	GtkWidget* eventLabel;
	GtkWidget* eventData;
	GtkWidget* menu;
	GtkWidget* spin;
	LABELBUTTON_FUNCTION_MODE_ENUM mode;
	popStruct* popPara;
	int isSpecialButton;
	int menuPopStatus;
	void (*callbackPress)(GtkWidget* ,void* para);
	void* callbackPara;
}
	labelButtonStruct;

typedef enum{
	LABELBUTTON_MODE_NONE,
	LABELBUTTON_MODE_HIDE,
	LABELBUTTON_MODE_DATA3,
	LABELBUTTON_MODE_BIGMENU,
	LABELBUTTON_MODE_SPINBUTTON,
	LABELBUTTON_MODE_DISABLE,
	LABELBUTTON_MODE_MAX
}
	LABELBUTTON_MODE_ENUM;

typedef enum{
	LABELBUTTON_STATUS_NORMAL,
	LABELBUTTON_STATUS_PRESS,
	LABELBUTTON_STATUS_RELEASE,
	LABELBUTTON_STATUS_HIDE,
	LABELBUTTON_STATUS_MAX
}
	LABELBUTTON_STATUS_ENUM;
GtkWidget* labelButtonNew();
void labelButtonSetSpecial(GtkWidget* widget ,int isSpecialButton);
void labelButtonInstallCallback(GtkWidget* widget ,void(*func)(GtkWidget* ,void*) ,GtkWidget* dialog ,void* para);
void labelButtonSetDisplayMode(GtkWidget* widget ,LABELBUTTON_MODE_ENUM mode);
void labelButtonSetData(GtkWidget* widget ,const char* str);
void labelButtonSetLabel(GtkWidget* widget ,const char* str);
void labelButtonSetFunctionModeAndPara(GtkWidget* widget ,LABELBUTTON_FUNCTION_MODE_ENUM mode ,void* p_para);
void labelButtoncallbackPress(GtkWidget* widget);
void labelButtoncallbackRelease(GtkWidget* widget);

#endif /* LABELBUTTON_H_ */
