#pragma once
#pragma once
#ifndef NF_VECTORMASK_H
#define NF_VECTORMASK_H


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
#include <windows.h>
#include <string>

PF_Err DrawMask(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask
);
PF_Err DrawColorMask(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask,
	PF_Pixel target_color
);

#endif // DEBUG_FONT_H
