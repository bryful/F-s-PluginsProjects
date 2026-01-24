#include "tiny_blur.h"

// スレッド間で共有する構造体
typedef struct {
    PF_InData* in_data;
    PF_EffectWorld* world;
    A_long radius;
    A_long width;
    A_long height;
    A_long rowbytes;
} BlurInfo;

#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))


template <typename T, typename CH>
static PF_Err MultT(
    void* refconPV,
    A_long y,
    CH max_chan)
{
    PF_Err err = PF_Err_NONE;
    BlurInfo* bi = static_cast<BlurInfo*>(refconPV);

    // 行ポインタの取得 (バイト計算後に型キャスト)
    T* rowP = reinterpret_cast<T*>(reinterpret_cast<char*>(bi->world->data) + (y * bi->rowbytes));

    for (A_long i = 0; i < bi->world->width; i++) {
        // アルファが0ならRGBを0にする（ストレート変換的な処理）
        if (rowP[i].alpha <= 0) {
            rowP[i].red = 0;
            rowP[i].green = 0;
            rowP[i].blue = 0;
            continue; // ※元コードのreturnをcontinueに修正（行の全ピクセルを処理するため）
        }
        // アルファが最大値ならそのまま
        else if (rowP[i].alpha >= max_chan) {
            continue;
        }

        // アルファ比率の計算
        A_FpLong v = static_cast<A_FpLong>(rowP[i].alpha) / max_chan;

        // クランプ処理
        if (v < 0.0) v = 0.0; else if (v > 1.0) v = 1.0;

        // 乗算（32bit Floatの場合はそのまま、8/16bitは整数キャスト）
        rowP[i].red = static_cast<CH>(rowP[i].red * v);
        rowP[i].green = static_cast<CH>(rowP[i].green * v);
        rowP[i].blue = static_cast<CH>(rowP[i].blue * v);
    }

    return err;
}
// 8-bit用
static PF_Err Mult8(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return MultT<PF_Pixel8, A_u_char>(refcon, y, PF_MAX_CHAN8);
}

// 16-bit用
static PF_Err Mult16(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return MultT<PF_Pixel16, A_u_short>(refcon, y, PF_MAX_CHAN16);
}

// 32-bit用 (Float)
static PF_Err MultFloat(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    // 32bitの場合、max_chanは 1.0f
    return MultT<PF_PixelFloat, PF_FpShort>(refcon, y, 1.0f);
}
template <typename T, typename CH>
static PF_Err UnMultT(
    void* refconPV,
    A_long y,
    CH max_chan)
{
    PF_Err err = PF_Err_NONE;
    BlurInfo* bi = static_cast<BlurInfo*>(refconPV);

    // 行ポインタの取得 (バイト計算後に型キャスト)
    T* rowP = reinterpret_cast<T*>(reinterpret_cast<char*>(bi->world->data) + (y * bi->rowbytes));

    for (A_long i = 0; i < bi->world->width; i++) {
        // アルファが0ならRGBを0にする（ストレート変換的な処理）
        if (rowP[i].alpha <= 0) {
            rowP[i].red = 0;
            rowP[i].green = 0;
            rowP[i].blue = 0;
            continue; // ※元コードのreturnをcontinueに修正（行の全ピクセルを処理するため）
        }
        // アルファが最大値ならそのまま
        else if (rowP[i].alpha >= max_chan) {
            continue;
        }

        // アルファ比率の計算
        A_FpLong v = static_cast<A_FpLong>(max_chan / rowP[i].alpha);

        // クランプ処理
        if (v < 0.0) v = 0.0; else if (v > 1.0) v = 1.0;

        // 乗算（32bit Floatの場合はそのまま、8/16bitは整数キャスト）
        rowP[i].red = static_cast<CH>(rowP[i].red * v);
        rowP[i].green = static_cast<CH>(rowP[i].green * v);
        rowP[i].blue = static_cast<CH>(rowP[i].blue * v);
    }

    return err;
}
// 8-bit用
static PF_Err UnMult8(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultT<PF_Pixel8, A_u_char>(refcon, y, PF_MAX_CHAN8);
}

