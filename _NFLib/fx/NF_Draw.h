
#pragma once
#ifndef NF_LINE_H
#define NF_LINE_H


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

PF_Err DrawLine(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel fill_color
);
PF_Err DrawLine(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel fill_color
);
PF_Err DrawBox(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel color
);
PF_Err DrawBoxFill(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel color
);
PF_Err DrawCircle(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    PF_Point pos,
    A_long radius,
    PF_Pixel color
);
PF_Err DrawCircleFill(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    PF_Point pos,
    A_long radius,
    PF_Pixel color
);
#endif // NF_DRAW_H