//-----------------------------------------------------------------------------------
/*
	Star for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Star_H
#define Star_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
//#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
//#include "..\_NFLib\fx\NF_Minmax.h"
//#include "..\_NFLib\fx\NF_blur.h"
//#include "..\_NFLib\fx\debug_font.h"
//#include "..\_NFLib\fx\NF_Paint.h"
//#include "..\_NFLib\fx\NF_Noise.h"
//#include "..\_NFLib\fx\NF_Draw.h"

#include <math.h>
#include <string>
#include <vector>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,

	ID_MODE_TOPIC,
	ID_ANGLE,
	ID_LENGTH,
	ID_INTENSITY,
	ID_HYPERBOLIC,

	ID_COLOR_MODE,
	ID_COLOR,
	ID_MODE_TOPIC_END,

	// 各ラインの設定 (4方向分)
	ID_LINE1_TOPIC,
	ID_LINE1_LEN, 
	ID_LINE1_INT, 
	ID_LINE1_ANGLE,
	ID_LINE1_TOPIC_END,

	ID_LINE2_TOPIC,
	ID_LINE2_LEN,
	ID_LINE2_INT, 
	ID_LINE2_ANGLE,
	ID_LINE2_TOPIC_END,

	ID_LINE3_TOPIC,
	ID_LINE3_LEN,
	ID_LINE3_INT, 
	ID_LINE3_ANGLE,
	ID_LINE3_TOPIC_END,

	ID_LINE4_TOPIC,
	ID_LINE4_LEN,
	ID_LINE4_INT, 
	ID_LINE4_ANGLE,
	ID_LINE4_TOPIC_END,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_ANGLE			"Overall Angle"
#define	STR_INTENSITY		"Overall Intensity"
#define	STR_LENGTH			"Overall Length (px)"

#define	STR_HYPERBOLIC		"hyperbolic"

#define	STR_MODE_TOPIC		"Params"


#define	STR_COLOR_MODE		"Color Mode"
#define	STR_COLOR_MODE_ITEMS "Original|CustomColor"
#define	STR_COLOR			"CustomColor"
#define	STR_THRESH_LOW		"Threshold low"
#define	STR_THRESH_HIGH		"Threshold High"

#define	STR_LINE1_TOPIC		"Line 1 (Vertical)"
#define	STR_LINE2_TOPIC		"Line 2 (Diagonal 1)"
#define	STR_LINE3_TOPIC		"Line 3 (Horizontal)"
#define	STR_LINE4_TOPIC		"Line 4 (Diagonal 2)"

#define	STR_LINE_LEN		"Length_"
#define	STR_LINE_INT		"Intensity_"
#define	STR_LINE_ANGLE		"AngleOffset_"


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	overall_angle;     // ラジアン
	PF_FpLong	overall_intensity; // 0.0-1.0
	PF_FpLong	overall_length;    // ピクセル
	PF_FpLong	hyperbolic;        // ハイパーボリック強度
	A_long		color_mode;        // 1: Original, 2: Custom
	PF_PixelFloat cross_color;     // 0.0-1.0 normalized

	struct {
		PF_FpLong	len_per;       // 0.0-1.0
		PF_FpLong	int_per;       // 0.0-1.0
		PF_FpLong	angle_offset;  // ラジアン
	} lines[4];

	//PF_FpLong	thresh_low;        // 0.0-1.0
	//PF_FpLong	thresh_high;       // 0.0-1.0

} ParamInfo, *ParamInfoP, **ParamInfoH;


struct StarSource {
	int x, y;
	float brightness;
};
//-------------------------------------------------------
PF_Err ExtractMask(
	PF_InData* in_dataP,
	PF_EffectWorld* inputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask,
	A_long mode
);
PF_Err DrawMask(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask
);

std::vector<std::vector<float>> CalcMask(
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* src
);
std::vector<StarSource> ExtractStarSources(
	std::vector<std::vector<float>>& mask,
	A_long* count
);
PF_Err StarMain(
	NF_AE* ae,
	std::vector<StarSource> mask,
	ParamInfo* infoP
);
PF_Err StarBlend(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<std::vector<float>>* mask
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
#endif // Star_H

