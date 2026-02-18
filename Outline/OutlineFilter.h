#pragma once
#ifndef OutlineFilter_H
#define OutlineFilter_H

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

typedef struct FilterInfo {
	PF_Pixel	color;
	PF_Pixel16	color16;
	PF_PixelFloat	color32;
} FilterInfo;

PF_Err FilterImage(
	PF_InData* in_dataP,
	PF_EffectWorld* inputP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_Boolean isWhite
);
PF_Err FilterLastImage(
	PF_InData* in_dataP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_Pixel color
);
#endif