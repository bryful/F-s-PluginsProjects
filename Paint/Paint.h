//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once

#ifndef Paint_H
#define Paint_H

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


#define	StrEXECUTED_CB1		"塗りつぶす"
#define	StrEXECUTED_CB2		"ON"
#define	StrPOS				"位置"
#define	StrCOLOR			"ペイント色"
#define	StrCOLOR_Opacity	"不透明度(%)"
#define	StrERR_getParams	"パラメータエラーです。御免なさい。"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_EXECUTED_CB,		//boolean
	ID_POS,
	ID_COLOR,
	ID_COLOR_OPACITY,
	ID_NUM_PARAMS
	};

typedef struct PaintInfo{
	PF_Boolean		executed;
	PF_FixedPoint	pos;
	PF_Pixel  		color;
	PF_Fixed		opacity;
} PaintInfo, *PaintInfoP, **PaintInfoH;

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
	void			*extraP);
}

#endif // Paint_H
