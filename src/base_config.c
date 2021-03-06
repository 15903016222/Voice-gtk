/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base_config.h"
#include "base.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* 材料 (Material) 28Byte OK */
typedef struct _Material 
{
	unsigned int	Velocity_LW;	/* 声速 单位 0.01m/s 纵波 快点 */
	unsigned int	Velocity_SW;	/* 声速 单位 0.01m/s 横波 慢点 */
	char			Name[20];		/* 材料名字 */
} st_MATERIAL;
#if 0
static const st_MATERIAL data_en[] =
{
	{626000, 308000, "Aluminum"},			/* 0 */
	{592000, 323000, "Steel common"},		/* 1 */
	{566400, 310000, "Steel stainless"},			/* 2 */
	{439400, 200000, "Brass"},					/* 3 */
	{470000, 226000, "Copper"},				/* 4 */
	{589300, 323000, "Iron"},				/* 5 */
	{215900, 0, "Lead"},					/* 6 */
	{220000, 0, "Nylon"},					/* 7 */
	{360700, 160000, "Silver"},					/* 8 */
	{325100, 0, "Gold"},					/* 8 */
	{421600, 240000, "Zinc"},					/* 10 */
	{609600, 310000, "Titanium"},				/* 11 */
	{332700, 170000, "Tin"},						/* 12 */
	{254000, 110000, "Epoxy resin"},		/* 13 */
	{398800, 200000, "Ice"},						/* 14 */
	{563900, 300000, "Nickel"},					/* 15 */
	{273000, 146000, "Plexiglass"},			/* 16 */
	{235000, 115000, "Polystyrene"},		/* 17 */
	{584200, 0, "Porcelain"},				/* 18 */
	{238800, 0, "PVC"},						/* 19 */
	{563900, 0, "Quartz glass"},			/* 20 */
	{231100, 0, "Rubber vulcanized"},		/* 21 */
	{142200, 0, "Teflon"},					/* 22 */
	{148000, 0, "Water"}					/* 23 */
};
#endif
static const st_MATERIAL data_en[] =
{
	{630000, 310000, "Aluminum"},			/* 0 */
	{589000, 324000, "Steel common"},		/* 1 */
	{580000, 310000, "Steel stainless"},			/* 2 */
	{430000, 200000, "Brass"},					/* 3 */
	{470000, 230000, "Copper"},				/* 4 */
	{590000, 320000, "Iron"},				/* 5 */
	{220000, 220000, "Lead"},					/* 6 */
	{260000, 260000, "Nylon"},					/* 7 */
	{360000, 160000, "Silver"},					/* 8 */
	{320000, 320000, "Gold"},					/* 8 */
	{420000, 240000, "Zinc"},					/* 10 */
	{610000, 310000, "Titanium"},				/* 11 */
	{330000, 170000, "Tin"},						/* 12 */
	{254000, 110000, "Epoxy resin"},		/* 13 */
	{400000, 200000, "Ice"},						/* 14 */
	{560000, 300000, "Nickel"},					/* 15 */
	{270000, 146000, "Plexiglass"},			/* 16 */
	{240000, 115000, "Polystyrene"},		/* 17 */
	{584200, 584200, "Porcelain"},				/* 18 */
	{238800, 238800, "PVC"},						/* 19 */
	{563900, 563900, "Quartz glass"},			/* 20 */
	{231100, 231100, "Rubber vulcanized"},		/* 21 */
	{140000, 140000, "Teflon"},					/* 22 */
	{148000, 148000, "Water"}					/* 23 */
};
static const st_MATERIAL data_ch[] =
{
	{630000, 310000, "铝"},			/* 0 */
	{589000, 324000, "普通钢"},		/* 1 */
	{580000, 310000, "不锈钢"},			/* 2 */
	{430000, 200000, "黄铜"},					/* 3 */
	{470000, 230000, "铜"},				/* 4 */
	{590000, 320000, "铁"},				/* 5 */
	{220000, 220000, "铅"},					/* 6 */
	{260000, 260000, "尼龙"},					/* 7 */
	{360000, 160000, "银"},					/* 8 */
	{320000, 320000, "金"},					/* 8 */
	{420000, 240000, "锌"},					/* 10 */
	{610000, 310000, "钛"},				/* 11 */
	{330000, 170000, "锡"},						/* 12 */
	{254000, 110000, "环氧树脂"},		/* 13 */
	{400000, 200000, "冰"},						/* 14 */
	{560000, 300000, "镍"},					/* 15 */
	{270000, 146000, "树脂玻璃"},			/* 16 */
	{240000, 115000, "聚苯乙烯"},		/* 17 */
	{584200, 584200, "瓷器"},				/* 18 */
	{238800, 238800, "聚氯乙烯"},						/* 19 */
	{563900, 563900, "石英玻璃"},			/* 20 */
	{231100, 231100, "硫化橡胶"},		/* 21 */
	{140000, 140000, "聚四氟乙烯"},					/* 22 */
	{148000, 148000, "水"}					/* 23 */
};
#if 0
static const st_MATERIAL data_ch[] =
{
	{630000, 310000, "铝"},			/* 0 */
	{589000, 324000, "普通钢"},		/* 1 */
	{580000, 310000, "不锈钢"},			/* 2 */
	{430000, 200000, "黄铜"},					/* 3 */
	{470000, 230000, "铜"},				/* 4 */
	{590000, 320000, "铁"},				/* 5 */
	{220000, 0, "铅"},					/* 6 */
	{260000, 0, "尼龙"},					/* 7 */
	{360000, 160000, "银"},					/* 8 */
	{320000, 0, "金"},					/* 8 */
	{420000, 240000, "锌"},					/* 10 */
	{610000, 310000, "钛"},				/* 11 */
	{330000, 170000, "锡"},						/* 12 */
	{254000, 110000, "环氧树脂"},		/* 13 */
	{400000, 200000, "冰"},						/* 14 */
	{560000, 300000, "镍"},					/* 15 */
	{270000, 146000, "树脂玻璃"},			/* 16 */
	{240000, 115000, "聚苯乙烯"},		/* 17 */
	{584200, 0, "瓷器"},				/* 18 */
	{238800, 0, "聚氯乙烯"},						/* 19 */
	{563900, 0, "石英玻璃"},			/* 20 */
	{231100, 0, "硫化橡胶"},		/* 21 */
	{140000, 0, "聚四氟乙烯"},					/* 22 */
	{148000, 0, "水"}					/* 23 */
};
#endif
unsigned int get_damping_pos (CONFIG *p)
{
	int grp = get_current_group(p);
	if(GROUP_VAL_POS(grp , group_mode) == UT1_SCAN )
	{
		return p->damping_pos_ut1 ;
	}
	if(GROUP_VAL_POS(grp , group_mode) == UT2_SCAN )
	{
		return p->damping_pos_ut2 ;
	}
	return 0;
}
void set_damping_pos(CONFIG *p, unsigned int data)
{
	int grp = get_current_group(p);
	if(GROUP_VAL_POS(grp , group_mode) == UT1_SCAN )
	{
		p->damping_pos_ut1 = data;
	}
	if(GROUP_VAL_POS(grp , group_mode) == UT2_SCAN )
	{
		p->damping_pos_ut2 = data;
	}

}

