//-----------------------------------------------------------------------------------
/*
	Ligtning for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Ligtning_H
#define Ligtning_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\json.hpp"
#include "..\_NFLib\NF_Json.h"
#include "..\_NFLib\NF_Utils.h"

/*
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
*/
#include "..\_NFLib\vector\NF_VectorLine.h"
#include "..\_NFLib\vector\NF_VectorMask.h"


//#include "noiseFilter.h"
#include <string>
#include <vector>

enum {
	MODE_2POINT = 1,
	MODE_PATH,
	MODE_RADICAL,
	MODE_NUM
};

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE,		//モード用のポップアップ
	
	// -----
	ID_AUTO_SEED,		//ノイズのシードを自動で変化させるかどうかのチェックボックス
	ID_SEED,			//ノイズのシード値の数値入力
	// -----
	ID_START_POS,
	ID_END_POS,
	// -----
	ID_PARAMS_TOPIC,
	ID_WIPE,			// wipe
	ID_START_WEIGHT,	// 線の太さの開始値
	ID_END_WEIGHT,		// 線の太さの終了値
	ID_WEIGHT,		// 線の太さ
	ID_COMPLEXTTY,	// 分割数
	ID_JAGGEDNESS,	// 分割ピクセルの距離
	ID_COMPL_ANGLE, //分割時の
	ID_FORK,		// 描画回数
	ID_COLOR,		// 線の色
	ID_PARAMS_TOPIC_END,
	
	ID_NUM_PARAMS
};

//UIの表示文字列

//-------
#define	STR_MODE			"mode"
#define	STR_MODE_ITEMS		"2point|path|radical"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		MODE_2POINT

#define	STR_AUTO_SEED		"autoSeed"
#define	STR_SEED			"seed"
#define	STR_START_POS		"start"
#define	STR_END_POS			"end"


//-------
#define	STR_PARAMS_TOPIC	"params"
#define	STR_WIPE			"wipe"
#define	STR_START_WEIGHT	"startWeight"
#define	STR_END_WEIGHT		"endWeight"
#define	STR_WEIGHT			"weight"
#define	STR_COMPLEXTTY		"complexity"
#define	STR_JAGGEDNESS		"jaggedness"
#define	STR_COMPL_ANGLE		"comple_angle"
#define	STR_FORK			"fork"
#define	STR_COLOR			"color"
//-------

//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	PF_Boolean	auto_seed;
	A_long		seed;
	A_long		seedAct;

	PF_FpLong	wipe;
	PF_FpLong	startWeight;
	PF_FpLong	endWeight;
	PF_FpLong	weight;
	A_long		complexity;
	PF_FpLong	jaggedness;
	PF_FpLong	comple_angle;
	A_long		fork;


	PF_Point	posTwin[2];
	PF_Pixel	color;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------

PF_Err LigtningDraw(
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
#endif // Ligtning_H

