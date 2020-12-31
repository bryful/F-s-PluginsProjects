//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef RGBAControl_H
#define RGBAControl_H

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


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_RED_FIXED,	//PF_Fixed(long)
	ID_GREEN_FIXED,	//PF_Fixed(long)
	ID_BLUE_FIXED,	//PF_Fixed(long)
	ID_ALPHA_FIXED,	//PF_Fixed(long)
	ID_NUM_PARAMS
};

#define STR_RED		"Red(%)"
#define STR_GREEN	"Green(%)"
#define STR_BLUE	"Blue(%)"
#define STR_ALPHA	"Alpha(%)"

//ＡＥ上のパラメータは -100 〜 100の値を取るが
//内部では -1 〜 1の値とする
typedef struct ParamInfo{
	PF_FpShort		red;
	PF_FpShort		green;
	PF_FpShort		blue;
	PF_FpShort		alpha;
} ParamInfo;

//**************************************************************************
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

#endif // RGBAControl_H
