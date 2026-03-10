
#pragma once
#ifndef NF_RANDOM_SHIFT_H
#define NF_RANDOM_SHIFT_H


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
#include "NF_Mosaic.h"

#include <vector>

PF_Err RandomShiftMain(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long count,
	A_long rWidth,
	A_long rHeight,
	A_long offsetX,
	A_long offsetY,
	A_long seed

);

#endif // NF_DRAW_H
