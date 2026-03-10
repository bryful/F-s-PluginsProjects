//-----------------------------------------------------------------------------------
/*
	RandomMosaic for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef MUZZKE_FLASH_H
#define MUZZKE_FLASH_H

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
#include "..\_NFLib\fx\NF_Mosaic.h"
#include "..\_NFLib\fx\NF_RandomShift.h"
*/

#include "MuzzleFlashMain.h"

#include <string>


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	// -----
	ID_PRONGS_POINT,
	ID_PRONGS_COUNT,
	ID_PRONGS_ROOT_SIZE,
	ID_PRONGS_LENGTH,
	ID_PRONGS_BRANCH,
	ID_PRONGS_DIR,
	ID_PRONGS_ANGLE,
	ID_PRONGS_SCALE,
	ID_PRONGS_COLOR,

	// -----
	ID_AUTO_SEED,		//ノイズのシードを自動で変化させるかどうかのチェックボックス
	ID_SEED,			//ノイズのシード値の数値入力
	// -----

	ID_NUM_PARAMS
};

//UIの表示文字列

//-------
#define	STR_MASTER			"master"

#define	STR_PRONGS_POINT		"center"
#define	STR_PRONGS_COUNT		"count"
#define	STR_PRONGS_ROOT_SIZE	"rootSize"
#define	STR_PRONGS_LENGTH		"length"
#define	STR_PRONGS_BRANCH		"branch"
#define	STR_PRONGS_DIR			"direction"
#define	STR_PRONGS_ANGLE		"angle"
#define	STR_PRONGS_SCALE		"scale"
#define	STR_PRONGS_COLOR		"color"


#define	STR_AUTO_SEED		"autoSeed"
#define	STR_SEED			"seed"




//UIのパラメータ
typedef struct ParamInfo {
	float		pointX;
	float		pointY;
	A_long		count;
	float		rootSize;
	float		length;
	A_long		branch;
	float		direction;
	float		angle;
	float		scale;
	PF_Pixel	color;

	PF_Boolean autoSeed;
	A_long		seed;
} ParamInfo, * ParamInfoP, ** ParamInfoH;

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
			PF_InData* in_data,
			PF_OutData* out_data,
			PF_ParamDef* params[],
			PF_LayerDef* output,
			void* extra);
}
#endif // RandomMosaic_H

