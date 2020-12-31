//-----------------------------------------------------------------------------------
/*
	F's Plugins-cs4 for VS2008
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef LineDetection_H
#define LineDetection_H

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


#define STR_CB				"ON"

#define STR_DELTA_CB		"RGB差分検出"
#define STR_DELTA_OPACITY	"RGB差分検出の濃度"

#define STR_ALPHA_CB		"Alpha差分検出"
#define STR_ALPHA_OPACITY	"Alpha差分検出の濃度"

#define STR_LINE_COLOR		"検出した線の色"

/*
#define STR_TOPIC			"Extra Colors"
#define STR_COLOR_OPACITY	"カラー検出の濃度"
#define STR_COLOR0_CB		"Color0_Enabled"
#define STR_COLOR0			"Color0"
#define STR_COLOR0_LEVEL	"Color0_Range"
#define STR_COLOR1_CB		"Color1_Enable"
#define STR_COLOR1			"Color1"
#define STR_COLOR1_LEVEL	"Color1_Range"
#define STR_COLOR2_CB		"Color2_Enable"
#define STR_COLOR2			"Color2"
#define STR_COLOR2_LEVEL	"Color2_Range"
#define STR_COLOR3_CB		"Color3_Enable"
#define STR_COLOR3			"Color3"
#define STR_COLOR3_LEVEL	"Color3_Range"
#define STR_COLOR4_CB		"Color4_Enable"
#define STR_COLOR4			"Color4"
#define STR_COLOR4_LEVEL	"Color4_Range"
#define STR_COLOR5_CB		"Color5_Enable"
#define STR_COLOR5			"Color5"
#define STR_COLOR5_LEVEL	"Color5_Range"
#define STR_COLOR6_CB		"Color6_Enable"
#define STR_COLOR6			"Color6"
#define STR_COLOR6_LEVEL	"Color6_Range"
#define STR_COLOR7_CB		"Color7_Enable"
#define STR_COLOR7			"Color7"
#define STR_COLOR7_LEVEL	"Color7_Range"
*/



//AfterEffectsの情報をまとめた構造体
typedef struct{
	PF_InData		*in_data;
	PF_OutData		*out_data;

	PF_EffectWorld 	*input;
	PF_EffectWorld	*output;
	PF_PixelPtr  	inData;			//入力画像のアドレス	
	PF_PixelPtr  	outData;		//出力画像のアドレス	
	long			width;					//対象画像の横幅(pixel)
	long			height;					//対象画像の縦	幅(pixel)
	
	long			inWidth;				//入力画像の横幅の実サイズpixel)
	long			outWidth;				//出力画像の横幅の実サイズpixel)
	long			offsetInWidth;	// inWidth - width	補正サイズ	
	long			offsetOutWidth;	// outWidth - width
	long			Frame;					//描画中のフレーム（０スタート）
	PF_Boolean		is16Bit;		//モードが16BitならTRUE
	unsigned char	reserve1;
	unsigned short	reserve2;
	long			reserve3;
	long			reserve4;
	/* 無理やり64byteにする*/
} FsAEParams;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 

	ID_DELTA_CB,
	ID_DELTA_OPACITY,

	ID_ALPHA_CB,
	ID_ALPHA_OPACITY,

	
	ID_LINE_COLOR,

	/*
	ID_TOPIC,

	ID_COLOR_OPACITY,

	ID_COLOR0_CB,
	ID_COLOR0,	//ターゲットの色
	ID_COLOR0_LEVEL,	//ターゲットの色

	ID_COLOR1_CB,
	ID_COLOR1,	//ターゲットの色
	ID_COLOR1_LEVEL,	//ターゲットの色

	ID_COLOR2_CB,
	ID_COLOR2,	//ターゲットの色
	ID_COLOR2_LEVEL,	//ターゲットの色

	ID_COLOR3_CB,
	ID_COLOR3,	//ターゲットの色
	ID_COLOR3_LEVEL,	//ターゲットの色

	ID_COLOR4_CB,
	ID_COLOR4,	//ターゲットの色
	ID_COLOR4_LEVEL,	//ターゲットの色

	ID_COLOR5_CB,
	ID_COLOR5,	//ターゲットの色
	ID_COLOR5_LEVEL,	//ターゲットの色

	ID_COLOR6_CB,
	ID_COLOR6,	//ターゲットの色
	ID_COLOR6_LEVEL,	//ターゲットの色

	ID_COLOR7_CB,
	ID_COLOR7,	//ターゲットの色
	ID_COLOR7_LEVEL,	//ターゲットの色

	ID_TOPIC_END,
	*/
	ID_NUM_PARAMS
	};

#define COLOR_COUNT 8
//プラグイン独自のパラメータを集めた構造体
typedef struct{
	PF_Boolean		delta_flg;
	PF_Fixed		delta_opacity;
	PF_Boolean		alpha_flg;
	PF_Fixed		alpha_opacity;
	//PF_Fixed		color_opacity;
	PF_Pixel		line_color;
	/*
	long			tbl_count;
	PF_Pixel		color_tbl[COLOR_COUNT];
	PF_Fixed		level_tbl[COLOR_COUNT];
	*/
} ParamInfo;

typedef struct {
	PF_PixelPtr		data;
	long			width;
	long			height;
	long			widthTrue;
	long			widthOffset;
	PF_FixedPoint	pos;
	PF_Fixed		opacity;
} PsetPrm; 

typedef struct {
	PF_PixelPtr		data;
	long			width;
	long			height;
	long			widthTrue;
	long			widthOffset;
	long			x;
	long			y;
	long			w;
	long			h;
} BlockPrm; 

typedef struct {
	PsetPrm			pp;
	PF_FixedPoint	start;
	PF_FixedPoint	center;
	PF_Fixed		length_i;
	PF_Fixed		length_o;
	PF_Fixed		opacity;
} LineDetectionInfo;

typedef struct {
	PF_PixelPtr		data;
	long			width;
	long			height;
	long			widthTrue;
	long			widthOffset;
	long			x;
	long			y;
	long			adr;
} PointPrm; 



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
//-------------------------------------------------------
// FsAEParams構造体を作成する
PF_Err getFsAEParams (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	FsAEParams		*aeprm
	);
//-------------------------------------------------------
PF_Err getParams (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	ParamInfo		*prm
	);


void FindTarget8(CFsAE *ae , ParamInfo *infoP);
void FindTargetDelta8(CFsAE *ae , ParamInfo *infoP);

void FindTarget16(CFsAE *ae , ParamInfo *infoP);
void FindTargetDelta16(CFsAE *ae , ParamInfo *infoP);

void FindTarget32(CFsAE *ae , ParamInfo *infoP);
void FindTargetDelta32(CFsAE *ae , ParamInfo *infoP);


#endif // LineDetection_H
