//-----------------------------------------------------------------------------------
/*
	AlphaFill for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef AlphaFill_H
#define AlphaFill_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include <string>
#include <cmath>
#include <algorithm>
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE,
	ID_COLOR,
	ID_THRESH,
	ID_SOFT,
	ID_SPILL,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE		"Mode"

#define	STR_MODE_ITEMS	"AlphaFill|ColorKey"
#define	STR_MODE_COUNT	2
#define	STR_MODE_DFLT	1

#define	STR_COLOR		"Color"
#define	STR_THRESH		"Thresh"
#define	STR_SOFT		"Soft"
#define	STR_SPILL		"Spill amt"


//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	PF_Pixel	color;
	PF_Pixel16	color16;
	PF_PixelFloat	colorFloat;
	PF_Point	paint_pos;
	
	PF_FpLong	thresh;
	PF_FpLong	soft;
	PF_FpLong	Spill_amt;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err AlphaFillExec(
	NF_AE* ae,
	ParamInfo* infoP
);
PF_Err ChromaKeyExec(
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
#endif // AlphaFill_H

