//-----------------------------------------------------------------------------------
/*
	MaskFromRGB for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef MaskFromRGB_H
#define MaskFromRGB_H

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
	ID_POPUP,
	ID_LEVEL,
	ID_NUM_PARAMS
};

namespace M_MODE
{
	enum{
		NONE = 0,
		RED = 1,
		GREEN,
		BLUE,
		YELLOW,
		MAGENTA,
		CYAN,
		MAX
	};
};
//UIの表示文字列

#define	STR_POPUP			"Mask"
#define	STR_POPUP_ITEMS		"Red|Green|Blue|Yellow|Magenta|Cyan|Max"
#define	STR_POPUP_COUNT		6
#define	STR_POPUP_DFLT		1
#define	STR_LEVEL		"Level"


#define BD_HI	192
#define BD_LO	192

//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	PF_FpLong	col_level;

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
#endif // MaskFromRGB_H
