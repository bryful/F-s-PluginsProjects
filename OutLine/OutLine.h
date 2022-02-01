//-----------------------------------------------------------------------------------
/*
	OutLine for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef OutLine_H
#define OutLine_H

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

	ID_Y,
	ID_COLOR,
	ID_LEVEL,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_Y			"Value"
#define	STR_COLOR			"Color"
#define	STR_LEVEL			"Level(%)"


//UIのパラメータ
typedef struct ParamInfo {
	A_long			value;
	PF_Pixel		color;
	PF_Pixel16		color16;
	PF_PixelFloat	color32;
	PF_FpLong		level;
	
} ParamInfo, *ParamInfoP, **ParamInfoH;

typedef struct MinInfo {
	PF_PixelPtr		data;
	A_long			width;
	A_long			widthTrue;
	A_long			height;

	A_long			value;
	
} MinInfo;

//-------------------------------------------------------


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
#endif // OutLine_H
