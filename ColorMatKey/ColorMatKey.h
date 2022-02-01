//-----------------------------------------------------------------------------------
/*
	ColorMatKey for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ColorMatKey_H
#define ColorMatKey_H


#include <math.h>

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

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_REV,


	ID_TARGET_ENABLE0,
	ID_TARGET_COLOR0,
	ID_TAGET_BORDER0,
	ID_TAGET_SOFT0,

	ID_TARGET_ENABLE1,
	ID_TARGET_COLOR1,
	ID_TAGET_BORDER1,
	ID_TAGET_SOFT1,

	ID_TARGET_ENABLE2,
	ID_TARGET_COLOR2,
	ID_TAGET_BORDER2,
	ID_TAGET_SOFT2,

	ID_TARGET_ENABLE3,
	ID_TARGET_COLOR3,
	ID_TAGET_BORDER3,
	ID_TAGET_SOFT3,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_ALPHA_REV			"InvertAlpha"

#define	STR_ON					"on"
#define	STR_TARGET_ENABLED0		"Enabled0"
#define	STR_TARGET_COLOR0		"Color0"
#define	STR_TAGET_BORDER0		"Border0"
#define	STR_TAGET_SOFT0			"Softness0"

#define	STR_TARGET_ENABLED1		"Enabled1"
#define	STR_TARGET_COLOR1		"Color1"
#define	STR_TAGET_BORDER1		"Border1"
#define	STR_TAGET_SOFT1			"Softness1"

#define	STR_TARGET_ENABLED2		"Enabled2"
#define	STR_TARGET_COLOR2		"Color2"
#define	STR_TAGET_BORDER2		"Border2"
#define	STR_TAGET_SOFT2			"Softness2"

#define	STR_TARGET_ENABLED3		"Enabled3"
#define	STR_TARGET_COLOR3		"Color3"
#define	STR_TAGET_BORDER3		"Border3"
#define	STR_TAGET_SOFT3			"Softness3"


//UIのパラメータ
typedef struct CInfo {
	PF_Boolean	target_enabled;
	PF_FpLong	target_border;
	PF_FpLong	target_softness;
	PF_FpLong	target_start;
	LABA		target_lab;


} CInfo, *CInfoP, **CInfoH;


typedef struct ParamInfo {
	CInfo		ci[4];
	PF_Boolean	rev;
	PF_InData	*in_data;


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
#endif // ColorMatKey_H
