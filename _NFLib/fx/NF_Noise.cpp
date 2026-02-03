#include "NF_Noise.h"

/**
 * 決定論的なハッシュ関数 (0.0 - 1.0)
 */
inline float hash_01(int x, int y, int seed) {
    uint32_t n = x * 374761393 + y * 668265263 + seed * 1103515245;
    n = (n ^ (n >> 13)) * 1274126177;
    return (float)(n & 0x7fffffff) / 2147483647.0f;
}

/**
 * バリューノイズ (3-6pxの粒感を作るための補間処理)
 */
float GetValueNoise(float x, float y, float size, int seed) {
    int ix = (int)floor(x / size);
    int iy = (int)floor(y / size);
    float fx = (x / size) - (float)ix;
    float fy = (y / size) - (float)iy;

    float v00 = hash_01(ix, iy, seed);
    float v10 = hash_01(ix + 1, iy, seed);
    float v01 = hash_01(ix, iy + 1, seed);
    float v11 = hash_01(ix + 1, iy + 1, seed);

    float ux = fx * fx * (3.0f - 2.0f * fx);
    float uy = fy * fy * (3.0f - 2.0f * fy);

    return (v00 * (1.0f - ux) + v10 * ux) * (1.0f - uy) +
        (v01 * (1.0f - ux) + v11 * ux) * uy;
}

// -------------------------------------------------------------------
// データ構造
// -------------------------------------------------------------------

typedef struct {
    PF_EffectWorld* output;
    float noise_size;
    float amount;
    float downsample_x;
    float downsample_y;
    int seed;
    bool is_color;
} NoiseData;

typedef struct {
    PF_EffectWorld* output;
    float amount;
    int seed;
    int num_blocks; // 追加：アクセントの数
} BurstData;

// -------------------------------------------------------------------
// 1. 基本ノイズ（ザラザラ）処理
// -------------------------------------------------------------------

template <typename T>
static PF_Err MyNoiseScanlineInPlaceT(
    void* refconPV,
    A_long thread_indexL,
    A_long i,
    A_long iterationsL)
{
    NoiseData* data = (NoiseData*)refconPV;
    A_long y = i;
    T* p = (T*)((char*)data->output->data + (y * data->output->rowbytes));
    int width = data->output->width;

    for (int x = 0; x < width; ++x) {
        float sample_x = (float)x / data->downsample_x;
        float sample_y = (float)y / data->downsample_y;

        float n_r, n_g, n_b;
        if (data->is_color) {
            n_r = GetValueNoise(sample_x, sample_y, data->noise_size, data->seed);
            n_g = GetValueNoise(sample_x, sample_y, data->noise_size, data->seed + 100);
            n_b = GetValueNoise(sample_x, sample_y, data->noise_size, data->seed + 200);
        }
        else {
            n_r = n_g = n_b = GetValueNoise(sample_x, sample_y, data->noise_size, data->seed);
        }

        float off_r = (n_r * 2.0f) - 1.0f;
        float off_g = (n_g * 2.0f) - 1.0f;
        float off_b = (n_b * 2.0f) - 1.0f;

        if constexpr (std::is_same_v<T, PF_Pixel8>) {
            p->red = (A_u_char)AE_CLAMP((int)p->red + (int)(off_r * data->amount), 0, 255);
            p->green = (A_u_char)AE_CLAMP((int)p->green + (int)(off_g * data->amount), 0, 255);
            p->blue = (A_u_char)AE_CLAMP((int)p->blue + (int)(off_b * data->amount), 0, 255);
        }
        else if constexpr (std::is_same_v<T, PF_Pixel16>) {
            float amt16 = data->amount * 128.0f;
            p->red = (A_u_short)AE_CLAMP((int)p->red + (int)(off_r * amt16), 0, 32768);
            p->green = (A_u_short)AE_CLAMP((int)p->green + (int)(off_g * amt16), 0, 32768);
            p->blue = (A_u_short)AE_CLAMP((int)p->blue + (int)(off_b * amt16), 0, 32768);
        }
        else if constexpr (std::is_same_v<T, PF_PixelFloat>) {
            float amtF = data->amount / 255.0f;
            p->red += off_r * amtF;
            p->green += off_g * amtF;
            p->blue += off_b * amtF;
        }
        p++;
    }
    return PF_Err_NONE;
}

// -------------------------------------------------------------------
// 2. ブロックシフト（アクセント）処理
// -------------------------------------------------------------------

