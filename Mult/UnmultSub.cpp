#include "Mult.h"

static PF_Err
Unmult8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	if (inP->alpha == 0) {
		*outP = { 0,0,0,0 };
	}
	else {
		PF_FpLong r = 0;
		PF_FpLong g = 0;
		PF_FpLong b = 0;
		PF_FpLong a = PF_MAX_CHAN8;
		if (inP->alpha == PF_MAX_CHAN8)
		{
			r = (PF_FpLong)inP->red;
			g = (PF_FpLong)inP->green;
			b = (PF_FpLong)inP->blue;
		}
		else {
			PF_FpLong a = (PF_FpLong)inP->alpha / PF_MAX_CHAN8;
			r = ((PF_FpLong)inP->red * a);
			g = ((PF_FpLong)inP->green * a);
			b = ((PF_FpLong)inP->blue * a);
		}
		a = MAX(r, MAX(g, b));
		outP->alpha = RoundByteFpLong(a);
		a = PF_MAX_CHAN8 / a;
		outP->red = RoundByteFpLong(r * a);
		outP->green = RoundByteFpLong(g * a);
		outP->blue = RoundByteFpLong(b * a);
	}

	return err;
}
static PF_Err
Unmult16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	if (inP->alpha == 0) {
		*outP = { 0,0,0,0 };
	}
	else {
		PF_FpLong r = 0;
		PF_FpLong g = 0;
		PF_FpLong b = 0;
		PF_FpLong a = PF_MAX_CHAN16;
		if (inP->alpha == PF_MAX_CHAN16)
		{
			r = (PF_FpLong)inP->red;
			g = (PF_FpLong)inP->green;
			b = (PF_FpLong)inP->blue;
		}
		else {
			PF_FpLong a = (PF_FpLong)inP->alpha / PF_MAX_CHAN16;
			r = ((PF_FpLong)inP->red * a);
			g = ((PF_FpLong)inP->green * a);
			b = ((PF_FpLong)inP->blue * a);
		}
		a = MAX(r, MAX(g, b));
		outP->alpha = RoundShortFpLong(a);
		a = PF_MAX_CHAN16 / a;
		outP->red = RoundShortFpLong(r * a);
		outP->green = RoundShortFpLong(g * a);
		outP->blue = RoundShortFpLong(b * a);
	}
	return err;
}
static PF_Err
Unmult32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP)
{
	PF_Err			err = PF_Err_NONE;
	if (inP->alpha == 0) {
		*outP = { 0,0,0,0 };
	}
	else {
		PF_FpLong r = 0;
		PF_FpLong g = 0;
		PF_FpLong b = 0;
		PF_FpLong a = 1;
		if (inP->alpha >= 1)
		{
			r = (PF_FpLong)inP->red;
			g = (PF_FpLong)inP->green;
			b = (PF_FpLong)inP->blue;
		}
		else {
			PF_FpLong a = (PF_FpLong)inP->alpha;
			r = ((PF_FpLong)inP->red * a);
			g = ((PF_FpLong)inP->green * a);
			b = ((PF_FpLong)inP->blue * a);
		}
		a = MAX(r, MAX(g, b));
		if (a > 1) a = 1;
		outP->alpha = RoundFpShortDouble(a);
		a = 1 / a;
		outP->red = RoundFpShortDouble(r * a);
		outP->green = RoundFpShortDouble(g * a);
		outP->blue = RoundFpShortDouble(b * a);
	}
	return err;
}
PF_Err Mult::UnmultExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)infoP, Unmult32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)infoP, Unmult16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, Unmult8, dst->world);
		break;
	default:
		break;
	}
	return err;
}