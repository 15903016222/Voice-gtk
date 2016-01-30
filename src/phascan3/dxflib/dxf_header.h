/**
 * @file dxf_header.h
 * @brief dxf header层
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __DXF_HEADER_H__
#define __DXF_HEADER_H__

#include "dxfile.h"

#define HEADER_ACADMAINTVER     "$ACADMAINTVER" /*维护版本号（应忽略）*/
#define HEADER_ACADVER          "$ACADVER"      /*AutoCAD 图形数据库版本号：
                                                    AC1006 = R10;
                                                    AC1009 = R11 and R12;
                                                    AC1012 = R13；AC1014 = R14；
                                                    AC1015 = AutoCAD 2000；
                                                    AC1018 = AutoCAD 2004；
                                                    AC1021 = AutoCAD 2007;
                                                    AC1024 = AutoCAD 2010*/
#define HEADER_ANGBASE          "$ANGBASE"      /*0 角度方向*/
#define HEADER_ANGDIR           "$ANGDIR"       /*1 = 顺时针角度
                                                  0 = 逆时针角度*/
#define HEADER_ATTMODE          "$ATTMODE"      /*属性的可见性：
                                                    0 = 无
                                                    1 = 普通
                                                    2 = 全部*/
#define HEADER_AUNITS           "$AUNITS"       /*角度的单位格式*/
#define HEADER_AUPREC           "$AUPREC"       /*角度的单位精度*/
#define HEADER_CECOLOR          "$CECOLOR"      /*当前图元颜色号：
                                                    0 = BYBLOCK；
                                                    256 = BYLAYER*/
#define HEADER_CELTSCALE        "$CELTSCALE"    /*当前图元线型比例*/
#define HEADER_CELTYPE          "$CELTYPE"      /*图元线型名称、BYBLOCK或BYLAYER*/
#define HEADER_CELWEIGHT        "$CELWEIGHT"    /*新对象的线宽*/
#define HEADER_CEPSNID          "$CEPSNID"      /*新对象的打印样式句柄。如果 CEPSN-
                                                    TYPE 为 3，则此值代表该句柄*/
#define HEADER_CEPSNTYPE        "$CEPSNTYPE"    /*新对象的打印样式类型：
                                                    0 = “BYLAYER” 打印样式
                                                    1 = BYBLOCK打印样式
                                                    2 = 词典默认的打印样式
                                                    3 = 随对象 ID/句柄打印样式*/

#define HEADER_CHAMFERA         "$CHAMFERA"     /*第一个倒角距离*/
#define HEADER_CHAMFERB         "$CHAMFERB"     /*第二个倒角距离*/
#define HEADER_CHAMFERC         "$CHAMFERC"     /*倒角长度*/
#define HEADER_CHAMFERD         "$CHAMFERD"     /*倒角角度*/
#define HEADER_CLAYER           "$CLAYER"       /*当前图层名*/
#define HEADER_CMLJUST          "$CMLJUST"      /*当前多线对正：
                                                    0 = 顶端对正；
                                                    1 = 居中对正；
                                                    2 = 底端对正*/
#define HEADER_CMLSCALE         "$CMLSCALE"     /*当前多线比例*/
#define HEADER_CMLSTYLE         "$CMLSTYLE"     /*当前多线样式名称*/
#define HEADER_CSHADOW          "$CSHADOW"      /*三维对象的阴影模式：
                                                    0 = 投射和接收阴影
                                                    1 = 投射阴影
                                                    2 = 接收阴影
                                                    3 = 忽略阴影*/
#define HEADER_DIMADEC          "$DIMADEC"      /*角度标注中显示的精度位的位数*/
#define HEADER_DIMALT           "$DIMALT"       /*非零时执行的换算单位标注*/
#define HEADER_DIMALTD          "$DIMALTD"      /*换算单位小数位*/
#define HEADER_DIMALTF          "$DIMALTF"      /*换算单位比例因子*/
#define HEADER_DIMALTRND        "$DIMALTRND"    /*确定换算单位的舍入值*/
#define HEADER_DIMALTTD         "$DIMALTTD"     /*换算单位标注的公差值的小数位数*/
#define HEADER_DIMALTTZ         "$DIMALTTZ"     /*控制是否对换算公差值做消零处理：
                                                    0 = 消除零英尺和零英寸
                                                    1 = 包含零英尺和零英寸
                                                    2 = 包含零英尺，消除零英寸
                                                    3 = 包含零英寸，消除零英尺*/

