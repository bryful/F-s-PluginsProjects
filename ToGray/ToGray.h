//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ToGray_H
#define ToGray_H

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
#include "../FsLibrary/FsHLS.h"
//#include "FsAE.h"


#define STR_GRAY_MODE_POP1	"mode"
#define STR_GRAY_MODE_POP2	"(Y)uv|h(L)s|(R+G+B)/3|(R)gb|r(G)b|rg(B)|RGBMax|(L)ab"
#define STR_GRAY_MODE_COUNT	8
#define STR_GRAY_MODE_DEF	1
#define STR_ORG_BLEND		"blend original(%)"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_GRAY_MODE,	//long 1..
	ID_ORG_BLEND,	//PF_Fixed(long)
	ID_NUM_PARAMS
	};

//プラグイン独自のパラメータを集めた構造体
typedef struct ParamsInfo{
	PF_InData	*in_data;
	A_long		gray_mode;
	PF_FpShort	org_blend;
} ParamInfo;

//-------------------------------------------------------
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
#endif // ToGray_H
