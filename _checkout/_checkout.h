//-----------------------------------------------------------------------------------
/*
	_checkout for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef _checkout_H
#define _checkout_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\NF_Utils.h"
#include "..\_NFLib\fx\NF_SpatUtils.h"


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

#include "_checkoutFilter.h"
*/
#include <string>
#include <vector>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_LAYER,
	ID_POS,
	ID_COUNT,
	ID_RADIUS,
	ID_SCALE,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_LAYER			"layer"
#define	STR_POS				"pos"
#define	STR_RADIUS			"radius"
#define	STR_COUNT			"count"
#define	STR_SCALE			"scale"
//UIのパラメータ
typedef struct ParamInfo {
	PF_LayerDef	layer;
	PF_Point	pos;
	A_long		count;
	A_long		radius;
	float		scale;
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
#endif // _checkout_H

