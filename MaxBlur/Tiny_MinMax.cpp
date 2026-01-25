#include "Tiny_MinMax.h"

typedef struct LineBuf {
    A_FpShort	level;
    A_long		index;
} LineBufInfo;

typedef struct MinMaxInfo {

	PF_InData* in_data;
    A_long			max;
    PF_Boolean		maxMinus;
	A_long          width;
	A_long		    widthTrue;
	A_long          rowbytes;
    A_long          height;
	PF_EffectWorld  *world;
    //PF_PixelPtr     line;
	//LineBuf*        forward;
    //LineBuf*        backward;

}MaxFastInfo;


// ************************************************************************
template <typename PixelType, typename ChannelType, typename FloatType>
static PF_Err
Max_SubH(
    void* refconPV,
    A_long thread_idxL,
    A_long y,
    A_long itrtL,
    FloatType maxChan)
{
    PF_Err err = PF_Err_NONE;
    MinMaxInfo* infoP = static_cast<MinMaxInfo*>(refconPV);
    int radius = infoP->max;
    if (radius <= 0) return err;

    PF_InData* in_data = infoP->in_data;
    A_long w = infoP->width;
    PixelType* outP = (PixelType*)infoP->world->data + (y * infoP->widthTrue);

    // 1. 半径が画像幅以上の場合は全画面最大値を計算
    if (radius >= w) {
        PixelType maax = outP[0];
        FloatType maaxV = (((FloatType)maax.red * 0.299) + ((FloatType)maax.green * 0.587) + ((FloatType)maax.blue * 0.114))*10 + (FloatType)maax.alpha /maxChan;

        for (A_long x = 1; x < w; x++) {
            FloatType v = (((FloatType)outP[x].red * 0.299) + ((FloatType)outP[x].green * 0.587) + ((FloatType)outP[x].blue * 0.114)) + (FloatType)outP[x].alpha/maxChan;
            if (v > maaxV) { maax = outP[x]; maaxV = v; }
        }
        for (A_long x = 0; x < w; x++) outP[x] = maax;
        return PF_Err_NONE;
    }

    // メモリ確保
    A_long w2 = w + radius;
    w2 = (w2 + (16 - w2 % 16));

    // --- vectorを使用したラインメモリの実装 ---
    std::vector<PixelType> line(w2);
    std::vector<FloatType> lineLevel(w2);
    std::vector<LineBufInfo> forward(w2);
    std::vector<LineBufInfo> backward(w2);


    
    for (A_long i = 0; i < w; i++) {

        line[i] = outP[i];
        lineLevel[i] = (((FloatType)line[i].red * 0.299) + ((FloatType)line[i].green * 0.587) + ((FloatType)line[i].blue * 0.114))*10 + (FloatType)line[i].alpha/maxChan;
    }

    // VHGアルゴリズム
    int L = radius * 2 + 1;
    for (int b = 0; b < w; b += L) {
        int end = MIN(b + L, w);

        forward[b].level = lineLevel[b];
        forward[b].index = b;
        for (int i = b + 1; i < end; i++) {
            if (forward[i - 1].level >= lineLevel[i]) {
                forward[i] = forward[i - 1];
            }
            else {
                forward[i].level = lineLevel[i];
                forward[i].index = i;
            }
        }

        backward[end - 1].level = lineLevel[end - 1];
        backward[end - 1].index = end - 1;
        for (int i = end - 2; i >= b; i--) {
            if (backward[i + 1].level >= lineLevel[i]) {
                backward[i] = backward[i + 1];
            }
            else {
                backward[i].level = lineLevel[i];
                backward[i].index = i;
            }
        }
    }

    // 出力書き戻し
    for (int i = 0; i < w; i++) {
        int left = i - radius;
        int right = i + radius;

        if (right < 0) {
            outP[i] = line[forward[0].index];
        }
        else if (left >= w) {
            outP[i] = line[backward[w - 1].index];
        }
        else {
            int safe_left = (left < 0) ? 0 : left;
            int safe_right = (right >= w) ? w - 1 : right;

            int left_block = safe_left / L;
            int right_block = safe_right / L;

            FloatType maxLevel = -FLT_MAX;
            int maxIndex = safe_left;

            if (left_block == right_block) {
                for (int j = safe_left; j <= safe_right; j++) {
                    if (lineLevel[j] > maxLevel) {
                        maxLevel = lineLevel[j];
                        maxIndex = j;
                    }
                }
            }
            else {
                if (backward[safe_left].level > maxLevel) {
                    maxLevel = backward[safe_left].level;
                    maxIndex = backward[safe_left].index;
                }

                for (int b = left_block + 1; b < right_block; b++) {
                    int block_start = b * L;
                    int block_end = MIN(block_start + L - 1, w - 1);
                    if (forward[block_end].level > maxLevel) {
                        maxLevel = forward[block_end].level;
                        maxIndex = forward[block_end].index;
                    }
                }

                if (forward[safe_right].level > maxLevel) {
                    maxLevel = forward[safe_right].level;
                    maxIndex = forward[safe_right].index;
                }
            }

            outP[i] = line[maxIndex];
        }
    }

    return PF_Err_NONE;
}

