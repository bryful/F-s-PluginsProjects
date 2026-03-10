#include "RandomMosaic.h"
// 内部計算用スレッド共有データ

template <typename T>
struct CEmbossInfo{
	std::vector<std::vector<T>> *buffer;
    PF_EffectWorld* world;
    float           dx;
    PF_Boolean	   isColor;
    PF_Boolean	   isAlpha;
};

template <typename T>
inline void SampleBilinear2(
    std::vector<std::vector<T>>* buf,
    float x, int y,
    float* r, float* g, float* b, float* a) {
    float wf = (float)buf->at(0).size(); // buf[0] → buf->at(0) に修正
    float hf = (float)buf->size();

    x = AE_CLAMP(x, 0.0f, wf - 1.001f);
    y = AE_CLAMP(y, 0, (A_long)hf - 1);

    int xi = (int)x;
    int xi2 = AE_CLAMP(xi + 1, 0, (A_long)wf - 1); // wf-1 に修正
    float v1 = x - (float)xi;
    float v0 = 1.0f - v1;

    T* p0 = &buf->at(y).at(xi);
    T* p1 = &buf->at(y).at(xi2);
    *r = p0->red   * v0 + p1->red   * v1;
    *g = p0->green * v0 + p1->green * v1;
    *b = p0->blue  * v0 + p1->blue  * v1;
    *a = p0->alpha * v0 + p1->alpha * v1;
}

// --- 3. テンプレート化されたカーネル ---
template <typename T, typename CT, CT max_chan, CT half_chan>
static PF_Err ColorEmbossHorT(
    void* refcon,
    A_long t_idx,
    A_long y,
    A_long t_total)
{
    CEmbossInfo<T>* infoP = (CEmbossInfo<T>*)refcon;

    T* outP = (T*)((char*)infoP->world->data + (y * infoP->world->rowbytes));

    for (A_long x = 0; x < infoP->world->width; ++x)
    {
        infoP->buffer->at(y).at(x) = outP[x];
    }
    for (A_long x = 0; x < infoP->world->width; ++x) {
        float r1, g1, b1, a1, r2, g2, b2, a2;

        SampleBilinear2<T>(infoP->buffer, (float)x - infoP->dx, y, &r1, &g1, &b1, &a1);
        SampleBilinear2<T>(infoP->buffer, (float)x + infoP->dx, y, &r2, &g2, &b2, &a2);

        if (!infoP->isColor) {
            // 明暗共通の式を使用（if/else if を廃止）
            float v0 = ((r1 + g1 + b1) - (r2 + g2 + b2)) / 3.0f;
            outP[x].red   = (CT)AE_CLAMP((float)outP[x].red   + v0 + outP[x].red   * v0 / (float)max_chan, 0, max_chan);
            outP[x].green = (CT)AE_CLAMP((float)outP[x].green + v0 + outP[x].green * v0 / (float)max_chan, 0, max_chan);
            outP[x].blue  = (CT)AE_CLAMP((float)outP[x].blue  + v0 + outP[x].blue  * v0 / (float)max_chan, 0, max_chan);
        }
        else {
            // 各チャンネル独立、明暗共通の式を使用
            float r0 = r1 - r2;
            outP[x].red   = (CT)AE_CLAMP((float)outP[x].red   + r0 + outP[x].red   * r0 / (float)max_chan, 0, max_chan);
            float g0 = g1 - g2;
            outP[x].green = (CT)AE_CLAMP((float)outP[x].green + g0 + outP[x].green * g0 / (float)max_chan, 0, max_chan);
            float b0 = b1 - b2;
            outP[x].blue  = (CT)AE_CLAMP((float)outP[x].blue  + b0 + outP[x].blue  * b0 / (float)max_chan, 0, max_chan);
        }
        if (infoP->isAlpha) {
            float a0 = ABS(a1 - a2);
            outP[x].alpha = (CT)AE_CLAMP((float)outP[x].alpha + a0 + outP[x].alpha * a0 / (float)max_chan, 0, max_chan);
        }
    }
    return PF_Err_NONE;
}

// --- 4. メインのエントリポイント ---
PF_Err ColorEmbossHorMain(
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_EffectWorld* world,
    float distance,
	PF_Boolean isColor,
	PF_Boolean isAlpha
    )
{
    if (distance == 0) {
        return PF_Err_NONE;
    }
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
    {
        CEmbossInfo<PF_Pixel> info;
        info.world = world;
        info.dx = distance;
        std::vector<std::vector<PF_Pixel>> buffer(world->height, std::vector<PF_Pixel>(world->width, { 0,0,0,0 }));
        info.buffer = &buffer;
		info.isColor = isColor;
		info.isAlpha = isAlpha;
        // PF_Pixel8 → PF_Pixel に修正
        return suitesP->Iterate8Suite2()->iterate_generic(world->height, &info, ColorEmbossHorT<PF_Pixel, A_u_char, PF_MAX_CHAN8, PF_HALF_CHAN8>);
    }
    case PF_PixelFormat_ARGB64:
    {
        CEmbossInfo<PF_Pixel16> info;
        info.world = world;
        info.dx = distance;
        std::vector<std::vector<PF_Pixel16>> buffer(world->height, std::vector<PF_Pixel16>(world->width, { 0,0,0,0 }));
        info.buffer = &buffer;
        info.isColor = isColor;
        info.isAlpha = isAlpha;
        // PF_Pixel8 → PF_Pixel16、A_u_char → A_u_short、MAX_CHAN8 → MAX_CHAN16 に修正
        return suitesP->Iterate8Suite2()->iterate_generic(world->height, &info, ColorEmbossHorT<PF_Pixel16, A_u_short, PF_MAX_CHAN16, PF_HALF_CHAN16>);
    }
    case PF_PixelFormat_ARGB128:
    {
        CEmbossInfo<PF_PixelFloat> info;
        info.world = world;
        info.dx = distance;
        std::vector<std::vector<PF_PixelFloat>> buffer(world->height, std::vector<PF_PixelFloat>(world->width, { 0,0,0,0 }));
        info.buffer = &buffer;
        info.isColor = isColor;
        info.isAlpha = isAlpha;
        // PF_Pixel8 → PF_PixelFloat、float 型定数に修正
        return suitesP->Iterate8Suite2()->iterate_generic(world->height, &info, ColorEmbossHorT<PF_PixelFloat, PF_FpShort, 1.0f, 0.5f>);
    }
    }
    return PF_Err_BAD_CALLBACK_PARAM;
}