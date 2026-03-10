
#pragma once
#ifndef NF_RANDOM_MISAIC_H
#define NF_RANDOM_MISAIC_H


#include "AEConfig.h" 
#include "entry.h"

#ifdef AE_OS_WIN
#include "string.h"
#endif
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"
#include "AE_GeneralPlug.h"

#include "AEFX_SuiteHelper.h"
#define refconType void*

#include "..\NF_Utils.h"
#include <vector>

template <typename T>
struct GData {
    std::vector< std::vector<T>>;
    A_long width;
    A_long height;
};
struct NF_Rect {
	A_long x;
	A_long y;
    A_long width;
    A_long height;
};
PF_Err Bilateral
(
    PF_InData* in_dataP,
    PF_EffectWorld* input,
    PF_EffectWorld* output,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    int radius,
    float sigma_s,
    float sigma_r
);


#endif // NF_DRAW_H
