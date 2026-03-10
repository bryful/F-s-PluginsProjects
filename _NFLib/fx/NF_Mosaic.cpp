#include "NF_Mosaic.h"

template <typename T>
PF_Err MosaicFlatT(
    PF_EffectWorld* worldP,
    A_long xL,
    A_long yL,
    A_long mc_width,
    A_long mc_height,
    A_long widthTrue,
    PF_FpLong ar,
    PF_FpLong ag,
    PF_FpLong ab,
    PF_FpLong Max_Chan
)
{
    PF_Err err = PF_Err_NONE;
    T* dataP = reinterpret_cast<T*>(worldP->data);

    A_long x0 = AE_CLAMP(xL, 0, worldP->width - 1);
    A_long x1 = AE_CLAMP(xL + mc_width, 0, worldP->width - 1);
    A_long y0 = AE_CLAMP(yL, 0, worldP->height - 1);
    A_long y1 = AE_CLAMP(yL + mc_height, 0, worldP->height - 1);
    A_long dx = x1 - x0;
    A_long dy = y1 - y0;
    if (dx <= 0 || dy <= 0) return PF_Err_NONE;

    PF_FpLong sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    A_long count = 0;
    for (A_long y = 0; y < dy; y++) {
        for (A_long x = 0; x < dx; x++) {
            T* p = &dataP[(y0 + y) * widthTrue + (x0 + x)];
            sumR += (PF_FpLong)p->red;
            sumG += (PF_FpLong)p->green;
            sumB += (PF_FpLong)p->blue;
            sumA += (PF_FpLong)p->alpha;
            count++;
        }
    }
    sumR /= count;
    sumG /= count;
    sumB /= count;
    sumA /= count;
    sumR = AE_CLAMP(sumR+ar,0, Max_Chan);
    sumG = AE_CLAMP(sumG+ag, 0, Max_Chan);
    sumB = AE_CLAMP(sumB+ab, 0, Max_Chan);
    sumA = AE_CLAMP(sumA, 0, Max_Chan);
    for (A_long y = 0; y < dy; y++) {
        for (A_long x = 0; x < dx; x++) {
            T* p = &dataP[(y0 + y) * widthTrue + (x0 + x)];
            p->red = static_cast<decltype(T::red)>(sumR);
            p->green = static_cast<decltype(T::green)>(sumG);
            p->blue = static_cast<decltype(T::blue)>(sumB);
            p->alpha = static_cast<decltype(T::alpha)>(sumA);
        }
    }

	return err;
}

