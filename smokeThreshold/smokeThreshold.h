//-----------------------------------------------------------------------------------
/*
	smokeThreshold for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef smokeThreshold_H
#define smokeThreshold_H

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
	ID_ALPHA,
	ID_HI_ENABLED,
	ID_SDW1_ENABLED,
	ID_SDW2_ENABLED,
	ID_HI,
	ID_SDW1,
	ID_SDW2,
	ID_HI_COL,
	ID_NOR_COL,
	ID_SDW1_COL,
	ID_SDW2_COL,
	ID_LINE_TOPIC,
	ID_LINE_WEIGHT,
	ID_NOR_LINE_ENABLED,
	ID_SDW1_LINE_ENABLED,
	ID_SDW2_LINE_ENABLED,
	ID_NOR_LINE,
	ID_SDW1_LINE,
	ID_SDW2_LINE,
	ID_LINE_TOPIC_END,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_ALPHA			"alphaThreshold"
#define	STR_HI				"hilight"
#define	STR_SDW1			"shadow1"
#define	STR_SDW2			"shadow2"

#define	STR_ON				"on"
#define	STR_ALPHA_E			"alphaThresholdEnabled"
#define	STR_HI_E			"hilightEnabled"
#define	STR_SDW1_E			"shadow1Enabled"
#define	STR_SDW2_E			"shadow2Enabled"

#define	STR_HI_COL			"hilightColor"
#define	STR_NOR_COL			"normalColor"
#define	STR_SDW1_COL		"shadow1Color"
#define	STR_SDW2_COL		"shadow2Color"

#define	STR_LINE_WEIGHT		"LineWeight"

#define STR_OUTLINE			"OutLine"

#define	STR_NOR_LINE		"normalLine"
#define	STR_SDW1_LINE		"shadow1Line"
#define	STR_SDW2_LINE		"shadow2Line"

#define	STR_NOR_LINE_E		"normalLine_Enabled"
#define	STR_SDW1_LINE_E		"shadow1Line_Enabled"
#define	STR_SDW2_LINE_E		"shadow2Line_Enabled"

//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	h_on;
	PF_Boolean	s1_on;
	PF_Boolean	s2_on;

	PF_Boolean	nl_on;
	PF_Boolean	s1l_on;
	PF_Boolean	s2l_on;

	double		alpha;	
	double		hi;	
	double		sdw1;	
	double		sdw2;	

	PF_Pixel	hiCol;
	PF_Pixel	norCol;
	PF_Pixel	sdw1Col;
	PF_Pixel	sdw2Col;

	A_long		lineWeight;
	PF_Pixel	norLine;
	PF_Pixel	sdw1Line;
	PF_Pixel	sdw2Line;
} ParamInfo, *ParamInfoP, **ParamInfoH;

typedef struct ParamInfo8 {
	PF_Boolean	a_on;
	PF_Boolean	h_on;
	PF_Boolean	s1_on;
	PF_Boolean	s2_on;

	PF_Boolean	nl_on;
	PF_Boolean	s1l_on;
	PF_Boolean	s2l_on;

	A_u_char	alpha;	
	A_u_char	hi;	
	A_u_char	sdw1;	
	A_u_char	sdw2;	

	PF_Pixel	hiCol;
	PF_Pixel	norCol;
	PF_Pixel	sdw1Col;
	PF_Pixel	sdw2Col;

	A_long		lineWeight;
	PF_Pixel	norLine;
	PF_Pixel	sdw1Line;
	PF_Pixel	sdw2Line;

} ParamInfo8, *ParamInfo8P, **ParamInfo8H;
typedef struct ParamInfo16 {
	PF_Boolean	a_on;
	PF_Boolean	h_on;
	PF_Boolean	s1_on;
	PF_Boolean	s2_on;

	PF_Boolean	nl_on;
	PF_Boolean	s1l_on;
	PF_Boolean	s2l_on;

	A_u_short	alpha;	
	A_u_short	hi;	
	A_u_short	sdw1;	
	A_u_short	sdw2;	

	PF_Pixel16	hiCol;
	PF_Pixel16	norCol;
	PF_Pixel16	sdw1Col;
	PF_Pixel16	sdw2Col;

	A_long		lineWeight;
	PF_Pixel16	norLine;
	PF_Pixel16	sdw1Line;
	PF_Pixel16	sdw2Line;
} ParamInfo16, *ParamInfo16P, **ParamInfo16H;
typedef struct ParamInfo32 {
	PF_Boolean	a_on;
	PF_Boolean	h_on;
	PF_Boolean	s1_on;
	PF_Boolean	s2_on;

	PF_Boolean	nl_on;
	PF_Boolean	s1l_on;
	PF_Boolean	s2l_on;

	PF_FpShort	alpha;	
	PF_FpShort	hi;	
	PF_FpShort	sdw1;	
	PF_FpShort	sdw2;	

	PF_PixelFloat	hiCol;
	PF_PixelFloat	norCol;
	PF_PixelFloat	sdw1Col;
	PF_PixelFloat	sdw2Col;

	A_long		lineWeight;
	PF_PixelFloat	norLine;
	PF_PixelFloat	sdw1Line;
	PF_PixelFloat	sdw2Line;
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
#endif // smokeThreshold_H
