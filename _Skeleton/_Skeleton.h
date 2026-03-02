//-----------------------------------------------------------------------------------
/*
	_Skeleton for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef _Skeleton_H
#define _Skeleton_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\json.hpp"
#include "..\_NFLib\NF_Json.h"
#include "..\_NFLib\NF_Utils.h"

#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_AlphaHyperbolic.h"
#include "..\_NFLib\fx\NF_Bilateral.h"
#include "..\_NFLib\fx\NF_Blend.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"
#include "..\_NFLib\fx\NF_DrawWorld.h"
#include "..\_NFLib\fx\NF_Gradient.h"
#include "..\_NFLib\fx\NF_Hyperbolic.h"
#include "..\_NFLib\fx\NF_LogicGray.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_Mosaic.h"
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_SpatUtils.h"
#include "..\_NFLib\fx\NF_UnMult.h"

#include "..\_NFLib\vector\NF_VectorLine.h"
#include "..\_NFLib\vector\NF_VectorMask.h"


#include "noiseFilter.h"
#include <string>

enum {
	MODE_NOISE = 1,
	MODE_RECT,
	MODE_LINE,
	MODE_PAINT,
	MODE_MINMAX,
	MODE_BLUR,
	MODE_CHAN_MINMAX,
	MODE_DRAW,
	MODE_DEBUG_FONT,
	MODE_NUM
};

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MODE,		//モード用のポップアップ
	
	// -----
	ID_AUTO_SEED,		//ノイズのシードを自動で変化させるかどうかのチェックボックス
	ID_SEED,			//ノイズのシード値の数値入力
	// -----
	ID_NOISE_TOPIC,
	ID_NOISE_VALUE,
	ID_NOISE_ISCOLOR,
	ID_NOISE_TOPIC_END,
	// -----
	ID_RECT_TOPIC,
	ID_RECT_POS1,
	ID_RECT_POS2,
	ID_RECT_POS3,
	ID_RECT_POS4,
	ID_RECT_COLOR,
	ID_RECT_TOPIC_END,
	// -----
	ID_LINE_TOPIC,
	ID_LINE_POS1,
	ID_LINE_W1,
	ID_LINE_POS2,
	ID_LINE_W2,
	ID_LINE_POS3,
	ID_LINE_W3,
	ID_LINE_POS4,
	ID_LINE_W4,
	ID_LINE_COLOR,
	ID_LINE_TOPIC_END,


	
	ID_NUM_PARAMS
};

//UIの表示文字列

//-------
#define	STR_MODE			"mode"
#define	STR_MODE_ITEMS		"noise|rect|line"
#define	STR_MODE_COUNT		3
#define	STR_MODE_DFLT		MODE_NOISE

#define	STR_AUTO_SEED		"autoSeed"
#define	STR_SEED			"seed"


//-------
#define	STR_NOISE_TOPIC		"noise"
#define	STR_NOISE_VALUE		"noiseValue"
#define	STR_NOISE_ISCOlOR	"noiseColor"
//-------
#define	STR_RECT_TOPIC		"rect"
#define	STR_RECT			"point_"
#define	STR_RECT_COLOR		"rect_clor"
//-------
#define	STR_LINE_TOPIC		"line"
#define	STR_LINE			"point_"
#define	STR_LINE_W			"weight_"
#define	STR_LINE_COLOR		"rect_clor"



#define	STR_PAINT_CB		"paint"
#define	STR_PAINT_POS		"paintPos"
#define	STR_PAINT_COLOR		"paintColor"

#define	STR_MINMAX			"minmax"
#define	STR_BLUR			"blur"

#define	STR_CHAN_MINMAX_MODE	"channelMinMax"
#define	STR_CHAN_MINMAX_ITEMS		"r|g|b|a"
#define	STR_CHAN_MINMAX_COUNT		4
#define	STR_CHAN_MINMAX_DFLT		1
#define	STR_CHAN_MINMAX		"channelBlur"



#define	STR_DRAW_POP		"draw"
#define	STR_DRAW_ITEMS		"none|line|box|boxfill|Circle|CircleFill|DrawLineAA"
#define	STR_DRAW_COUNT		7
#define	STR_DRAW_DFLT		1
#define	STR_START_POS		"startPos"
#define	STR_END_POS			"endPos"
#define	STR_DRAW_COLOR		"drawColor"
#define	STR_START_OPA		"startOpacity"
#define	STR_END_OPA			"endOpacity"

#define	STR_DEBUG_FONT_CB	"debugFont"
#define	STR_DEBUG_FONT_POS	"debugPos"
#define	STR_DEBUG_FONT_COLOR "debugColor"


#define	STR_HIDDEN_ON1		"oba-Q!"

#define	STR_HIDDEN_ON		"Menyo--!"
#define	STR_TOPIC			"Sample Topics"
#define	STR_ANGLE			"Angle"

#define	STR_BUTTON1			"button"
#define	STR_BUTTON2			"push"

//UIのパラメータ
typedef struct ParamInfo {
	A_long		mode;
	PF_Boolean	auto_seed;
	A_long		seed;

	PF_FpLong	noise;
	PF_Boolean	noise_is_color;


	PF_Point	rect_pos[4];
	PF_Pixel	rect_color;

	a_linePrm	line_pos[4];
	PF_Pixel	line_color;
	/*
	PF_Boolean	paint_cb;
	PF_Pixel	paint_color;
	PF_Point	paint_pos;

	A_long		minmax;
	A_long		blur;
	A_long		chan_minmax_mode;
	A_long		chan_minmax_value;
	
	PF_Boolean	noise_auto;
	A_long		noise_seed;

	A_long		draw;
	PF_Point	draw_start;
	PF_Point	draw_end;
	PF_Pixel	draw_color;
	PF_FpLong	draw_start_opa;
	PF_FpLong	draw_end_opa;

	PF_Boolean	debug_font_cb;
	PF_Point	debug_font_pos;
	PF_Pixel	debug_font_color;
	*/
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
#endif // _Skeleton_H

