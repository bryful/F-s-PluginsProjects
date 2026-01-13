#include "TargetGradRadical.h"

//-----------------------------------------------------------------------------------
// 2. マスク生成コア（テンプレート）
//-----------------------------------------------------------------------------------
static void AssignGrad(RadialMaskInfo* info, PF_Pixel* out) {
    *out = info->grad8;
}
static void AssignGrad(RadialMaskInfo* info, PF_Pixel16* out) {
    *out = info->grad16;
}
static void AssignGrad(RadialMaskInfo* info, PF_PixelFloat* out) {
    *out = info->grad32;
}
template <typename T, typename T_COMP>
static PF_Err SimpleMaskIteratorF(
    void* refcon,
    A_FpShort x,
    A_FpShort y,
    T* in,
    T* out)
{

    MaskInfo* info = static_cast<MaskInfo*>(refcon);
    if ((out->alpha == 0) && (info->isAll == FALSE)) {
        return PF_Err_NONE;
    }

    // 1. 投影比率 t の計算
    float px = static_cast<float>(x) - static_cast<float>(info->p1.x);
    float py = static_cast<float>(y) - static_cast<float>(info->p1.y);
    float t = (px * info->dx + py * info->dy) * info->inv_len_sq;

    // 2. 0.0 - 1.0 にクランプ
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;

    // 3. 双曲線（Hyperbolic）階調変換の適用
    // info->hyperbolic が 0.0 の時はリニア、正の値で急峻なカーブになります
    float k = info->hyperbolic;
    float v = ((1.0f + k) * t) / (1.0f + k * t);

    // 4. 反転設定
    // !info->invert の条件は既存コードの仕様を維持
    if (!info->invert) v = 1.0f - v;

    // 5. 各深度の最大値を取得 (8bit:255, 16bit:32768, 32bit:1.0)
    // T_COMP が無符号整数の場合があるため、オーバーフローに注意した既存ロジックを維持
    float max_val = 1.0f;
    if (sizeof(T_COMP) == 1)      max_val = 255.0f;
    else if (sizeof(T_COMP) == 2) max_val = 32768.0f;
    else                          max_val = 1.0f;

    // 最終的なアルファ値を計算
    T_COMP final_pixel_val = static_cast<T_COMP>(v * max_val);

    // 6. 出力への書き込み
    AssignGrad(info, out); // 色情報をコピー
    out->alpha = final_pixel_val; // アルファにグラデーション値を代入

    return PF_Err_NONE;
}

template <typename T, typename T_COMP>
static PF_Err TargetGradRadialIterator(
    void* refcon,
    A_long x,
    A_long y,
    T* in,
    T* out)
{
    RadialMaskInfo* info = static_cast<RadialMaskInfo*>(refcon);

    if ((out->alpha == 0) && (info->isAll == FALSE)) {
        return PF_Err_NONE;
    }

    // --- mask_val 計算 ---
    float dx = (float)x - (float)info->center_p.x;
    float dy = (float)y - (float)info->center_p.y;
    float rx = dx * info->cos_q - dy * info->sin_q;
    float ry = dx * info->sin_q + dy * info->cos_q;
    rx *= info->inv_aspect;
    float dist = sqrtf(rx * rx + ry * ry);
    float t = dist / info->radius;

    // クランプ (0.0 - 1.0)
    t = fminf(fmaxf(t, 0.0f), 1.0f);
    if (info->invert) t = 1.0f - t;

    float mask_val = 0.0f;

    // --- 双曲線（Hyperbolic）階調変換の追加 ---
    // info->k は曲がり具合を制御する係数 (例: 0.0ならリニア, 5.0なら急峻なカーブ)
    // 式: v = ((1 + k) * t) / (1 + k * t)
    float k = info->hyperbolic;
    float t_hyper = ((1.0f + k) * t) / (1.0f + k * t);

    // 既存のフェザー(Smoothstep)計算と組み合わせる、または置き換える
    // ここでは指定されたtを双曲線変換した後にエッジ処理を行っています
    float feather_start = fmaxf(0.0f, 1.0f - info->feather);

    if (t_hyper <= feather_start) {
        mask_val = 0.0f;
    }
    else if (t_hyper >= 1.0f) {
        mask_val = 1.0f;
    }
    else {
        float v = (t_hyper - feather_start) / (1.0f - feather_start);
        mask_val = v * v * (3.0f - 2.0f * v);
    }
    if (info->invert) mask_val = 1.0f - mask_val;

    if (!info->invert) mask_val = 1.0f - mask_val;
    // --- end mask_val ---

    // 安全にターゲット型へ変換して代入
    float max_val_f = 1.0f;
    if (std::is_same<T, PF_Pixel>::value) {
        max_val_f = 255.0f;
    }
    else if (std::is_same<T, PF_Pixel16>::value) {
        max_val_f = 32768.0f;
    }
    else {
        max_val_f = 1.0f;
    }

    T_COMP vcomp;
    if (std::is_same<T, PF_PixelFloat>::value) {
        vcomp = static_cast<T_COMP>(fminf(fmaxf(mask_val, 0.0f), 1.0f));
    }
    else {
        float tmp = fminf(fmaxf(mask_val * max_val_f, 0.0f), max_val_f);
        vcomp = static_cast<T_COMP>(tmp + 0.5f);
    }

    // 出力へ書き込み
    AssignGrad(info, out);
    out->alpha = vcomp;

    return PF_Err_NONE;
}

