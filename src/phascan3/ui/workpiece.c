/**
 * File: workpiece.c
 * Desc: load and paint workpiece
 * Author: JakeYang
 * Log: 2015-09-21
 */

#include "workpiece.h"

#include "../drawui.h"
#include "../callback.h"
#include "../main.h"

#include <gtk/gtk.h>
#include <math.h>

#include "dxf.h"
#include "dxf_paint.h"

typedef struct _WorkPieceInfo WorkPieceInfo;
struct _WorkPieceInfo {
    Dxf *dxf;
    gdouble basePointX;
    gdouble basePointY;
};

static WorkPieceInfo workPieceInfo[WORKPIECE_NUM];
static gboolean initFlag = FALSE;   /*初始化标志*/
static gint bitLock = 0;

void workpiece_init()
{
    if (initFlag) {
        return;
    }
    dxf_init();
    memset(workPieceInfo, 0, sizeof(workPieceInfo));
    initFlag = TRUE;
}

void workpiece_uninit()
{
    gint i;
    if (!initFlag) {
        return;
    }

    for (i = 0; i < WORKPIECE_NUM; ++i) {
        dxf_delete(workPieceInfo[i].dxf);
        workPieceInfo[i].dxf = NULL;
    }

    dxf_uninit();
}

gboolean workpiece_load_file(const gchar *filename, guint grp)
{
    g_return_val_if_fail( filename != NULL, FALSE );
    g_return_val_if_fail( grp < WORKPIECE_NUM, FALSE );

    Dxf *d = dxf_new_for_file(filename, DXF_PARSE_HEADER | DXF_PARSE_ENTITIES);

    g_bit_lock(&bitLock, grp);
    if (NULL != workPieceInfo[grp].dxf) {
        dxf_delete(workPieceInfo[grp].dxf);
        workPieceInfo[grp].dxf = NULL;
    }
    /*信息初始化*/
    workPieceInfo[grp].dxf = d;
    g_bit_unlock(&bitLock, grp);

    return TRUE;
}

void workpiece_paint(GtkWidget* widget , const DRAW_AREA_P prule , cairo_t* cr)
{
    WorkPieceInfo *info = NULL;
    gint width = 0;
    gint height = 0;
    DxfUnits yUnit = DXF_UNIT_NONE;
    DxfUnits xUnit = DXF_UNIT_NONE;
    gdouble scaleX = 0.0;
    gdouble scaleY = 0.0;
    gdouble ucsx = 0;
    gdouble ucsy = 0;

    g_return_if_fail( widget != NULL );
    g_return_if_fail( prule != NULL );
    g_return_if_fail( prule->group < WORKPIECE_NUM );
    g_return_if_fail( cr != NULL );

    g_bit_lock(&bitLock, prule->group);
    info = &workPieceInfo[prule->group];

    if (NULL == info->dxf
            || NULL == info->dxf->header) {
        g_bit_unlock(&bitLock, prule->group);
        return;
    }

    if (prule->w_unit == UNIT_MM) {
        xUnit = DXF_UNIT_MM;
    } else {
        xUnit = DXF_UNIT_INCHES;
    }
    if (prule->h1_unit == UNIT_MM) {
        yUnit = DXF_UNIT_MM;
    } else {
        yUnit = DXF_UNIT_INCHES;
    }

    gtk_widget_get_size_request(widget, &width, &height);
    scaleX = unit_conversion(info->dxf->header->insunits, xUnit)
            / (fabs(prule->wmax1-prule->wmin1)/(width-20-20-10));
    scaleY = unit_conversion(info->dxf->header->insunits, yUnit)
            / (fabs(prule->hmax1-prule->hmin1)/(height-20));

    /*ucs*/
    DxfHeader *h = info->dxf->header;
    if (h->ucsxdir.x == DXF_AXIS_X_RIGHT) {            /*ucs X轴向右*/
        ucsx = - h->ucsorg.x * scaleX;
    } else if (h->ucsxdir.x == DXF_AXIS_X_LEFT) {    /*ucs X轴向左*/
        ucsx = (width-20-20-10 - h->ucsorg.x) * scaleX;
    } else {                            /*无ucs，采用wcs X轴向右*/
        ucsx = 0;
    }
    if (h->ucsydir.y == DXF_AXIS_Y_UPWARD) {            /*ucs Y轴向上*/
        ucsy = (height-20 + h->ucsorg.y) * scaleY;
    } else if (h->ucsydir.y == DXF_AXIS_Y_DOWN){     /*ucs Y轴向下*/
        ucsy = (h->ucsorg.y) * scaleY;
    } else {                            /*无ucs， 采用wcs Y轴向上*/
        ucsy = (height-20) * scaleY;
    }

    cairo_save(cr);

    cairo_translate(cr,
                    info->basePointX + ucsx,
                    info->basePointY + ucsy);
    cairo_set_dash(cr, NULL, 0, 0);
    cairo_scale(cr, scaleX, scaleY);
    cairo_set_source_rgb(cr, 0, 1.0, 0);
    cairo_set_line_width(cr, 1/scaleX);

    dxf_paint(info->dxf, cr, scaleX, scaleY);
    g_bit_unlock(&bitLock, prule->group);

    cairo_restore(cr);
}

