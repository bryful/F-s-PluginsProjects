//-----------------------------------------------------------------------------------
/*
	Gradient for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Gradient_H
#define Gradient_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
*/
#include "..\_NFLib\fx\NF_Gradient.h"
#include "..\_NFLib\fx\NF_Blend.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE_POP,
	ID_START_POS,
	ID_START_COLOR,
	ID_START_OPA,

	ID_END_POS,
	ID_END_COLOR,
	ID_END_OPA,

	ID_ID_ALPHA_HYPERBOLIC,
	ID_ID_RGB_HYPERBOLIC,

	ID_SWAP_COLOR_BTN,
	ID_BLEND,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE_POP		"draw"
#define	STR_MODE_ITEMS		"Linear|Radial"
#define	STR_MODE_COUNT		2
#define	STR_MODE_DFLT		1


#define	STR_START_COLOR		"startColor"
#define	STR_START_POS		"startPoint"
#define	STR_START_OPACITY	"startOpacity"
#define	STR_END_POS			"endPoint"
#define	STR_END_COLOR		"endColor"
#define	STR_END_OPACITY		"endOpacity"
#define	STR_SWAP_COLOR_BTN	"swapColor"
#define	STR_SWAP_COLOR_BTN2	"swap"

#define	STR_alpha_hyperbolic	"alphaHyperbolic"
#define	STR_rgb_hyperbolic		"rgbHyperbolic"
#define	STR_BLEND			"Blend with original"

//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	A_FloatPoint	start_pos;
	PF_Pixel	start_color;
	PF_FpLong	start_opa;

	A_FloatPoint	end_pos;
	PF_Pixel	end_color;
	PF_FpLong	end_opa;

	PF_FpLong	alpha_hyperbolic;
	PF_FpLong	rgb_hyperbolic;

	PF_Boolean	isBlend;

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
#endif // Gradient_H

