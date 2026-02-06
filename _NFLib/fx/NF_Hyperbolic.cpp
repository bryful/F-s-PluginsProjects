#include "NF_AlphaHyperbolic.h"

typedef struct AHInfo {
    PF_FpLong		alpha_hyperbolic;
    PF_Boolean		alpha_invert;
    PF_FpLong		rgb_hyperbolic;
    PF_Boolean		rgb_invert;
} AHInfo, * AHInfoP, ** AHInfoH;

//-----------------------------------------------------------------------------------
// テンプレート版: 8bit/16bit/32bit共通処理
//-----------------------------------------------------------------------------------
template <typename PixelType, typename ComponentType>
static PF_Err HyperbolicPixel(
    void* refcon,
    A_long x,
    A_long y,
    PixelType* in,
    PixelType* out)
{
    AHInfo* infoP = static_cast<AHInfo*>(refcon);

    float r,r2;
	float g,g2;
	float b,b2;
    float a,a2;
    if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        // 32bit float版
		r2 = r = out->red;
		g2 = g = out->green;
		b2 = b = out->blue;
        a2 = a = out->alpha;
    }
    else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        r2 = r = static_cast<float>(out->red) / PF_MAX_CHAN16;
        g2 = g = static_cast<float>(out->green) / PF_MAX_CHAN16;
		b2 = b = static_cast<float>(out->blue) / PF_MAX_CHAN16;
        a2 = a = static_cast<float>(out->alpha) / PF_MAX_CHAN16;
    }
    else {
        r2 = r = static_cast<float>(out->red) / PF_MAX_CHAN8;
		g2 = g = static_cast<float>(out->green) / PF_MAX_CHAN8;
		b2 = b = static_cast<float>(out->blue) / PF_MAX_CHAN8;
        a2 = a = static_cast<float>(out->alpha) / PF_MAX_CHAN8;
    }

    float k = (float)infoP->alpha_hyperbolic;
    if (k != 0 && a > 0 && a < 1) {
        if (a < 0.0f) a = 0.0f; else if (a > 1.0f) a = 1.0f;
        if (!infoP->alpha_invert) {
            a = 1.0f - a;
        }
        a2 = ((1.0f + k) * a) / (1.0f + k * a);
        if (a2 < 0.0f) a2 = 0.0f; else if (a2 > 1.0f) a2 = 1.0f;
        if (!infoP->alpha_invert) {
            a2 = 1.0f - a2;
        }
    }
    k = (float)infoP->rgb_hyperbolic;
    if (k != 0&& r > 0 && r < 1) {
        if (r < 0.0f) r = 0.0f; else if (r > 1.0f) r = 1.0f;
        if (!infoP->rgb_invert) {
            r = 1.0f - r;
        }
        r2 = ((1.0f + k) * r) / (1.0f + k * r);
        if (r2 < 0.0f) r2 = 0.0f; else if (r2 > 1.0f) r2 = 1.0f;
        if (!infoP->rgb_invert) {
            r2 = 1.0f - r2;
        }
    }
    if (k != 0 && g > 0 && g < 1) {
        if (g < 0.0f) g = 0.0f; else if (g > 1.0f) g = 1.0f;
        if (!infoP->rgb_invert) {
            g = 1.0f - g;
        }
        g2 = ((1.0f + k) * g) / (1.0f + k * g);
        if (g2 < 0.0f) g2 = 0.0f; else if (g2 > 1.0f) g2 = 1.0f;
        if (!infoP->rgb_invert) {
            g2 = 1.0f - g2;
        }
    }
    if (k != 0 && b > 0 && b < 1) {
        if (b < 0.0f) b = 0.0f; else if (b > 1.0f) b = 1.0f;
        if (!infoP->rgb_invert) {
            b = 1.0f - b;
        }
        b2 = ((1.0f + k) * b) / (1.0f + k * b);
        if (b2 < 0.0f) b2 = 0.0f; else if (b2 > 1.0f) b2 = 1.0f;
        if (!infoP->rgb_invert) {
            b2 = 1.0f - b2;
        }
	}

 

    // アルファ値を元のフォーマットに戻す
    if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        // 32bit float版
        out->alpha = a2;
		out->red = r2;
		out->green = g2;
		out->blue = b2;

    }
    else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        // 16bit版
        out->alpha = static_cast<A_u_short>(a2 * PF_MAX_CHAN16);
        out->red = static_cast<A_u_short>(r2 * PF_MAX_CHAN16);
		out->green = static_cast<A_u_short>(g2 * PF_MAX_CHAN16);
		out->blue = static_cast<A_u_short>(b2 * PF_MAX_CHAN16);

    }
    else {
        // 8bit版
        out->alpha = static_cast<A_u_char>(a2 * PF_MAX_CHAN8);
        out->red = static_cast<A_u_char>(r2 * PF_MAX_CHAN8);
		out->green = static_cast<A_u_char>(g2 * PF_MAX_CHAN8);
		out->blue = static_cast<A_u_char>(b2 * PF_MAX_CHAN8);
    }

    return PF_Err_NONE;
}
static PF_Err HyperbolicPixel8(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel* in,
    PF_Pixel* out)
{
    return HyperbolicPixel<PF_Pixel, A_u_char>(refcon, x, y, in, out);
}
static PF_Err HyperbolicPixel16(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel16* in,
    PF_Pixel16* out)
{
    return HyperbolicPixel<PF_Pixel16, A_u_short>(refcon, x, y, in, out);
}
static PF_Err HyperbolicPixelFloat(
    void* refcon,
    A_long x,
    A_long y,
    PF_PixelFloat* in,
    PF_PixelFloat* out)
{
    return HyperbolicPixel<PF_PixelFloat, PF_FpShort>(refcon, x, y, in, out);
}
//-----------------------------------------------------------------------------------
// 実処理本体
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// 実処理本体
//-----------------------------------------------------------------------------------
PF_Err Hyperbolic(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpLong		rgb_hyperbolic,
	PF_FpLong		alpha_hyperbolic
)
{
    PF_Err err = PF_Err_NONE;
    if (rgb_hyperbolic ==0 && alpha_hyperbolic==0) {
        return err;
	}
    AHInfo info;
    if (rgb_hyperbolic >= 0) {
        info.rgb_hyperbolic = rgb_hyperbolic;
        info.rgb_invert = FALSE;
    }
    else {
        info.rgb_hyperbolic = rgb_hyperbolic * -1;
        info.rgb_invert = TRUE;
    }

    if (alpha_hyperbolic >= 0) {
        info.alpha_hyperbolic = alpha_hyperbolic;
        info.alpha_invert = FALSE;
    }
    else {
        info.alpha_hyperbolic = alpha_hyperbolic * -1;
        info.alpha_invert = TRUE;
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
            HyperbolicPixel8,		// pixel function pointer
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
            HyperbolicPixel16,
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
            HyperbolicPixelFloat,
            worldP);
        break;
    default:
        err = PF_Err_BAD_CALLBACK_PARAM;
        break;
    }
    return err;
}

