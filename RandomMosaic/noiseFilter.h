#pragma once
#ifndef _NoiseFilter_H
#define _NoiseFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"




// *******************************************************************************



PF_Err MazzleFlashExec(
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	float		pointX,
	float		pointY,
	A_long		count,
	float		rootSize,
	float		length,
	A_long		branch,
	float		direction,
	float		angle,
	PF_Pixel	color,
);

#endif