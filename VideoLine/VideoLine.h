//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once

#ifndef VideoLine_H
#define VideoLine_H


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


enum {
	ID_INPUT = 0,		// default input layer 
	ID_BRIGHT,
	ID_HEIGHT,
	ID_REV,
	ID_INTER,
	ID_DIR,
	ID_OFFSET,
	ID_NUM_PARAMS
};

#define	STR_BRIGHT "ラインの明るさ(%)"
#define	STR_HEIGHT "ラインの高さ(dot)"
#define	STR_REV1	"ラインの位置"
#define	STR_REV2	"反転する"
#define STR_INTER	"間隔補正(dot)"
#define STR_DIR		"方向"
#define STR_DIRSTR	"水平|垂直"
#define STR_OFFSET	"オフセット"

typedef struct{
	PF_FpShort	bright;
	PF_Boolean	minus;
	A_long		height;
	A_long		inter;
	PF_Boolean	revFlag;
	PF_Boolean	vurFlag;
	PF_FpShort	offset;
} ParamInfo;

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

#endif // VideoLine_H