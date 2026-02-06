//-----------------------------------------------------------------------------------
/*
	SelectiveColorBlur for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SelectiveColorBlur_H
#define SelectiveColorBlur_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"

#include <string>
#include <vector>
#include <cmath>

#define COLOR_COUNT 8
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	//以下参考用
	ID_BLUR_VALUE,
	//ID_TARGET_RANGE,

	ID_TOPIC_COLOR,

	ID_TARGET_ENABLED0,
	ID_TARGET_COL0,

	ID_TARGET_ENABLED1,
	ID_TARGET_COL1,

	ID_TARGET_ENABLED2,
	ID_TARGET_COL2,

	ID_TARGET_ENABLED3,
	ID_TARGET_COL3,

	ID_TARGET_ENABLED4,
	ID_TARGET_COL4,

	ID_TARGET_ENABLED5,
	ID_TARGET_COL5,

	ID_TARGET_ENABLED6,
	ID_TARGET_COL6,

	ID_TARGET_ENABLED7,
	ID_TARGET_COL7,

};
#define ID_TARGET_ENABLED(IDX) (ID_TARGET_ENABLED0+(IDX*2))
#define ID_TARGET_COL(IDX) (ID_TARGET_COL0+(IDX*2))
enum {
	ID_TOPIC_COLOR_END=19,
	ID_NUM_PARAMS
};
//UIの表示文字列

#define	STR_BLUR_VALUE	"blur"
#define	STR_TOPIC_COLOR	"TargetColors"
#define	STR_TARGET_ENABLE	"TurnOn"
#define	STR_TARGET_ENABLE1	"on"

#define	STR_TARGET_COL	"targetColor"

#define	STR_TARGET_COL1	"target1"

#define	STR_TARGET_COL0	"target0"
#define	STR_TARGET_COL1	"target1"
#define	STR_TARGET_COL2	"target2"
#define	STR_TARGET_COL3	"target3"
#define	STR_TARGET_COL4	"target4"
#define	STR_TARGET_COL5	"target5"
#define	STR_TARGET_COL6	"target6"
#define	STR_TARGET_COL7	"target7"


//UIのパラメータ
typedef struct ParamInfo {
	A_long			blur_value;
	A_long			target_count;
	PF_Pixel		target_col[COLOR_COUNT];
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
typedef struct blurPrm
{
	//PF_InData*		in_data;
	AEGP_SuiteHandler* suitesP;
	PF_PixelPtr		data;		// 出力先バッファへのポインタ
	A_long			width;		// 処理幅
	A_long			rowbytes;	// 実際のrowbytes（ピクセル単位）
	A_long			height;		// 処理高さ
	A_long			blur_value;	// ぼかし半径
	PF_PixelFormat	pixelFormat;// ピクセルフォーマット
}blurPrm;
PF_Err BlurSub(blurPrm* prm);
PF_Err SelectColorCopy(NF_AE* ae, ParamInfo* infoP);
PF_Err CombineInput(NF_AE* ae);
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
#endif // SelectiveColorBlur_H

