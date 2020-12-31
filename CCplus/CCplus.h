//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once

#ifndef CCplus_H
#define CCplus_H

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


#define COLOR_TABLE_LENGTH8		(PF_MAX_CHAN8+1)
#define COLOR_TABLE_LENGTH16	(PF_MAX_CHAN16+1)

#define COLOR_TABLE_SIZE8		((COLOR_TABLE_LENGTH8)  * sizeof(PF_Pixel))
#define COLOR_TABLE_SIZE16		((COLOR_TABLE_LENGTH16) * sizeof(PF_Pixel16))


#define	StrName					"CCplus"
#define	StrDescription			"CCplus bry-ful AfterEffects EffectPlugin"
#define	StrMADEBY				"bry-ful"

#define	StrORG_REV1				"original_reverce"	//PF_Pixel
#define	StrORG_REV2				"on"	//PF_Pixel

#define	StrCOLOR_START			"start_color"
#define	StrCOLOR_CENTER			"center_color"//PF_Pixel
#define	StrCOLOR_END			"end_color"//PF_Pixel
#define	StrCOLOR_CENTER_POS		"center_color_pos(%)"

#define	StrALPHA_START			"start_alpha(%)"	//透明にする
#define	StrALPHA_END			"end_alpha(%)"//透明にする

#define	StrNOISE_VALUE			"noise_value"//ノイズを強さ

#define	StrERR_getFsAEParams	"画像バッファーサイズエラーです。御免なさい。"
#define	StrERR_getParams		"パラメータエラーです。御免なさい。"




//AfterEffectsの情報をまとめた構造体
typedef struct{
	PF_InData		*in_data;
	PF_OutData		*out_data;

	PF_EffectWorld 	*input;
	PF_EffectWorld	*output;
	PF_PixelPtr  	inData;			//入力画像のアドレス	
	PF_PixelPtr  	outData;		//出力画像のアドレス	
	A_long			width;					//対象画像の横幅(pixel)
	A_long			height;					//対象画像の縦	幅(pixel)
	
	A_long			inWidth;				//入力画像の横幅の実サイズpixel)
	A_long			outWidth;				//出力画像の横幅の実サイズpixel)
	A_long			offsetInWidth;	// inWidth - width	補正サイズ	
	A_long			offsetOutWidth;	// outWidth - width
	A_long			Frame;					//描画中のフレーム（０スタート）
	PF_Boolean		is16Bit;		//モードが16BitならTRUE
	A_u_char		reserve1;
	A_u_short		reserve2;
	A_long			reserve3;
	A_long			reserve4;
	/* 無理やり64byteにする*/
} FsAEParams;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 

	ID_ORG_REV,		//元絵を白黒反転

	ID_COLOR_START,		//PF_Pixel
	ID_COLOR_CENTER,	//PF_Pixel
	ID_COLOR_END,		//PF_Pixel
	ID_COLOR_CENTER_POS,

	ID_ALPHA_START,		//透明にする
	ID_ALPHA_END,		//透明にする

	ID_NOISE_VALUE,		//ノイズの量
	ID_NUM_PARAMS

	};
/*
//プラグイン独自のパラメータを集めた構造体
typedef struct{
	PF_Boolean		org_rev;
	
	PF_Pixel  		color_start;
	PF_Pixel  		color_center;
	PF_Pixel  		color_end;
	PF_Fixed		color_center_pos;

	PF_Fixed		alpha_start;
	PF_Fixed		alpha_end;

	A_long			noise_value;

	PF_PixelPtr		colorTbl;
	PF_Handle		colorTblH;
		
} ID_Params;
*/
typedef struct CCPInfo{
	PF_Boolean		org_rev;
	PF_Pixel		color_start;
	PF_Pixel		color_center;
	PF_Pixel		color_end;
	
	A_u_char		color_border;
	A_long			alpha_start;
	A_long			alpha_end;
	A_long			noise_value;
	PF_Pixel		*colorTbl;
} CCPInfo, *CCPInfoP, **CCPInfoH;

typedef struct CCPInfo16{
	PF_Boolean		org_rev;
	PF_Pixel16		color_start;
	PF_Pixel16		color_center;
	PF_Pixel16		color_end;
	
	A_u_short		color_border;
	A_long			alpha_start;
	A_long			alpha_end;
	A_long			noise_value;
	PF_Pixel16		*colorTbl;
} CCPInfo16, *CCPInfo16P, **CCPInfo16H;


typedef struct CCPInfo32{
	PF_Boolean		org_rev;
	PF_PixelFloat	color_start;
	PF_PixelFloat	color_center;
	PF_PixelFloat	color_end;
	
	PF_FpShort		color_border;

	PF_FpShort		alpha_start;
	PF_FpShort		alpha_end;
	A_long			noise_value;
	PF_PixelFloat	colorTbl;
} CCPInfo32, *CCPInfo32P, **CCPInfo32H;

typedef struct {
	PF_Fixed	value;
	PF_ProgPtr	ref;
	PF_SampPB	samp_pb;
	PF_InData	*in_data;
} CCplusInfo;



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

#endif // CCplus_H
