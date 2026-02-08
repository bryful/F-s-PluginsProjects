#include "Emboss.h"
// 内部計算用スレッド共有データ
typedef struct {
    PF_EffectWorld* input_world;
    float           dx;
    float           dy;
    A_long          width;
    A_long          height;
} EmbossRenderPayload;
typedef struct {
    PF_EffectWorld* input_world;
    PF_EffectWorld* output_world;
    float           dx;
    float           dy;
} EmbossGenericPayload;
// 8bit用クランプ関数 (高速化のためマクロまたはインライン)
inline A_u_char CLAMP8(int val) {
    return (A_u_char)(val < 0 ? 0 : (val > 255 ? 255 : val));
}

// 高速バイリニアサンプリング
inline void SampleBilinear8(PF_EffectWorld* world, float x, float y, int* r, int* g, int* b) {
    // 範囲チェック（簡易）
    if (x < 0) x = 0; if (y < 0) y = 0;
    if (x > world->width - 2) x = (float)world->width - 2;
    if (y > world->height - 2) y = (float)world->height - 2;

    int xi = (int)x;
    int yi = (int)y;
    float xf = x - xi;
    float yf = y - yi;

    // ポインタ演算によるピクセル取得
    PF_Pixel* p00 = (PF_Pixel*)((char*)world->data + (yi * world->rowbytes) + (xi * sizeof(PF_Pixel)));
    PF_Pixel* p01 = p00 + 1;
    PF_Pixel* p10 = (PF_Pixel*)((char*)p00 + world->rowbytes);
    PF_Pixel* p11 = p10 + 1;

    // 重み付け計算
    float w00 = (1.0f - xf) * (1.0f - yf);
    float w01 = xf * (1.0f - yf);
    float w10 = (1.0f - xf) * yf;
    float w11 = xf * yf;

    *r = (int)(p00->red * w00 + p01->red * w01 + p10->red * w10 + p11->red * w11);
    *g = (int)(p00->green * w00 + p01->green * w01 + p10->green * w10 + p11->green * w11);
    *b = (int)(p00->blue * w00 + p01->blue * w01 + p10->blue * w10 + p11->blue * w11);
}
static PF_Err
EmbossGenericKernel(
    void* refcon,
    A_long      thread_index,
    A_long      iter_index,     // 現在の行 (y)
    A_long      iter_total)     // 総行数
{
    EmbossGenericPayload* pld = (EmbossGenericPayload*)refcon;
    A_long y = iter_index;

    // 行の先頭ポインタを取得
    PF_Pixel* out_ptr = (PF_Pixel*)((char*)pld->output_world->data + (y * pld->output_world->rowbytes));
    PF_Pixel* in_ptr = (PF_Pixel*)((char*)pld->input_world->data + (y * pld->input_world->rowbytes));

    for (A_long x = 0; x < pld->output_world->width; ++x) {
        int r1, g1, b1, r2, g2, b2;

        // サンプリング座標を計算
        float sx1 = (float)x + pld->dx;
        float sy1 = (float)y + pld->dy;
        float sx2 = (float)x - pld->dx;
        float sy2 = (float)y - pld->dy;

        // 高速バイリニアサンプリング (前述の inline 関数)
        SampleBilinear8(pld->input_world, sx1, sy1, &r1, &g1, &b1);
        SampleBilinear8(pld->input_world, sx2, sy2, &r2, &g2, &b2);

        // エンボス演算とポインタ書き込み
        out_ptr->red = CLAMP8((r1 - r2) + 128);
        out_ptr->green = CLAMP8((g1 - g2) + 128);
        out_ptr->blue = CLAMP8((b1 - b2) + 128);
        out_ptr->alpha = in_ptr->alpha;

        // ポインタを次のピクセルへ進める (ここが最速のポイント)
        out_ptr++;
        in_ptr++;
    }

    return PF_Err_NONE;
}
// PF_Iterateから呼ばれるコールバック
static PF_Err EmbossKernel8(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel* in,
    PF_Pixel* out)
{
    EmbossRenderPayload* pld = (EmbossRenderPayload*)refcon;

    int r1, g1, b1, r2, g2, b2;

    // 正負両方向をサンプリング
    SampleBilinear8(pld->input_world, (float)x + pld->dx, (float)y + pld->dy, &r1, &g1, &b1);
    SampleBilinear8(pld->input_world, (float)x - pld->dx, (float)y - pld->dy, &r2, &g2, &b2);

    // エンボス演算: (差分) + 中間グレー
    out->red = CLAMP8((r1 - r2) + 128);
    out->green = CLAMP8((g1 - g2) + 128);
    out->blue = CLAMP8((b1 - b2) + 128);
    out->alpha = in->alpha;

    return PF_Err_NONE;
}

PF_Err EmbossFilter(
	NF_AE* ae,
	ParamInfo* infoP
    )
{
    PF_Err err = PF_Err_NONE;

    PF_FpLong angle = infoP->direction - 90.0f; // 固定小数点をfloatに
    PF_FpLong dist = infoP->distance;

    // ラジアン変換とオフセットベクトルの計算
    // ※ 1回だけ計算してスレッド間で共有
    float rad = (float)(angle * M_PI / 180.0);


    EmbossGenericPayload pld;
    pld.input_world = ae->input;
    pld.output_world = ae->output;
    pld.dx = (float)(cosf(rad) * dist);
    pld.dy = (float)(sinf(rad) * dist);

    switch (ae->pixelFormat())
    {
        case PF_PixelFormat_ARGB128:
            // 32bit版は未実装
			break;
        break;
        case PF_PixelFormat_ARGB64:
			// 16bit版は未実装
            break;
        case PF_PixelFormat_ARGB32:
            // 8bit版
            err = ae->suitesP->Iterate8Suite2()->iterate_generic(
                ae->input->height,
                &pld,
                EmbossGenericKernel
            );
            return err;
			break;
    }

    return err;
}
