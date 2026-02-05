#include "Tiny_Blur.h"
#include <vector>

// スレッド間で共有する構造体
typedef struct {
    PF_InData* in_data;
    PF_EffectWorld* world;
    A_long radius;
    A_long width;
    A_long height;
    A_long rowbytes;
    void* temp_buffer;  // 一時バッファへのポインタ
} BlurInfo;

#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

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
    char* row_base = (char*)bi->world->data + (y * bi->rowbytes);
    PixelType* rowP = reinterpret_cast<PixelType*>(row_base);
    
    // 一時バッファ（スレッドセーフのため、各行ごとに別の領域を使用）
    PixelType* tempP = static_cast<PixelType*>(bi->temp_buffer) + (y * width);

    double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    A_long window = (r * 2) + 1;

    // 初期ウィンドウの計算
    for (A_long i = -r; i <= r; i++) {
        A_long idx = AE_CLAMP(i, 0, width - 1);
        sumR += (double)rowP[idx].red;
        sumG += (double)rowP[idx].green;
        sumB += (double)rowP[idx].blue;
        sumA += (double)rowP[idx].alpha;
    }

    // スライディングウィンドウ処理（一時バッファに書き込む）
    for (A_long i = 0; i < width; i++) {
        // 結果を一時バッファに書き込む
        tempP[i].red = static_cast<decltype(PixelType::red)>(AE_CLAMP(sumR / window, 0, MaxChan));
        tempP[i].green = static_cast<decltype(PixelType::green)>(AE_CLAMP(sumG / window, 0, MaxChan));
        tempP[i].blue = static_cast<decltype(PixelType::blue)>(AE_CLAMP(sumB / window, 0, MaxChan));
        tempP[i].alpha = static_cast<decltype(PixelType::alpha)>(AE_CLAMP(sumA / window, 0, MaxChan));

        // ウィンドウをスライド（元のデータから読む）
        A_long left = AE_CLAMP(i - r, 0, width - 1);
        A_long right = AE_CLAMP(i + r + 1, 0, width - 1);
        
        sumR += ((double)rowP[right].red - (double)rowP[left].red);
        sumG += ((double)rowP[right].green - (double)rowP[left].green);
        sumB += ((double)rowP[right].blue - (double)rowP[left].blue);
        sumA += ((double)rowP[right].alpha - (double)rowP[left].alpha);
    }
    
    // 一時バッファから元のバッファにコピー
    for (A_long i = 0; i < width; i++) {
        rowP[i] = tempP[i];
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
    
    // rowbytes を使って正しいストライドを計算
    char* base = (char*)bi->world->data;
    
    // 一時バッファ（列方向）
    PixelType* tempP = static_cast<PixelType*>(bi->temp_buffer) + (x * height);

    double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    A_long window = (r * 2) + 1;

    // 初期ウィンドウの計算
    for (A_long i = -r; i <= r; i++) {
        A_long idx = AE_CLAMP(i, 0, height - 1);
        PixelType* p = reinterpret_cast<PixelType*>(base + idx * bi->rowbytes) + x;
        sumR += (double)p->red;
        sumG += (double)p->green;
        sumB += (double)p->blue;
        sumA += (double)p->alpha;
    }

    // スライディングウィンドウ処理
    for (A_long i = 0; i < height; i++) {
        // 結果を一時バッファに書き込む
        tempP[i].red = static_cast<decltype(PixelType::red)>(AE_CLAMP(sumR / window, 0, MaxChan));
        tempP[i].green = static_cast<decltype(PixelType::green)>(AE_CLAMP(sumG / window, 0, MaxChan));
        tempP[i].blue = static_cast<decltype(PixelType::blue)>(AE_CLAMP(sumB / window, 0, MaxChan));
        tempP[i].alpha = static_cast<decltype(PixelType::alpha)>(AE_CLAMP(sumA / window, 0, MaxChan));

        // ウィンドウをスライド
        A_long left = AE_CLAMP(i - r, 0, height - 1);
        A_long right = AE_CLAMP(i + r + 1, 0, height - 1);
        
        PixelType* leftP = reinterpret_cast<PixelType*>(base + left * bi->rowbytes) + x;
        PixelType* rightP = reinterpret_cast<PixelType*>(base + right * bi->rowbytes) + x;
        
        sumR += ((double)rightP->red - (double)leftP->red);
        sumG += ((double)rightP->green - (double)leftP->green);
        sumB += ((double)rightP->blue - (double)leftP->blue);
        sumA += ((double)rightP->alpha - (double)leftP->alpha);
    }
    
    // 一時バッファから元のバッファにコピー
    for (A_long i = 0; i < height; i++) {
        PixelType* target = reinterpret_cast<PixelType*>(base + i * bi->rowbytes) + x;
        *target = tempP[i];
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

    // 一時バッファを確保
    size_t pixel_size;
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:  pixel_size = sizeof(PF_Pixel8); break;
    case PF_PixelFormat_ARGB64:  pixel_size = sizeof(PF_Pixel16); break;
    case PF_PixelFormat_ARGB128: pixel_size = sizeof(PF_PixelFloat); break;
    default: return PF_Err_INVALID_INDEX;
    }
    
    size_t buffer_size = std::max(worldP->width * worldP->height, worldP->height * worldP->width) * pixel_size;
    std::vector<char> temp_buffer(buffer_size);

    BlurInfo bi;
    bi.in_data = in_dataP;
    bi.world = worldP;
    bi.radius = value;
    bi.width = worldP->width;
    bi.height = worldP->height;
    bi.rowbytes = worldP->rowbytes;
    bi.temp_buffer = temp_buffer.data();

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        for (int n = 0; n < 3; n++) {
            ERR(iter_scope->iterate_generic(bi.height, &bi, BoxBlurH8));
            ERR(iter_scope->iterate_generic(bi.width, &bi, BoxBlurV8));
        }
        break;

    case PF_PixelFormat_ARGB64:
        for (int n = 0; n < 3; n++) {
            ERR(iter_scope->iterate_generic(bi.height, &bi, BoxBlurH16));
            ERR(iter_scope->iterate_generic(bi.width, &bi, BoxBlurV16));
        }
        break;

    case PF_PixelFormat_ARGB128:
        for (int n = 0; n < 3; n++) {
            ERR(iter_scope->iterate_generic(bi.height, &bi, BoxBlurH32));
            ERR(iter_scope->iterate_generic(bi.width, &bi, BoxBlurV32));
        }
        break;
    }
    return err;
}

