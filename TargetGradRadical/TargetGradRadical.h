//-----------------------------------------------------------------------------------
/*
	TargetGradRadical for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef TargetGradRadical_H
#define TargetGradRadical_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
//#include "..\_NFLib\NF_ParamsSetup.h"

/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
*/
#include "..\_NFLib\NF_Settings.h"
#include "..\_NFLib\fx\NF_Blend.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

//#include "RotPoint.h"
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

	ID_CENTER_POS,

	ID_BLUR,
	ID_FEATHER,
	ID_HYPERBOLIC,

	ID_ASPECT,
	ID_ANGLE,

	ID_LOAD_BTN,
	ID_SAVE_BTN,
	ID_BLEND,
	ID_NUM_PARAMS
};
#define ID_COLOR(IDX) (ID_TCOLOR1 + (IDX) * 2)
#define ID_COLOR_ENABLED(IDX) (ID_TCOLOR1_ENABLED + (IDX) * 2)

//UIの表示文字列
#define	STR_TARGET			"target"
#define	STR_TARGET_ITEMS	"targetColors|alphaOn|all"
#define	STR_TARGET_COUNT	3
#define	STR_TARGET_DFLT		3

#define	STR_TOPIC_COLOR		"targetColors"
#define	STR_TARGET			"target"
#define	STR_TARGET_CB		"targetEnabled"
#define	STR_TARGET_CB2		"on"

#define	STR_GRADCOLOR		"gradColor"
#define	STR_INVERT			"invert"
#define	STR_INVERT2			"on"


#define	STR_CENTER			"center"

#define	STR_BLUR			"radius"
#define	STR_HYPERBOLIC		"hyperbolic"

#define	STR_ASPECT			"aspect"

#define	STR_ANGLE			"angle"

#define	STR_FEATHER			"feather"


#define	STR_LAOD_CAP		"colorTable"
#define	STR_LOAD_BTN		"load"
#define	STR_SAVE_BTN		"save"
#define	STR_BLEND			"Blend with original"

#define COLOR_TABLE_COUNT	8

//UIのパラメータ
typedef struct ParamInfo {
	A_long			targetColorMode;
	PF_Pixel8		targetColors[COLOR_TABLE_COUNT];
	A_long			targetColorCount;
	PF_Pixel8		gradColor;

	A_FloatPoint	cenertPos;

	PF_FpLong		blur;
	PF_FpLong		hyperbolic;
	PF_FpLong		angle;

	PF_FpLong		aspect;
	PF_FpLong		feather;

	PF_Boolean		invert;
	PF_Boolean		isBlend;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
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
struct RadialMaskInfo {
	A_FloatPoint center_p;  // 中心座標
	float radius;           // 半径
	float inv_aspect;       // 1.0 / aspect_ratio
	float cos_q;            // 回転計算用 cos(-angle)
	float sin_q;            // 回転計算用 sin(-angle)
	PF_Boolean invert;      // 反転フラグ
	float feather;          // ぼかし幅 (0.0〜1.0)
	PF_Pixel8 grad8;        // グラデーションカラー
	PF_Pixel16 grad16;      // グラデーションカラー
	PF_Pixel32 grad32;      // グラデーションカラー
	PF_Boolean isAll;		// 全画面対応
	float hyperbolic;
};
// -----------------------------------------------------------------------------------
typedef struct {
	PF_Pixel8 match_colors[8]; // 最大8色の判定用リスト
	A_long color_count;        // 実際の色数
} ColorFilterInfo;

PF_Err ExtractColor(NF_AE* ae, ParamInfo* info);
PF_Err RenderTargetGradRadial(
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
#endif // TargetGradRadical_H

