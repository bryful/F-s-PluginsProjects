//-----------------------------------------------------------------------------------
/*
	YuvControl for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef YuvControl_H
#define YuvControl_H

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
	ID_UV_AUTO,
	ID_U,
	ID_V,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_Y			"Y"
#define	STR_U			"U"
#define	STR_V			"V"

#define	STR_UV_AUTO		"UVをYと連動させる"
#define	STR_ON			"on"

//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	y;
	PF_FpLong	u;
	PF_FpLong	v;
	PF_Boolean	uvAuto;
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
#endif // YuvControl_H
