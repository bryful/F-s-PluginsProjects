//-----------------------------------------------------------------------------------
/*
	grayToCountourLine for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef grayToCountourLine_H
#define grayToCountourLine_H

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

	ID_COUNT_LO,
	ID_COUNT_MID,
	ID_COUNT_HI,
	ID_DRAW_COL,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_LO				"Count_Lo"
#define	STR_MID				"Count_Mid"
#define	STR_HI				"Count_Hi"
#define	STR_DRAW_COL		"DrawCol"
#define	STR_ON				"on"


enum GBDR
{
	ZERO = 0,
	LO = 85,
	HI = 170,
	MAX = 256
};
//UIのパラメータ
typedef struct ParamInfo {
	A_u_char	tbl[256];
	PF_Boolean	drawCol;
	A_u_char	col[30];
	A_long		colCount;
	A_long		colWidth;
	A_long		colHeight;

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
#endif // grayToCountourLine_H
