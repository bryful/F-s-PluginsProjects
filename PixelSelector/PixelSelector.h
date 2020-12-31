//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once
#ifndef PixelSelector_H
#define PixelSelector_H

#include "Fs_Target.h"

#include "AEConfig.h"
#include "entry.h"

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
#else
	#include "PF_Suite_Helper.h"
	#define refconType A_long
#endif

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#include "../FsLibrary/FsAE.h"
//#include "FsAE.h"

//-----------------------------------------------------------------------------
//ParamsSetup用の定数
#define PS_PRM_COUNT	24
enum {
	paramsOffset_topic = 0,
	paramsOffset_enabled,
	paramsOffset_s_color,
	paramsOffset_topic_end,
	paramsOffset_num
};
enum{
	PixelSelector_INPUT	= 0,
	PixelSelector_TOPIC,
	PixelSelector_REV,
	PixelSelector_FILL,
	PixelSelector_FILL_COLOR,
	PixelSelector_FILL_OPACITY,
	PixelSelector_TOPIC_END,
	PixelSelector_NUM,

};
#define PARAMS_IDX(I,J) (PixelSelector_NUM + (I) *paramsOffset_num + (J) )
#define PixelSelector_LV (PixelSelector_NUM + PS_PRM_COUNT * paramsOffset_num)
#define PixelSelector_POP (PixelSelector_NUM + PS_PRM_COUNT * paramsOffset_num+1)
#define PixelSelector_NUM_PARAMS (PixelSelector_NUM + PS_PRM_COUNT * paramsOffset_num+2)
#define ID_NUM_PARAMS PixelSelector_NUM_PARAMS
//-----------------------------------------------------------------------------
//パラメータUI用の文字列
#define UI_TOPIC	"Option"
#define UI_ON		"ON"
#define UI_REV		"反転する"
#define UI_FILL		"塗りつぶす"
#define UI_FILL_COLOR	"fillColor"
#define UI_FILL_OPA		"fillColor_opacity"

#define UI_ENABLED	"Enabled"
#define UI_SRC_COLOR	"targetColor"

#define UI_LV			"level"

#define UI_POP1			"Disp"
#define UI_POP2			"1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24"

#define ERR_GET_AEPRM	"すみませんエラーです。\nAfterEffects_Params error!"
#define ERR_GET_PRM		"すみませんエラーです。\nPixelSelector_Params error!"
//-----------------------------------------------------------------------------

typedef struct
{
	PF_Pixel		src[PS_PRM_COUNT];
	A_long			count;
	PF_Boolean		rev;
	PF_Boolean		fill;
	PF_Pixel		col;
	PF_Pixel16		col16;
	PF_PixelFloat	col32;
	A_long		dispCount;
	A_u_char	lv;
} ParamInfo;

//-----------------------------------------------------------------------------
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

#endif // PixelSelector_H