// 8bit版
static PF_Err Max_SubH8(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL)
{
    return Max_SubH<PF_Pixel8, A_u_char, A_FpShort>(refconPV, thread_idxL, y, itrtL, (A_FpShort)PF_MAX_CHAN8);
}

// 16bit版
static PF_Err Max_SubH16(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL)
{
    return Max_SubH<PF_Pixel16, A_u_short, A_FpShort>(refconPV, thread_idxL, y, itrtL, (A_FpShort)PF_MAX_CHAN16);
}

// 32bit版
static PF_Err Max_SubH32(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL)
{
    return Max_SubH<PF_PixelFloat, PF_FpShort, PF_FpShort>(refconPV, thread_idxL, y, itrtL, 1.0f);
}
// ************************************************************************
// ************************************************************************
template <typename PixelType, typename ChannelType, typename FloatType>
static PF_Err
Min_SubH(
    void* refconPV,
    A_long thread_idxL,
    A_long y,
    A_long itrtL,
    FloatType maxChan)
{
    PF_Err err = PF_Err_NONE;
    MinMaxInfo* infoP = static_cast<MinMaxInfo*>(refconPV);
    int radius = infoP->max;
    if (radius <= 0) return err;

    PF_InData* in_data = infoP->in_data;
    A_long w = infoP->width;
    PixelType* outP = (PixelType*)infoP->world->data + (y * infoP->widthTrue);

    // 1. 半径が画像幅以上の場合は全画面最小値を計算
    if (radius >= w) {
        PixelType miin = outP[0];
        FloatType miinV = (((FloatType)miin.red * 0.299) + ((FloatType)miin.green * 0.587) + ((FloatType)miin.blue * 0.114))*10 + (FloatType)miin.alpha/maxChan;

        for (A_long x = 1; x < w; x++) {
            FloatType v = (((FloatType)outP[x].red * 0.299) + ((FloatType)outP[x].green * 0.587) + ((FloatType)outP[x].blue * 0.114))*10 + (FloatType)outP[x].alpha/maxChan;
            if (v < miinV) { miin = outP[x]; miinV = v; }  // < に変更
        }
        for (A_long x = 0; x < w; x++) outP[x] = miin;
        return PF_Err_NONE;
    }

    // メモリ確保
    A_long w2 = w + radius;
    w2 = (w2 + (16 - w2 % 16));

    // --- vectorを使用したラインメモリの実装 ---
    std::vector<PixelType> line(w2);
    std::vector<FloatType> lineLevel(w2);
    std::vector<LineBufInfo> forward(w2);
    std::vector<LineBufInfo> backward(w2);

    for (A_long i = 0; i < w; i++) {
        line[i] = outP[i];
        lineLevel[i] = (((FloatType)line[i].red * 0.299) + ((FloatType)line[i].green * 0.587) + ((FloatType)line[i].blue * 0.114))*10 + (FloatType)line[i].alpha/maxChan;
    }

    // VHGアルゴリズム（最小値用に修正）
    int L = radius * 2 + 1;
    for (int b = 0; b < w; b += L) {
        int end = MIN(b + L, w);

        forward[b].level = lineLevel[b];
        forward[b].index = b;
        for (int i = b + 1; i < end; i++) {
            if (forward[i - 1].level <= lineLevel[i]) {  // <= に変更
                forward[i] = forward[i - 1];
            }
            else {
                forward[i].level = lineLevel[i];
                forward[i].index = i;
            }
        }

        backward[end - 1].level = lineLevel[end - 1];
        backward[end - 1].index = end - 1;
        for (int i = end - 2; i >= b; i--) {
            if (backward[i + 1].level <= lineLevel[i]) {  // <= に変更
                backward[i] = backward[i + 1];
            }
            else {
                backward[i].level = lineLevel[i];
                backward[i].index = i;
            }
        }
    }

    // 出力書き戻し
    for (int i = 0; i < w; i++) {
        int left = i - radius;
        int right = i + radius;

        if (right < 0) {
            outP[i] = line[forward[0].index];
        }
        else if (left >= w) {
            outP[i] = line[backward[w - 1].index];
        }
        else {
            int safe_left = (left < 0) ? 0 : left;
            int safe_right = (right >= w) ? w - 1 : right;

            int left_block = safe_left / L;
            int right_block = safe_right / L;

            FloatType minLevel = FLT_MAX;  // FLT_MAX に変更
            int minIndex = safe_left;

            if (left_block == right_block) {
                for (int j = safe_left; j <= safe_right; j++) {
                    if (lineLevel[j] < minLevel) {  // < に変更
                        minLevel = lineLevel[j];
                        minIndex = j;
                    }
                }
            }
            else {
                if (backward[safe_left].level < minLevel) {  // < に変更
                    minLevel = backward[safe_left].level;
                    minIndex = backward[safe_left].index;
                }

                for (int b = left_block + 1; b < right_block; b++) {
                    int block_start = b * L;
                    int block_end = MIN(block_start + L - 1, w - 1);
                    if (forward[block_end].level < minLevel) {  // < に変更
                        minLevel = forward[block_end].level;
                        minIndex = forward[block_end].index;
                    }
                }

                if (forward[safe_right].level < minLevel) {  // < に変更
                    minLevel = forward[safe_right].level;
                    minIndex = forward[safe_right].index;
                }
            }

            outP[i] = line[minIndex];
        }
    }
    return PF_Err_NONE;
}

