//-----------------------------------------------------------------------------------
/*
	RimFill for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef RimFill_H
#define RimFill_H

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
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_WIDTH,
	ID_MODE,
	ID_CUSTOMCOLOR,
	ID_WHITE,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_WIDTH			"width"
#define	STR_MODE			"FillMethod"
#define	STR_MODE_ITEMS		"CustomColor|AdjacentColor"
#define	STR_MODE_COUNT		2
#define	STR_MODE_DFLT		1
#define	STR_CUSTOMCOLOR		"CustomColor"
#define	STR_WHITE		"Treat White as Transparent"


//UIのパラメータ
typedef struct ParamInfo {
	A_long			lineWidth;
	PF_Boolean		isWhite;
	PF_Boolean		isCustomColor;
	PF_Pixel8		customColor;
	PF_Pixel16		customColor16;
	PF_PixelFloat	customColor32;
	PF_InData* in_data;
	PF_EffectWorld* dst;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------

PF_Err RimFill_Sub(
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
#endif // RimFill_H

