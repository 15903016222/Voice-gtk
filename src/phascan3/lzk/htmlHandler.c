/*
 * htmlHandler.c
 *
 *  Created on: 2012-6-14
 *      Author: lzk
 */

#include <string.h>
#include "fileHandler.h"
#include "../drawui.h"
#include "../globalData.h"
#include "../string/_string.h"
#include "../ui/ui.h"
#include "../lzk/fft.h"

void getHttpFileName(char* newFileName ,const char* filename)
{
	int sizeofHttpDir = strlen(gThttpdPath);
	strcpy(newFileName ,WEB_PREFIX);
	if(0 == strncmp(gThttpdPath ,filename ,sizeofHttpDir))
	{
		strcat(newFileName ,filename + sizeofHttpDir);
	}
	else
	{
		strcat(newFileName ,filename);
	}
}

void changeNewLine(char* strHtml ,const char* str)
{
	if(str)
	{
		const char* newLine = "<br>";
		while(*str)
		{
			if('\n' == *str)
			{
				strcpy(strHtml ,newLine);
				strHtml += sizeof(newLine);
				str++;
			}
			else
			{
				*strHtml++ = *str++;
			}
		}
	}
}

//*****************生成setup，data用于webkit显示临时html文件*************
int htmlStat(const char* tmpFileName ,tmpHtmlStruct* tmp)
{
    FILE *fp = NULL;

    fp = fopen(tmpFileName,"w+");

    if(fp == NULL)
    {
        return 1;
    }

    const char* titleFormat = "b";
    const char* bodyFormat = "i";
    const char* newLineFormat = " style= \"word-break:break-all \"";

    fprintf(fp,"<html>\n");

    fprintf(fp ,"\t<body>\n");

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[0] ,titleFormat);
    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,bodyFormat ,newLineFormat ,tmp->filename ,bodyFormat);

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[1] ,titleFormat);
    fprintf(fp ,"\t\t<%s%s>%d</%s></br>\n"
    		,bodyFormat ,newLineFormat ,tmp->fileSize ,bodyFormat);

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[2] ,titleFormat);
    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,bodyFormat ,newLineFormat ,tmp->fileSaveDate ,bodyFormat);

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[3] ,titleFormat);
    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,bodyFormat ,newLineFormat ,tmp->softwareVersion ,bodyFormat);
/*
    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[4] ,titleFormat);
    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,bodyFormat ,newLineFormat ,tmp->module ,bodyFormat);

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[5] ,titleFormat);
    if(tmp->note)
    {
		fprintf(fp ,"\t\t<table width = \"100%%\" border=\"1\">\n");
		fprintf(fp ,"\t\t<td%s><%s>%s</%s></td>\n"
				,newLineFormat ,bodyFormat ,tmp->note ,bodyFormat);
		fprintf(fp ,"\t\t</tr>\n");
		fprintf(fp ,"\t\t</table>\n");
    }
    else
    {
		fprintf(fp ,"\t\t</br></br>\n");
    }

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[5] ,titleFormat);
    if(tmp->head)
    {
		fprintf(fp ,"\t\t<table width = \"100%%\" border=\"1\">\n");
		fprintf(fp ,"\t\t\t<tr>\n");
		fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
				,newLineFormat ,bodyFormat ,tmp->head ,bodyFormat);
		fprintf(fp ,"\t\t\t</tr>\n");
		fprintf(fp ,"\t\t</table>\n");
    }
    else
    {
		fprintf(fp ,"\t\t</br></br>\n");
    }

    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,titleFormat ,newLineFormat ,getCommonString(COMMONSTRING_TMP_HTML).stringDict[6] ,titleFormat);
    fprintf(fp ,"\t\t<%s%s>%s</%s></br>\n"
    		,bodyFormat ,newLineFormat ,tmp->channelQuantity ,bodyFormat);

    fprintf(fp ,"\t\t</br></br></br>\n");

    if(tmp->maskOfUserFeild > 0)
    {
		fprintf(fp ,"\t\t<table width = \"100%%\" border=\"1\">\n");
		fprintf(fp ,"\t\t\t<tr>\n");
		int i;
		for(i = 0 ;i < 10 ;++i)
		{
			if(0x01 == (0x01 & (tmp->maskOfUserFeild >> i)))
			{
				fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
						,newLineFormat ,bodyFormat ,tmp->labelUserFeild[i],bodyFormat);
				fprintf(fp ,"\t\t\t<td%s><%s>%s</%s></td>\n"
						,newLineFormat ,bodyFormat ,tmp->nameUserFeild[i],bodyFormat);
			}
		}
		fprintf(fp ,"\t\t\t</tr>\n");
		fprintf(fp ,"\t\t</table>\n");
    }
//*/
    fprintf(fp ,"\t</body>\n");

    fprintf(fp ,"</html>\n");

    fclose(fp);

    return 0;
}

