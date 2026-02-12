#pragma once
#ifndef InkingLineBlurFilter_H
#define InkingLineBlurFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"




// *******************************************************************************


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