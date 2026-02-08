//-----------------------------------------------------------------------------------
/*
	Emboss for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Emboss_H
#define Emboss_H

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

//#include "EmbossFilter.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_DIRECTION,
	ID_DISTANCE,
	ID_PAINT_COLOR,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_DIRECTION		"direction"
#define	STR_DISTANCE		"distance"
/*
#define	STR_PAINT_POS		"paintPos"
#define	STR_PAINT_COLOR		"paintColor"

#define	STR_MINMAX			"minmax"
#define	STR_BLUR			"blur"

#define	STR_NOISE_SIZE		"noiseSize"
#define	STR_NOISE_AMOUNT	"noiseAmount"
#define	STR_NOISE_ACCENT_AMOUNT	"accentAmount"
#define	STR_NOISE_ISCOlOR	"noiseColor"
#define	STR_NOISE_AUTO		"noiseAuto"
#define	STR_NOISE_SEED		"noiseSeed"

#define	STR_DRAW_POP		"draw"
#define	STR_DRAW_ITEMS		"none|line|box|boxfill|Circle|CircleFill"
#define	STR_DRAW_COUNT		6
#define	STR_DRAW_DFLT		1
#define	STR_START_POS		"startPos"
#define	STR_END_POS			"endPos"
#define	STR_DRAW_COLOR		"drawColor"

#define	STR_DEBUG_FONT_CB	"debugFont"
#define	STR_DEBUG_FONT_POS	"debugPos"
#define	STR_DEBUG_FONT_COLOR "debugColor"


#define	STR_HIDDEN_ON1		"oba-Q!"

#define	STR_HIDDEN_ON		"Menyo--!"
#define	STR_TOPIC			"Sample Topics"
#define	STR_ANGLE			"Angle"

#define	STR_BUTTON1			"button"
#define	STR_BUTTON2			"push"
*/
//UIのパラメータ
typedef struct ParamInfo {
	
	PF_FpLong	direction;
	PF_FpLong	distance;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err EmbossFilter(
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
#endif // Emboss_H

