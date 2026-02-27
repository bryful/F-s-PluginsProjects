//-----------------------------------------------------------------------------------
/*
	Chroma for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Chroma_H
#define Chroma_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
/*
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
*/
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "ChromaFilter.h"
#include <string>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_R_SCALE,
	ID_G_SCALE,
	ID_B_SCALE,

	ID_R_BLUR,
	ID_G_BLUR,
	ID_B_BLUR,

	ID_CENTER,
	ID_ALPHA_MODE,



	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_R_SCALE		"red_scale"
#define	STR_G_SCALE		"green_scale"
#define	STR_B_SCALE		"blue_scale"
#define	STR_R_BLUR		"red_blur"
#define	STR_G_BLUR		"green_blur"
#define	STR_B_BLUR		"blue_blur"
#define	STR_CENTER		"center"
#define	STR_ALPHA_MODE	"alpha"
#define	STR_ALPHA_ITEMS	"original|dynamic"
#define	STR_ALPHA_COUNT	2
#define	STR_ALPHA_DFLT	1


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	r_scale;
	PF_FpLong	g_scale;
	PF_FpLong	b_scale;
	PF_Point	center;
	A_long		r_blur;
	A_long		g_blur;
	A_long		b_blur;
	A_long		alpha_mode;

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
#endif // Chroma_H