// 8bit版
static PF_Err Min_SubH8(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL)
{
    return Min_SubH<PF_Pixel8, A_u_char, A_FpShort>(refconPV, thread_idxL, y, itrtL, (A_FpShort)PF_MAX_CHAN8);
}

// 16bit版
static PF_Err Min_SubH16(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL)
{
    return Min_SubH<PF_Pixel16, A_u_short, A_FpShort>(refconPV, thread_idxL, y, itrtL, (A_FpShort)PF_MAX_CHAN16);
}

// 32bit版
static PF_Err Min_SubH32(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL)
{
    return Min_SubH<PF_PixelFloat, PF_FpShort, PF_FpShort>(refconPV, thread_idxL, y, itrtL, 1.0f);
}
// ************************************************************************
template <typename PixelType, typename ChannelType, typename FloatType>
static PF_Err
Max_SubV(
    void* refconPV,
    A_long thread_idxL,
    A_long x,
    A_long itrtL,
    FloatType maxChan)
{
    PF_Err err = PF_Err_NONE;
    MinMaxInfo* infoP = static_cast<MinMaxInfo*>(refconPV);
    int radius = infoP->max;
    if (radius <= 0) return err;

    PF_InData* in_data = infoP->in_data;
    A_long h = infoP->height;
    A_long wt = infoP->widthTrue;

    // 1. 半径が画像高さ以上の場合は全列最大値を計算
    if (radius >= h) {
        PixelType maax = *((PixelType*)infoP->world->data + x);
        FloatType maaxV = (((FloatType)maax.red * 0.299) + ((FloatType)maax.green * 0.587) + ((FloatType)maax.blue * 0.114))*10 + (FloatType)maax.alpha/maxChan;

        for (A_long y = 1; y < h; y++) {
            PixelType* p = (PixelType*)infoP->world->data + (y * wt + x);
            FloatType v = (((FloatType)p->red * 0.299) + ((FloatType)p->green * 0.587) + ((FloatType)p->blue * 0.114)) + (FloatType)p->alpha / maxChan;
            if (v > maaxV) { maax = *p; maaxV = v; }
        }
        for (A_long y = 0; y < h; y++) {
            *((PixelType*)infoP->world->data + (y * wt + x)) = maax;
        }
        return PF_Err_NONE;
    }

    // 2. メモリ確保
    A_long h2 = h + radius;
    h2 = (h2 + (16 - h2 % 16));

    PF_EffectWorld wld;
    AEFX_CLR_STRUCT(wld);
    ERR((*in_data->utils->new_world)(in_data->effect_ref, h2, 8, PF_NewWorldFlag_DEEP_PIXELS, &wld));
    if (err) return err;

    PixelType* line = (PixelType*)wld.data;
    FloatType* lineLevel = (FloatType*)(line + h2);
    LineBufInfo* forward = (LineBufInfo*)(lineLevel + h2);
    LineBufInfo* backward = forward + h2;

    // 3. 列データをコピー
    for (A_long y = 0; y < h; y++) {
        line[y] = *((PixelType*)infoP->world->data + (y * wt + x));
        lineLevel[y] = (((FloatType)line[y].red * 0.299) + ((FloatType)line[y].green * 0.587) + ((FloatType)line[y].blue * 0.114)) + (FloatType)line[y].alpha/maxChan;
    }

    // 4. VHGアルゴリズム
    int L = radius * 2 + 1;
    for (int b = 0; b < h; b += L) {
        int end = MIN(b + L, h);

        forward[b].level = lineLevel[b];
        forward[b].index = b;
        for (int i = b + 1; i < end; i++) {
            if (forward[i - 1].level >= lineLevel[i]) {
                forward[i] = forward[i - 1];
            }
            else {
                forward[i].level = lineLevel[i];
                forward[i].index = i;
            }
        }

        backward[end - 1].level = lineLevel[end - 1];
        backward[end - 1].index = end - 1;
        for (int i = end - 2; i >= b; i--) {
            if (backward[i + 1].level >= lineLevel[i]) {
                backward[i] = backward[i + 1];
            }
            else {
                backward[i].level = lineLevel[i];
                backward[i].index = i;
            }
        }
    }

    // 5. 出力
    for (int y = 0; y < h; y++) {
        int top = y - radius;
        int bottom = y + radius;

        if (bottom < 0) {
            *((PixelType*)infoP->world->data + (y * wt + x)) = line[forward[0].index];
        }
        else if (top >= h) {
            *((PixelType*)infoP->world->data + (y * wt + x)) = line[backward[h - 1].index];
        }
        else {
            int safe_top = (top < 0) ? 0 : top;
            int safe_bottom = (bottom >= h) ? h - 1 : bottom;

            int top_block = safe_top / L;
            int bottom_block = safe_bottom / L;

            FloatType maxLevel = -FLT_MAX;
            int maxIndex = safe_top;

            if (top_block == bottom_block) {
                for (int j = safe_top; j <= safe_bottom; j++) {
                    if (lineLevel[j] > maxLevel) {
                        maxLevel = lineLevel[j];
                        maxIndex = j;
                    }
                }
            }
            else {
                if (backward[safe_top].level > maxLevel) {
                    maxLevel = backward[safe_top].level;
                    maxIndex = backward[safe_top].index;
                }

                for (int b = top_block + 1; b < bottom_block; b++) {
                    int block_start = b * L;
                    int block_end = MIN(block_start + L - 1, h - 1);
                    if (forward[block_end].level > maxLevel) {
                        maxLevel = forward[block_end].level;
                        maxIndex = forward[block_end].index;
                    }
                }

                if (forward[safe_bottom].level > maxLevel) {
                    maxLevel = forward[safe_bottom].level;
                    maxIndex = forward[safe_bottom].index;
                }
            }

            *((PixelType*)infoP->world->data + (y * wt + x)) = line[maxIndex];
        }
    }

    ERR((*in_data->utils->dispose_world)(in_data->effect_ref, &wld));
    return PF_Err_NONE;
}