static inline int get_bit_value (int val, int bit)
{
	assert (bit < 32);
	assert (bit >= 0);
	return ((val & (0x01 << bit)) != 0);
}

static inline int set_bit_value (int val, int bit, int val1)
{
	assert (bit < 32);
	assert (bit >= 0);
	return ((val & ~(0x1 << bit)) | (val1 << bit));

}

char* get_material_name (int nPos_)
{
	if(get_language (pp->p_config)==0)
		return (char *)(data_en[nPos_].Name);
	else
		return (char *)(data_ch[nPos_].Name);
}

unsigned int get_material_lw (int nPos_)
{
	return data_en[nPos_].Velocity_LW;
}

unsigned int get_material_sw (int nPos_)
{
	return data_en[nPos_].Velocity_SW;
}

unsigned char get_part_geometry (int grp)
{
	return GROUP_VAL_POS(grp , part.Geometry);
}

void set_part_geometry (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.Geometry) = (unsigned char)((unsigned int)(data));
}

unsigned char get_part_material (int grp)
{
	return GROUP_VAL_POS(grp , part.Material_pos) ;
}

void set_part_material (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.Material_pos) = (unsigned char)((unsigned int) (data));
}

unsigned int get_part_thickness (int grp)
{
	return GROUP_VAL_POS(grp , part.Thickness);
}

void set_part_thickness (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.Thickness) = (unsigned int)((unsigned int) (data));
}

unsigned int get_part_diameter (int grp)
{
	return GROUP_VAL_POS(grp , part.Diameter);
}

void set_part_diameter (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.Diameter) = (unsigned int)((unsigned int) (data));
}

unsigned char get_part_weld (int grp)
{
	return GROUP_VAL_POS(grp , part.Weld);
}

void set_part_weld (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.Weld) = (unsigned int)((unsigned int) (data));
}
unsigned char get_part_symmetry (int grp)
{
	return GROUP_VAL_POS(grp , part.symmetry);
}

void set_part_symmetry (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.symmetry) = (unsigned int)((unsigned int) (data));
}

unsigned int get_weland_height (int grp)
{
	return GROUP_VAL_POS(grp , part.weland_height);
}

void set_weland_height (int grp , void* data)
{
	GROUP_VAL_POS(grp , part.weland_height) = (unsigned int)((unsigned int) (data));
}

unsigned int get_weland_offset (int grp)
{
	return GROUP_VAL_POS(grp , part.weland_offset);
}

void set_weland_offset (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.weland_offset) = (unsigned int)((unsigned int) (data));
}

unsigned int get_fizone_height (int grp)
{
	return GROUP_VAL_POS(grp , part.fizone_height);
}

void set_fizone_height (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.fizone_height) = (unsigned int)((unsigned int) (data));
}

unsigned int get_fizone_angle (int grp)
{
	return GROUP_VAL_POS(grp , part.fizone_angle);
}

void set_fizone_angle (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.fizone_angle) = (unsigned int)((unsigned int) (data));
}

unsigned int get_fizone_radius (int grp)
{
	return GROUP_VAL_POS(grp , part.fizone_radius);
}

void set_fizone_radius (int grp, void* data)
{
	GROUP_VAL_POS(grp , part.fizone_radius) = (unsigned int)((unsigned int) (data));
}

GROUP *get_group_by_id (CONFIG *p, int group)
{
	return &(p->group[group]);
}

/* 配置信息的操作 */
unsigned char get_current_group (CONFIG *p)	/* 当前p活动的group 0-7 */
{
	return	p->groupId;
}


unsigned char get_group_qty (CONFIG *p)	/* 取得p当前group数量 */
{
	return	p->groupQty;
}

void set_group_qty (CONFIG *p, unsigned char data)	/* 设置p当前group数量 */
{
	assert (data < 9);
	p->groupQty = data;
}

unsigned char get_voltage (CONFIG *p, unsigned int group_num)	/* 取得p发射电压,发射与group的模式相关 */
{
	switch ((p->group[group_num].group_mode))
	{

		case UT1_SCAN:
		case UT2_SCAN:
			return p->voltage_ut;
			break;
		case UT_SCAN:
		case PA_SCAN:
			return p->voltage_pa;
			break;
		default:
			g_debug ("Voltage get value error\n");
			break;
	}
	return 0 ;
}

