#pragma once
#ifndef _ColorBlockFilter_H   // 正しいガード名に変更
#define _ColorBlockFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"




// *******************************************************************************



PF_Err ColorBlockMain(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long count,
	A_long width,
	A_long height,
	float value,
	PF_Boolean alpha,
	A_long seed

);

#endif