//********report format***********
void fprintfReportStart(FILE* fp)
{
    fprintf(fp,"<html>\n");
    fprintf(fp,"<head>\n");
    fprintf(fp,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
    fprintf(fp,"<style>\n");
//    fprintf(fp,"<!--\n"); font-size: 9;
//    fprintf(fp,"table {font-size: 10;  font:\"tahoma\"; }\n");
//    fprintf(fp,"body {font-size: 10; font:\"tahoma\";  }\n");
    fprintf(fp,"h3 { page-break-before: always; }\n");
    fprintf(fp,"\n");
    fprintf(fp,".general_cell { width : 16%%; valign:\"bottom\"; }\n");
//    fprintf(fp,"-->\n");
    fprintf(fp,"</style>\n");
    fprintf(fp,"</head>\n");
    fprintf(fp,"<body>\n");
    fprintf(fp,"<br>\n");
}

void fprintfReportEnd(FILE* fp)
{
    fprintf(fp,"</body>\n");
    fprintf(fp,"</html>\n");
}

const char* tableWidth = "width=690 style=\"table-layout:fixed\"";
const char* tableThStyle = "class=\"general_cell\"  align=left";
const char* tableTdStyle = "class=\"general_cell\"";
const char* sonTableStyle = "width=100% style=\"table-layout:fixed\"";

void fprintfVoid(FILE* fp)
{
    fprintf(fp,"<td %s>-</td>\n" ,tableTdStyle);
}

#include <time.h>
#include "../globalData.h"
void fprintfReportHead(reportParaStruct* pPara)
{
    int i;
    FILE* fp = pPara->fp;
	fprintf(fp,"<table %s>\n" ,tableWidth);

	char fileName[100];
	memset(fileName ,0 ,100);
	GetDirName(fileName ,pPara->imageDirName);
    fprintf(fp,"<tr>\n");
	fprintf(fp,"<td align=left><img src = \"%s/logo.png\"></td>\n" ,fileName);
//	if(!pPara->isTmp)
	{
		char cmd[256];
		memset(cmd ,0 ,256);
		sprintf(cmd ,"cp %s %s" ,gLogoFile ,pPara->imageDirName);
		system(cmd);
		system("sync");
	}
	fprintf(fp,"<td align=right><h1>%s</h1></td>\n" ,getReportDictString(REPORTSTRINGDICT_TITLE ,0));
    fprintf(fp,"</tr>\n");
    fprintf(fp,"<tr>\n");
	fprintf(fp,"<td align=left style=\"word-break:break-all; word-wrap:break-word;\" colspan=2>%s</td>\n" ,get_header_info(pp->p_config));
    fprintf(fp,"</tr>\n\n");
	fprintf(fp,"</table>\n");

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_HEAD1) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_HEAD1 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    time_t timep;
    struct tm *p;
    time(&timep);
    p=localtime(&timep);
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%d / %d / %d</td>\n" ,tableTdStyle ,1900 + p->tm_year ,1 + p->tm_mon ,p->tm_mday);//"Report Date"
    fprintf(fp,"<td %s>1.0</td>\n" ,tableTdStyle);//"Report Version"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,gData->file.setupfile);//"Setup File Name"
    fprintf(fp,"<td %s>%d / %d / %d</td>\n" ,tableTdStyle ,1900 + p->tm_year ,1 + p->tm_mon ,p->tm_mday);//"Inspection Date"
    fprintf(fp,"<td %s>1.0</td>\n" ,tableTdStyle);//"Inspection Version"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[SAVE_MODE+get_file_save_mode (pp->p_config)]);//"Save Mode"
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_HEAD2) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_HEAD2 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>PhaScan</td>\n" ,tableTdStyle);//"Doppler Type"
	GetDirName(fileName ,pPara->fileName);
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,fileName);//"Data File Name"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
//    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//"Doppler Serial"
//    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//"Module Type"
//    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//"Module Serial #"
//    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//"Cailbration Due"
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table

    fprintf(fp,"<br>\n");
}