void set_voltage (CONFIG *p, unsigned int group_num, unsigned char data)	/* 取得p发射电压,发射与group的模式相关 */
{
	assert (data < 3);
	switch ((p->group[group_num].group_mode))
	{
		case UT1_SCAN:
		case UT2_SCAN: p->voltage_ut  = data;	break;
		case UT_SCAN:
		case PA_SCAN:  p->voltage_pa  = data;	break;
		default:g_debug ("Voltage set value error\n");	break;
	}
}

unsigned char get_language (CONFIG *p)		/* 获取语言 */
{
	return p->language;
}

void set_language (CONFIG *p, unsigned char data)	/* 设置语言 */
{
	assert (data < 2);			/* 支持语言的总数量 */
	p->language = data;
}

unsigned char get_reject (CONFIG *p)		/* 获取抑制 */
{
	return p->reject;
}

void set_reject (CONFIG *p, unsigned char data)	/* 设置抑制 */
{
	assert (data < 101);			/* 支持语言的总数量 */
	p->reject = data;
}

unsigned char get_display_pos (CONFIG *p)		/* 获取当前显示模式 ABSC... */
{
	return p->display_pos;
}

void set_display_pos (CONFIG *p, unsigned char data)	/* 设置当前显示模式 ABSC */
{
	assert (data < 11);
	p->display_pos = data;
}

unsigned char get_display_group (CONFIG *p)		/* 获取显示是全部还是当前 返回1是当前 0是全部 */
{
	return p->display_group ;
}

void set_display_group (CONFIG *p, unsigned char data)	/* 设置显示是当前还是全部 */
{
	p->display_group = data;
}

unsigned char	get_cscan_source (CONFIG *p, unsigned int pos)	/* 获取c scan source */
{
	assert (pos < 2);
	switch (get_display_pos(p))
	{
		case C_SCAN:
		case A_B_C_SCAN:
        case PA_TOFD_CSCAN:
			return (p->c_scan_pos & 0x03); break;			/* c_scan1 第0~1位 0 1 2 3 */
		case A_C_CC_SCAN:
			if (pos == 0)
				return (p->c_scan_pos & 0x03);				/* c_scan1 第0~1位 0 1 2 3 */
			else if (pos == 1)
				return (p->c_scan_pos & 0x1c) >> 2;			/* c_scan2 第2-4位 0 1 2 3 4 */
			break;
		case A_S_CC_SCAN:
			return (p->c_scan_pos & 0xe0) >> 5; break;		/* c_scan3 5-7位  0 1 2 3 4*/
		case A_SCAN: 
		case B_SCAN:
		case S_SCAN:
		case A_B_SCAN:
		case A_B_S_SCAN:
		case PA_TOFD:
		     return 4 ;
		     break ;
		//case Strip_Chart_AA: return 4;break;
		default:break;
	}
	return 4;
}

void set_cscan_source (CONFIG *p, unsigned char data, unsigned int pos)	/* 设置c scan source */
{
	assert (pos < 2);
	assert (data < 5);
	switch (get_display_pos(p))
	{
		case C_SCAN:
		case A_B_C_SCAN:
		case PA_TOFD_CSCAN:
			p->c_scan_pos = (p->c_scan_pos & ~0x03) | data; break;			/* c_scan1 第0~1位 0 1 2 3 */
		case A_C_CC_SCAN:
			if (pos == 0)
				p->c_scan_pos = (p->c_scan_pos & ~0x03) | data;		/* c_scan1 第0~1位 0 1 2 3 */
			else if (pos == 1)
				p->c_scan_pos = (p->c_scan_pos & ~0x1c) | (data << 2);		/* c_scan2 第2-4位 0 1 2 3 4 */
			break;
		case A_S_CC_SCAN:
			p->c_scan_pos = (p->c_scan_pos & ~0xe0) | (data << 5); break;	/* c_scan3 5-7位  0 1 2 3 4*/
		case A_SCAN: 
		case B_SCAN:
		case S_SCAN:
		case A_B_SCAN:
		case A_B_S_SCAN:
		case PA_TOFD:
		      return ;
		      break  ;
		//case Strip_Chart_AA: return ;break;
		default:break;
	}
}

unsigned char get_stripscan_data1 (CONFIG *p)
{
	return p->data1;
}

unsigned char get_stripscan_data2 (CONFIG *p)
{
	return p->data2;
}

unsigned char get_stripscan_mode (CONFIG *p)
{
	return p->dis_mode;
}

unsigned int get_stripscan_disrange (CONFIG *p)
{
	return p->dis_range;
}

void set_stripscan_data1 (CONFIG *p, unsigned char data)
{
	assert (data < 3);
	p->data1 = data;
}

void set_stripscan_data2 (CONFIG *p, unsigned char data)
{
	assert (data < 4);
	p->data2 = data;
}

void set_stripscan_mode (CONFIG *p, unsigned char data)
{
	assert (data < 4);
	p->dis_mode = data;
}

void set_stripscan_disrange (CONFIG *p, unsigned int data)
{
	assert (data > 0 );
	p->dis_range = data;
}

unsigned char get_alarm_pos (CONFIG *p)				/* 16中不同的报警信息 */
{
	return p->alarm_pos;
}

void set_alarm_pos (CONFIG *p, unsigned char data)
{
	assert (data < 16);
	p->alarm_pos = data;
}

/* alarm_info 分布 3 4 1 3 4 groupa conditiona operator groupb conditionb */
unsigned char get_alarm_groupa (CONFIG *p)
{
	return p->alarm_info[get_alarm_pos(p)] & 0x7;
}

