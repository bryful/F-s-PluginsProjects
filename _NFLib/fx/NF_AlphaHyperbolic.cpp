#include "NF_AlphaHyperbolic.h"

typedef struct AHInfo {
    PF_FpLong		hyperbolic;
    PF_Boolean		invert;
} AHInfo, * AHInfoP, ** AHInfoH;

//-----------------------------------------------------------------------------------
// テンプレート版: 8bit/16bit/32bit共通処理
//-----------------------------------------------------------------------------------
template <typename PixelType, typename ComponentType>
static PF_Err HyperbolicAlpha(
    void* refcon,
    A_long x,
    A_long y,
    PixelType* in,
    PixelType* out)
{
    AHInfo* infoP = static_cast<AHInfo*>(refcon);

    // アルファ値を0.0～1.0に正規化
    float t;
    if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        // 32bit float版
        if ((out->alpha <= 0.0f) || (out->alpha >= 1.0f)) {
            return PF_Err_NONE;
        }
        t = out->alpha;
    }
    else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        // 16bit版
        if ((out->alpha == 0) || (out->alpha >= PF_MAX_CHAN16)) {
            return PF_Err_NONE;
        }
        t = static_cast<float>(out->alpha) / PF_MAX_CHAN16;
    }
    else {
        // 8bit版
        if ((out->alpha == 0) || (out->alpha >= PF_MAX_CHAN8)) {
            return PF_Err_NONE;
        }
        t = static_cast<float>(out->alpha) / PF_MAX_CHAN8;
    }

    // 範囲チェック
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;

    // 反転処理
    if (infoP->invert) {
        t = 1.0f - t;
    }

    // ハイパーボリック処理
    float k = (float)infoP->hyperbolic;
	float kk = k * t;
    if (kk == -1.0f) {
        kk = -0.9999f; // ゼロ除算防止
	}
    float v = ((1.0f + k) * t) / (1.0f +kk);

    // 範囲チェック
    if (v < 0.0f) v = 0.0f;
    else if (v > 1.0f) v = 1.0f;

    // 反転処理
    if (infoP->invert) {
        v = 1.0f - v;
    }

    // アルファ値を元のフォーマットに戻す
    if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        // 32bit float版
        out->alpha = v;
    }
    else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        // 16bit版
        out->alpha = static_cast<A_u_short>(v * PF_MAX_CHAN16);
    }
    else {
        // 8bit版
        out->alpha = static_cast<A_u_char>(v * PF_MAX_CHAN8);
    }

    return PF_Err_NONE;
}
static PF_Err HyperbolicAlpha8(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel* in,
    PF_Pixel* out)
{
    return HyperbolicAlpha<PF_Pixel, A_u_char>(refcon, x, y, in, out);
}
static PF_Err HyperbolicAlpha16(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel16* in,
    PF_Pixel16* out)
{
    return HyperbolicAlpha<PF_Pixel16, A_u_short>(refcon, x, y, in, out);
}
static PF_Err HyperbolicAlphaFloat(
    void* refcon,
    A_long x,
    A_long y,
    PF_PixelFloat* in,
    PF_PixelFloat* out)
{
    return HyperbolicAlpha<PF_PixelFloat, PF_FpShort>(refcon, x, y, in, out);
}
//-----------------------------------------------------------------------------------
// 実処理本体
//-----------------------------------------------------------------------------------
static PF_Err HyperbolicAlphaImpl(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    PF_FpLong		hyperbolic
)
{
    PF_Err err = PF_Err_NONE;

	AHInfo info;
    if (hyperbolic >= 0) {
        info.hyperbolic = hyperbolic;
        info.invert = TRUE;
    }
    else {
        info.hyperbolic = hyperbolic *-1;
        info.invert = FALSE;
    }
    PF_WorldSuite2* ws2P;
    PF_PixelFormat pixelFormat;

    AEGP_SuiteHandler* suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
    AEFX_AcquireSuite(in_dataP,
        out_dataP,
        kPFWorldSuite,
        kPFWorldSuiteVersion2,
        "Couldn't load suite.",
        (void**)&(ws2P));
    ws2P->PF_GetPixelFormat(worldP, &pixelFormat);

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        return suitesP->Iterate8Suite1()->iterate(
            in_dataP,
            0,														// progress base
            worldP->height,	// progress final
            NULL,		// src 
            NULL,		// area - null for all pixels
            &info,		// refcon - your custom data pointer
            HyperbolicAlpha8,		// pixel function pointer
            worldP);	// dest
        break;
    case PF_PixelFormat_ARGB64:
        return suitesP->Iterate16Suite1()->iterate(
            in_dataP,
            0,
            worldP->height,
            NULL,
            NULL,
            &info,
            HyperbolicAlpha16,
            worldP);
        break;
    case PF_PixelFormat_ARGB128:
        return suitesP->IterateFloatSuite1()->iterate(
            in_dataP,
            0,
            worldP->height,
            NULL,
            NULL,
            &info,
            HyperbolicAlphaFloat,
            worldP);
        break;
    default:
        err = PF_Err_BAD_CALLBACK_PARAM;
        break;
    }
    return err;
}

