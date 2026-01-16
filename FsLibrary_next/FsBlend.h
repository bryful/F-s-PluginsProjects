#pragma once
#ifndef FsBLEND_H
#define FsBLEND_H


#include "FsAEHeader.h"
#include "FsUtils.h"
#include "FsG.h"

enum BLEND_MODE
{
	Normal = 1,
	Add,
	Screen,
	Lighten,
	LighterColor,
	Multiply,
	Darken,
	DarkerColor,
	Overlay,
};

PF_Err Blend16To08(PF_EffectWorldPtr src, PF_EffectWorldPtr dst, A_long blendMode, PF_FpLong par);
PF_Err Blend16To16(PF_EffectWorldPtr src, PF_EffectWorldPtr dst, A_long blendMode, PF_FpLong par);
PF_Err Blend16To16(PF_EffectWorldPtr src, PF_EffectWorldPtr dst, A_long blendMode, PF_FpLong par);

#endif
