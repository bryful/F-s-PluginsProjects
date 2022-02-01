//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once

#ifndef TouchDrawStraght_H
#define TouchDrawStraght_H

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

#include <math.h>

#include "../FsLibrary/FsAE.h"
//#include "FsAE.h"

#include "FsTDpset.h"


#define	STR_SEED			"RandomSeed"
#define	STR_TARGET_VALUE	"Value"
#define	STR_TARGET_TOPIC	"Target"
#define	STR_TARGET_MODE1	"Mode"
#define	STR_TARGET_MODE2	"Color|BrightnessDelta|AlphaDelta"
#define	STR_TARGET_COLOR	"Color"
#define	STR_COLOR_RANGE		"Color_Range"
#define	STR_DELTA_RANGE		"Delta_Range"
#define	STR_ROT				"Rot"
#define	STR_LENGTH_I_MAX	"Inside_Length"
#define	STR_LENGTH_I_RND	"Inside_Length_Random"
#define	STR_LENGTH_O_MAX	"Outside_Length"
#define	STR_LENGTH_O_RND	"Outside_Length_Random"
#define	STR_COLOR			"Color"
#define	STR_OPACITY			"Opacity"
#define	STR_OPACITY_RND		"Opacity_Random"

#define	STR_POINT_COUNT		"Point_Count"
#define	STR_POINT_LENGTH	"Point_Length"

#define	STR_BLOCK_VALUE		"Block_Value"
#define	STR_BLOCK_SIZE		"Block_Size"

#define	STR_ORG_CB1			"Original_Blend"
#define	STR_ORG_CB2			"ON"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	
	ID_SEED,			//ランダムの基点
	ID_TARGET_VALUE,	//タッチが発生する確率
	ID_TARGRT_TOPIC,
	ID_TARGET_MODE,		// 1:差分 2:color 
	ID_TARGET_COLOR,	//ターゲットの色
	ID_COLOR_RANGE,	//色の範囲
	ID_DELTA_RANGE,	//差分の範囲
	ID_TARGRT_TOPIC_END,

	ID_ROT,			//タッチ線の方向
	ID_LENGTH_I_MAX,	//タッチ線の長さin方向
	ID_LENGTH_I_RND,	//タッチ線の長さin方向のランダムさ
	ID_LENGTH_O_MAX,	//タッチ線の長さout方向
	ID_LENGTH_O_RND,	//タッチ線の長さout方向のランダムさ

	ID_COLOR,			//タッチ線の色
	ID_OPACITY,			//タッチ線の不透明度
	ID_OPACITY_RND,		//タッチ線の不透明度のランダム

	ID_POINT_COUNT,		//タッチが発生する数
	ID_POINT_LENGTH,	//タッチが発生する範囲

	ID_BLOCK_VALUE,
	ID_BLOCK_SIZE,

	ID_ORG,
	ID_NUM_PARAMS
	};

//プラグイン独自のパラメータを集めた構造体
typedef struct{
	A_long			seed;
	PF_Fixed		target_value;
	A_long			target_mode;
	PF_Pixel		target_color;
	PF_Fixed		color_range;
	PF_Fixed		delta_range;
	PF_Fixed		rot;
	A_long			length_i_max;
	PF_Fixed		length_i_rnd;
	A_long			length_o_max;
	PF_Fixed		length_o_rnd;

	PF_Pixel		color;
	PF_Fixed		opacity;
	PF_Fixed		opacity_rnd;
	A_long			point_count;
	PF_Fixed		point_length;
	A_long			target_count;
	PF_Fixed		block_value;
	A_long			block_count;
	A_long			block_size;
	PF_Boolean		org;

} ParamInfo;



typedef struct {
	PsetPrm			pp;
	PF_FixedPoint	start;
	PF_Fixed		rot;
	PF_Fixed		length_i;
	PF_Fixed		length_o;

	PF_Fixed		opacity;
	PF_InData		*in_data;

} TouchDrawStraghtInfo;

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
	void			*extraP);
}

//blue
PF_Err FindTarget(CFsAE *ae , PF_Pixel target_color, A_long color_range);
PF_Err FindTargetDeltaBright(CFsAE *ae , A_long delta_range);
PF_Err FindTargetDeltaAlpha(CFsAE *ae , A_long delta_range);

//blue
PF_Err blockDraw(CFsAE *ae , A_long bSize, A_long bCount,A_long seed);


PF_Err greenBlur(CFsAE *ae);

//green -> RGB
PF_Err copyAlpha(CFsAE *ae , PF_Pixel color, PF_Boolean org);

void drawTouchS8(CFsAE *ae,ParamInfo	*infoP);
void drawTouchS16(CFsAE *ae,ParamInfo	*infoP);
void drawTouchS32(CFsAE *ae,ParamInfo	*infoP);

void drawTouchSub32(TouchDrawStraghtInfo *p);
void drawTouchSub16(TouchDrawStraghtInfo *p);
void drawTouchSub8(TouchDrawStraghtInfo *p);


#endif // TouchDrawStraght_H
