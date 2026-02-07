//-----------------------------------------------------------------------------------
/*
	LogicGray for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef LogicGray_H
#define LogicGray_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
//#include "..\_NFLib\NF_ParamsSetup.h"

/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
*/
#include <math.h>

#include "..\_NFLib\fx\NF_LogicGray.h"
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_GRAY_MODE,	//long 1..
	ID_ORG_BLEND,	//PF_Fixed(long)
	ID_NUM_PARAMS
};

//UIの表示文字列
#define STR_GRAY_MODE_POP1	"LogicType"
#define STR_GRAY_MODE_POP2	"Luma(YUV)|Lightness(HLS)|Average(RGB)|Red Channel|Green Channel|Blue Channel|Max(RGB)|Perceptual(Lab)"
#define STR_GRAY_MODE_COUNT	8
#define STR_GRAY_MODE_DEF	1
#define STR_ORG_BLEND		"Original Blend"


//UIのパラメータ
typedef struct ParamInfo {
	PF_InData* in_data;
	A_long		gray_mode;
	PF_FpShort	org_blend;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------


//-----------------------------------------------------------------------------------
extern "C" {

DllExport
PF_Err PluginDataEntryFunction2(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB2 inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion);

DllExport 
PF_Err 
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}
#endif // LogicGray_H

