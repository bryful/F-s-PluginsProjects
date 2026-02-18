//-----------------------------------------------------------------------------------
/*
	InkingLineBlur for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef InkingLineBlur_H
#define InkingLineBlur_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
#include "..\_NFLib\fx\NF_Blend.h"
//#include "..\_NFLib\fx\debug_font.h"
//#include "..\_NFLib\fx\NF_Paint.h"
//#include "..\_NFLib\fx\NF_Noise.h"
//#include "..\_NFLib\fx\NF_Draw.h"

//#include "InkingLineBlurFilter.h"
#include <string>
#include "NF_SelectPixels.h"
#include "NF_TargetPixels.h"

#define COLOR_INKING_COUNT 4
#define COLOR_TARGET_COUNT 8
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_INKING_TOPIC,
	ID_INKING_COUNT,
	ID_INKING_COLOR1,
	ID_INKING_COLOR2,
	ID_INKING_COLOR3,
	ID_INKING_COLOR4,
	ID_INKING_COLOR_MODE,
	ID_CUSTUM_COLOR,
	ID_INKING_TOPIC_END,


	ID_MINMAX,
	ID_BLUR,
	ID_HYPERBOLIC,


	ID_TARGET_TOPIC,
	ID_TARGET_MODE,
	ID_TARGET_COLOR1,
	ID_TARGET_COLOR2,
	ID_TARGET_COLOR3,
	ID_TARGET_COLOR4,
	ID_TARGET_COLOR5,
	ID_TARGET_COLOR6,
	ID_TARGET_COLOR7,
	ID_TARGET_COLOR8,
	ID_TARGET_TOPIC_END,
	ID_BLEND,
	ID_NUM_PARAMS
};
#define ID_INKING_COLOR(IDX) (ID_INKING_COLOR1+IDX)
#define ID_TARGET_COLOR(IDX) (ID_TARGET_COLOR1+IDX)

//UIの表示文字列
#define	STR_INKING_TOPIC		"Inking Colors"
#define	STR_INKING_COUNT		"Inking Color Count"
#define	STR_INKING_ITEMS		"1|2|3|4"
#define	STR_INKING_DFLT			1
#define	STR_INKING_NUMS			4
#define	STR_INKING_COLOR		"Inking"


#define	STR_MINMAX			"MinMax"
#define	STR_BLUR			"Blur"
#define STR_HYPERBOLIC		"Hyperbolic"
#define	STR_INKING_COLOR_MODE	"Inking Color"
#define	STR_INKING_COLOR_ITEMS	"Original|CustumColor"
#define	STR_CUSTAM_COLOR			"CustumColor"

#define	STR_TARGET_TOPIC		"Target Colors"
#
//#define STR_INKING_COLOR_MODE	"Inking Color Mode"


#define	STR_TARGET_MODE			"TargetMode"
#define	STR_TARGET_ITEMS		"1|2|3|4|5|6|7|8|All"
#define	STR_TARGET_COLOR		"Target"
#define	STR_BLEND				"Blend with original"

//UIのパラメータ
typedef struct ParamInfo {
	A_long		inking_count;
	PF_Pixel	inking_colors[COLOR_INKING_COUNT];

	A_long		minmax;
	A_long		blur;
	PF_FpLong	hyperbolic;
	
	A_long		inking_color_mode;
	PF_Pixel	custum_color;

	PF_Boolean	isAll;	
	A_long		target_mode;
	PF_Pixel	target_color[COLOR_TARGET_COUNT];
	PF_Boolean	isBlend;

} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------


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
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}
#endif // InkingLineBlur_H

