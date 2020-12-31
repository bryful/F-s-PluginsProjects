//-----------------------------------------------------------------------------------
/*
	ChannelBlur for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ChannelBlur_H
#define ChannelBlur_H

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

	ID_MODE,

	ID_0CH_R,
	ID_1CH_G,
	ID_2CH_B,
	ID_3CH_A,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE			"Mode"
#define	STR_MODE_ITEMS		"RGB|HLS|Yuv"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		1

#define	STR_0CH_R			"R_H_Y_blur"
#define	STR_1CH_G			"G_L_u_blur"
#define	STR_2CH_B			"B_S_v_blur"
#define	STR_3CH_A			"Alpha_blur"




//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	A_long		r_blur;
	A_long		g_blur;
	A_long		b_blur;
	A_long		a_blur;

} ParamInfo, *ParamInfoP, **ParamInfoH;

typedef struct ParamInfo8 {
	ParamInfo	info;


} ParamInfo8, *ParamInfo8P, **ParamInfo8H;

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
#endif // ChannelBlur_H
