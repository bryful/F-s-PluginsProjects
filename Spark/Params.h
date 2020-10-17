#pragma once
#ifndef Params_H
#define Params_H

#include "Fs_Target.h"

#include "AEConfig.h"
#include "entry.h"

//#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"

#if defined(PF_AE100_PLUG_IN_VERSION)
#include "AEFX_SuiteHelper.h"
#define refconType void*
#else
#include "PF_Suite_Helper.h"
#define refconType A_long
#endif


#ifdef AE_OS_WIN
#include <Windows.h>
#endif


#include "../FsLibrary/FsAE.h"


#define POINT_TABLE_SIZE	1024

//UIÇÃÉpÉâÉÅÅ[É^
typedef struct PointInfo {
	A_LPoint	p;
	PF_FpLong	s;

} PointInfo, * PointInfoP, ** PointInfoH;

typedef struct ParamInfo {
	PointInfo	start;
	A_long		startRandX;
	A_long		startRandY;
	
	PointInfo	last;
	A_long		lastRandX;
	A_long		lastRandY;

	PF_FpLong	rotRandom;

	PF_FpLong	lineSize;
	A_long		lineMove;
	A_long		foldCount;
	A_long		drawCount;
	A_long		subCount;

	PF_FpLong	offset;
	A_long		seed;
	A_long		frame;

	PF_Pixel	color;
	PF_Boolean	blend;

} ParamInfo, * ParamInfoP, ** ParamInfoH;

// **************************************************************
inline A_LPoint PosFromRot(A_long len, PF_FpLong rot,PF_InData *in_data)
{
	A_LPoint ret;
	ret.x = 0;
	ret.y = 0;
	if (len == 0) return ret;
	rot = PF_FMOD(rot, 360);
	if (rot < 0) rot += 360;

	PF_FpLong r = rot * PF_PI / 180;
	ret.x = (A_long)((PF_FpShort)len * PF_COS(r) + 0.5);
	ret.y = (A_long)((PF_FpShort)len * PF_SIN(r) + 0.5);


	return ret;

}
// **************************************************************
inline PointInfo RandomPoint(PointInfo p, A_long x, A_long y)
{
	PointInfo ret;
	ret.p.x = p.p.x;
	ret.p.y = p.p.y;
	ret.s = p.s;

	if (x > 0)
	{
		ret.p.x += (A_long)(-(double)x + (double)x * 2 * xorShiftDouble());
	}
	if (y > 0)
	{
		ret.p.y += (A_long)(-(double)y + (double)y * 2 * xorShiftDouble());
	}
	return ret;

}
//-------------------------------------------------------


#endif