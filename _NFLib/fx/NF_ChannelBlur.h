#pragma once
#pragma once
#ifndef NF_CBUR_H
#define NF_CBUR_H


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
#include <cstdio>  // sprintf_s用に追加
#include <windows.h>  // OutputDebugStringA用




PF_Err ChannelBlur(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    A_long value,
    A_long channel  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
);
#endif // NF_CBUR_H