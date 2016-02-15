/*
 * fileHeadler.h
 *
 *  Created on: 2012-3-30
 *      Author: lzk
 */

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_


int FileCommand(const char* command);
extern int fileStat(const char* fileName);
extern int fileCopy(const char* fileName ,const char* path ,const char* commandOption);
extern int fileMove(const char* fileName ,const char* path);
extern int fileRename(const char* fileName ,const char* newFileName);
extern int fileDelete(const char* fileName ,const char* commandOption);
extern int fileDetectUSB();

int fileInit();
void GetFileName(char* filename ,const char* fullfilename ,int disableSuffix);
#define GetDirName(x ,y) GetFileName(x ,y ,FALSE)
int GetImageDirName(char* imageDirName ,const char* filename);
int MkImageDirName(char* imageDirName ,const char* filename);
extern char* filenameCheck(const char* filename ,const char* suffix);
extern int readConfigFile(const char* configFile);
extern int saveConfigFile(const char* configFile);
extern int readInspectDataFile(const char* dataFile);

typedef struct{
	const char* filename;
	unsigned int fileSize;
	const char* fileSaveDate;
	const char* softwareVersion;
	const char* module;
	const char* note;
	const char* head;
	const char* channelQuantity;
	int maskOfUserFeild;
	const char* labelUserFeild[10];
	const char* nameUserFeild[10];
}
	tmpHtmlStruct;
void getHttpFileName(char* newFileName ,const char* filename);
extern int htmlStat(const char* tmpFileName ,tmpHtmlStruct* tmp);
void changeNewLine(char* strHtml ,const char* str);

enum
{
	FILETYPE_SETUP = 0x1,
	FILETYPE_FOCALLAW = 0xaabbcc,
};

typedef struct
{
	int type;
	int version;
	int size;//校验大小，不是文件大小
	int reserved;
}
	fileHeadStruct;
int fileVerify(fileHeadStruct* pFileHead ,int size);
extern int saveInspectData(const char* strFileName_);
int ReadDataFileSetup(const char* dataFile);
int ReadDataFileData(const char* dataFile);
extern int checkDataFile(const char* dataFile);
#endif /* FILEHANDLER_H_ */
