#pragma once
#pragma once
#ifndef NF_NOISE_H
#define NF_NOISE_H


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

PF_Err NoiseAndBlock(
    PF_InData* in_data,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpShort noise_size,
    PF_FpShort amount,
    PF_FpShort accent_amount, // 追加
    A_long seed,
    A_Boolean is_color
);

PF_Err Noise(
    PF_InData* in_data,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpShort noise_size,
    PF_FpShort amount,
    A_long seed,
    A_Boolean is_color
);

#endif // NF_NOISE_H