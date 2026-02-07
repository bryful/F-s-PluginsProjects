//-----------------------------------------------------------------------------------
/*
	RGBShifter for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef RGBShifter_H
#define RGBShifter_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
*/
#include  "RGBShifterFilter.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE,

	ID_R,
	ID_G,
	ID_B,

	ID_Y,
	ID_U,
	ID_V,

	ID_TARGETCOLOR,

	ID_BLEND,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE_POP		"ShiftMode"
#define	STR_MODE_ITEMS		"RGB Channel|YUV Channel|Luma Identity"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		1

#define	STR_R				"RedShift"
#define	STR_G				"GreenShift"
#define	STR_B				"BlueShift"

#define	STR_Y				"Y_Luminance"
#define	STR_U				"U_Chroma"
#define	STR_V				"V_Chroma"

#define	STR_TARGETCOLOR		"TargetColor"
#define	STR_BLEND			"Blend with Original"
//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	PF_FpLong	r_shift;
	PF_FpLong	g_shift;
	PF_FpLong	b_shift;
	PF_FpLong	y_shift;
	PF_FpLong	u_shift;
	PF_FpLong	v_shift;
	PF_Pixel	target_color;
	PF_FpLong	target_u;
	PF_FpLong	target_v;
	PF_FpLong	blend;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err RGBShiftFX(
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
#endif // RGBShifter_H

