#include "CellEmboss.h"
#include <type_traits>

// --- ビット深度ごとの特性を定義するテンプレート ---
template <typename T> struct ECTraits;

template <> struct ECTraits<PF_Pixel8> {
    typedef A_u_char comp;
    static inline comp max_C() { return 255; }
};

template <> struct ECTraits<PF_Pixel16> {
    typedef A_u_short comp;
    static inline comp max_C() { return 32768; }
};

template <> struct ECTraits<PF_PixelFloat> {
    typedef PF_FpShort comp;
    static inline comp max_C() { return 1.0f; }
};

// --- テンプレート化した着色関数 ---
template<typename PixelType, typename AlphaType>
static PF_Err
EmbossColorise_Template(
    refconType  refcon,
    A_long      xL,
    A_long      yL,
    PixelType* inP,    // 使用しない
    PixelType* outP,   // ここからHi/Loを読み取り、色を書き込む
    AlphaType   maxValue)
{
    PF_Err err = PF_Err_NONE;
    ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

    // 1. マスクチェック (Rチャンネルが0なら完全に透明にする)
    if (outP->red == 0)
    {
        outP->red = outP->green = outP->blue = outP->alpha = 0;
        return err;
    }

    // 2. 深度に合わせた色の選択
    PixelType colHI;
    PixelType colLO;

    if constexpr (std::is_same_v<PixelType, PF_Pixel>) {
        colHI = infoP->colorHi;
        colLO = infoP->colorLo;
    }
    else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
        colHI = infoP->colorHi16;
        colLO = infoP->colorLo16;
    }
    else if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
        colHI = infoP->colorHi32;
        colLO = infoP->colorLo32;
    }

    // 3. モードに応じたチャンネルからアルファを設定し、色を塗る
    if (infoP->mode == 1) // Hi
    {
        outP->alpha = outP->green; // greenをそのままアルファに
        outP->red = colHI.red;
        outP->green = colHI.green;
        outP->blue = colHI.blue;
    }
    else if (infoP->mode == 2) // Lo
    {
        outP->alpha = outP->blue;  // blueをそのままアルファに
        outP->red = colLO.red;
        outP->green = colLO.green;
        outP->blue = colLO.blue;
    }
    else if (infoP->mode == 3) // Hi-Reverse
    {
        outP->alpha = maxValue - outP->green; // greenを反転してアルファに
        outP->red = colHI.red;
        outP->green = colHI.green;
        outP->blue = colHI.blue;
    }
    else if (infoP->mode == 4) // Hi-Lo
    {
        if (outP->green == 0 && outP->blue == 0) {
            outP->alpha = 0;
            outP->red = outP->green = outP->blue = 0;
        }else if(outP->green >0 && outP->blue == 0) {
            outP->alpha = outP->green; // greenをそのままアルファに
            outP->red = colHI.red;
            outP->green = colHI.green;
            outP->blue = colHI.blue;
        }
        else if (outP->green == 0 && outP->blue > 0) {
            outP->alpha = outP->blue;  // blueをそのままアルファに
            outP->red = colLO.red;
            outP->green = colLO.green;
			outP->blue = colLO.blue;
        }
        else {

            float hip = (float)outP->green / (float)maxValue;
            float lop = (float)outP->blue / (float)maxValue;
            float r = (float)colHI.red * hip + (float)colLO.red * lop;
            float g = (float)colHI.green * hip + (float)colLO.green * lop;
            float b = (float)colHI.blue * hip + (float)colLO.blue * lop;
            float a = AE_CLAMP((float)outP->green + (float)outP->blue - ((float)outP->green * (float)outP->blue) / maxValue,0, maxValue);
            r = AE_CLAMP(r * maxValue / a, 0, maxValue);
            g = AE_CLAMP(g * maxValue / a, 0, maxValue);
            b = AE_CLAMP(g * maxValue / a, 0, maxValue);
            outP->alpha = static_cast<AlphaType>(a);
            outP->red = static_cast<AlphaType>(r);
            outP->green = static_cast<AlphaType>(g);
            outP->blue = static_cast<AlphaType>(b);
        }
    }
    return err;
}

// 各ビット深度用のラッパー
static PF_Err EmbossColorise8(refconType ref, A_long x, A_long y, PF_Pixel* in, PF_Pixel* out) {
    return EmbossColorise_Template<PF_Pixel, A_u_char>(ref, x, y, in, out, 255);
}

static PF_Err EmbossColorise16(refconType ref, A_long x, A_long y, PF_Pixel16* in, PF_Pixel16* out) {
    return EmbossColorise_Template<PF_Pixel16, A_u_short>(ref, x, y, in, out, 32768);
}

static PF_Err EmbossColorise32(refconType ref, A_long x, A_long y, PF_PixelFloat* in, PF_PixelFloat* out) {
    return EmbossColorise_Template<PF_PixelFloat, PF_FpShort>(ref, x, y, in, out, 1.0f);
}

// フィルタメイン処理
PF_Err EmbossColoriseFilter(NF_AE* ae, ParamInfo* infoP)
{
    PF_Err err = PF_Err_NONE;

    // ソースに ae->output を指定することで、inP と outP が同じバッファ（処理中データ）を指すようにします
    switch (ae->pixelFormat()) {
    case PF_PixelFormat_ARGB32:
        return ae->suitesP->Iterate8Suite2()->iterate(ae->in_data, 0, ae->output->height, ae->output, NULL, infoP, EmbossColorise8, ae->output);
    case PF_PixelFormat_ARGB64:
        return ae->suitesP->Iterate16Suite2()->iterate(ae->in_data, 0, ae->output->height, ae->output, NULL, infoP, EmbossColorise16, ae->output);
    case PF_PixelFormat_ARGB128:
        return ae->suitesP->IterateFloatSuite2()->iterate(ae->in_data, 0, ae->output->height, ae->output, NULL, infoP, EmbossColorise32, ae->output);
    }
    return err;
}