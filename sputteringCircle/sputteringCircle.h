//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once

#ifndef sputteringCircle_H
#define sputteringCircle_H


#include "Fs_Target.h"
#include "AEConfig.h"
#include "entry.h"


//#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"

#if defined(PF_AE100_PLUG_IN_VERSION)
	#include "AEFX_SuiteHelper.h"
	#define refconType void*
#else
	#include "PF_Suite_Helper.h"
	#define refconType A_long
#endif

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#include "../FsLibrary/FsAE.h"
//#include "FsAE.h"

#include "../FsLibrary/FsSputtering.h"



//-----------------------------------------------------------------------------
//ParamsSetup用の定数
enum {
	ID_INPUT = 0,

	ID_SEED,				//乱数の基準
	ID_Y,				//量
	ID_OPACITY_RAND,		//透明度の最大値

	ID_POS,				//位置
	ID_RADIUS,			//半径
	ID_ASPECT,			//アスペクト
	ID_LENGTH_SCALE,		//スケール

	ID_ANCHOR_ENABLED,	//アンカーポイントを有効にするか
	ID_ANCHOR,			//アンカーポイント
	
	ID_POINT_VALUE,	//一回の描画で打つ点の数
	ID_POINT_LENGTH,	//点の距離
	ID_POINT_LEN_SYNC,	//点の距離

	ID_SIZE,

	ID_COLOR1,
	ID_COLOR_TOPIC,
	ID_COLOR_MAX,
	ID_COLOR2,
	ID_COLOR3,
	ID_COLOR4,
	ID_COLOR_TOPIC_END,

	ID_ORG,

	ID_NUM_PARAMS
};
//-----------------------------------------------------------------------------
//パラメータUI用の文字列
#define UI_SEED			"seed"
#define UI_VALUE		"value"
#define UI_OPA_RND		"opacity_rand"

#define UI_POS			"Position"
#define UI_RADIUS		"Radius"
#define UI_ASPECT		"Aspect"
#define UI_LENGTH_SCALE		"Length_Scale"
#define UI_ANCHOR_ENABLED1	"Anchor_Enabled"
#define UI_ANCHOR_ENABLED2	"ON"
#define UI_ANCHOR			"Anchor"

#define UI_POINT_VALUE	"PointValue"
#define UI_POINT_LENGTH	"PointLength"
#define UI_POINT_LEN_SYSNC1	"Point_length_sysnc"
#define UI_POINT_LEN_SYSNC2	"ON"

#define UI_SIZE1		"size"
#define UI_SIZE2		"極小|小|中|大|特大"
#define UI_SIZE_COUNT	5
#define UI_SIZE_DFLT	2

#define UI_COLOR1		"Color1"
#define UI_COLOR_TOPIC	"ExtraColors"
#define UI_COLOR_MAX	"ColorMax"
#define UI_COLOR_MAX_V	4
#define UI_COLOR2		"Color2"
#define UI_COLOR3		"Color3"
#define UI_COLOR4		"Color4"

#define UI_ORG1			"Blend with original"
#define UI_ORG2			"ON"

//-----------------------------------------------------------------------------


typedef struct
{
	A_long		seed;
	A_long		value;
	PF_Fixed	opa;
	PF_FixedPoint	pos;
	A_long		radius;
	PF_Fixed	aspect;
	PF_Fixed	length_scale;
	PF_Boolean	anchor_enabled;
	PF_FixedPoint	anchor;

	A_long		point_value;
	A_long		point_length;
	PF_Boolean	point_len_sync;
	A_long		size;
	A_long		color_max;
	PF_Pixel	colors[UI_COLOR_MAX_V];
	A_u_char	*sputRandTable;
	PF_Boolean	org;
} ParamInfo;


//-----------------------------------------------------------------------------
//関数の宣言
PF_Err MainRender8 (CFsAE *ae, ParamInfo *prm);
PF_Err MainRender16 (CFsAE *ae, ParamInfo *prm);
PF_Err MainRender32 (CFsAE *ae, ParamInfo *prm);

//-----------------------------------------------------------------------------
extern "C" {

DllExport 
PF_Err 
EntryPointFunc (	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}
//-----------------------------------------------------------------------------

#endif // sputteringCircle_H