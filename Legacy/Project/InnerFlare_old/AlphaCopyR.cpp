#include "AlphaCopyR.h"

typedef struct {
    PF_InData* in_data;
	PF_Boolean reverse;
	PF_Pixel color;
	PF_Pixel16 color16;
    PF_PixelFloat color32;
} AlpheCopyInfo;

#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define _CONVERT8TO16(A)		( (((A_long)(A) * PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8 )
#define _CONVERT8TO32(A)      ((double)(long)((double)A*10000.0/(double)PF_MAX_CHAN8 + 0.5)/10000.0)

//-------------------------------------------------------------------------------------------------
// 8ビット → 16ビット変換
inline void ConvertPixel8To16(const PF_Pixel* in8, PF_Pixel16* out16)
{
    // 0-255 → 0-32768 に変換
    out16->alpha = _CONVERT8TO16(in8->alpha);
    out16->red = _CONVERT8TO16(in8->red);
    out16->green = _CONVERT8TO16(in8->green);
    out16->blue = _CONVERT8TO16(in8->blue);
}

//-------------------------------------------------------------------------------------------------
// 8ビット → 32ビット変換
inline void ConvertPixel8To32(const PF_Pixel* in8, PF_PixelFloat* out32)
{
    // 0-255 → 0.0-1.0 に変換
    out32->alpha = (PF_FpShort)_CONVERT8TO32(in8->alpha);
    out32->red = (PF_FpShort)_CONVERT8TO32(in8->red);
    out32->green = (PF_FpShort)_CONVERT8TO32(in8->green);
    out32->blue = (PF_FpShort)_CONVERT8TO32(in8->blue);
}
//-------------------------------------------------------------------------------------------------
// テンプレート版 - 8/16/32ビット共通処理
template<typename PixelType, typename AlphaType>
static PF_Err
AlphaCopyRT(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PixelType* inP,
    PixelType* outP,
    AlphaType maxValue)
{
    PF_Err			err = PF_Err_NONE;
    AlpheCopyInfo* infoP = reinterpret_cast<AlpheCopyInfo*>(refcon);

    PixelType col;
    AlphaType a = inP->alpha;
    a = infoP->reverse ? (maxValue - a) : a;
    if constexpr (std::is_same_v<PixelType, PF_Pixel>) {
        col = infoP->color;
    }
    else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        col = infoP->color16;
    }
    else {
        col = infoP->color32;
    }
    outP->red = col.red;
    outP->green = col.green;
    outP->blue = col.blue;
    outP->alpha = (AlphaType)((double)outP->alpha * (double)a / maxValue);  // PixelType → AlphaType に修正

    return err;
}

//-------------------------------------------------------------------------------------------------
// 8ビット用ラッパー
static PF_Err
AlphaCopyR8(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PF_Pixel* inP,
    PF_Pixel* outP)
{
    return AlphaCopyRT<PF_Pixel, A_u_char>(refcon, xL, yL, inP, outP, PF_MAX_CHAN8);
}

//-------------------------------------------------------------------------------------------------
// 16ビット用ラッパー
static PF_Err
AlphaCopyR16(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PF_Pixel16* inP,
    PF_Pixel16* outP)
{
    return AlphaCopyRT<PF_Pixel16, A_u_short>(refcon, xL, yL, inP, outP, PF_MAX_CHAN16);
}

//-------------------------------------------------------------------------------------------------
// 32ビット用ラッパー
static PF_Err
AlphaCopyR32(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PF_PixelFloat* inP,
    PF_PixelFloat* outP)
{
    return AlphaCopyRT<PF_PixelFloat, PF_FpShort>(refcon, xL, yL, inP, outP, 1.0f);
}

static PF_Err AlphaCopyRMImpl(
    PF_InData* in_dataP,
    PF_EffectWorld* inP,
    PF_EffectWorld* outP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_Pixel color,
	PF_Boolean reverse
)
{
    PF_Err err = PF_Err_NONE;
    AlpheCopyInfo info;
    info.in_data = in_dataP;
    info.color = color;
	info.reverse = reverse;
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        ERR(suitesP->Iterate8Suite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopyR8, outP));
        break;

    case PF_PixelFormat_ARGB64:
         ConvertPixel8To16(&color, &info.color16);
        ERR(suitesP->Iterate16Suite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopyR16, outP));
        break;

    case PF_PixelFormat_ARGB128:
        ConvertPixel8To32(&color, &info.color32);
        ERR(suitesP->IterateFloatSuite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopyR32, outP));
        break;
    }
    return err;
}


PF_Err AlphaCopyRM(
    PF_InData* in_dataP,
    PF_EffectWorld* inP,
    PF_EffectWorld* outP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
	PF_Pixel color,
    PF_Boolean reverse
)
{
    return AlphaCopyRMImpl(in_dataP, inP, outP, pixelFormat, suitesP, color, reverse);
}