void workpiece_clean()
{
    gint i;

    for (i = 0; i < WORKPIECE_NUM; ++i) {
        g_bit_lock(&bitLock, i);
        dxf_delete(workPieceInfo[i].dxf);
        memset(&workPieceInfo[i], 0, sizeof(WorkPieceInfo));
        g_bit_unlock(&bitLock, i);
    }

    pp->p_config->bPartLoaded = 0 ;
}

void workpiece_clean_grp(guint grp)
{
    g_bit_lock(&bitLock, grp);
    dxf_delete(workPieceInfo[grp].dxf);
    workPieceInfo[grp].dxf = NULL;
    g_bit_unlock(&bitLock, grp);
}

void workpiece_basepoint(guint grp, gdouble *x, gdouble *y)
{
    g_return_if_fail( grp < WORKPIECE_NUM );
    g_return_if_fail( x != NULL );
    g_return_if_fail( y != NULL );

    g_bit_lock(&bitLock, grp);
    *x = workPieceInfo[grp].basePointX;
    *y = workPieceInfo[grp].basePointY;
    g_bit_unlock(&bitLock, grp);
}

void workpiece_set_basepoint(guint grp, gdouble x, gdouble y)
{
    g_return_if_fail( grp < WORKPIECE_NUM );

    g_bit_lock(&bitLock, grp);
    workPieceInfo[grp].basePointX = x;
    workPieceInfo[grp].basePointY = y;
    g_bit_unlock(&bitLock, grp);

}

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>
#include <gtk/gtk.h>

const double PI = 3.1415926  ;

PART_INFO  g_Part[8];
/*
PART_INFO  g_Part  =
{
	.pNccCmd  = NULL ,
	.nCmdQty  = 0 ,
	.nPosx    = 0 ,
	.nPosy    = 0 ,
};
*/
// clear PART INFOR
// model 0 : init model
// model 1 : free malloc memory
void ClearPartInfor(int nModel_)
{
	//printf("ClearPartInfor!\n");
	int i;PART_INFO_P _pInfo;
	if(nModel_)
	{
		for(i = 0 ; i< 8 ; i++)
		{
			_pInfo = &(g_Part[i]) ;
			if(_pInfo->pNccCmd)
			{
				free(_pInfo->pNccCmd);
			}
			_pInfo->nScaleRate = 1  ;

		}
	}
	memset((void*)g_Part , 0 , sizeof(PART_INFO) * 8);
	pp->p_config->bPartLoaded = 0 ;
}

double GetScaleRate(int grp)
{
	PART_INFO_P _pInfo;
	_pInfo = &(g_Part[grp]) ;
	return _pInfo->nScaleRate  ;
}

