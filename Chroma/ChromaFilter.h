#pragma once
#ifndef ChromaFilter_H
#define ChromaFilter_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"





PF_Err Chromatic(
    PF_InData* in_dataP,
    PF_EffectWorld* input,
    PF_EffectWorld* output,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpLong scale_r,
    PF_FpLong scale_g,
    PF_FpLong scale_b,
    PF_Point center,
    A_long mode
);

#endif