PF_Err MosaicFlat(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long widthTrue,
    A_long xL,
    A_long yL,
    A_long mc_width,
    A_long mc_height,
	PF_FpLong ar,
	PF_FpLong ag,
	PF_FpLong ab
)
{
	PF_Err err = PF_Err_NONE;
    switch (pixelFormat)
    {
        case PF_PixelFormat_ARGB32:
            ERR(MosaicFlatT<PF_Pixel8>( worldP, xL, yL, mc_width, mc_height, widthTrue,ar,ag,ab, PF_MAX_CHAN8));
			break;
        case PF_PixelFormat_ARGB64:
            ERR(MosaicFlatT<PF_Pixel16>(worldP, xL, yL, mc_width, mc_height, widthTrue, ar, ag, ab, PF_MAX_CHAN16));
            break;
        case PF_PixelFormat_ARGB128:
            ERR(MosaicFlatT<PF_PixelFloat>(worldP, xL, yL, mc_width, mc_height,  widthTrue, ar, ag, ab, 1.0f));
            break;
    }
	return err;
}
/// ---------------------------------------------------------------------------------
// ******************************************************************************************************
// ******************************************************************************************************
template <typename T, typename CT, CT max_chan>
PF_Err BlockMosaicT(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long widthTrue,
	A_long count,
	A_long mc_width,
	A_long mc_height,
	float noise,
	PF_Boolean is_color,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;

	for (int cnt = 0; cnt < count; cnt++) {
		A_long rw = AE_CLAMP((A_long)((float)mc_width / 2 + hash_float(cnt * 10, 10, seed) * (float)mc_width / 2), 1, mc_width);
		A_long rh = AE_CLAMP((A_long)((float)mc_height / 2 + hash_float(cnt * 10, 50, seed) * (float)mc_height / 2), 1, mc_height);
		A_long tx = (A_long)((float)(worldP->width - 1) * hash_float(cnt * 10, 100, seed));
		A_long ty = (A_long)((float)(worldP->height - 1) * hash_float(cnt * 10, 200, seed));
		float ar = 0, ag = 0, ab = 0;
		ar = ag = ab = (1.0f - 2.0f * hash_float(cnt * 10, 300, seed)) * noise * max_chan;
		if (is_color) {
			ag = (1.0f - 2.0f * hash_float(cnt * 10, 400, seed)) * noise * max_chan;
			ab = (1.0f - 2.0f * hash_float(cnt * 10, 500, seed)) * noise * max_chan;
		}
		err = MosaicFlat(
			worldP,
			pixelFormat,
			widthTrue,
			tx,
			ty,
			rw,
			rh,
			ar,
			ag,
			ab
		);
	}

	return err;
}
// ------------------------------------------------------------------------------------
PF_Err BlockMosaicMain(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long count,
	A_long mc_width,
	A_long mc_height,
	float noise,
	PF_Boolean is_color,
	A_long seed

)
{
	PF_Err err = PF_Err_NONE;
	if (count <= 0 || mc_width <= 0 || mc_height <= 0) return PF_Err_NONE;

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB32:
		err = BlockMosaicT<PF_Pixel, A_u_char, PF_MAX_CHAN8>(
			worldP,
			pixelFormat,
			worldP->rowbytes / sizeof(PF_Pixel),
			count,
			mc_width,
			mc_height,
			noise,
			is_color,
			seed);
		break;
	case PF_PixelFormat_ARGB64:
		err = BlockMosaicT<PF_Pixel16, A_u_short, PF_MAX_CHAN16>(
			worldP,
			pixelFormat,
			worldP->rowbytes / sizeof(PF_Pixel16),
			count,
			mc_width,
			mc_height,
			noise,
			is_color,
			seed);
		break;
	case PF_PixelFormat_ARGB128:
		err = BlockMosaicT<PF_PixelFloat, PF_FpShort, 1.0f>(
			worldP,
			pixelFormat,
			worldP->rowbytes / sizeof(PF_PixelFloat),
			count,
			mc_width,
			mc_height,
			noise,
			is_color,
			seed);
		break;
	default:
		err = PF_Err_INVALID_INDEX;
	}


	return err;
}
// ******************************************************************************************************
// ******************************************************************************************************
template <typename T, typename CT, CT max_chan>
PF_Err MosaicPatT(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long widthTrue,
	float amount, 
	A_long mc_width,
	A_long mc_height,
	float noise,
	PF_Boolean is_color,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;

	A_long xc = worldP->width / mc_width;
	if (worldP->width % mc_width != 0) xc++;
	A_long yc = worldP->height / mc_height;
	if (worldP->height % mc_height != 0) yc++;

	for(int yy = 0; yy < yc; yy++) {
		A_long yL = yy * mc_height;
		for (int xx = 0; xx < xc; xx++) {
			if (hash_float(xx * 10, yy * 10, seed) > amount) continue;
			A_long xL = xx * mc_width;
			float ar = 0, ag = 0, ab = 0;
			ar = ag = ab = (1.0f - 2.0f * hash_float(xx * 10, yy * 10 + 5, seed)) * noise * max_chan;
			if (is_color) {
				ag = (1.0f - 2.0f * hash_float(xx * 10, yy * 20+10, seed)) * noise * max_chan;
				ab = (1.0f - 2.0f * hash_float(xx * 10, yy * 30+15, seed)) * noise * max_chan;
			}
			err = MosaicFlat(
				worldP,
				pixelFormat,
				widthTrue,
				xL,
				yL,
				mc_width,
				mc_height,
				ar,
				ag,
				ab
			);
		}
	}
	return err;
}
PF_Err MosaicPatMain(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	float amount,
	A_long mc_width,
	A_long mc_height,
	float noise,
	PF_Boolean is_color,
	A_long seed

)
{
	PF_Err err = PF_Err_NONE;
	if (amount <= 0 || mc_width <= 0 || mc_height <= 0) return PF_Err_NONE;

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB32:
		err = MosaicPatT<PF_Pixel, A_u_char, PF_MAX_CHAN8>(
			worldP,
			pixelFormat,
			worldP->rowbytes / sizeof(PF_Pixel),
			amount,
			mc_width,
			mc_height,
			noise,
			is_color,
			seed);
		break;
	case PF_PixelFormat_ARGB64:
		err = MosaicPatT<PF_Pixel16, A_u_short, PF_MAX_CHAN16>(
			worldP,
			pixelFormat,
			worldP->rowbytes / sizeof(PF_Pixel16),
			amount,
			mc_width,
			mc_height,
			noise,
			is_color,
			seed);
		break;
	case PF_PixelFormat_ARGB128:
		err = MosaicPatT<PF_PixelFloat, PF_FpShort, 1.0f>(
			worldP,
			pixelFormat,
			worldP->rowbytes / sizeof(PF_PixelFloat),
			amount,
			mc_width,
			mc_height,
			noise,
			is_color,
			seed);
		break;
	default:
		err = PF_Err_INVALID_INDEX;
	}


	return err;
}