void set_alarm_groupa (CONFIG *p, unsigned char data)
{
	assert (data < 8);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x07) | data; 		
}

unsigned char get_alarm_conditiona (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x78) >> 3;
}

void set_alarm_conditiona (CONFIG *p, unsigned char data)
{
	assert (data < 9);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x78) | (data << 3); 		
}

unsigned char get_alarm_operator (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x80) >> 7;
}

void set_alarm_operator (CONFIG *p, unsigned char data)
{
	assert (data < 2);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x80) | (data << 7); 
}

unsigned char get_alarm_groupb (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x700) >> 8;
}

void set_alarm_groupb (CONFIG *p, unsigned char data)
{
	assert (data < 8);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x700) | (data << 8); 
}

unsigned char get_alarm_conditionb (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x7800) >> 11;
}

void set_alarm_conditionb (CONFIG *p, unsigned char data)
{
	assert (data < 9);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x7800) | (data << 11);
}

/* OUPUT */
unsigned char	get_output_pos (CONFIG *p)
{
	return p->output_pos;
}

void set_output_pos (CONFIG *p, unsigned char data)
{
	assert (data < 5);
	p->output_pos = data;
}

unsigned short	get_output_alarm (CONFIG *p)
{
	return p->output1[get_output_pos(p)].alarm_info;
}

unsigned short	get_1output_alarm_pos (CONFIG *p)
{
	int i;
	assert (get_output_alarm_qty (p) == 1);
	for (i = 0; i < 16; i++)
	{
		if (get_output_alarm_pos (p, i + 1))
		if (get_output_alarm_pos (p, i + 1))
			return (i + 1);
	}
	return 0;
}

unsigned char get_output_alarm_qty (CONFIG *p)
{
	int qty = 0, i;
	for (i = 0; i < 16; i++)
	{
		if (get_output_alarm_pos (p, i + 1))
			qty += 1;
	}
	return (unsigned char)(qty);
}

unsigned char get_output_alarm_pos (CONFIG *p, unsigned int pos)
{
	assert (pos < 17);
	return (p->output1[get_output_pos(p)].alarm_info >> (pos - 1)) & 0x01;
}

void set_output_alarm (CONFIG *p, unsigned char data, unsigned int pos)
{
	assert (data < 2);
	assert (data < 17);
	p->output1[get_output_pos(p)].alarm_info = 
		(p->output1[get_output_pos(p)].alarm_info & ~(0x01 << (pos - 1))) | (data << (pos -1));
}

unsigned char get_output_count (CONFIG *p)
{
	return p->output1[get_output_pos(p)].count;
}

unsigned char get_output_sound (CONFIG *p)
{
	return p->output1[get_output_pos(p)].sound;
}

void set_output_count (CONFIG *p, unsigned char data)
{
	assert (data < 101);
	p->output1[get_output_pos(p)].count = data;
}

void set_output_sound (CONFIG *p, unsigned char data)
{
	assert (data < 5);
	p->output1[get_output_pos(p)].sound = data;
}

unsigned int get_output_delay (CONFIG *p)
{
	return p->output1[get_output_pos(p)].delay;
}

unsigned int get_output_holdtime (CONFIG *p)
{
	return p->output1[get_output_pos(p)].holdtime;
}

void set_output_delay (CONFIG *p, unsigned int data)
{
	p->output1[get_output_pos(p)].delay = data;
}

void set_output_holdtime (CONFIG *p, unsigned int data)
{
	p->output1[get_output_pos(p)].holdtime = data;
}

unsigned char get_output_group (CONFIG *p)
{
	return (p->analog1[get_output_pos(p) - 3] & 0x7);
}

unsigned char get_output_data (CONFIG *p)
{
	return ((p->analog1[get_output_pos(p) - 3] & 0x18) >> 3);
}

void set_output_group (CONFIG *p, unsigned char data)
{
	assert (data < 8);
	p->analog1[get_output_pos(p) - 3] = 
		(p->analog1[get_output_pos(p) - 3] & ~(0x07)) | data;
}

void set_output_data (CONFIG *p, unsigned char data)
{
	assert (data < 4);
	p->analog1[get_output_pos(p) - 3] = 
		(p->analog1[get_output_pos(p) - 3] & ~(0x18)) | (data << 3);
}

/* 设置获取亮度 */
unsigned char get_bright (CONFIG *p)
{
	return	p->bright;
}

void set_bright (CONFIG *p, unsigned char data)
{

	if(data>100)     p->bright = 100 ;
	else if(data<1)  p->bright = 1   ;
	else             p->bright = data;
#if ARM
    write(pp->fd_key, &(p->bright),1);
#endif
}

/* Reading list field 设置当前读取的数值 */
unsigned char get_reading_list (CONFIG *p)
{
	return p->list;
}

void set_reading_list (CONFIG *p, unsigned char data)
{
	assert (data < 2);
	p->list = data;
}

unsigned char get_reading_field1 (CONFIG *p)
{
	return p->field[0];
}

void set_reading_field1 (CONFIG *p, unsigned char data)
{
	assert (data < 128);
	p->field[0] = data;
}

unsigned char get_reading_field2 (CONFIG *p)
{
	return p->field[1];
}

void set_reading_field2 (CONFIG *p, unsigned char data)
{
	assert (data < 128);
	p->field[1] = data;
}

unsigned char get_reading_field3 (CONFIG *p)
{
	return p->field[2];
}

void set_reading_field3 (CONFIG *p, unsigned char data)
{
	assert (data < 128);
	p->field[2] = data;
}

unsigned char get_reading_field4 (CONFIG *p)
{
	return p->field[3];
}

