//-----------------------------------------------------------------------------------
/*
	_Test for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef _Test_H
#define _Test_H

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
#include "..\_NFLib\vector\NF_VectorRect.h"
#include "..\_NFLib\vector\NF_VectorMask.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_POS1,
	ID_POS2,
	ID_POS3,
	ID_POS4,
	ID_POSA1,
	ID_POSA2,
	ID_POSA3,
	ID_WA1,
	ID_WA2,
	ID_WA3,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_POS1			"pos1"
#define	STR_POS2			"pos2"
#define	STR_POS3			"pos3"
#define	STR_POS4			"pos4"
#define	STR_POSA1			"posA1"
#define	STR_POSA2			"posA2"
#define	STR_POSA3			"posA3"
#define	STR_WA1			"wA1"
#define	STR_WA2			"wA2"
#define	STR_WA3			"wA3"
//UIのパラメータ
typedef struct ParamInfo {
	PF_Point	pos[4];
	PF_Point	posA[3];
	float		wA[3];
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
#endif // _Test_H

