#pragma once
#ifndef _SkeletonFilter_H
#define _SkeletonFilter_H

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
	PF_FpLong	r;
	PF_FpLong	g;
	PF_FpLong	b;

	PF_FpLong	noise;
	//PF_Boolean	noise_frame;
	A_long		noise_offset;

} FilterInfo;

PF_Err FilterImage(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	FilterInfo* infoP
);

#endif