void set_reading_field4 (CONFIG *p, unsigned char data)
{
	assert (data < 128);
	p->field[3] = data;
}

/* 单位 */
unsigned char get_unit (CONFIG *p)
{
	return p->unit;
}

void set_unit (CONFIG *p, unsigned char data)
{
	assert (data < 2);
	p->unit = data;
}

unsigned char get_display_table (CONFIG *p)
{
	return get_bit_value (p->on_off_status1, 0);		
}

void set_display_table (CONFIG *p, unsigned char data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status1 = set_bit_value (p->on_off_status1, 0, data);
}

unsigned char get_entry_image (CONFIG *p)
{
	return get_bit_value (p->on_off_status1, 1);		
}

void set_entry_image (CONFIG *p, unsigned char data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status1 = set_bit_value (p->on_off_status1, 1, data);
}

unsigned int get_entry_qty (CONFIG *p)
{
	return p->entry_qty;
}

void set_entry_qty (CONFIG *p, unsigned int data)
{
	p->entry_qty = data;
}

unsigned int get_cur_entry (CONFIG *p)
{
	return p->cur_entry;
}

void set_cur_entry (CONFIG *p, unsigned int data)
{
	p->cur_entry = data;
}

unsigned char get_echo_qty (CONFIG *p)
{
	return p->echo_qty;
}

void set_echo_qty (CONFIG *p, unsigned char data)
{
	assert (data < 11);
	assert (data != 0);
	p->echo_qty = data;
}

/* Display Overlay 菜单数值的保存 */
unsigned char get_overlay_grid (CONFIG *p)
{
	return p->grid;
}

void set_overlay_grid (CONFIG *p, unsigned char data)
{
	assert (data < 6);
	p->grid = data;
}

/* 0位是overlay sizing curves */
int get_overlay_sizing_curves (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 0);		
}

void set_overlay_sizing_curves (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 0, data);
}

/* 1位是overlay gate */
int get_overlay_gate (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 1);		
}

void set_overlay_gate (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 1, data);
    if (data == 1) {
        TMP(gateShow)[0] = (unsigned char)0xff;
        TMP(gateShow)[1] = (unsigned char)0xff;
        TMP(gateShow)[2] = (unsigned char)0xff;
        TMP(gateShow)[3] = (unsigned char)0xff;
    } else {
        TMP(gateShow)[0] = (unsigned char)0x00;
        TMP(gateShow)[1] = (unsigned char)0x00;
        TMP(gateShow)[2] = (unsigned char)0x00;
        TMP(gateShow)[3] = (unsigned char)0x00;
    }
}

/* 2位是overlay cursor */
int get_overlay_cursor (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 2);		
}

void set_overlay_cursor (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 2, data);
}

/* 3位是overlay overlay */
int get_overlay_overlay (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 3);		
}

void set_overlay_overlay (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 3, data);
}

/* Display Properties 操作 */
unsigned char get_dis_prop_scan (CONFIG *p)
{
	return p->dis_prop_scan;
}

void set_dis_prop_scan (CONFIG *p, unsigned char data)
{
	assert (data < 6);
	p->dis_prop_scan = data;
}

unsigned int get_b_compress (CONFIG *p)
{
	return p->bcompress;
}

void set_b_compress (CONFIG *p, unsigned int data)
{
	p->bcompress = data;
}

/* 4位是bscan optimum */
int get_dis_prop_boptimum (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 4);		
}

void set_dis_prop_boptimum (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 4, data);
}

/* 5位是cscan ratio */
int get_dis_prop_cratio (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 5);		
}

void set_dis_prop_cratio (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 5, data);
}

/* 6位是sscan interpolation */
int get_dis_prop_sinterpolation (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 6);		
}

void set_dis_prop_sinterpolation (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 6, data);
}

/* 7位是strip chart orientation */
int get_dis_prop_strip_orientation (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 7);		
}

void set_dis_prop_strip_orientation (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 7, data);
}

unsigned char get_fft_color (CONFIG *p)
{
	return p->fft_color;
}

void set_fft_color (CONFIG *p, unsigned char data)
{
	assert (data < 4);
	p->fft_color = data;
}

unsigned int get_weld_display()
{
	return pp->p_config->weld_display_on_off ;
}

void set_weld_display(unsigned int data)
{
	pp->p_config->weld_display_on_off = data;
}

/* 8位自动检测探头 */
int get_auto_detect (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 8);		
}

void set_auto_detect (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 8, data);
}

/* 9位选择探头或者是自定义探头 */
int get_probe_select (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 9);		
}

void set_probe_select (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 9, data);
}

/* 10位选择探头FFT开关 */
int get_probe_fft (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 10);		
}

void set_probe_fft (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 10, data);
}

/* 11位选择聚焦法则自动计算开关 */
int get_auto_focal (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 11);		
}

void set_auto_focal (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 11, data);
}

/* 选择当前encoder 1或者2 */
int get_cur_encoder (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 12);		
}

void set_cur_encoder (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 12, data);
}

/* 设置encoder属性 */
unsigned char get_enc_polarity (CONFIG *p, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	return p->encoder1[enc_id].Polarity;
}

void set_enc_polarity (CONFIG *p, unsigned char data, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	assert ((data == 1) || (data == 0));
	p->encoder1[enc_id].Polarity = data;
}

unsigned char get_enc_type (CONFIG *p, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	return p->encoder1[enc_id].Type;
}

void set_enc_type (CONFIG *p, unsigned char data, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	assert (data < 16);
	p->encoder1[enc_id].Type = data;
}

unsigned int get_enc_resolution (CONFIG *p, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	return p->encoder1[enc_id].Resolution;
}