void fprintfReportUserField(FILE* fp)
{
	int i;
	int numOfUserField = 0;
	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table
	for (i=0 ;i<10 ;i++)
	{
		if (get_report_userfield_enable(pp->p_config ,i))
		{
			++numOfUserField;
			if(numOfUserField % 2)
			{
			    fprintf(fp,"<tr>\n");
			}
			fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,get_report_userfield_label(pp->p_config,i));
			fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,get_report_userfield_content(pp->p_config,i));
			fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);
			if(0 == numOfUserField % 2)
			{
			    fprintf(fp,"</tr>\n");
			}
		}
	}

    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportGroupStart(FILE* fp ,int group)
{
    fprintf(fp,"<br />\n");
    fprintf(fp ,"<h3></h3>");
	fprintf(fp,"<table %s>\n" ,tableWidth);
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td><hr /></td>\n" );
    fprintf(fp,"</tr>\n");
    fprintf(fp,"<tr>\n");
//    fprintf(fp,"<th align=left>Group %d</th>\n" ,group + 1);
    fprintf(fp,"<th align=left>%s</th>\n" ,getMainDictString(MAINSTRINGDICT_GROUP ,group));
    fprintf(fp,"</tr>\n");
    fprintf(fp,"</table>\n\n");
}

void FrpintfFft(FILE* fp ,double value)
{
	if(value < 0.00001)//0
	{
		fprintfVoid(fp);
	}
	else
	{
		fprintf(fp,"<td %s>%.2f</td>\n" ,tableTdStyle ,value);
	}
}

void fprintfReportGroupProbe(FILE* fp ,int group)
{
    int i;
    GROUP *pGroup = get_group_by_id (pp->p_config, group);
    fprintf(fp,"<br />\n");
    fprintf(fp,"%s\n" ,getReportDictString(REPORTSTRINGDICT_TITLE ,1));

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_PROBE1) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_PROBE1 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<Tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,pGroup->probe.Model);//"Probe Model"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,pGroup->probe.Serial);//"Probe Serial #"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_PROBE2) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_PROBE2 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%.1f MHz</td>\n" ,tableTdStyle ,pGroup->probe.Frequency * 0.001);//"Probe Frequency"
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * gData->fft[group].peakPoint);//"Peak Frequency"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,pGroup->wedge.Model);//"Wedge Model"
    fprintf(fp,"<td %s>%.1f °</td>\n" ,tableTdStyle ,pGroup->wedge.Angle * 0.1);//"Wedge Angle"
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Elem_qty));//"Probe Aperture"
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空1行
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_PROBE3) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_PROBE3 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>-6 dB</td>\n" ,tableTdStyle);//空
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * gData->fft[group].db6minPoint);//"Lower Frequency"
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * gData->fft[group].db6maxPoint);//"Higher Frequency"
    FrpintfFft(fp ,FftGetCenterFreq(&gData->fft[group] ,6));//"Center Frequency"
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * (gData->fft[group].db6maxPoint - gData->fft[group].db6minPoint));//"Bandwidth (MHz)"
    FrpintfFft(fp ,(gData->fft[group].db6maxPoint > gData->fft[group].db6minPoint) ?gData->fft[group].peakPoint * 100.0 / (gData->fft[group].db6maxPoint - gData->fft[group].db6minPoint) :0);//"Bandwidth (%%)"
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>-20 dB</td>\n" ,tableTdStyle);//空
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * gData->fft[group].db20minPoint);//"Lower Frequency"
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * gData->fft[group].db20maxPoint);//"Higher Frequency"
    FrpintfFft(fp ,FftGetCenterFreq(&gData->fft[group] ,20));//"Center Frequency"
    FrpintfFft(fp ,gData->fft[group].hzPerPoint * (gData->fft[group].db20maxPoint - gData->fft[group].db20minPoint));//"Bandwidth (MHz)"
    FrpintfFft(fp ,(gData->fft[group].db20maxPoint > gData->fft[group].db20minPoint) ?gData->fft[group].peakPoint * 100.0 / (gData->fft[group].db20maxPoint - gData->fft[group].db20minPoint) :0);//"Bandwidth (%%)"
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空1行
    fprintf(fp,"</tr>\n\n");
