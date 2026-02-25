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

#include "TouchLineFilter.h"
*/
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
#define	STR_BLEND			"Blend with original"


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong value;
	A_long	threshold_mode;
	PF_FpLong lum_level;
	PF_FpLong alpha_level;
	PF_Pixel target_color;
	PF_FpLong target_color_level;
	PF_Boolean draw_target;
	PF_Boolean isBlend;

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
#endif // TouchLine_H

