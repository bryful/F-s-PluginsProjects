//-----------------------------------------------------------------------------------
/*
	RadialLines for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef RadialLines_H
#define RadialLines_H

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
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"


#include "RadialLinesFilter.h"
#include	<string>
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_AUTO_SEED,
	ID_SEED,

	ID_VALUE,
	
	ID_POS,
	ID_LENGTH,
	ID_LENGTH_RAND,

	ID_WEIGHT,
	ID_WEIGHT_RAND,


	ID_OPACITY,
	ID_OPACITY_RAND,

	ID_HYPERBOLIC,
	ID_NOISE,

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
	ID_COLOR_END,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_AUTO_SEED		"AutoSeed"
#define	STR_SEED			"Seed"
#define	STR_VALUE			"Value"

#define	STR_POS				"CenterPos"
#define	STR_LENGTH			"Length"
#define	STR_LENGTH_RAND		"LengthRand"
#define	STR_WEIGHT			"Weight"
#define	STR_WEIGHT_RAND		"WeightRand"

#define	STR_OPACITY			"Opacity"
#define	STR_OPACITY_RAND	"OpacityRand"
#define	STR_COLOR_TOPIC		"Colors"
#define	STR_COLOR_COUNT		"ColorCount"
#define	STR_COLOR			"Color"

#define	STR_NOISE			"Noise"
#define	STR_HYPERBOLIC		"Hyperboilc"
//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	auto_seed;
	A_long			seed;
	A_long			value;
	A_long			values[4];
	A_long			colorCount;
	PF_Pixel		colors[8];
	A_FloatPoint	cente_pos;
	PF_FpLong		length_par;
	PF_FpLong		length_rand;
	PF_FpLong		weight;
	PF_FpLong		weight_rand;
	PF_FpLong		opacity_par;
	PF_FpLong		opacity_rand;
	PF_FpLong		noise;
	PF_FpLong		hyperbolic;



} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err Exec8_16_32(
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
#endif // RadialLines_H

