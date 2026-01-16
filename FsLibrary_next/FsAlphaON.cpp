
#include "FsG.h"

PF_Err AlphaONSub08(refconType refcon, A_long x, A_long y, PF_Pixel* px)
{
	PF_Err		err = PF_Err_NONE;
	px->alpha = PF_MAX_CHAN8;
	return err;

}
PF_Err AlphaON08(PF_EffectWorldPtr world)
{
	return iterateWorld8(NULL, world, AlphaONSub08);
}
PF_Err AlphaONSub16(refconType refcon, A_long x, A_long y, PF_Pixel16* px)
{
	PF_Err		err = PF_Err_NONE;
	px->alpha = PF_MAX_CHAN16;
	return err;

}
PF_Err AlphaON16(PF_EffectWorldPtr world)
{
	return iterateWorld16(NULL, world, AlphaONSub16);
}
PF_Err AlphaONSub32(refconType refcon, A_long x, A_long y, PF_PixelFloat* px)
{
	PF_Err		err = PF_Err_NONE;
	px->alpha = 1;
	return err;

}
PF_Err AlphaON32(PF_EffectWorldPtr world)
{
	return iterateWorld32(NULL, world, AlphaONSub32);
}
inline PF_Err AlphaON(PF_EffectWorldPtr world, PF_PixelFormat format)
{
	switch (format)
	{
	case PF_PixelFormat_ARGB32:
		AlphaON08(world);
		break;
	case PF_PixelFormat_ARGB64:
		AlphaON16(world);
		break;
	case PF_PixelFormat_ARGB128:
		AlphaON32(world);
		break;
	default:
		break;
	}
}

