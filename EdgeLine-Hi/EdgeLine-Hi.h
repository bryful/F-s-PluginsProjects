//-----------------------------------------------------------------------------------
/*
	EdgeLine-Hi for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef EdgeLineHi_H
#define EdgeLineHi_H

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

	ID_TARGET_COLOR,
	ID_SAMPLE_COLOR,

	ID_LEVEL,

	ID_LENGTH,
	ID_ROT,

	ID_DRAW_COLOR,
	ID_SCANFLAG,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_TARGET_COLOR	"TargetColor"
#define	STR_SAMPLE_COLOR	"SampleColor"
#define	STR_LENGTH			"Length(px)"
#define	STR_ROT				"Rot"
#define	STR_LEVEL			"Level(%)"
#define	STR_DRAW_COLOR		"frame rander"
#define	STR_SCANFLAG		"scanFlag"
#define	STR_SCANFLAG2		"GiveUpSoon"



//UIのパラメータ
typedef struct ParamInfo {
	CFsAE			*ae;
	PF_Pixel		target_color;
	PF_Pixel		sample_color;
	PF_Fixed		rot;
	double			xscan;
	double			yscan;
	A_long			length;
	A_u_char		level;
	PF_Pixel		draw_color;
	PF_Pixel16		draw_color16;
	PF_Pixel32		draw_color32;
	PF_Boolean		GiveUpSoon;
	


} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err EdgeLineSub8(CFsAE *ae, ParamInfo *infoP);


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
#endif // EdgeLineHi_H
