//-----------------------------------------------------------------------------------
/*
	SpeedLines for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SpeedLines_H
#define SpeedLines_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

/*
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
*/
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_Blend.h"
//#include "..\_NFLib\NF_Utils.h"

//#include "SpeedLinesFilter.h"
#include <string>
#include <vector>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_VALUE,		
	ID_POS_TOPIC,
	ID_ANGLE,
	ID_LENGTH1_VALUE,
	ID_LENGTH1_FAR,
	ID_LENGTH1_NEAR,
	ID_LENGTH2_VALUE,
	ID_LENGTH2_FAR,
	ID_LENGTH2_NEAR,
	ID_CENTER,
	ID_AUTO_SEED,
	ID_SEED,
	ID_POS_TOPIC_END,

	ID_LINE_TOPIC,
	ID_LINE_LENGTH,
	ID_LINE_LENGTH_RAND,
	ID_LINE_SIZE,
	ID_LINE_SIZE_RAND,
	ID_LINE_OPACITY,
	ID_LINE_OPACITY_RAND,
	ID_LINE_BLUR,
	ID_LINE_NOISE,
	ID_LINE_TOPIC_END,

	ID_COLOR_TOPIC,
	ID_COLOR_COUNT,
	ID_COLOR1,
	ID_COLOR2,
	ID_COLOR3,
	ID_COLOR4,
	ID_COLOR5,
	ID_COLOR6,
	ID_COLOR7,
	ID_COLOR8,
	ID_COLOR_TOPIC_END,
	ID_BLEND,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_VALUE			"value"

#define	STR_POS_TOPIC		"position"
#define	STR_ANGLE			"angle"
#define	STR_AUTO_SEED		"autoSeed"
#define	STR_SEED			"seed"
#define	STR_LENGTH1_VALUE	"length1_value(%)"
#define	STR_LENGTH1_FAR		"length1_Far(%)"
#define	STR_LENGTH1_NEAR	"length1_Near(%)"
#define	STR_LENGTH2_VALUE	"length2_value(%)"
#define	STR_LENGTH2_FAR		"length2_Far(%)"
#define	STR_LENGTH2_NEAR	"length2_Near(%)"
#define	STR_CENTER			"center"

#define	STR_LINE_TOPIC		"line"
#define	STR_LINE_LENGTH		"length"
#define	STR_LINE_LENGTH_RAND	"lengthRand"
#define	STR_LINE_SIZE		"size"
#define	STR_LINE_SIZE_RAND	"sizeRand"
#define	STR_LINE_OPACITY	"opacity"
#define	STR_LINE_OPACITY_RAND	"opacityRand"
#define	STR_LINE_BLUR		"blur"
#define	STR_LINE_NOISE		"noise"

#define	STR_COLOR_TOPIC		"color"
#define	STR_COLOR_COUNT		"colorCount"
#define	STR_COLOR_ITEMS		"1|2|3|4|5|6|7|8"
#define	STR_COLOR			"color_"
#define	STR_BLEND			"blend with orijinal"


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	value;
	float		angle;
	PF_Boolean	is_autoSeed;
	A_long		seed;
	PF_FpLong	length1_value;
	PF_FpLong	length1_far;
	PF_FpLong	length1_near;
	PF_FpLong	length2_value;
	PF_FpLong	length2_far;
	PF_FpLong	length2_near;
	PF_Point	center;

	PF_FpLong	length;
	PF_FpLong	length_rand;
	PF_FpLong	size;
	PF_FpLong	sizeRand;
	PF_FpLong	opacity;
	PF_FpLong	opacityRand;
	A_long		blue;
	PF_FpLong	noise;
	A_long		colorCount;
	PF_Pixel	colors[8];
	PF_Boolean	isBlend;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
#include <cmath>


PF_Err SpeedLineExec(
	NF_AE* ae,
	ParamInfo* infoP
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
#endif // SpeedLines_H