// 16-bit用
static PF_Err UnMult16(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultT<PF_Pixel16, A_u_short>(refcon, y, PF_MAX_CHAN16);
}

// 32-bit用 (Float)
static PF_Err UnMultFloat(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    // 32bitの場合、max_chanは 1.0f
    return UnMultT<PF_PixelFloat, PF_FpShort>(refcon, y, 1.0f);
}
/* --- 水平ボックスブラー (iterate_generic用) --- */
template <typename PixelType, A_long MaxChan>
static PF_Err
BoxBlurH(
    void* refconPV,
    A_long thread_idxL,
    A_long y,
    A_long itrtL)
{
    BlurInfo* bi = static_cast<BlurInfo*>(refconPV);
    A_long width = bi->width;
    A_long r = bi->radius;

    // 行の先頭ポインタ取得
    PixelType* rowP = (PixelType*)((char*)bi->world->data + (y * bi->rowbytes));

    double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    A_long window = (r * 2) + 1;

    // 初期ウィンドウの計算
    for (A_long i = -r; i <= r; i++) {
        PixelType* p = rowP + AE_CLAMP(i, 0, width - 1);
        sumR += (double)p->red; sumG += (double)p->green; sumB += (double)p->blue; sumA += (double)p->alpha;
    }

    // スライディングウィンドウ処理
    // 注：インプレース処理のため、本来は一時バッファが必要ですが、
    // BlueSub.cppの元実装に合わせて直接書き換えています。
    for (A_long i = 0; i < width; i++) {
        A_long left = AE_CLAMP(i - r, 0, width - 1);
        A_long right = AE_CLAMP(i + r + 1, 0, width - 1);

        PixelType current = rowP[i]; // 現在の値を保存（加算用）

        rowP[i].red = static_cast<decltype(PixelType::red)>(AE_CLAMP(sumR / window, 0, MaxChan));
        rowP[i].green = static_cast<decltype(PixelType::green)>(AE_CLAMP(sumG / window, 0, MaxChan));
        rowP[i].blue = static_cast<decltype(PixelType::blue)>(AE_CLAMP(sumB / window, 0, MaxChan));
        rowP[i].alpha = static_cast<decltype(PixelType::alpha)>(AE_CLAMP(sumA / window, 0, MaxChan));

        sumR += ((double)rowP[right].red - (double)rowP[left].red);
        sumG += ((double)rowP[right].green - (double)rowP[left].green);
        sumB += ((double)rowP[right].blue - (double)rowP[left].blue);
        sumA += ((double)rowP[right].alpha - (double)rowP[left].alpha);
    }
    return PF_Err_NONE;
}

/* --- 垂直ボックスブラー (iterate_generic用) --- */
template <typename PixelType, A_long MaxChan>
static PF_Err
BoxBlurV(
    void* refconPV,
    A_long thread_idxL,
    A_long x,
    A_long itrtL)
{
    BlurInfo* bi = static_cast<BlurInfo*>(refconPV);
    A_long height = bi->height;
    A_long r = bi->radius;
    A_long stride = bi->rowbytes / sizeof(PixelType);

    PixelType* colTopP = (PixelType*)bi->world->data + x;

    double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    A_long window = (r * 2) + 1;

    for (A_long i = -r; i <= r; i++) {
        PixelType* p = colTopP + (AE_CLAMP(i, 0, height - 1) * stride);
        sumR += (double)p->red; sumG += (double)p->green; sumB += (double)p->blue; sumA += (double)p->alpha;
    }

    for (A_long i = 0; i < height; i++) {
        PixelType* target = colTopP + (i * stride);

        A_long left = AE_CLAMP(i - r, 0, height - 1);
        A_long right = AE_CLAMP(i + r + 1, 0, height - 1);

        target->red = static_cast<decltype(PixelType::red)>(AE_CLAMP(sumR / window, 0, MaxChan));
        target->green = static_cast<decltype(PixelType::green)>(AE_CLAMP(sumG / window, 0, MaxChan));
        target->blue = static_cast<decltype(PixelType::blue)>(AE_CLAMP(sumB / window, 0, MaxChan));
        target->alpha = static_cast<decltype(PixelType::alpha)>(AE_CLAMP(sumA / window, 0, MaxChan));

        sumR += (double)(colTopP + right * stride)->red - (double)(colTopP + left * stride)->red;
        sumG += (double)(colTopP + right * stride)->green - (double)(colTopP + left * stride)->green;
        sumB += (double)(colTopP + right * stride)->blue - (double)(colTopP + left * stride)->blue;
        sumA += (double)(colTopP + right * stride)->alpha - (double)(colTopP + left * stride)->alpha;
    }
    return PF_Err_NONE;
}

