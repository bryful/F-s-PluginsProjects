//-----------------------------------------------------------------------------------
/*
	Extract_Edge for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Extract_Edge_H
#define Extract_Edge_H

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

	ID_CB_EDGE,
	ID_LO_LV,
	ID_HI_LV,

	ID_CB_ALPHA,
	ID_ALPHA_LV,

	ID_CB_COLOR,
	ID_COLOR,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_ON				"on"
#define	STR_CB_EDGE			"Edge"
#define	STR_LO_LV			"LoLevel"
#define	STR_HI_LV			"HiLevel"
#define	STR_CB_ALPHA		"AlphaEdge"
#define	STR_ALPHA_LV		"AlphaLevel"
#define	STR_CB_COLOR		"UseColor"
#define	STR_COLOR			"Color"



//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	Edge;
	PF_FpLong	LoLevel;
	PF_FpLong	HiLevel;
	PF_FpLong	HiLo;

	PF_Boolean	EdgeAlpha;
	PF_FpLong	AlphaLevel;

	PF_Boolean	UseColor;
	PF_Pixel	Color;
} ParamInfo, *ParamInfoP, **ParamInfoH;

typedef struct ParamInfo8 {
	ParamInfo info;
	GInfo	gin;
	GInfo	gout;

} ParamInfo8, *ParamInfo8P, **ParamInfo8H;


//-------------------------------------------------------
PF_Err Exec8(CFsAE *ae, ParamInfo *infoP);

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
#endif // Extract_Edge_H
