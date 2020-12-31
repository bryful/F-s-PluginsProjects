//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ColorChange_H
#define ColorChange_H

#include "Fs_Target.h"


#include "AEConfig.h"
#include "entry.h"


/*
	CS5でコンパイルする時は、TARGETCS5を有効にして
	Supporting codeのCS4onlyをビルドから除外、CS5onlyをビルド有効にする

	そして、プラットフォームをx64へプロパティを変更する事
*/


//#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"


#if defined(PF_AE100_PLUG_IN_VERSION)
	#include "AEFX_SuiteHelper.h"
	#define refconType void*
	#define CONVERT16TO8(A)		( (((long)(A) * PF_MAX_CHAN8) + PF_HALF_CHAN16) / PF_MAX_CHAN16)
	#define	FLOAT_2_FIX(F)			((PF_Fixed)((F) * 65536 + (((F) < 0) ? -0.5 : 0.5)))
#else
	#include "PF_Suite_Helper.h"
	#define refconType A_long
#endif

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#include "../FsLibrary/FsAE.h"

#define ColorChangeCount 8

enum {
	ID_INPUT = 0,		// default input layer 
	ID_LEVEL,

	ID_ENABLE,

	ID_TARGET0,
	ID_SRCCOL0,
	ID_DSTCOL0,

	ID_TARGET1,
	ID_SRCCOL1,
	ID_DSTCOL1,

	ID_TARGET2,
	ID_SRCCOL2,
	ID_DSTCOL2,

	ID_TARGET3,
	ID_SRCCOL3,
	ID_DSTCOL3,

	ID_TARGET4,
	ID_SRCCOL4,
	ID_DSTCOL4,

	ID_TARGET5,
	ID_SRCCOL5,
	ID_DSTCOL5,

	ID_TARGET6,
	ID_SRCCOL6,
	ID_DSTCOL6,

	ID_TARGET7,
	ID_SRCCOL7,
	ID_DSTCOL7,

	ID_NUM_PARAMS
};


#define STR_LV		"許容値"
#define STR_MODE1	"モード"
#define STR_EXEC	"実行する"

#define STR_TARGET0	"Target0"
#define STR_SRCCOL0	"元の色0"
#define STR_DSTCOL0	"新しい色0"

#define STR_TARGET1	"Target1"
#define STR_SRCCOL1	"元の色1"
#define STR_DSTCOL1	"新しい色1"

#define STR_TARGET2	"Target2"
#define STR_SRCCOL2	"元の色2"
#define STR_DSTCOL2	"新しい色2"

#define STR_TARGET3	"Target3"
#define STR_SRCCOL3	"元の色3"
#define STR_DSTCOL3	"新しい色3"

#define STR_TARGET4	"Target4"
#define STR_SRCCOL4	"元の色4"
#define STR_DSTCOL4	"新しい色4"

#define STR_TARGET5	"Target5"
#define STR_SRCCOL5	"元の色5"
#define STR_DSTCOL5	"新しい色5"

#define STR_TARGET6	"Target6"
#define STR_SRCCOL6	"元の色6"
#define STR_DSTCOL6	"新しい色6"

#define STR_TARGET7	"Target7"
#define STR_SRCCOL7	"元の色7"
#define STR_DSTCOL7	"新しい色7"


typedef struct ParamInfo{
	PF_Boolean	enabled;
	A_u_char		Lv;
	A_long			count;
	PF_Pixel		Src[ColorChangeCount];
	PF_Pixel		Dst[ColorChangeCount];
	PF_Pixel16		Dst16[ColorChangeCount];
	PF_PixelFloat	Dst32[ColorChangeCount];
} ParamInfo,*ParamInfoP,**ParamInfoH;



extern "C" {

DllExport 
PF_Err 
EntryPointFunc (	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}



#endif // ColorChange_H