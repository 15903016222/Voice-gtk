/*
 * threads.c
 *
 *  Created on: 2013-5-2
 *      Author: wolflord
 */
#include "../drawui.h"
#include "../file_op.h"
#include "../callback.h"
#include "../main.h"
#include "../calibration.h"
#include <stdlib.h>
#include <time.h>
#include <linux/rtc.h>
#include <string.h>
#include <assert.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>



/* 显示系统时间 */
static gchar buffer[32];
gboolean time_handler1 (GtkWidget *widget)
{
	time_t curtime;
	struct tm *loctime;
	gchar *markup;
	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime (buffer, 32, "%F %T", loctime);

	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>", buffer);
    gdk_threads_lock();
	gtk_label_set_markup (GTK_LABEL(pp->label[4]),markup);
    gdk_threads_leave();
	g_free (markup);
	return TRUE;
}



//#if ARM
void process_key_press (char key)
{
	//printf("key %x\n" , key);
	switch((int)key)
	{
		case 0xd0:
			fakekey_press_keysym(pp->fk, XK_Up, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd8:
			fakekey_press_keysym(pp->fk, XK_Down, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd1:
			fakekey_press_keysym(pp->fk, XK_Left, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd7:
			fakekey_press_keysym(pp->fk, XK_Right, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd6:
			fakekey_press_keysym(pp->fk, XK_Return, 0);
			fakekey_release(pp->fk);
			break;
		case 0xf1:
			fakekey_press_keysym(pp->fk, XK_Return, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd2:
			fakekey_press_keysym(pp->fk, XK_Super_L, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd9:
			fakekey_press_keysym(pp->fk, XK_Escape, 0);
			fakekey_release(pp->fk);
			break;
		case 0xef:
			fakekey_press_keysym(pp->fk, XK_Escape, 0);
			fakekey_release(pp->fk);
			break;

		case 0xd3:
			fakekey_press_keysym(pp->fk, XK_KP_8, 0);
			fakekey_release(pp->fk);
			break;
		case 0xda:
			fakekey_press_keysym(pp->fk, XK_KP_9, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd4:
			fakekey_press_keysym(pp->fk, XK_KP_6, 0);
			fakekey_release(pp->fk);
			break;
		case 0xdb:
#if 0
			gdk_threads_enter();
			SaveDataProcess("Saving Data!") ;
			gdk_threads_leave();
#else
			fakekey_press_keysym(pp->fk, XK_KP_7, 0);
			fakekey_release(pp->fk);
#endif
			break;
		case 0xd5:
			fakekey_press_keysym(pp->fk, XK_KP_4, 0);
			fakekey_release(pp->fk);
			break;
		case 0xdc:
			fakekey_press_keysym(pp->fk, XK_KP_5, 0);
			fakekey_release(pp->fk);
			break;
		case 0xdd:
			fakekey_press_keysym(pp->fk, XK_KP_2, 0);
			fakekey_release(pp->fk);
			break;
		case 0xde:
			fakekey_press_keysym(pp->fk, XK_KP_3, 0);
			fakekey_release(pp->fk);
			break;
		case 0xdf:
			fakekey_press_keysym(pp->fk, XK_KP_0, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe0:
			fakekey_press_keysym(pp->fk, XK_KP_1, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe1:
			fakekey_press_keysym(pp->fk, XK_KP_Subtract, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe2:
			fakekey_press_keysym(pp->fk, XK_KP_Decimal, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe3:
			fakekey_press_keysym(pp->fk, XK_Super_L, 0);
			fakekey_release(pp->fk);
			break;

		case 0xe4:
			fakekey_press_keysym(pp->fk, XK_F2, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe5:
			fakekey_press_keysym(pp->fk, XK_F3, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe6:
			fakekey_press_keysym(pp->fk, XK_F4, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe7:
			fakekey_press_keysym(pp->fk, XK_F5, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe8:
			fakekey_press_keysym(pp->fk, XK_F6, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe9:
			fakekey_press_keysym(pp->fk, XK_F1, 0);
			fakekey_release(pp->fk);
			break;
		case 0xea:
			fakekey_press_keysym(pp->fk, XK_F7, 0);
			fakekey_release(pp->fk);
			break;
		case 0xeb:
			fakekey_press_keysym(pp->fk, XK_F8, 0);
			fakekey_release(pp->fk);
			break;
		case 0xec:
			fakekey_press_keysym(pp->fk, XK_F9, 0);
			fakekey_release(pp->fk);
			break;
		case 0xed:
			fakekey_press_keysym(pp->fk, XK_F10, 0);
			fakekey_release(pp->fk);
			break;
		case 0xee:
			fakekey_press_keysym(pp->fk, XK_F11, 0);
			fakekey_release(pp->fk);
			break;
		case 0xf2:
			fakekey_press_keysym(pp->fk, XK_F12, 0);
			fakekey_release(pp->fk);
			break;
	}
}

//static int searialsCounter = 0 ;
static void key_message_thread(void)
{
	char key = 0;
	while(1)
	{
		if (read(pp->fd_key, &key, 1) > 0)
		{
			process_key_press (key);
		}
	}
}

static void battery_information_read_thread(void)
{
	unsigned char key[3] ;
	unsigned char battery[28] ;
	//AUTO_PROBE  _Probe ;
	BATTERY _TmpBattryInfo    ;

	short* temp ;
	short* temp1;
	short  xxx  ;
	int        i;
	PROBE _probe;
	PROBE_P _pProbe  = (PROBE_P)(((void*)&_probe) + 4);

	while(1)
	{
		//将所有电池信息全部读取出来
		if(read(pp->fd_key1,  key, 1) > 0)
		{
			if(key[0] == 0x55 || key[0] == 0x53)
			{
				if(read(pp->fd_key1,  key, 2) > 0)
				{
					if(key[0] == 0x55  && key[1] == 0x55)
					{
						memset(battery , 0 , 28);
						if(read(pp->fd_key1,  battery, 28) == 28)
						{
							if(battery[24] == 0xaa || battery[24] == 0xcc)
								ExitApplication(NULL, NULL,	(gpointer)-1);

							memcpy( &_TmpBattryInfo , battery, 28 );
							temp = (short*)(&_TmpBattryInfo);
							for(i = 0 ; i< 12; i++)
							{
								temp1 = temp + i ;
								xxx = (short)(*temp1 >> 8) + (short)(*temp1 << 8) ;
								*temp1 = xxx ;
							}
							memcpy( &(pp->battery) , &_TmpBattryInfo , 28 );
							gdk_threads_enter();
							gtk_widget_queue_draw (pp->drawing_area);
							gtk_widget_queue_draw (pp->status_area);
							gdk_threads_leave();

						}
					}
					else if(key[0] == 0x53 && key[1] == 0x53)
					{
						if(get_auto_detect (pp->p_config))
						{
							if(read(pp->fd_key1,  (void*)(&_probe), 512) == 512)
							{
								if(CheckProbeFileReasonable(_pProbe , PA_SCAN))
								{
									if(pp->pos==0) //&&(pp->pos1[pp->pos]==1))
									{
										memcpy((void*)(&g_tmp_group_struct.probe ), _pProbe , sizeof(PROBE) - 4);
										request_refresh(REFRESH_PROBE_LOAD);
									}
									else
									{
										memcpy((void*)(&GROUP_VAL_POS(get_current_group(pp->p_config ), probe)) , _pProbe , sizeof(PROBE) - 4);
										request_refresh(REFRESH_PROBE_LOAD);
									}
									g_debug("auto detect: Probe Parameters Reasonable!\n");
								}
								else
								{
									g_debug("auto detect: Probe Parameters Not Reasonable!\n");
								}
							}
						}
					}
				}
			}
			else
			{
				read(pp->fd_key1,  (void*)(&_probe), 100) ;
			}
		}
		usleep(1000000) ;
	}
}



#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_KEY     0x780078 //定义IPC消息key
//定义IPC消息传送的内容
typedef struct _msgcontent{
     char szAction[64];   //USB产生的动作
     char szDevice[64]; //USB设备名称
}UMSGCONTENT;

typedef struct _umsg{
     long msgtype;
     UMSGCONTENT content;
}UMSG;

static void UsbStorageMessage(void)
{
    int _nMessageId;
    UMSG _msg      ;
    _nMessageId = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
    if (_nMessageId == -1)
    {
        //printf("msgget failed with error: %d\n", errno);
        return ;
    }
    while(1)
    {
        if (msgrcv(_nMessageId, (void *)&_msg, sizeof(UMSGCONTENT),
                   1, 0) == -1)
        {
        	g_debug ("msgrcv failed with error\n");
            return ;
        }
        else
        {
        	//printf("%s  %s\n" ,_msg.content.szAction ,  _msg.content.szDevice);
        	if (!strcmp("add" ,_msg.content.szAction))
        	{
        		TMP(bUSBStrorage) = 0xffffff ;
        		set_file_storage (pp->p_config, 2);
        		if((pp->pos == 8) && (pp->pos1[8] == 0))
        		{
        			gdk_threads_enter();
        			MENU_STATUS = MENU3_STOP;
        			draw_menu3(0 , NULL) ;
        			gdk_threads_leave();
        		}
        	}
        	if (!strcmp("remove" ,_msg.content.szAction))
        	{
        		TMP(bUSBStrorage) = 0 ;
        		if(get_file_storage (pp->p_config) == 2)
        		{
        			set_file_storage (pp->p_config, 0);
            		if((pp->pos == 8) && (pp->pos1[8] == 0))
            		{
            			gdk_threads_enter();
            			MENU_STATUS = MENU3_STOP;
            			draw_menu3(0 , NULL) ;
            			gdk_threads_leave();
            		}
        		}
        	}
        }
    }
}

#include "../net/DopplerNetServer.h"
void DopplerPhyscanTcpServer()
{
	if(ServerTcpSocketInit() ) return;
	if(InitUdpClient(IP_ADDR))
		g_debug("InitUdpClient error!") ;
	else
		g_debug("InitUdpClient!") ;

	int ret ;
	while(1)
	{
		ServerTcpAcceptConnect() ;
		ret = 0 ;
		while(!ret)
		{
			ret = ServerTcpRespondToSocket() ;
		}
	}
	ServerTcpCloseClientSocket();
	ServerTcpCloseServerSocket();

	UdpCloseClient() ;
	return ;

}

int key_read_delay()
{
	int ret;
	pthread_t tid0 , tid1 , thid2;
	ret = pthread_create (&tid0, NULL, (void*)key_message_thread, NULL);
	if(ret){
		perror("in1:");
		return 0;
	}
	ret = pthread_create (&tid1, NULL, (void*)battery_information_read_thread, NULL);
	if(ret){
		perror("in1:");
		return 0;
	}

	ret = pthread_create (&thid2, NULL, (void*)UsbStorageMessage, NULL);
	if(ret){
		perror("in1:");
		return 0;
	}
	//取消屏保和节能
	system("/usr/bin/xset s 0");
	system("/usr/bin/xset -dpms");

	return 0;

}
