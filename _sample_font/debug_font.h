#pragma once
#pragma once
#ifndef DEBUG_FONT_H
#define DEBUG_FONT_H


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

void DrawDebugString(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    int x,
    int y,
    const char* str,
    PF_Pixel color
);
#endif // DEBUG_FONT_H
