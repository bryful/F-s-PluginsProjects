//-----------------------------------------------------------------------------------
/*
	EdgeLine for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef EdgeLine_H
#define EdgeLine_H

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

	ID_TARGET_COL,
	ID_SAMPLE_COL,
	ID_LEVEL,
	ID_LENGTH,
	ID_DRAW_COL,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_TARGET			"TargetColor"
#define	STR_SAMPLE			"SampleColor"
#define	STR_LEVEL			"level(%)"
#define	STR_LENGTH			"length(px)"
#define	STR_DRAW			"DrawColor"


//UIのパラメータ
typedef struct ParamInfo8 {
	PF_Pixel	targetCol;
	PF_Pixel	sampleCol;
	A_u_char	level;
	A_long		length;
	PF_Pixel	drawCol;
	A_long		w;
	A_long		h;
	CFsAE		*ae;

} ParamInfo8;

typedef struct ParamInfo16 {
	PF_Pixel	targetCol;
	PF_Pixel	sampleCol;
	A_u_char	level;
	A_long		length;
	PF_Pixel16	drawCol;
	A_long		w;
	A_long		h;
	CFsAE		*ae;
} ParamInfo16;

typedef struct ParamInfo32 {
	PF_Pixel		targetCol;
	PF_Pixel		sampleCol;
	A_u_char		level;
	A_long			length;
	PF_PixelFloat	drawCol;
	A_long		w;
	A_long		h;
	CFsAE		*ae;
} ParamInfo32;
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
#endif // EdgeLine_H
