//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once

#ifndef Paint_H
#define Paint_H

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


#define	STR_TOPIC			"Point_%d"
#define	STR_EXECUTED_CB1	"塗りつぶす_%d"
#define	STR_EXECUTED_CB2	"ON"
#define	STR_POS				"位置_%d"
#define	STR_COLOR			"ペイント色_%d"

#ifdef AE_OS_WIN
#define	STR_GUIDE_CB1		"ガイド表示_%d"
#define	STR_GUIDE_ALL_CB2	"非表示"
#define STR_GUIDE_ALL_CB1 "ガイドをすべて非表示にする"
#else
#define	STR_GUIDE_CB1		"ガイド表\示_%d"
#define	STR_GUIDE_ALL_CB2	"非表\示"
#define STR_GUIDE_ALL_CB1 "ガイドをすべて非表\示にする"
#endif


typedef struct{
	PF_FixedPoint	pos;
	PF_Pixel		color;
	PF_Boolean		disp_guide;
} PointColorInfo;

#define POINT_COLOR_COUNT	8

typedef struct{
	A_long			Count;
	PointColorInfo	prms[POINT_COLOR_COUNT];
	PF_Boolean		disp_guide_all;
} ParamInfo;

//ユーザーインターフェースのID
#define ID_INPUT	0
#define ID_UI_START	1
#define ID_UI_COUNT	6
#define ID_UI_TOPIC(X)  ((X)*ID_UI_COUNT + ID_UI_START + 0)
#define ID_UI_CB(X)		((X)*ID_UI_COUNT + ID_UI_START + 1)
#define ID_UI_POS(X)	((X)*ID_UI_COUNT + ID_UI_START + 2)
#define ID_UI_COL(X)	((X)*ID_UI_COUNT + ID_UI_START + 3)
#define ID_UI_GID(X)	((X)*ID_UI_COUNT + ID_UI_START + 4)
#define ID_UI_TOPIC_END(X)  ((X)*ID_UI_COUNT + ID_UI_START + 5)
#define ID_UI_GUIDE_VISIBLE  (POINT_COLOR_COUNT * ID_UI_COUNT + ID_UI_START)
#define ID_NUM_PARAMS   (ID_UI_GUIDE_VISIBLE + 1)

//-------------------------------------------------------
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

#endif // Paint_H