#define HEADER_DIMALTU          "$DIMALTU"      /*所有标注样式族成员（角度标注除外）
                                                    的换算单位的单位格式：
                                                    1 = 科学；2 = 小数；3 = 工程；
                                                    4 = 建筑（堆叠）；5 = 分数（堆叠）；
                                                    6 = 建筑；7 = 分数*/
#define HEADER_DIMALTZ          "$DIMALTZ"      /*控制是否对换算单位标注值做消零处理：
                                                    0 = 消除零英尺和零英寸
                                                    1 = 包含零英尺和零英寸
                                                    2 = 包含零英尺，消除零英寸
                                                    3 = 包含零英寸，消除零英尺*/
#define HEADER_DIMAPOST         "$DIMAPOST"     /*换算标注后缀*/
#define HEADER_DIMASO           "$DIMASO"       /*1 = 创建关联标注
                                                  0 = 绘制单独图元*/

#define HEADER_DIMASSOC         "$DIMASSOC"     /*控制标注对象的关联性
                                                    0 = 创建分解标注；即标注的各元素
                                                        之间没有任何关联，直线、圆弧、
                                                        箭头及标注文字均作为单独的
                                                        对象进行绘制
                                                    1 = 创建无关联标注对象；将标注的
                                                        各种元素组成单一对象，如果
                                                        对象上的定义点移动，标注值将
                                                        被更新
                                                    2 = 创建关联标注对象；标注的各种
                                                        元素组成单一对象，并且标注的
                                                        一个或多个定义点与几何对象上
                                                        的关联点相联结*/
#define HEADER_DIMASZ           "$DIMASZ"       /*标注箭头尺寸*/
#define HEADER_DIMATFIT         "$DIMATFIT"     /*当尺寸界线的空间不足以同时放置标注
                                                    文字和箭头时用来控制两者的位置：
                                                    0 = 将文字和箭头均置于尺寸界线之外
                                                    1 = 先移动箭头，然后移动文字
                                                    2 = 先移动文字，然后移动箭头
                                                    3 = 移动文字和箭头中较合适的一个
                                                    如果 DIMTMOVE 设定为 1，
                                            AutoCAD 将为被移动的标注文字添加一条引线*/

#define HEADER_DIMAUNIT         "$DIMAUNIT"     /*角度标注的角度格式：
                                               0 = 十进制度数；1 = 度/分/秒；"
                                               2 = 百分度；3 = 弧度；4 = 勘测单位*/
#define HEADER_DIMAZIN          "$DIMAZIN"      /*控制是否对角度标注做消零处理：
                                                    0 = 显示所有前导零和后续零
                                                    1 = 消除十进制标注中的前导零
                                                    2 = 消除十进制标注中的后续零
                                                    3 = 消除前导零和后续零*/
#define HEADER_DIMBLK           "$DIMBLK"       /*箭头块名称*/
#define HEADER_DIMBLK1          "$DIMBLK1"      /*第一箭头块名称*/
#define HEADER_DIMBLK2          "$DIMBLK2"      /*第二箭头块名称*/
#define HEADER_DIMCEN           "$DIMCEN"       /*中心标记/中心线的大小*/
#define HEADER_DIMCLRD          "$DIMCLRD"      /*尺寸线颜色：
                                            范围是：0 = BYBLOCK；256 = BYLAYER*/
#define HEADER_DIMCLRE          "$DIMCLRE"      /*尺寸界线颜色：
                                            范围是：0 = BYBLOCK；256 = BYLAYER*/
#define HEADER_DIMCLRT          "$DIMCLRT"      /*标注文字的颜色：
                                            范围是：0 = BYBLOCK；256 = BYLAYER*/
