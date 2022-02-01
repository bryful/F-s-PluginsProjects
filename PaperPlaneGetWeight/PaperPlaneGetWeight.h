//-----------------------------------------------------------------------------------
/*
	PaperPlaneGetWeight for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef PaperPlaneGetWeight_H
#define PaperPlaneGetWeight_H

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

	ID_ISHALF,
	ID_COUNT,
	ID_WEIGHT,

	ID_NUM_PARAMS
};

#define WEIGHT_SCALE 5
enum {
	pw180 = 180 / WEIGHT_SCALE,
	pw225 = 225 / WEIGHT_SCALE,
	pw310 = 310 / WEIGHT_SCALE,
	pw360 = 360 / WEIGHT_SCALE,
};

//UIの表示文字列
#define	STR_ISHALF		"HalfSize"
#define	STR_COUNT		"Count"
#define	STR_WEIGHT			"Weight"
#define	STR_WEIGHT_ITEMS	"180|225|310|360"
#define	STR_WEIGHT_COUNT	4
#define	STR_WEIGHT_DFLT		2


//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean		isHalf;
	A_long			count;
	A_long			weight;
	A_u_char		weightG;
	A_u_char		weightR;
	A_u_char		weightB;

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
#endif // PaperPlaneGetWeight_H
