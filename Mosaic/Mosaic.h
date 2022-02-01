//-----------------------------------------------------------------------------------
/*
	F's Plugins-cs4 for VS2008
*/
//-----------------------------------------------------------------------------------
#pragma once

#ifndef Mosaic_H
#define Mosaic_H

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

enum {
	ID_INPUT = 0,	// default input layer 
	ID_SIZE,		// モザイクの大きさ
	ID_POS,			//位置
	ID_RAND,		//強さのチラツキ
	ID_RAND_GRAY,		//強さのチラツキ
	ID_FRAME_RAND_ON,
	ID_NUM_PARAMS
};

#define STR_SIZE		"サイズ"
#define STR_POS			"位置"
#define STR_RAND		"ちらつき"
#define STR_RAND_GRAY	"ちらつきをグレーに"
#define STR_FRAME_RAND_ON	"フレーム毎にちらつかせる"
#define STR_ON			"ON"

//UIのパラメータ
typedef struct ParamInfo {
	A_long			size;	//モザイクの大きさ
	PF_FixedPoint	pos;	//モザイクの
	PF_FpShort		rand;
	PF_Boolean		rand_gray;
	PF_Boolean		frameRandOn;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-----------------------------------------------------------------

extern "C" {
DllExport
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP);
}


#endif // RandomMosaic