//-----------------------------------------------------------------------------------
// 公開関数
//-----------------------------------------------------------------------------------
PF_Err HyperbolicAlpha(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    PF_FpLong		hyperbolic
)
{
    return HyperbolicAlphaImpl(in_dataP, out_dataP, worldP, hyperbolic);
}
//-----------------------------------------------------------------------------------
// 実処理本体
//-----------------------------------------------------------------------------------
static PF_Err HyperbolicAlphaImpl(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    PF_Iterate8Suite1 *iter_scopeP,
    PF_FpLong		hyperbolic
)
{
    PF_Err err = PF_Err_NONE;

    AHInfo info;
    if (hyperbolic >= 0) {
        info.hyperbolic = hyperbolic;
        info.invert = TRUE;
    }
    else {
        info.hyperbolic = hyperbolic * -1;
        info.invert = FALSE;
    }

    AEGP_SuiteHandler* suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
   

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        return suitesP->Iterate8Suite2()->iterate(
            in_dataP,
            0,														// progress base
            worldP->height,	// progress final
            worldP,		// src 
            NULL,		// area - null for all pixels
            &info,		// refcon - your custom data pointer
            HyperbolicAlpha8,		// pixel function pointer
            worldP);	// dest
        break;
    case PF_PixelFormat_ARGB64:
        return suitesP->Iterate16Suite2()->iterate(
            in_dataP,
            0,
            worldP->height,
            worldP,
            NULL,
            &info,
            HyperbolicAlpha16,
            worldP);
        break;
    case PF_PixelFormat_ARGB128:
        return suitesP->IterateFloatSuite2()->iterate(
            in_dataP,
            0,
            worldP->height,
            worldP,
            NULL,
            &info,
            HyperbolicAlphaFloat,
            worldP);
        break;
    default:
        err = PF_Err_BAD_CALLBACK_PARAM;
        break;
    }
    return err;
}

//-----------------------------------------------------------------------------------
// 公開関数
//-----------------------------------------------------------------------------------
PF_Err HyperbolicAlpha(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    PF_Iterate8Suite1 *iter_scopeP,
    PF_FpLong		hyperbolic
)
{
    return HyperbolicAlphaImpl(in_dataP, worldP, pixelFormat, iter_scopeP,hyperbolic);
}
//-----------------------------------------------------------------------------------
// 実処理本体
//-----------------------------------------------------------------------------------
PF_Err HyperbolicAlpha(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpLong		hyperbolic
)
{
    PF_Err err = PF_Err_NONE;

    AHInfo info;
    if (hyperbolic >= 0) {
        info.hyperbolic = hyperbolic;
        info.invert = TRUE;
    }
    else {
        info.hyperbolic = hyperbolic * -1;
        info.invert = FALSE;
    }



    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        return suitesP->Iterate8Suite2()->iterate(
            in_dataP,
            0,														// progress base
            worldP->height,	// progress final
            worldP,		// src 
            NULL,		// area - null for all pixels
            &info,		// refcon - your custom data pointer
            HyperbolicAlpha8,		// pixel function pointer
            worldP);	// dest
        break;
    case PF_PixelFormat_ARGB64:
        return suitesP->Iterate16Suite2()->iterate(
            in_dataP,
            0,
            worldP->height,
            worldP,
            NULL,
            &info,
            HyperbolicAlpha16,
            worldP);
        break;
    case PF_PixelFormat_ARGB128:
        return suitesP->IterateFloatSuite2()->iterate(
            in_dataP,
            0,
            worldP->height,
            worldP,
            NULL,
            &info,
            HyperbolicAlphaFloat,
            worldP);
        break;
    default:
        err = PF_Err_BAD_CALLBACK_PARAM;
        break;
    }
    return err;
}