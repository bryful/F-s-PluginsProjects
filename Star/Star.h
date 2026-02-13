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
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
//#include "..\_NFLib\fx\NF_Minmax.h"
//#include "..\_NFLib\fx\NF_blur.h"
//#include "..\_NFLib\fx\debug_font.h"
//#include "..\_NFLib\fx\NF_Paint.h"
//#include "..\_NFLib\fx\NF_Noise.h"
//#include "..\_NFLib\fx\NF_Draw.h"

//#include "StarFilter.h"
#include <math.h>
#include <string>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,

	ID_ANGLE,
	ID_INTENSITY,
	ID_LENGTH,
	ID_HYPERBOLIC,

	ID_MODE_TOPIC,
	ID_COLOR_MODE,
	ID_COLOR,
	ID_THRESH_LOW,
	ID_THRESH_HIGH,
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

	PF_FpLong	thresh_low;        // 0.0-1.0
	PF_FpLong	thresh_high;       // 0.0-1.0

} ParamInfo, *ParamInfoP, **ParamInfoH;
template <typename T> struct PixelTraits;

template <> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char channel_type;
};

template <> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short channel_type;
};

template <> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort channel_type;
};
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}
typedef struct {
	A_long x;
	A_long y;
	PF_PixelFloat color; // 色と輝度情報を保持
} LitPixel;
//-------------------------------------------------------
PF_Err ExtractLumaMask(
	PF_InData* in_dataP,
	PF_EffectWorld* inputP,
	PF_EffectWorld* maskP, // 書き出し先（事前にNewWorldしたもの）
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	ParamInfo* infoP
);
PF_Err DrawStar(NF_AE* ae, PF_EffectWorld* mask_world, ParamInfo* infoP);

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

