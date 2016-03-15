/*
 * authorization.c
 *
 *  Created on: 2013-1-5
 *      Author: wolflord
 */


#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

#include "../main.h"

#define NAND_DEVICE				 "/dev/mtd0"
#define NAND_DEVICE_BLOCK 		 "/dev/mtdblock2"
#define NAND_FILE_PATH1 		 "/home/tt/TT/source/user/.nand"
#define NAND_FILE_PATH2 		 "/tmp/.nand"
#define NAND_FILE_HEADER 		 "DOPPLER NAND AUTHORIZATION FILE HEADER***"
#define AUTHORIZE_WINDOW  		 "/home/tt/TT/pic/phascan.png"

int gMaxElementTriggerQty = 16 ;
int gMaxElementRecieveQty = 64 ;
int gPithCatchEnable      = 0  ;
int gTofdEnable           = -1 ;
char* g_pVersion = NULL;
char* g_szVersion[] =
{
	"32:128 PR" ,
	"32:128" ,
	"32:64" ,
	"16:64"
} ;


void EncodeSerialNo(unsigned char* nSerial_ , int nLength_)
{
	unsigned char tmp;
	int i;
	for(i = 0 ; i < nLength_ ; i ++)
	{
		tmp = ~(nSerial_[i]) + i;
		tmp *= 3 ;
		tmp += i % 10 ;
		nSerial_[i] = tmp ;
	}
}

void FormateAuthorizationString( char* nSource_ , int nLengthSource_ , char* nResult_ , int nLengthResult_)
{
	memset(nResult_ , 0 , nLengthResult_);
	strcpy(nResult_ , NAND_FILE_HEADER);
	char _szTmp[10];
	int i;
	for(i = 0 ; i< nLengthSource_ ; i++)
	{
		sprintf(_szTmp , "%03d" , nSource_[i]);
		strcat(nResult_ , _szTmp);
	}
	strcat(nResult_ , "***\"");
}

int SaveNandIdFile(unsigned char* szData_ , int length)
{
	char _szCmd[1024] ;
	memset(_szCmd , 0 , 1024) ;
	strcpy(_szCmd , "echo ")  ;
	strcat(_szCmd , "\"")  ;
	strcat(_szCmd , NAND_FILE_HEADER);
	char _szTmp[10];
	int i;
	for(i = 0 ; i< length ; i++)
	{
		sprintf(_szTmp , "%03d" , szData_[i]);
		strcat(_szCmd , _szTmp);
	}

	strcat(_szCmd , "***\"");
	strcat(_szCmd , " > ");
	strcat(_szCmd , NAND_DEVICE_BLOCK) ;
	system(_szCmd);
	//printf("%s" , _szCmd);
	return 0;
}

int ReadNandIdFile(const char* szPath_ ,  char* szId_, int nLength_)
{
	int fd;
	int	i;
	char buff[1024] ;
	if ((fd = open(szPath_, O_RDONLY ))<0)
	{
		return -1 ;
	}

	i = read (fd, buff, nLength_);
	if(i < 0)
		return -1;
	int _nReadLength = nLength_ > 1024 ? 1024 : nLength_ ;
	memcpy( szId_ , buff, _nReadLength);
	close(fd);
	return 0  ;
}

int ReadNandIdFromNand(unsigned char* szData_ , int length)
{
	// read nand flash block 3 to file  NAND_FILE_PATH2
	char _szCmd[1024] , _szBuff[1024];
	memset(_szCmd , 0 , 1024) ;
	strcpy(_szCmd , "cat ")  ;
	strcat(_szCmd , NAND_DEVICE_BLOCK) ;
	strcat(_szCmd , " > ") ;
	strcat(_szCmd , NAND_FILE_PATH2);
	system(_szCmd);
	//printf("%s" , _szCmd);
	// read NAND_FILE_PATH2 to string
	ReadNandIdFile(NAND_FILE_PATH2 , _szBuff , length) ;
	memcpy(szData_ , _szBuff , length);
	return 0;
}

int GetHardwareNandId(unsigned char* szId_ , int nLength_)
{
	int fd,  ret;
	fd = open(NAND_DEVICE , O_RDONLY);
	if (fd < 0) {
		perror(NAND_DEVICE);
		return -1;
	}

	unsigned char buff[1024] ;
	ret = ioctl(fd, OTPGETREGIONCOUNT, buff);
	if(ret < 0)
		return -1 ;

	close(fd);

	int _nReadLength = nLength_ > 16 ? 16 : nLength_ ;
	memcpy(szId_ , buff , _nReadLength);

	return 0;
}

int CompareId(unsigned char* nId1_ , unsigned char* nId2_ , int nLength_)
{
	//printf("LENGTH %d \n" , nLength_);
	int ret = 0 , tmp;

	int i ;
	for(i = 0 ; i< nLength_ ; i++)
	{
		tmp = abs(nId1_[i] - nId2_[i]) ;
		ret += tmp ;
	}
	return ret;
}

