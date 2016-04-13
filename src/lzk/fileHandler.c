/*
 * fileHandler.c
 *
 *  Created on: 2012-3-27
 *      Author: lzk
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include "../drawui.h"
#include "../main.h"
#include "../globalData.h"

#include "../string/_string.h"
#include "fileHandler.h"

enum{
	FILE_ISDIR,//文件是个目录
	FILE_NOENT,//文件不存在
	FILE_ELSE,//其他
};

enum{
	FILE_COMMAND_ACCESS = 0,
	FILE_COMMAND_FAIL 	= 1,
	FILE_SHELL_FAIL 	= 2,
	FILE_EXIST 			= 3,
	PATH_NOT_EXIST 		= 4,
	FILE_NOT_EXIST		= 5,
};

int fileStat(const char* fileName)
{
	struct stat buf;
	if (0 == stat(fileName, &buf))
	{
		if(S_ISDIR(buf.st_mode))
		{
            return FILE_ISDIR;
		}
	}
	if(ENOENT == errno)
	{
		return FILE_NOENT;
	}
	return FILE_ELSE;
}

int FileCommand(const char* command)
{
	int status;
	status = system(command);
	//printf("%s\n",command);
	 if (-1 != status)//shell命令正确条件1
	 {
		 //printf("exit status value = [0x%x]\n", status);
		 if (WIFEXITED(status))//shell命令正确条件2
		 {
			 if (0 == WEXITSTATUS(status))//shell命令正确条件3
			 {//
				//printf("run shell script successfully.\n");
				return FILE_COMMAND_ACCESS;
			 }
			 else
			 {
				 //printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
				 return FILE_SHELL_FAIL;
			 }
		 }
		 else
		 {
			 //printf("exit status = [%d]\n", WEXITSTATUS(status));
			 return FILE_SHELL_FAIL;
		 }
	 }
	// printf("system(%s) error!",command);
	 return FILE_COMMAND_FAIL;
}

int fileCopy(const char* fileName ,const char* path ,const char* commandOption)
{
    char command[256];
	if(FILE_ISDIR == fileStat(path))
	{
		memset(command,0,sizeof(command));
		strcpy(command,"cp ");
		if(commandOption && (0 != strcmp(" ",commandOption)))
		{
			strcat(command,commandOption);
			strcat(command," ");
		}
		strcat(command,"\"");
		strcat(command,fileName);
		strcat(command,"\" ");
		strcat(command,path);
		//printf("copy file:%s to path:%s\n",path,path);
		return FileCommand(command);
	}
	//printf("path:%s not exist!\n",path);
	return PATH_NOT_EXIST;
}

int fileMove(const char* fileName ,const char* path)
{
    char command[256];
	if(FILE_ISDIR == fileStat(path))
	{
		memset(command,0,sizeof(command));
		strcpy(command,"mv \"");
		strcat(command,fileName);
		strcat(command,"\" ");
		strcat(command,path);
		//printf("move file:%s ==> %s\n",fileName,path);
		return FileCommand(command);
	}
	//printf("path:%s not exist!\n",path);
	return PATH_NOT_EXIST;
}

int fileRename(const char* fileName ,const char* newFileName)
{
    char command[256];
	if(0 != access(newFileName, F_OK))
	{
		memset(command,0,sizeof(command));
		strcpy(command,"mv \"");//命名
		strcat(command,fileName);
		strcat(command,"\" ");
		strcat(command,newFileName);
		//printf("rename file:%s to %s\n",fileName,newFileName);
		return FileCommand(command);
	}
	//printf("file:%s exist!",newFileName);
	return FILE_EXIST;
}

int fileDelete(const char* fileName ,const char* commandOption)
{
    char command[256];
	if(0 == access(fileName, F_OK))//文件存在
	{
        memset(command,0,sizeof(command));
        strcpy(command,"rm ");
		if(commandOption && (0 != strcmp(" ",commandOption)))
		{
			strcat(command,commandOption);
			strcat(command," ");
		}
		strcat(command,"\"");
		strcat(command,fileName);
		strcat(command,"\"");
		//printf("delete file:%s!\n",fileName);
		return FileCommand(command);
	}
	//printf("file:%s not exist!\n",fileName);
	return FILE_NOT_EXIST;
}

int fileDetectUSB()
{
	if(TMP(bUSBStrorage))
		return 0 ;

	return 1 ;
#if 0 //ARM
	struct stat _tmp;
	int stat1 ;
	stat1 = stat("/dev/sda" , &_tmp);
	ret = stat1 ;
	return ret ;
#endif

}

int FileMkdir(const char* dirname)
{
	char cmd[100];
	memset(cmd ,0 ,100);
	sprintf(cmd ,"mkdir %s" ,dirname);
	return FileCommand(cmd);
}

int fileInit()
{
	if(FILE_ISDIR != fileStat(gTmpReportImagePath))
	{
		char fileCommand[100];
		memset(fileCommand ,0 ,100);
		sprintf(fileCommand ,"mkdir %s" ,gTmpReportPath);
		FileCommand(fileCommand);
		memset(fileCommand ,0 ,100);
		sprintf(fileCommand ,"mkdir %s" ,gTmpReportImagePath);
		FileCommand(fileCommand);
		if(FILE_ISDIR == fileStat(gTmpReportImagePath))
		{
			memset(fileCommand ,0 ,100);
			sprintf(fileCommand ,"cp %s %s" ,gLogoFile ,gTmpReportImagePath);
			FileCommand(fileCommand);
		}
	}//*/
	return TRUE;
}

