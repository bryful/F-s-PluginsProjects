#include "Mult.h"

// inputを調べてターゲットならば白、違うのなら透明をoutputに書き込む
static PF_Err
TargetPixel8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel w = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
	PF_Pixel b = { 0,0, 0, 0 };

	switch (infoP->targetMode)
	{
	case 3:
		if (compPix8(*inP, infoP->targetColor))
		{
			*outP = w;
		}else {
			*outP = b;
		}
		break;
	case 2:
		if (inP->alpha > 0)
		{
			*outP = w;
		}else {
			*outP = b;
		}
		break;
	case 1:
	default:
		*outP = w;
		break;
	}

	return err;
}

static PF_Err
TargetPixel16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 w = { PF_MAX_CHAN16,PF_MAX_CHAN16, PF_MAX_CHAN16, PF_MAX_CHAN16 };
	PF_Pixel16 b = { 0,0, 0, 0 };

	switch (infoP->targetMode)
	{
	case 3:
		if (compPix16_8(*inP, infoP->targetColor))
		{
			*outP = w;
		}
		else {
			*outP = b;
		}
		break;
	case 2:
		if (inP->alpha > 0)
		{
			*outP = w;
		}
		else {
			*outP = b;
		}
		break;
	case 1:
	default:
		*outP = w;
		break;
	}

	return err;
}
static PF_Err
TargetPixel32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel32 w = { 1.0, 1.0, 1.0, 1.0 };
	PF_Pixel32 b = { 0,0, 0, 0 };

	switch (infoP->targetMode)
	{
	case 3:
		if (compPix32_8(*inP, infoP->targetColor))
		{
			*outP = w;
		}
		else {
			*outP = b;
		}
		break;
	case 2:
		if (inP->alpha > 0)
		{
			*outP = w;
		}
		else {
			*outP = b;
		}
		break;
	case 1:
	default:
		*outP = w;
		break;
	}

	return err;
}
PF_Err Mult::TargetExec(ParamInfo* infoP,NFWorld *src,NFWorld *dst)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)infoP, TargetPixel32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)infoP, TargetPixel16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, TargetPixel8, dst->world);
		break;
	default:
		break;
	}
	return err;
}