#pragma once
#ifndef CameraShakeFilter_H
#define CameraShakeFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"

typedef struct ParamInfo {
	PF_FpLong	camera_value;
	PF_FpLong	camera_valueRand;
	PF_FpLong	camera_subShake;
	PF_FpLong	camera_angle;
	PF_FpLong	camera_angleRand;

	PF_FpLong	hand_Value;
	PF_FpLong	hand_speed;
	PF_FpLong	hand_seed;
	PF_FpLong	hand_amplitude;

	PF_FpLong	hand_X_value;
	PF_FpLong	hand_X_speed1;
	PF_FpLong	hand_X_speed2;
	PF_FpLong	hand_X_seed;
	PF_FpLong	hand_Y_value;
	PF_FpLong	hand_Y_speed1;
	PF_FpLong	hand_Y_speed2;
	PF_FpLong	hand_Y_seed;

	A_long		frame;
	PF_FpLong	fps;
	PF_FpLong	sec;
	A_long		edgeMode;

} ParamInfo, * ParamInfoP, ** ParamInfoH;


// *******************************************************************************
// -- - 1. 型解決用のトレイト(PixelTraits) -- -
template <typename T> struct PixelTraits;

template <> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char channel_type;
};

template <> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short channel_type;
};

template <> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort channel_type;
};

typedef struct CameraShakeInfo {
	PF_PixelPtr	data;
	A_long		widthTrue;
	A_long		width;
	A_long		height;
	float		offsetX;
	float		offsetY;
	PF_Boolean	isInt;
	A_long		frame;
	PF_FpLong	fps;
	ParamInfo	*pi;
}CameraShakeInfo;

PF_Err CameraShakeExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	ParamInfo *pi
);

#endif