/*
    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_PROBE4) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_PROBE4 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Date
    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Time
    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Procedure
    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Calibration Block
    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Characterization Gain
    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//A%
    fprintf(fp,"</tr>\n\n");
//*/
    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportGroupSetup(FILE* fp ,int group)
{
    int i;
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	const char* unitUT = NULL ,*unit = NULL;
	int velocity = get_group_val (p_grp ,GROUP_VELOCITY);
	double dbVelocity = velocity / 200000.0;
	double factorUT = 1.0 ,multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		unit = "mm";
	}
	else
	{
		unit = "in";
		multiplier = 0.03937;
	}
	unsigned char utUnit;
	switch(utUnit = GROUP_VAL_POS( grp , ut_unit))
	{
	case UT_UNIT_TRUE_DEPTH:
	case UT_UNIT_SOUNDPATH:
		if(UNIT_MM == get_unit(pp->p_config))
		{
			unitUT = "mm";
		}
		else
		{
			unitUT = "in";
			factorUT = 0.03937;
		}
		break;
	case UT_UNIT_TIME:
	default:
		unitUT = "μs";
		break;
	}
    fprintf(fp,"<br />\n");
    fprintf(fp,"%s\n"  ,getReportDictString(REPORTSTRINGDICT_TITLE ,2));

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SETUP1) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SETUP1 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%.2f μs</td>\n" ,tableTdStyle ,GROUP_VAL_POS(grp , beam_delay[TMP(beam_num[grp])]) * 0.001);//Beam Delay
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_group_val (p_grp ,GROUP_START) * 0.001 * dbVelocity ,unit);//Start (Half Path)
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_group_val (p_grp ,GROUP_RANGE) * 0.001 * dbVelocity ,unit);//Range (Half Path)
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,GROUP_VAL_POS(0 , prf1) / 10);//PRF
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[ GROUP_MODE_P +(GROUP_VAL_POS(group,group_mode))]);//Type
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,1 << get_group_val (get_group_by_id (pp->p_config, group), GROUP_AVERAGING));//Averaging Factor
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SETUP2) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SETUP2 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,(int)((get_group_val (p_grp, GROUP_RANGE) * 0.1) / GROUP_VAL_POS(grp ,point_qty)));//Scale Factor
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[ OFF_ON + get_group_val (p_grp ,GROUP_VIDEO_FILTER)]);//Video Filter
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[ RECTIFIER + (get_group_val (p_grp ,GROUP_RECTIFIER))]);//Rectification
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[ FILTER + get_group_val (p_grp ,GROUP_FILTER)]);//Band-Pass Filter
//    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Scale Type
//    fprintf(fp,"<td %s>N/A</td>\n" ,tableTdStyle);//Pretrig.
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SETUP3) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SETUP3 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s V</td>\n" ,tableTdStyle ,menu_content[VOLTAGE + 3 + get_voltage (pp->p_config, grp)]);//Voltage
    fprintf(fp,"<td %s>%.1f dB</td>\n" ,tableTdStyle ,get_group_val (p_grp ,GROUP_GAIN) / 100.0);//Gain
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[ TX_RX_MODE + get_group_val (p_grp, GROUP_TX_RX_MODE)]);//Mode
    int material = get_part_material(group);
    int sw = velocity > get_material_sw(material) ? velocity - get_material_sw(material) :get_material_sw(material) - velocity;
    int lw = velocity > get_material_lw(material) ? velocity - get_material_lw(material) :get_material_lw(material) - velocity;
    if(sw < lw)
    {
		fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,1));//Wave Type //SW
    }
    else
    {
		fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,0));//Wave Type //LW
    }
    fprintf(fp,"<td %s>%.1f m/s</td>\n" ,tableTdStyle ,get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) * 0.01);//Sound Velocity
    fprintf(fp,"<td %s>%.1f ns</td>\n" ,tableTdStyle ,get_group_val (p_grp, GROUP_PW_VAL) / (double)PW_DIV);//Pulse Width
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SETUP4) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SETUP4 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%.1f %s</td>\n" ,tableTdStyle ,GROUP_VAL_POS(group,scan_offset) * 0.1 * multiplier ,unit);//Scan Offset
    fprintf(fp,"<td %s>%.1f %s</td>\n" ,tableTdStyle ,GROUP_VAL_POS(group,index_offset) * 0.1 * multiplier ,unit);//Index Offset
    fprintf(fp,"<td %s>%.1f °</td>\n" ,tableTdStyle ,GROUP_VAL_POS(group,skew) * 0.01);//Skew
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr><td colspan=6><hr /></td></tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SETUP5) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SETUP5 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>I</td>\n" ,tableTdStyle);//Gate
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[2].start * 0.001 * factorUT * dbVelocity ,unitUT);//Start
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[2].width * 0.001 * factorUT * dbVelocity ,unitUT);//Width
    fprintf(fp,"<td %s>%d %%</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[2].height);//Threshold
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[SYNCHRO + pp->p_config->group[group].gate[2].synchro]);//Synchro
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>A</td>\n" ,tableTdStyle);//Gate
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[0].start * 0.001 * factorUT * dbVelocity ,unitUT);//Start
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[0].width * 0.001 * factorUT * dbVelocity ,unitUT);//Width
    fprintf(fp,"<td %s>%d %%</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[0].height);//Threshold
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[SYNCHRO + pp->p_config->group[group].gate[0].synchro]);//Synchro
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>B</td>\n" ,tableTdStyle);//Gate
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[1].start * 0.001 * factorUT * dbVelocity ,unitUT);//Start
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[1].width * 0.001 * factorUT * dbVelocity ,unitUT);//Width
    fprintf(fp,"<td %s>%d %%</td>\n" ,tableTdStyle ,pp->p_config->group[group].gate[1].height);//Threshold
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[SYNCHRO + pp->p_config->group[group].gate[1].synchro]);//Synchro
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportGroupCalculator(FILE* fp ,int group)
{
    int i;
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
    int txRxMode = get_group_val(p_grp, GROUP_TX_RX_MODE);
    fprintf(fp,"<br />\n");

    fprintf(fp,"%s\n"  ,getReportDictString(REPORTSTRINGDICT_TITLE ,3));

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_CALCULATOR1) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_CALCULATOR1 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Elem_qty));//Used Element Qty.
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,First_tx_elem));//First TX Element
    fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Last_tx_elem));//Last TX Element
    switch(txRxMode)
    {
    case 0://pc
    case 2://tt
		fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,First_rx_elem));//First RX Element
		fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Last_rx_elem));//Last RX Element
    	break;
    case 1://pe
		fprintfVoid(fp);
		fprintfVoid(fp);
    	break;
    case 3://tofd
    default:
    	break;
    }
	if(LINEAR_SCAN == LAW_VAL_POS(group , Focal_type))//线扫
	{
		fprintf(fp,"<td %s>%d</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Elem_step));//Resolution
	}
	else// if(AZIMUTHAL_SCAN == LAW_VAL_POS(group , Focal_type))//扇扫
	{
		fprintfVoid(fp);
	}
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_CALCULATOR2) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_CALCULATOR2 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
	int velocity = get_group_val (get_group_by_id (pp->p_config,  get_current_group (pp->p_config)) ,GROUP_VELOCITY);
	int material = get_part_material(group);
	int sw = velocity > get_material_sw(material) ? velocity - get_material_sw(material) :get_material_sw(material) - velocity;
    int lw = velocity > get_material_lw(material) ? velocity - get_material_lw(material) :get_material_lw(material) - velocity;
    if(sw < lw)
    {
    	fprintf(fp,"<td %s>%s %d m/s</td>\n" ,tableTdStyle ,getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,1) ,velocity / 100);//Wave Type //SW
    }
    else
    {
    	fprintf(fp,"<td %s>%s %d m/s</td>\n" ,tableTdStyle ,getMainDictString(MAINSTRINGDICT_S_WAVE_TYPE ,0) ,velocity / 100);//Wave Type //LW
    }