// 8bit版
static PF_Err Max_SubV8(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL)
{
    return Max_SubV<PF_Pixel8, A_u_char, A_FpShort>(refconPV, thread_idxL, x, itrtL, (A_FpShort)PF_MAX_CHAN8);
}

// 16bit版
static PF_Err Max_SubV16(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL)
{
    return Max_SubV<PF_Pixel16, A_u_short, A_FpShort>(refconPV, thread_idxL, x, itrtL, (A_FpShort)PF_MAX_CHAN16);
}

// 32bit版
static PF_Err Max_SubV32(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL)
{
    return Max_SubV<PF_PixelFloat, PF_FpShort, PF_FpShort>(refconPV, thread_idxL, x, itrtL, 1.0f);
}
template <typename PixelType, typename ChannelType, typename FloatType>
static PF_Err
Min_SubV(
    void* refconPV,
    A_long thread_idxL,
    A_long x,
    A_long itrtL,
    FloatType maxChan)
{
    PF_Err err = PF_Err_NONE;
    MinMaxInfo* infoP = static_cast<MinMaxInfo*>(refconPV);
    int radius = infoP->max;
    if (radius <= 0) return err;

    PF_InData* in_data = infoP->in_data;
    A_long h = infoP->height;
    A_long wt = infoP->widthTrue;

    // 1. 半径が画像高さ以上の場合は全列最小値を計算
    if (radius >= h) {
        PixelType miin = *((PixelType*)infoP->world->data + x);
        FloatType miinV = (((FloatType)miin.red * 0.299) + ((FloatType)miin.green * 0.587) + ((FloatType)miin.blue * 0.114))*10+ (FloatType)miin.alpha /maxChan;

        for (A_long y = 1; y < h; y++) {
            PixelType* p = (PixelType*)infoP->world->data + (y * wt + x);
            FloatType v = (((FloatType)p->red * 0.299) + ((FloatType)p->green * 0.587) + ((FloatType)p->blue * 0.114))*10 + (FloatType)p->alpha/maxChan;
            if (v < miinV) { miin = *p; miinV = v; }  // < に変更
        }
        for (A_long y = 0; y < h; y++) {
            *((PixelType*)infoP->world->data + (y * wt + x)) = miin;
        }
        return PF_Err_NONE;
    }

    // 2. メモリ確保
    A_long h2 = h + radius;
    h2 = (h2 + (16 - h2 % 16));

    std::vector<PixelType> line(h2);
    std::vector<FloatType> lineLevel(h2);
    std::vector<LineBufInfo> forward(h2);
    std::vector<LineBufInfo> backward(h2);

    // 3. 列データをコピー
    for (A_long y = 0; y < h; y++) {
        line[y] = *((PixelType*)infoP->world->data + (y * wt + x));
        lineLevel[y] = (((FloatType)line[y].red * 0.299) + ((FloatType)line[y].green * 0.587) + ((FloatType)line[y].blue * 0.114))*10 + (FloatType)line[y].alpha/maxChan;
    }

    // 4. VHGアルゴリズム（最小値用に修正）
    int L = radius * 2 + 1;
    for (int b = 0; b < h; b += L) {
        int end = MIN(b + L, h);

        forward[b].level = lineLevel[b];
        forward[b].index = b;
        for (int i = b + 1; i < end; i++) {
            if (forward[i - 1].level <= lineLevel[i]) {  // <= に変更
                forward[i] = forward[i - 1];
            }
            else {
                forward[i].level = lineLevel[i];
                forward[i].index = i;
            }
        }

        backward[end - 1].level = lineLevel[end - 1];
        backward[end - 1].index = end - 1;
        for (int i = end - 2; i >= b; i--) {
            if (backward[i + 1].level <= lineLevel[i]) {  // <= に変更
                backward[i] = backward[i + 1];
            }
            else {
                backward[i].level = lineLevel[i];
                backward[i].index = i;
            }
        }
    }

    // 5. 出力
    for (int y = 0; y < h; y++) {
        int top = y - radius;
        int bottom = y + radius;

        if (bottom < 0) {
            *((PixelType*)infoP->world->data + (y * wt + x)) = line[forward[0].index];
        }
        else if (top >= h) {
            *((PixelType*)infoP->world->data + (y * wt + x)) = line[backward[h - 1].index];
        }
        else {
            int safe_top = (top < 0) ? 0 : top;
            int safe_bottom = (bottom >= h) ? h - 1 : bottom;

            int top_block = safe_top / L;
            int bottom_block = safe_bottom / L;

            FloatType minLevel = FLT_MAX;  // FLT_MAX に変更
            int minIndex = safe_top;

            if (top_block == bottom_block) {
                for (int j = safe_top; j <= safe_bottom; j++) {
                    if (lineLevel[j] < minLevel) {  // < に変更
                        minLevel = lineLevel[j];
                        minIndex = j;
                    }
                }
            }
            else {
                if (backward[safe_top].level < minLevel) {  // < に変更
                    minLevel = backward[safe_top].level;
                    minIndex = backward[safe_top].index;
                }

                for (int b = top_block + 1; b < bottom_block; b++) {
                    int block_start = b * L;
                    int block_end = MIN(block_start + L - 1, h - 1);
                    if (forward[block_end].level < minLevel) {  // < に変更
                        minLevel = forward[block_end].level;
                        minIndex = forward[block_end].index;
                    }
                }

                if (forward[safe_bottom].level < minLevel) {  // < に変更
                    minLevel = forward[safe_bottom].level;
                    minIndex = forward[safe_bottom].index;
                }
            }

            *((PixelType*)infoP->world->data + (y * wt + x)) = line[minIndex];
        }
    }
    return PF_Err_NONE;
}

