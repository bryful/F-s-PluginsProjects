#include "NF_ChannelBlur.h"
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
    A_long channel;  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
} BlurInfo;

#ifndef AE_CLAMP
#define AE_CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (MIN) : ((VAL) > (MAX) ? (MAX) : (VAL)))
#endif

/* --- 水平ボックスブラー (iterate_generic用) --- */
template <typename PixelType, A_long MaxChan>
static PF_Err
BoxBlurH(
    void* refconPV,
    A_long thread_idxL,
    A_long y,
    A_long itrtL)
{
    // この行を追加
    if (itrtL == PF_Iterations_ONCE_PER_PROCESSOR) {
        return PF_Err_NONE;
    }
    /*
    if (y == 0) {
        char dbg[256];
        BlurInfo* bi = static_cast<BlurInfo*>(refconPV);
        sprintf_s(dbg, "BoxBlurH: y=%d, channel=%d, radius=%d, width=%d\n",
            y, bi->channel, bi->radius, bi->width);
        OutputDebugStringA(dbg);
    }
    */
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
	if (bi->channel == 0) // Rチャンネルのみ
    {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, width - 1);
            sumR += (double)rowP[idx].red;
        }

        // スライディングウィンドウ処理（一時バッファに書き込む）
        for (A_long i = 0; i < width; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].red = static_cast<decltype(PixelType::red)>(AE_CLAMP(sumR / window, 0, MaxChan));

            // ウィンドウをスライド（元のデータから読む）
            A_long left = AE_CLAMP(i - r, 0, width - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, width - 1);

            sumR += ((double)rowP[right].red - (double)rowP[left].red);
        }

        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < width; i++) {
            rowP[i].red = tempP[i].red;
        }
    }
    else if (bi->channel == 1)
    {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, width - 1);
            sumG += (double)rowP[idx].green;
        }

        // スライディングウィンドウ処理（一時バッファに書き込む）
        for (A_long i = 0; i < width; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].green = static_cast<decltype(PixelType::green)>(AE_CLAMP(sumG / window, 0, MaxChan));

            // ウィンドウをスライド（元のデータから読む）
            A_long left = AE_CLAMP(i - r, 0, width - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, width - 1);

            sumG+= ((double)rowP[right].green - (double)rowP[left].green);
        }

        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < width; i++) {
            rowP[i].green = tempP[i].green;
        }
    }
    else if (bi->channel == 2) {
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, width - 1);
            sumB += (double)rowP[idx].blue;
        }

        // スライディングウィンドウ処理（一時バッファに書き込む）
        for (A_long i = 0; i < width; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].blue = static_cast<decltype(PixelType::blue)>(AE_CLAMP(sumB / window, 0, MaxChan));

            // ウィンドウをスライド（元のデータから読む）
            A_long left = AE_CLAMP(i - r, 0, width - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, width - 1);

            sumB += ((double)rowP[right].blue - (double)rowP[left].blue);
        }

        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < width; i++) {
            rowP[i].blue = tempP[i].blue;
        }
    }
    else if (bi->channel == 3) {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, width - 1);
            sumA += (double)rowP[idx].alpha;
        }

        // スライディングウィンドウ処理（一時バッファに書き込む）
        for (A_long i = 0; i < width; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].alpha = static_cast<decltype(PixelType::alpha)>(AE_CLAMP(sumA / window, 0, MaxChan));

            // ウィンドウをスライド（元のデータから読む）
            A_long left = AE_CLAMP(i - r, 0, width - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, width - 1);

            sumA += ((double)rowP[right].alpha - (double)rowP[left].alpha);
        }
        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < width; i++) {
            rowP[i].alpha = tempP[i].alpha;
        }
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
    // この行を追加
    if (itrtL == PF_Iterations_ONCE_PER_PROCESSOR) {
        return PF_Err_NONE;
    }
    /*
    if (x == 0) {
        char dbg[256];
        BlurInfo* bi = static_cast<BlurInfo*>(refconPV);
        sprintf_s(dbg, "BoxBlurV: x=%d, channel=%d, radius=%d, height=%d\n",
            x, bi->channel, bi->radius, bi->height);
        OutputDebugStringA(dbg);
    }
    */
    BlurInfo* bi = static_cast<BlurInfo*>(refconPV);
    A_long height = bi->height;
    A_long r = bi->radius;

    // rowbytes を使って正しいストライドを計算
    char* base = (char*)bi->world->data;

    // 一時バッファ（列方向）
    PixelType* tempP = static_cast<PixelType*>(bi->temp_buffer) + (x * height);

    double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    A_long window = (r * 2) + 1;
    if (bi->channel == 0) // Rチャンネルのみ
    {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, height - 1);
            PixelType* p = reinterpret_cast<PixelType*>(base + idx * bi->rowbytes) + x;
            sumR += (double)p->red;
        }

        // スライディングウィンドウ処理
        for (A_long i = 0; i < height; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].red = static_cast<decltype(PixelType::red)>(AE_CLAMP(sumR / window, 0, MaxChan));

            // ウィンドウをスライド
            A_long left = AE_CLAMP(i - r, 0, height - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, height - 1);

            PixelType* leftP = reinterpret_cast<PixelType*>(base + left * bi->rowbytes) + x;
            PixelType* rightP = reinterpret_cast<PixelType*>(base + right * bi->rowbytes) + x;

            sumR += ((double)rightP->red - (double)leftP->red);
        }

        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < height; i++) {
            PixelType* target = reinterpret_cast<PixelType*>(base + i * bi->rowbytes) + x;
            target->red = tempP[i].red;
        }
    }
    else if (bi->channel == 1) {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, height - 1);
            PixelType* p = reinterpret_cast<PixelType*>(base + idx * bi->rowbytes) + x;
            sumG += (double)p->green;
        }

        // スライディングウィンドウ処理
        for (A_long i = 0; i < height; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].green = static_cast<decltype(PixelType::green)>(AE_CLAMP(sumG / window, 0, MaxChan));

            // ウィンドウをスライド
            A_long left = AE_CLAMP(i - r, 0, height - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, height - 1);

            PixelType* leftP = reinterpret_cast<PixelType*>(base + left * bi->rowbytes) + x;
            PixelType* rightP = reinterpret_cast<PixelType*>(base + right * bi->rowbytes) + x;

            sumG += ((double)rightP->green - (double)leftP->green);
        }

        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < height; i++) {
            PixelType* target = reinterpret_cast<PixelType*>(base + i * bi->rowbytes) + x;
            target->green = tempP[i].green;
        }
    }

    else if (bi->channel == 2) {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, height - 1);
            PixelType* p = reinterpret_cast<PixelType*>(base + idx * bi->rowbytes) + x;
            sumB += (double)p->blue;
        }

        // スライディングウィンドウ処理
        for (A_long i = 0; i < height; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].blue = static_cast<decltype(PixelType::blue)>(AE_CLAMP(sumB / window, 0, MaxChan));

            // ウィンドウをスライド
            A_long left = AE_CLAMP(i - r, 0, height - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, height - 1);

            PixelType* leftP = reinterpret_cast<PixelType*>(base + left * bi->rowbytes) + x;
            PixelType* rightP = reinterpret_cast<PixelType*>(base + right * bi->rowbytes) + x;

            sumB += ((double)rightP->blue - (double)leftP->blue);
        }

        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < height; i++) {
            PixelType* target = reinterpret_cast<PixelType*>(base + i * bi->rowbytes) + x;
            target->blue = tempP[i].blue;
        }
    }
    else if (bi->channel == 3) {
        // 初期ウィンドウの計算
        for (A_long i = -r; i <= r; i++) {
            A_long idx = AE_CLAMP(i, 0, height - 1);
            PixelType* p = reinterpret_cast<PixelType*>(base + idx * bi->rowbytes) + x;
            sumA += (double)p->alpha;
        }

        // スライディングウィンドウ処理
        for (A_long i = 0; i < height; i++) {
            // 結果を一時バッファに書き込む
            tempP[i].alpha = static_cast<decltype(PixelType::alpha)>(AE_CLAMP(sumA / window, 0, MaxChan));

            // ウィンドウをスライド
            A_long left = AE_CLAMP(i - r, 0, height - 1);
            A_long right = AE_CLAMP(i + r + 1, 0, height - 1);

            PixelType* leftP = reinterpret_cast<PixelType*>(base + left * bi->rowbytes) + x;
            PixelType* rightP = reinterpret_cast<PixelType*>(base + right * bi->rowbytes) + x;

            sumA += ((double)rightP->alpha - (double)leftP->alpha);
         }
        // 一時バッファから元のバッファにコピー
        for (A_long i = 0; i < height; i++) {
            PixelType* target = reinterpret_cast<PixelType*>(base + i * bi->rowbytes) + x;
            target->alpha = tempP[i].alpha;
        }

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


PF_Err ChannelBlur(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    A_long value,
    A_long channel  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
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
    
    size_t buffer_size = max(worldP->width * worldP->height, worldP->height * worldP->width) * pixel_size;
    std::vector<char> temp_buffer(buffer_size);

    BlurInfo bi;
    bi.in_data = in_dataP;
    bi.world = worldP;
    bi.radius = value;
    bi.width = worldP->width;
    bi.height = worldP->height;
    bi.rowbytes = worldP->rowbytes;
    bi.temp_buffer = temp_buffer.data();
	bi.channel = channel;
 

    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        for (int n = 0; n < 3; n++) {
            char msg[256];  // 変数を宣言
            ERR(suitesP->Iterate8Suite2()->iterate_generic(bi.height, &bi, BoxBlurH8));
            sprintf_s(msg, "BoxBlurH8: err=%d\n", err);
            OutputDebugStringA(msg);

            ERR(suitesP->Iterate8Suite2()->iterate_generic(bi.width, &bi, BoxBlurV8));
            sprintf_s(msg, "BoxBlurV8: err=%d\n", err);
            OutputDebugStringA(msg);
        }
        break;

    case PF_PixelFormat_ARGB64:
        for (int n = 0; n < 3; n++) {
            ERR(suitesP->Iterate8Suite2()->iterate_generic(bi.height, &bi, BoxBlurH16));
            ERR(suitesP->Iterate8Suite2()->iterate_generic(bi.width, &bi, BoxBlurV16));
        }
        break;

    case PF_PixelFormat_ARGB128:
        for (int n = 0; n < 3; n++) {
            ERR(suitesP->Iterate8Suite2()->iterate_generic(bi.height, &bi, BoxBlurH32));
            ERR(suitesP->Iterate8Suite2()->iterate_generic(bi.width, &bi, BoxBlurV32));
        }
        break;
    }
    return err;
}

