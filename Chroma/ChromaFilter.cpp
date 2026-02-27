#include "ChromaFilter.h"
#include <type_traits> // static_assert や is_same 用

// ----------------------------------------------------------------------
// 1. イテレーション用データ構造
// ----------------------------------------------------------------------
typedef struct {
    PF_FpShort center_x;
    PF_FpShort center_y;
    PF_FpShort inv_scale_r;
    PF_FpShort inv_scale_g;
    PF_FpShort inv_scale_b;
    PF_EffectWorld* input_world;
	A_long alpha_mode; // 0:アルファは変更しない, 1:アルファも変化させる
} ChromaticInfo;

// チャンネルの最大値を取得するヘルパーテンプレート
template <typename T> inline float GetMaxVal();
template <> inline float GetMaxVal<PF_Pixel8>() { return 255.0f; }
template <> inline float GetMaxVal<PF_Pixel16>() { return 32768.0f; }
template <> inline float GetMaxVal<PF_PixelFloat>() { return 1.0f; }

// テンプレート化された単一チャンネルサンプラー
template <typename T>
static inline float SampleChannel(
    PF_EffectWorld* input,
    float x,
    float y,
    int channel_offset)
{
    // 範囲外クランプ（バイリニア参照用に1px内側で制限）
    x = MAX(0.0f, MIN(x, (float)input->width - 1.001f));
    y = MAX(0.0f, MIN(y, (float)input->height - 1.001f));

    int x1 = static_cast<int>(x);
    int y1 = static_cast<int>(y);
    float fx = x - static_cast<float>(x1);
    float fy = y - static_cast<float>(y1);

    // チャンネルの値を取得するためのヘルパー（AEピクセル構造体は各型で共通のメンバ名を持つ）
    auto get_val = [&](int tx, int ty) -> float {
        T* ptr = reinterpret_cast<T*>(reinterpret_cast<char*>(input->data) + (ty * input->rowbytes) + (tx * sizeof(T)));
        // channel_offset に応じて値を返す (0:blue, 1:green, 2:red, 3:alpha) 
        // ※プラットフォームにより並びが異なるため注意。一般的には以下の順序：
        if (channel_offset == 0) return static_cast<float>(ptr->red);
        if (channel_offset == 1) return static_cast<float>(ptr->green);
        if (channel_offset == 2) return static_cast<float>(ptr->blue);
        return static_cast<float>(ptr->alpha);
        };

    float v11 = get_val(x1, y1);
    float v21 = get_val(x1 + 1, y1);
    float v12 = get_val(x1, y1 + 1);
    float v22 = get_val(x1 + 1, y1 + 1);

    return (1.0f - fx) * (1.0f - fy) * v11 +
        fx * (1.0f - fy) * v21 +
        (1.0f - fx) * fy * v12 +
        fx * fy * v22;
}

template <typename T>
static PF_Err
ChromaticT(
    void* refcon,
    A_long x,
    A_long y,
    T* in_p,
    T* out_p)
{
    const ChromaticInfo* info = reinterpret_cast<const ChromaticInfo*>(refcon);
    const float dx = static_cast<float>(x) - info->center_x;
    const float dy = static_cast<float>(y) - info->center_y;

    // RGB各チャンネルをサンプリング
    float r = SampleChannel<T>(info->input_world, info->center_x + (dx * info->inv_scale_r), info->center_y + (dy * info->inv_scale_r), 0);
    float ra = SampleChannel<T>(info->input_world, info->center_x + (dx * info->inv_scale_r), info->center_y + (dy * info->inv_scale_r), 3);
    float g = SampleChannel<T>(info->input_world, info->center_x + (dx * info->inv_scale_g), info->center_y + (dy * info->inv_scale_g), 1);
    float ga = SampleChannel<T>(info->input_world, info->center_x + (dx * info->inv_scale_g), info->center_y + (dy * info->inv_scale_g), 3);
    float b = SampleChannel<T>(info->input_world, info->center_x + (dx * info->inv_scale_b), info->center_y + (dy * info->inv_scale_b), 2);
    float ba = SampleChannel<T>(info->input_world, info->center_x + (dx * info->inv_scale_b), info->center_y + (dy * info->inv_scale_b), 3);

    float max_v = GetMaxVal<T>();
    r = r * ra / max_v;
    g = g * ga / max_v;
    b = b * ba / max_v;

    // 浮動小数点か整数かで丸め処理を切り替え
    if constexpr (std::is_same_v<T, PF_PixelFloat>) {
        out_p->red = MAX(0.0f, MIN(r, max_v));
        out_p->green = MAX(0.0f, MIN(g, max_v));
        out_p->blue = MAX(0.0f, MIN(b, max_v));
    }
    else {
        // 整数型(8/16bit)の場合は 0.5f を足して四捨五入
        out_p->red = static_cast<decltype(T::red)>(MAX(0.0f, MIN(r + 0.5f, max_v)));
        out_p->green = static_cast<decltype(T::green)>(MAX(0.0f, MIN(g + 0.5f, max_v)));
        out_p->blue = static_cast<decltype(T::blue)>(MAX(0.0f, MIN(b + 0.5f, max_v)));
    }
    float a2 = static_cast<float>(in_p->alpha);
    if(info->alpha_mode == 2) { // alpha_mode が 2 の場合はアルファも変更
        a2 = AE_CLAMP(a2 + (r + g + b) / 3,0, max_v);
    }
    if constexpr (std::is_same_v<T, PF_PixelFloat>) {
        out_p->alpha = a2;
    }
    else {
        out_p->alpha = static_cast<decltype(T::alpha)>(AE_CLAMP(a2 + 0.5f,0, max_v));
    }
    return PF_Err_NONE;
}

PF_Err Chromatic(
    PF_InData* in_dataP,
    PF_EffectWorld* input,
    PF_EffectWorld* output,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_FpLong scale_r,
    PF_FpLong scale_g,
    PF_FpLong scale_b,
    PF_Point center,
    A_long mode
)
{
    PF_Err err = PF_Err_NONE;
    ChromaticInfo info;
    AEFX_CLR_STRUCT(info);

    info.center_x = static_cast<PF_FpShort>(center.x);
    info.center_y = static_cast<PF_FpShort>(center.y);

    PF_FpShort s_r = static_cast<PF_FpShort>(MAX(0.001, scale_r));
    PF_FpShort s_g = static_cast<PF_FpShort>(MAX(0.001, scale_g));
    PF_FpShort s_b = static_cast<PF_FpShort>(MAX(0.001, scale_b));

    info.inv_scale_r = 1.0f / s_r;
    info.inv_scale_g = 1.0f / s_g;
    info.inv_scale_b = 1.0f / s_b;
    info.input_world = input;
	info.alpha_mode = mode;

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB64:
        ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, output->height, input, NULL, &info, ChromaticT<PF_Pixel16>, output));
        break;
    case PF_PixelFormat_ARGB128:
        ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, output->height, input, NULL, &info, ChromaticT<PF_PixelFloat>, output));
        break;
    case PF_PixelFormat_ARGB32:
        ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, output->height, input, NULL, &info, ChromaticT<PF_Pixel>, output));
        break;
    }
    return err;
}