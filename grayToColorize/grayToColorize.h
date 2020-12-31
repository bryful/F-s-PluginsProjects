//-----------------------------------------------------------------------------------
/*
	grayToColorize for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef grayToColorize_H
#define grayToColorize_H

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

#include <math.h>

#define TBL_COUNT	5
#define SAMPLE_BIT	4
#define SAMPLE_MAX	4
#define SAMPLE_HARF	2
//#define SAMPLE_LEN	53.693575034635196099350836586682 // 
#define SAMPLE_LEN	5369 //  100倍

#define FROM8_SAMPLE(A)  ((((A_long)(A) * SAMPLE_MAX) + PF_HALF_CHAN8) / PF_MAX_CHAN8)
#define FROM16_SAMPLE(A) ((((A_long)(A) * SAMPLE_MAX) + PF_HALF_CHAN16) / PF_MAX_CHAN16)
#define FROM32_SAMPLE(A) ( (A_u_long)( (A * SAMPLE_MAX) + 0.5))

#define To8_SAMPLE(A)	 ((((A_long)(A) * PF_MAX_CHAN8) + SAMPLE_HARF) / SAMPLE_MAX )
#define To16_SAMPLE(A)	 ((((A_long)(A) * PF_MAX_CHAN16) + SAMPLE_HARF) / SAMPLE_MAX )
#define To32_SAMPLE(A)	 ( ((double)A + SAMPLE_HARF) / SAMPLE_MAX )

inline A_u_char RountSample(A_long v) 
{
	if (v<0) v=0;
	else if (v>SAMPLE_MAX) v = SAMPLE_MAX;
	return (A_u_char)v;
}
inline A_u_char RountSampleDouble(double v) 
{
	if (v<0) v=0;
	else if (v>SAMPLE_MAX) v = SAMPLE_MAX;
	return (A_u_char)v;
}

inline PF_Pixel conv8ToSample(PF_Pixel s)
{
	PF_Pixel ret;
	ret.red		= RountSample( FROM8_SAMPLE(s.red));
	ret.green	= RountSample( FROM8_SAMPLE(s.green));
	ret.blue	= RountSample( FROM8_SAMPLE(s.blue));
	ret.alpha	= s.alpha;
	return ret;
}
inline PF_Pixel convSampleTo8(PF_Pixel s)
{
	PF_Pixel ret;
	ret.red		= RoundByteLong( To8_SAMPLE(s.red));
	ret.green	= RoundByteLong( To8_SAMPLE(s.green));
	ret.blue	= RoundByteLong( To8_SAMPLE(s.blue));
	ret.alpha	= s.alpha;
	return ret;
}


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_ALPHA_THRESHOLD,
	ID_COL1,
	ID_COL2,
	ID_COL3,
	ID_COL4,
	ID_COL5,
	ID_MAT_CB,
	ID_MAT_COLOR,
	ID_NUM_PARAMS
};

//UIの表示文字列

#define	STR_ALPHA_THRESHOLD		"AlphaThreshold"
#define	STR_MAT_CB				"Mat"
#define	STR_MAT_COL				"MatColor"
#define	STR_COLOR_LEVEL			"ColorLevel"
#define	STR_COL_COUNT			"ColorCount"
#define	STR_COL					"TargetColor"
#define	STR_COL1				"Color1"
#define	STR_COL2				"Color2"
#define	STR_COL3				"Color3"
#define	STR_COL4				"Color4"
#define	STR_COL5				"Color5"
#define	STR_ON					"on"


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	alpha_level;
	A_u_char	alpha_border;

	PF_Pixel	col[TBL_COUNT];

	PF_Boolean	mat_on;
	PF_Pixel	mat_color;

} ParamInfo, *ParamInfoP, **ParamInfoH;
typedef struct ParamInfo16 {
	PF_FpLong	alpha_level;
	A_u_short	alpha_border;

	PF_Pixel16	col[TBL_COUNT];

	PF_Boolean	mat_on;
	PF_Pixel16	mat_color;

} ParamInfo16, *ParamInfo16P, **ParamInfo16H;
typedef struct ParamInfo32 {
	PF_FpLong	alpha_level;
	PF_FpShort	alpha_border;

	PF_PixelFloat	col[TBL_COUNT];

	PF_Boolean	mat_on;
	PF_PixelFloat	mat_color;

} ParamInfo32, *ParamInfo32P, **ParamInfo32H;

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
#endif // grayToColorize_H