static PF_Err RenderTargetGradRadialImpl(
    CFsAE* ae,
    ParamInfo* infoP,
    PF_EffectWorld* output
    )
{
    PF_Err err = PF_Err_NONE;

    // 1. ループ外での事前計算
    RadialMaskInfo info;
    info.center_p = infoP->cenertPos;
    float ds_x = (float)ae->in_data->downsample_x.num / (float)ae->in_data->downsample_x.den;

    info.radius = max(1.0f, ((float)infoP->radius* ds_x));
    info.inv_aspect = 1.0f / max(0.01f, (float)infoP->aspect);
    info.invert = infoP->invert;
    info.feather = max(0.0f, min(1.0f, (float)infoP->feather));
	info.isAll = infoP->targetColorMode == 3 ? TRUE : FALSE;
    // 角度をラジアンに変換 (AEの角度は時計回りなので逆回転させる)
    float rad = infoP->angle * 0.01745329f;
    info.cos_q = cosf(-rad);
    info.sin_q = sinf(-rad);

    info.grad8 = infoP->gradColor;
    info.grad16 = CONV8TO16(infoP->gradColor);
    info.grad32 = CONV8TO32(infoP->gradColor);

	info.hyperbolic = infoP->hyperbolic; // 双曲線の曲がり具合の係数（必要に応じて調整可能）
    if (!err) {
        PF_Point			origin;
        origin.h = (A_short)ae->in_data->output_origin_x;
        origin.v = (A_short)ae->in_data->output_origin_y;

        switch (ae->pixelFormat()) {
        case PF_PixelFormat_ARGB32:
            ERR(ae->suitesP->Iterate8Suite2()->iterate_origin(
                ae->in_data,
                0,
                output->height,
                ae->input,
                &output->extent_hint,
                &origin,
                &info,
                reinterpret_cast<PF_Err(*)(void*, A_long, A_long, PF_Pixel*, PF_Pixel*)>(TargetGradRadialIterator<PF_Pixel, A_u_char>),
                output));
            /*
            err = ae->suitesP->Iterate8Suite1()->iterate(
                ae->in_data,
                0,
                output->height,
                NULL,
                NULL,
                &info,
                reinterpret_cast<PF_Err(*)(void*, A_long, A_long, PF_Pixel*, PF_Pixel*)>(TargetGradRadialIterator<PF_Pixel, A_u_char>),
                output);*/
            break;

        case PF_PixelFormat_ARGB64:
            ERR(ae->suitesP->Iterate16Suite1()->iterate_origin(
                ae->in_data,
                0,
                output->height,
                ae->input,
                &output->extent_hint,
                &origin,
                &info,
                reinterpret_cast<PF_Err(*)(void*, A_long, A_long, PF_Pixel16*, PF_Pixel16*)>(TargetGradRadialIterator<PF_Pixel16, A_u_short>),
                output));
            /*
            err = ae->suitesP->Iterate16Suite1()->iterate(
                ae->in_data,
                0,
                output->height,
                NULL,
                NULL,
                &info,
                reinterpret_cast<PF_Err(*)(void*, A_long, A_long, PF_Pixel16*, PF_Pixel16*)>(TargetGradRadialIterator<PF_Pixel16, A_u_short>),
                output);*/
            break;

        case PF_PixelFormat_ARGB128:
            ERR(ae->suitesP->IterateFloatSuite1()->iterate_origin(
                ae->in_data,
                0,
                output->height,
                ae->input,
                &output->extent_hint,
                &origin,
                &info,
                reinterpret_cast<PF_Err(*)(void*, A_long, A_long, PF_PixelFloat*, PF_PixelFloat*)>(TargetGradRadialIterator<PF_PixelFloat, PF_FpShort>),
                output));
            /*
            err = ae->suitesP->IterateFloatSuite1()->iterate(
                ae->in_data,
                0,
                output->height,
                NULL,
                NULL,
                &info,
                reinterpret_cast<PF_Err(*)(void*, A_long, A_long, PF_PixelFloat*, PF_PixelFloat*)>(TargetGradRadialIterator<PF_PixelFloat, PF_FpShort>),
                output);
            */
            break;

        default:
            err = PF_Err_BAD_CALLBACK_PARAM;
            break;
        }
    }

    return err;
}

// 公開シンボル：PF_EffectWorld* を受け取る既定の宣言
PF_Err RenderTargetGradRadial(
    CFsAE* ae,
    ParamInfo* infoP,
    PF_EffectWorld* output)
{
    return RenderTargetGradRadialImpl(ae, infoP, output);
}
/*
// 既存の呼び出しが PF_LayerDef* を使っている翻訳単位向けの互換ラッパー
PF_Err RenderTargetGradRadial(
    CFsAE* ae,
    ParamInfo* infoP,
    PF_LayerDef* output)
{
    return RenderTargetGradRadialImpl(ae, infoP, reinterpret_cast<PF_EffectWorld*>(output));
}
*/