/*
 * table.h
 *
 *  Created on: 2012-10-23
 *      Author: lzk
 */

#ifndef TABLE_H_
#define TABLE_H_

void tableInit();
void setFieldIndex(int list ,int fieldNO ,unsigned char value) ;
unsigned char getFieldIndex(int list ,int fieldNO) ;
void tableDisplay(int show) ;
int tableHasEntry() ;
void tableExport() ;
void tableAddEntry() ;
void tableDeleteEntry() ;

typedef struct
{
	FILE* fp ;
	const char* fileName;
	const char* imageDirName;
	int isTmp;
}
	reportParaStruct;
int SaveTableFile(const char* filename ,int isTmp);
void TableReport(reportParaStruct* pPara);
void CurrentViewReport(reportParaStruct* pPara);
#endif /* TABLE_H_ */
