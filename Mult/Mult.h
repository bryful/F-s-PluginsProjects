#pragma once
#ifndef Mult_H
#define Mult_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"
#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_UnMult.h"

//UIのパラメータ
typedef struct ParamInfo {
	A_long		Mode;
} ParamInfo, * ParamInfoP, ** ParamInfoH;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_MODE,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_MODE_POP		"Mode"
#define	STR_MODE_ITEMS		"Unmult|Mult|ToStraight|ToPremultiplited"
#define	STR_MODE_COUNT		4
#define	STR_MODE_DFLT		1


// 関数定義
/*
static PF_Err
Mult8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP);
static PF_Err
Mult16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP);
static PF_Err
Mult32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP);
static PF_Err
Unmult8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP);
static PF_Err
Unmult16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP);
static PF_Err
Unmult32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP);
static PF_Err
ToStraight8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP);
static PF_Err
ToStraight16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP);
static PF_Err
ToStraight32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP);
static PF_Err
ToPremult8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP);
static PF_Err
ToPremult16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP);
static PF_Err
ToPremult32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP);
*/

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
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			void* extraP);

}
#endif
