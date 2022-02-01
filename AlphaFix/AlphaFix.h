//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010 and VS2012

*/
//-----------------------------------------------------------------------------------
#pragma once


#ifndef AlphaFix_H
#define AlphaFix_H

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
	#define CONVERT16TO8(A)		( (((long)(A) * PF_MAX_CHAN8) + PF_HALF_CHAN16) / PF_MAX_CHAN16)
	#define	FLOAT_2_FIX(F)			((PF_Fixed)((F) * 65536 + (((F) < 0) ? -0.5 : 0.5)))
#else
	#include "PF_Suite_Helper.h"
	#define refconType A_long
#endif


#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#define	STR_BASE_COLOR	"背景色"

#include "../FsLibrary/FsAE.h"
	
//プラグイン独自のパラメータを集めた構造体
typedef struct ParamInfo8{
	PF_Pixel8	base_color;	
} ParamInfo8, *ParamInfo8P, **ParamInfo8H;

typedef struct ParamInfo16{
	PF_Pixel16	base_color;	
} ParamInfo16, *ParamInfo16P, **ParamInfo16H;

typedef struct ParamInfo32{
	PF_PixelFloat	base_color;	
} ParamInfo32, *ParamInfo32P, **ParamInfo32H;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_BASE_COLOR,	//
	ID_NUM_PARAMS
	};

//-----------------------------------------------------------------------------------
inline	PF_Pixel16 toPixel16(PF_Pixel s)
{
		PF_Pixel16 r;
		r.alpha	= (A_u_short)CONVERT8TO16(s.alpha);
		r.red	= (A_u_short)CONVERT8TO16(s.red);
		r.green	= (A_u_short)CONVERT8TO16(s.green);
		r.blue	= (A_u_short)CONVERT8TO16(s.blue);
		return r;
}
//-----------------------------------------------------------------------------------
inline	PF_PixelFloat toPixelFloat(PF_Pixel s)
{
	PF_PixelFloat r;
	r.alpha	= ((PF_FpShort)s.alpha / PF_MAX_CHAN8);
	r.red	= ((PF_FpShort)s.red / PF_MAX_CHAN8);
	r.green	= ((PF_FpShort)s.green / PF_MAX_CHAN8);
	r.blue	= ((PF_FpShort)s.blue / PF_MAX_CHAN8);
	return r;
}
//-------------------------------------------------------------------
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

//-------------------------------------------------------------------

#endif // AlphaFix_H