static PF_Err TinyBlueMImpl(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scopeP,
    A_long value
)
{
    PF_Err err = PF_Err_NONE;
    if (value <= 0) return err;

    // 一時バッファを確保
    size_t pixel_size;
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:  pixel_size = sizeof(PF_Pixel8); break;
    case PF_PixelFormat_ARGB64:  pixel_size = sizeof(PF_Pixel16); break;
    case PF_PixelFormat_ARGB128: pixel_size = sizeof(PF_PixelFloat); break;
    default: return PF_Err_INVALID_INDEX;
    }
    
    size_t buffer_size = std::max(worldP->width * worldP->height, worldP->height * worldP->width) * pixel_size;
    std::vector<char> temp_buffer(buffer_size);

    BlurInfo bi;
    bi.in_data = in_dataP;
    bi.world = worldP;
    bi.radius = value;
    bi.width = worldP->width;
    bi.height = worldP->height;
    bi.rowbytes = worldP->rowbytes;
    bi.temp_buffer = temp_buffer.data();

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        for (int n = 0; n < 3; n++) {
            ERR(iter_scopeP->iterate_generic(bi.height, &bi, BoxBlurH8));
            ERR(iter_scopeP->iterate_generic(bi.width, &bi, BoxBlurV8));
        }
        break;

    case PF_PixelFormat_ARGB64:
        for (int n = 0; n < 3; n++) {
            ERR(iter_scopeP->iterate_generic(bi.height, &bi, BoxBlurH16));
            ERR(iter_scopeP->iterate_generic(bi.width, &bi, BoxBlurV16));
        }
        break;

    case PF_PixelFormat_ARGB128:
        for (int n = 0; n < 3; n++) {
            ERR(iter_scopeP->iterate_generic(bi.height, &bi, BoxBlurH32));
            ERR(iter_scopeP->iterate_generic(bi.width, &bi, BoxBlurV32));
        }
        break;
    }
    return err;
}

PF_Err NFBlue(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    A_long value
)
{
    return TinyBlueImpl(in_dataP, out_dataP, worldP, value);
}

PF_Err TinyBlueM(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scopeP,
    A_long value
)
{
    return TinyBlueMImpl(in_dataP, worldP, pixelFormat, iter_scopeP, value);
}