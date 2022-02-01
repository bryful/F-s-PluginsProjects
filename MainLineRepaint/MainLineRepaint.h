//-----------------------------------------------------------------------------------
/*
	MainLineRepaint for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef MainLineRepaint_H
#define MainLineRepaint_H

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

	ID_Main_Color,
	ID_level,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_Main_Color	"主線の色"
#define	STR_level		"許容値(%)"

//UIのパラメータ
typedef struct ParamInfo {
	PF_Pixel  		Main_Color;
	A_u_char		lv;
	A_long			count;
	PF_PixelPtr		scanline;
} ParamInfo, *ParamInfoP, **ParamInfoH;




//-------------------------------------------------------
inline A_long pxV8(PF_Pixel p)
{
	if (p.alpha == 0) return PF_MAX_CHAN16;
	PF_PixelFloat pp = CONV8TO32(p);
	if (pp.alpha>1) pp.alpha = 1.0;
	if ( pp.alpha <1.0){
		pp.red = (PF_FpShort)(pp.red * pp.alpha +  1.0 *(1.0- pp.alpha)); 
		pp.green = (PF_FpShort)(pp.green * pp.alpha +  1.0 *(1.0- pp.alpha)); 
		pp.blue = (PF_FpShort)(pp.blue * pp.alpha +  1.0 *(1.0- pp.alpha)); 
	}
	return (A_long)((( 0.29891 * pp.red) + ( 0.58661 * pp.green) + ( 0.11448 * pp.blue)) * PF_MAX_CHAN16);
}
//-------------------------------------------------------
inline A_long pxV16(PF_Pixel16 p)
{
	if (p.alpha == 0) return PF_MAX_CHAN16;
	PF_PixelFloat pp = CONV16TO32(p);
	if (pp.alpha>1) pp.alpha = 1.0;
	if ( pp.alpha <1.0){
		pp.red = pp.red * pp.alpha +  1- pp.alpha; 
		pp.green = pp.green * pp.alpha +  1- pp.alpha; 
		pp.blue = pp.blue * pp.alpha +  1- pp.alpha; 
	}
	return (A_long)((( 0.29891 * pp.red) + ( 0.58661 * pp.green) + ( 0.11448 * pp.blue)) * PF_MAX_CHAN16);
}
//-------------------------------------------------------
inline A_long pxV32(PF_PixelFloat p)
{
	if (p.alpha == 0) return PF_MAX_CHAN16;
	if (p.alpha>1) p.alpha = 1.0;
	if ( p.alpha <1.0){
		p.red = p.red * p.alpha +  1- p.alpha; 
		p.green = p.green * p.alpha +  1- p.alpha; 
		p.blue = p.blue * p.alpha +  1- p.alpha; 
	}
	return (A_long)((( 0.29891 * p.red) + ( 0.58661 * p.green) + ( 0.11448 * p.blue)) * PF_MAX_CHAN16);
}
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
#endif // MainLineRepaint_H
