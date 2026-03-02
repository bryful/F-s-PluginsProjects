#pragma once
#ifndef NF_VECTOR_RECT_H
#define NF_VECTOR_RECT_H


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
#include <algorithm>
#include <cmath>

struct a_linePrm {
    float		x;
	float		y;
	float		b;
};

void draw_a_rect(std::vector<std::vector<float>>& data,
    float x0, float y0, float x1, float y1,
    float x2, float y2, float x3, float y3,
    float value);
void draw_a_rect(std::vector<std::vector<float>>& data,
    PF_Point p0, PF_Point p1, PF_Point p2, PF_Point p3,
    float value);
void draw_a_line(
    std::vector<std::vector<float>>& data,
    float x0, float y0, float b0,
    float x1, float y1, float b1,
    float value);

void draw_a_line(std::vector<std::vector<float>>& data,
    PF_Point p0, float b0, PF_Point p1, float b1,
    float value);
void draw_a_line(std::vector<std::vector<float>>& data,
	a_linePrm p0, a_linePrm p1,
    float value);
void draw_a_line(std::vector<std::vector<float>>& data,
    std::vector<a_linePrm> prms,
    float value);
void draw_polyline(
    std::vector<std::vector<float>>& data,
    const std::vector<a_linePrm>& points, float v
);
std::vector<a_linePrm> offsetLinePrm(const std::vector<a_linePrm>& points, A_long ox, A_long oy);
std::vector<a_linePrm> offsetLinePrm(A_long size, a_linePrm* points, A_long ox, A_long oy);

#endif // DEBUG_FONT_H
