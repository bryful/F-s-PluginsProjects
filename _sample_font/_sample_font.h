//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010 and VS2012

*/
//-----------------------------------------------------------------------------------
#pragma once


#ifndef _sample_font_H
#define _sample_font_H

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
	

#include "debug_font.h"

//プラグイン独自のパラメータを集めた構造体
typedef struct ParamInfo8{
	PF_Pixel	color;
	PF_Point	pos;
} ParamInfo8, *ParamInfo8P, **ParamInfo8H;


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_COLOR,	//
	ID_POS,
	ID_NUM_PARAMS
	};
#define	STR_COLOR		"color"
#define	STR_POS			"pos"


//-------------------------------------------------------------------
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

//-------------------------------------------------------------------

#endif // _sample_font_H