/* --- ビット深度ごとの関数ラップ --- */
static PF_Err BoxBlurH8(void* ref, A_long t, A_long i, A_long l) { return BoxBlurH<PF_Pixel8, PF_MAX_CHAN8>(ref, t, i, l); }
static PF_Err BoxBlurV8(void* ref, A_long t, A_long i, A_long l) { return BoxBlurV<PF_Pixel8, PF_MAX_CHAN8>(ref, t, i, l); }
static PF_Err BoxBlurH16(void* ref, A_long t, A_long i, A_long l) { return BoxBlurH<PF_Pixel16, PF_MAX_CHAN16>(ref, t, i, l); }
static PF_Err BoxBlurV16(void* ref, A_long t, A_long i, A_long l) { return BoxBlurV<PF_Pixel16, PF_MAX_CHAN16>(ref, t, i, l); }
static PF_Err BoxBlurH32(void* ref, A_long t, A_long i, A_long l) { return BoxBlurH<PF_PixelFloat, 1>(ref, t, i, l); } // Floatは1.0
static PF_Err BoxBlurV32(void* ref, A_long t, A_long i, A_long l) { return BoxBlurV<PF_PixelFloat, 1>(ref, t, i, l); }

static PF_Err TinyBlueImpl(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
   A_long value
)
{
    PF_Err err = PF_Err_NONE;
    if (value <= 0) return err;

    BlurInfo bi;
    bi.in_data = in_dataP;
    bi.world = worldP;
    bi.radius = value;
    bi.width = worldP->width;
    bi.height = worldP->height;
    bi.rowbytes = worldP->rowbytes;

    PF_WorldSuite2* ws2P;
    PF_PixelFormat pixelFormat;
    AEFX_AcquireSuite(in_dataP,
        out_dataP,
        kPFWorldSuite,
        kPFWorldSuiteVersion2,
        "Couldn't load suite.",
        (void**)&(ws2P));

    ws2P->PF_GetPixelFormat(worldP, &pixelFormat);

    AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scope(
        in_dataP,
        kPFIterate8Suite,
        kPFIterate8SuiteVersion1,
        out_dataP
    );

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        ERR(iter_scope->iterate_generic(bi.height, &bi, Mult8));
        for (int n = 0; n < 3; n++) {
            ERR(iter_scope->iterate_generic(bi.height, &bi, BoxBlurH8));
            ERR(iter_scope->iterate_generic(bi.width, &bi, BoxBlurV8));
        }
        ERR(iter_scope->iterate_generic(bi.height, &bi, UnMult8));
        break;

    case PF_PixelFormat_ARGB64:
        ERR(iter_scope->iterate_generic(bi.height, &bi, Mult16));
        for (int n = 0; n < 3; n++) {
            ERR(iter_scope->iterate_generic(bi.height, &bi, BoxBlurH16));
            ERR(iter_scope->iterate_generic(bi.width, &bi, BoxBlurV16));
        }
        ERR(iter_scope->iterate_generic(bi.height, &bi, UnMult16));
        break;

    case PF_PixelFormat_ARGB128:
        ERR(iter_scope->iterate_generic(bi.height, &bi, MultFloat));
        for (int n = 0; n < 3; n++) {
            ERR(iter_scope->iterate_generic(bi.height, &bi, BoxBlurH32));
            ERR(iter_scope->iterate_generic(bi.width, &bi, BoxBlurV32));
        }
        ERR(iter_scope->iterate_generic(bi.height, &bi, UnMultFloat));
        break;
    }
    return err;
}

PF_Err TinyBlue(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    A_long value
)
{
    return TinyBlueImpl(in_dataP, out_dataP, worldP, value);
}