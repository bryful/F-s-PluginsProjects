#include "Mult.h"

static PF_Err
ToStraight8(
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
		if (inP->alpha == PF_MAX_CHAN8)
		{
			*outP = *inP;
		}
		else {
			PF_FpLong a = PF_MAX_CHAN8/(PF_FpLong)inP->alpha;
			outP->red = RoundByteFpLong((PF_FpLong)inP->red * a);
			outP->green = RoundByteFpLong((PF_FpLong)inP->green * a);
			outP->blue = RoundByteFpLong((PF_FpLong)inP->blue * a);
			outP->alpha = inP->alpha;
		}
	}

	return err;
}
static PF_Err
ToStraight16(
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
		if (inP->alpha == PF_MAX_CHAN16)
		{
			*outP = *inP;
		}
		else {
			PF_FpLong a = PF_MAX_CHAN16/(PF_FpLong)inP->alpha;
			outP->red = RoundShortFpLong((PF_FpLong)inP->red * a);
			outP->green = RoundShortFpLong((PF_FpLong)inP->green * a);
			outP->blue = RoundShortFpLong((PF_FpLong)inP->blue * a);
			outP->alpha = inP->alpha;
		}
	}

	return err;
}
static PF_Err
ToStraight32(
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
		if (inP->alpha >= 1)
		{
			*outP = *inP;
		}
		else {
			PF_FpLong a = 1/(PF_FpLong)inP->alpha;
			outP->red = RoundFpShortDouble((PF_FpLong)inP->red * a);
			outP->green = RoundFpShortDouble((PF_FpLong)inP->green * a);
			outP->blue = RoundFpShortDouble((PF_FpLong)inP->blue * a);
			outP->alpha = inP->alpha;
		}
	}
	return err;
}
PF_Err Mult::ToStraightExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)infoP, ToStraight32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)infoP, ToStraight16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, ToStraight8, dst->world);
		break;
	default:
		break;
	}
	return err;
}