void SetScaleRage(int grp , double scale)
{
	PART_INFO_P _pInfo;
	_pInfo = &(g_Part[grp]) ;
	_pInfo->nScaleRate  = scale ;
}

void CvtCoordinate(FPoint_P source , FPoint_P result , DISPLAY_ZOOM_P pDisZoom_)
{
	result->x = pDisZoom_->nDisStartX + (source->x - pDisZoom_->nStartX) * pDisZoom_->nDisRangeX /pDisZoom_->nRangeX ;
	result->y = pDisZoom_->nDisStartY + (source->y - pDisZoom_->nStartY) * pDisZoom_->nDisRangeY /pDisZoom_->nRangeY ;
}


void CurrentPointMoveTo(int nCmdIndex_ , int nGroupId_)
{
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
	NCC_CMD_P _pCmd  = &(_pPart->pNccCmd[nCmdIndex_]) ;
	_pPart->nPosx = _pCmd->p1 ;
	_pPart->nPosy = _pCmd->p2 ;
}

void DrawLineTo(cairo_t* cr , int nCmdIndex_ ,DISPLAY_ZOOM_P pDisZoom_, int nGroupId_)
{
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
	NCC_CMD_P _pCmd  = &(_pPart->pNccCmd[nCmdIndex_]) ;
	//int _nPosx , _nPosy ;
	FPoint source , result ;
	source.x = _pPart->nPosx ;
	source.y = _pPart->nPosy ;
	CvtCoordinate(&source , &result ,  pDisZoom_);
	cairo_move_to(cr , result.x , result.y);
	source.x = _pCmd->p1 ;
	source.y = _pCmd->p2 ;
	CvtCoordinate(&source , &result ,  pDisZoom_);
	cairo_line_to(cr , result.x , result.y);

	cairo_stroke(cr);

	_pPart->nPosx = _pCmd->p1 ;
	_pPart->nPosy = _pCmd->p2 ;
}

void DrawCureTo(cairo_t* cr , int nCmdIndex_ , DISPLAY_ZOOM_P pDisZoom_, int nGroupId_)
{
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
	NCC_CMD_P _pCmd  = &(_pPart->pNccCmd[nCmdIndex_]) ;
	FPoint source , result , result1 , result2 ;
	source.x = _pPart->nPosx ;
	source.y = _pPart->nPosy ;
	CvtCoordinate(&source , &result ,  pDisZoom_);
	source.x = _pCmd->p1 ;
	source.y = _pCmd->p2 ;
	CvtCoordinate(&source , &result1 ,  pDisZoom_);
	source.x = _pCmd->p3 ;
	source.y = _pCmd->p4 ;
	CvtCoordinate(&source , &result2 ,  pDisZoom_);
	cairo_curve_to(cr,result.x,result.y,result1.x,result1.y,result2.x,result2.y) ;
	cairo_stroke(cr);

	_pPart->nPosx = _pCmd->p3 ;
	_pPart->nPosy = _pCmd->p4 ;
}

