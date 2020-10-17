//-----------------------------------------------------------------------------------
/*
	Spark for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Spark_H
#define Spark_H

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

#include "Params.h"
#include "CDraw8.h"
#include "CDraw16.h"
#include "CDraw32.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_SEED,
	ID_OFFSET,

	ID_START,
	ID_STARTRANDX,
	ID_STARTRANDY,

	ID_LAST,
	ID_LASTRANDX,
	ID_LASTRANDY,

	ID_LINESIZE,
	ID_LINEMOVE,
	
	ID_SUB_COUNT,
	ID_FOLD_COUNT,

	ID_DRAW_COUNT,


	ID_COLOR,
	ID_BLEND,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_START		"start"
#define	STR_LAST		"last"

#define	STR_STARTRANDX	"startRandX"
#define	STR_STARTRANDY	"startRandY"

#define	STR_LASTRANDX	"lastRandX"
#define	STR_LASTRANDY	"lastRandY"


#define	STR_LINESIZE	"lineSize"
#define	STR_LINEMOVE	"lineMove"
#define	STR_FOLDCOUNT	"foldCount"
#define	STR_DRAWCOUNT	"drawCount"
#define	STR_SUBCOUNT	"subCount"


#define	STR_OFFSET		"offset"
#define	STR_SEED		"seed"

#define	STR_COLOR		"color"
#define	STR_BLEND		"blend"
#define	STR_ON			"on"




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

#endif // Spark_H
