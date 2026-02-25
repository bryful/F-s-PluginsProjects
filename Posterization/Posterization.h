//-----------------------------------------------------------------------------------
/*
	Posterization for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Posterization_H
#define Posterization_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Bilateral.h"
/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
*/
#include "PosterizationFilter.h"
#include <string>

#define COLOR_TABLE_MAX	15
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_LEVEL,
	ID_MODE,
	ID_IS_COLOR,
	ID_COLOR_TOPIC,
	ID_COLOR_1,
	ID_COLOR_2,
	ID_COLOR_3,
	ID_COLOR_4,
	ID_COLOR_5,
	ID_COLOR_6,
	ID_COLOR_7,
	ID_COLOR_8,
	ID_COLOR_9,
	ID_COLOR_10,
	ID_COLOR_11,
	ID_COLOR_12,
	ID_COLOR_13,
	ID_COLOR_14,
	ID_COLOR_15,
	ID_BTN_TWEEN2,
	ID_BTN_TWEEN3,
	ID_COLOR_TOPIC_END,

	ID_PRE_TOPIC,
	ID_BILATERAL_CB,
	ID_B_RADIUS,
	ID_B_SIGMA_S,
	ID_B_SIGMA_R,
	ID_PRE_TOPIC_END,
	ID_BORDER_TOPIC,
	ID_BORDER_INNER,
	ID_BORDER_OUTER,
	ID_BORDER_COLOR,
	ID_BORDER_TOPIC_END,

	ID_NUM_PARAMS
};

//UIの表示文字列
const A_u_char LEVEL_TBL[] = {0,3,5,7,9,11,13,15};
#define	STR_LEVEL		"level"
#define	STR_LEVEL_ITEMS	"3|5|7|9|11|13|15"
#define	STR_LEVEL_COUNT	7
#define	STR_LEVEL_DFLT	3
#define	STR_MODE		"Mode"
#define	STR_MODE_ITEMS	"Posterization|With Border|Border Only"
#define	STR_MODE_COUNT	3
#define	STR_MODE_DFLT	1

#define	STR_IS_COLOR	"Colorise"
#define	STR_COLOR_TOPIC	"Colors"
#define	STR_COLOR		"Color"
#define	STR_BTN_TWEEN2	"Tween 2"
#define	STR_BTN_TWEEN2B	"Start to End"
#define	STR_BTN_TWEEN3	"Tween 3"
#define	STR_BTN_TWEEN3B	"Start, Mid, End"
#define	STR_BORDER_TOPIC	"Border"
#define	STR_BORDER_INNER	"InnerSize"
#define	STR_BORDER_OUTER	"OuterSize"
#define	STR_BORDER_COLOR	"BorderColor"

#define	STR_PRE_TOPIC		"Pre-processing"
#define STR_BILATERAL_CB	"Bilateral"
#define STR_B_RADIUS		"Radius"
#define STR_B_SIGMA_S		"Sigma_Spatial"
#define STR_B_SIGMA_R		"Sigma_Range"




//UIのパラメータ
typedef struct ParamInfo {
	A_long level;
	A_long mode;
	PF_Boolean is_color;
	PF_Pixel colors[COLOR_TABLE_MAX];
	A_long border_inner;
	A_long border_outer;
	PF_Pixel border_color;

	PF_Boolean  use_bilateral;
	int radius;
	float sigma_s;
	float sigma_r;
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
#endif // Posterization_H

