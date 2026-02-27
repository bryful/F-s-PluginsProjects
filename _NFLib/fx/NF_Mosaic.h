#pragma once
#pragma once
#ifndef NF_MISAIC_H
#define NF_MISAIC_H


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


PF_Err MosaicFlat(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long widthTrue,
    A_long xL,
    A_long yL,
    A_long mc_width,
    A_long mc_height
);
#endif // NF_BLUR_H