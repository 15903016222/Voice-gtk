
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * file_op.h
 * 定义各种各种配置文件的路径 以及打开方法
 *
 *
 * 探头 楔块 报告 配置文件 数据
 *
 * 谭登华
 * 2011-03-07
 */

#include "file_op.h"
#include "drawui.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <glib/gprintf.h>

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "globalData.h"
#include "lzk/fileHandler.h"
#include "string/_string.h"
#include "ui/ui.h"
#define DEV_FB0 "/dev/fb0"
#define DEV_FB1 "/dev/fb1"

struct _FBInfo;
typedef struct _FBInfo FBInfo;
typedef int (*UnpackPixel)(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b);

struct _FBInfo
{
	int fd;
	UnpackPixel unpack;
	unsigned char *bits;
	struct fb_fix_screeninfo fi;
	struct fb_var_screeninfo vi;
};

#define fb_width(fb)  ((fb)->vi.xres)
#define fb_height(fb) ((fb)->vi.yres)
#define fb_bpp(fb)    ((fb)->vi.bits_per_pixel>>3)
#define fb_size(fb)   ((fb)->vi.xres * (fb)->vi.yres * fb_bpp(fb))

char SOURCE_FILE_NAME[FILE_NAME_MAX];
char SOURCE_FILE_PATH[FILE_NAME_MAX];
char TARGET_FILE_NAME[FILE_NAME_MAX];
char TARGET_FILE_PATH[FILE_NAME_MAX];

void SaveFrameBufferToImage(char *filename) ;
int Save_Inspec_Table(char *strFileName) ;
/* 探头 PA opp UT oup  */

int CheckProbeFileReasonable( PROBE_P p , int nGroupModel)
{

	//printf("PA_probe_type  %d - UT_probe_type  %d - Elem_qty  %d\n" ,p->PA_probe_type , p->UT_probe_type,  p->Elem_qty ) ;
	//printf("Model  %s - Serial  %s  \n"    ,  p->Model, p->Serial ) ;
	//printf("Pitch  %d - Reference_Point  %d  - Frequency %d \n"  ,p->Pitch , p->Reference_Point , p->Frequency ) ;

	if(nGroupModel == PA_SCAN)
	{
		if(p->Elem_qty < 1 || p->Elem_qty > 128)
		{
			return FALSE ;
		}
		if(p->Pitch < 10 || p->Pitch > 65000)
		{
			return FALSE ;
		}
		if(p->Reference_Point > 200000)
		{
			return FALSE ;
		}
	}

	return TRUE ;

}
int read_probe_file (const gchar *file_path, PROBE_P p)
{
	int fd;
	int	i; 
	GROUP* p_grp ;
	PROBE _TmpProbe  ;
	int grp ;
	if (pp->pos != 0)
	{
		grp = get_current_group(pp->p_config);
		p_grp = get_group_by_id (pp->p_config, grp);
	}
	else
	{
		//其实要指明一个临时的group
		p_grp = &g_tmp_group_struct;
	}
	memset((void*)(&_TmpProbe) , 0 , sizeof(PROBE));
	if ((fd = open(file_path, O_RDONLY )) < 0)
	{
		perror("open:");
		return FALSE ;
	}
	else 
	{
		if (p_grp->group_mode == PA_SCAN || p_grp->group_mode == UT_SCAN)
		{
			lseek (fd, 4, SEEK_SET);
			i = read (fd, &_TmpProbe, sizeof(PROBE) - 4);
			if(!CheckProbeFileReasonable(&_TmpProbe , PA_SCAN))
				return FALSE;
		}
		else //if (GROUP_VAL(group_mode) == UT12_SCAN)
		{
			i = read (fd, &_TmpProbe, sizeof(PROBE) );
			_TmpProbe.Frequency = _TmpProbe.Elem_qty | (_TmpProbe.Freq2 << 8);
			if(!CheckProbeFileReasonable(&_TmpProbe , UT_SCAN))
				return FALSE;
		}
		close (fd);
	}
	memcpy(p , &_TmpProbe, sizeof(PROBE) );
	return TRUE ;
}

/* 探头 PA opp UT oup  */
void save_probe_file (const gchar *file_path, PROBE_P p)
{
	int fd;
	int	i; 
	int grp ;
	GROUP* p_grp ;
	if (pp->pos != 0)
	{
		grp = get_current_group(pp->p_config);
		p_grp = get_group_by_id (pp->p_config, grp);
	}
	else
	{
		//其实要指明一个临时的group
		p_grp = &g_tmp_group_struct;
	}
    char tmp[] = "    ";
	if ((fd = open(file_path,  O_CREAT| O_RDWR,0666))<0) 
	{
		perror("open:");
	}
	else 
	{
		if (p_grp->group_mode == PA_SCAN || p_grp->group_mode == UT_SCAN)
		{
			i = write (fd,tmp,strlen(tmp));
			i = write (fd, p, sizeof(PROBE) - 4);
		}
		else// if (GROUP_VAL(group_mode) == UT_SCAN)
		{
			//p->Frequency = p->Elem_qty | (p->Freq2 << 8);
			p->Freq2 = (p->Frequency & 0xff00) >> 8;
			p->Elem_qty = p->Frequency & 0xff;
			i = write (fd, p, sizeof(PROBE) );
		}
		close (fd);
	}
}


