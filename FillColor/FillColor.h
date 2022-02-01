//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------


#pragma once
#ifndef FillColor_H
#define FillColor_H

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


#define STR_ENABLED_CB1	"実行する"
#define STR_ENABLED_CB2	"ON"
#define STR_COLOR		"色"
#define STR_OPA_FIXED	"不透明度"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	
	//以下参考用
	ID_ENABLED_CB,			//boolean
	ID_FILL_COLOR,	//
	ID_OPA_COLOR_FIXED,
	ID_NUM_PARAMS
	};
	
typedef struct ParamInfo8{
	PF_Boolean	enabled;
	PF_Pixel  	fill_color;
	PF_FpShort	opa_color;
	PF_FpShort	opa_color2;
	
} ParamInfo8, *ParamInfo8P, **ParamInfo8H;

typedef struct ParamInfo16{
	PF_Boolean	enabled;
	PF_Pixel16  fill_color;
	PF_FpShort	opa_color;
	PF_FpShort	opa_color2;
	
} ParamInfo16, *ParamInfo16P, **ParamInfo16H;

typedef struct ParamInfo32{
	PF_Boolean		enabled;
	PF_PixelFloat	fill_color;
	PF_FpShort		opa_color;
	PF_FpShort		opa_color2;
	
} ParamInfo32, *ParamInfo32P, **ParamInfo32H;

//-----------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------------
#endif // FillColor_H
