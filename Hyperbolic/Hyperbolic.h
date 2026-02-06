//-----------------------------------------------------------------------------------
/*
	Hyperbolic for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Hyperbolic_H
#define Hyperbolic_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\fx\NF_Hyperbolic.h"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_RGB,
	ID_ALPHA,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_RGB		"rgbHyperbolic"
#define	STR_ALPHA	"alphaHyperbolic"

//UIのパラメータ
typedef struct ParamInfo {
	
	
	PF_FpLong	rgbHyperbolic;
	PF_FpLong	alphaHyperbolic;

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
#endif // Hyperbolic_H