//    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[WAVE_TYPE + LAW_VAL_POS(group,Wave_type)]);//Wave Type
    fprintf(fp,"<td %s>%.1f °</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Angle_min) * 0.01);//Start Angle
	if(AZIMUTHAL_SCAN == LAW_VAL_POS(group , Focal_type))//扇扫
	{
	    fprintf(fp,"<td %s>%.1f °</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Angle_max) * 0.01);//Stop Angle
	    fprintf(fp,"<td %s>%.1f °</td>\n" ,tableTdStyle ,LAW_VAL_POS(group,Angle_step)  * 0.01);//Angle Resolution
	}
	else// if(LINEAR_SCAN == LAW_VAL_POS(group , Focal_type))//线扫
	{
		fprintfVoid(fp);
		fprintfVoid(fp);
	}
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[L_CONFIG + LAW_VAL_POS(group , Focal_type)]);//Law Configuration
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);
    fprintf(fp,"</tr>\n\n");

    REPORTSTRINGDICT_ENUM strIndex;
    switch(LAW_VAL_POS(group , Focal_point_type))
    {
	case 2:
	    strIndex = REPORTSTRINGDICT_FOCALTYPE2;
		break;
	case 3:
	    strIndex = REPORTSTRINGDICT_FOCALTYPE3;
		break;
	case 0:
	case 1:
	default:
	    strIndex = REPORTSTRINGDICT_FOCALTYPE1;
		break;
    }
    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(strIndex) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(strIndex ,i));
    }
    fprintf(fp,"</tr>\n\n");

    const char* unit;
    double multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		unit = "mm";
	}
	else
	{
		unit = "in";
		multiplier = 0.03937;
	}
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[FOCAL_POINT_TYPE1 + LAW_VAL_POS(group , Focal_point_type)]);//Focal Type
    switch(LAW_VAL_POS(group , Focal_point_type))
    {
	case 0:
	case 1:
	    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Position_start) * 0.001 * multiplier ,unit);//Position Start
