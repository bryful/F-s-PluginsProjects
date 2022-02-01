//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef EdgeBlur_H
#define EdgeBlur_H



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



//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------

#define	STR_BLUR	"blur"
#define	STR_OFFSET	"offset"

//-----------------------------------------------------------------------------------
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_BLUR,	//
	ID_OFFSET,	//
		ID_NUM_PARAMS
	};

//-----------------------------------------------------------------------------------

typedef struct ParamInfo{
	A_long			blur;
	A_long			offset;
	
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-----------------------------------------------------------------------------------
typedef struct edgeBlurPrm
{
	PF_InData		*in_data;
	PF_PixelPtr		data;
	A_long			width;
	A_long			widthTrue;
	A_long			height;
	A_long			scanlineWidth;
	PF_Handle		scanlineH;
	A_long			gf[301];
	A_long			blur;
	A_long			offset;
}edgeBlurPrm;

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

//-------------------------------------------------------------------------------------------------
PF_Err EdgeBlurSub8(edgeBlurPrm *prm);
PF_Err EdgeBlurSub16(edgeBlurPrm *prm);
PF_Err EdgeBlurSub32(edgeBlurPrm *prm);

//-------------------------------------------------------------------------------------------------

#endif // EdgeBlur_H
