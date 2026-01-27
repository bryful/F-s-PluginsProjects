#include "AlphaCopyDD.h"
#include <cmath>

typedef struct {
    PF_InData* in_data;
    PF_FpLong rot;
    PF_FpLong length;
    PF_FpLong offset;
    PF_FpLong cosV;
    PF_FpLong sinV;
    A_long  width;
    A_long  height;
    A_long rowbytes;
    PF_PixelPtr data;
} AlpheCopyInfo;

#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

//-------------------------------------------------------------------------------------------------
// テンプレート版 - 8/16/32ビット共通処理
template<typename PixelType, typename AlphaType>
static PF_Err
AlphaCopyDDT(
    refconType refcon,
    A_long xL,
    A_long yL,
    PixelType* inP,
    PixelType* outP,
    AlphaType maxValue)
{
    PF_Err err = PF_Err_NONE;
    AlpheCopyInfo* infoP = reinterpret_cast<AlpheCopyInfo*>(refcon);

    // 3点のサンプリング座標を計算
    A_long pX = AE_CLAMP((A_long)(xL + infoP->cosV + 0.5), 0, infoP->width - 1);
    A_long pY = AE_CLAMP((A_long)(yL + infoP->sinV + 0.5), 0, infoP->height - 1);
    
    A_long pX2 = AE_CLAMP((A_long)(xL + infoP->cosV + infoP->cosV * infoP->offset + 0.5), 0, infoP->width - 1);
    A_long pY2 = AE_CLAMP((A_long)(yL + infoP->sinV + infoP->sinV * infoP->offset + 0.5), 0, infoP->height - 1);

    A_long pX3 = AE_CLAMP((A_long)(xL + infoP->cosV + infoP->cosV * infoP->offset * 2 + 0.5), 0, infoP->width - 1);
    A_long pY3 = AE_CLAMP((A_long)(yL + infoP->sinV + infoP->sinV * infoP->offset * 2 + 0.5), 0, infoP->height - 1);

    // 入力ピクセルのアルファ値を正規化（0.0-1.0)
    //PF_FpLong av = (PF_FpLong)inP->alpha / (PF_FpLong)maxValue;
    //PF_FpLong avr = 1.0 - av;

    // 各サンプリング位置のピクセルを取得
    PixelType* pP = (PixelType*)((A_u_char*)infoP->data + infoP->rowbytes * pY + sizeof(PixelType) * pX);
    PixelType* pP2 = (PixelType*)((A_u_char*)infoP->data + infoP->rowbytes * pY2 + sizeof(PixelType) * pX2);
    PixelType* pP3 = (PixelType*)((A_u_char*)infoP->data + infoP->rowbytes * pY3 + sizeof(PixelType) * pX3);

    // 各サンプリング位置のアルファを反転して正規化
    PF_FpLong apP = 1.0 - (PF_FpLong)pP->alpha / (PF_FpLong)maxValue;
    PF_FpLong apP2 = 1.0 - (PF_FpLong)pP2->alpha / (PF_FpLong)maxValue;
    PF_FpLong apP3 = 1.0 - (PF_FpLong)pP3->alpha / (PF_FpLong)maxValue;

    // 3点のアルファを乗算して戻す
    PF_FpLong resultAlpha = apP * apP2 * apP3;
    
    // 型に応じて変換
    AlphaType alpha;
    if (std::is_same<AlphaType, PF_FpShort>::value) {
        // 32ビット（float）の場合
        alpha = (AlphaType)AE_CLAMP(resultAlpha, 0.0, 1.0);
    } else {
        // 8/16ビット（整数）の場合
        alpha = (AlphaType)AE_CLAMP(resultAlpha * maxValue + 0.5, 0.0, (PF_FpLong)maxValue);
    }

    outP->alpha = outP->red = outP->green = outP->blue = alpha;
    
    return err;
}

//-------------------------------------------------------------------------------------------------
// 8ビット用ラッパー
static PF_Err
AlphaCopyDD8(
    refconType refcon,
    A_long xL,
    A_long yL,
    PF_Pixel* inP,
    PF_Pixel* outP)
{
    return AlphaCopyDDT<PF_Pixel, A_u_char>(refcon, xL, yL, inP, outP, PF_MAX_CHAN8);
}

//-------------------------------------------------------------------------------------------------
// 16ビット用ラッパー
static PF_Err
AlphaCopyDD16(
    refconType refcon,
    A_long xL,
    A_long yL,
    PF_Pixel16* inP,
    PF_Pixel16* outP)
{
    return AlphaCopyDDT<PF_Pixel16, A_u_short>(refcon, xL, yL, inP, outP, PF_MAX_CHAN16);
}

//-------------------------------------------------------------------------------------------------
// 32ビット用ラッパー
static PF_Err
AlphaCopyDD32(
    refconType refcon,
    A_long xL,
    A_long yL,
    PF_PixelFloat* inP,
    PF_PixelFloat* outP)
{
    return AlphaCopyDDT<PF_PixelFloat, PF_FpShort>(refcon, xL, yL, inP, outP, 1.0f);
}

//-------------------------------------------------------------------------------------------------
static PF_Err AlphaCopyDDImpl(
    PF_InData* in_dataP,
    PF_EffectWorld* inP,
    PF_EffectWorld* outP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpLong rot,
    PF_FpLong length,
    PF_FpLong offset
)
{
    PF_Err err = PF_Err_NONE;
    AlpheCopyInfo info;
    info.in_data = in_dataP;
    info.rot = rot;
    info.length = length;
	info.offset = offset;
    info.width = inP->width;
    info.height = inP->height;
    info.rowbytes = inP->rowbytes;
    info.data = inP->data;
    
    const double PI = acos(-1.0);
    double rad = (rot - 90.0) * PI / 180.0;
    info.sinV = sin(rad) * length;
    info.cosV = cos(rad) * length;
    
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        ERR(suitesP->Iterate8Suite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopyDD8, outP));
        break;

    case PF_PixelFormat_ARGB64:
        ERR(suitesP->Iterate16Suite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopyDD16, outP));
        break;

    case PF_PixelFormat_ARGB128:
        ERR(suitesP->IterateFloatSuite1()->iterate(
            in_dataP, 0, outP->height, inP, NULL, &info, AlphaCopyDD32, outP));
        break;
    }
    return err;
}

//-------------------------------------------------------------------------------------------------
PF_Err AlphaCopyDDM(
    PF_InData* in_dataP,
    PF_EffectWorld* inP,
    PF_EffectWorld* outP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpLong rot,
    PF_FpLong length,
	PF_FpLong offset
)
{
    return AlphaCopyDDImpl(in_dataP, inP, outP, pixelFormat, suitesP, rot, length, offset);
}