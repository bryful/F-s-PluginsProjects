//-----------------------------------------------------------------------------------
/*
	Noise for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Noise_H
#define Noise_H

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
#include "..\_NFLib\fx\NF_Spat_Data.h"
#include <string>
#include <thread>
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_AUTO_SEED,
	ID_SEED,

	ID_NOISE_AMAOUNT,
	ID_NOISE_INDENSITY,

	ID_ACCENT_COUNT,
	ID_ACCENT_INTENSITY,

	ID_IS_COLOR,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_AUTO_SEED		"Auto Seed"
#define	STR_SEED			"Seed"
#define	STR_NOISE_AMAOUNT	"Noise Amount"
#define	STR_NOISE_INDENSITY	"Noise Intensity"
#define	STR_ACCENT_COUNT		"Accent Amount"
#define	STR_ACCENT_INTENSITY	"Accent Intensity"
#define	STR_COLOR			"Color"


//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	autoSeed;
	A_long		seed;
	PF_FpLong	noiseAmount;
	PF_FpLong	noiseIntensity;
	A_long		accentAmount;
	PF_FpLong	accentIntensity;
	PF_Boolean isColor;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err NoiseMain(
	NF_AE* ae,
	PF_EffectWorld* worldP,
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
#endif // Noise_H

