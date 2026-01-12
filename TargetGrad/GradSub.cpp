#include "TargetGrad.h"

//-----------------------------------------------------------------------------------
// 2. マスク生成コア（テンプレート）
//-----------------------------------------------------------------------------------
static void AssignGrad(MaskInfo* info, PF_Pixel* out) {
    *out = info->grad8;
}
static void AssignGrad(MaskInfo* info, PF_Pixel16* out) {
    *out = info->grad16;
}
static void AssignGrad(MaskInfo* info, PF_PixelFloat* out) {
    *out = info->grad32;
}
template <typename T, typename T_COMP>
static PF_Err SimpleMaskIteratorF(
    void* refcon,
    A_FpShort x,
    A_FpShort y,
    T* in,
    T* out);



static PF_Err SimpleMaskIterator8(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel8* in,
    PF_Pixel8* out)
{
    // AEのA_long→A_FpShort変換
    return SimpleMaskIteratorF<PF_Pixel8, A_u_char>(refcon, static_cast<A_FpShort>(x), static_cast<A_FpShort>(y), in, out);
}

static PF_Err SimpleMaskIterator16(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel16* in,
    PF_Pixel16* out)
{
    return SimpleMaskIteratorF<PF_Pixel16, A_u_short>(refcon, static_cast<A_FpShort>(x), static_cast<A_FpShort>(y), in, out);
}

static PF_Err SimpleMaskIterator32(
    void* refcon,
    A_long x,
    A_long y,
    PF_PixelFloat* in,
    PF_PixelFloat* out)
{
    return SimpleMaskIteratorF<PF_PixelFloat, float>(refcon, static_cast<A_FpShort>(x), static_cast<A_FpShort>(y), in, out);
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
    //if (!info->invert) 
        v = 1.0f - v;

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

//-----------------------------------------------------------------------------------
// 3. メイン関数（実処理はImplに集約）
//-----------------------------------------------------------------------------------

// 実処理本体（内部用）
static PF_Err RenderSimpleMaskImpl(
    CFsAE* ae,
	ParamInfo* infoP,
    PF_EffectWorld* output)
{
    PF_Err err = PF_Err_NONE;

    MaskInfo info;
    info.p1 = infoP->startPos2D;
    info.dx = (A_FpShort)(infoP->lastPos2D.x - infoP->startPos2D.x);
    info.dy = (A_FpShort)(infoP->lastPos2D.y - infoP->startPos2D.y);
 
    //info.invert = infoP->invert;
    info.hyperbolic = infoP->hyperbolic; // 追加：UIからの係数をセット
    info.grad8 = infoP->gradColor;
    info.grad16 = CONV8TO16( infoP->gradColor);
    info.grad32 = CONV8TO32(infoP->gradColor);

    float len_sq = (info.dx * info.dx) + (info.dy * info.dy);
    info.inv_len_sq = (len_sq < 0.0001f) ? 1.0f : (1.0f / len_sq);

	info.isAll = (infoP->targetColorMode == 3) ? TRUE : FALSE;

    if (!err) {
        switch (ae->pixelFormat()) {
        case PF_PixelFormat_ARGB32:
            err = ae->suitesP->Iterate8Suite1()->iterate(
                ae->in_data, 0, output->height, NULL, NULL, &info,
                SimpleMaskIterator8, output);
            break;
        case PF_PixelFormat_ARGB64:
            err = ae->suitesP->Iterate16Suite1()->iterate(
                ae->in_data, 0, output->height, NULL, NULL, &info,
                SimpleMaskIterator16, output);
            break;
        case PF_PixelFormat_ARGB128:
            err = ae->suitesP->IterateFloatSuite1()->iterate(
                ae->in_data, 0, output->height, NULL, NULL, &info,
                SimpleMaskIterator32, output);
            break;
        default:
            err = PF_Err_BAD_CALLBACK_PARAM;
            break;
        }
    }
    return err;
}

// 公開シンボル：PF_EffectWorld* を受け取る既定の宣言に合わせる
PF_Err RenderSimpleMask(
    CFsAE* ae,
    ParamInfo* infoP,
    PF_EffectWorld* output)
{
    return RenderSimpleMaskImpl(ae, infoP,output);
}
/*
// 互換用ラッパー：別の翻訳単位がPF_LayerDef* や char を期待してビルドされている場合に備える
// （PF_LayerDef と PF_EffectWorld が同等であれば reinterpret_cast で橋渡し）
PF_Err RenderSimpleMask(
    CFsAE* ae,
    PF_LayerDef* output, // 別 TU がこの型でコンパイルしている可能性があるため用意
    A_FloatPoint p1,
    A_FloatPoint p2,
    char invert) // 一部 TU では char でコンパイルされる場合があるため用意
{
    return RenderSimpleMaskImpl(ae, reinterpret_cast<PF_EffectWorld*>(output), p1, p2, static_cast<PF_Boolean>(invert));
}
*/