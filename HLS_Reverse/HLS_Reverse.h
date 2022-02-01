//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------


#pragma once
#ifndef HLS_Reverse_H
#define HLS_Reverse_H


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

#define	STR_HUE			"hue"
#define	STR_LUM			"lightness"
#define	STR_SAT			"saturation"
#define	STR_REV			"Reverse"
#define	STR_ORG_BLEND	"blend original(%)"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_HUE,
	ID_LUM,
	ID_SAT,
	ID_ORG_BLEND,	//PF_Fixed(long)
	ID_NUM_PARAMS
	};

//プラグイン独自のパラメータを集めた構造体
typedef struct{
	PF_FpShort		org_blend;
	PF_Boolean		h;
	PF_Boolean		l;
	PF_Boolean		s;
	PF_InData		*in_data;
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
#endif // HLS_Reverse_H
