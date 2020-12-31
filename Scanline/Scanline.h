//-----------------------------------------------------------------------------------
/*
	Scanline for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Scanline_H
#define Scanline_H

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

	ID_HEIGHT,
	ID_LEVEL0,
	ID_OPACITY0,
	ID_LEVEL1,
	ID_OPACITY1,
	ID_DIR,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_HEIGHT		"ラインの太さ(pixel)"
#define	STR_LEVEL0		"奇数列ラインの明るさ(%)"
#define	STR_OPACITY0	"奇数列ラインの不透明度(%)"
#define	STR_LEVEL1		"偶数列ラインの明るさ(%)"
#define	STR_OPACITY1	"偶数列ラインの不透明度(%)"
#define STR_DIR		"方向"
#define STR_DIRSTR	"水平|垂直"

//UIのパラメータ
typedef struct ParamInfo {
	A_long		height;

	PF_FpLong	level0;
	PF_FpLong	level1;
	PF_FpLong	opacity0;
	PF_FpLong	opacity1;

	A_long		dir;

} ParamInfo, *ParamInfoP, **ParamInfoH;

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
#endif // Scanline_H
