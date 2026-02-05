#include "FsG.h"


//-------------------------------------------------------------------
//=========================================================================================
typedef struct NoiseInfo
{
	A_long	noise;
	PF_FpLong	noiseD;

}NoiseInfo, *NoiseInfoP, **NoiseInfoH;
//=========================================================================================
PF_Err
NoiseSub32(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*px)
{
	PF_Err			err = PF_Err_NONE;
	NoiseInfo *	infoP = reinterpret_cast<NoiseInfo*>(refcon);

	PF_FpLong n = infoP->noiseD;
	PF_FpLong n1 = n - 2 * n * (PF_FpLong)FsRand() / FSRAND_MAX;
	PF_FpLong n2 = n - 2 * n * (PF_FpLong)FsRand() / FSRAND_MAX;
	PF_FpLong n3 = n - 2 * n * (PF_FpLong)FsRand() / FSRAND_MAX;

	px->red = RoundFpShortDouble((PF_FpLong)px->red + n1);
	px->green = RoundFpShortDouble((PF_FpLong)px->green + n2);
	px->blue = RoundFpShortDouble((PF_FpLong)px->blue + n3);


	return err;
}//=========================================================================================
PF_Err
NoiseSub16(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*px)
{
	PF_Err			err = PF_Err_NONE;
	NoiseInfo *	infoP = reinterpret_cast<NoiseInfo*>(refcon);

	A_long n = infoP->noise;
	A_long n1 = n - 2 * n * FsRand() / FSRAND_MAX;
	A_long n2 = n - 2 * n * FsRand() / FSRAND_MAX;
	A_long n3 = n - 2 * n * FsRand() / FSRAND_MAX;

	px->red = RoundShort((A_long)px->red + n1);
	px->green = RoundShort((A_long)px->green + n2);
	px->blue = RoundShort((A_long)px->blue + n3);


	return err;
}
//=========================================================================================
PF_Err
NoiseSub08(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel	*px)
{
	PF_Err			err = PF_Err_NONE;
	NoiseInfo *	infoP = reinterpret_cast<NoiseInfo*>(refcon);

	A_long n = infoP->noise;
	A_long n1 = n - 2 * n * FsRand() / FSRAND_MAX;
	A_long n2 = n - 2 * n * FsRand() / FSRAND_MAX;
	A_long n3 = n - 2 * n * FsRand() / FSRAND_MAX;

	px->red = RoundByteLong((A_long)px->red + n1);
	px->green = RoundByteLong((A_long)px->green + n2);
	px->blue = RoundByteLong((A_long)px->blue + n3);


	return err;
}
//=========================================================================================
PF_Err Noise32(PF_EffectWorldPtr world, PF_FpLong noise)
{
	PF_Err			err = PF_Err_NONE;
	NoiseInfo ni;

	ni.noiseD = noise;
	ERR(iterateWorld32(&ni, world, NoiseSub32));
	return err;
}
//=========================================================================================
PF_Err Noise16(PF_EffectWorldPtr world, PF_FpLong noise)
{
	PF_Err			err = PF_Err_NONE;
	NoiseInfo ni;

	ni.noise = (A_long)(PF_MAX_CHAN16 * noise);
	ERR(iterateWorld16(&ni, world, NoiseSub16));
	return err;
}
//=========================================================================================
PF_Err Noise08(PF_EffectWorldPtr world, PF_FpLong noise)
{
	PF_Err			err = PF_Err_NONE;
	NoiseInfo ni;
	ni.noise = (A_long)(PF_MAX_CHAN8 * noise);
	ERR(iterateWorld8(&ni, world, NoiseSub08));
	return err;
}