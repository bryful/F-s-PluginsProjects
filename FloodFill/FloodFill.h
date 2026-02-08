//-----------------------------------------------------------------------------------
/*
	FloodFill for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef FloodFill_H
#define FloodFill_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"


#include <string>
//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる

#define PAINT_COLOR_NUM 16
enum {
	ID_INPUT = 0,	// default input layer

	ID_PAINT_COUNT,
	ID_PAINT_TOPIC,

	ID_ENABLE0,
	ID_GUIDE0,
	ID_POS0,
	ID_COLOR0,

	ID_ENABLE1,
	ID_GUIDE1,
	ID_POS1,
	ID_COLOR1,

	ID_ENABLE2,
	ID_GUIDE2,
	ID_POS2,
	ID_COLOR2,

	ID_ENABLE3,
	ID_GUIDE3,
	ID_POS3,
	ID_COLOR3,

	ID_ENABLE4,
	ID_GUIDE4,
	ID_POS4,
	ID_COLOR4,

	ID_ENABLE5,
	ID_GUIDE5,
	ID_POS5,
	ID_COLOR5,

	ID_ENABLE6,
	ID_GUIDE6,
	ID_POS6,
	ID_COLOR6,

	ID_ENABLE7,
	ID_GUIDE7,
	ID_POS7,
	ID_COLOR7,

	ID_ENABLE8,
	ID_GUIDE8,
	ID_POS8,
	ID_COLOR8,

	ID_ENABLE9,
	ID_GUIDE9,
	ID_POS9,
	ID_COLOR9,

	ID_ENABLE10,
	ID_GUIDE10,
	ID_POS10,
	ID_COLOR10,

	ID_ENABLE11,
	ID_GUIDE11,
	ID_POS11,
	ID_COLOR11,

	ID_ENABLE12,
	ID_GUIDE12,
	ID_POS12,
	ID_COLOR12,

	ID_ENABLE13,
	ID_GUIDE13,
	ID_POS13,
	ID_COLOR13,

	ID_ENABLE14,
	ID_GUIDE14,
	ID_POS14,
	ID_COLOR14,

	ID_ENABLE15,
	ID_GUIDE15,
	ID_POS15,
	ID_COLOR15,

	ID_PAINT_TOPIC_END,
	ID_GUIDE_COLOR,
	ID_GUIDE_ALL_CLEAR,
	ID_NUM_PARAMS
};
#define ID_ENABLE(IDX) (ID_ENABLE0 + (IDX) * 4)
#define ID_POS(IDX) (ID_POS0 + (IDX) * 4)
#define ID_COLOR(IDX) (ID_COLOR0 + (IDX) * 4)
#define ID_GUIDE(IDX) (ID_GUIDE0 + (IDX) * 4)


//UIの表示文字列
#define	STR_PAINT_COUNT		"paintCount"
#define	STR_PAINT_ITEMS		"1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16"

#define	STR_PAINT_TOPIC		"paintItems"
#define	STR_ENABLE			"enable"
#define	STR_ENABLE_BAR		"----"
#define	STR_POS				"pos"
#define	STR_COLOR			"color"
#define	STR_GUIDE			"guide"
#define	STR_GUIDE_ALL_CLEAR			"All Guide Clear"
#define	STR_GUIDE_COLOR		"guideColor"



//UIのパラメータ
typedef struct ParamInfo {
	A_long		paintCount;

	PF_Boolean	paintEnables[PAINT_COLOR_NUM];
	PF_Point	paintPoints[PAINT_COLOR_NUM];
	PF_Pixel	paintColors[PAINT_COLOR_NUM];
	PF_Boolean	paintGuides[PAINT_COLOR_NUM];
	PF_Pixel	guideColor;
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
#endif // FloodFill_H

