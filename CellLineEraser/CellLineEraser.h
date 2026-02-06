//-----------------------------------------------------------------------------------
/*
	CellLineEraser for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef CellLineEraser_H
#define CellLineEraser_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include <atomic>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_Color_COUNT,
	ID_Color1, ID_Color2, ID_Color3, ID_Color4, ID_Color5,
	ID_Color6, ID_Color7, ID_Color8, ID_Color9, ID_Color10,
	ID_KEEP_PIXELS,
	ID_FillUnknownColors,
	ID_Fill_Color,
	ID_MakeWhiteTransparent,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_COLOR_COUNT					"TargetColorCount"
#define	STR_Color1				"color1"
#define	STR_Color2				"color2"
#define	STR_Color3				"color3"
#define	STR_Color4				"color4"
#define	STR_Color5				"color5"
#define	STR_Color6				"color6"
#define	STR_Color7				"color7"
#define	STR_Color8				"color8"
#define	STR_Color9				"color9"
#define	STR_Color10				"color10"
#define	STR_KEEP_PIXELS			"KeepPixels"
#define	STR_FillUnknownColors	"Fill unremoved pixels"
#define	STR_FillColor			"FillColor"
#define	STR_MakeWhiteTransparent	"Make White Transparent"

//UIのパラメータ
typedef struct ParamInfo {
	A_long              target_count;
	PF_Pixel8           targets[10];      // 比較基準（常に8bit）
	PF_EffectWorld* src_world;        // 反復処理用参照
	std::atomic<long>* pixels_changed;   // 収束判定用
	PF_Boolean          KeepPixel;
	PF_Boolean          FillUnknownColors;
	PF_Pixel8           GiveUpColor8;
	PF_Pixel16          GiveUpColor16;
	PF_PixelFloat       GiveUpColor32;
	PF_Boolean          MakeWhiteTransparent;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
/* 関数宣言 */
PF_Err CellLineEraserSub(NF_AE* ae, ParamInfo* infoP);


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
#endif // CellLineEraser_H

