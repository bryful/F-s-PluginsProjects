#include "Emboss.h"
// 内部計算用スレッド共有データ

typedef struct {
    PF_EffectWorld* input_world;
    PF_EffectWorld* output_world;
    float           dx;
    float           dy;
} EmbossGenericPayload;

// --- 1. ビット深度ごとの特性を定義するテンプレート ---
template <typename T> struct EmbossTraits;

template <> struct EmbossTraits<PF_Pixel8> {
    typedef A_u_char comp;
    static inline comp half() { return 128; }
    static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 255); }
};

template <> struct EmbossTraits<PF_Pixel16> {
    typedef A_u_short comp;
    static inline comp half() { return 16384; } // AE 16bitは32768が上限
    static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 32768); }
};

template <> struct EmbossTraits<PF_PixelFloat> {
    typedef PF_FpShort comp;
    static inline comp half() { return 0.5f; }
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

// --- 3. テンプレート化されたカーネル ---
template <typename T>
static PF_Err EmbossKernel(void* refcon, A_long t_idx, A_long y, A_long t_total) {
    EmbossGenericPayload* pld = (EmbossGenericPayload*)refcon;
    typedef EmbossTraits<T> Traits;

    T* out_ptr = (T*)((char*)pld->output_world->data + (y * pld->output_world->rowbytes));
    T* in_ptr = (T*)((char*)pld->input_world->data + (y * pld->input_world->rowbytes));

    for (A_long x = 0; x < pld->output_world->width; ++x) {
        float r1, g1, b1, r2, g2, b2;

        SampleBilinear<T>(pld->input_world, (float)x + pld->dx, (float)y + pld->dy, &r1, &g1, &b1);
        SampleBilinear<T>(pld->input_world, (float)x - pld->dx, (float)y - pld->dy, &r2, &g2, &b2);

        out_ptr->red = Traits::clamp((r1 - r2) + Traits::half());
        out_ptr->green = Traits::clamp((g1 - g2) + Traits::half());
        out_ptr->blue = Traits::clamp((b1 - b2) + Traits::half());
        out_ptr->alpha = in_ptr->alpha;

        out_ptr++; in_ptr++;
    }
    return PF_Err_NONE;
}

// --- 4. メインのエントリポイント ---
PF_Err EmbossFilter(NF_AE* ae, ParamInfo* infoP) {
    // 角度修正: 0度を12時方向にする
    float angle = (float)infoP->direction + 90.0f;
    float dist = (float)infoP->distance;
    float rad = angle * (float)M_PI / 180.0f;

    EmbossGenericPayload pld;
    pld.input_world = ae->input;
    pld.output_world = ae->output;
    pld.dx = cosf(rad) * dist;
    pld.dy = sinf(rad) * dist;

    // フォーマットに応じてテンプレートをディスパッチ
    switch (ae->pixelFormat()) {
    case PF_PixelFormat_ARGB32:
        return ae->in_data->utils->iterate_generic(ae->output->height, &pld, EmbossKernel<PF_Pixel8>);
    case PF_PixelFormat_ARGB64:
        return ae->in_data->utils->iterate_generic(ae->output->height, &pld, EmbossKernel<PF_Pixel16>);
    case PF_PixelFormat_ARGB128:
        return ae->in_data->utils->iterate_generic(ae->output->height, &pld, EmbossKernel<PF_PixelFloat>);
    }
    return PF_Err_BAD_CALLBACK_PARAM;
}