#include "NF_DrawAALine.h"

// マクロ競合を避けるため、std::min/max を (std::min) と記述します。

template <typename T, typename T_COMP, int MAX_VAL>
void DrawAALineT(
    PF_EffectWorld* output,
    float x0, float y0, float x1, float y1,
    T col,
    float startAlpha, float endAlpha,
    float startWeight, float endWeight)
{
    float maxW = (std::max)(startWeight, endWeight) + 2.0f;
    int minX = (std::max)(0, (int)((std::min)(x0, x1) - maxW));
    int maxX = (std::min)((int)output->width - 1, (int)((std::max)(x0, x1) + maxW));
    int minY = (std::max)(0, (int)((std::min)(y0, y1) - maxW));
    int maxY = (std::min)((int)output->height - 1, (int)((std::max)(y0, y1) + maxW));

    float dx = x1 - x0;
    float dy = y1 - y0;
    float l2 = dx * dx + dy * dy;
    float inv_l2 = (l2 == 0) ? 0 : 1.0f / l2;

    if (maxX < minX || maxY < minY) return;

    for (int y = minY; y <= maxY; y++) {
        T* dstP = (T*)((char*)output->data + (y * output->rowbytes) + (minX * sizeof(T)));
        float py0 = (float)y - y0;

        for (int x = minX; x <= maxX; x++, dstP++) {
            float px0 = (float)x - x0;
            float t = (std::max)(0.0f, (std::min)(1.0f, (px0 * dx + py0 * dy) * inv_l2));

            float projX = x0 + t * dx;
            float projY = y0 + t * dy;
            float distSq = ((float)x - projX) * ((float)x - projX) + ((float)y - projY) * ((float)y - projY);

            float curW = startWeight + (endWeight - startWeight) * t;
            float edge0 = curW * 0.5f + 0.5f;
            float edge1 = (std::max)(0.0f, curW * 0.5f - 0.5f);

            if (distSq > edge0 * edge0) continue;

            float aa = (distSq < edge1 * edge1) ? 1.0f : (std::max)(0.0f, (std::min)(1.0f, (edge0 - (float)std::sqrt(distSq)) / (edge0 - edge1)));

            float curAlpha = startAlpha + (endAlpha - startAlpha) * t;
            float src_a = (col.alpha / (float)MAX_VAL) * curAlpha * aa;

            if (src_a <= 0.0f) continue;

            dstP->red = (T_COMP)((std::min)((float)MAX_VAL, dstP->red * (1.0f - src_a) + col.red * src_a));
            dstP->green = (T_COMP)((std::min)((float)MAX_VAL, dstP->green * (1.0f - src_a) + col.green * src_a));
            dstP->blue = (T_COMP)((std::min)((float)MAX_VAL, dstP->blue * (1.0f - src_a) + col.blue * src_a));
            dstP->alpha = (T_COMP)((std::max)((float)dstP->alpha, col.alpha * curAlpha * aa));
        }
    }
}

// --- Iterate Suite 用 構造体 ---
template <typename T>
struct LineRefData {
    float x0, y0, x1, y1;
    T col;
    float startAlpha, endAlpha;
    float startWeight, endWeight;
    float dx, dy, l2, inv_l2;
    float max_val;
};

template <typename T, typename T_COMP>
static PF_Err LineWorkerT(void* refcon, A_long x, A_long y, T* in, T* out) {
    LineRefData<T>* data = (LineRefData<T>*)refcon;

    float t = (std::max)(0.0f, (std::min)(1.0f, (((float)x - data->x0) * data->dx + ((float)y - data->y0) * data->dy) * data->inv_l2));

    float projX = data->x0 + t * data->dx;
    float projY = data->y0 + t * data->dy;
    float distSq = ((float)x - projX) * ((float)x - projX) + ((float)y - projY) * ((float)y - projY);

    float curW = data->startWeight + (data->endWeight - data->startWeight) * t;
    float edge0 = curW * 0.5f + 0.5f;
    if (distSq > edge0 * edge0) return PF_Err_NONE;

    float edge1 = (std::max)(0.0f, curW * 0.5f - 0.5f);
    float aa = (distSq < edge1 * edge1) ? 1.0f : (std::max)(0.0f, (std::min)(1.0f, (edge0 - (float)std::sqrt(distSq)) / (edge0 - edge1)));

    float curAlpha = data->startAlpha + (data->endAlpha - data->startAlpha) * t;
    float src_a = (data->col.alpha / data->max_val) * curAlpha * aa;

    out->red = (T_COMP)((std::min)(data->max_val, out->red * (1.0f - src_a) + data->col.red * src_a));
    out->green = (T_COMP)((std::min)(data->max_val, out->green * (1.0f - src_a) + data->col.green * src_a));
    out->blue = (T_COMP)((std::min)(data->max_val, out->blue * (1.0f - src_a) + data->col.blue * src_a));
    out->alpha = (T_COMP)((std::max)((float)out->alpha, data->col.alpha * curAlpha * aa));

    return PF_Err_NONE;
}

// ---------------------------------------------------------------------------------------
// ラッパー関数群
// ---------------------------------------------------------------------------------------

PF_Err LineWorker8(void* ref, A_long x, A_long y, PF_Pixel* i, PF_Pixel* o) { return LineWorkerT<PF_Pixel, A_u_char>(ref, x, y, i, o); }
PF_Err LineWorker16(void* ref, A_long x, A_long y, PF_Pixel16* i, PF_Pixel16* o) { return LineWorkerT<PF_Pixel16, A_u_short>(ref, x, y, i, o); }
PF_Err LineWorker32(void* ref, A_long x, A_long y, PF_PixelFloat* i, PF_PixelFloat* o) { return LineWorkerT<PF_PixelFloat, float>(ref, x, y, i, o); }

