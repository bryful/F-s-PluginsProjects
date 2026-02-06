//-----------------------------------------------------------------------------------
/*
	Grain for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Grain_H
#define Grain_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

/*
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_blur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Draw.h"
*/
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Spat_Data.h"

#include "_SkeltonFilter.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MASTER,
	ID_AMOUNT,
	ID_SIZE,
	
	ID_SUB_AMOUNT,
	ID_SUB_SIZE,

	ID_ACCENT_COUNT,
	ID_ACCENT_LOLIMIT,
	ID_ACCENT_INTENSITY,

	ID_MONOCHROME,
	ID_AUTO_SEED,
	ID_SEED,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MASTER			"masterPercent"
#define	STR_AMOUNT			"amount"
#define	STR_SIZE			"size"
#define	STR_SUB_AMOUNT		"subAmountPercent"
#define	STR_SUB_SIZE		"subSizePercent"

#define	STR_MONOCHROME		"monochrome"
#define	STR_MONOCHROME2		"on"

#define	STR_ACCENT_COUNT	"accntCout"
#define	STR_ACCENT_LOLIMIT	"accntLoLimitPercent"
#define	STR_ACCENT_INTENSITY	"accntintensity"


#define	STR_AUTO_SEED		"autoSeed"
#define	STR_AUTO_SEED2		"on"
#define	STR_SEED			"seed"


//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	master_percent;
	PF_FpLong	amount;
	PF_FpLong	size;
	PF_FpLong	sub_amount_percent;
	PF_FpLong	sub_size_percent;

	A_long		accentCount;
	PF_FpLong	accentCountLoLimit;
	PF_FpLong	accentIntensity;

	PF_Boolean	is_monochrome;
	PF_Boolean	is_autoSeed;
	A_long		seed;

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
#endif // Grain_H

