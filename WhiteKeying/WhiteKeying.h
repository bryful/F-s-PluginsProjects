//-----------------------------------------------------------------------------------
/*
	WhiteKeying for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef WhiteKeying_H
#define WhiteKeying_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"


#include "WhiteKeyingFilter.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_COLOR,
	ID_LEVEL,
	ID_REVERSE,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_COLOR			"keyColor"
#define	STR_LEVEL			"keyLevel"
#define	STR_REVERSE			"reverse"


//UIのパラメータ
typedef struct ParamInfo {
	PF_Pixel	keyColor;
	PF_FpLong	keyLevel;
	PF_Boolean	reverse;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------


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
#endif // WhiteKeying_H