//不带后缀和路径的文件名
void GetFileName(char* filename ,const char* fullfilename ,int disableSuffix)
{
	const char* _filename;
	const char* str = strrchr(fullfilename ,'/');
	if(str)
	{
		_filename = str + 1;
	}
	else
	{
		_filename = fullfilename;
	}
	if(disableSuffix)
	{
		str = strrchr(_filename ,'.');
		if(str)
		{
			memcpy(filename ,_filename ,str - _filename);
		}
		else
		{
			strcpy(filename ,_filename);
		}//*/
	}
	else
	{
		strcpy(filename ,_filename);
	}
}

int GetImageDirName(char* imageDirName ,const char* filename)
{
	int ret = 1;
	if(filename)
	{
		const char* str = strrchr(filename ,'.');
		if(str)
		{
			memcpy(imageDirName ,filename ,str - filename);
		}
		else
		{
			strcpy(imageDirName ,filename);
		}
		strcat(imageDirName ,"_files");
		ret = fileStat(imageDirName);
	}
	return ret;
}

int MkImageDirName(char* imageDirName ,const char* filename)
{
	int ret;
	if(filename)
	{
		GetImageDirName(imageDirName ,filename);
		char cmd[100];
		if(0 == access(imageDirName ,F_OK))
		{
			sprintf(cmd ,"rm %s -rf" ,imageDirName);
			system(cmd);
		}
		sprintf(cmd ,"mkdir %s" ,imageDirName);
		system(cmd);
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	return ret;
}

char* filenameCheck(const char* filename ,const char* suffix)
{
	if(filename)
	{
		const char* str = strrchr(filename ,'.');
		char* filenameReturn;
		if(str)
		{
			if(0 != strcmp(suffix ,str))
				filenameReturn = g_strdup_printf("%s%s" ,filename ,suffix);
			else
				filenameReturn = g_strdup_printf("%s" ,filename);
		}
		else
		{
			filenameReturn = g_strdup_printf("%s%s" ,filename ,suffix);
		}
		return filenameReturn;
	}
	else
	{
		return NULL;
	}
}

int fileVerify(fileHeadStruct* pFileHead ,int size)
{
	int ret = TRUE;
	if(pFileHead->type == FILETYPE_SETUP)
	{
		if(pFileHead->version < VERSION_SETUPFILE)
		{
			ret = FALSE;
		}
	}
	else if(pFileHead->type == FILETYPE_FOCALLAW)
	{

	}
	else
	{
		return FALSE;
	}

	if(pFileHead->size != size)
	{
		ret = FALSE;
	}
	return ret;
}

int saveConfigFile(const char* filename)
{
	int i;
	int fd;
	OutputSave();
	fd = open (filename ,O_RDWR | O_CREAT ,0644);
    //printf("%s\n" , filename);
	if(fd < 0)
	{
		char systemCommand[256];
		memset(systemCommand ,0 ,256);
		strcpy(systemCommand ,"rm \"");
		strcat(systemCommand ,filename);
		strcat(systemCommand ,"\"");
		system(systemCommand);
		g_print("error open config file:%s\n" ,filename);
		return 1;
	}
	else
	{
		fileHeadStruct fileHead;
		fileHead.type = FILETYPE_FOCALLAW;
		fileHead.size = sizeof(fileHeadStruct) + sizeof(CONFIG) + sizeof(struct systemStruct);
		fileHead.version = VERSION_SETUPFILE;

		strcpy(pp->p_config->cfg_end ,"abcdefgh");

		i = lseek (fd, 0, SEEK_SET);
		i = write (fd, &fileHead, sizeof(fileHeadStruct));
		i = write (fd, pp->p_config, sizeof(CONFIG));
		i = write (fd, &(gData->system), sizeof(struct systemStruct));
		fsync(fd) ;
		close (fd);
		system("sync");
		system("sync");
		return 0;
	}
}

int readConfigFile(const char* configFile)
{
	int fd;
	int ret;
	fd = open (configFile ,O_RDWR | O_CREAT ,0644);
	if (fd < 0)
	{
		char systemCommand[256];
		memset(systemCommand ,0 ,256);
		strcpy(systemCommand ,"rm \"");
		strcat(systemCommand ,configFile);
		strcat(systemCommand ,"\"");
		system(systemCommand);
		//g_print("error open config file:%s\n" ,configFile);
		ret = 1;
	}
	else
	{
		int fileSize = sizeof(fileHeadStruct) + sizeof(CONFIG) + sizeof(struct systemStruct);

		fileHeadStruct fileHead;
		lseek( fd,0,SEEK_SET );
		read (fd, &fileHead, sizeof(fileHeadStruct));

		if(fileVerify(&fileHead ,fileSize))
		{
			read (fd, pp->p_config, sizeof(CONFIG));
			read (fd, &(gData->system), sizeof(struct systemStruct));
			close (fd);
			//g_print("open %s successfully\n" ,configFile);
			ret = 0;
		}
		else
		{
			close (fd);
			//g_print("file:%s does not match\n" ,configFile);
			ret = 2;
		}
	}
	gData->file.loadFail = ret;
	return ret;
}

typedef struct _DRAW_PACK_
{
	int nGroupNum;
	int nBeamNum[8];
	int nJunction[8];
	//st_PART	part;
	int nScanStart ;
	int nScanEnd   ;
    int nScanResolution;
    int nInspecStart;
    int nInspecEnd;
    int nEncOrigin;
	int nEncodeType ;
	unsigned char bScanMark[1024 * 256] ;
}DRAW_INFO_PACK;


int WriteDataToFile(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	int ret = 0 ;
	ret = fwrite(ptr, size, nmemb, stream);
	if(ret == nmemb)  return 0 ;
	return -1 ;
}


int saveInspectData(const char* filename)
{
	g_debug("saveInspectData");

	DRAW_INFO_PACK _DrawInfor;
	int _nEncoderType = get_inspec_source(pp->p_config) ;
	int _nScanTimmerCounter, _bScanTimmerCircled  		;
	int _nDataSize       = pp->ScanInfor.DataSize    ;
	int _nScanStepQty    = pp->ScanInfor.ScanStepQty ;
	int _nMaxStoreIndex  = pp->ScanInfor.MaxStoreIndex;

	int _nScanStart      = pp->p_config->inspection_scan_start     ;
	int _nScanEnd        = pp->p_config->inspection_scan_end       ;
	int _nScanResolution = pp->p_config->inspection_scan_resolution;

	int i  ;
	char* _pStoreData ;
	unsigned char cEndFile = 0xfe;

    memset(&_DrawInfor, 0, sizeof(DRAW_INFO_PACK));

	_DrawInfor.nEncodeType = _nEncoderType ;
	_DrawInfor.nGroupNum = get_group_qty(pp->p_config);
	for (i = 0; i < 8; i++)
	{
		_DrawInfor.nBeamNum[i] = TMP(beam_qty[i]);
		_DrawInfor.nJunction[i] = TMP(Junction[i]);
	}

	FILE* _pFile;
	_pFile = fopen(filename,"w+");

	fileHeadStruct fileHead;
	fileHead.type = FILETYPE_FOCALLAW;
	fileHead.size = sizeof(fileHeadStruct) + _DrawInfor.nGroupNum * sizeof(GROUP) + sizeof(DRAW_INFO_PACK);
	if(pp->ScanStart)// if scan is started , save the scan data from store buffer
	{
		if(_nEncoderType)// encoder
		{
			fileHead.reserved =  _nDataSize * _nScanStepQty ;
			fileHead.version = fileHead.size + fileHead.reserved + 1;

			_DrawInfor.nScanStart      =  _nScanStart     ;
			_DrawInfor.nScanEnd        =  _nScanEnd       ;
			_DrawInfor.nScanResolution =  _nScanResolution;
            _DrawInfor.nEncOrigin = get_enc_origin(pp->p_config, get_cur_encoder(pp->p_config));
			memcpy((void*)_DrawInfor.bScanMark , (void*)ScanDataMark , _nScanStepQty);

			if(WriteDataToFile(&fileHead, sizeof(fileHeadStruct), 1 , _pFile))  return -1;
			if(WriteDataToFile(&_DrawInfor, sizeof(DRAW_INFO_PACK) , 1 , _pFile))  return -1;
			if(WriteDataToFile((void*)pp->p_config->group , _DrawInfor.nGroupNum * sizeof(GROUP), 1 , _pFile))  return -1;

//			if(WriteDataToFile((void*)(&_Extra) , sizeof(struct ExtraGlobalParaStruct) , 1 , _pFile))  return -1;

			if(WriteDataToFile((void*)TMP(StoreBuffAddr) , fileHead.reserved , 1 , _pFile))  return -1;
		}
		else // timmer
		{
			_nScanTimmerCounter  = DMA_MARK[9]  ;
			_bScanTimmerCircled  = DMA_MARK[10] ;
			if(_bScanTimmerCircled)
			{
				fileHead.reserved =  _nDataSize * _nMaxStoreIndex ;
				fileHead.version = fileHead.size + fileHead.reserved + 1;

				_nScanEnd         = _nMaxStoreIndex - 1;
				_nScanStart       = 0 ;
				_nScanResolution  = get_prf()/10 ;

				_nScanStart      *= 1000   ;
				_nScanEnd        *= 1000   ;
				_nScanResolution *= 1000   ;

				_DrawInfor.nScanStart      =  _nScanStart     ;
				_DrawInfor.nScanEnd        =  _nScanEnd       ;
				_DrawInfor.nScanResolution =  _nScanResolution;

				memset((void*)_DrawInfor.bScanMark , 0xff , _nMaxStoreIndex);

				if(WriteDataToFile(&fileHead, sizeof(fileHeadStruct), 1 , _pFile))  return -1;
				if(WriteDataToFile(&_DrawInfor, sizeof(DRAW_INFO_PACK) , 1 , _pFile))  return -1;
				if(WriteDataToFile((void*)pp->p_config->group , _DrawInfor.nGroupNum * sizeof(GROUP), 1 , _pFile))  return -1;

	//			if(WriteDataToFile((void*)(&_Extra) , sizeof(struct ExtraGlobalParaStruct) , 1 , _pFile))  return -1;

				_pStoreData = (char*)(TMP(StoreBuffAddr) + _nDataSize * (_nScanTimmerCounter + 1)) ;
				if(WriteDataToFile( _pStoreData , _nDataSize * (_nMaxStoreIndex - _nScanTimmerCounter - 1) , 1 , _pFile))  return -1;
				_pStoreData = (char*)TMP(StoreBuffAddr) ;
				if(WriteDataToFile( _pStoreData , _nDataSize * (_nScanTimmerCounter + 1 ), 1 , _pFile))  return -1;
			}
			else
			{
				fileHead.reserved =  _nDataSize * (_nScanTimmerCounter + 1) ;
				fileHead.version = fileHead.size + fileHead.reserved + 1;

				_nScanEnd         = _nScanTimmerCounter  ;
				_nScanStart       = 0 ;
				_nScanResolution  = get_prf()/10 ;
				_nScanStart      *= 1000   ;
				_nScanEnd        *= 1000   ;
				_nScanResolution *= 1000   ;

				_DrawInfor.nScanStart      =  _nScanStart     ;
				_DrawInfor.nScanEnd        =  _nScanEnd       ;
				_DrawInfor.nScanResolution =  _nScanResolution;
				memset((void*)_DrawInfor.bScanMark , 0xff ,  _nScanTimmerCounter + 1);
				if(WriteDataToFile(&fileHead, sizeof(fileHeadStruct), 1 , _pFile))  return -1;
				if(WriteDataToFile(&_DrawInfor, sizeof(DRAW_INFO_PACK) , 1 , _pFile))  return -1;
				if(WriteDataToFile((void*)pp->p_config->group , _DrawInfor.nGroupNum * sizeof(GROUP), 1 , _pFile))  return -1;

	//			if(WriteDataToFile((void*)(&_Extra) , sizeof(struct ExtraGlobalParaStruct) , 1 , _pFile))  return -1;

				if(WriteDataToFile((void*)TMP(StoreBuffAddr) , fileHead.reserved, 1 , _pFile))  return -1;
			}
		}
	}
	else // only save data of current frame
	{
		fileHead.reserved =  _nDataSize ;
		fileHead.version = fileHead.size + fileHead.reserved + 1;

		_DrawInfor.nScanStart      =  0     ;
		_DrawInfor.nScanEnd        =  pp->p_config->inspection_scan_end;
		_DrawInfor.nScanResolution =  1000  ;
		_DrawInfor.bScanMark[0]    =  0xff  ;

		if(WriteDataToFile(&fileHead, sizeof(fileHeadStruct), 1 , _pFile))  return -1;
		if(WriteDataToFile(&_DrawInfor, sizeof(DRAW_INFO_PACK) , 1 , _pFile))  return -1;
		if(WriteDataToFile((void*)pp->p_config->group , _DrawInfor.nGroupNum * sizeof(GROUP), 1 , _pFile))  return -1;

//		if(WriteDataToFile((void*)(&_Extra) , sizeof(struct ExtraGlobalParaStruct) , 1 , _pFile))  return -1;

		if(WriteDataToFile((void*)TMP(dma_data_add) , fileHead.reserved , 1 , _pFile))  return -1;
	}

	if(WriteDataToFile(&cEndFile,1,1,_pFile))  return -1;
	fsync(_pFile->_fileno) ;
	fsync(_pFile->_fileno) ;
	fclose(_pFile);
    system("sync");
    system("sync");

	return 0 ;

}

int checkDataFile(const char* strFileName_)
{
	int fd , ret = 0;
	fd = open ( strFileName_,O_RDWR | O_CREAT ,0644);
	if (fd < 0)
	{
		ret = 1;
	}
	else
	{

		fileHeadStruct fileHead;
		DRAW_INFO_PACK _DrawInfor ;
		lseek( fd, 0 , SEEK_SET );
		read ( fd, &fileHead, sizeof(fileHeadStruct));
		read ( fd, &_DrawInfor, sizeof(DRAW_INFO_PACK));
		int fileSize = sizeof(fileHeadStruct) + _DrawInfor.nGroupNum * sizeof(GROUP) + sizeof(DRAW_INFO_PACK);
		if(fileVerify(&fileHead ,fileSize))
		{
			ret = 0;
		}
		else
		{
			ret = 2;
		}
		close(fd);
	}
	return ret;
}

int ReadDataFileSetup(const char* dataFile)
{
	int fd ,retVar;
	fd = open (dataFile ,O_RDWR | O_CREAT ,0644);
	if (fd < 0)
	{
		char systemCommand[256];
		memset(systemCommand ,0 ,256);
		strcpy(systemCommand ,"rm \"");
		strcat(systemCommand ,dataFile);
		strcat(systemCommand ,"\"");
		system(systemCommand);
		g_print("error open data file:%s\n" ,dataFile);
		retVar = 1;
	}
	else
	{
		fileHeadStruct fileHead;
		DRAW_INFO_PACK _DrawInfor ;
		lseek( fd, 0 , SEEK_SET );
		read ( fd, &fileHead, sizeof(fileHeadStruct));
		read ( fd, &_DrawInfor, sizeof(DRAW_INFO_PACK));
		int fileSize = sizeof(fileHeadStruct) + _DrawInfor.nGroupNum * sizeof(GROUP) + sizeof(DRAW_INFO_PACK);
		if(fileVerify(&fileHead ,fileSize))
		{
			pp->p_config->groupId = 0 ;
			pp->p_config->groupQty= _DrawInfor.nGroupNum ;

			set_inspec_source(pp->p_config , _DrawInfor.nEncodeType );
			set_inspec_type(pp->p_config , 0) ;

			if(_DrawInfor.nScanResolution < 80)  _DrawInfor.nScanResolution = 1000 ;
			if(_DrawInfor.nEncodeType )
			{
				pp->p_config->inspection_scan_start  = _DrawInfor.nScanStart   ;
				pp->p_config->inspection_scan_end    = _DrawInfor.nScanEnd    ;
				pp->p_config->inspection_scan_resolution = _DrawInfor.nScanResolution ;
                set_enc_origin(pp->p_config, _DrawInfor.nEncOrigin, get_cur_encoder(pp->p_config));
                if (_DrawInfor.nEncOrigin != 0) {
                    enc_set_preset(pp->p_config, get_cur_encoder(pp->p_config), TRUE);
                }
			}
			else
			{
				pp->p_config->inspection_scan_start  = 0  ;
				pp->p_config->inspection_scan_end    = _DrawInfor.nScanEnd  ;
				pp->p_config->inspection_scan_resolution = 1000 ;
			}


			lseek( fd ,sizeof(fileHeadStruct) +  sizeof(DRAW_INFO_PACK) ,SEEK_SET );
			read (fd ,(void*)pp->p_config->group , _DrawInfor.nGroupNum * sizeof(GROUP));
			retVar = 0;
		}
		else
		{
			retVar = 2;
			g_print("data file:%s does not match" ,dataFile);
		}
		close(fd);
	}
	return retVar;

}

int ReadDataFileData(const char* dataFile)
{
	int fd ,retVar;
	int _nTmpValue ;
	fd = open (dataFile ,O_RDWR | O_CREAT ,0644);
	if (fd < 0)
	{
		char systemCommand[256];
		memset(systemCommand ,0 ,256);
		strcpy(systemCommand ,"rm \"");
		strcat(systemCommand ,dataFile);
		strcat(systemCommand ,"\"");
		system(systemCommand);
		g_print("error open data file:%s\n" ,dataFile);
		retVar = 1;
	}
	else
	{
		DRAW_INFO_PACK _DrawInfor;


		fileHeadStruct fileHead;


        memset(&_DrawInfor, 0, sizeof(DRAW_INFO_PACK));

		lseek( fd,0,SEEK_SET );
		read (fd, &fileHead, sizeof(fileHeadStruct));
		read (fd, &_DrawInfor, sizeof(DRAW_INFO_PACK));
		int fileSize = sizeof(fileHeadStruct) + _DrawInfor.nGroupNum * sizeof(GROUP) + sizeof(DRAW_INFO_PACK);
		if(fileVerify(&fileHead ,fileSize))
		{

			update_widget_bg(pp->event[19], 15);
			lseek( fd ,fileSize ,SEEK_SET );

			read(fd ,(unsigned char *)TMP(StoreBuffAddr) ,fileHead.reserved);

			if(_DrawInfor.nEncodeType )
			{
				DMA_MARK[0] = 1;
				_nTmpValue = (_DrawInfor.nScanEnd - _DrawInfor.nScanStart) / _DrawInfor.nScanResolution + 1;
			}
			else
			{
				DMA_MARK[0] = 1;
				_nTmpValue = (_DrawInfor.nScanEnd + 1000) / 1000;
				DMA_MARK[9] = 0  ;
			}

			memset((void*)ScanDataMark , 0 , 256 * 1024) ;
			memcpy((void*)ScanDataMark ,(void*)_DrawInfor.bScanMark ,  _nTmpValue);
			memcpy((void*)TMP(dma_data_add) ,(void*)TMP(StoreBuffAddr) ,fileHead.reserved /_nTmpValue);
			retVar = 0;
            TMP(loadData) = 1;
		}
		else
		{
			retVar = 2;
			g_print("data file:%s does not match" ,dataFile);
		}
		close(fd);
	}
	return retVar;

}