#define HEADER_DIMDEC           "$DIMDEC"       /*主单位标注的公差值的小数位数*/
#define HEADER_DIMDLE           "$DIMDLE"       /*尺寸线超出尺寸界线的距离*/
#define HEADER_DIMDLI           "$DIMDLI"       /*尺寸线增量*/
#define HEADER_INSUNITS         "$INSUNITS"     /*AutoCAD 设计中心块的默认图形单位*/
#define HEADER_PUCSNAME         "$PUCSNAME"
#define HEADER_PUCSORG          "$PUCSORG"
#define HEADER_PUCSXDIR         "$PUCSXDIR"
#define HEADER_PUCSYDIR         "$PUCSYDIR"
#define HEADER_UCSNAME          "$UCSNAME"
#define HEADER_UCSORG           "$UCSORG"
#define HEADER_UCSXDIR          "$UCSXDIR"
#define HEADER_UCSYDIR          "$UCSYDIR"
#define HEADER_LTSCALE          "$LTSCALE"

/*角度方向*/
typedef enum {
    DXF_ANGDIR_COUNTERCLOCKWISE,    /*0:逆时针角度*/
    DXF_ANGDIR_CLOCKWISE,           /*1:顺时针角度*/
    DXF_ANGDIR_MAX
} DxfAngdir;

/*图形单位*/
typedef  enum {
    DXF_UNIT_NONE,      /*0 = 无单位*/
    DXF_UNIT_INCHES,    /*1 = 英寸*/
    DXF_UNIT_FEET,      /*2 = 英尺*/
    DXF_UNIT_MILES,     /*3 = 英里*/
    DXF_UNIT_MM,        /*4 = 毫米*/
    DXF_UNIT_CM,        /*5 = 厘米*/
    DXF_UNIT_M,         /*6 = 米*/
    DXF_UNIT_KM,        /*7 = 千米*/
    DXF_UNIT_MICROIN,   /*8 = 微英寸*/
    DXF_UNIT_MILS,      /*9 = 密耳*/
    DXF_UNIT_YARDS,     /*10 = 码*/
    DXF_UNIT_ANGSTROMS, /*11 = 埃*/
    DXF_UNIT_NM,        /*12 = 纳米*/
    DXF_UNIT_NS,        /*13 = 微米*/
    DXF_UNIT_DM,        /*14 = 分米*/
    DXF_UNIT_DAM,       /*15 = 十米*/
    DXF_UNIT_HM,        /*16 = 百米*/
    DXF_UNIT_GM,        /*17 = 百万公里*/
    DXF_UNIT_AU,        /*18 = 天文单位*/
    DXF_UNIT_LY,        /*19 = 光年*/
    DXF_UNIT_PS,        /*20 = 秒差距*/
} DxfUnits;

typedef enum {
    DXF_AXIS_X_RIGHT = 1,
    DXF_AXIS_X_LEFT  = -1,
    DXF_AXIS_Y_UPWARD= 1,
    DXF_AXIS_Y_DOWN  = -1
}DXF_AXIS_DIR;

typedef struct _DxfHeader DxfHeader;
struct _DxfHeader {
//    gint16 mainver;         /*维护版本号（应忽略）*/
    gchar *acadver;         /*AutoCAD图形数据库版本号*/
    gdouble angbase;
    /*page*/
    gchar *pucsname;        /*当前图纸空间 UCS 名称*/
    DxfPointData pucsorg;   /*当前图纸空间 UCS 原点*/
    DxfPointData pucsxdir;  /*当前图纸空间的 UCS X 轴*/
    DxfPointData pucsydir;  /*当前图纸空间的 UCS Y 轴*/

    gchar *ucsname;         /*当前 UCS 的名称*/
    DxfPointData ucsorg;    /*当前 UCS 的原点（在 WCS 中）*/
    DxfPointData ucsxdir;   /*当前 UCS X 轴的方向（在 WCS 中）*/
    DxfPointData ucsydir;   /*当前 UCS Y 轴的方向（在 WCS 中）*/

    DxfAngdir angdir;       /*角度方向*/
    DxfUnits insunits;      /*设计中心块的默认图形单位*/
    gdouble ltscale;        /*全局线型比例*/
};

extern void dxf_header_init();
extern void dxf_header_uninit();
extern DxfHeader *dxf_header_parse(Dxfile *f);
extern void dxf_header_delete(DxfHeader *h);
extern gchar *dxf_header_print(DxfHeader *h);

#endif /* __DXF_HEADER_H__ */