int CheckWedgeFileReasonable( WEDGE_P pWedge_ , int nWedgeType_)
{
	return TRUE ;
	if(pWedge_->Angle >= 900)
	{
		return FALSE ;
	}

	if(nWedgeType_ == PA_SCAN)
	{
		if(pWedge_->Velocity_PA < 600000 || pWedge_->Velocity_PA > 10000000)  return FALSE ;
		if(pWedge_->Orientation != 0 || pWedge_->Orientation != 1)            return FALSE ;
		if(pWedge_->Height > 1000000)         								  return FALSE ;
		if(pWedge_->Primary_offset > 1000000)            					  return FALSE ;
		if(pWedge_->Secondary_offset > 1000000)            					  return FALSE ;
	}
	else
	{
		if(pWedge_->Velocity_UT < 600000 || pWedge_->Velocity_UT > 10000000)  return FALSE ;
		if(pWedge_->Ref_point < 0 || pWedge_->Ref_point > 1000000)            return FALSE ;
		if(pWedge_->Probe_delay < 0 || pWedge_->Probe_delay > 100000)         return FALSE ;
	}

	return TRUE ;
}

/* 楔块 PA opw UT ouw */
int read_wedge_file (const gchar *file_path, WEDGE_P p)
{
	int fd;
	int	i; 
	WEDGE _TmpWedge  ;
	int _nWedgeType  ;

	memset((void*)&_TmpWedge , 0 , sizeof(WEDGE)) ;
	int nPathLength  = strlen(file_path);
	if(strcmp(file_path + nPathLength - 4 , ".opw") == 0 )
	{
		_nWedgeType = PA_SCAN  ;
	}
	else if(strcmp(file_path + nPathLength - 4 , ".ouw") == 0 )
	{
		_nWedgeType = UT_SCAN  ;
	}
	else
	{
		return FALSE ;
	}

	if ((fd = open(file_path, O_RDONLY ))<0) 
	{
		return FALSE ;
	}
	else 
	{
		i = read (fd, (void*) &_TmpWedge, 52);
		lseek (fd, 1, SEEK_CUR);
		i = read (fd, (void *)((int)( &_TmpWedge) + 52), 64);
		if(!CheckWedgeFileReasonable( &_TmpWedge , _nWedgeType))
		{
			return FALSE;
		}
	}
	memcpy((void*)p , (void*)(&_TmpWedge) , sizeof(WEDGE));
	close(fd);
	return TRUE  ;
}

/* 楔块 PA opw UT ouw */
void save_wedge_file (const gchar *file_path, WEDGE_P p)
{
	int fd;
	int	i; 
	char tmp[] = " ";

	if ((fd = open(file_path, O_CREAT| O_RDWR,0666))<0) 
	{
		perror("open:");
		//exit(1);
	}
	else 
	{
		i = write (fd, p, 52);
		//lseek (fd, 1, SEEK_CUR);
		i = write (fd,tmp,strlen(tmp));
		i = write (fd, (void *)((int)(p) + 52), 64);
	}

	close(fd);
}

int parseStory(xmlDocPtr doc, xmlNodePtr cur, int x, guint *sp_col, guint *col)
{
	xmlChar *key;
	xmlChar *Red, *Green, *Blue, *Fire;
	cur = cur->xmlChildrenNode;
	int  i = 0;
	while(cur != NULL)
	{
		if((!xmlStrcmp(cur->name, (const xmlChar*)"Version")))
		{
			key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			xmlFree(key);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"SpecialColors")))
		{
			parseStory(doc, cur, 1, sp_col, col);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"MainColors")))
		{
			parseStory(doc, cur, 2, sp_col, col);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"Color")))
		{
			Red		= xmlGetProp(cur, (const xmlChar*)"R");
			Green	= xmlGetProp(cur, (const xmlChar*)"G");
			Blue	= xmlGetProp(cur, (const xmlChar*)"B");
			Fire	= xmlGetProp(cur, (const xmlChar*)"F");

			if (x == 2)
				col[i++] = (atoi((const char *)(Red))) << 16 |
					(atoi((const char *)(Green))) << 8 | (atoi((const char *)Blue));
			else if (x == 1)
				sp_col[i++] = (atoi((const char *)(Red))) << 16 |
					(atoi((const char *)(Green))) << 8 | (atoi((const char *)Blue));
			xmlFree(Red);
			xmlFree(Green);
			xmlFree(Blue);
		}
		cur = cur->next;
	}
	return 0;

}

/* 读取XML的调色板信息 */
void read_palette_file (const gchar *file_path, guint *sp_col, guint *col)
{
	/* 定义2个指针 doc指向整个dom；cur指向结点 以后遍历树就靠这个指针 */
	xmlDocPtr doc;
	xmlNodePtr cur;
	/*获取doc指针 也是把其他格式转成utf8的功能 */
	doc = xmlParseFile (file_path);
	if (doc == NULL)
		return ;//exit(1);
	/* 取得结点指针 */
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL)
	{
		xmlFreeDoc (doc);
		return ;//exit(1);
	}
	/* 取得根结点指针 这里一定要是根结点*/
	if (xmlStrcmp(cur->name, (const xmlChar *)"Palette"))
	{
		xmlFreeDoc (doc);
		return ; //exit(1);
	}
	sp_col[0] = 0x12345678;
	sp_col[1] = 0x12345678;
	sp_col[2] = 0x12345678;
	/*通过这个递归函数，遍历出所有感兴趣的结点。*/
	parseStory (doc, cur, 0, sp_col, col);

	xmlFreeDoc (doc);

	return ;
}

