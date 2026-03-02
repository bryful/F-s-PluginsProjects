#pragma once
#ifndef _NoiseFilter_H
#define _NoiseFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"




// *******************************************************************************



PF_Err NoiseExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_FpLong noise,
	PF_Boolean isColor,
	A_long seed
);

#endif