//	    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Position_end) * 0.001 * multiplier ,unit);//Position End
//	    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Position_step) * 0.001 * multiplier ,unit);//Position Step
		fprintfVoid(fp);
		fprintfVoid(fp);
		break;
	case 2:
	    switch(txRxMode)
	    {
	    case 0://pc
	    case 2://tt
		    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Offset_start) * 0.001 * multiplier ,unit);//Offset Start
	    	break;
	    case 1://pe
			fprintfVoid(fp);
	    	break;
	    default://tofd
	    	break;
	    }
		fprintfVoid(fp);
		fprintfVoid(fp);
		fprintfVoid(fp);
//	    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Position_end) * 0.001 * multiplier ,unit);//Position End
//	    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Position_step) * 0.001 * multiplier ,unit);//Position Step
//	    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Depth_end) * 0.001 * multiplier ,unit);//Depth End
		break;
	case 3:
	    switch(txRxMode)
	    {
	    case 0://pc
	    case 2://tt
		    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Offset_start) * 0.001 * multiplier ,unit);//Offset Start
		    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Offset_end) * 0.001 * multiplier ,unit);//Offset End
		    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Depth_start) * 0.001 * multiplier ,unit);//Depth Start
		    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,LAW_VAL_POS(group , Depth_end) * 0.001 * multiplier ,unit);//Depth End
	    	break;
	    case 1://pe
			fprintfVoid(fp);
			fprintfVoid(fp);
			fprintfVoid(fp);
			fprintfVoid(fp);
	    	break;
	    default://tofd
	    	break;
	    }
		break;
	default:
		break;
    }
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);
    fprintf(fp,"</tr>\n\n");


    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportGroupPart(FILE* fp ,int group)
{
    int i;
    fprintf(fp,"<br />\n");
    fprintf(fp,"%s\n"  ,getReportDictString(REPORTSTRINGDICT_TITLE ,4));

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_PART) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_PART ,i));
    }
    fprintf(fp,"</tr>\n\n");

    const char* unit;
    double multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		unit = "mm";
	}
	else
	{
		unit = "in";
		multiplier = 0.03937;
	}
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[MATERIAL + get_part_material(group)]);//Material
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[GEOMETRY + get_part_geometry(group)]);//Geometry
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,GROUP_VAL_POS(group , part.Thickness) * 0.001 * multiplier ,unit);//Thickness
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportGroupArea(FILE* fp ,int group)
{
    int i;
    fprintf(fp,"<br />\n");
    fprintf(fp,"%s\n"  ,getReportDictString(REPORTSTRINGDICT_TITLE ,5));

	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SCANAREA1) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SCANAREA1 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    const char* unit;
    double multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		unit = "mm";
	}
	else
	{
		unit = "in";
		multiplier = 0.03937;
	}
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_area_scanstart (pp->p_config) * 0.001 * multiplier ,unit);//Scan Start
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_area_scanend (pp->p_config) * 0.001 * multiplier ,unit);//Scan Length
    fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_area_scanresolution (pp->p_config) * 0.001 * multiplier ,unit);//Scan Resolution
    if(0 ==get_inspec_type (pp->p_config))//单线扫查
    {
    	fprintfVoid(fp);
    	fprintfVoid(fp);
    	fprintfVoid(fp);
    }
    else
    {
		fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_area_indexstart (pp->p_config) * 0.001 * multiplier ,unit);//Index Start
		fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_area_indexend (pp->p_config) * 0.001 * multiplier ,unit);//Index Length
		fprintf(fp,"<td %s>%.2f %s</td>\n" ,tableTdStyle ,get_area_indexresolution (pp->p_config) * 0.001 * multiplier ,unit);//Index Resolution
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SCANAREA2) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SCANAREA2 ,i));
    }
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    if(0 != get_inspec_source (pp->p_config))
    {
    	fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,con2_p[7][0][0]);//Synchro
    }
    else
    {
    	fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[I_SCAN]);//Synchro
    }
	fprintf(fp,"<td %s>%.2f %s/s</td>\n" ,tableTdStyle ,get_inspec_speed (pp->p_config) * 0.001 * multiplier ,unit);//Scan Speed
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr><td colspan=6><hr /></td></tr>\n\n");

    fprintf(fp,"<tr>\n");
    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SCANAREA3) ;++i)
    {
    	fprintf(fp,"<th %s>%s</th>\n" ,tableThStyle ,getReportDictString(REPORTSTRINGDICT_SCANAREA3 ,i));
    }
    fprintf(fp,"</tr>\n\n");

	unsigned char _nCurrentEcoderType ;
	unsigned char _nCurrentEcoderType2 ;
	unsigned char _nInspecSource = (int)get_inspec_source (pp->p_config);
	if(_nInspecSource == 0)
	{
		_nInspecSource = 1 ;
	}
	_nCurrentEcoderType  = get_enc_type (pp->p_config,  _nInspecSource - 1);
	_nCurrentEcoderType2  = (_nCurrentEcoderType >> 3)  & 1 ;
	_nCurrentEcoderType = (_nCurrentEcoderType & 0x3 ) ;

	_nCurrentEcoderType = 3 - _nCurrentEcoderType ;
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,con2_p[7][1][1]);//Axis
    if(0 != get_inspec_source (pp->p_config))
    {
		fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[I_SCAN + get_inspec_source (pp->p_config)]);//Encoder
		fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[E_TYPE + _nCurrentEcoderType]);//Encoder Type
		fprintf(fp,"<td %s>%.3f Step/mm</td>\n" ,tableTdStyle , get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config)) * 0.001);//Encoder Resolution
		fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,menu_content[POLARITY + _nCurrentEcoderType2 ]);//Polarity
    }
    else
    {
        fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Encoder
        fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Encoder Type
        fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Encoder Resolution
        fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Polarity
    }
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,con2_p[7][1][2]);//Axis
    fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Encoder
    fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Encoder Type
    fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Encoder Resolution
    fprintf(fp,"<td %s>Off</td>\n" ,tableTdStyle );//Polarity
    fprintf(fp,"<td %s>%s</td>\n" ,tableTdStyle ,tdSpace);//空
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportGroup(FILE* fp ,int group)
{
	fprintfReportGroupStart(fp ,group);

	if (get_report_format_probe(pp->p_config))
		fprintfReportGroupProbe(fp ,group);

	if (get_report_format_setup(pp->p_config))
		fprintfReportGroupSetup(fp ,group);

	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	if((3 != get_group_val(p_grp, GROUP_TX_RX_MODE))//not tofd
			&&(1 == GROUP_VAL_POS(group,group_mode)))//pa
	{
		fprintfReportGroupCalculator(fp ,group);
	}

	fprintfReportGroupPart(fp ,group);

	fprintfReportGroupArea(fp ,group);
}