/* 读取当前group的聚焦法则 */
void read_law_file (const gchar *file_path, gint group)
{
	char	version[16];
	gint	temp[32];
	int		i, j, k, l;
	FILE	*fd;

	if((fd = fopen(file_path, "rb")) == NULL)
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		l = fscanf(fd, "%s 	%d", version, &TMP(beam_qty[group]));
		k = TMP(beam_qty[group]);
		for (i = 0; i < k; i++)
		{
			l =	fscanf(fd, "%d 	 %d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d",
					&temp[0], &temp[1], &temp[2],/* &temp[3],*/
					&temp[4], &temp[5], &temp[6], &temp[7],
					&temp[8], &temp[9], &temp[10],&temp[11],
					&temp[12],&temp[13],&temp[14]);

			TMP(focal_law_all_beam[group][i]).N_ActiveElements	= temp[0];
			TMP(focal_law_all_beam[group][i]).frequency			= temp[1];
			TMP(focal_law_all_beam[group][i]).cycle				= temp[2];
			//TMP(focal_law_all_beam[offset + i]).sumgain				= temp[3];
			TMP(focal_law_all_beam[group][i]).mode				= temp[4];
			TMP(focal_law_all_beam[group][i]).filter				= temp[5];
			TMP(focal_law_all_beam[group][i]).R_angle				= temp[6];
			TMP(focal_law_all_beam[group][i]).S_angle				= temp[7];
			TMP(focal_law_all_beam[group][i]).T_first				= temp[8];
			TMP(focal_law_all_beam[group][i]).R_first				= temp[9];
			TMP(focal_law_all_beam[group][i]).Scan_offset			= temp[10];
			TMP(focal_law_all_beam[group][i]).Index_offset		    = temp[11] ;
			TMP(focal_law_all_beam[group][i]).G_delay				= temp[12];
			TMP(focal_law_all_beam[group][i]).F_depth				= temp[13];
			TMP(focal_law_all_beam[group][i]).M_velocity			= temp[14];

			for (j = 0; j < TMP(focal_law_all_beam[group][i]).N_ActiveElements; j++)
			{
				l = fscanf(fd, "%d 	%d 	%d 	%d 	%d 	%d", &temp[0],&temp[1],
						&temp[2], &temp[3], &temp[4], &temp[5]);
				TMP(focal_law_all_elem[group][i][j]).E_number		= temp[0];
				TMP(focal_law_all_elem[group][i][j]).FL_gain		= temp[1];
				TMP(focal_law_all_elem[group][i][j]).T_delay		= temp[2];
				TMP(focal_law_all_elem[group][i][j]).R_delay		= temp[3];
				TMP(focal_law_all_elem[group][i][j]).Amplitude	    = temp[4];
				TMP(focal_law_all_elem[group][i][j]).P_width		= temp[5];
			} 
		} 
	}
	fsync(fd->_fileno) ;
	fclose (fd);
}

/* 保存当前group的聚焦法则 */
void save_law_file (const gchar *file_path, gint group)
{
	int		i, j;
	FILE	*fd;

	if((fd = fopen(file_path, "wb+")) == NULL)
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		fprintf(fd, "%s 	%d\015\012", "dp1.00", TMP(beam_qty[group]));
		for (i = 0; i < TMP(beam_qty[group]); i++)
		{
			fprintf(fd, " %d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d\015\012",
					TMP(focal_law_all_beam[group][i]).N_ActiveElements,
					TMP(focal_law_all_beam[group][i]).frequency,
					TMP(focal_law_all_beam[group][i]).cycle,
					//TMP(focal_law_all_beam[offset + i]).sumgain,
					TMP(focal_law_all_beam[group][i]).mode,
					TMP(focal_law_all_beam[group][i]).filter,
					TMP(focal_law_all_beam[group][i]).R_angle,
					TMP(focal_law_all_beam[group][i]).S_angle,
					TMP(focal_law_all_beam[group][i]).T_first,
					TMP(focal_law_all_beam[group][i]).R_first,
					TMP(focal_law_all_beam[group][i]).Scan_offset,
					TMP(focal_law_all_beam[group][i]).Index_offset,
					TMP(focal_law_all_beam[group][i]).G_delay,
					TMP(focal_law_all_beam[group][i]).F_depth,
					TMP(focal_law_all_beam[group][i]).M_velocity );


			for (j = 0; j < TMP(focal_law_all_beam[group][i]).N_ActiveElements; j++)
			{
					fprintf(fd, "%d 	%d 	%d 	%d 	%d 	%d\015\012",          
							TMP(focal_law_all_elem[group][i][j]).E_number,
							TMP(focal_law_all_elem[group][i][j]).FL_gain,
							TMP(focal_law_all_elem[group][i][j]).T_delay,
							TMP(focal_law_all_elem[group][i][j]).R_delay,
							TMP(focal_law_all_elem[group][i][j]).Amplitude,
							TMP(focal_law_all_elem[group][i][j]).P_width );
			} 

		} 
	}
	fsync(fd->_fileno) ;
	fclose (fd);
}

int Set_Source_File_Path(char *path)
{
    assert(path != NULL);
    strcpy(SOURCE_FILE_PATH,path);
    return 0;
}

char *Get_Source_File_Path()
{
    return (char *)&SOURCE_FILE_PATH[0];
}