void DrawArcPositive(cairo_t* cr , int nCmdIndex_ , DISPLAY_ZOOM_P pDisZoom_, int nGroupId_)
{
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
	NCC_CMD_P _pCmd  = &(_pPart->pNccCmd[nCmdIndex_]) ;
	double _nPosx , _nPosy ;
	double _nPosx1 , _nPosy1 ;

	double _nTmp   ;
	int _nMode  = 1;
	_nTmp = _pCmd->p2 ;
	if(fabs(_nTmp - 1) < 0.5)  _nMode = 1 ;
	else if(fabs(_nTmp - 2) < 0.5)  _nMode = 2 ;
	else if(fabs(_nTmp - 3) < 0.5)  _nMode = 3 ;
	else if(fabs(_nTmp - 4) < 0.5)  _nMode = 4 ;
	_nTmp = _pCmd->p1 ; //* sin(80 * PI / 180) ;

	switch(_nMode)
	{
		case 1:
			_nPosx  = _nTmp + _pPart->nPosx - _pCmd->p1  ;
			_nPosy  = _nTmp + _pPart->nPosy  ;
			_nPosx1 = _pPart->nPosx - _pCmd->p1  ;
			_nPosy1 = _pCmd->p1 + _pPart->nPosy  ;
			break;
		case 2:
			_nPosx  = _pPart->nPosx  - _nTmp ;
			_nPosy  = _nTmp + _pPart->nPosy - _pCmd->p1;
			_nPosx1 = _pPart->nPosx - _pCmd->p1  ;
			_nPosy1 = _pPart->nPosy - _pCmd->p1 ;
			break;
		case 3:
			_nPosx  = _pCmd->p1 -_nTmp + _pPart->nPosx ;
			_nPosy  = -_nTmp + _pPart->nPosy  ;
			_nPosx1 = _pPart->nPosx + _pCmd->p1  ;
			_nPosy1 = -_pCmd->p1 + _pPart->nPosy  ;
			break;
		case 4:
			_nPosx  = _pPart->nPosx  + _nTmp ;
			_nPosy  = -_nTmp + _pPart->nPosy + _pCmd->p1;
			_nPosx1 = _pPart->nPosx + _pCmd->p1  ;
			_nPosy1 = _pPart->nPosy + _pCmd->p1 ;
			break;
		default:
			break;
	}

	FPoint source , result , result1 , result2 ;
	source.x = _pPart->nPosx ;
	source.y = _pPart->nPosy ;
	CvtCoordinate(&source , &result ,  pDisZoom_);
	source.x = _nPosx ;
	source.y = _nPosy ;
	CvtCoordinate(&source , &result1 ,  pDisZoom_);
	source.x = _nPosx1 ;
	source.y = _nPosy1 ;
	CvtCoordinate(&source , &result2 ,  pDisZoom_);
	cairo_curve_to(cr,result.x,result.y,result1.x,result1.y,result2.x,result2.y) ;

	//cairo_curve_to(cr,_pPart->nPosx,_pPart->nPosy,_nPosx,_nPosy,_nPosx1,_nPosy1) ;
	cairo_stroke(cr);
	_pPart->nPosx = _nPosx1 ;
	_pPart->nPosy = _nPosy1 ;
}

void DrawArcNegative(cairo_t* cr , int nCmdIndex_ , DISPLAY_ZOOM_P pDisZoom_ , int nGroupId_)
{
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
	NCC_CMD_P _pCmd  = &(_pPart->pNccCmd[nCmdIndex_]) ;
	double _nPosx , _nPosy ;
	double _nPosx1 , _nPosy1 ;

	double _nTmp   ;
	int _nMode  = 1;
	_nTmp = _pCmd->p2 ;
	if(fabs(_nTmp - 1) < 0.5)  _nMode = 1 ;
	else if(fabs(_nTmp - 2) < 0.5)  _nMode = 2 ;
	else if(fabs(_nTmp - 3) < 0.5)  _nMode = 3 ;
	else if(fabs(_nTmp - 4) < 0.5)  _nMode = 4 ;
	//_nTmp = _pCmd->p1 * sin(PI/4) ;
    _nTmp = _pCmd->p1 ; //* sin(80 * PI / 180) ;
	switch(_nMode)
	{
		case 1:
			_nPosx  = _nTmp + _pPart->nPosx - _pCmd->p1  ;
			_nPosy  = -_nTmp + _pPart->nPosy  ;
			_nPosx1 = _pPart->nPosx - _pCmd->p1  ;
			_nPosy1 = -_pCmd->p1 + _pPart->nPosy  ;
			break;
		case 2:
			_nPosx  = _pPart->nPosx  - _nTmp ;
			_nPosy  = -_nTmp + _pPart->nPosy + _pCmd->p1;
			_nPosx1 = _pPart->nPosx - _pCmd->p1  ;
			_nPosy1 = _pPart->nPosy + _pCmd->p1  ;
			break;
		case 3:
			_nPosx  = -_nTmp + _pPart->nPosx + _pCmd->p1 ;
			_nPosy  = _nTmp + _pPart->nPosy  ;
			_nPosx1 = _pPart->nPosx + _pCmd->p1  ;
			_nPosy1 = _pCmd->p1 + _pPart->nPosy  ;
			break;
		case 4:
			_nPosx  = _pPart->nPosx  + _nTmp ;
			_nPosy  = -_nTmp + _pPart->nPosy + _pCmd->p1;
			_nPosx1 = _pPart->nPosx + _pCmd->p1  ;
			_nPosy1 = _pPart->nPosy - _pCmd->p1 ;
			break;
		default:
			break;
	}

	FPoint source , result , result1 , result2 ;
	source.x = _pPart->nPosx ;
	source.y = _pPart->nPosy ;
	CvtCoordinate(&source , &result ,  pDisZoom_);
	source.x = _nPosx ;
	source.y = _nPosy ;
	CvtCoordinate(&source , &result1 ,  pDisZoom_);
	source.x = _nPosx1 ;
	source.y = _nPosy1 ;
	CvtCoordinate(&source , &result2 ,  pDisZoom_);
	cairo_curve_to(cr,result.x,result.y,result1.x,result1.y,result2.x,result2.y) ;

	//cairo_curve_to(cr,_pPart->nPosx,_pPart->nPosy,_nPosx,_nPosy,_nPosx1,_nPosy1) ;
	cairo_stroke(cr);

	_pPart->nPosx = _nPosx1 ;
	_pPart->nPosy = _nPosy1 ;
}

