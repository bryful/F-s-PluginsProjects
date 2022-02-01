//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once

#ifndef Star_H
#define Star_H

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


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	
	ID_LENGTH_PAR,
	ID_STRONG_PAR,
	ID_ROT,

	ID_COLOR_TOPIC,
	ID_COLOR_KIND,
	ID_COLOR_A,
	ID_COLOR_B,
	ID_COLOR_BORDER,
	ID_COLOR_TOPIC_END,
	
	ID_LINE1_TOPIC,
	ID_LINE1_LENGTH,
	ID_LINE1_STRONG,
	ID_LINE1_ROT,
	ID_LINE1_TOPIC_END,
	
	ID_LINE2_TOPIC,
	ID_LINE2_LENGTH,
	ID_LINE2_STRONG,
	ID_LINE2_ROT,
	ID_LINE2_TOPIC_END,
	
	ID_LINE3_TOPIC,
	ID_LINE3_LENGTH,
	ID_LINE3_STRONG,
	ID_LINE3_ROT,
	ID_LINE3_TOPIC_END,
	
	ID_LINE4_TOPIC,
	ID_LINE4_LENGTH,
	ID_LINE4_STRONG,
	ID_LINE4_ROT,
	ID_LINE4_TOPIC_END,
	
	ID_NUM_PARAMS
	};

//	ID_COLOR_KIND
enum {
	skAtoB = 1,
	skBtoA,
	skAonly,
	skBonly,
	skScreen,
};

#define	STR_LENGTH_PAR		"Length(%)"
#define	STR_STRONG_PAR		"Strong(%)"
#define	STR_ROT				"Rot"
#define	STR_COLOR_TOPIC		"Color"
#define	STR_COLOR_KIND		"Kind"
#define	STR_COLOR_KIND_POP	"A To B|B To A|A Only|B Only|Screen"
#define	STR_COLOR_KIND_CNT	5
#define	STR_COLOR_KIND_DEF	1
#define	STR_COLOR_A			"Color A"
#define	STR_COLOR_B			"Color B"
#define	STR_COLOR_BORDER	"Color Border(%)"

#define	STR_LINE1_TOPIC		"Line1"
#define	STR_LINE1_LENGTH	"Line1 Length"
#define	STR_LINE1_STRONG	"Line1 Strong"
#define	STR_LINE1_ANGLE		"Line1 Rot"

#define	STR_LINE2_TOPIC		"Line2"
#define	STR_LINE2_LENGTH	"Line2 Length"
#define	STR_LINE2_STRONG	"Line2 Strong"
#define	STR_LINE2_ANGLE		"Line2 Rot"

#define	STR_LINE3_TOPIC		"Line3"
#define	STR_LINE3_LENGTH	"Line3 Length"
#define	STR_LINE3_STRONG	"Line3 Strong"
#define	STR_LINE3_ANGLE		"Line3 Rot"

#define	STR_LINE4_TOPIC		"Line4"
#define	STR_LINE4_LENGTH	"Line4 Length"
#define	STR_LINE4_STRONG	"Line4 Strong"
#define	STR_LINE4_ANGLE		"Line4 Rot"

typedef struct LineInfo{
	A_long		length;
	PF_FpShort	strong;
	PF_Fixed	angle;
}LineInfo;

typedef struct ColorTableInfo{
	A_long		kind;
	PF_Pixel	col1;
	PF_Pixel	col2;
	PF_FpShort	border;
	A_long		maxLength;
}ColorTableInfo;

typedef struct ParamInfo{
	ColorTableInfo	colTable;
	LineInfo	line[4];
} ParamInfo;


typedef struct ColorTableInfo8{
	PF_Pixel		col1;
	PF_Pixel		col2;
	PF_FpShort		border;
	A_long			length;
	PF_FpShort		strong;
	PF_Field		angle;
	A_long			value;
	PF_InData		*in_data;
}ColorTableInfo8;

typedef struct ColorTableInfo16{
	PF_Pixel16		col1;
	PF_Pixel16		col2;
	PF_FpShort		border;
	A_long			length;
	PF_FpShort		strong;
	PF_Field		angle;
	A_long			value;
	PF_InData		*in_data;
}ColorTableInfo16;

typedef struct ColorTableInfo32{
	PF_PixelFloat	col1;
	PF_PixelFloat	col2;
	PF_FpShort		border;
	A_long			length;
	PF_FpShort		strong;
	PF_Field		angle;
	A_long			value;
	PF_InData		*in_data;
}ColorTableInfo32;


typedef struct{
	ColorTableInfo8	colTable;
	PF_Pixel		*tbl;
	LineInfo		line[4];
	PF_InData		*in_data;
	CFsGraph		*g;
	A_long			x;
	A_long			y;
} ParamInfo8;

typedef struct{
	ColorTableInfo16	colTable;
	PF_Pixel16		*tbl;
	LineInfo		line[4];
	PF_InData		*in_data;
	CFsGraph		*g;
	A_long			x;
	A_long			y;
} ParamInfo16;

typedef struct{
	ColorTableInfo32	colTable;
	PF_PixelFloat		*tbl;
	LineInfo		line[4];
	PF_InData		*in_data;
	CFsGraph		*g;
	A_long			x;
	A_long			y;
} ParamInfo32;

//**************************************************************************
PF_Err	TargetCopy (CFsAE *ae);
void	MinMask(CFsAE *ae,A_long min);
PF_Err SubtrackAlpha (CFsAE *ae);
PF_Err ScreenCopy (CFsAE *ae);
PF_Err FillColorCopy (CFsAE *ae,PF_Pixel c);

PF_Err StarDraw (CFsAE *ae, ParamInfo *infoP);

PF_Err AlphaMake (CFsAE *ae);
//**************************************************************************

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



//-------------------------------------------------------
#endif // Star_H