template <typename T>
static PF_Err MyBurstScanlineInPlaceT(
    void* refconPV,
    A_long thread_indexL,
    A_long i,
    A_long iterationsL)
{
    BurstData* data = (BurstData*)refconPV;
    A_long y = i;
    T* p = (T*)((char*)data->output->data + (y * data->output->rowbytes));
    int width = data->output->width;
    int height = data->output->height;

    // パラメータから渡されたブロック数を使用
    for (int k = 0; k < data->num_blocks; ++k) {
        int b_y_start = static_cast<int>(hash_01(k, 10, data->seed) * height);
        int b_h = static_cast<int>(10 + hash_01(k, 20, data->seed) * 90);

        if (y < b_y_start || y >= b_y_start + b_h) continue;

        int b_x_start = static_cast<int>(hash_01(k, 30, data->seed) * width);
        int b_w = static_cast<int>(10 + hash_01(k, 40, data->seed) * 90);
        int b_x_end = b_x_start + b_w;

        int shift_x = static_cast<int>((hash_01(k, 50, data->seed) - 0.5f) * 200.0f * (data->amount / 100.0f));
        int micro_jitter = (hash_01(y, k, data->seed) > 0.5f) ? 1 : -1;

        for (int x = 0; x < width; ++x) {
            if (x >= b_x_start && x < b_x_end) {
                int src_x = AE_CLAMP(x + shift_x + micro_jitter, 0, width - 1);
                T* src_p = (T*)((char*)data->output->data + (y * data->output->rowbytes)) + src_x;
                p[x] = *src_p;
            }
        }
    }
    return PF_Err_NONE;
}

PF_Err NoiseAndBlock(
    PF_InData* in_data,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpShort noise_size,
    PF_FpShort amount,
    PF_FpShort accent_amount, // 追加
    A_long seed,
    A_Boolean is_color
)
{
    PF_Err err = PF_Err_NONE;
    auto iterateSuite = suitesP->Iterate8Suite2();

    // 1. ベースノイズ（ザラザラ）
    NoiseData nData;
    nData.output = worldP;
    nData.noise_size = (float)noise_size;
    nData.amount = (float)amount;
    nData.seed = seed;
    nData.is_color = (is_color != 0);
    nData.downsample_x = (float)in_data->downsample_x.num / in_data->downsample_x.den;
    nData.downsample_y = (float)in_data->downsample_y.num / in_data->downsample_y.den;

    if (pixelFormat == PF_PixelFormat_ARGB128)
        err = iterateSuite->iterate_generic(worldP->height, &nData, MyNoiseScanlineInPlaceT<PF_PixelFloat>);
    else if (pixelFormat == PF_PixelFormat_ARGB64)
        err = iterateSuite->iterate_generic(worldP->height, &nData, MyNoiseScanlineInPlaceT<PF_Pixel16>);
    else
        err = iterateSuite->iterate_generic(worldP->height, &nData, MyNoiseScanlineInPlaceT<PF_Pixel8>);

    // 2. ブロックシフトアクセント
    // accent_amount(0.0-100.0) を最大20個のブロック数に変換
    int num_blocks = static_cast<int>((accent_amount / 100.0f) * 20.0f);

    if (num_blocks > 0) {
        BurstData bData;
        bData.output = worldP;
        bData.amount = (float)amount;
        bData.seed = seed + 500;
        bData.num_blocks = num_blocks;

        if (pixelFormat == PF_PixelFormat_ARGB128)
            iterateSuite->iterate_generic(worldP->height, &bData, MyBurstScanlineInPlaceT<PF_PixelFloat>);
        else if (pixelFormat == PF_PixelFormat_ARGB64)
            iterateSuite->iterate_generic(worldP->height, &bData, MyBurstScanlineInPlaceT<PF_Pixel16>);
        else
            iterateSuite->iterate_generic(worldP->height, &bData, MyBurstScanlineInPlaceT<PF_Pixel8>);
    }

    return err;
}
PF_Err Noise(
    PF_InData* in_data,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpShort noise_size,
    PF_FpShort amount,
    A_long seed,
    A_Boolean is_color
)
{
    PF_Err err = PF_Err_NONE;
    auto iterateSuite = suitesP->Iterate8Suite2();

    // 1. ベースノイズ（ザラザラ）
    NoiseData nData;
    nData.output = worldP;
    nData.noise_size = (float)noise_size;
    nData.amount = (float)amount;
    nData.seed = seed;
    nData.is_color = (is_color != 0);
    nData.downsample_x = (float)in_data->downsample_x.num / in_data->downsample_x.den;
    nData.downsample_y = (float)in_data->downsample_y.num / in_data->downsample_y.den;

    if (pixelFormat == PF_PixelFormat_ARGB128)
        err = iterateSuite->iterate_generic(worldP->height, &nData, MyNoiseScanlineInPlaceT<PF_PixelFloat>);
    else if (pixelFormat == PF_PixelFormat_ARGB64)
        err = iterateSuite->iterate_generic(worldP->height, &nData, MyNoiseScanlineInPlaceT<PF_Pixel16>);
    else
        err = iterateSuite->iterate_generic(worldP->height, &nData, MyNoiseScanlineInPlaceT<PF_Pixel8>);


    return err;
}