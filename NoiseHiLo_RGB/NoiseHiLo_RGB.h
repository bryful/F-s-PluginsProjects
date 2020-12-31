//-----------------------------------------------------------------------------------
/*
	NoiseHiLo_RGB for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef NoiseHiLo_RGB_H
#define NoiseHiLo_RGB_H

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

	ID_HI_POS,
	ID_LO_POS,

	ID_HI_LV,
	ID_MD_LV,
	ID_LO_LV,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_HI_POS			"HightPos(%)"
#define	STR_LO_POS			"LoPos(%)"
#define	STR_HI_LV			"HighLevel(%)"
#define	STR_MD_LV			"MidLevel(%)"
#define	STR_LO_LV			"LoLevel(%)"

//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	hiPos;
	PF_FpLong	loPos;

	PF_FpLong	hiLv;
	PF_FpLong	mdLv;
	PF_FpLong	loLv;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
inline PF_FpLong pxV8(PF_Pixel p)
{
	PF_PixelFloat pp = CONV8TO32(p);
	PF_FpLong r = (PF_FpLong)(( 0.29891 * pp.red) + ( 0.58661 * pp.green) + ( 0.11448 * pp.blue));
	if ( r<0) r = 0;
	else if(r>1) r = 1;
	return r;
}
//-------------------------------------------------------
inline PF_FpLong pxV16(PF_Pixel16 p)
{
	PF_PixelFloat pp = CONV16TO32(p);
	PF_FpLong r = (PF_FpLong)(( 0.29891 * pp.red) + ( 0.58661 * pp.green) + ( 0.11448 * pp.blue));
	if ( r<0) r = 0;
	else if(r>1) r = 1;
	return r;
}
//-------------------------------------------------------
inline PF_FpLong pxV32(PF_PixelFloat p)
{
	PF_FpLong r = (PF_FpLong)(( 0.29891 * p.red) + ( 0.58661 * p.green) + ( 0.11448 * p.blue));
	if ( r<0) r = 0;
	else if(r>1) r = 1;
	return r;
}


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
#endif // NoiseHiLo_RGB_H