void set_enc_resolution (CONFIG *p, unsigned int data, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	p->encoder1[enc_id].Resolution = data;
}

unsigned int get_enc_origin (CONFIG *p, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	return p->encoder1[enc_id].Origin;
}

void set_enc_origin (CONFIG *p, unsigned int data, int enc_id)
{
	assert ((enc_id == 1) || (enc_id == 0));
	p->encoder1[enc_id].Origin = data;
}

inline gboolean enc_get_preset(CONFIG *p, gint enc_id)
{
    g_return_val_if_fail(("enc preset", (enc_id == 1 || enc_id == 0)), false);
    return p->encoder1[enc_id].Preset;
}

inline void enc_set_preset(CONFIG *p, gint enc_id, gboolean val)
{
    g_return_if_fail(("enc preset", (enc_id == 1 || enc_id == 0)));
    p->encoder1[enc_id].Preset = val;
}

/* 设置scan属性 */
unsigned char get_inspec_type (CONFIG* p)
{
	return 0 ; //p->inspection_type;
}

void set_inspec_type (CONFIG* p, unsigned char data)
{
	//assert (data < 3);
	p->inspection_type = 0 ; //data;
}

unsigned char get_inspec_source (CONFIG *p)
{
    if(p->inspection_scan) p->inspection_scan = 1 ;
	else p->inspection_scan = 0 ;
	return p->inspection_scan;
}

void set_inspec_source (CONFIG *p, unsigned char data)
{
	if(data) p->inspection_scan = 1 ;
	else p->inspection_scan = 0 ;
}

unsigned char get_inspec_index (CONFIG *p)
{
	return p->inspection_index;
}

void set_inspec_index (CONFIG *p, unsigned char data)
{
	assert (data < 5);
	p->inspection_index = data;
}

unsigned int get_inspec_speed (CONFIG *p) 
{
	return p->inspection_scanspeed;
}

void set_inspec_speed (CONFIG *p, unsigned int data)
{
	p->inspection_scanspeed = data;
}

unsigned int get_inspec_rpmspeed (CONFIG *p)
{
	return p->inspection_rpmscanspeed;
}

void set_inspec_rpmspeed (CONFIG *p, unsigned int data)
{
	p->inspection_rpmscanspeed = data;
}

unsigned int get_inspec_indexspeed (CONFIG *p)
{
	return p->inspection_indexspeed;
}

void set_inspec_indexspeed (CONFIG *p, unsigned int data)
{
	p->inspection_indexspeed = data;
}

/* 设置 scan area 属性 */
int get_area_scanstart (CONFIG *p)
{
	return p->inspection_scan_start;
}

void set_area_scanstart (CONFIG *p, int data)
{
	p->inspection_scan_start = data;
}

int get_area_scanend (CONFIG *p)
{
	return p->inspection_scan_end;
}

void set_area_scanend (CONFIG *p, int data)
{
	p->inspection_scan_end = data;
}

int get_area_scanresolution (CONFIG *p)
{
	return p->inspection_scan_resolution;
}

void set_area_scanresolution (CONFIG *p, int data)
{
	p->inspection_scan_resolution = data;
}

int get_area_indexstart (CONFIG *p)
{
	return p->inspection_index_start;
}

void set_area_indexstart (CONFIG *p, int data)
{
	p->inspection_index_start = data;
}

int get_area_indexend (CONFIG *p)
{
	return p->inspection_index_end;
}

void set_area_indexend (CONFIG *p, int data)
{
	p->inspection_index_end = data;
}

int get_area_indexresolution (CONFIG *p)
{
	return p->inspection_index_resolution;
}

void set_area_indexresolution (CONFIG *p, int data)
{
	p->inspection_index_resolution = data;
}

/* 设置 scan start */
unsigned char get_start_mode (CONFIG *p)
{
	return (get_bit_value (p->on_off_status, 14) | 
		(get_bit_value (p->on_off_status, 15) << 1));		
}

void set_start_mode (CONFIG *p, unsigned char data)
{
	assert ((data == 2) ||(data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 14, (data & 0x01));
	p->on_off_status = set_bit_value (p->on_off_status, 15, (data & 0x02) >> 1);
}

int get_start_pause (CONFIG *p)
{
	return TMP(freeze);
	//return get_bit_value (p->on_off_status, 16);
}

void set_start_pause (CONFIG *p, int data)
{
	TMP(freeze) = data ;
	//assert ((data == 1) || (data == 0));
	//p->on_off_status = set_bit_value (p->on_off_status, 16, data);
}

/* 设置 scan Data */
unsigned char get_data_storage (CONFIG *p)
{
	return (get_bit_value (p->on_off_status, 17) | 
		(get_bit_value (p->on_off_status, 18) << 1));		
}

void set_data_storage (CONFIG *p, unsigned char data)
{
	assert ((data == 3) || (data == 2) ||(data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 17, (data & 0x01));
	p->on_off_status = set_bit_value (p->on_off_status, 18, (data & 0x02) >> 1);
}

int get_data_inspec_data (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 19);
}

void set_data_inspec_data (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 19, data);
}

/* 设置 file->file */
unsigned char get_file_storage (CONFIG *p)
{
	return TMP(nDataSavePlace) ;
	//return (get_bit_value (p->on_off_status, 20) |
	//	(get_bit_value (p->on_off_status, 21) << 1));
}

void set_file_storage (CONFIG *p, unsigned char data)
{
	assert ((data == 3) || (data == 2) ||(data == 1) || (data == 0));
	TMP(nDataSavePlace) = data;
	//p->on_off_status = set_bit_value (p->on_off_status, 20, (data & 0x01));
	//p->on_off_status = set_bit_value (p->on_off_status, 21, (data & 0x02) >> 1);
}