void _DrawPart(cairo_t* cr , DISPLAY_ZOOM_P pDisplayZoom , int nGroupId_)
{
	int i ;
	NCC_CMD_P _pCmd ;
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
	_pPart->nPosx = 0 ;
	_pPart->nPosy = 0 ;
	const double dashed1[] ={2.0, 2.0 };
	for(i = 0 ; i < _pPart->nCmdQty ; i++)
	{
		_pCmd = &(_pPart->pNccCmd[i])  ;
		switch(_pCmd->nCmd)
		{
			case NCC_CMD_NULL:
				break;
			case NCC_CMD_MOVE_TO:
				CurrentPointMoveTo(i , nGroupId_);
				//printf("%d-%f-%f-%f-%f\n" , _pCmd->nCmd , _pCmd->p1 , _pCmd->p2 , _pCmd->p3 , _pCmd->p4);
				break;
			case NCC_CMD_LINE_TO:
				DrawLineTo(cr , i , pDisplayZoom , nGroupId_);
				//printf("%d-%f-%f-%f-%f\n" , _pCmd->nCmd , _pCmd->p1 , _pCmd->p2 , _pCmd->p3 , _pCmd->p4);
				break;
			case NCC_CMD_CURVE_TO:
				DrawCureTo(cr , i , pDisplayZoom , nGroupId_);
				break;
			case NCC_CMD_ARC_POSITIVE:
				DrawArcPositive(cr , i , pDisplayZoom , nGroupId_);
				break;
			case NCC_CMD_ARC_NEGATIVE:
				DrawArcNegative(cr , i , pDisplayZoom , nGroupId_);
				break;
			case NCC_CMD_LINE_WIDTH:
				cairo_set_line_width (cr, _pCmd->p1);
				cairo_stroke(cr);
				break;
			case NCC_CMD_LINE_MODE:
				if(_pCmd->p1 > 0.5)
				{
					cairo_set_dash (cr, dashed1, 2, 0);
				}
				cairo_stroke(cr);

				break;
			case NCC_CMD_COLOR_CHANGE:
				if((_pCmd->p1 > 0.5)&&(_pCmd->p1 <= 1.5))
				{
					cairo_set_source_rgba( cr, 1 , 0 , 0 , 1.0 );
				}
				else if((_pCmd->p1 > 1.5)&&(_pCmd->p1 <= 2.5))
				{
					cairo_set_source_rgba( cr, 0 , 1 , 0 , 1.0 );
				}
				else if((_pCmd->p1 > 2.5)&&(_pCmd->p1 <= 3.5))
				{
					cairo_set_source_rgba( cr, 0 , 0 , 1 , 1.0 );
				}
				else
				{
					cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
				}
				cairo_stroke(cr);
				break;
			default:
				break;
		}
	}
	cairo_set_line_width (cr, 0.5);
	cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_stroke(cr);
}

