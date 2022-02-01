//-----------------------------------------------------------------------------------
/*
	grayToWaveLine for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef grayToWaveLine_H
#define grayToWaveLine_H

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

	ID_SXCount, //500
	ID_SYCount, //10

	ID_DRAW_POS, //
	ID_GRAPH_XSCALE, //
	ID_GRAPH_YMAX, //

	ID_TILT,

	ID_X_OFFSET,
	ID_Y_OFFSET,

	ID_LINEHEIGHT,
	ID_GRAPH_COLOR,
	ID_BASE_COLOR,
	ID_MODE,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_SXCOUNT			"SampleXCount"
#define	STR_SYCOUNT			"SampleYCount"
#define	STR_DRAW_POS		"DrawPos"
#define	STR_GRAPH_XSCALE	"GraphXScale"
#define	STR_GRAPH_YMAX		"GraphYMax"
#define	STR_TILT			"GraphTilt"

#define	STR_X_OFFSET		"GraphXOffset"
#define	STR_Y_OFFSET		"GraphYOffset"

#define	STR_LINE_HEIGHT		"LineHeight"
#define	STR_GRAPH_COLOR		"GraphColor"
#define	STR_BASE_COLOR		"BaseColor"

#define	STR_MODE			"mode"
#define	STR_MODE_ITEM		"Wave+Base|WaveOnly|BaseOnly"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		1

enum
{
	MODE_ALL=1,
	MODE_WAVE,
	MODE_BASE
};

//UIのパラメータ
typedef struct ParamInfo {
	A_long		SXCount;
	A_long		SYCount;

	A_long		Draw_X;
	A_long		Draw_Y;
	A_long		Graph_XScale;
	A_long		Graph_YMax;
	A_long		Graph_Tilt;

	PF_Pixel	Graph_Color;
	PF_Pixel	Base_Color;
	A_long		LineHeight;

	A_long		Draw_X_Offset;
	A_long		Draw_Y_Offset;

	PF_Handle	SampleDataH;
	PF_FpLong	*SampleData;

	A_long		mode;

} ParamInfo, *ParamInfoP, **ParamInfoH;
typedef struct ParamInfo16 {
	A_long		SXCount;
	A_long		SYCount;

	A_long		Draw_X;
	A_long		Draw_Y;
	A_long		Graph_XScale;
	A_long		Graph_YMax;
	A_long		Graph_Tilt;

	PF_Pixel16	Graph_Color;
	PF_Pixel16	Base_Color;
	A_long		LineHeight;

	A_long		Draw_X_Offset;
	A_long		Draw_Y_Offset;

	PF_Handle	SampleDataH;
	PF_FpLong*	SampleData;

	A_long		mode;

} ParamInfo16, * ParamInfo16P, ** ParamInfo16H;
//-------------------------------------------------------
PF_Err exec8(CFsAE* ae, ParamInfo* infoP);
PF_Err exec16(CFsAE* ae, ParamInfo16* infoP);


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
#endif // grayToWaveLine_H
