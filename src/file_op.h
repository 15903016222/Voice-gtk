
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * file_op.h
 * 定义各种各种配置文件的路径 以及打开方法
 *
 * 谭登华
 * 2011-03-07
 *
 */

#ifndef __FILE_OP_H_
#define __FILE_OP_H_

#include "base.h"
#include "drawui.h"

extern int read_probe_file (const gchar *file_path, PROBE_P p);	/* 读取探头信息 */
extern void save_probe_file (const gchar *file_path, PROBE_P p);	/* 保存探头信息 */
extern int read_wedge_file (const gchar *file_path, WEDGE_P p);	/* 读取楔块信息 */
extern void save_wedge_file (const gchar *file_path, WEDGE_P p);	/* 保存楔块信息 */
extern void read_palette_file (const gchar *file_path, guint *sp_col, guint *col);
extern void save_law_file (const gchar *file_path, gint group);
extern void read_law_file (const gchar *file_path, gint group);


#if 0
#define PA_UNKNOWN_PROBE	 "/media/card/config/probe/unknown_pa.opp"
#define UT_UNKNOWN_PROBE	 "/media/card/config/probe/unknown_ut.oup"
#define PA_PROBE_PATH		 "/media/card/config/probe/PA/"
#define UT_PROBE_PATH		 "/media/card/config/probe/UT/"
#define PA_WEDGE_PATH		 "/media/card/config/wedge/PA/"
#define UT_WEDGE_PATH		 "/media/card/config/wedge/UT/"
#define PA_PROBE_USER_PATH	 "/media/card/config/probe/PA/user/"
#define UT_PROBE_USER_PATH   "/media/card/config/probe/UT/user/"
#define PA_WEDGE_USER_PATH	 "/media/card/config/wedge/PA/user/"
#define UT_WEDGE_USER_PATH	 "/media/card/config/wedge/UT/user/"
#define USER_CFG_PATH	     "/media/card/config/setup/"
#define INSPECT_DATA_PATH    "/media/card/data/InspectData/"
#define INSPECT_TABLE_PATH   "/media/card/data/InspectTable/"
#define INNER_IMAGE_PATH     "/media/card/data/Image/"

#else
#define PA_UNKNOWN_PROBE	"/home/tt/TT/source/system/Model/Probe/unknown_pa.opp"
#define UT_UNKNOWN_PROBE	"/home/tt/TT/source/system/Model/Probe/unknown_ut.oup"
#define PA_PROBE_PATH	    "/home/tt/TT/source/system/Model/Probe/PA/"
#define UT_PROBE_PATH	    "/home/tt/TT/source/system/Model/Probe/UT/"
#define PA_WEDGE_PATH	    "/home/tt/TT/source/system/Model/Wedge/PA/"
#define UT_WEDGE_PATH	    "/home/tt/TT/source/system/Model/Wedge/UT/"
#define PA_PROBE_USER_PATH	"/home/tt/TT/source/system/Model/Probe/PA/user/"
#define UT_PROBE_USER_PATH  "/home/tt/TT/source/system/Model/Probe/UT/user/"
#define PA_WEDGE_USER_PATH	"/home/tt/TT/source/system/Model/Wedge/PA/user/"
#define UT_WEDGE_USER_PATH	"/home/tt/TT/source/system/Model/Wedge/UT/user/"
#define USER_CFG_PATH	    "/home/tt/TT/source/user/setup/"


#define INSPECT_DATA_PATH    "/home/tt/TT/data/InspectData/"
#define INSPECT_TABLE_PATH   "/home/tt/TT/data/InspectTable/"
#define INNER_IMAGE_PATH     "/home/tt/TT/data/Image/"

#endif

#define USB_DEV_PATH         "/opt/usbStorage/"
#define USER_LAW_PATH	     "/home/tt/TT/source/user/law/"
#define PALETTE_PATH	     "/home/tt/TT/source/system/Sample/Palette/"


#define FILE_NAME_MAX 256

extern char SOURCE_FILE_NAME[FILE_NAME_MAX];
extern char SOURCE_FILE_PATH[FILE_NAME_MAX];
extern char TARGET_FILE_NAME[FILE_NAME_MAX];
extern char TARGET_FILE_PATH[FILE_NAME_MAX];


extern int Set_Source_File_Path(char *path);

extern char *Get_Source_File_Path();

extern int Set_Target_File_Path(char *path);

extern char *Get_Target_File_Path();

extern int Set_Source_File_Name(char *name);

extern char *Get_Source_File_Name();

extern int Set_Target_File_Name(char *name);

extern char *Get_Target_File_Name();

extern int Scan_Target_File(GtkWidget *widget,GtkTreeModel *model,char *file_name);

extern int Select_File();

extern int Unselect_File();

extern gboolean Select_All_File (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern int Unselect_All_File();

extern gboolean Copy_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern gboolean Move_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern gboolean Delect_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern int  SaveDataAll() ;

extern void Save_Report_File(char *html_file_name);

extern int Save_Inspec_Table(char *file_name);

extern void Auto_Add_Suffix_Name(char *path,char *file_name,char *suffix_name);
extern void LoadPalete() ;
extern int CheckProbeFileReasonable( PROBE_P p , int nGroupModel) ;
#endif
