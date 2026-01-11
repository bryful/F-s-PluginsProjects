//-----------------------------------------------------------------------------------
/*
	TargetGrad for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef TargetGrad_H
#define TargetGrad_H

#include "../FsLibrary/Fs.h"
#include "Fs_Target.h"
#include "../FsLibrary/FsAE.h"
#include "../FsLibrary/tinyfiledialogs.h"
#include "../FsLibrary/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "FsSettings.h"
#include "RotPoint.h"

using json = nlohmann::json;

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
#define	STR_TARGET_ITEMS	"targetColors|alphaOn"
#define	STR_TARGET_COUNT	2
#define	STR_TARGET_DFLT		1

#define	STR_TOPIC_COLOR		"targetColors"
#define	STR_TARGET			"target"
#define	STR_TARGET_CB		"targetEnabled"
#define	STR_TARGET_CB2		"on"

#define	STR_GRADCOLOR		"gradColor"
#define	STR_INVERT			"invert"
#define	STR_INVERT2			"on"

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
typedef struct ParamInfo {
	A_long			targetColorMode;
	PF_Pixel8		targetColors[COLOR_TABLE_COUNT];
	A_long			targetColorCount;
	PF_Pixel8		gradColor;

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
enum {
	WRITE_CHANNEL_ALPHA = 1 << 0,
	WRITE_CHANNEL_RED = 1 << 1,
	WRITE_CHANNEL_GREEN = 1 << 2,
	WRITE_CHANNEL_BLUE = 1 << 3,
};
#define WRITE_CHANNEL_ALL (WRITE_CHANNEL_RED | WRITE_CHANNEL_GREEN | WRITE_CHANNEL_BLUE | WRITE_CHANNEL_ALPHA)

struct MaskInfo {
	A_FloatPoint p1;
	A_FpShort dx;
	A_FpShort dy;
	A_FpShort inv_len_sq;
	A_long writeChannel; // 書き込みチャンネル指定
	PF_Pixel grad8;
	PF_Pixel16 grad16;
	PF_PixelFloat grad32;
	PF_Boolean invert; // 白→黒 か 黒→白 か
	//PF_Point	topLeft;
	//PF_Point	bottomRight;
};
PF_Err RenderSimpleMask(
	CFsAE* ae,
	ParamInfo* infoP,
	PF_EffectWorld* output);
// -----------------------------------------------------------------------------------
typedef struct {
	PF_Pixel8 match_colors[8]; // 最大8色の判定用リスト
	A_long color_count;        // 実際の色数
} ColorFilterInfo;

PF_Err ExtractColor(CFsAE* ae, ParamInfo* info);

// -----------------------------------------------------------------------------------



static std::string SaveJsonFileDialog(std::string title, std::string defp)
{
	const char* filterPatterns[] = { "*.tgj" };
	const char* selectedFile = tinyfd_saveFileDialog(
		title.c_str(),                      // ダイアログのタイトル
		defp.c_str(),                       // 初期ディレクトリ
		1,                          // フィルタパターンの数
		filterPatterns,             // フィルタパターン
		"tgj files" // フィルタの説明
	);
	std::string ret;
	if (selectedFile)
	{
		ret = std::string(selectedFile);
	}

	return ret;
}
static std::string OpenJsonFileDialog(std::string title, std::string defp)
{
	const char* filterPatterns[] = { "*.tgj" };
	const char* selectedFile = tinyfd_openFileDialog(
		title.c_str(),                      // ダイアログのタイトル
		defp.c_str(),                       // 初期ディレクトリ
		1,                          // フィルタパターンの数
		filterPatterns,             // フィルタパターン
		"tgj files", // フィルタの説明
		0                           // マルチセレクトの可否 (0 = No, 1 = Yes)
	);
	std::string ret;
	if (selectedFile)
	{
		ret = std::string(selectedFile);
	}

	return ret;
}

static inline A_FloatPoint PointFromCenterAngle(
	A_FloatPoint center,
	double r_deg,           // 角度 (度)
	double len,             // 基準距離
	double scalePercent     // 補正値 (%)。負は反対方向 (角度+180)
)
{
	const double MIN_SCALE = -500.0;
	const double MAX_SCALE = 500.0;
	const double PI = 3.14159265358979323846;

	// clamp
	if (scalePercent < MIN_SCALE) scalePercent = MIN_SCALE;
	if (scalePercent > MAX_SCALE) scalePercent = MAX_SCALE;

	A_FloatPoint out;
	out.x = center.x;
	out.y = center.y;

	// 0% は中心点
	if (scalePercent == 0.0 || len == 0.0) {
		return out;
	}

	bool isNegative = (scalePercent < 0.0);
	double absScale = isNegative ? -scalePercent : scalePercent;

	// 距離を計算（percent -> ratio）
	double ratio = absScale / 100.0;
	double dist = len * ratio;

	// 負なら角度を反転（+180度）
	double deg = r_deg + (isNegative ? 180.0 : 0.0);

	double rad = deg * (PI / 180.0);

	out.x = center.x + (A_FpLong)(cos(rad) * dist);
	out.y = center.y + (A_FpLong)(sin(rad) * dist);

	return out;
}
static inline A_FloatPoint PointOnSegmentFraction(const A_FloatPoint& p1, const A_FloatPoint& p2, float frac)
{
	//if (frac < 0.0f) frac = 0.0f;
	//if (frac > 1.0f) frac = 1.0f;
	A_FloatPoint out;
	// 精度を保つため double 演算を使用してから型を戻す
	out.x = static_cast<A_FpLong>(static_cast<double>(p1.x) + (static_cast<double>(p2.x) - static_cast<double>(p1.x)) * static_cast<double>(frac));
	out.y = static_cast<A_FpLong>(static_cast<double>(p1.y) + (static_cast<double>(p2.y) - static_cast<double>(p1.y)) * static_cast<double>(frac));
	return out;
}

// パーセンテージ（0〜100）指定版のラッパー
static inline A_FloatPoint PointOnSegmentPercent(const A_FloatPoint& p1, const A_FloatPoint& p2, float percent)
{
	return PointOnSegmentFraction(p1, p2, percent / 100.0f);
}

//-----------------------------------------------------------------------------------
extern "C" {

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