void fprintfReportNote(FILE* fp)
{
	fprintf(fp,"<tr>\n");
    fprintf(fp,"<td width=10%%>%s</td>\n" ,getReportDictString(REPORTSTRINGDICT_TITLE ,6));
    fprintf(fp,"<td >%s</td>\n" ,tdSpace);
	fprintf(fp,"</tr>\n");

	fprintf(fp,"<tr><td style=\"word-break:break-all; word-wrap:break-word;\" colspan=2>%s</td></tr>\n" ,get_edit_notes_info(pp->p_config));
	fprintf(fp ,"<tr><td>%s</td></tr>\n" ,tdSpace);
}

void fprintfReportSignature(FILE* fp)
{
    int i;
    fprintf(fp,"<br></br>\n");
    fprintf(fp ,"<h3></h3>");
	fprintf(fp,"<table %s frame=box>\n<tr><td><table %s>\n" ,tableWidth ,sonTableStyle);//table

	//判断一下用户有没有选择生成那个notef
	if ( get_report_format_note(pp->p_config) )
		fprintfReportNote(fp);

    for(i = 0 ;i < getReportDictSize(REPORTSTRINGDICT_SIGNATURE) ;++i)
    {
       // fprintf(fp,"<tr>\n");
       // fprintf(fp,"<td>%s</td>\n" ,tdSpace);
		//fprintf(fp,"</tr>\n\n");

        fprintf(fp,"<tr>\n");
		fprintf(fp,"<td width=10%%>%s</td>\n" ,getReportDictString(REPORTSTRINGDICT_SIGNATURE ,i));
		fprintf(fp,"<td >%s</td>\n" , tdSpace);
		//fprintf(fp,"<td valign=bottom><table width=100%% frame=below></table></td>\n");
//		fprintf(fp,"<td>_______________________________________________________________________________________________</td>\n");
		fprintf(fp,"</tr>\n");

	    fprintf(fp,"<tr>\n");
	    fprintf(fp,"<td width=10%%>%s</td>\n"  ,tdSpace);
	    fprintf(fp,"<td><hr /></td>\n");
		fprintf(fp,"</tr>\n\n");
    }
  //  fprintf(fp,"<tr>\n");
  //  fprintf(fp,"<td>%s</td>\n" ,tdSpace);
	//fprintf(fp,"</tr>\n\n");

    fprintf(fp,"</table></td></tr>\n</table>\n\n");//table
}

