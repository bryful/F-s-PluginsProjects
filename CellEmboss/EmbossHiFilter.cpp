#include "CellEmboss.h"
// 内部計算用スレッド共有データ

typedef struct {
    //PF_EffectWorld* input_world;
    PF_EffectWorld* output_world;
	PF_Boolean	    isHi;
    PF_Boolean	    isLo;
    float           dHIx;
    float           dHIy;
    float           dLOx;
    float           dLOy;
} EmbossHiGenericPayload;

// --- 1. ビット深度ごとの特性を定義するテンプレート ---
template <typename T> struct CellEmbossTraits;

template <> struct CellEmbossTraits<PF_Pixel8> {
    typedef A_u_char comp;
    static inline comp half() { return 128; }
    static inline comp max_C() { return 255; }
    static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 255); }
};

template <> struct CellEmbossTraits<PF_Pixel16> {
    typedef A_u_short comp;
    static inline comp half() { return 16384; } // AE 16bitは32768が上限
    static inline comp max_C() { return 32768; }
    static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 32768); }
};

template <> struct CellEmbossTraits<PF_PixelFloat> {
    typedef PF_FpShort comp;
    static inline comp half() { return 0.5f; }
    static inline comp max_C() { return 1.0f; }
    static inline comp clamp(float v) { return (comp)v; } // Floatは基本クランプ不要
};

// --- 2. 汎用バイリニアサンプリング ---
template <typename T>
inline void SampleBilinear(PF_EffectWorld* world, float x, float y, float* r, float* g, float* b) {
    float wf = (float)world->width;
    float hf = (float)world->height;

    // 安全な範囲にクランプ
    x = AE_CLAMP(x, 0.0f, wf - 1.001f);
    y = AE_CLAMP(y, 0.0f, hf - 1.001f);

    int xi = (int)x;
    int yi = (int)y;
    float xf = x - xi;
    float yf = y - yi;

    T* p00 = (T*)((char*)world->data + (yi * world->rowbytes) + (xi * sizeof(T)));
    T* p01 = p00 + 1;
    T* p10 = (T*)((char*)p00 + world->rowbytes);
    T* p11 = p10 + 1;

    float w00 = (1.0f - xf) * (1.0f - yf);
    float w01 = xf * (1.0f - yf);
    float w10 = (1.0f - xf) * yf;
    float w11 = xf * yf;

    *r = p00->red * w00 + p01->red * w01 + p10->red * w10 + p11->red * w11;
    *g = p00->green * w00 + p01->green * w01 + p10->green * w10 + p11->green * w11;
    *b = p00->blue * w00 + p01->blue * w01 + p10->blue * w10 + p11->blue * w11;
}
template <typename T>
inline void SampleNear(PF_EffectWorld* world, float x, float y, float* r, float* g, float* b) {
    float wf = (float)world->width;
    float hf = (float)world->height;

    // 安全な範囲にクランプ
    A_long xi = AE_CLAMP((A_long)(x+0.5), 0, world->width - 1);
    A_long yi = AE_CLAMP((A_long)(y+0.5), 0, world->height - 1);

    //T* p00 = (T*)((char*)world->data + (yi * world->rowbytes) + (xi * sizeof(T)));


    // rowbytesは行ごとのバイト数なので、(y * rowbytes) は正しい。
     // x方向は (x * 1ピクセルあたりのサイズ) を足す。
    T* p00 = reinterpret_cast<T*>(reinterpret_cast<char*>(world->data) +
        (yi * world->rowbytes) +
        (xi * sizeof(T)));

    // 4. 値の取得
    // AEのピクセル値は、8bitなら0-255、16bitなら0-32768。
    // floatとして返すならそのまま代入でOKですが、正規化(0.0-1.0)が必要な場合はここで割ります。
    *r = static_cast<float>(p00->red);
    *g = static_cast<float>(p00->green);
    *b = static_cast<float>(p00->blue);
}

// --- 3. テンプレート化されたカーネル ---
template <typename T>
static PF_Err EmbossHiKernel(void* refcon, A_long t_idx, A_long y, A_long t_total) {
    EmbossHiGenericPayload* pld = (EmbossHiGenericPayload*)refcon;
    typedef CellEmbossTraits<T> Traits;

    T* out_ptr = (T*)((char*)pld->output_world->data + (y * pld->output_world->rowbytes));

    for (A_long x = 0; x < pld->output_world->width; ++x) {
        float r1, g1, b1, r2, g2, b2;

        if (pld->isHi == TRUE) {
            SampleNear<T>(pld->output_world, (float)x + pld->dHIx, (float)y + pld->dHIy, &r1, &g1, &b1);
            SampleNear<T>(pld->output_world, (float)x - pld->dHIx, (float)y - pld->dHIy, &r2, &g2, &b2);
            float v = r1 - r2;
            if (v >= 0)
            {
                out_ptr->green = Traits::clamp(v);
            }
            else {
                out_ptr->green = 0;
            }
        }
        else {
            out_ptr->green = 0;
        }
        if (pld->isLo == TRUE) {
            SampleNear<T>(pld->output_world, (float)x + pld->dLOx, (float)y + pld->dLOy, &r1, &g1, &b1);
            SampleNear<T>(pld->output_world, (float)x - pld->dLOx, (float)y - pld->dLOy, &r2, &g2, &b2);
            float v2 = r2 - r1;
            if (v2 >= 0)
            {
                out_ptr->blue = Traits::clamp(v2);
            }
            else {
                out_ptr->blue = 0;
            }
 
        }
        else {
            out_ptr->blue = 0;
        }
        /*
        if (out_ptr->red == 0)
        {
            out_ptr->blue = 0;
            out_ptr->green = 0;
        }
        */

        //out_ptr->alpha = Traits::max_C();

        out_ptr++;
    }
    return PF_Err_NONE;
}

// --- 4. メインのエントリポイント ---
PF_Err EmbossHiFilter(NF_AE* ae, ParamInfo* infoP) {
    // 角度修正: 0度を12時方向にする
    float angle = (float)infoP->direction + 90.0f;
    float distHi = (float)infoP->distanceHI;
    float distLo = (float)infoP->distanceLO;
    float rad = angle * (float)M_PI / 180.0f;

    EmbossHiGenericPayload pld;
    pld.output_world = ae->output;
    pld.dHIx = cosf(rad) * distHi;
    pld.dHIy = sinf(rad) * distHi;
    pld.dLOx = cosf(rad) * distLo;
    pld.dLOy = sinf(rad) * distLo;
	pld.isHi = (distHi > 0.0f);
    pld.isLo = (distLo > 0.0f);
    // フォーマットに応じてテンプレートをディスパッチ
    switch (ae->pixelFormat()) {
    case PF_PixelFormat_ARGB32:
        return ae->in_data->utils->iterate_generic(ae->output->height, &pld, EmbossHiKernel<PF_Pixel8>);
    case PF_PixelFormat_ARGB64:
        return ae->in_data->utils->iterate_generic(ae->output->height, &pld, EmbossHiKernel<PF_Pixel16>);
    case PF_PixelFormat_ARGB128:
        return ae->in_data->utils->iterate_generic(ae->output->height, &pld, EmbossHiKernel<PF_PixelFloat>);
    }
    return PF_Err_BAD_CALLBACK_PARAM;
}