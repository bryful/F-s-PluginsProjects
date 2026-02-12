#pragma once
#pragma once
#ifndef NF_TARGET_PIXELS_H
#define NF_TARGET_PIXELS_H

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

#include "..\_NFLib\NF_Utils.h"

#ifndef AE_CLAMP
#define AE_CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (MIN) : ((VAL) > (MAX) ? (MAX) : (VAL)))
#endif
#define SP_COLOR_TABLE_MAX 16



PF_Err TargetPixelsExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long count,
	PF_Pixel* colors,
	A_long none_count,
	PF_Pixel* none_colors,
	PF_Boolean isALL
);






#endif // NF_SELECT_PIXELS_H
