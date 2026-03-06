//-----------------------------------------------------------------------------------
/*
	BurstLines for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef BurstLines_H
#define BurstLines_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
//#include "..\_NFLib\json.hpp"
//#include "..\_NFLib\NF_Json.h"
#include "..\_NFLib\NF_Utils.h"

/*
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
#include "..\_NFLib\fx\NF_Bilateral.h"
#include "..\_NFLib\fx\NF_Blend.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawWorld.h"
#include "..\_NFLib\fx\NF_Gradient.h"
#include "..\_NFLib\fx\NF_Hyperbolic.h"
#include "..\_NFLib\fx\NF_LogicGray.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_Mosaic.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_SpatUtils.h"
#include "..\_NFLib\fx\NF_UnMult.h"

#include "..\_NFLib\vector\NF_VectorLine.h"
#include "..\_NFLib\vector\NF_VectorMask.h"
*/
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_Mult.h"

#include <string>
#include <vector>


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_LINE_CENTER,
	ID_LINE_COUNT,
	ID_DISTANCE,
	ID_LENGTH,
	// -----
	ID_PARAMS_TOIPC,
	ID_LINE_SUB_COUNT,
	ID_LINE_SUB_ANGLE,
	ID_DISTANCE_RAND,
	ID_LENGTH_RAND,
	ID_THICKNESS,
	ID_THICKNESS_RAND,

	ID_THICKNESS_START,
	ID_THICKNESS_END,
	ID_OPACITY_START,
	ID_OPACITY_END,

	ID_DIRECTION,
	ID_ANGLE_RANGE,
	ID_PARAMS_TOIPC_END,
	// -----
	ID_COLOR_TOIPC,
	ID_COLOR_COUNT,
	ID_COLOR_1,
	ID_COLOR_2,
	ID_COLOR_3,
	ID_COLOR_4,
	ID_COLOR_5,
	ID_COLOR_6,
	ID_COLOR_7,
	ID_COLOR_8,
	ID_COLOR_TOIPC_END,
	// -----
	ID_AUTO_SEED,		//ノイズのシードを自動で変化させるかどうかのチェックボックス
	ID_SEED,			//ノイズのシード値の数値入力
	// -----


	
	ID_NUM_PARAMS
};

//UIの表示文字列

//-------
#define	STR_LINE_CENTER		"center"
#define	STR_LINE_COUNT		"lineCount"
#define	STR_DISTANCE		"distance"
#define	STR_LENGTH			"length"

#define	STR_PARAMS_TOIPC	"params"
#define	STR_LINE_SUB_COUNT	"subCount"
#define	STR_LINE_SUB_ANGLE	"subAngle"
#define	STR_DISTANCE_RAND	"distanceRand"
#define	STR_LENGTH_RAND		"lengthRand"
#define	STR_THICKNESS		"thickness"
#define	STR_THICKNESS_RAND	"thicknessRand"

#define	STR_THICKNESS_START		"thick_start"
#define	STR_THICKNESS_END		"thick_end"

#define	STR_OPACITY_START	"opacity_start"
#define	STR_OPACITY_END		"opacity_end"

#define	STR_DIRECTION		"direction"
#define	STR_ANGLE_RANGE		"angleRange"
//-------
#define	STR_COLOR_TOIPC		"colors"
#define	STR_COLOR_CNT		"colorCount"
#define	STR_COLOR_CNT_NUM	8
#define	STR_COLOR_CNT_DFT	1
#define	STR_COLOR_CNT_ITEMS	"1|2|3|4|5|6|7|8"
#define	STR_COLOR			"color_"



//UIのパラメータ
typedef struct ParamInfo {
	A_FloatPoint	center;
	A_long		lineCount;
	A_long		lineSubCount;
	float		lineSubAngle;
	float		distance;
	float		distanceRand;
	float		length;
	float		lengthRand;
	float		thickness;
	float		thicknessRand;
	float		thick_start;
	float		thick_end;
	float		opacity_start;
	float		opacity_end;

	float		direction;
	float		angleRange;
	A_long		colorCount;
	PF_Pixel	colors[8];

	PF_Boolean	auto_seed;
	A_long		seed;
	A_long		seedAct;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err BurstLinesExec(
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
#endif // BurstLines_H

