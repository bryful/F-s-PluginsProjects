
#include "FsG.h"

// ****************************************************************************************
PF_Err FromPremultiplySub08(refconType refcon, A_long x, A_long y, PF_Pixel* px)
{
	PF_Err		err = PF_Err_NONE;
	PF_Pixel bl = { 0,0,0,0 };
	double a = (double)px->alpha;
	if (a == 0)
	{
		*px = bl;
	}
	else {
		px->red = RoundByteDouble((double)px->red *PF_MAX_CHAN8 / a);
		px->green = RoundByteDouble((double)px->green *PF_MAX_CHAN8 / a);
		px->blue = RoundByteDouble((double)px->blue *PF_MAX_CHAN8 / a);

	}
	return err;

}
// ****************************************************************************************
PF_Err FromPremultiply08(PF_EffectWorldPtr world)
{
	return iterateWorld8(NULL, world, FromPremultiplySub08);
}
// ****************************************************************************************
PF_Err FromPremultiplySub16(refconType refcon, A_long x, A_long y, PF_Pixel16* px)
{
	PF_Err		err = PF_Err_NONE;
	PF_Pixel16 bl = { 0,0,0,0 };
	double a = (double)px->alpha;
	if (a == 0)
	{
		*px = bl;
	}
	else {
		px->red = RoundShortFpLong((double)px->red *PF_MAX_CHAN16 / a);
		px->green = RoundShortFpLong((double)px->green *PF_MAX_CHAN16 / a);
		px->blue = RoundShortFpLong((double)px->blue *PF_MAX_CHAN16 / a);

	}
	return err;

}
// ****************************************************************************************
PF_Err FromPremultiply16(PF_EffectWorldPtr world)
{
	return iterateWorld16(NULL, world, FromPremultiplySub16);
}
// ****************************************************************************************
PF_Err FromPremultiplySub32(refconType refcon, A_long x, A_long y, PF_PixelFloat* px)
{
	PF_Err		err = PF_Err_NONE;
	PF_PixelFloat bl = { 0,0,0,0 };
	double a = (double)px->alpha;
	if (a == 0)
	{
		*px = bl;
	}
	else {
		px->red = RoundFpShortDouble((double)px->red  / a);
		px->green = RoundFpShortDouble((double)px->green  / a);
		px->blue = RoundFpShortDouble((double)px->blue / a);

	}
	return err;

}
// ****************************************************************************************
PF_Err FromPremultiply32(PF_EffectWorldPtr world)
{
	return iterateWorld32(NULL, world, FromPremultiplySub32);
}
// ****************************************************************************************
PF_Err FromPremultiply(PF_EffectWorldPtr world, PF_PixelFormat format)
{
	PF_Err		err = PF_Err_NONE;
	switch (format)
	{
	case PF_PixelFormat_ARGB32:
		ERR(FromPremultiply08(world));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(FromPremultiply16(world));
		break;
	case PF_PixelFormat_ARGB128:
		ERR(FromPremultiply32(world));
		break;
	default:
		break;
	}
	return err;
}
// ****************************************************************************************
// ****************************************************************************************

// ****************************************************************************************
// ****************************************************************************************
PF_Err ToPremultiplySub08(refconType refcon, A_long x, A_long y, PF_Pixel* px)
{
	PF_Err		err = PF_Err_NONE;
	PF_Pixel bl = { 0,0,0,0 };
	double a = (double)px->alpha /PF_MAX_CHAN8;
	if (a == 0)
	{
		*px = bl;
	}
	else {
		px->red = RoundByteDouble((double)px->red *a );
		px->green = RoundByteDouble((double)px->green *a );
		px->blue = RoundByteDouble((double)px->blue * a);

	}
	return err;

}
// ****************************************************************************************
PF_Err ToPremultiply08(PF_EffectWorldPtr world)
{
	return iterateWorld8(NULL, world, ToPremultiplySub08);
}
// ****************************************************************************************
PF_Err ToPremultiplySub16(refconType refcon, A_long x, A_long y, PF_Pixel16* px)
{
	PF_Err		err = PF_Err_NONE;
	PF_Pixel16 bl = { 0,0,0,0 };
	double a = (double)px->alpha / PF_MAX_CHAN16;
	if (a == 0)
	{
		*px = bl;
	}
	else {
		px->red = RoundShortFpLong((double)px->red * a);
		px->green = RoundShortFpLong((double)px->green * a);
		px->blue = RoundShortFpLong((double)px->blue * a);

	}
	return err;

}
// ****************************************************************************************
PF_Err ToPremultiply16(PF_EffectWorldPtr world)
{
	return iterateWorld16(NULL, world, ToPremultiplySub16);
}
// ****************************************************************************************
PF_Err ToPremultiplySub32(refconType refcon, A_long x, A_long y, PF_PixelFloat* px)
{
	PF_Err		err = PF_Err_NONE;
	PF_PixelFloat bl = { 0,0,0,0 };
	double a = (double)px->alpha;
	if (a > 1) a = 1;
	if (a == 0)
	{
		*px = bl;
	}
	else {
		px->red = RoundFpShortDouble((double)px->red * a);
		px->green = RoundFpShortDouble((double)px->green * a);
		px->blue = RoundFpShortDouble((double)px->blue * a);
		px->alpha = (PF_FpShort)a;

	}
	return err;

}
// ****************************************************************************************
PF_Err ToPremultiply32(PF_EffectWorldPtr world)
{
	return iterateWorld32(NULL, world, ToPremultiplySub32);
}
// ****************************************************************************************
PF_Err ToPremultiply(PF_EffectWorldPtr world, PF_PixelFormat format)
{
	PF_Err		err = PF_Err_NONE;
	switch (format)
	{
	case PF_PixelFormat_ARGB32:
		ERR(ToPremultiply08(world));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ToPremultiply16(world));
		break;
	case PF_PixelFormat_ARGB128:
		ERR(ToPremultiply32(world));
		break;
	default:
		break;
	}
	return err;
}
// ****************************************************************************************
