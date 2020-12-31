//-----------------------------------------------------------------------------------
/*
	PaperPlaneGetWeightInfo for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef PaperPlaneGetWeightInfo_H
#define PaperPlaneGetWeightInfo_H

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
#include "../FsLibrary/FsDebug.h"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_DRAW_GRAPH,
	ID_GLUE,
	ID_NUM_PARAMS
};

#define PILE_MAX 27
#define BASEVALUE 240

//UIの表示文字列
#define	STR_DRAW_GRAPH		"DrawGraph"
#define	STR_GLUE			"Glue"


//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	drawGraph;

	//A_u_longlong		pxCount;
	double				*weightTable;
	PF_Handle			tableH;

	A_long		center;
	A_long		pileTable[PILE_MAX];

	A_long		glue;
	double		weight;

	A_long		width;
	A_long		widthTrue;
	A_long		height;

} ParamInfo, *ParamInfoP, **ParamInfoH;

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
	void			*extra
	);

}
#endif // PaperPlaneGetWeightInfo_H