// 8bit版
static PF_Err Min_SubV8(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL)
{
    return Min_SubV<PF_Pixel8, A_u_char, A_FpShort>(refconPV, thread_idxL, x, itrtL, (A_FpShort)PF_MAX_CHAN8);
}

// 16bit版
static PF_Err Min_SubV16(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL)
{
    return Min_SubV<PF_Pixel16, A_u_short, A_FpShort>(refconPV, thread_idxL, x, itrtL, (A_FpShort)PF_MAX_CHAN16);
}

// 32bit版
static PF_Err Min_SubV32(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL)
{
    return Min_SubV<PF_PixelFloat, PF_FpShort, PF_FpShort>(refconPV, thread_idxL, x, itrtL, 1.0f);
}
static PF_Err TinyMinMaxImpl(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    A_long value
)
{
    //ae->DisposeWorld
    PF_Err err = PF_Err_NONE;
	if (value == 0) return err;

    PF_WorldSuite2* ws2P;
    PF_PixelFormat pixelFormat;
    AEFX_AcquireSuite(in_dataP,
        out_dataP,
        kPFWorldSuite,
        kPFWorldSuiteVersion2,
        "Couldn't load suite.",
        (void**)&(ws2P));
    ws2P->PF_GetPixelFormat(worldP, &pixelFormat);

    MinMaxInfo info;
	info.in_data = in_dataP;
    if (value < 0) {
        info.max = -value;
        info.maxMinus = TRUE;
    }
    else {
        info.max = value;
        info.maxMinus = FALSE;
    }
    info.world = worldP;
    info.width = worldP->width;
    info.rowbytes = worldP->rowbytes;
    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB32:
        info.widthTrue = worldP->rowbytes / sizeof(PF_Pixel);
		break;
    case PF_PixelFormat_ARGB64:
        info.widthTrue = worldP->rowbytes / sizeof(PF_Pixel16);
        break;
    case PF_PixelFormat_ARGB128:
        info.widthTrue = worldP->rowbytes / sizeof(PF_PixelFloat);
        break;
    }
    info.height = worldP->height;

    if (!err) {
        PF_Iterate8Suite1* iterS = nullptr;
        AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scope(
            in_dataP,
            kPFIterate8Suite,
            kPFIterate8SuiteVersion1,
            out_dataP
        );
        switch(pixelFormat)
        {
        case PF_PixelFormat_ARGB32:
            if (info.maxMinus == FALSE) {
                ERR(iter_scope->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Max_SubH8              // fn_func: コールバック関数
                ));

                ERR(iter_scope->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Max_SubV8    // fn_func: コールバック関数
                ));
            }
            else {
                ERR(iter_scope->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Min_SubH8              // fn_func: コールバック関数
                ));

                ERR(iter_scope->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Min_SubV8    // fn_func: コールバック関数
                ));
            }
            break;
        case PF_PixelFormat_ARGB64:
            if (info.maxMinus == FALSE) {
                ERR(iter_scope->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Max_SubH16              // fn_func: コールバック関数
                ));

                ERR(iter_scope->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Max_SubV16    // fn_func: コールバック関数
                ));
            }
            else {
                ERR(iter_scope->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Min_SubH16              // fn_func: コールバック関数
                ));

                ERR(iter_scope->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Min_SubV16    // fn_func: コールバック関数
                ));
            }
            break;
        case PF_PixelFormat_ARGB128:
            if (info.maxMinus == FALSE) {
                ERR(iter_scope->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Max_SubH32              // fn_func: コールバック関数
                ));

                ERR(iter_scope->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Max_SubV32    // fn_func: コールバック関数
                ));
            }
            else {
                ERR(iter_scope->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Min_SubH32              // fn_func: コールバック関数
                ));

                ERR(iter_scope->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Min_SubV32    // fn_func: コールバック関数
                ));
            }
            break;
        default:
            err = PF_Err_BAD_CALLBACK_PARAM;
            break;
        }
    }
    return err;
}