PF_Err DrawAA_Line8(PF_InData* in_data, PF_EffectWorld* output, AEGP_SuiteHandler* suites, float x0, float y0, float x1, float y1, PF_Pixel col, float sA, float eA, float sW, float eW) {
    LineRefData<PF_Pixel> d = { x0, y0, x1, y1, col, sA, eA, sW, eW, x1 - x0, y1 - y0 };
    d.l2 = d.dx * d.dx + d.dy * d.dy; d.inv_l2 = (d.l2 == 0) ? 0 : 1.0f / d.l2; d.max_val = 255.0f;
    float maxW = (std::max)(sW, eW) + 2.0f;
    PF_Rect r = { (short)(std::max)(0.0f, (std::min)(x0,x1) - maxW), (short)(std::max)(0.0f, (std::min)(y0,y1) - maxW), (short)(std::min)((float)output->width, (std::max)(x0,x1) + maxW), (short)(std::min)((float)output->height, (std::max)(y0,y1) + maxW) };
    return suites->Iterate8Suite2()->iterate(in_data, 0, r.bottom - r.top, NULL, &r, &d, LineWorker8, output);
}

PF_Err DrawAA_Line16(PF_InData* in_data, PF_EffectWorld* output, AEGP_SuiteHandler* suites, float x0, float y0, float x1, float y1, PF_Pixel16 col, float sA, float eA, float sW, float eW) {
    LineRefData<PF_Pixel16> d = { x0, y0, x1, y1, col, sA, eA, sW, eW, x1 - x0, y1 - y0 };
    d.l2 = d.dx * d.dx + d.dy * d.dy; d.inv_l2 = (d.l2 == 0) ? 0 : 1.0f / d.l2; d.max_val = 32768.0f;
    float maxW = (std::max)(sW, eW) + 2.0f;
    PF_Rect r = { (short)(std::max)(0.0f, (std::min)(x0,x1) - maxW), (short)(std::max)(0.0f, (std::min)(y0,y1) - maxW), (short)(std::min)((float)output->width, (std::max)(x0,x1) + maxW), (short)(std::min)((float)output->height, (std::max)(y0,y1) + maxW) };
    return suites->Iterate16Suite2()->iterate(in_data, 0, r.bottom - r.top, NULL, &r, &d, LineWorker16, output);
}

PF_Err DrawAA_Line32(PF_InData* in_data, PF_EffectWorld* output, AEGP_SuiteHandler* suites, float x0, float y0, float x1, float y1, PF_PixelFloat col, float sA, float eA, float sW, float eW) {
    LineRefData<PF_PixelFloat> d = { x0, y0, x1, y1, col, sA, eA, sW, eW, x1 - x0, y1 - y0 };
    d.l2 = d.dx * d.dx + d.dy * d.dy; d.inv_l2 = (d.l2 == 0) ? 0 : 1.0f / d.l2; d.max_val = 1.0f;
    float maxW = (std::max)(sW, eW) + 2.0f;
    PF_Rect r = { (short)(std::max)(0.0f, (std::min)(x0,x1) - maxW), (short)(std::max)(0.0f, (std::min)(y0,y1) - maxW), (short)(std::min)((float)output->width, (std::max)(x0,x1) + maxW), (short)(std::min)((float)output->height, (std::max)(y0,y1) + maxW) };
    return suites->IterateFloatSuite2()->iterate(in_data, 0, r.bottom - r.top, NULL, &r, &d, LineWorker32, output);
}

PF_Err DrawAA_Line(PF_InData* in_data, PF_EffectWorld* out, PF_PixelFormat format, AEGP_SuiteHandler* suites, float x0, float y0, float x1, float y1, PF_Pixel col, float sA, float eA, float sW, float eW) {
    switch (format) {
    case PF_PixelFormat_ARGB128: return DrawAA_Line32(in_data, out, suites, x0, y0, x1, y1, NF_Pixel8To32(col), sA, eA, sW, eW);
    case PF_PixelFormat_ARGB64:  return DrawAA_Line16(in_data, out, suites, x0, y0, x1, y1, NF_Pixel8To16(col), sA, eA, sW, eW);
    case PF_PixelFormat_ARGB32:  return DrawAA_Line8(in_data, out, suites, x0, y0, x1, y1, col, sA, eA, sW, eW);
    default: return PF_Err_UNRECOGNIZED_PARAM_TYPE;
    }
}

void DrawAALine8(PF_EffectWorld* o, float x0, float y0, float x1, float y1, PF_Pixel c, float sA, float eA, float sW, float eW) { DrawAALineT<PF_Pixel, A_u_char, 255>(o, x0, y0, x1, y1, c, sA, eA, sW, eW); }
void DrawAALine16(PF_EffectWorld* o, float x0, float y0, float x1, float y1, PF_Pixel16 c, float sA, float eA, float sW, float eW) { DrawAALineT<PF_Pixel16, A_u_short, 32768>(o, x0, y0, x1, y1, c, sA, eA, sW, eW); }
void DrawAALine32(PF_EffectWorld* o, float x0, float y0, float x1, float y1, PF_PixelFloat c, float sA, float eA, float sW, float eW) { DrawAALineT<PF_PixelFloat, float, 1>(o, x0, y0, x1, y1, c, sA, eA, sW, eW); }