//-----------------------------------------------------------------------------------
/*
	Mosaic for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Mosaic_H
#define Mosaic_H

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

#include "MosaicFilter.h"
*/
#include "..\_NFLib\fx\NF_Mosaic.h"
#include <string>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MOSAIC_UNLINK,
	ID_MOSAIC_W,
	ID_MOSAIC_H,
	ID_MOSAIC_CENTER,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MOSAIC_UNLINK	"Unlink M_Width and M_Height"
#define	STR_MOSAIC_W		"M_Width"
#define	STR_MOSAIC_H		"M_Height"
#define	STR_MOSAIC_CENTER	"center"

//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean unlink;
	A_long mosaicW;
	A_long mosaicH;
	PF_Point mosaicCenter;

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
#endif // Mosaic_H

