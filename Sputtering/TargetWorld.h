#pragma once
#ifndef TARGETWORLD_H
#define TARGETWORLD_H

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

#include <string>
#include <vector>

enum TARGET_MODE
{
	TARGET_NONE = 0,
	TARGET_BORDER = 128,
	TARGET_AREA = 255
};
#define TARGET_COLOR_MAX 8

PF_Err TargetWorldExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long count,
	PF_Pixel* colors,
	std::vector<A_u_char>* target_worlds
);
PF_Err FromTargetWorldExec(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<A_u_char>* target_worlds
);
PF_Err BorderExec(
	PF_InData* in_dataP,
	AEGP_SuiteHandler* suitesP,
	std::vector<A_u_char>* src,
	std::vector<A_u_char>* dst,
	A_long		width,
	A_long		height,
	PF_Boolean is_border_direction,
	float		direction_angle
);
#endif