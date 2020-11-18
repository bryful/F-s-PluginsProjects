#pragma once
#ifndef NFsSkeltonFX_H
#define NFsSkeltonFX_H

#include "../NFsLibrary/AE_SDK.h"
#include "../NFsLibrary/AEInfo.h"
#include "../NFsLibrary/NFsWorld.h"

#include "NFsSkelton.h"

class NFsSkeltonFX :public NFsWorld
{
public:
	NFsSkeltonFX(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	) : NFsWorld(wld, ind, fmt)
	{

	}
private:
	/*
	PF_Err Noise8(void* refcon, A_long x, A_long y, PF_Pixel* p)
	{
		PF_Err			err = PF_Err_NONE;
		ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

		PF_FpLong v = PF_MAX_CHAN8 * infoP->value;
		v = -v + 2 * v * xorShiftDouble();

		p->red = RoundByteFpLong((PF_FpLong)p->red + v);
		p->green = RoundByteFpLong((PF_FpLong)p->green + v);
		p->blue = RoundByteFpLong((PF_FpLong)p->blue + v);
		return err;
	}
	*/
	
};

#endif