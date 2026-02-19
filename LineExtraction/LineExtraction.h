//-----------------------------------------------------------------------------------
/*
	LineExtraction for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef LineExtraction_H
#define LineExtraction_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_Blend.h"

#include "LineExtractionFilter.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_PRE_TOPIC,
	ID_BILATERAL_CB,
	ID_B_RADIUS,
	ID_B_SIGMA_S,
	ID_B_SIGMA_R,
	ID_PRE_TOPIC_END,
	ID_OUTER_WIDTH,
	ID_INNER_WIDTH,
	ID_COLOR,
	ID_POST_LEVEL_CB,
	ID_POST_LEVEL_LO,
	ID_POST_LEVEL_HI,
	ID_BLEND,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_PRE_TOPIC		"Pre-processing"
#define STR_BILATERAL_CB	"Bilateral"
#define STR_B_RADIUS		"Radius"
#define STR_B_SIGMA_S		"Sigma_Spatial"
#define STR_B_SIGMA_R		"Sigma_Range"
#define	STR_POST_LEVEL_CB	"Post Level"
#define	STR_POST_LEVEL_LO	"Level_lo"
#define	STR_POST_LEVEL_HI	"Level_hi"
#define	STR_OUTER_WIDTH		"Outer Sampling"
#define	STR_INNER_WIDTH		"Inner Sampling"
#define	STR_COLOR			"Color"
#define	STR_BLEND			"Blend with original"

//UIのパラメータ
//-------------------------------------------------------
struct BilateralContext {
	float spatial_lut[441]; // 半径10 (21x21) まで対応
	float range_lut[256];   // 輝度差 0-255 の重み
	int radius;
	int kernel_size;
	float sigma_s;
	float sigma_r;
}; 

typedef struct ParamInfo {
	PF_Pixel	color;
	PF_Pixel16	color16;
	PF_PixelFloat	color32;

	A_long		innerWidth;
	A_long		outerWidth;
	PF_Boolean	isLevel;
	PF_FpLong	preLevelLo;
	PF_FpLong	preLevelHi;
	PF_Boolean	isBilateral;
	PF_Boolean	isBlend;
	BilateralContext bilateralCtx;
} ParamInfo, *ParamInfoP, **ParamInfoH;


PF_Err PF_BilateralExec
(
	NF_AE* ae,
	ParamInfo* infoP
);
PF_Err LevelExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_FpLong hi,
	PF_FpLong lo
);
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
#endif // LineExtraction_H

