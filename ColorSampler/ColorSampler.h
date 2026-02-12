//-----------------------------------------------------------------------------------
/*
	ColorSampler for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ColorSampler_H
#define ColorSampler_H

#include "..\_NFLib\AE_SDK.h"

#include "AEConfig.h"
#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "String_Utils.h"
#include "AEGP_SuiteHandler.h"
#include "AEFX_SuiteHelper.h"
#include "PF_Masks.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#define COLOR_TABLE_SIZE 32
#define MAX_NAME_LEN 32
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_COLOR01,
	ID_COLOR02,
	ID_COLOR03,
	ID_COLOR04,
	ID_COLOR05,
	ID_COLOR06,
	ID_COLOR07,
	ID_COLOR08,
	ID_COLOR09,
	ID_COLOR10,
	ID_COLOR11,
	ID_COLOR12,
	ID_COLOR13,
	ID_COLOR14,
	ID_COLOR15,
	ID_COLOR16,
	ID_COLOR17,
	ID_COLOR18,
	ID_COLOR19,
	ID_COLOR20,
	ID_COLOR21,
	ID_COLOR22,
	ID_COLOR23,
	ID_COLOR24,
	ID_COLOR25,
	ID_COLOR26,
	ID_COLOR27,
	ID_COLOR28,
	ID_COLOR29,
	ID_COLOR30,
	ID_COLOR31,
	ID_COLOR32,
	ID_COLOR_COUNT,
	ID_GET_BTN,
	ID_NAME_EDIT_BTN,

	ID_NUM_PARAMS
};
#define	ID_COLOR(IDX)(ID_COLOR01 + (IDX))

//UIの表示文字列
#define	STR_COLOR			"color"
#define	STR_NAME_EDIT_BTN	"Edit Color Names"
#define	STR_GET_BTN			"Get Colors from Masks"
#define	STR_REF_BTN			"Refresh Caption!"


//UIのパラメータ
typedef struct ParamInfo {
	A_long		color_count;
	PF_Pixel	colors[COLOR_TABLE_SIZE];
	PF_Boolean	show_params;

} ParamInfo, *ParamInfoP, **ParamInfoH;
typedef struct NameInfo {
	A_long		color_count;
	A_char		color_names[COLOR_TABLE_SIZE][MAX_NAME_LEN];

} NameInfo;

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
#endif // ColorSampler_H

