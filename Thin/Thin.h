//-----------------------------------------------------------------------------------
/*
	Thin for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Thin_H
#define Thin_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include <string>

#define TARGET_COLOR_MAX	8
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_TARGET_COLOR_TOPIC,
	ID_TARGET_COLOR_MODE,
	ID_TARGET_LEVEL,
	ID_TARGET_COLOR1,
	ID_TARGET_COLOR2,
	ID_TARGET_COLOR3,
	ID_TARGET_COLOR4,
	ID_TARGET_COLOR5,
	ID_TARGET_COLOR6,
	ID_TARGET_COLOR7,
	ID_TARGET_COLOR8,
	ID_TARGET_COLOR_TOPIC_END,

	ID_THIN_VALUE,
	
	ID_PARAMS_TOPIC,
	ID_WHITE,
	ID_ALPHAZERO,
	ID_EDGE,
	ID_PARAMS_TOPIC_END,
	ID_NUM_PARAMS

};
#define ID_TARGET_COLOR(n)	(ID_TARGET_COLOR1 + (n))
//UIの表示文字列
#define	STR_TARGET_COLOR_TOPIC		"Inking Lines"

#define	STR_TARGET_COLOR_MODE		"Inking Color Count"
#define	STR_TARGET_COLOR_ITEMS		"1|2|3|4|5|6|7|8"
#define	STR_TARGET_COLOR_COUNT		8
#define	STR_TARGET_COLOR_DFLT		1
#define	STR_TARGET_COLOR			"Target"
#define	STR_TARGET_LEVEL			"Range"

#define	STR_THIN_VALUE				"ThinValue"
#define	STR_WHITE					"Ignore White"
#define	STR_ALPHAZERO				"Ignore Transparent"
#define	STR_EDGE					"Refine Edges"
#define	ID_PARAMS_TOPIC				"Refine Edges"


//UIのパラメータ
typedef struct ParamInfo {
	PF_Pixel	target_color[TARGET_COLOR_MAX];
	A_long		target_color_count;
	PF_FpLong	target_level;

	A_long		thin_value;

	PF_Boolean	white;
	PF_Boolean	alphaZero;
	PF_Boolean	edge;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err thinExec8(NF_AE* ae, ParamInfo* infoP);
PF_Err thinExec16(NF_AE* ae, ParamInfo* infoP);
PF_Err thinExec32(NF_AE* ae, ParamInfo* infoP);
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
#endif // Thin_H