void fprintfReportTable(reportParaStruct* pPara)
{
	FILE* fp = pPara->fp;
    fprintf(fp,"<br />\n");
    fprintf(fp ,"<h3></h3>");
	fprintf(fp,"<table %s>\n" ,tableWidth);
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td><hr /></td>\n" );
    fprintf(fp,"</tr>\n");
    fprintf(fp,"<tr>\n");
//    fprintf(fp,"<th align=left>%s</th>\n" ,getReportString(REPORTSTRING_TITLE).stringDict[7]);
    fprintf(fp,"<th align=left>%s</th>\n" ,getReportDictString(REPORTSTRINGDICT_TITLE ,7));
    fprintf(fp,"</tr>\n");
    fprintf(fp,"</table>\n\n");
    fprintf(fp,"<br />\n");
	TableReport(pPara) ;
}

void fprintfReportcurrentView(reportParaStruct* pPara)
{
	FILE* fp = pPara->fp;
	fprintf(fp,"<br />\n");
    fprintf(fp ,"<h3></h3>");
	fprintf(fp,"<table %s>\n" ,tableWidth);
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<td><hr /></td>\n" );
    fprintf(fp,"</tr>\n");
    fprintf(fp,"<tr>\n");
    fprintf(fp,"<th align=left>%s</th>\n" ,getReportDictString(REPORTSTRINGDICT_TITLE ,8));
    fprintf(fp,"</tr>\n");
    fprintf(fp,"</table>\n\n");
    fprintf(fp,"<br />\n");
	CurrentViewReport(pPara);
}

void SaveReportFile(const char* fullFileName ,int isTmp)
{
	int i;
    FILE *fp = NULL;

	fp = fopen(fullFileName ,"w+");

    if(fp == NULL)
    {
        return ;
    }
    reportParaStruct para;
    reportParaStruct* pPara = &para;
	char imageDirName[100];
	memset(imageDirName ,0 ,100);
	pPara->fp = fp;
	pPara->imageDirName = imageDirName;
	MkImageDirName(imageDirName ,fullFileName);
	pPara->fileName = fullFileName;
	pPara->isTmp = isTmp;

	//文件的头
	fprintfReportStart(fp);
	//html的head
	fprintfReportHead(pPara);
	//如果打开报告,userfield的功能
	if (get_report_format_userfield(pp->p_config))
	{
		//如果打开，就在报告那里增加userfield功能
		fprintfReportUserField(fp);
	}

	//枚举group，每一个group，都是一个独立的信息
    for (i = 0; i < get_group_qty (pp->p_config); ++i)
    {
        fprintfReportGroup(fp ,i);
    }

    unsigned char reportFormatView = get_report_format_view(pp->p_config);
	switch(reportFormatView)
	{
	case 0://table
		fprintfReportTable(pPara);
		break;
	case 1://current view
		fprintfReportcurrentView(pPara);
		break;
	default://None
		break;
	}

	fprintfReportSignature(fp);
	fprintfReportEnd(fp);
    fclose(fp);
/*
extern int PDFsaveToFile(const char* fname);
	if(NULL == filename)
	{
		char fname[100];
		strcpy(fname ,"/tmp/pdf.pdf");
		PDFsaveToFile(fname);
	}//*/
    system("sync");
    system("sync");
}
