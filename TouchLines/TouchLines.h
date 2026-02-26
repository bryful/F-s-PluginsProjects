//-----------------------------------------------------------------------------------
/*
	TouchLine for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef TouchLine_H
#define TouchLine_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

/*
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"

#include "TouchLineFilter.h"
*/
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_Blend.h"
#include <string>
#include <vector>

#include "ThresholdLine.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_TARGET_TOPIC,
	ID_VALUE,
	ID_THRESHOLD,
	ID_LUM_LEVEL,
	ID_ALPHA_LEVEL,
	ID_TARGET_COLOR,
	ID_TARGET_COLOR_LEVEL,
	ID_DRAW_TARGET,
	ID_TARGET_TOPIC_END,
	
	ID_MODE_TOPIC,
	ID_MODE,
	ID_ANGLE,
	ID_CENTER,
	ID_INNER_LENGTH,
	ID_OUTER_LENGTH,
	ID_LENGTH_RAND,
	ID_SIZE,
	ID_SIZE_RAND,
	ID_OPACITY,
	ID_OPACITY_RAND,
	ID_BLUR,
	ID_NOISE,
	ID_AUTO_SEED,
	ID_SEED,

	ID_MODE_TOPIC_END,

	ID_COLOR_TOPIC,
	ID_COLOR_COUNT,
	ID_COLOR_1,
	ID_COLOR_2,
	ID_COLOR_3,
	ID_COLOR_4,
	ID_COLOR_5,
	ID_COLOR_6,
	ID_COLOR_7,
	ID_COLOR_8,
	ID_COLOR_TOPIC_ND,

	ID_BLEND,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_TARGET_TOPIC		"target"
#define	STR_VALUE			"value"
#define	STR_THRESHOLD		"threshold"
#define	STR_THRESHOLD_ITEMS	"luminance|alpha|color"
#define	STR_THRESHOLD_COUNT	3
#define	STR_THRESHOLD_DFLT	1
#define	STR_LUM_LEVEL		"lumLevel"
#define	STR_ALPHA_LEVEL		"alphaLevel"
#define	STR_TARGET_COLOR	"targetColor"
#define	STR_TARGET_COLOR_LEVEL	"colorLevel"
#define	STR_DRAW_TARGET		"Show target mask"

#define	STR_MODE_TOPIC		"params"
#define	STR_MODE			"mode"
#define	STR_MODE_ITEMS		"straight|radial"
#define	STR_MODE_COUNT		2
#define	STR_MODE_DFLT		1

#define	STR_ANGLE			"angle"
#define	STR_CENTER			"center"

#define	STR_INNER_LENGTH	"innerLength"
#define	STR_OUTER_LENGTH	"outerLength"
#define	STR_LENGTH_RAND		"lengthRand"
#define	STR_SIZE			"size"
#define	STR_SIZE_RAND		"sizeRand"
#define	STR_OPACITY			"opacity"
#define	STR_OPACITY_RAND	"opacityRand"
#define	STR_BLUR			"blur"
#define	STR_NOISE			"noise"

#define	STR_AUTO_SEED		"autoSeed"
#define	STR_SEED			"seed"

#define	STR_COLOR_TOPIC		"colors"
#define	STR_COLOR_COUNT		"colorCount"
#define	STR_COLOR_ITEMS		"1|2|3|4|5|6|7|8"
#define	STR_COLOR			"color_"


#define	STR_BLEND			"Blend with original"


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	value;
	A_long		threshold_mode;
	PF_FpLong	lum_level;
	PF_FpLong	alpha_level;
	PF_Pixel	target_color;
	PF_FpLong	target_color_level;
	PF_Boolean	draw_target;

	A_long		 mode;
	PF_FpLong	angle;
	PF_Point	center;

	PF_FpLong	inner_length;
	PF_FpLong	outer_length;
	PF_FpLong	length_rand;
	PF_FpLong	size;
	PF_FpLong	size_rand;
	PF_FpLong	opacity;
	PF_FpLong	opacity_rand;
	A_long		blur;
	PF_FpLong	noise;

	PF_Boolean	autoSeed;
	A_long		seed;
	A_long		seedAct;
	A_long		colorCount;
	PF_Pixel	colors[8];

	PF_Boolean isBlend;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err TouchStraight(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<PF_Point>& points
);
PF_Err TouchCenter(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<PF_Point>& points
);

PF_Err ApplyANoise(NF_AE* ae, A_long count, A_long seed);

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
#endif // TouchLine_H

