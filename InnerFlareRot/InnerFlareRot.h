//-----------------------------------------------------------------------------------
/*
	InnerFlareRot for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef InnerFlareRot_H
#define InnerFlareRot_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Channelblur.h"
//#include "..\_NFLib\fx\debug_font.h"
//#include "..\_NFLib\fx\NF_Paint.h"
//#include "..\_NFLib\fx\NF_Noise.h"
//#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
#include "..\_NFLib\fx\NF_Blend.h"

#include "AlphaCopyDD.h"
#include "AlphaCopyR.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_COLOR,
	ID_ROT,
	ID_LENGTH,
	ID_REVERSE,
	ID_MINMAX,
	ID_BLUR,
	ID_HYPERBOLIC,
	ID_WHITE,
	ID_BLEND,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_BLUR			"blur"
#define	STR_MINMAX			"minMax"
#define	STR_REVERSE			"reverse"
#define	STR_REVERSE2		"on"
#define	STR_HYPERBOLIC		"hyperbolic"
#define	STR_ROT				"rot"
#define	STR_LENGTH			"length"
#define	STR_OFFSET			"offset"
#define	STR_COLOR			"color"
#define	STR_WHITE			"Treat White as Transparent"
#define	STR_BLEND			"Blend with original"



//UIのパラメータ
typedef struct ParamInfo {
	A_long				blur;
	PF_FpLong			rot;
	PF_FpLong			length;
	A_long				minmax;
	PF_Boolean			reverse;
	PF_FpLong			hyperbolic;
	PF_Pixel			color;
	PF_Boolean			isWhite;
	PF_Boolean			isBlend;
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
#endif // InnerFlareRot_H

