//-----------------------------------------------------------------------------------
/*
	ColorSwitch for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ColorSwitch_H
#define ColorSwitch_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\\NF_Settings.h"

#include "..\_NFLib\json.hpp"
using json = nlohmann::json;

#include <string>

#define COLOR_TABLE_MAX	32
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_ENABLEALL,
	ID_ACTIVE_PARAM_COUNT,
	ID_MODE,

	ID_TOPIC_COLORS,

	ID_TURNON_0,
	ID_OLD_COLOR_0,
	ID_NEW_COLOR_0,
};
#define ID_TURNON(IDX) (ID_TURNON_0 + (IDX)*3)
#define ID_OLD_COLOR(IDX) (ID_OLD_COLOR_0 + (IDX)*3)
#define ID_NEW_COLOR(IDX) (ID_NEW_COLOR_0 + (IDX)*3)
enum {
	ID_TOPIC_END = 101,
	ID_ALL_TURNOFF,
	ID_SAVE_PARAMS,
	ID_LOAD_PARAMS,
	ID_NUM_PARAMS
};
//#define ID_TOPIC_END (ID_TURNON_0 + (COLOR_TABLE_MAX)*3)
//#define ID_ALL_TURNOFF (ID_TOPIC_END+1)
//#define ID_SAVE_PARAMS (ID_ALL_TURNOFF+1)
//#define ID_LOAD_PARAMS (ID_SAVE_PARAMS+1)
//#define ID_NUM_PARAMS (ID_LOAD_PARAMS+1)

//UIの表示文字列
#define	STR_ENABLEALL				"EnableAll"
#define	STR_ACTIVE_PARAM_COUNT		"ActiveParamCount"

#define	STR_MODE_POP		"Mode"
#define	STR_MODE_ITEMS		"Replace|Key|Extract"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		1

#define	STR_TOPIC			"Colors"

#define	STR_TURNON			"TurnON"
#define	STR_OLD				"Old"
#define	STR_NEW				"New"

#define	STR_ALL_TURNOFF1	"All turn off"
#define	STR_ALL_TURNOFF2	"OFF"
#define	STR_SAVE_PARAMS1	"Save Params"
#define	STR_SAVE_PARAMS2	"Save"
#define	STR_LOAD_PARAMS1	"Load Params"
#define	STR_LOAD_PARAMS2	"Load"

enum SwitchMode {
	SWITCH_MODE_REPLACE = 1,
	SWITCH_MODE_KEY = 2,
	SWITCH_MODE_EXTRACT = 3,
};
//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	enable_All;
	A_long		active_param_count;
	A_long		mode;
	PF_Boolean	turnon_colors[COLOR_TABLE_MAX];
	PF_Pixel	old_colors[COLOR_TABLE_MAX];
	PF_Pixel	new_colors[COLOR_TABLE_MAX];

} ParamInfo;
//-------------------------------------------------------
PF_Err ColSWitch(NF_AE* ae, ParamInfo* infoP);
json ParamInfoToJson(ParamInfo* infoP);
PF_Err JsonToParamInfo(json js, ParamInfo* infoP);

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
#endif // ColorSwitch_H