int Set_Target_File_Path(char *path)
{
    assert(path != NULL);
    strcpy(TARGET_FILE_PATH,path);
    return 0;
}

char *Get_Target_File_Path()
{
    return (char *)&TARGET_FILE_PATH[0];
}

int Set_Source_File_Name(char *name)
{
    assert(name != NULL);
    strcpy(SOURCE_FILE_NAME,name);
    return 0;
}

char *Get_Source_File_Name()
{
    return (char *)&SOURCE_FILE_NAME[0];
} 

int Set_Target_File_Name(char *name)
{
    assert(name != NULL);
    strcpy(TARGET_FILE_NAME,name);
    return 0;
}

char *Get_Target_File_Name()
{
    return (char *)&TARGET_FILE_NAME[0];
}

int Scan_Target_File(GtkWidget *widget,GtkTreeModel *model,char *strFileName)
{    
    int i;
    int value;
    char *value_name;
    GtkWidget *target_list = widget;
    GtkTreeIter target_iter;
    GtkTreeModel *target_model = model;

    value = gtk_tree_model_get_iter_from_string (target_model, &target_iter, "0");

    i = 0;

    while(value)
    {
        gtk_tree_model_get(target_model, &target_iter, 0, &value_name,  -1);
     
        if (strcmp(value_name,strFileName) == 0)
        {
			free(value_name);//add by lzk
            return -1;
        }
		free(value_name);//add by lzk
        value = gtk_tree_model_iter_next(target_model,&target_iter);
         
        i++; 
    }

    add_to_list(target_list,strFileName,i);

    return 0;
}

static int fb_unpack_rgb565(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	unsigned short color = *(unsigned short*)pixel;

	*r = ((color >> 11) & 0xff) << 3;
	*g = ((color >> 5) & 0xff)  << 2;
	*b = (color & 0xff )<< 3;

	return 0;
}

static int fb_unpack_rgb24(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	*r = pixel[fb->vi.red.offset>>3];
	*g = pixel[fb->vi.green.offset>>3];
	*b = pixel[fb->vi.blue.offset>>3];

	return 0;
}

static int fb_unpack_argb32(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	*r = pixel[fb->vi.red.offset>>3];
	*g = pixel[fb->vi.green.offset>>3];
	*b = pixel[fb->vi.blue.offset>>3];

	return 0;
}

static int fb_unpack_none(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	*r = *g = *b = 0;
	return 0;
}

static void set_pixel_unpacker(FBInfo* fb)
{
	if(fb_bpp(fb) == 2)
	{
		fb->unpack = fb_unpack_rgb565;
	}
	else if(fb_bpp(fb) == 3)
	{
		fb->unpack = fb_unpack_rgb24;
	}
	else if(fb_bpp(fb) == 4)
	{
		fb->unpack = fb_unpack_argb32;
	}
	else
	{
		fb->unpack = fb_unpack_none;
		printf("%s: not supported format.\n", __func__);
	}

	return;
}

static int fb_open(FBInfo* fb, const char* fbfilename)
{
	fb->fd = open(fbfilename, O_RDWR);

	if (fb->fd < 0)
	{
		fprintf(stderr, "can't open %s\n", fbfilename);

		return -1;
	}

	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fi) < 0)
		goto fail;

	if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vi) < 0)
		goto fail;

	fb->bits = mmap(0, fb_size(fb), PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);

	if (fb->bits == MAP_FAILED)
		goto fail;

	printf("---------------framebuffer---------------\n");
	printf("%s: \n  width : %8d\n  height: %8d\n  bpp   : %8d\n  r(%2d, %2d)\n  g(%2d, %2d)\n  b(%2d, %2d)\n",
		fbfilename, fb_width(fb), fb_height(fb), fb_bpp(fb), 
		fb->vi.red.offset, fb->vi.red.length,
		fb->vi.green.offset, fb->vi.green.length,
		fb->vi.blue.offset, fb->vi.blue.length);
	printf("-----------------------------------------\n");

	set_pixel_unpacker(fb);

	return 0;

fail:
	printf("%s is not a framebuffer.\n", fbfilename);
	close(fb->fd);

	return -1;
}

static void fb_close(FBInfo* fb)
{
	munmap(fb->bits, fb_size(fb));
	close(fb->fd);

	return;
}

