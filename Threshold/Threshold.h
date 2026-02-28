//-----------------------------------------------------------------------------------
/*
	Threshold for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Threshold_H
#define Threshold_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
*/
#include <string>
#include <vector>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE,
	ID_LUMA_CB,
	ID_RED_CB,
	ID_GREEN_CB,
	ID_BLUE_CB,
	ID_ALPHA_CB,
	ID_LUMA_LEVEL,
	ID_RED_LEVEL,
	ID_GREEN_LEVEL,
	ID_BLUE_LEVEL,
	ID_ALPHA_LEVEL,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE			"mode"
#define	STR_MODE_ITEMS		"Luma|RGB"
#define	STR_RED				"red"
#define	STR_GREEN			"green"
#define	STR_BLUE			"blue"
#define	STR_ALPHA			"alpha"
#define	STR_LUMA_LEVEL		"luma_level"
#define	STR_RED_LEVEL		"red_level"
#define	STR_GREEN_LEVEL		"green_level"
#define	STR_BLUE_LEVEL		"blue_level"
#define	STR_ALPHA_LEVEL		"alpha_level"

enum {
	CHAN_LUMA = 0,
	CHAN_R,
	CHAN_G,
	CHAN_B,
	CHAN_A,
};
//UIのパラメータ
typedef struct ParamInfo {
	A_long 	mode;
	PF_Boolean	enabled[5];
	PF_FpLong	level[5];

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err FilterImage(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
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
#endif // Threshold_H

