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