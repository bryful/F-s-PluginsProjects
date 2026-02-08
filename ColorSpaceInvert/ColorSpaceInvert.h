//-----------------------------------------------------------------------------------
/*
	ColorSpaceInvert for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ColorSpaceInvert_H
#define ColorSpaceInvert_H

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

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE,
	ID_VALUE,
	//ID_HUE_ROT,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE			"mode"
#define	STR_VALUE			"value"

#define	STR_MODE_ITEMS		"RGBChannels|InvertLuma(Y)|InvertHue(H)"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		1
//#define	STR_HUE_ROT			"Hue Rotation"
//#define	STR_HUE_ROT_ITEMS	"Clockwise|Counter-Clockwise"
//#define	STR_HUE_ROT_COUNT		2
//#define	STR_HUE_ROT_DFLT		1


//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	PF_FpLong	value;
	PF_Boolean	hue_rot_ccw;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------



PF_Err FilterImage(
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
#endif // ColorSpaceInvert_H

