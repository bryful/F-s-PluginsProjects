#include "AlphaHyperbolic.h"

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
    ParamInfo* infoP = static_cast<ParamInfo*>(refcon);
    
    // アルファ値を0.0～1.0に正規化
    float t;
    if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        // 32bit float版
        if ((out->alpha <= 0.0f) || (out->alpha >= 1.0f)) {
            return PF_Err_NONE;
        }
        t = out->alpha;
    } else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        // 16bit版
        if ((out->alpha == 0) || (out->alpha >= PF_MAX_CHAN16)) {
            return PF_Err_NONE;
        }
        t = static_cast<float>(out->alpha) / PF_MAX_CHAN16;
    } else {
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
    if (!infoP->invert) {
        t = 1.0f - t;
    }
    
    // ハイパーボリック処理
    float k = infoP->hyperbolic;
    float v = ((1.0f + k) * t) / (1.0f + k * t);
    
    // 範囲チェック
    if (v < 0.0f) v = 0.0f;
    else if (v > 1.0f) v = 1.0f;
    
    // 反転処理
    if (!infoP->invert) {
        v = 1.0f - v;
    }
    
    // アルファ値を元のフォーマットに戻す
    if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        // 32bit float版
        out->alpha = v;
    } else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        // 16bit版
        out->alpha = static_cast<A_u_short>(v * PF_MAX_CHAN16);
    } else {
        // 8bit版
        out->alpha = static_cast<A_u_char>(v * PF_MAX_CHAN8);
    }

    return PF_Err_NONE;
}

//-----------------------------------------------------------------------------------
// 実処理本体
//-----------------------------------------------------------------------------------
static PF_Err HyperbolicAlphaImpl(
    CFsAE* ae,
    ParamInfo* infoP)
{
    PF_Err err = PF_Err_NONE;

    switch (ae->pixelFormat()) {
    case PF_PixelFormat_ARGB32:
        ERR(ae->iterate8(infoP, HyperbolicAlpha<PF_Pixel8, A_u_char>));
        break;
    case PF_PixelFormat_ARGB64:
        ERR(ae->iterate16(infoP, HyperbolicAlpha<PF_Pixel16, A_u_short>));
        break;
    case PF_PixelFormat_ARGB128:
        ERR(ae->iterate32(infoP, HyperbolicAlpha<PF_PixelFloat, PF_FpShort>));
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
    CFsAE* ae,
    ParamInfo* infoP)
{
    return HyperbolicAlphaImpl(ae, infoP);
}
