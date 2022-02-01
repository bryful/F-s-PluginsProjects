//-----------------------------------------------------------------------------------
/*
	MaskFromRGB_Multi for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef MaskFromRGB_Multi_H
#define MaskFromRGB_Multi_H

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
namespace M_COL{
	enum{
		R =0,
		G,
		B,
		Y,
		M,
		C,
		Count
	};
};
enum {
	ID_INPUT = 0,	// default input layer
	ID_LEVEL,
	ID_R,
	ID_G,
	ID_B,
	ID_Y,
	ID_M,
	ID_C,
	ID_NUM_PARAMS
};

//UIの表示文字列

#define	STR_LEVEL		"Level"
#define	STR_R			"Red"
#define	STR_G			"Green"
#define	STR_B			"Blue"
#define	STR_Y			"Yellow"
#define	STR_M			"Magenta"
#define	STR_C			"Cyan"
#define	STR_ON			"on"


//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean		colFlag[M_COL::Count];
	PF_FpLong		col_level;

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
#endif // MaskFromRGB_Multi_H