int CheckAuthorization()
{
	unsigned char _szId[16]    ;
	unsigned char _szId1[1024] ;
	unsigned char _szId2[1024] ;
	// read id from nand device
	if(GetHardwareNandId(_szId , 16))
		return -1;

	EncodeSerialNo(_szId , 16);
	FormateAuthorizationString((void*)_szId , 16 ,(void*)_szId1 , 1024) ;

	ReadNandIdFromNand(_szId2 , strlen((void*)_szId1)) ;
	printf("string1: %s \n" , _szId1);
	printf("string2: %s \n" , _szId2);
	int ret = memcmp(_szId1 , _szId2, strlen((void*)_szId1) - 1) ;
	return ret;
}


static GdkPixbuf* loadAllPixbuffers(char* path)
{
    GdkPixbuf* pixbuff = NULL ;
    pixbuff = gdk_pixbuf_new_from_file(path,NULL);
    return pixbuff ;
}

static void changeWidgetBackGroudPicture(GtkWidget *widget, GdkPixbuf* pixbuf)
{
    GtkStyle *style;
    GdkPixmap *pixmap;
    gint width, height;
    width = gdk_pixbuf_get_width(pixbuf);
    height = gdk_pixbuf_get_height(pixbuf);
    pixmap = gdk_pixmap_new(NULL, width, height, 24);
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, NULL, 0);
    style = gtk_style_copy(GTK_WIDGET (widget)->style);

    if (style->bg_pixmap[GTK_STATE_NORMAL])
    g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);

    style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);
    gtk_widget_set_style(GTK_WIDGET (widget), style);
    g_object_unref(style);
}

static GtkWidget* g_pAuthorizeWindow = NULL ;

int time_out(void*  data)
{
	MainInit ();
	gtk_widget_hide(g_pAuthorizeWindow);
	return 0;
}


#define MTD_DEVICE "/dev/mtdblock2"
int GetPermission(char* szPermission_)
{
	int ret = -1;

	if     (!memcmp(szPermission_ , "32-128-PR-TOFD" , sizeof("32-128-PR-TOFD")-1))  ret = 0;
	else if(!memcmp(szPermission_ , "32-128-TOFD"    , sizeof("32-128-TOFD")-1))     ret = 1;
	else if(!memcmp(szPermission_ , "32-64-TOFD"     , sizeof("32-64-TOFD")-1))      ret = 2;
	else if(!memcmp(szPermission_ , "16-64-TOFD"     , sizeof("16-64-TOFD")-1))      ret = 3;

	printf("ComparePermissionString:  %d \n" , ret) ;

	return ret ;
}

void SetSoftwareVersion(int nVersion)
{
	switch(nVersion)
	{
	case 0:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 128 ;
		gPithCatchEnable      = -1  ;
		gTofdEnable           = -1  ;
		g_pVersion = g_szVersion[0] ;
		break;
	case 1:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 128 ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = -1  ;
		g_pVersion = g_szVersion[1] ;
		break;
	case 2:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = -1  ;
		g_pVersion = g_szVersion[2] ;
		break;
	case 3:
		gMaxElementTriggerQty = 16  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = -1  ;
		g_pVersion = g_szVersion[3] ;
		break;
	default:
		gMaxElementTriggerQty = 16  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = -1  ;
		g_pVersion = g_szVersion[3] ;
		break;

	}
}

void CheckVersion()
{
	char buff[100] ;
	memset(buff , 0 , 100);
	int ret  ;
#if ARM
    int mtd;
	if ((mtd = open(MTD_DEVICE, O_RDWR | O_SYNC)) == -1)
	{
	    perror (MTD_DEVICE);
		return ;
	}
	ret = read(mtd , buff , 30);
	if(ret == -1)
	{
		printf("read count %d\n", ret);
	}
	printf("read mtd string --- %s  \n" , buff);
	close(mtd);
#endif
	ret = GetPermission(buff) ;
	SetSoftwareVersion(ret) ;

}

//不再用之前的读取NAND 工厂ID 的方案
void AuthorizetionWindow()
{
	CheckVersion();
	MainInit ();
}

int ComparePermissionString(char* szPermission_)
{
	int ret = -1;

	if     (!strcmp(szPermission_ , "32-128-PR-TOFD")) ret = 0;
	else if(!strcmp(szPermission_ , "32-128-TOFD"))    ret = 1;
	else if(!strcmp(szPermission_ , "32-64-TOFD"))     ret = 2;
	else if(!strcmp(szPermission_ , "16-64-TOFD"))     ret = 3;
	//else if(!strcmp(szPermission_ , "16-64"))          ret = 4;
	g_debug("ComparePermissionString:  %d" , ret) ;

	return ret ;
}



void SetSoftwareVersionPermission(char* szPermission_)
{
	int _nPermission = ComparePermissionString(szPermission_) ;
	g_debug("SetSoftwareVersionPermission: %s" , szPermission_);
	switch(_nPermission)
	{
	case 0:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 128 ;
		gPithCatchEnable      = -1  ;
		gTofdEnable           = -1  ;
		break;
	case 1:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 128 ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = -1  ;
		break;
	case 2:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = -1  ;
		gTofdEnable           = -1  ;
		break;
	case 3:
		gMaxElementTriggerQty = 32  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = -1  ;
		break;
	case 4:
		gMaxElementTriggerQty = 16  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = 0   ;
		break;
	default:
		gMaxElementTriggerQty = 16  ;
		gMaxElementRecieveQty = 64  ;
		gPithCatchEnable      = 0   ;
		gTofdEnable           = 0   ;
		break;

	}
}
