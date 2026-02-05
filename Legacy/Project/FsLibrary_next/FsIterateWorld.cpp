#include "FsG.h"

//*********************************************************************************
PF_Err iterateWorld8(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel* px)
)
{
	PF_Err err = PF_Err_NONE;
	A_long w = src->width;
	A_long offset = src->rowbytes / sizeof(PF_Pixel) - w;
	A_long h = src->height;


	PF_Pixel* px = (PF_Pixel*)src->data;
	for (A_long y = 0; y < h; y++) {
		for (A_long x = 0; x < w; x++) {
			err = pix_fn((refconType)refcon, x, y, px);
			if (err != PF_Err_NONE) return err;
			px++;
		}
		px += offset;
	}
	return err;
}
//*********************************************************************************
PF_Err iterateWorld16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* px)
)
{
	PF_Err err = PF_Err_NONE;
	A_long w = src->width;
	A_long offset = src->rowbytes / sizeof(PF_Pixel16) - w;
	A_long h = src->height;


	PF_Pixel16* px = (PF_Pixel16*)src->data;
	for (A_long y = 0; y < h; y++) {
		for (A_long x = 0; x < w; x++) {
			err = pix_fn((refconType)refcon, x, y, px);
			if (err != PF_Err_NONE) return err;
			px++;
		}
		px += offset;
	}
	return err;
}
//*********************************************************************************
PF_Err iterateWorld32(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat* px)
)
{
	PF_Err err = PF_Err_NONE;
	A_long w = src->width;
	A_long offset = src->rowbytes / sizeof(PF_PixelFloat) - w;
	A_long h = src->height;


	PF_PixelFloat* px = (PF_PixelFloat *)src->data;
	for (A_long y = 0; y < h; y++) {
		for (A_long x = 0; x < w; x++) {
			err = pix_fn((refconType)refcon, x, y, px);
			if (err != PF_Err_NONE) return err;
			px++;
		}
		px += offset;
	}
	return err;
}
// ****************************************************************************************
PF_Err iterateWorld16to08(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel* outP))
{
	PF_Err err = PF_Err_NONE;

	A_long srcW = src->width;
	A_long srcH = src->height;
	A_long srcWt = src->rowbytes / sizeof(PF_Pixel16);
	A_long srcOff = srcWt - srcW;

	A_long dstW = dst->width;
	A_long dstH = dst->height;
	A_long dstWt = dst->rowbytes / sizeof(PF_Pixel);
	A_long dstOff = dstWt - dstW;


	PF_Pixel16* srcD = (PF_Pixel16*)src->data;
	PF_Pixel* dstD = (PF_Pixel*)dst->data;

	for (A_long y = 0; y < dstH; y++)
	{
		for (A_long x = 0; x < dstW; x++)
		{
			err = pix_fn((refconType)refcon, x, y, srcD, dstD);
			if (err != PF_Err_NONE) return err;
			srcD++;
			dstD++;
		}
		srcD += srcOff;
		dstD += dstOff;
	}
	return err;
}

// ****************************************************************************************
PF_Err iterateWorld16to16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel16* outP))
{
	PF_Err err = PF_Err_NONE;

	A_long dstW = dst->width;
	A_long dstH = dst->height;
	A_long dstWt = dst->rowbytes / sizeof(PF_Pixel16);
	A_long dstOff = dstWt - dstW;

	A_long srcW = src->width;
	A_long srcH = src->height;
	A_long srcWt = src->rowbytes / sizeof(PF_Pixel16);
	A_long srcOff = srcWt - srcW;

	PF_Pixel16* srcD = (PF_Pixel16*)src->data;
	PF_Pixel16* dstD = (PF_Pixel16*)dst->data;

	for (A_long y = 0; y < dstH; y++)
	{
		for (A_long x = 0; x < dstW; x++)
		{
			err = pix_fn((refconType)refcon, x, y, srcD, dstD);
			if (err != PF_Err_NONE) return err;
			srcD++;
			dstD++;
		}
		srcD += srcOff;
		dstD += dstOff;
	}
	return err;
}
// ****************************************************************************************
PF_Err iterateWorld16to32(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel32* outP))
{
	PF_Err err = PF_Err_NONE;

	A_long srcW = src->width;
	A_long srcH = src->height;
	A_long srcWt = src->rowbytes / sizeof(PF_Pixel16);
	A_long srcOff = srcWt - srcW;

	A_long dstW = dst->width;
	A_long dstH = dst->height;
	A_long dstWt = dst->rowbytes / sizeof(PF_Pixel32);
	A_long dstOff = dstWt - dstW;


	PF_Pixel16* srcD = (PF_Pixel16*)src->data;
	PF_Pixel32* dstD = (PF_Pixel32*)dst->data;

	for (A_long y = 0; y < dstH; y++)
	{
		for (A_long x = 0; x < dstW; x++)
		{
			err = pix_fn((refconType)refcon, x, y, srcD, dstD);
			if (err != PF_Err_NONE) return err;
			srcD++;
			dstD++;
		}
		srcD += srcOff;
		dstD += dstOff;
	}
	return err;
}
// ****************************************************************************************
// ****************************************************************************************
PF_Err iterateWorld08to16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel* inP, PF_Pixel16* outP))
{
	PF_Err err = PF_Err_NONE;

	A_long srcW = src->width;
	A_long srcH = src->height;
	A_long srcWt = src->rowbytes / sizeof(PF_Pixel8);
	A_long srcOff = srcWt - srcW;

	A_long dstW = dst->width;
	A_long dstH = dst->height;
	A_long dstWt = dst->rowbytes / sizeof(PF_Pixel16);
	A_long dstOff = dstWt - dstW;


	PF_Pixel* srcD = (PF_Pixel*)src->data;
	PF_Pixel16* dstD = (PF_Pixel16*)dst->data;

	for (A_long y = 0; y < dstH; y++)
	{
		for (A_long x = 0; x < dstW; x++)
		{
			err = pix_fn((refconType)refcon, x, y, srcD, dstD);
			if (err != PF_Err_NONE) return err;
			srcD++;
			dstD++;
		}
		srcD += srcOff;
		dstD += dstOff;
	}
	return err;
}
// ****************************************************************************************
PF_Err iterateWorld32to16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel32* inP, PF_Pixel16* outP))
{
	PF_Err err = PF_Err_NONE;

	A_long dstW = dst->width;
	A_long dstH = dst->height;
	A_long dstWt = dst->rowbytes / sizeof(PF_Pixel16);
	A_long dstOff = dstWt - dstW;

	A_long srcW = src->width;
	A_long srcH = src->height;
	A_long srcWt = src->rowbytes / sizeof(PF_Pixel32);
	A_long srcOff = srcWt - srcW;

	PF_Pixel32* srcD = (PF_Pixel32*)src->data;
	PF_Pixel16* dstD = (PF_Pixel16*)dst->data;

	for (A_long y = 0; y < dstH; y++)
	{
		for (A_long x = 0; x < dstW; x++)
		{
			err = pix_fn((refconType)refcon, x, y, srcD, dstD);
			if (err != PF_Err_NONE) return err;
			srcD++;
			dstD++;
		}
		srcD += srcOff;
		dstD += dstOff;
	}
	return err;
}
// ****************************************************************************************