unsigned int get_file_save_mode (CONFIG *p)
{
	//return (get_bit_value (p->on_off_status, 22) |
	//	(get_bit_value (p->on_off_status, 23) << 1));
	return p->data_save_model ;
}

void set_file_save_mode (CONFIG *p, unsigned char data)
{
	assert ((data == 3) || (data == 2) ||(data == 1) || (data == 0));
	p->data_save_model = data ;
	//p->on_off_status = set_bit_value (p->on_off_status, 22, (data & 0x01));
	//p->on_off_status = set_bit_value (p->on_off_status, 23, (data & 0x02) >> 1);
}

/* 设置 file->report */
int get_report_template (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 24);
}

void set_report_template (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 24, data);
}

unsigned char get_report_paper_size (CONFIG *p)
{
	return p->report_paper_size;
}

void set_report_paper_size (CONFIG *p, unsigned char data)
{
	assert (data < 4);
	p->report_paper_size = data;;
}

/* 设置 file->format */
int get_report_format_userfield (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 25);
}

void set_report_format_userfield (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 25, data);
}

int get_report_format_probe (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 26);
}

void set_report_format_probe (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 26, data);
}

int get_report_format_setup (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 27);
}

void set_report_format_setup (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 27, data);
}

int get_report_format_note (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 28);
}

void set_report_format_note (CONFIG *p, int data)
{
	assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 28, data);
}

unsigned char get_report_format_view (CONFIG *p)
{
	return (get_bit_value (p->on_off_status, 29) | 
		(get_bit_value (p->on_off_status, 30) << 1));		
}