int ThanslateCmdOneLine(char pStr_[5][10] , NCC_CMD_P pCmd_)
{
	char* _pError ;
	//printf("%s-%s-%s-%s-%s\n" ,  pStr_[0], pStr_[1] , pStr_[2], pStr_[3], pStr_[4]);
	if(pStr_[0][0] != 'G')  return -1  ;
	_pError = NULL ;
	pCmd_->nCmd  = (int)strtol( &(pStr_[0][1]) , &_pError , 10) ;
	if(*_pError)  return -1  ;
	_pError = NULL ;
	pCmd_->p1    = strtod( pStr_[1] , &_pError) ;
	if(*_pError)  return -1  ;
	_pError = NULL ;
	pCmd_->p2    = strtod( pStr_[2] , &_pError) ;
	if(*_pError)  return -1  ;
	_pError = NULL ;
	pCmd_->p3    = strtod( pStr_[3] , &_pError) ;
	if(*_pError)  return -1  ;
	_pError = NULL ;
	pCmd_->p4    = strtod( pStr_[4] , &_pError) ;
	if(*_pError)  return -1  ;

	return 0;
}

int ThanslateCmd(char* pStr_ , int nCmdQty_ , int nGroupId_)
{
	PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
    if(_pPart->pNccCmd)
    {
		free(_pPart->pNccCmd)  ;
		_pPart->pNccCmd = NULL ;
    }
    _pPart->pNccCmd  = (NCC_CMD_P)malloc(sizeof(NCC_CMD) * nCmdQty_);
    char* _pStr  =  pStr_ ;
    char  _strBuff[5][10] ;
    int i , j , k , t;
    for(i = 0 ; i< nCmdQty_ ; i++ )
    {
		k = 0 ;
		t = 0  ;
		memset((void*)_strBuff , 0 , 50) ;
		for(j = 0 ; j < 100 ; j++ , _pStr++ )
		{
			if(*_pStr == '\n' || k > 4)
			{
				j = 100 ;
				continue;
			}
			if(*_pStr == 32)
			{
				if(_pStr[1] != 32)
				{
						k++  ;
						t = 0 ;
				}
				continue;
			}
			_strBuff[k][t] = *_pStr ;
			t++ ;
		}
		if(ThanslateCmdOneLine(_strBuff , &(_pPart->pNccCmd[i])))
		{
			return -1 ;
		}
    }

	return 0;
}

