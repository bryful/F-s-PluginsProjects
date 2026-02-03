#pragma once
#pragma once
#ifndef NF_PAINT_H
#define NF_PAINT_H


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

PF_Err Paint(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long xL,
    A_long yL,
    PF_Pixel fill_color
);

#endif // NF_PAINT_H