void set_report_format_view (CONFIG *p, unsigned char data)
{
	assert ((data == 2) ||(data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 29, (data & 0x01));
	p->on_off_status = set_bit_value (p->on_off_status, 30, (data & 0x02) >> 1);
}

/* 设置 file->userfield */
unsigned char get_report_userfield_select (CONFIG *p)
{
	return p->userfield_select;
}

void set_report_userfield_select (CONFIG *p, unsigned char data)
{
	assert (data < 10);
	p->userfield_select = data; 
}

int get_report_userfield_enable (CONFIG *p, int select_pos)
{
	assert (select_pos < 10);
	return ((p->userfield_select_enable & (0x01 << select_pos)) != 0);
}

void set_report_userfield_enable (CONFIG *p, int data, int select_pos)
{
	assert (select_pos < 10);
	assert ((data == 1) || (data ==0));
	p->userfield_select_enable = 
		((p->userfield_select_enable & ~(0x1 << select_pos)) | (data << select_pos));
}

const char *get_report_userfield_label (CONFIG *p, int select_pos)
{
	assert (select_pos < 10);
	return p->user_label[select_pos];
}

void set_report_userfield_label (CONFIG *p, const char *label, int select_pos)
{
	assert (select_pos < 10);
	assert (label);
	strcpy (p->user_label[select_pos], label);
}

const char *get_report_userfield_content (CONFIG *p, int select_pos)
{
	assert (select_pos < 10);
	return p->user_label_content[select_pos];
}

void set_report_userfield_content (CONFIG *p, const char *content, int select_pos)
{
	assert (select_pos < 10);
	assert (content);
	strcpy (p->user_label_content[select_pos], content);
}

/* 保存配置信息 */
const char *get_edit_notes_info (CONFIG *p)
{
	return p->edit_notes_info;
}

void set_edit_notes_info (CONFIG *p, const char *content)
{
	assert (content);
	strcpy (p->edit_notes_info, content);
}

const char *get_header_info (CONFIG *p)
{
	return p->edit_header_info;
}

void set_header_info (CONFIG *p, const char *content)
{
	assert (content);
	strcpy (p->edit_header_info, content);
}

const char *get_file_name_info (CONFIG *p)
{
	return p->file_name_info;
}

void set_file_name_info (CONFIG *p, const char *content)
{
	assert (content);
	strcpy (p->file_name_info, content);
}

char* get_file_name_all (CONFIG *p)
{
	switch (get_file_save_mode (p))
	{
		case SAVE_MODE_INSPEC_DATA:
			return p->file_name_inspection_data;break;
		case SAVE_MODE_INSPEC_TABLE:
			return p->file_name_inspection_table;break;
		case SAVE_MODE_SCREEN:
			return p->file_name_screen;break;
		case SAVE_MODE_REPORT:
			return p->file_name_report;break;
		default:break;
	}
	return NULL;
}

void set_file_name_all (CONFIG *p, const char *content)
{
	assert (content);
	switch (get_file_save_mode (p))
	{
		case SAVE_MODE_INSPEC_DATA:
			strcpy (p->file_name_inspection_data, content);break;
		case SAVE_MODE_INSPEC_TABLE:
			strcpy (p->file_name_inspection_table, content);break;
		case SAVE_MODE_SCREEN:
			strcpy (p->file_name_screen, content);break;
		case SAVE_MODE_REPORT:
			strcpy (p->file_name_report, content);break;
		default:break;
	}
}


/* gll probe_type */
char get_probe_type (CONFIG *p)
{
	return p->probe_type;
}

void set_probe_type (CONFIG *p, char data)
{
	p->probe_type = data;
}

/**
 * Group 参数操作接口
 */

gshort group_get_gain(gint grp)
{
    return GROUP_VAL_POS(grp, gain);
}
void group_set_gain(gint grp, gshort val)
{
    GROUP_VAL_POS(grp, gain) = val;
#if ARM
    group_spi_set_gain(grp, (val)/10);
#endif
}

gshort group_get_refgain(gint grp)
{
    return GROUP_VAL_POS(grp, refgain);
}
void group_set_refgain(gint grp, gshort val)
{
    GROUP_VAL_POS(grp, refgain) = val;
}

gint group_get_start(gint grp)
{
    return GROUP_VAL_POS(grp, start);
}
void group_set_start(gint grp, gint val)
{
    GROUP_VAL_POS(grp, start) = val;
}

gint group_get_mode(gint grp)
{
    return GROUP_VAL_POS(grp, group_mode);
}

gint group_get_rx_tx_mode(gint grp)
{
    return GROUP_VAL_POS(grp, rx_tx_mode);
}
void group_set_rx_tx_mode(gint grp, gint val)
{
    GROUP_VAL_POS(grp, rx_tx_mode) = val;
}

gint group_get_range(gint grp)
{
    return GROUP_VAL_POS(grp, range);
}
void group_set_range(gint grp, gint val)
{
    GROUP_VAL_POS(grp, range) = val;
}

gint group_get_pw(gint grp)
{
    return GROUP_VAL_POS(grp, pulser_width1);
}
void group_set_pw(gint grp, gint val)
{
    GROUP_VAL_POS(grp, pulser_width1) = val;
}

/* GROUP 参数的保存读取*/
/**
 */
int get_group_val (GROUP *p, int type)
{
	int tt = 0;
	switch (type)
	{
//		case GROUP_START:tt = p->start;break;
//		case GROUP_RANGE:tt = p->range;break;
		case GROUP_WEDGE_DELAY:tt = p->wedge_delay;break;
		case GROUP_VELOCITY:tt = p->velocity;break;
		case GROUP_PULSER:tt = p->pulser1;break;
		case GROUP_RECEIVER:tt = p->receiver1;break;
		case GROUP_FILTER:tt = p->filter1;break;
		case GROUP_DB_REF:tt = get_bit_value (p->on_off_status, 0);break;
		case GROUP_FILTER_POS:tt = p->filter_pos1;break;
		case GROUP_RECTIFIER:tt = p->rectifier1;break;
		case GROUP_AVERAGING: tt = p->averaging1;break;	/*0:1 1:2 2:4 3:8 4:16*/
		case GROUP_VIDEO_FILTER: tt = get_bit_value (p->on_off_status, 1);break;
		case GROUP_FREQ_VAL:tt = p->frequency1;break;
		case GROUP_FREQ_POS:tt = p->freq_pos1;break;
		case GROUP_PW_POS:tt = p->pw_pos1;break;
		case GROUP_PW_VAL:tt = p->pulser_width1;break;
		case GROUP_PRF_POS:tt = p->prf_pos1;break;
		case GROUP_PRF_VAL:tt = p->prf1;break;
//		case GROUP_PQTY_POS:tt = p->point_qty_pos1;break;
//		case GROUP_PQTY_VAL:tt = p->point_qty1;break;
		default:break;
	}
	return tt;
}

void set_group_val (GROUP *p, int type, int val)
{
	switch (type)
	{
//		case GROUP_START:p->start = val;break;
//		case GROUP_RANGE:p->range = val;break;
		case GROUP_WEDGE_DELAY:p->wedge_delay = val;break;
		case GROUP_VELOCITY:p->velocity = val;break;
		case GROUP_PULSER:p->pulser1 = (unsigned char)(val);break;
		case GROUP_RECEIVER:p->receiver1 = (unsigned char)(val);break;
		case GROUP_FILTER:p->filter1=(unsigned char)(val);break;
		case GROUP_DB_REF: p->on_off_status = set_bit_value (p->on_off_status, 0, val);break;
		case GROUP_FILTER_POS:p->filter_pos1 = (unsigned char)(val);break;
		case GROUP_RECTIFIER: p->rectifier1 = (unsigned char)(val);break;
		case GROUP_AVERAGING: p->averaging1 = (unsigned char)(val);break;	/*0:1 1:2 2:4 3:8 4:16*/
		case GROUP_VIDEO_FILTER: p->on_off_status = set_bit_value (p->on_off_status, 1, val);break;
		case GROUP_FREQ_VAL: p->frequency1 = (unsigned short)(val);break;
		case GROUP_FREQ_POS: p->freq_pos1 = (unsigned char)(val);break;
		case GROUP_PW_POS:p->pw_pos1 = (unsigned char)(val);break;
		case GROUP_PW_VAL:p->pulser_width1 = (unsigned short)(val);break;
		case GROUP_PRF_POS: p->prf_pos1 = (unsigned char)(val);break;
		case GROUP_PRF_VAL: p->prf1 = (int)(val);break;
//		case GROUP_PQTY_POS: p->point_qty_pos1 = (char)(val);break;
//		case GROUP_PQTY_VAL: p->point_qty1 = (short)(val);break;
		default:break;
	}
}


int get_group_db_ref (CONFIG *p, int group_id)
{
	return get_bit_value (p->group[group_id].on_off_status, 0);		
}

void set_group_db_ref (CONFIG *p, int group_id, int data)
{
	assert ((data == 1) || (data == 0));
	p->group[group_id].on_off_status = set_bit_value (p->group[group_id].on_off_status, 0, data);
}

/* group操作*/
void grpcpy (CONFIG *p, unsigned int dst, unsigned int src)		/* 把src group 配置复制到 dst group */
{
	assert (src < 8);
	assert (dst < 8);
	assert (src != dst);
	memcpy (&p->group[dst], &p->group[src] , sizeof(GROUP));
}

void RequestProbeAutoDetect()
{
#if ARM
	char data = 0xcc ;
	write(pp->fd_key, &data , 1) ;
#endif
}

char get_file_type (CONFIG *p)
{
	return p->file_type;
}

void set_file_type (CONFIG *p, char data)
{
	p->file_type = data;
}
