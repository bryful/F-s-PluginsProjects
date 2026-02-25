#pragma once
#ifndef PosterizationFilter_H
#define PosterizationFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"




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


PF_Err PosterizationExec(
	PF_InData* in_dataP,
	//PF_EffectWorld* inputP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long level
);
PF_Err PostExec(
	PF_InData* in_dataP,
	//PF_EffectWorld* inputP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long level, 
	A_long color_count,
	PF_Pixel* colors,
	A_long mode,
	PF_Pixel bcolors

);
#endif