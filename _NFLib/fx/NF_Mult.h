#pragma once
#pragma once
#ifndef NF_MULT_H
#define NF_MULT_H


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

PF_Err UnMult8(void* refcon, A_long thread_idx, A_long y, A_long itrt);
PF_Err UnMult16(void* refcon, A_long thread_idx, A_long y, A_long itrt);
PF_Err UnMult32(void* refcon, A_long thread_idx, A_long y, A_long itrt);


PF_Err Mult(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    PF_Boolean isUnMult
);
PF_Err Mult(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_Boolean isUnMult
);
#endif // TINY_BLUR_H