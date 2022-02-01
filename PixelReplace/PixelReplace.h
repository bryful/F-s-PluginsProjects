//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------
#pragma once
#ifndef PixelReplace_H
#define PixelReplace_H

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
#define PR_PRM_COUNT	24
enum {
	paramsOffset_topic =0,
	paramsOffset_enabled,
	paramsOffset_s_color,
	paramsOffset_d_color,
	paramsOffset_d_opacity,
	paramsOffset_topic_end,
	paramsOffset_num
};
#define PixelReplace_INPUT  0

#define PixelReplace_LV  (PR_PRM_COUNT * paramsOffset_num +1)
#define PixelReplace_POP  (PR_PRM_COUNT * paramsOffset_num +2)
#define PixelReplace_NUM_PARAMS (PR_PRM_COUNT * paramsOffset_num +3)
#define ID_NUM_PARAMS PixelReplace_NUM_PARAMS //コーディング対策

#define PARAMS_IDX(I,J) ((I) *paramsOffset_num + (J) +1)
//-----------------------------------------------------------------------------
//パラメータUI用の文字列
#define UI_TOPIC	"Target"

#define UI_ENABLED1	"Enabled"
#define UI_ENABLED2	"ON"
#define UI_SRC_COLOR	"targetColor"
#define UI_REP_COLOR	"replaceColor"
#define UI_REP_OPACITY	"replaceOpacity"

#define UI_LV		"lebel"

#define UI_POP1			"Disp"
#define UI_POP2			"1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24"


//-----------------------------------------------------------------------------
typedef struct ParamInfo8 {
	PF_Pixel8		src[PR_PRM_COUNT];
	PF_Pixel8		dst[PR_PRM_COUNT];
	PF_Pixel16		dst16[PR_PRM_COUNT];
	PF_PixelFloat	dst32[PR_PRM_COUNT];
	A_long		count;
	A_long		dispCount;
	A_u_char		Lv;

} ParamInfo8;


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


//-----------------------------------------------------------------------------

#endif // PixelReplace_H