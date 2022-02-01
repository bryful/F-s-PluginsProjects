//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Max_H
#define Max_H

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


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 

	ID_MAX,
	ID_DIR,
	ID_CHANNEL,
	ID_OUT,
	ID_NUM_PARAMS
};

#define STR_MAX		"value"
#define STR_DIR1	"ScanLine"
#define STR_DIR2	"Horizon+Vurtual|Horizon|Vurtual"
#define STR_CH1		"Channel"
#define STR_CH2		"RGB+Alpha|RGB|alpha"
#define STR_OUT		"OutOnly"

typedef struct ParamInfo{
	A_long		max;
	A_long		dir;
	A_long		ch;
	PF_Boolean	isOutOnly;
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

#endif // Max_H
