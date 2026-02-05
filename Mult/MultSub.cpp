#include "Mult.h"

static PF_Err
Mult8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	if (inP->alpha == PF_MAX_CHAN8)
	{
		*outP = *inP;
	}
	else if (inP->alpha==0) {
		outP->red = 0;
		outP->green = 0;
		outP->blue = 0;
		outP->alpha = PF_MAX_CHAN8;
	}
	else {
		PF_FpLong a = (PF_FpLong)inP->alpha / PF_MAX_CHAN8;
		outP->red = RoundByteFpLong((PF_FpLong)inP->red * a);
		outP->green = RoundByteFpLong((PF_FpLong)inP->green * a);
		outP->blue = RoundByteFpLong((PF_FpLong)inP->blue * a);
		outP->alpha = PF_MAX_CHAN8;
	}
	return err;
}
static PF_Err
Mult16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	if (inP->alpha == PF_MAX_CHAN16)
	{
		*outP = *inP;
	}
	else if (inP->alpha == 0)
	{
		outP->red = 0;
		outP->green = 0;
		outP->blue = 0;
		outP->alpha = PF_MAX_CHAN16;
	}
	else {
		PF_FpLong a = (PF_FpLong)inP->alpha / PF_MAX_CHAN16;
		outP->red = RoundShortFpLong(inP->red * a);
		outP->green = RoundShortFpLong(inP->green * a);
		outP->blue = RoundShortFpLong(inP->blue * a);
		outP->alpha = PF_MAX_CHAN16;
	}
	return err;
}
static PF_Err
Mult32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP)
{
	PF_Err			err = PF_Err_NONE;
	if (inP->alpha >= 1.0)
	{
		*outP = *inP;
	}
	else if (inP->alpha == 0)
	{
		outP->red = 0;
		outP->green = 0;
		outP->blue = 0;
		outP->alpha = 1.0;
	}
	else {
		outP->red = RoundFpShortDouble(inP->red * inP->alpha);
		outP->green = RoundFpShortDouble(inP->green * inP->alpha);
		outP->blue = RoundFpShortDouble(inP->blue * inP->alpha);
		outP->alpha = 1.0;
	}
	return err;
}
PF_Err Mult::MultExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)infoP, Mult32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)infoP, Mult16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, Mult8, dst->world);
		break;
	default:
		break;
	}
	return err;
}