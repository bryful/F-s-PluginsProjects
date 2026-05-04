//-----------------------------------------------------------------------------------
/*
	SimplexNoise for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SimplexNoise_H
#define SimplexNoise_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\NF_Utils.h"

/*
#include "..\_NFLib\json.hpp"
#include "..\_NFLib\NF_Json.h"

#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
#include "..\_NFLib\fx\NF_Bilateral.h"
#include "..\_NFLib\fx\NF_Blend.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_DrawWorld.h"
#include "..\_NFLib\fx\NF_Gradient.h"
#include "..\_NFLib\fx\NF_Hyperbolic.h"
#include "..\_NFLib\fx\NF_LogicGray.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_Mosaic.h"
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_SpatUtils.h"
#include "..\_NFLib\fx\NF_UnMult.h"

#include "..\_NFLib\vector\NF_VectorLine.h"
#include "..\_NFLib\vector\NF_VectorMask.h"
*/

//#include "noiseFilter.h"
#include "SimplexNoiseEngine.h"
#include <string>
#include <vector>


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_EVOLUTION,		//モード用のポップアップ
	ID_OCTAVES,		//モード用のポップアップ
	ID_SCALE,
	ID_NUM_PARAMS
};

//UIの表示文字列

//-------
#define	STR_EVOLUTION	"evolution"
#define	STR_OCTAVES		"octaves"
#define	STR_SCALE		"scale"




//UIのパラメータ
typedef struct ParamInfo {
	float	evolution;
	A_long	octaves;
	float	scale;
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
#endif // SimplexNoise_H

