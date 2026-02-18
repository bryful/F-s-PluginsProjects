//-----------------------------------------------------------------------------------
/*
	CellEmboss for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef CellEmboss_H
#define CellEmboss_H

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
#include "NF_SelectPixels.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\NF_Blend.h"
#include <string>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_COUNT,
	ID_COLOR_TOPIC,
	ID_COLOR0,
	ID_COLOR1,
	ID_COLOR2,
	ID_COLOR3,
	ID_COLOR4,
	ID_COLOR5,
	ID_COLOR6,
	ID_COLOR7,
	//ID_WHITE,
	ID_COLOR_TOPIC_END,

	ID_DIRECTION,

	ID_MODE,

	ID_HI_TOPIC,
	ID_DISTANCE_HI,
	ID_BLUR_HI,
	ID_COLOR_HI,
	ID_HI_TOPIC_END,

	ID_LO_TOPIC,
	ID_DISTANCE_LO,
	ID_BLUR_LO,
	ID_COLOR_LO,
	ID_LO_TOPIC_END,
	ID_BLEND,
	ID_NUM_PARAMS
};
#define ID_COLOR(IDX)		(ID_COLOR0 + (IDX))
//UIの表示文字列
#define	STR_COUNT			"colorCount"
#define	STR_COUNT_ITEMS		"A-On|A-OnWithoutWhite|1|2|3|4|5|6|7|8"
#define STR_COLOR_TOPIC		"TargetColors"
#define	STR_DIRECTION		"direction"
#define	STR_MODE			"mode"
#define	STR_MODE_ITEMS		"Hi|Lo|Hi-Reverce|Hi+Lo"
//#define	STR_WHITE			"Treat White as Transparent"

#define	STR_HI_TOPIC		"Hi"
#define	STR_DISTANCE_HI		"distance_Hi"
#define	STR_BLUR_HI			"blue_Hi"
#define	STR_COLOR_HI		"color_Hi"

#define	STR_LO_TOPIC		"Lo"
#define	STR_DISTANCE_LO		"distance_Lo"
#define	STR_BLUR_LO			"blue_Lo"
#define	STR_COLOR_LO		"color_Lo"
#define	STR_BLEND			"Blend with original"


//UIのパラメータ
typedef struct ParamInfo {
	A_long		count;
	A_long		mode;
	PF_Pixel	colors[8];
	//PF_Boolean	isWhite;
	PF_Boolean	isBlend;
	PF_FpLong	direction;
	PF_FpLong	distanceHI;
	PF_FpLong	distanceLO;
	A_long		blurHI;
	A_long		blurLO;
	PF_Pixel	colorHi;
	PF_Pixel	colorLo;
	PF_Pixel16	colorHi16;
	PF_Pixel16	colorLo16;
	PF_PixelFloat	colorHi32;
	PF_PixelFloat	colorLo32;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err EmbossHiFilter(
	NF_AE* ae,
	ParamInfo* infoP
);
PF_Err EmbossColoriseFilter(
	NF_AE* ae,
	ParamInfo* infoP
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
#endif // CellEmboss_H

