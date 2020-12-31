//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once

#ifndef VideoLine2nd_H
#define VideoLine2nd_H


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
	ID_INTER,
	ID_DIR,
	ID_NUM_PARAMS
};

#define	STR_BRIGHT "ラインの明るさ(%)"
#define	STR_HEIGHT "ラインの高さ(dot)"
#define STR_INTER	"間隔補正(dot)"
#define STR_DIR		"方向"
#define STR_DIRSTR	"水平|垂直"

typedef struct{
	PF_FpLong	bright;
	A_long		height;
	A_long		inter;
	PF_Boolean	vurFlag;
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

#endif // VideoLine2nd_H