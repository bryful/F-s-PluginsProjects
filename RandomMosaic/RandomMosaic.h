//-----------------------------------------------------------------------------------
/*
	RandomMosaic for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef RandomMosaic_H
#define RandomMosaic_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
//#include "..\_NFLib\json.hpp"
//#include "..\_NFLib\NF_Json.h"
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

#include "..\_NFLib\vector\NF_VectorLine.h"
#include "..\_NFLib\vector\NF_VectorMask.h"
*/
#include "..\_NFLib\fx\NF_Mosaic.h"
#include "..\_NFLib\fx\NF_RandomShift.h"
#include "ColorBlockFilter.h"
#include "noiseFilter.h"

#include <string>


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	// -----
	ID_MASTER,
	// -----
	ID_MOSAIC_TOPIC,
	ID_MOSAIC_ENABLED,
	ID_MOSAIC_AMOUNT,
	ID_MOSAIC_WIDTH,
	ID_MOSAIC_HEIGHT,
	ID_MOSAIC_VALUE,
	ID_MOSAIC_ISCOLOR,
	ID_MOSAIC_TOPIC_END,
	// -----
	ID_CBLOCK_TOPIC,
	ID_CBLOCK_ENABLED,
	ID_CBLOCK_COUNT,
	ID_CBLOCK_WIDTH,
	ID_CBLOCK_HEIGHT,
	ID_CBLOCK_VALUE,
	ID_CBLOCK_ALPHA,
	ID_CBLOCK_TOPIC_END,
	// -----
	ID_SHIFT_TOPIC,
	ID_SHIFT_ENABLED,
	ID_SHIFT_COUNT,
	ID_SHIFT_WIDTH,
	ID_SHIFT_HEIGHT,
	ID_SHIFT_OFFSET_X,
	ID_SHIFT_OFFSET_Y,
	ID_SHIFT_TOPIC_END,
	// -----
	ID_BLOCK_TOPIC,
	ID_BLOCK_ENABLED,
	ID_BLOCK_COUNT,
	ID_BLOCK_WIDTH,
	ID_BLOCK_HEIGHT,
	ID_BLOCK_NOISE,
	ID_BLOCK_NOISE_ISCOLOR,
	ID_BLOCK_TOPIC_END,
	// -----
	ID_EMBOSS_TOPIC,
	ID_EMBOSS_ENABLED,
	ID_EMBOSS_DISTANCE,
	ID_EMBOSS_ISCOLOR,
	ID_EMBOSS_ALPHA,
	ID_EMBOSS_TOPIC_END,
	// -----
	ID_NOISE_TOPIC,
	ID_NOISE_ENABLED,
	ID_NOISE_AMOUNT,
	ID_NOISE_ISCOLOR,
	ID_NOISE_TOPIC_END,
	// -----
	ID_AUTO_SEED,		//ノイズのシードを自動で変化させるかどうかのチェックボックス
	ID_SEED,			//ノイズのシード値の数値入力
	// -----
	
	ID_NUM_PARAMS
};

//UIの表示文字列

//-------
#define	STR_MASTER			"master"

#define	STR_MOSAIC_TOPIC		"mosaic"
#define	STR_MOSAIC_ENABLED		"m_enabled"
#define	STR_MOSAIC_AMOUNT		"m_amount"
#define	STR_MOSAIC_WIDTH		"m_width"
#define	STR_MOSAIC_HEIGHT		"m_height"
#define	STR_MOSAIC_VALUE		"m_add"
#define	STR_MOSAIC_ISCOLOR		"m_isColor"

#define	STR_CBLOCK_TOPIC		"colorBlock"
#define	STR_CBLOCK_ENABLED		"c_enabled"
#define	STR_CBLOCK_COUNT		"c_count"
#define	STR_CBLOCK_WIDTH		"c_width"
#define	STR_CBLOCK_HEIGHT		"c_height"
#define	STR_CBLOCK_VALUE		"c_add"
#define	STR_CBLOCK_ALPHA		"c_isAlpha"

#define	STR_SHIFT_TOPIC			"shift"
#define	STR_SHIFT_ENABLED		"s_enabled"
#define	STR_SHIFT_COUNT			"s_count"
#define	STR_SHIFT_WIDTH			"s_width"
#define	STR_SHIFT_HEIGHT		"s_height"
#define	STR_SHIFT_OFFSET_X		"s_offsetX"
#define	STR_SHIFT_OFFSET_Y		"s_offsetY"

#define	STR_BLOCK_TOPIC			"block"
#define	STR_BLOCK_ENABLED		"b_enabled"
#define	STR_BLOCK_COUNT			"b_count"
#define	STR_BLOCK_WIDTH			"b_width"
#define	STR_BLOCK_HEIGHT		"b_height"
#define	STR_BLOCK_NOISE			"b_noise"
#define	STR_BLOCK_NOISE_ISCOLOR	"b_isColor"

#define	STR_EMBOSS_TOPIC		"emboss"
#define	STR_EMBOSS_ENABLED		"e_enabled"
#define	STR_EMBOSS_DISTANCE		"e_distance"
#define	STR_EMBOSS_ISCOLOR		"e_isColor"
#define	STR_EMBOSS_ALPHA		"e_isAlpha"

#define	STR_NOISE_TOPIC			"noise"
#define	STR_NOISE_ENABLED		"n_enabled"
#define	STR_NOISE_AMOUNT		"n_amaount"
#define	STR_NOISE_ISCOLOR		"n_isColor"


#define	STR_AUTO_SEED		"autoSeed"
#define	STR_SEED			"seed"




//UIのパラメータ
typedef struct ParamInfo {
	float		master;

	PF_Boolean	mEnabled;
	float		mAmount;
	A_long		mWidth;
	A_long		mHeight;
	float		mValue;
	PF_Boolean	mIsColor;

	PF_Boolean	cbEnabled;
	A_long		cbCount;
	A_long		cbWidth;
	A_long		cbHeight;
	float		cbValue;
	PF_Boolean	cbAlpha;

	PF_Boolean	rsEnabled;
	A_long		rsCount;
	A_long		rsWidth;
	A_long		rsHeight;
	A_long		rsOffsetX;
	A_long		rsOffsetY;

	PF_Boolean	bmEnabled;
	A_long		bmCount;
	A_long		bmWidth;
	A_long		bmHeight;
	float		bmNoise;
	PF_Boolean	bmNoiseIsColor;

	PF_Boolean	ebEnabled;
	float		ebDistance;
	PF_Boolean	ebIsColor;
	PF_Boolean	ebIsAlpha;

	PF_Boolean	noiseEnabled;
	float		noiseAmount;
	PF_Boolean	noiseIsColor;


	PF_Boolean	autoSeed;
	A_long		seed;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err ColorEmbossHorMain(
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_EffectWorld* world,
	float distance,
	PF_Boolean isColor,
	PF_Boolean isAlpha
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
#endif // RandomMosaic_H

