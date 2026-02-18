#pragma once
#ifndef ALPHACOPY_R_H
#define ALPHACOPY_R_H


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

#if defined(PF_AE100_PLUG_IN_VERSION)
#include "AEFX_SuiteHelper.h"
#define refconType void*
#else
#include "PF_Suite_Helper.h"
#define refconType A_long
#endif

PF_Err AlphaCopyRM(
    PF_InData* in_dataP,
    PF_EffectWorld* inP,
    PF_EffectWorld* outP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_Pixel color,
    PF_Boolean reverse,
    PF_Boolean isWhite
);
#endif // ALPHACOPY_H