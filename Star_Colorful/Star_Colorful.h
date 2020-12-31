//-----------------------------------------------------------------------------------
/*
	Star_Colorful for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Star_Colorful_H
#define Star_Colorful_H


#include <math.h>

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
#include "../FsLibrary/FsHLS.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_TOPIC0,
	ID_TAGET_KIND,
	ID_TAGET_BORDER,
	ID_TAGET_SOFT,
	ID_TARGET_COLOR,
	ID_TARGET_MASK,
	ID_TOPIC0_END,

	ID_TOPICA,
	ID_LENGTH,
	ID_OPACITY,

	ID_ROT,
	ID_AUTOROLLING,
	ID_ROLLINGDPEED,
	ID_TOPICA_END,


	ID_TOPIC1,
	ID_COLOR_KIND,
	ID_COLOR,
	ID_BRIGHTNESS,
	ID_RAINBOW_OFFSET,
	ID_RAINBOW_DELTA,
	ID_TOPIC1_END,


	ID_TOPIC2,
	ID_VER_LEN,
	ID_VER_ROT,
	ID_VER_OPACITY,

	ID_HOR_LEN,
	ID_HOR_ROT,
	ID_HOR_OPACITY,

	ID_DIA_LEN,
	ID_DIA_ROT,
	ID_DIA_ROT2,
	ID_DIA_OPACITY,


	ID_TOPIC2_END,

	ID_BLEND,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_TOPIC0				"Target"

#define	STR_TARGET_KIND			"Target"
#define	STR_TARGET_ITEMS		"RGBMax|R+G+B|Brightness|TargetColor"
#define	STR_TARGET_COUNT		4
#define	STR_TARGET_DFLT			1
#define	STR_TARGET_COLOR		"TargetColor"
#define	STR_TARGET_MASK			"TargetDisp"
#define	STR_ON					"on"

#define	STR_TAGET_BORDER		"TargetBorder"
#define	STR_TAGET_SOFT			"TargetSoftness"


#define	STR_TOPICA				"Control"
#define	STR_ROT					"Rot"
#define	STR_AUTOROLLING			"AutoRolling"
#define	STR_ON					"on"
#define	STR_ROLLINGSPEED		"RollingSpeed"

#define	STR_LENGTH				"Length"
#define	STR_OPACITY				"Opacity"

#define	STR_TOPIC1				"Color"
#define	STR_COLOR_KIND			"ColorKind"
#define	STR_COLOR				"Color"
#define	STR_BRIGHT				"ColorBrightness"
#define	STR_COLOR_ITEMS			"Color|Screen|Rainbow"
#define	STR_COLOR_COUNT			3
#define	STR_COLOR_DFLT			1

#define	STR_RAINBOW_OFFSET		"RainbowOffset"
#define	STR_RAINBOW_DELTA		"RainbowDelta"


#define	STR_TOPIC2				"StarDetail"
#define	STR_VER_LEN				"VerticalLength"
#define	STR_VER_ROT				"VerticalRot"
#define	STR_VER_OPACITY			"verticalOpacity"
#define	STR_HOR_LEN				"HorizonLength"
#define	STR_HOR_ROT				"HorizonRot"
#define	STR_HOR_OPACITY			"HorizonOpacity"

#define	STR_DIA_LEN				"DiagonalLength"
#define	STR_DIA_ROT				"DiagonalRot"
#define	STR_DIA_ROT2			"Diagonal2Rot"
#define	STR_DIA_OPACITY			"DiagonalOpacity"

#define	STR_BLEND				"Blemnd"
#define	STR_BLEND_ITEMS			"None|Screen|Add|Normal"
#define	STR_BLEND_COUNT			4
#define	STR_BLEND_DFLT			1



//UIのパラメータ
typedef struct ParamInfo {
	A_long		target_kind;
	PF_FpLong	target_border;
	PF_FpLong	target_softness;
	PF_Pixel	target_color;
	PF_Boolean	target_maskDraw;

	PF_FpLong	rot;
	PF_Boolean	autoRolling;
	PF_FpLong	rollingSpeed;

	PF_FpLong	length;
	PF_FpLong	opacity;
	PF_FpLong	brigthness;

	A_long		color_kind;
	PF_Pixel	color;
	PF_FpLong	rainbowOffset;
	PF_FpLong	rainbowDelta;

	A_long		verLength;
	PF_FpLong	verRot;
	PF_FpLong	verOpacity;
	A_long		horLength;
	PF_FpLong	horRot;
	PF_FpLong	horOpacity;
	A_long		diaLength;
	PF_FpLong	diaRot;
	PF_FpLong	diaRot2;
	PF_FpLong	diaOpacity;

	A_long		blend;

} ParamInfo, *ParamInfoP, **ParamInfoH;


typedef struct StarInfo {

	A_long		len;
	PF_FpLong	addX;
	PF_FpLong	addY;
	PF_FpLong	opa;
	PF_FpLong	rot;

}StarInfo;



typedef struct ParamInfo8 {
	ParamInfo	info;
	LABA		taget_lab;
	PF_FpLong	target_border;
	PF_FpLong	target_softness;
	PF_FpLong	target_start;

	StarInfo	hor;
	StarInfo	ver;
	StarInfo	dia;
	StarInfo	dia2;
	PF_FpLong	rot;

	PF_Pixel	color;

	GInfo		gin;
	GInfo		gout;
	GInfo		gtmp;

	PF_Handle	bufH;
	//A_u_char	*scanline;
	PF_Pixel	*horTable;
	PF_Pixel	*verTable;
	PF_Pixel	*diaTable;

} ParamInfo8, *ParamInfo8P, **ParamInfo8H;

typedef struct ParamInfo16 {
	ParamInfo	info;
	LABA		taget_lab;
	PF_FpLong	target_border;
	PF_FpLong	target_softness;
	PF_FpLong	target_start;

	StarInfo	hor;
	StarInfo	ver;
	StarInfo	dia;
	StarInfo	dia2;
	PF_FpLong	rot;

	PF_Pixel16	color;

	GInfo		gin;
	GInfo		gout;
	GInfo		gtmp;

	PF_Handle	bufH;
	//A_u_char	*scanline;
	PF_Pixel16	*horTable;
	PF_Pixel16	*verTable;
	PF_Pixel16	*diaTable;

} ParamInfo16, *ParamInfo16P, **ParamInfo16H;

typedef struct ParamInfo32 {
	ParamInfo	info;
	LABA		taget_lab;
	PF_FpLong	target_border;
	PF_FpLong	target_softness;
	PF_FpLong	target_start;

	StarInfo	hor;
	StarInfo	ver;
	StarInfo	dia;
	StarInfo	dia2;
	PF_FpLong	rot;

	PF_PixelFloat	color;

	GInfo		gin;
	GInfo		gout;
	GInfo		gtmp;

	PF_Handle	bufH;
	PF_PixelFloat	*horTable;
	PF_PixelFloat	*verTable;
	PF_PixelFloat	*diaTable;

} ParamInfo32, *ParamInfo32P, **ParamInfo32H;

//-------------------------------------------------------
PF_Err StarExec8(CFsAE *ae, ParamInfo *infoP);
PF_Err StarExec16(CFsAE *ae, ParamInfo *infoP);
PF_Err StarExec32(CFsAE *ae, ParamInfo *infoP);

//-----------------------------------------------------------------------------------
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
#endif // Star_Colorful_H
