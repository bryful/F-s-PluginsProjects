#pragma once
#pragma once
#ifndef NF_DRAW_AA_LINE_H
#define NF_DRAW_AA_LINE_H


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
#include <cmath>
#include <algorithm>



// シングルスレッド版 (DrawAALineT は内部実装用)
void DrawAALine8(PF_EffectWorld* output, float x0, float y0, float x1, float y1, PF_Pixel col, float startAlpha, float endAlpha, float startWeight, float endWeight);
void DrawAALine16(PF_EffectWorld* output, float x0, float y0, float x1, float y1, PF_Pixel16 col, float startAlpha, float endAlpha, float startWeight, float endWeight);
void DrawAALine32(PF_EffectWorld* output, float x0, float y0, float x1, float y1, PF_PixelFloat col, float startAlpha, float endAlpha, float startWeight, float endWeight);

// マルチスレッド版 (Iterate Suite)
PF_Err DrawAA_Line8(
	PF_InData* in_data, 
	PF_EffectWorld* out, 
	AEGP_SuiteHandler* suites, 
	float x0, float y0, float x1, float y1, 
	PF_Pixel col, 
	float startAlpha, float endAlpha,
	float startWeight, float endWeight);
PF_Err DrawAA_Line16(PF_InData* in_data, PF_EffectWorld* out, AEGP_SuiteHandler* suites, float x0, float y0, float x1, float y1, PF_Pixel16 col, float startAlpha, float endAlpha, float startWeight, float endWeight);
PF_Err DrawAA_Line32(PF_InData* in_data, PF_EffectWorld* out, AEGP_SuiteHandler* suites, float x0, float y0, float x1, float y1, PF_PixelFloat col, float startAlpha, float endAlpha, float startWeight, float endWeight);

// 汎用フォーマット判別版
PF_Err DrawAA_Line(
	PF_InData* in_data, 
	PF_EffectWorld* out, 
	PF_PixelFormat format, 
	AEGP_SuiteHandler* suites, 
	float x0, float y0, float x1, float y1, 
	PF_Pixel col, 
	float startAlpha, float endAlpha, 
	float startWeight, float endWeight);


#endif // NF_DRAWWORLD_H