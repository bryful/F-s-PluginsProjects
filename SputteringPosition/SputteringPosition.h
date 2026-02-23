//-----------------------------------------------------------------------------------
/*
	SputteringPosition for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SputteringPosition_H
#define SputteringPosition_H

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

#include "SputteringPositionFilter.h"
*/
#include "..\_NFLib\fx\NF_Blend.h"
#include <string>
#include <vector>

#define TARGET_COLOR_COUNT	8	//ターゲットカラーの数
#define COLOR_COUNT	8	//
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE_TOPIC,
	ID_COUNT,

	ID_MODE,
	ID_CENTER,
	ID_RADIUS,
	ID_START_POINT,
	ID_END_POINT,
	ID_SCALABLE,
	ID_SCALE,
	ID_ANCHOR_POINT_ENABLED,
	ID_ANCHOR_POINT,
	ID_MODE_TOPIC_END,

	ID_PARAMS_TOPIC,
	ID_SPUT_LAYER,
	
	ID_SIZE,
	ID_SIZE_RAND,
	ID_SPUT_LOOP,
	ID_SPUT_LOOP_KOMA,
	ID_PARAMS_TOPIC_END,

	ID_PARAMS2_TOPIC,
	ID_SPAWN_COUNT,
	ID_SPAWN_OFFSET,

	ID_OPACITY,
	ID_OPACITY_RAND,
	ID_AUTO_SEED,
	ID_SEED,
	ID_PARAMS2_TOPIC_END,

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
#define	STR_MODE_TOPIC			"Mode"
#define	STR_MODE				"Style"
#define	STR_MODE_ITEMS			"Sphere|Rectangle"
#define	STR_MODE_COUNT			2
#define	STR_MODE_DFLT			1
#define	STR_CENTER				"Center"
#define	STR_RADIUS				"Radius"
#define	STR_START_POINT			"StartPoint"
#define	STR_END_POINT			"EndPoint"
#define	STR_SCALE				"Scale"
#define	STR_COUNT				"Count"
#define	STR_MODE_TOPIC			"Mode"
#define	STR_SCALABLE			"Scaleable"
#define	STR_SCALABLE_ITEMS		"None|Scale|Scale_Anchor"
#define	STR_SCALABLE_COUNT		3
#define	STR_SCALABLE_DFLT		1

#define	STR_ANCHOR_POINT_ENABLED	"AnchorPointEnabled"
#define	STR_ANCHOR_POINT		"AnchorPoint"

#define	STR_PARAMS_TOPIC	"Texture"
#define	STR_PARAMS2_TOPIC	"Parameters"
#define	STR_SPUT_LAYER		"SputteringPosition_Texcure_Layer"
#define	STR_SPUT_LOOP		"TimeSampling"
#define	STR_SPUT_LOOP_ITEMS	"Random|Loop"
#define	STR_SPUT_LOOP_COUNT	2
#define	STR_SPUT_LOOP_DFLT	1
#define	STR_SPUT_LOOP_KOMA	"koma"

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
	A_long		mode;
	PF_Point	center;
	float		radius;
	PF_Point	start_point;
	PF_Point	end_point;
	float		scale;
	PF_Point	anchor_point;
	PF_Boolean	is_scalable;
	PF_Boolean	is_anchor_point_enabled;
	A_long		count;

	float		size;
	float		sizeRandom;
	A_long		spawn_count;
	float		spawn_offset;
	float		opacity;
	float		opacityRand;
	PF_Boolean	autoSeed;
	A_long		seed;
	A_long		colorCount;
	PF_Pixel	colors[COLOR_COUNT];
	PF_Boolean	isBlend;

	A_long		sput_loop_mode;
	A_long		sput_loop_koma;
	A_long		sput_loop_seed;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err SputMain
(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<std::vector<A_u_char>>* sput
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
#endif // SputteringPosition_H

