//-----------------------------------------------------------------------------------
/*
	CellLineEraser for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef CellLineEraser_H
#define CellLineEraser_H

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

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_Color_COUNT,
	ID_Color1,
	ID_Color2,
	ID_Color3,
	ID_Color4,
	ID_Color5,
	ID_Color6,
	ID_Color7,
	ID_Color8,
	ID_Color9,
	ID_Color10,
	ID_KEEP_PIXELS,
	ID_Fill_Unknown_Colors,
	ID_Fill_Color,
	ID_MakeWhiteTransparent,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_Use			"TargetColorCount"
#define	STR_Color1		"color1"
#define	STR_Color2		"color2"
#define	STR_Color3		"color3"
#define	STR_Color4		"color4"
#define	STR_Color5		"color5"
#define	STR_Color6		"color6"
#define	STR_Color7		"color7"
#define	STR_Color8		"color8"
#define	STR_Color9		"color9"
#define	STR_Color10		"color10"
#define	STR_KEEP_PIXELS	"KeepPixels"
#define	STR_FillUnknownColors	"FillUnknownColors"
#define	STR_FillColor	"FilColor"
#define	STR_MakeWhiteTransparent	"Make White Transparent"


//UIのパラメータ
typedef struct ParamInfo {
	A_long			TargetColorCount;
	PF_Pixel  		Colors[16];
	PF_Boolean		KeepPixel;
	PF_Boolean		FillUnknownColors;
	PF_Pixel  		GiveUpColor;
	PF_Boolean		MakeWhiteTransparent;

	A_long			count;
	PF_PixelPtr		scanline;
	A_long			scanLength;
	A_long			scanCount;
} ParamInfo, *ParamInfoP, **ParamInfoH;





//-------------------------------------------------------
#define PPNORMAL 255
#define PPTRANS 256
#define PPMAIN 257

#define PS_NORMAL 0
#define PS_TRANS 1
#define PS_MAIN 2

inline A_long PxStatus(A_long v)
{
	if (v >= PPMAIN) {
		return PS_MAIN;
	}
	else if (v == PPTRANS)
	{
		return PS_TRANS;
	}
	else
	{
		return PS_NORMAL;
	}
}
//-------------------------------------------------------
inline A_long pV8(PF_Pixel p,ParamInfo* infoP)
{
	A_long ret = PF_MAX_CHAN8;
	if (p.alpha == 0) {
		ret = PPTRANS;
		return ret;
	}
	for (int i = 0; i < infoP->TargetColorCount; i++)
	{
		if (compPix8(p, infoP->Colors[i]) == TRUE)
		{
			ret = PPMAIN;
			return ret;
		}
	}
	double r = (double)p.red / PF_MAX_CHAN8;
	double g = (double)p.green / PF_MAX_CHAN8;
	double b = (double)p.blue / PF_MAX_CHAN8;
	ret= (A_long)(((0.29891 * r) + (0.58661 * g) + (0.11448 * b)) * PF_MAX_CHAN8+0.5);
	if (ret > PF_MAX_CHAN8) 
		ret = PF_MAX_CHAN8;
	else if (ret < 0)
		ret = 0;

	return ret;
}
//-------------------------------------------------------
inline A_long pV16(PF_Pixel16 p, ParamInfo* infoP)
{
	A_long ret = PF_MAX_CHAN8;
	if (p.alpha == 0) {
		ret = PPTRANS;
		return ret;
	}
	PF_Pixel p2 = CONV16TO8(p);
	for (int i = 0; i < infoP->TargetColorCount; i++)
	{
		if (compPix8(p2, infoP->Colors[i]) == TRUE)
		{
			ret = PPMAIN;
			return ret;
		}
	}
	double r = (double)p.red / PF_MAX_CHAN16;
	double g = (double)p.green / PF_MAX_CHAN16;
	double b = (double)p.blue / PF_MAX_CHAN16;
	ret = (A_long)(((0.29891 * r) + (0.58661 * g) + (0.11448 * b)) * PF_MAX_CHAN8 + 0.5);
	if (ret > PF_MAX_CHAN8)
		ret = PF_MAX_CHAN8;
	else if (ret < 0)
		ret = 0;

	return ret;
}
//-------------------------------------------------------
inline A_long pV32(PF_PixelFloat p, ParamInfo* infoP)
{
	A_long ret = PF_MAX_CHAN8;
	if (p.alpha == 0) {
		ret = PPTRANS;
		return ret;
	}
	PF_Pixel p2 = CONV32TO8(p);
	for (int i = 0; i < infoP->TargetColorCount; i++)
	{
		if (compPix8(p2, infoP->Colors[i]) == TRUE)
		{
			ret = PPMAIN;
			return ret;
		}
	}
	double r = (double)p.red;
	if (r > 1) r = 1;
	double g = (double)p.green;
	if (g > 1) g = 1;
	double b = (double)p.blue;
	if (b > 1) b = 1;
	ret = (A_long)(((0.29891 * r) + (0.58661 * g) + (0.11448 * b)) * PF_MAX_CHAN8 + 0.5);
	if (ret > PF_MAX_CHAN8)
		ret = PF_MAX_CHAN8;
	else if (ret < 0)
		ret = 0;

	return ret;
}
//-----------------------------------------------------------------------------------

 PF_Err Exec8(CFsAE* ae, ParamInfo* infoP);
PF_Err Exec16(CFsAE* ae, ParamInfo* infoP);
PF_Err Exec32(CFsAE* ae, ParamInfo* infoP);
PF_Err Giveup8(CFsAE* ae, ParamInfo* infoP);
PF_Err Giveup16(CFsAE* ae, ParamInfo* infoP);
PF_Err Giveup32(CFsAE* ae, ParamInfo* infoP);
PF_Err White8(CFsAE* ae, ParamInfo* infoP);
PF_Err White16(CFsAE* ae, ParamInfo* infoP);
PF_Err White32(CFsAE* ae, ParamInfo* infoP);

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
#endif // CellLineEraser_H

