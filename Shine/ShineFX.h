#pragma once
#ifndef ShineFX_H
#define ShineFX_H

#include "../NFsLibrary/AE_SDK.h"
#include "../NFsLibrary/AEInfo.h"
#include "../NFsLibrary/NFsWorld.h"

#include "Shine.h"


/*
// **************************************************************
static PF_Err Noise8(void* refcon, A_long x, A_long y, PF_Pixel* p)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = PF_MAX_CHAN8 * infoP->value;
	v = v - 2 * v * xorShiftDouble();

	p->red = RoundByteFpLong(p->red + v);
	p->green = RoundByteFpLong(p->green + v);
	p->blue = RoundByteFpLong(p->blue + v);

	return err;
}
// **************************************************************
static PF_Err Noise16(void* refcon, A_long x, A_long y, PF_Pixel16* p)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = PF_MAX_CHAN16 * infoP->value;
	v = v - 2 * v * xorShiftDouble();

	p->red = RoundShortFpLong(p->red + v);
	p->green = RoundShortFpLong(p->green + v);
	p->blue = RoundShortFpLong(p->blue + v);

	return err;
}
// **************************************************************
static PF_Err Noise32(void* refcon, A_long x, A_long y, PF_PixelFloat* p)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = infoP->value;
	v = v - 2 * v * xorShiftDouble();

	p->red = RoundFpShortDouble(p->red + v);
	p->green = RoundFpShortDouble(p->green + v);
	p->blue = RoundFpShortDouble(p->blue + v);

	return err;
}
*/
class ShineFX :public NFsWorld
{
private:

public:
	ShineFX(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	) : NFsWorld(wld, ind, fmt)
	{

	}
#pragma region Noise

	// **************************************************************
	/*
	PF_Err Noise(ParamInfo* infoP)
	{
		PF_Err			err = PF_Err_NONE;
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			ERR(Iterate32((void*)infoP, Noise32));
			break;
		case PF_PixelFormat_ARGB64:
			ERR(Iterate16((void*)infoP, Noise16));
			break;
		case PF_PixelFormat_ARGB32:
			ERR(Iterate8((void*)infoP, Noise8));
			break;
		}
		return err;
	}
	*/
#pragma endregion
};

#endif