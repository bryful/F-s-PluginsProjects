//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef TouchDrawCenter_H
#define TouchDrawCenter_H
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



#define	STR_SEED			"RandomSeed"
#define	STR_CENTER			"Center"
#define	STR_LENGTH_MAX		"Length"
#define	STR_LENGTH_RND		"Length_Random"
#define	STR_Y			"Value"
#define	STR_COLOR			"Color"
#define	STR_OPACITY			"Opacity"
#define	STR_OPACITY_RND		"Opacity_Random"

#define	STR_POINT_COUNT		"Point_Count"
#define	STR_POINT_LENGTH	"Point_Length"
#define	STR_ORG_CB1			"Original_Blend"
#define	STR_ORG_CB2			"ON"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	
	ID_SEED,		//ランダムの基点
	ID_CENTER,
	ID_LENGTH_MAX,		//タッチ線の長さ
	ID_LENGTH_RND,		//長さのランダムさ

	ID_Y,		//タッチが発生する数
	ID_COLOR,				//タッチ線の色
	ID_OPACITY,				//タッチ線の色
	ID_OPACITY_RND,				//タッチ線の色

	ID_POINT_COUNT,		//タッチが発生する数
	ID_POINT_LENGTH,		//タッチが発生する範囲
	ID_ORG,

	ID_NUM_PARAMS
	};

//プラグイン独自のパラメータを集めた構造体
#define GSeedLength 4

typedef struct ParamInfo{
	A_long			seed;
	PF_FpShort		centeX;
	PF_FpShort		centeY;
	PF_FpShort		length_max;
	PF_FpShort		length_rnd;
	A_long			value;
	A_long			value_left;
	A_long			value_right;
	A_long			value_top;
	A_long			value_bottom;

	PF_Pixel		color;
	PF_FpShort		opacity;
	PF_FpShort		opacity_rnd;
	A_long			point_count;
	PF_FpShort		point_length;
	PF_Boolean		org;
} ParamInfo;

typedef struct PsetAlphaPrm{
	PF_FpShort		x;
	PF_FpShort		y;
	PF_FpShort		opacity;
} PsetAlphaPrm; 

typedef struct TouchDrawCenterInfo{
	PF_FpShort		startX;
	PF_FpShort		startY;
	PF_FpShort		centerX;
	PF_FpShort		centerY;
	PF_FpShort		length;
	PF_FpShort		opacity;

} TouchDrawCenterInfo;

typedef struct copyImageInfo8{
	PF_Pixel	col;
	PF_Boolean	org;
} copyImageInfo8; 

typedef struct copyImageInfo16{
	PF_Pixel16	col;
	PF_Boolean	org;
} copyImageInfo16; 

typedef struct copyImageInfo32{
	PF_PixelFloat	col;
	PF_Boolean	org;
} copyImageInfo32; 



//-------------------------------------------------------
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

//-------------------------------------------------------
void drawTouch8(CFsGraph *g, ParamInfo *info);
void AlphaBlur8(CFsGraph *g);
PF_Err CopyImage8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP);

void drawTouch16(CFsGraph *g, ParamInfo *info);
void AlphaBlur16(CFsGraph *g);
PF_Err CopyImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP);

void drawTouch32(CFsGraph *g, ParamInfo *info);
void AlphaBlur32(CFsGraph *g);
PF_Err CopyImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP);

#endif // TouchDrawCenter_H
