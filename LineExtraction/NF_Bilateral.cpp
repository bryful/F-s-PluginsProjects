#include "LineExtraction.h"

// 内部用コンテキスト
struct BilateralInfo {
    PF_EffectWorld* src;
    PF_EffectWorld* dst;
    BilateralContext* ctx;
};

// --- 輝度算出ヘルパー ---
template <typename T>
inline float GetLuma01(const T* p);

template <>
inline float GetLuma01<PF_Pixel8>(const PF_Pixel8* p) {
    return (0.299f * (float)p->red + 0.587f * (float)p->green + 0.114f * (float)p->blue) / 255.0f;
}

template <>
inline float GetLuma01<PF_Pixel16>(const PF_Pixel16* p) {
    return (0.299f * (float)p->red + 0.587f * (float)p->green + 0.114f * (float)p->blue) / 32768.0f;
}

template <>
inline float GetLuma01<PF_PixelFloat>(const PF_PixelFloat* p) {
    return (0.299f * p->red + 0.587f * p->green + 0.114f * p->blue);
}

// --- 重みテーブルの初期化 ---
void SetupBilateralContext(BilateralContext* ctx, int radius, float sigma_s, float sigma_r) {
    ctx->radius = radius;
    ctx->kernel_size = radius * 2 + 1;
    ctx->sigma_s = sigma_s;
    ctx->sigma_r = sigma_r;

    // Spatial LUT (距離の重み)
    float s_divisor = 2.0f * sigma_s * sigma_s + 0.00001f;
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            ctx->spatial_lut[(y + radius) * ctx->kernel_size + (x + radius)] =
                expf(-(float)(x * x + y * y) / s_divisor);
        }
    }

    // Range LUT (8bit専用: 輝度差 0.0-1.0 を 256分割して事前計算)
    float r_divisor = 2.0f * sigma_r * sigma_r;
    if (r_divisor < 0.00001f) r_divisor = 0.00001f; // ゼロ除算・消失防止

    for (int i = 0; i < 256; i++) {
        float diff = (float)i / 255.0f;
        ctx->range_lut[i] = expf(-(diff * diff) / r_divisor);
    }
}

// --- フィルタ本体 (Template) ---
template <typename T, typename U, int MAX_VAL_INT>
static PF_Err BilateralFilterGeneric(
    void* refcon,
    A_long thread_idx,
    A_long iterNum,
    A_long iterTotal)
{
    BilateralInfo* info = reinterpret_cast<BilateralInfo*>(refcon);
    BilateralContext* ctx = info->ctx;

    A_long y = iterNum;
    T* src_row_ptr = reinterpret_cast<T*>((uint8_t*)info->src->data + (y * info->src->rowbytes));
    T* dst_row_ptr = reinterpret_cast<T*>((uint8_t*)info->dst->data + (y * info->dst->rowbytes));

    // 16/32bit用計算用の分母を事前に用意
    float r_v = 2.0f * ctx->sigma_r * ctx->sigma_r;
    if (r_v < 0.00001f) r_v = 0.00001f;

    for (A_long x = 0; x < info->src->width; x++) {
        T* center_pixel = &src_row_ptr[x];
        float center_luma = GetLuma01(center_pixel);

        float sum_r = 0, sum_g = 0, sum_b = 0, sum_w = 0;

        for (int ky = -ctx->radius; ky <= ctx->radius; ky++) {
            A_long cur_y = AE_CLAMP(y + ky, 0, info->src->height - 1);
            T* kernel_row_ptr = reinterpret_cast<T*>((uint8_t*)info->src->data + (cur_y * info->src->rowbytes));

            for (int kx = -ctx->radius; kx <= ctx->radius; kx++) {
                A_long cur_x = AE_CLAMP(x + kx, 0, info->src->width - 1);
                T* p = &kernel_row_ptr[cur_x];

                // 1. 空間重み (LUT参照)
                float weight = ctx->spatial_lut[(ky + ctx->radius) * ctx->kernel_size + (kx + ctx->radius)];

                // 2. 輝度重み
                float neighbor_luma = GetLuma01(p);
                float luma_diff = center_luma - neighbor_luma;

                if constexpr (MAX_VAL_INT == 255) {
                    // 8bit: 事前計算LUTを使用
                    int diff_idx = static_cast<int>(fabsf(luma_diff) * 255.0f + 0.5f);
                    weight *= ctx->range_lut[AE_CLAMP(diff_idx, 0, 255)];
                }
                else {
                    // 16/32bit: 直接計算
                    weight *= expf(-(luma_diff * luma_diff) / r_v);
                }

                sum_r += (float)p->red * weight;
                sum_g += (float)p->green * weight;
                sum_b += (float)p->blue * weight;
                sum_w += weight;
            }
        }

        if (sum_w > 0.0f) {
            float inv_w = 1.0f / sum_w;
            if constexpr (MAX_VAL_INT > 0) {
                // 8bit / 16bit 整数型
                dst_row_ptr[x].red = static_cast<U>(static_cast<int>(AE_CLAMP(sum_r * inv_w + 0.5f, 0.0f, (float)MAX_VAL_INT)));
                dst_row_ptr[x].green = static_cast<U>(static_cast<int>(AE_CLAMP(sum_g * inv_w + 0.5f, 0.0f, (float)MAX_VAL_INT)));
                dst_row_ptr[x].blue = static_cast<U>(static_cast<int>(AE_CLAMP(sum_b * inv_w + 0.5f, 0.0f, (float)MAX_VAL_INT)));
            }
            else {
                // 32bit Float
                dst_row_ptr[x].red = sum_r * inv_w;
                dst_row_ptr[x].green = sum_g * inv_w;
                dst_row_ptr[x].blue = sum_b * inv_w;
            }
        }
        dst_row_ptr[x].alpha = center_pixel->alpha;
    }
    return PF_Err_NONE;
}

// --- 実行関数 ---
PF_Err PF_BilateralExec(NF_AE* ae, ParamInfo* infoP)
{
    PF_Err err = PF_Err_NONE;

    BilateralContext ctx;
    SetupBilateralContext(
        &ctx,
        infoP->bilateralCtx.radius,
        infoP->bilateralCtx.sigma_s,
        infoP->bilateralCtx.sigma_r);

    BilateralInfo info;
    info.src = ae->input;
    info.dst = ae->output;
    info.ctx = &ctx;

    switch (ae->pixelFormat())
    {
    case PF_PixelFormat_ARGB128:
        err = ae->suitesP->Iterate8Suite2()->iterate_generic(
            ae->output->height, &info, BilateralFilterGeneric<PF_PixelFloat, float, 0>);
        break;

    case PF_PixelFormat_ARGB64:
        err = ae->suitesP->Iterate8Suite2()->iterate_generic(
            ae->output->height, &info, BilateralFilterGeneric<PF_Pixel16, A_u_short, 32768>);
        break;

    case PF_PixelFormat_ARGB32:
        err = ae->suitesP->Iterate8Suite2()->iterate_generic(
            ae->output->height, &info, BilateralFilterGeneric<PF_Pixel8, A_u_char, 255>);
        break;

    default:
        err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
        break;
    }

    return err;
}