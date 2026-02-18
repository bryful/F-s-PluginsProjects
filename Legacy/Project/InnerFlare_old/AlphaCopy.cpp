#include "AlphaCopy.h"

typedef struct {
    PF_InData* in_data;
	PF_Boolean reverse;
    PF_Boolean isWhite;

} AlpheCopyInfo;

#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))


//-------------------------------------------------------------------------------------------------
// テンプレート版 - 8/16/32ビット共通処理
template<typename PixelType, typename AlphaType>
static PF_Err
AlphaCopyT(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PixelType* inP,
    PixelType* outP,
    AlphaType maxValue)
{
    PF_Err			err = PF_Err_NONE;
    AlpheCopyInfo* infoP = reinterpret_cast<AlpheCopyInfo*>(refcon);

    if (infoP->isWhite == TRUE) {
        if (inP->red == maxValue && inP->green == maxValue && inP->blue == maxValue) {
            outP->alpha = 0;
            outP->red = outP->green = outP->blue = 0;
            return err;
        }
    }
    AlphaType a = inP->alpha;

    if (infoP->reverse == FALSE) {
        a = maxValue - a;
    }
    outP->alpha = a;
    outP->red = a;
    outP->green = a;
    outP->blue = a;

    return err;
}

//-------------------------------------------------------------------------------------------------
// 8ビット用ラッパー
static PF_Err
AlphaCopy8(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PF_Pixel* inP,
    PF_Pixel* outP)
{
    return AlphaCopyT<PF_Pixel, A_u_char>(refcon, xL, yL, inP, outP, PF_MAX_CHAN8);
}

//-------------------------------------------------------------------------------------------------
// 16ビット用ラッパー
static PF_Err
AlphaCopy16(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PF_Pixel16* inP,
    PF_Pixel16* outP)
{
    return AlphaCopyT<PF_Pixel16, A_u_short>(refcon, xL, yL, inP, outP, PF_MAX_CHAN16);
}

//-------------------------------------------------------------------------------------------------
// 32ビット用ラッパー
static PF_Err
AlphaCopy32(
    refconType	refcon,
    A_long		xL,
    A_long		yL,
    PF_PixelFloat* inP,
    PF_PixelFloat* outP)
{
    return AlphaCopyT<PF_PixelFloat, PF_FpShort>(refcon, xL, yL, inP, outP, 1.0f);
}

PF_Err AlphaCopyM(
    PF_InData* in_dataP,
    PF_EffectWorld* inP,
    PF_EffectWorld* outP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_Boolean reverse,
	PF_Boolean isWhite
)
{
    PF_Err err = PF_Err_NONE;
    AlpheCopyInfo info;
    info.in_data = in_dataP;
    info.reverse = reverse;
	info.isWhite = isWhite;
    
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        ERR(suitesP->Iterate8Suite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopy8, outP));
        break;

    case PF_PixelFormat_ARGB64:
        ERR(suitesP->Iterate16Suite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopy16, outP));
        break;

    case PF_PixelFormat_ARGB128:
        ERR(suitesP->IterateFloatSuite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopy32, outP));
        break;
    }
    return err;
}


