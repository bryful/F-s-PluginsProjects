//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once


#ifndef Frame_H
#define Frame_H

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
	ID_WIDTH_ADD,	//long
	ID_HEIGHT_ADD,	//long
	ID_LINE_COLOR,		//PF_Pixel
	ID_SAFE_ADD,	//long
	ID_CENTER_CB,	//boolean
	ID_OUT_COLOR,		//PF_Pixel
	ID_OUT_OPACITY,
	ID_LINE_HEIGHT,
	ID_SIZE_POP,
	ID_NUM_PARAMS
	};
	
enum {
	SIZE_CUSTUM = 1,
	SIZE_720_540,
	SIZE_1024_576,
	SIZE_1024_768,
	SIZE_1280_720,
	SIZE_1440_810,
	SIZE_1920_1080,
};

	
#define	Str_WIDTH_ADD			"width"
#define	Str_HEIGHT_ADD			"height"
#define	Str_LINE_COLOR			"line_color"
#define	Str_SAFE_ADD			"safe_frame"
#define	Str_CENTER_CB1			"center_line"
#define	Str_CENTER_CB2			"ON"
#define	Str_OUT_COLOR			"out_color"
#define	Str_OUT_OPACITY			"out_opacity"
#define	Str_LINE_HEIGHT			"line_height"
#define	Str_SIZE_POP1			"size"
#define	Str_SIZE_POP2			"上の数値|720x540|1024x576|1024x768|1280x720|1440x810|1920x1080"


typedef struct {
	A_long			frame_Width;
	A_long			frame_Height;
	PF_Pixel		line_color;
	A_long			safe_frame;
	PF_Boolean		center_line;
	PF_Pixel		out_color;
	PF_FpShort		out_opacity;
	A_long			line_height;
	A_long			center_x;
	A_long			center_y;
	PF_Rect			frm;
	PF_Rect			safe;
	A_long			size_kind;
} ParamInfo;




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
//-------------------------------------------------------

#endif // Frame_H