PF_Err TinyMinMax(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    A_long value
)
{
	return TinyMinMaxImpl(in_dataP, out_dataP, worldP,value);
}
static PF_Err TinyMinMaxMImpl(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scopeP,
    A_long value
)
{
    PF_Err err = PF_Err_NONE;
    if (value == 0) return err;

    MinMaxInfo info;
    info.in_data = in_dataP;
    if (value < 0) {
        info.max = -1*value;
        info.maxMinus = TRUE;
    }
    else {
        info.max = value;
        info.maxMinus = FALSE;
    }
    info.world = worldP;
    info.width = worldP->width;
    info.rowbytes = worldP->rowbytes;
    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB32:
        info.widthTrue = worldP->rowbytes / sizeof(PF_Pixel);
        break;
    case PF_PixelFormat_ARGB64:
        info.widthTrue = worldP->rowbytes / sizeof(PF_Pixel16);
        break;
    case PF_PixelFormat_ARGB128:
        info.widthTrue = worldP->rowbytes / sizeof(PF_PixelFloat);
        break;
    }
    info.height = worldP->height;

    if (!err) {
        switch (pixelFormat)
        {
        case PF_PixelFormat_ARGB32:
            if (info.maxMinus == FALSE) {
                ERR(iter_scopeP->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Max_SubH8              // fn_func: コールバック関数
                ));

                ERR(iter_scopeP->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Max_SubV8    // fn_func: コールバック関数
                ));
            }
            else {
                ERR(iter_scopeP->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Min_SubH8              // fn_func: コールバック関数
                ));

                ERR(iter_scopeP->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Min_SubV8    // fn_func: コールバック関数
                ));
            }
            break;
        case PF_PixelFormat_ARGB64:
            if (info.maxMinus == FALSE) {
                ERR(iter_scopeP->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Max_SubH16              // fn_func: コールバック関数
                ));

                ERR(iter_scopeP->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Max_SubV16    // fn_func: コールバック関数
                ));
            }
            else {
                ERR(iter_scopeP->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Min_SubH16              // fn_func: コールバック関数
                ));

                ERR(iter_scopeP->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Min_SubV16    // fn_func: コールバック関数
                ));
            }
            break;
        case PF_PixelFormat_ARGB128:
            if (info.maxMinus == FALSE) {
                ERR(iter_scopeP->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Max_SubH32              // fn_func: コールバック関数
                ));

                ERR(iter_scopeP->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Max_SubV32    // fn_func: コールバック関数
                ));
            }
            else {
                ERR(iter_scopeP->iterate_generic(
                    info.height,     // iterationsL: 実行回数（＝行数）
                    &info,                  // refconPV: ユーザー定義データ
                    Min_SubH32              // fn_func: コールバック関数
                ));

                ERR(iter_scopeP->iterate_generic(
                    info.width,           // iterationsL: 実行回数（＝行数）
                    &info,                    // refconPV: ユーザー定義データ
                    Min_SubV32    // fn_func: コールバック関数
                ));
            }
            break;
        default:
            err = PF_Err_BAD_CALLBACK_PARAM;
            break;
        }
    }
    return err;
}
PF_Err TinyMinMaxM(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scopeP,
    A_long value
)
{
    return TinyMinMaxMImpl(in_dataP, worldP, pixelFormat, iter_scopeP,value);
}