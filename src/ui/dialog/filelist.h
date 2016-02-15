/*
 * filelist.h
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#ifndef FILELIST_H_
#define FILELIST_H_

#include <gtk/gtk.h>

enum{
	FILELIST_COLUMN_IMAGE,
	FILELIST_COLUMN_FILENAME,
	FILELIST_COLUMN_MAX
};

int filelistUpdate(GtkWidget* treeView);
int filelistSetPath(GtkWidget* treeView ,const char* path);
int filelistSetSuffix(GtkWidget* treeView ,const char* suffix);
int filelistSetPathAndSuffix(GtkWidget* treeView ,const char* path ,const char* suffix);
GtkWidget* filelistScrollNew();

GtkWidget* filelistDialogNew(GtkWidget* fatherWidget);
extern GtkWidget* workpiece_chooser_dialog_new(GtkWidget* parent);

#define NUM_OF_BUTTON 12
#define NUM_OF_BUTTON_BOTTOM 6
#endif /* FILELIST_H_ */
