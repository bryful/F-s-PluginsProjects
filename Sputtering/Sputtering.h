//-----------------------------------------------------------------------------------
/*
	Sputtering for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Sputtering_H
#define Sputtering_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\NF_Utils.h"
#include "..\_NFLib\fx\NF_SpatUtils.h"


/*
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

#include "SputteringFilter.h"
*/
#include <string>
#include <vector>

#define TARGET_COLOR_COUNT	8	//ターゲットカラーの数
#define COLOR_COUNT	8	//
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_TARGET_TOPIC,
	ID_TARGET,
	ID_TARGET_COLOR1,
	ID_TARGET_COLOR2,
	ID_TARGET_COLOR3,
	ID_TARGET_COLOR4,
	ID_TARGET_COLOR5,
	ID_TARGET_COLOR6,
	ID_TARGET_COLOR7,
	ID_TARGET_COLOR8,
	ID_BORDER_DIRECTION,
	ID_DIRECTION_ANGLE,
	ID_TARGET_TOPIC_END,

	ID_PARAMS_TOPIC,
	ID_SPUT_LAYER,
	ID_SIZE,
	ID_SIZE_RAND,
	ID_AREA_RATE,
	ID_BORDER_RATE,
	ID_SPAWN_COUNT,
	ID_SPAWN_OFFSET,

	ID_OPACITY,
	ID_OPACITY_RAND,
	ID_AUTO_SEED,
	ID_SEED,
	ID_PARAMS_TOPIC_END,

	ID_COLOR_TOPIC,
	ID_COLOR_NUM,
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
#define	STR_TARGET_TOPIC	"Target"
#define	STR_TARGET			"Target_Mode"
#define	STR_TARGET_ITEMS	"Alpha|Color1|Color2|Color3|Color4|Color5|Color6|Color7|Color8"
#define	STR_TARGET_COUNT	9
#define	STR_TARGET_DFLT		1
#define	STR_TARGET_COLOR	"Target_Color_"
#define	STR_BORDER_DIRECTION	"Border_Direction"
#define	STR_DIRECTION_ANGLE		"Direction_Angle"

#define	STR_PARAMS_TOPIC	"Parameters"
#define	STR_SPUT_LAYER		"Sputtering_Texcure_Layer"
#define	STR_SIZE			"Size"
#define	STR_SIZE_RAND		"Size_Random"
#define	STR_AREA_RATE		"Area_Rate"
#define	STR_BODER_RATE		"Border_Rate"
#define	STR_SPAWN_COUNT		"Spawn_Count"
#define	STR_SPAWN_OFFSET	"Spawn_Offset"
#define	STR_OPACITY			"Opacity"
#define	STR_OPACITY_RAND	"Opacity_random"

#define	STR_AUTO_SEED		"Auto_Seed"
#define	STR_SEED			"Seed"

#define	STR_COLORS_TOPIC	"Colors"
#define	STR_COLORS			"Color_"
#define	STR_COLORS_NUM		"Colors_Count"
#define	STR_COLORS_ITEMS	"1|2|3|4|5|6|7|8"
#define	STR_COLORS_COUNT	8
#define	STR_COLORS_DFLT		1
#define	STR_BLEND		"Blend with original"


//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	is_border_direction;
	float		direction_angle;
	A_long		targetColorCount;
	PF_Pixel	targetColors[TARGET_COLOR_COUNT];
	PF_LayerDef	sputLayer;
	float		size;
	float		sizeRandom;
	float		area_rate;
	float		border_rate;
	A_long		spawn_count;
	float		spawn_offset;
	float		opacity;
	float		opacityRand;
	PF_Boolean	autoSeed;
	A_long		seed;
	A_long		colorCount;
	PF_Pixel	colors[COLOR_COUNT];
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
#endif // Sputtering_H

