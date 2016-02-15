/*
 * picbuf_inline.c
 *
 *  Created on: 2012-9-20
 *      Author: gener
 */

#include <gtk/gtk.h>
#include "pic00.h"
#include "pic01.h"
#include "pic02.h"
#include "pic10.h"
#include "pic11.h"
#include "pic12.h"
#include "pic20.h"
#include "pic21.h"
#include "pic22.h"
#include "pic30.h"
#include "pic31.h"
#include "pic32.h"
#include "pic_tt.h"
#include "pic311.h"
#include "pic322.h"
#include "pic_snow.h"
#include "pic_black25.h"
#include "pic_file_name40.h"
#include "pic_clock.h"
#include "pic_gear.h"

const guint8* gPicInline[] =
{
		PIC10, 		PIC11, 			PIC12,
		PIC00, 		PIC01, 			PIC02,
		PIC20, 		PIC21, 			PIC22,
		PIC30, 		PIC31, 			PIC32,
		PIC_TT,	 	PIC311, 		PIC322,
		PIC_SNOW, 	PIC_BLACK25, 	PIC_FILE_NAME40,
		PIC_CLOCK, 	PIC_GEAR,
};
