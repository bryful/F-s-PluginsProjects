#pragma once
#ifndef MuzzleFlashMain_H
#define MuzzleFlashMain_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"

#include "..\_NFLib\vector\NF_VectorLine.h"
#include "..\_NFLib\vector\NF_VectorMask.h"



// *******************************************************************************



PF_Err MuzzleFlashExec(
	PF_InData* in_data,
	AEGP_SuiteHandler* suites,
	PF_PixelFormat pixelFormat,
	PF_EffectWorld* output,
	float		pointX,
	float		pointY,
	A_long		count,
	float		rootSize,
	float		length,
	A_long		branch,
	float		direction,
	float		angle,
	A_long		sparkCount,
	A_long		sparkSubCount,
	float		sparkScale,
	PF_Pixel	color,
	float		scale,
	A_long seed

);
#endif