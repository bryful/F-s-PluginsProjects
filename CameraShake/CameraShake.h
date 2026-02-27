//-----------------------------------------------------------------------------------
/*
	CameraShake for VS2026
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef CameraShake_H
#define CameraShake_H

#include "..\_NFLib\AE_SDK.h"

#include "NF_Target.h" //これを最初にincludeすること

#include "..\_NFLib\NF_AE.h"
#include "..\_NFLib\NF_ParamsSetup.h"

#include "..\_NFLib\fx\NF_Mult.h"
#include "..\_NFLib\fx\NF_Minmax.h"
#include "..\_NFLib\fx\NF_ChannelMinmax.h"
#include "..\_NFLib\fx\NF_Blur.h"
#include "..\_NFLib\fx\NF_ChannelBlur.h"
#include "..\_NFLib\fx\debug_font.h"
#include "..\_NFLib\fx\NF_Paint.h"
#include "..\_NFLib\fx\NF_Noise.h"
#include "..\_NFLib\fx\NF_Draw.h"
#include "..\_NFLib\fx\NF_DrawAALine.h"

#include "CameraShakeFilter.h"
#include <string>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer


	ID_SHAKE_VALUE,
	ID_CAMERA_SHAKE_TOPIC,
	ID_SHAKE_VALUE_RAND,
	ID_SUB_SHAKE,
	ID_ANGLE,
	ID_ANGLE_RAND,
	ID_CAMERA_SHAKE_TOPIC_END,

	ID_HAND_SHAKE_VALUE,
	ID_HAND_SHAKE_SPEED,
	ID_HAND_SHAKE_SEED,
	ID_HAND_SHAKE_TOPIC,
	ID_AMPLITUDE,
	ID_X_VALUE,
	ID_X_SPEED1,
	ID_X_SPEED2,
	ID_X_SEED,
	ID_Y_VALUE,
	ID_Y_SPEED1,
	ID_Y_SPEED2,
	ID_Y_SEED,
	ID_HANDSHAKE_TOPIC_END,

	ID_EDGE_MODE,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_SHAKE_VALUE		"camera_value"
#define	STE_CAMERA_SHAKE_TOPIC		"cameraShake"
#define	STR_SHAKE_VALUE_RAND	"valueRand"
#define	STR_SUB_SHAKE		"subShake"
#define	STR_ANGLE			"angle"
#define	STR_ANGLE_RAND		"angleRand"
#define	STR_REP_MODE		"edgeMode"

#define	STR_HAND_SHAKE_VALUE		"hand_value"
#define	STR_HAND_SHAKE_SPEED		"hand_speed"
#define	STR_HAND_SHAKE_SEED			"hand_seed"
#define	STR_HAND_SHAKE_TOPIC		"handShake"
#define	STR_AMPLITUDE		"amplitude"
#define	STR_X_VALUE		"x_value"
#define	STR_X_SPEED1	"x_speed1"
#define	STR_X_SPEED2	"x_speed2"
#define	STR_X_SEED		"x_seed"
#define	STR_Y_VALUE		"y_value"
#define	STR_Y_SPEED1	"y_speed1"
#define	STR_Y_SPEED2	"y_speed2"
#define	STR_Y_SEED		"y_seed"

//#define	STR_HANDSHAKE_VALUE		"handShakeValue"

#define	STR_REP_MODE_ITEMS	"tiling|stretch|none"

//UIのパラメータ


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
#endif // CameraShake_H

