//-----------------------------------------------------------------------------------
/*
	IroTore for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef IroTore_H
#define IroTore_H

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
#include "../FsLibrary/FsDebug.h"
#include "../FsLibrary/FsHLS.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	
	//境界線のみ
	ID_LINE_ONLY,
	//主線の色
	ID_COLOR1_ON,
	ID_COLOR1,
	ID_COLOR2_ON,
	ID_COLOR2,
	ID_COLOR3_ON,
	ID_COLOR3,
	ID_COLOR4_ON,
	ID_COLOR4,
	ID_COLOR5_ON,
	ID_COLOR5,
	ID_COLOR6_ON,
	ID_COLOR6,
	
	//主線検出レベル
	ID_LEVEL,
	
	//マッチしなかった時の色
	ID_IG_COLOR,
	
	//主線部分最大最小
	ID_LINE_MINMAX1,
	ID_LINE_MINMAX2,

	//主線部分にブラー
	ID_LINE_BLUR,
	
	//主線部分の色変え
	ID_LINE_H,
	ID_LINE_S,
	ID_LINE_L,
	ID_LINE_RED,
	ID_LINE_GREEN,
	ID_LINE_BLUE,

	ID_NEG_COLOR1_ON,
	ID_NEG_COLOR1,
	ID_NEG_COLOR2_ON,
	ID_NEG_COLOR2,
	ID_NEG_COLOR3_ON,
	ID_NEG_COLOR3,
	ID_NEG_COLOR4_ON,
	ID_NEG_COLOR4,


	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_LINE_ONLY		"LineOnly"
#define	STR_ON				"on"
#define	STR_COLOR1_ON		"EnabledColor1"
#define	STR_COLOR2_ON		"EnabledColor2"
#define	STR_COLOR3_ON		"EnabledColor3"
#define	STR_COLOR4_ON		"EnabledColor4"
#define	STR_COLOR5_ON		"EnabledColor5"
#define	STR_COLOR6_ON		"EnabledColor6"

#define	STR_COLOR1			"Color1"
#define	STR_COLOR2			"Color2"
#define	STR_COLOR3			"Color3"
#define	STR_COLOR4			"Color4"
#define	STR_COLOR5			"Color5"
#define	STR_COLOR6			"Color6"

#define	STR_LEVEL			"level"
#define	STR_IG_COLOR		"あきらめ色"

#define	STR_LINE_MINMAX1	"Min/Max1"
#define	STR_LINE_MINMAX2	"Min/Max2"
#define	STR_LINE_BLUR		"LineBlur"
#define	STR_LINE_H			"LineH"
#define	STR_LINE_S			"LineS"
#define	STR_LINE_L			"LineL"
#define	STR_LINE_R			"LineRed"
#define	STR_LINE_G			"LineGreen"
#define	STR_LINE_B			"LineBlue"

#define	STR_NEG_COLOR1_ON		"EnabledN1"
#define	STR_NEG_COLOR2_ON		"EnabledN2"
#define	STR_NEG_COLOR3_ON		"EnabledN3"
#define	STR_NEG_COLOR4_ON		"EnabledN4"

#define	STR_NEG_COLOR1			"NegColor1"
#define	STR_NEG_COLOR2			"NegColor2"
#define	STR_NEG_COLOR3			"NegColor3"
#define	STR_NEG_COLOR4			"NegColor4"

#define PF_HALF_CHAN32 (0.5)
#define PF_MAX_CHAN32 (1.0)

#define COLOR_MAX	6
#define NEG_COLOR_MAX	4
//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	lineOnly;
	A_long		colorMax;
	PF_Pixel	color[COLOR_MAX];
	A_long		ngColorMax;
	PF_Pixel	ngColor[NEG_COLOR_MAX];
	A_long		level;

	PF_Pixel	igColor;
	
	A_long		blur;
	A_long		minmax1;
	A_long		minmax2;
	PF_FpLong	hue;
	PF_FpLong	sat;
	PF_FpLong	light;
	PF_FpLong	red;
	PF_FpLong	green;
	PF_FpLong	blue;

	PF_Handle	scanlineH;
	PF_Pixel	*scanline;
	PF_Pixel	*data;
	A_long		targetCount;
	A_long		w;
	A_long		wt;
	A_long		wt2;
	A_long		h;
	A_long		offset;
	A_long		nowX;
	A_long		nowY;


} ParamInfo;
typedef struct ParamInfo16 {
	PF_Boolean	lineOnly;
	A_long		colorMax;
	PF_Pixel	color[COLOR_MAX];
	A_long		ngColorMax;
	PF_Pixel	ngColor[NEG_COLOR_MAX];
	A_long		level;

	PF_Pixel16	igColor;
	
	A_long		blur;
	A_long		minmax1;
	A_long		minmax2;
	PF_FpLong	hue;
	PF_FpLong	sat;
	PF_FpLong	light;
	PF_FpLong	red;
	PF_FpLong	green;
	PF_FpLong	blue;

	PF_Handle	scanlineH;
	PF_Pixel16	*scanline;
	PF_Pixel16	*data;
	A_long		targetCount;
	A_long		w;
	A_long		wt;
	A_long		wt2;
	A_long		h;
	A_long		offset;
	A_long		nowX;
	A_long		nowY;


} ParamInfo16;
//-------------------------------------------------------
//--------------------------------------------------------------------xFF--------------
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


PF_Err irotoreExec8(CFsAE *ae , ParamInfo *infoP);
PF_Err irotoreExec16(CFsAE *ae , ParamInfo16 *infoP);
#endif // IroTore_H
