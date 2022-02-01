//-----------------------------------------------------------------------------------
/*
	Max_Kasumi for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Max_Kasumi_H
#define Max_Kasumi_H

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

	ID_MAX,
	ID_DIR,
	ID_CHANNEL,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define STR_MAX		"Value"
#define STR_DIR1	"ScanLine"
#define STR_DIR2	"Horizon+Vertical|Horizon|Vertical"
#define STR_CH1		"Channel"
#define STR_CH2		"RGB+Alpha|RGB|alpha"




//UIのパラメータ
typedef struct ParamInfo {
	A_long		max;
	PF_Boolean	minus;
	A_long		dir;
	A_long		ch;
} ParamInfo;

typedef struct ParamInfo8 {
	ParamInfo	info;

	PF_InData	*in_data;
	A_long		width;
	A_long		widthTrue;
	A_long		widthOffset;
	A_long		height;
	PF_Pixel	*data;

	A_long		bufSize;
	PF_Handle	bufH;
	PF_Pixel	*scanline;
} ParamInfo8;

typedef struct ParamInfo16 {
	ParamInfo	info;

	PF_InData	*in_data;
	A_long		width;
	A_long		widthTrue;
	A_long		widthOffset;
	A_long		height;
	PF_Pixel16	*data;

	A_long		bufSize;
	PF_Handle	bufH;
	PF_Pixel16	*scanline;
} ParamInfo16;

typedef struct ParamInfo32 {
	ParamInfo	info;

	PF_InData	*in_data;
	A_long		width;
	A_long		widthTrue;
	A_long		widthOffset;
	A_long		height;
	PF_PixelFloat	*data;

	A_long		bufSize;
	PF_Handle	bufH;
	PF_PixelFloat	*scanline;
} ParamInfo32;

//-------------------------------------------------------
PF_Err Exec8(CFsAE *ae, ParamInfo *infoP);
PF_Err Exec16(CFsAE *ae, ParamInfo *infoP);
PF_Err Exec32(CFsAE *ae, ParamInfo *infoP);
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
#endif // Max_Kasumi_H
