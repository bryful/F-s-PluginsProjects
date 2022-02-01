#pragma once
#ifndef FsG_H
#define FsG_H


#include "FsAEHeader.h"
#include "FsUtils.h"

//アルファーをすべてONにする
PF_Err AlphaON08(PF_EffectWorldPtr world);
PF_Err AlphaON16(PF_EffectWorldPtr world);
PF_Err AlphaON32(PF_EffectWorldPtr world);
PF_Err AlphaON(PF_EffectWorldPtr world, PF_PixelFormat format);

//Matをストレートに直す
PF_Err FromPremultiply08(PF_EffectWorldPtr world);
PF_Err FromPremultiply16(PF_EffectWorldPtr world);
PF_Err FromPremultiply32(PF_EffectWorldPtr world);
PF_Err FromPremultiply(PF_EffectWorldPtr world, PF_PixelFormat format);

PF_Err ToPremultiply08(PF_EffectWorldPtr world);
PF_Err ToPremultiply16(PF_EffectWorldPtr world);
PF_Err ToPremultiply32(PF_EffectWorldPtr world);
PF_Err ToPremultiply(PF_EffectWorldPtr world, PF_PixelFormat format);

PF_Err iterateWorld8(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel* px));
PF_Err iterateWorld16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* px));
PF_Err iterateWorld32(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat* px));

//16Bit from 8Bit
PF_Err iterateWorld08to16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst16,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel* inP, PF_Pixel16* outP));

PF_Err iterateWorld16to16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst16,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel16* outP));

PF_Err iterateWorld32to16(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst16,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel32* inP, PF_Pixel16* outP));

PF_Err iterateWorld16to08(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst16,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel* outP));

PF_Err iterateWorld16to32(
	refconType refcon,
	PF_EffectWorldPtr src,
	PF_EffectWorldPtr dst16,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel32* outP));

PF_Err World08To16(PF_EffectWorld *src, PF_EffectWorld *dst);
PF_Err World16To16(PF_EffectWorld *src, PF_EffectWorld *dst);
PF_Err World32To16(PF_EffectWorld *src, PF_EffectWorld *dst);

#define FSRAND_MAX	0x7FFF
void FsSrand(A_u_long s);
A_u_long FsRand();
double FsRandDouble();

PF_Err Noise08(PF_EffectWorldPtr world, PF_FpLong noise);
PF_Err Noise16(PF_EffectWorldPtr world, PF_FpLong noise);
PF_Err Noise32(PF_EffectWorldPtr world, PF_FpLong noise);


#endif