int LoadWeldFile(const char* strPathName_ , int nGroupId_)
{
	int fd;  int i , _nFileLength;
	char _szBuff[100] ;
	char* _pStr       ;
	char _cTmp        ;
	_pStr = _szBuff   ;

	if ((fd = open(strPathName_, O_RDONLY )) < 0)
	{
		perror("open:");
		return -1 ;
	}
	else
	{
		memset(_pStr  , 0 , 100) ;
		i = read (fd, _pStr, sizeof("#DOPPLER NCC FILE") - 1);

		if(strcmp("#DOPPLER NCC FILE" , _pStr))
		{
			//printf("DOPPLER NCC FILE Error!\n");
			return -1;
		}

		do
		{
			i = read (fd, &_cTmp, 1);
		}while(('\n' != _cTmp) && i)  ;
		_nFileLength = 0;
		do
		{
			i = read (fd, &_cTmp, 1);
			_nFileLength++ ;
		}while(('#' != _cTmp) && i)   ;


		memset(_pStr  , 0 , 100) ;
		i = read (fd, _pStr, sizeof("end") - 1);
		if(strcmp("end" , _pStr))
		{
			//printf("DOPPLER NCC FILE Error!\n");
			return -1;
		}
		//printf("File Read %d\n"  , _nFileLength);
		_pStr = malloc(_nFileLength);

		lseek (fd, 0, SEEK_SET);
		do
		{
			i = read (fd, &_cTmp, 1);
		}while(('\n' != _cTmp) && i)  ;
		read (fd, _pStr, _nFileLength);

		int nCmdQty = 0 ;
		for(i = 0 ; i < _nFileLength ; i++)
		{
			if(_pStr[i] == 'G')
			{
				nCmdQty++  ;
			}
		}

		if(ThanslateCmd(_pStr , nCmdQty , nGroupId_))
		{
			return -1 ;
			//printf("Error!\n");
		}
		PART_INFO_P _pPart =  &(g_Part[nGroupId_]) ;
		_pPart->nCmdQty = nCmdQty ;
		NCC_CMD_P _pCmd = NULL ;
		for(i = 0 ; i < nCmdQty ; i++)
		{
			_pCmd = &(_pPart->pNccCmd[i])   ;
			//printf("%d-%f-%f-%f-%f\n" , _pCmd->nCmd , _pCmd->p1 , _pCmd->p2 , _pCmd->p3 , _pCmd->p4);
		}
		free(_pStr);

		close (fd);
	}
	return 0;
}


void DrawPart(GtkWidget* widget , gpointer data , cairo_t* cr)
{
	int w,h ;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	DISPLAY_ZOOM DisplayZoom ;
	unsigned char grp = prule->group ;

	if ((prule->scan_type != S_SCAN_A ) && (prule->scan_type != S_SCAN_L )
			&& (&(g_Part[grp])) )
	{
		return ;
	}


	gtk_widget_get_size_request (widget, &w, &h);
	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	w = w - 50 ;
	h = h - 20 ;

	//double current_pos ;
	double _nRulerStart = prule->wmin1   ;
	double _nRulerStop  = prule->wmax1   ;
	double sample_start = prule->hmin1 ;
	double sample_range = prule->hmax1 - sample_start;
	if(UNIT_INCH == get_unit(pp->p_config))
	{
		_nRulerStart = _nRulerStart /  0.03937;
		_nRulerStop = _nRulerStop /  0.03937;
		sample_start = sample_start /  0.03937;
		sample_range = sample_range /  0.03937;
	}

	//double _nZeroPosition = 20 -  w * _nRulerStart / (_nRulerStop - _nRulerStart) ;

	//cairo_set_line_width (cr, 0.5);
	//static const double dashed1[] ={5.0, 2.0 , 5.0};
	//cairo_set_source_rgba( cr, 0.8 , 0.8 , 0.8 , 1.0 );
	//current_pos = (int)_nZeroPosition + 0.5  ;
	//if(current_pos >= 20 && current_pos <= w + 20)
	//{
	//	cairo_set_dash (cr, dashed1, 3, 0);
	//	cairo_move_to (cr, current_pos  , 0  );
	//	cairo_line_to (cr, current_pos  , h  );
	//	cairo_stroke(cr);
	//	cairo_set_dash (cr, NULL, 0, 0);
	//	cairo_stroke(cr);
	//}

	DisplayZoom.nDisStartX = 20 ;
	DisplayZoom.nDisStartY = 0  ;
	DisplayZoom.nDisRangeX = w  ;
	DisplayZoom.nDisRangeY = h  ;
	DisplayZoom.nStartX = _nRulerStart ;
	DisplayZoom.nStartY = sample_start ;
	DisplayZoom.nRangeX = _nRulerStop - _nRulerStart ;
	DisplayZoom.nRangeY = sample_range ;
	//printf("sample_start , sample_range , _nRulerStart , _nRulerStop \n%f %f %f %f\n" , sample_start , sample_range , _nRulerStart , _nRulerStop);
	//LoadWeldFile("dpl.ncc") ;
	_DrawPart(cr , &DisplayZoom , grp);

}
#endif
