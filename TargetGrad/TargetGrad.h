//-----------------------------------------------------------------------------------
/*
	TargetGrad for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef TargetGrad_H
#define TargetGrad_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
//#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\NF_Settings.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawWorld.h"
/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
*/
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

#include "RotPoint.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_TARGET,

	ID_TOPIC_COLOR,
	ID_TCOLOR1_ENABLED,
	ID_TCOLOR1,
	ID_TCOLOR2_ENABLED,
	ID_TCOLOR2,
	ID_TCOLOR3_ENABLED,
	ID_TCOLOR3,
	ID_TCOLOR4_ENABLED,
	ID_TCOLOR4,
	ID_TCOLOR5_ENABLED,
	ID_TCOLOR5,
	ID_TCOLOR6_ENABLED,
	ID_TCOLOR6,
	ID_TCOLOR7_ENABLED,
	ID_TCOLOR7,
	ID_TCOLOR8_ENABLED,
	ID_TCOLOR8,

	ID_TOPIC_TCOLOR_END,

	ID_GRAD_COLOR,
	ID_INVERT,
	ID_HYPERBOLIC,

	ID_TOPIC_2POINT,
	ID_START_POS,
	ID_LAST_POS,
	ID_TOPIC_2POINT_END,

	ID_AUTO_POS,
	ID_ROT,

	ID_START_PER,
	ID_LAST_PER,

	ID_OFFSET_X,
	ID_OFFSET_Y,

	ID_GUIDE_ENABLED,
	ID_GUIDE_COLOR,

	ID_LOAD_BTN,
	ID_SAVE_BTN,

	ID_NUM_PARAMS
};

#define ID_COLOR(IDX) (ID_TCOLOR1 + (IDX) * 2)
#define ID_COLOR_ENABLED(IDX) (ID_TCOLOR1_ENABLED + (IDX) * 2)

//UIの表示文字列
#define	STR_TARGET			"target"
#define	STR_TARGET_ITEMS	"targetColors|alphaOn|all"
#define	STR_TARGET_COUNT	3
#define	STR_TARGET_DFLT		1

#define	STR_TOPIC_COLOR		"targetColors"
#define	STR_TARGET			"target"
#define	STR_TARGET_CB		"targetEnabled"
#define	STR_TARGET_CB2		"on"

#define	STR_GRADCOLOR		"gradColor"
#define	STR_INVERT			"swapPoint"
#define	STR_INVERT2			"on"

#define	STR_HYPERBOLIC		"hyperbolic"

#define	STR_AUTO_POS		"autoPos"
#define	STR_AUTO_POS2		"on"

#define	STR_TOPIC_2POINT	"2Point"
#define	STR_START			"start"
#define	STR_LAST			"last"

#define	STR_ROT				"rot"

#define	STR_START_PER		"startPercent"
#define	STR_LAST_PER		"lastPercent"

#define	STR_OFFSET_X		"offsetX"
#define	STR_OFFSET_Y		"offsetY"


#define	STR_LAOD_CAP		"colorTable"
#define	STR_LOAD_BTN		"load"
#define	STR_SAVE_BTN		"save"

#define	STR_GUIDE_ENABLED	"guideDraw"
#define	STR_GUIDE_ENABLED2	"on"
#define	STR_GUIDE_COLOR		"guideColor"

//UIのパラメータ
#define COLOR_TABLE_COUNT	8

//UIのパラメータ
typedef struct ParamInfo {
	A_long			targetColorMode;
	PF_Pixel8		targetColors[COLOR_TABLE_COUNT];
	A_long			targetColorCount;
	PF_Pixel8		gradColor;
	PF_FpLong		hyperbolic;

	PF_Boolean		autoPos;

	A_FloatPoint	startPos;
	A_FloatPoint	lastPos;

	A_FpLong		rot;

	PF_FpLong		startPercent;
	PF_FpLong		lastPercent;

	A_FloatPoint	startPos2;
	A_FloatPoint	lastPos2;
	A_FloatPoint	startPos2D;
	A_FloatPoint	lastPos2D;

	A_FpLong		offsetX;
	A_FpLong		offsetY;

	PF_Boolean		invert;
	PF_Boolean		guideEnabled;
	PF_Pixel		guideColor;
	PF_Rect			area;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
//保存用データ構造体
typedef struct PixelTable {
	A_long			targetColorMode;
	PF_Boolean		targetEnabled[COLOR_TABLE_COUNT];
	PF_Pixel8		target[COLOR_TABLE_COUNT];
	PF_Pixel8		gradColor;
	PF_Boolean		invert;
	PF_Pixel		guideColor;
	PF_Boolean		guideEnabled;
	bool			ok;
} PixelTable;
//-------------------------------------------------------
/*
enum {
	WRITE_CHANNEL_ALPHA = 1 << 0,
	WRITE_CHANNEL_RED = 1 << 1,
	WRITE_CHANNEL_GREEN = 1 << 2,
	WRITE_CHANNEL_BLUE = 1 << 3,
};
*/
//#define WRITE_CHANNEL_ALL (WRITE_CHANNEL_RED | WRITE_CHANNEL_GREEN | WRITE_CHANNEL_BLUE | WRITE_CHANNEL_ALPHA)
PF_Err ExtractColor(NF_AE* ae, ParamInfo* info);
struct MaskInfo {
	A_FloatPoint p1;
	A_FpShort dx;
	A_FpShort dy;
	A_FpShort inv_len_sq;
	A_long writeChannel; // 書き込みチャンネル指定
	PF_Pixel grad8;
	PF_Pixel16 grad16;
	PF_PixelFloat grad32;
	//PF_Boolean invert; // 白→黒 か 黒→白 か
	PF_FpLong		hyperbolic;
	PF_Boolean isAll;
	float			origin_x;
	float			origin_y;
	float step_x;           // den / num (ダウンサンプル補正)
	float step_y;
};
PF_Err TargetGradMain(
	NF_AE* ae,
	ParamInfo* infoP,
	PF_EffectWorld* output);
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
#endif // TargetGrad_H