static int snap2jpg(const char * filename, int quality, FBInfo* fb)
{
	//一行的字节数
	int row_stride = 0; 
	FILE * outfile = NULL;
	//一行位图
	JSAMPROW row_pointer[1] = {0};
	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;

	memset(&jerr, 0x00, sizeof(jerr));
	memset(&cinfo, 0x00, sizeof(cinfo));

	//设置错误的回调函数为jpeg_std_error
	cinfo.err = jpeg_std_error(&jerr);
	//初始化解码
	jpeg_create_compress(&cinfo);
	//打开输出文件
	if ((outfile = fopen(filename, "wb+")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", filename);
		return -1;
	}
	//把jpg的输出文件设为outfile
	jpeg_stdio_dest(&cinfo, outfile);
	//jpeg图像的宽
	cinfo.image_width = fb_width(fb);
	//jpeg图像的高
	cinfo.image_height = fb_height(fb);
	//jpeg图像的色彩为3，3为彩色，1为灰度
	cinfo.input_components = 3;
	//jpeg图像的色彩为JCS_RGB，JCS_RGB为彩色，JCS_GRAYSCALE为灰度
	cinfo.in_color_space = JCS_RGB;
	//其它东西设为默认值，上面的四个一定要设置
	jpeg_set_defaults(&cinfo);
	//设定编码jpeg的质量
	jpeg_set_quality(&cinfo, quality, TRUE);
	//开始压缩
	jpeg_start_compress(&cinfo, TRUE);
	//一行的字节数
	row_stride = fb_width(fb) * 2;
	//
	JSAMPLE* image_buffer = malloc(3 * fb_width(fb));
	//对每一行压缩
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		int i = 0;
		int offset = 0;
		unsigned char* line = fb->bits + cinfo.next_scanline * fb_width(fb) * fb_bpp(fb);

		for(i = 0; i < fb_width(fb); i++, offset += 3, line += fb_bpp(fb))
		{
			fb->unpack(fb, line, image_buffer+offset, image_buffer + offset + 1, image_buffer + offset + 2);
		}

		row_pointer[0] = image_buffer;
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	//结束解码
	jpeg_finish_compress(&cinfo);
	//关闭输出文件
	fsync(outfile->_fileno) ;
	fclose(outfile);
	//释放解码对像
	jpeg_destroy_compress(&cinfo);

	return 0;
}


//Ref: http://blog.chinaunix.net/space.php?uid=15059847&do=blog&cuid=2040565
static int snap2png(const char * filename, int quality, FBInfo* fb)
{
	FILE *outfile;
	if ((outfile = fopen(filename, "wb+")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", filename);
		return -1;
	}

	/* prepare the standard PNG structures */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	
	png_infop info_ptr = png_create_info_struct(png_ptr);

	/* setjmp() must be called in every function that calls a PNG-reading libpng function */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fsync(outfile->_fileno) ;
		fclose(outfile);
		return -1;
	}

	/* initialize the png structure */
	png_init_io(png_ptr, outfile);

	//
	int width = 0;
	int height = 0;
	int bit_depth = 8;
	int color_type = PNG_COLOR_TYPE_RGB;
	int interlace = 0;
	width = fb_width(fb);
	height = fb_height(fb);

	png_set_IHDR (png_ptr, info_ptr, width, height, bit_depth, color_type,
					(!interlace) ? PNG_INTERLACE_NONE : PNG_INTERLACE_ADAM7,
					PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* write the file header information */
	png_write_info(png_ptr, info_ptr);

	png_bytep row_pointers[height];
	png_byte* image_buffer = malloc(3 * width);

	int i = 0;
	int j = 0;
	unsigned char* line = NULL;
	for( ; i < height; i++ )
	{
		line = fb->bits + i * width * fb_bpp(fb);
		for(j = 0; j < width; j++, line += fb_bpp(fb))
		{
			int offset = j * 3;
			fb->unpack(fb, line, image_buffer+offset, image_buffer+offset+1, image_buffer+offset+2);
		}
		row_pointers[i] = image_buffer;
		png_write_rows(png_ptr, &row_pointers[i], 1);
	}
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fsync(outfile->_fileno) ;
	fclose(outfile);
	return 0;

}

//add by lzk
//allocation: the coordinates and width height of area to save
//include fb1
void saveDrawAreaToImage(const char* filename ,GtkAllocation allocation)
{
	FBInfo fb0;
	FBInfo fb1;

	memset(&fb0, 0x00, sizeof(fb0));
	memset(&fb1, 0x00, sizeof(fb1));
    // open fb0 fb1
	if (fb_open(&fb0, DEV_FB0 ) != 0)  return ;
	if (fb_open(&fb1, DEV_FB1 ) != 0)  return ;

	//get heigh and width of fb0 and fb1
	int width_fb0 = fb_width(&fb0);
	int height_fb0 = fb_height(&fb0);
	int width_fb1 = fb_width(&fb1);
	int height_fb1= fb_height(&fb1);

    //cp fb0 structure to tmp_fb
	FBInfo tmp_fb;
	memcpy(&tmp_fb, &fb0, sizeof(fb0));
	fb_width(&tmp_fb) = allocation.width;
	fb_height(&tmp_fb) = allocation.height;

	// malloc tmp memory for image storage
	tmp_fb.bits = (unsigned char*)malloc( allocation.width * allocation.height * fb_bpp(&fb0)) ;
    // format the image pixle pointer
	short* fb0_pixel = (short*)fb0.bits    ;
	short* fb1_pixel = (short*)fb1.bits    ;
	short* tmp_pixel = (short*)tmp_fb.bits ;
	short* tmp_p;
	int i ,j;

	const int x_fb1_offset = 20;//fb1在fb0坐标系中的坐标
	const int y_fb1_offset = 105;
	int x_fb1 = allocation.x - x_fb1_offset;//fb1坐标系中的起点坐标
	int y_fb1 = allocation.y - y_fb1_offset;

	assert(width_fb0 >= allocation.width + allocation.x);
	assert(height_fb0 >= allocation.height + allocation.y);

	for(j = 0 ;j < allocation.height ;++j)
	{
		for(i = 0 ;i < allocation.width ;++i)
		{
			tmp_p = tmp_pixel + i + j * allocation.width;
			*tmp_p = *(fb0_pixel + (i + allocation.x) + (j + allocation.y) * width_fb0);
			if(1 == *tmp_p)
			{
				if(
					(i + x_fb1 >= 0)//fb1坐标系中的坐标
					&& (j + y_fb1 >= 0)
					&& (i + x_fb1 <= width_fb1)
					&& (j + y_fb1 < height_fb1)//modify by lzk,= height_fb1时越界
				)
					*tmp_p = *(fb1_pixel + (i + x_fb1) + (j + y_fb1) * width_fb1);
			}
		}
	}

    // save tmp_fb to image
	if(strstr(filename, ".png") != NULL)
	{
		snap2png(filename, 100, &tmp_fb);
	}
	else
	{
		snap2jpg(filename, 100, &tmp_fb);
	}
	fb_close(&fb0);
	fb_close(&fb1);
    free(tmp_fb.bits);
}

#define DARK_LEVEL(a)  ((a&0x3f)+((a>>6)&0x1f)+((a>>11)&0x3f))
//全屏保存
void SaveFrameBufferToImage(char *filename)
{
	FBInfo fb0;
	FBInfo fb1;
	FBInfo tmp_fb;

	unsigned short* tmp_pixel;
	unsigned short* fb0_pixel;
	unsigned short* fb1_pixel;
	unsigned short* tmp_p    ;
	int i;
	int j;
	int width;
	int height;
	int width_fb1;
	int height_fb1;
	memset(&fb0, 0x00, sizeof(fb0));
	memset(&fb1, 0x00, sizeof(fb1));
    // open fb0 fb1
	if (fb_open(&fb0, DEV_FB0 ) != 0)  return ;
	if (fb_open(&fb1, DEV_FB1 ) != 0)  return ;
    //cp fb0 structure to tmp_fb
	memcpy(&tmp_fb, &fb0, sizeof(fb0));
	//get heigh and width of fb0 and fb1
	width = fb_width(&fb0);
	height = fb_height(&fb0);
	width_fb1 = fb_width(&fb1);
	height_fb1= fb_height(&fb1);
	// malloc tmp memory for image storage
	tmp_fb.bits = (unsigned char*)malloc( width * height * fb_bpp(&fb0)) ;
    // format the image pixle pointer
	fb0_pixel = (unsigned short*)fb0.bits    ;
	fb1_pixel = (unsigned short*)fb1.bits    ;
	tmp_pixel = (unsigned short*)tmp_fb.bits ;
    // cp fb0 pixels to tmp_fb
	memcpy(tmp_fb.bits, fb0.bits,  width * height * fb_bpp(&fb0));

    for( j = 105; j< 495; j++)
	{
		for( i = 20; i < 625; i++ )
		{
			tmp_p =  tmp_pixel + i + j*width;
			if(*tmp_p == 1)
			{
				*tmp_p = *(fb1_pixel + width_fb1*(j-105) + i - 20);
				if(DARK_LEVEL(*tmp_p) <= 10) *tmp_p = 0x0841 ;  // 保证保存的图片黑的地方为全黑,
				                               // 在REPORT显示 的时候 不至于 颜色穿透
			}
		}
	}

    for( j = 495; j< 515; j++)
	{
		for( i = 0; i < 675; i++ )
		{
			tmp_p =  tmp_pixel + i + j*width;
			if(*tmp_p == 1)
			{
				*tmp_p = 0x0841 ;  // 保证保存的图片黑的地方为全黑
			}
		}
	}
    // save tmp_fb to image
	if(strstr(filename, ".png") != NULL)
	{
		snap2png(filename, 100, &tmp_fb);
	}
	else
	{
		snap2jpg(filename, 100, &tmp_fb);
	}
	fb_close(&fb0);
	fb_close(&fb1);
    free(tmp_fb.bits);

	return ;
}

#define MAX_PATH_LENGTH 256

int _SaveInspectData()
{
	int i = 0;
	char  _strAppendix[20] ;
	char  _strTmp[MAX_PATH_LENGTH]  ;
	char  _strPath[MAX_PATH_LENGTH] ;
	char* _strFileName = get_file_name_all(pp->p_config);
	unsigned char _nStorageMedia = get_file_storage(pp->p_config);
    memset(_strTmp , 0 , MAX_PATH_LENGTH);

	if ((_strFileName == NULL)||(strcmp(_strFileName , "") == 0))
	{
		_strFileName = "InspectData";
	}

	if (_nStorageMedia == STORAGE_USB1)
	{
		if(fileDetectUSB())
			return TRUE;

		strcpy(_strTmp , USB_DEV_PATH );
	}
	else
	{
		if(!access(INSPECT_DATA_PATH , F_OK))
			strcpy(_strTmp , INSPECT_DATA_PATH );
		else
		{
			g_debug ("path invalidate\n");
			return TRUE;
		}
	}

    strcat(_strTmp , _strFileName);
	do
	{
		memset(_strPath , 0 , MAX_PATH_LENGTH);
		strcpy(_strPath , _strTmp)  ;
		sprintf(_strAppendix , "%03d.data" , i) ;
		strcat(_strPath, _strAppendix)    ;
		i++  ;
	}while(!access(_strPath , F_OK));

	int ret = FALSE ;
	ret = saveInspectData (_strPath);
	return ret ;
}

int _SaveInspectTabel()
{
	int i = 0;
	char _strAppendix[20] ;
	char _strTmp[MAX_PATH_LENGTH]  ;
	char _strPath[MAX_PATH_LENGTH] ;
	char* _strFileName = get_file_name_all(pp->p_config);
	unsigned char _nStorageMedia = get_file_storage(pp->p_config);
	memset(_strTmp , 0 , MAX_PATH_LENGTH);
	if ((_strFileName == NULL)||(strcmp(_strFileName , "") == 0))
	{
		_strFileName = "InspectTable";
	}

	if (_nStorageMedia == STORAGE_USB1)
	{
		if(fileDetectUSB())
			return TRUE;

		strcpy(_strTmp , USB_DEV_PATH );
	}
	else
	{
		if(!access(INSPECT_TABLE_PATH , F_OK))
			strcpy(_strTmp , INSPECT_TABLE_PATH);
		else
		{
			//printf("path invalidate\n");
			return TRUE;
		}
	}

    strcat(_strTmp , _strFileName);
	do
	{
		memset(_strPath , 0 , MAX_PATH_LENGTH);
		strcpy(_strPath , _strTmp)  ;
		sprintf(_strAppendix, "%03d.table" , i) ;
		strcat(_strPath, _strAppendix)    ;
		i++  ;
	}while(!access(_strPath , F_OK));

	Save_Inspec_Table (_strPath);
	return FALSE ;
}

int _SaveSceenImage()
{
	int i = 0;
	char  _strAppendix[20] ;
	char  _strTmp[MAX_PATH_LENGTH]  ;
	char  _strPath[MAX_PATH_LENGTH] ;
	char* _strFileName = get_file_name_all(pp->p_config);
	unsigned char _nStorageMedia = get_file_storage(pp->p_config);
	memset(_strTmp , 0 , MAX_PATH_LENGTH);
	if ((_strFileName == NULL)||(strcmp(_strFileName , "") == 0))
	{
		_strFileName = "Screen";
	}

	if (_nStorageMedia == STORAGE_USB1)
	{
		if(fileDetectUSB())
			return TRUE;

		strcpy(_strTmp , USB_DEV_PATH );
	}
	else
	{
		if(!access(INNER_IMAGE_PATH , F_OK))
			strcpy(_strTmp , INNER_IMAGE_PATH );
		else
		{
			//printf("path invalidate\n");
			return TRUE;
		}
	}

    strcat(_strTmp , _strFileName);
	do
	{
		memset(_strPath , 0 , MAX_PATH_LENGTH);
		strcpy(_strPath , _strTmp)  ;
		sprintf(_strAppendix,"%03d.jpg",i) ;
		strcat(_strPath,_strAppendix)    ;
		i++  ;
	}while(!access(_strPath , F_OK));

	SaveFrameBufferToImage( _strPath );
    return FALSE ;
}

extern void SaveReportFile(char *filename ,int isTmp);
int _SaveDetectReport()
{
	int i = 0;

	char  _strAppendix[20] ;
	char  _strTmp[MAX_PATH_LENGTH]  ;
	char  _strPath[MAX_PATH_LENGTH] ;
	char* _strFileName = get_file_name_all(pp->p_config);
	unsigned char _nStorageMedia = get_file_storage(pp->p_config);
	memset(_strTmp , 0 , MAX_PATH_LENGTH);
	if ((_strFileName == NULL)||(strcmp(_strFileName , "") == 0))
	{
		_strFileName = "Report";
	}

	if (_nStorageMedia == STORAGE_USB1)
	{
		if(fileDetectUSB())
			return TRUE;
		strcpy(_strTmp , USB_DEV_PATH );
	}
	else
	{
		if(!access(gReportPath ,F_OK))
			strcpy(_strTmp , gReportPath);
		else
		{
			//printf("path invalidate\n");
			return TRUE;
		}
	}

    strcat(_strTmp , _strFileName);
	do
	{
		memset(_strPath , 0 , MAX_PATH_LENGTH);
		strcpy(_strPath , _strTmp)  ;
		sprintf(_strAppendix,"%03d.html",i) ;
		strcat(_strPath,_strAppendix)    ;
		i++  ;
	}while(!access(_strPath , F_OK));

	SaveReportFile(_strPath ,FALSE);
	return FALSE ;
}

int SaveDataAll()
{
	int ret = FALSE;
#if ARM
	int _nSaveModel = get_file_save_mode(pp->p_config);
	if(_nSaveModel == SAVE_MODE_INSPEC_DATA)
	{
		ret = _SaveInspectData() ;
	}

	else if(_nSaveModel == SAVE_MODE_INSPEC_TABLE)
	{
		ret = _SaveInspectTabel() ;
	}

	else if(_nSaveModel == SAVE_MODE_SCREEN)
	{
		ret = _SaveSceenImage() ;
	}

	else if(_nSaveModel == SAVE_MODE_REPORT )
	{
		ret = _SaveDetectReport();
	}
	return ret ;
    // 故意留两次的，确保不出问题
	//system("sync");
	//system("sync");
#endif
	return ret ;
}

void report_table(char *strFileName)
{
    FILE *fp = NULL;
    fp = fopen(strFileName ,"r+");
    if(fp != NULL)
    {
        reportParaStruct para;
        reportParaStruct* pPara = &para;
    	char imageDirName[100];
    	memset(imageDirName ,0 ,100);
    	pPara->fp = fp;
    	MkImageDirName(imageDirName ,strFileName);
    	pPara->imageDirName = imageDirName;
    	pPara->fileName = strFileName;
    	pPara->isTmp = FALSE;

        fseek(fp,0,SEEK_END);
        fprintf(fp,"<br />\n");
        fprintf(fp,"Table\n");
    	TableReport(pPara) ;
        fsync(fp->_fileno) ;
    	fclose(fp);
    }
}

void report_currentView(char *strFileName)
{
    FILE *fp = NULL;
    fp = fopen(strFileName ,"r+");
    if(fp != NULL)
    {
        reportParaStruct para;
        reportParaStruct* pPara = &para;
    	char imageDirName[100];
    	memset(imageDirName ,0 ,100);
    	pPara->fp = fp;
    	MkImageDirName(imageDirName ,strFileName);
    	pPara->imageDirName = imageDirName;
    	pPara->fileName = strFileName;
    	pPara->isTmp = FALSE;

        fseek(fp,0,SEEK_END);
        fprintf(fp,"<br />\n");
        fprintf(fp,"Current View\n");
        CurrentViewReport(pPara) ;
        fsync(fp->_fileno) ;
    	fclose(fp);
    }
}


int Save_Inspec_Table(char *strFileName)
{
	FILE *fp;

	fp = fopen(strFileName,"w+");
	
	if (fp == NULL)
	{
		return -1;
	}

	//参考draw_field_name函数
	//例如，调用get_reading_field3
	//pp->field[get_reading_field3(pp->p_config)], pp->field_unit[get_reading_field3(pp->p_config)]);
	
	//参考draw_field_value函数
	//各个field的值存在TMP(field[0])到TMP(field[3])里面
	//TMP(field[0])
	fsync(fp->_fileno) ;
	fclose(fp);
	return 0;
}

//自动增加后辍名，如果没有后辍名就加上后辍名，有就不用加上。
void Auto_Add_Suffix_Name(char *path,char *strFileName,char *suffix_name)
{
	assert(path != NULL);
	assert(strFileName != NULL);
	assert(suffix_name != NULL);

	if ( strlen(strFileName)  <= strlen(suffix_name) )
	{
		strcat(path,suffix_name);
	}
	else if( strcmp(strFileName + strlen(strFileName) - strlen(suffix_name),suffix_name) != 0 )
	{
		strcat(path,suffix_name);
	}
}
// configure palette name
//char    szPalettePathAmplitude[50];
//char    szPalettePathTofd[50];
//char    szPalettePathCorrosion[50];
//**********
void LoadPalete()
{
	int i ;
	char  _strTmp[MAX_PATH_LENGTH]  ;
	char  _strPath[MAX_PATH_LENGTH] ;
	char* _strFileName ; //= get_file_name_all(pp->p_config);
    memset(_strPath , 0 , MAX_PATH_LENGTH);
    strcpy(_strPath , PALETTE_PATH);

    memset(_strTmp , 0 , MAX_PATH_LENGTH);
    strcpy(_strTmp , _strPath) ;
    _strFileName = pp->p_config->szPalettePathAmplitude ;
    strcat(_strTmp , _strFileName);
    //printf("1  %s\n" , _strTmp);
    struct stat _tmp;
    int stat1;
    stat1 = stat(_strTmp , &_tmp);
    if(S_ISREG(_tmp.st_mode) && !stat1)
    {
    	read_palette_file (_strTmp, TMP(t_special_col), TMP(t_color));  /*   */
    }
    else
    {
    	read_palette_file ("/home/tt/TT/source/system/Sample/Palette/ONDT_Amplitude.pal",
    				TMP(t_special_col), TMP(t_color));  /*   */
    	strcpy(_strFileName , "ONDT_Amplitude.pal") ;
    }
	for (i = 0; i < 256; i ++)
		TMP(color_amp[i]) = COL_24_TO_16(TMP(t_color)[i]);

	memset(_strTmp , 0 , MAX_PATH_LENGTH);
    strcpy(_strTmp , _strPath) ;
    _strFileName = pp->p_config->szPalettePathTofd ;
    strcat(_strTmp , _strFileName);
    //printf("2  %s\n" , _strTmp);
    stat1 = stat(_strTmp , &_tmp);
    if(S_ISREG(_tmp.st_mode) && !stat1)
    {
    	read_palette_file (_strTmp, TMP(t_special_col), TMP(t_color));  /*   */
    }
    else
    {
    	read_palette_file ("/home/tt/TT/source/system/Sample/Palette/ONDT_RFTOFD.pal",
    				TMP(t_special_col), TMP(t_color));  /*   */
    	strcpy(_strFileName , "ONDT_RFTOFD.pal") ;
    }
	for (i = 0; i < 256; i ++)
		TMP(color_tofd[i])  = COL_24_TO_16(TMP(t_color)[i]);

	memset(_strTmp , 0 , MAX_PATH_LENGTH);
    strcpy(_strTmp , _strPath) ;
    _strFileName = pp->p_config->szPalettePathCorrosion ;
    strcat(_strTmp , _strFileName);
    //printf("3  %s\n" , _strTmp);
    stat1 = stat(_strTmp , &_tmp);
    if(S_ISREG(_tmp.st_mode) && !stat1)
    {
    	read_palette_file (_strTmp, TMP(t_special_col), TMP(t_color));  /*   */
    }
    else
    {
    	read_palette_file ("/home/tt/TT/source/system/Sample/Palette/ONDT_Corrosion.pal",
    				TMP(t_special_col), TMP(t_color));  /*   */
    	strcpy(_strFileName , "ONDT_Corrosion.pal") ;
    }
	for (i = 0; i < 256; i ++)
		TMP(color_depth[i]) = COL_24_TO_16(TMP(t_color)[i]);

}

