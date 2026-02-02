#include "NF_Mult.h"

typedef struct {
    PF_InData* in_data;
    PF_EffectWorld* world;
    A_long width;
    A_long height;
    A_long rowbytes;
} MultInfo;

#ifndef AE_CLAMP
#define AE_CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (MIN) : ((VAL) > (MAX) ? (MAX) : (VAL)))
#endif


template <typename T, typename CH>
static PF_Err MultT(
    void* refconPV,
    A_long y,
    CH max_chan)
{
    PF_Err err = PF_Err_NONE;
    MultInfo* bi = static_cast<MultInfo*>(refconPV);

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
        PF_FpLong v = static_cast<PF_FpLong>(rowP[i].alpha) / max_chan;

        // クランプ処理
        //if (v < 0.0) v = 0.0; else if (v > 1.0) v = 1.0;

        // 乗算（32bit Floatの場合はそのまま、8/16bitは整数キャスト）
        /*
        rowP[i].red = static_cast<CH>(rowP[i].red * v);
        rowP[i].green = static_cast<CH>(rowP[i].green * v);
        rowP[i].blue = static_cast<CH>(rowP[i].blue * v);
        */
        PF_FpLong r = rowP[i].red * v;
        if (r < 0.0) r = 0.0; else if (r > max_chan) r = max_chan;
        PF_FpLong g = rowP[i].green * v;
        if (g < 0.0) g = 0.0; else if (g > max_chan) g = max_chan;
        PF_FpLong b = rowP[i].blue * v;
        if (b < 0.0) b = 0.0; else if (b > max_chan) b = max_chan;

        rowP[i].red = static_cast<CH>(r);
        rowP[i].green = static_cast<CH>(g);
        rowP[i].blue = static_cast<CH>(b);
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
    MultInfo* bi = static_cast<MultInfo*>(refconPV);

    // 行ポインタの取得 (バイト計算後に型キャスト)
    T* rowP = reinterpret_cast<T*>(reinterpret_cast<char*>(bi->world->data) + (y * bi->rowbytes));

    for (A_long i = 0; i < bi->world->width; i++) {
        // アルファが0ならRGBを0にする
        if (rowP[i].alpha <= 0) {
            rowP[i].red = 0;
            rowP[i].green = 0;
            rowP[i].blue = 0;
            continue;
        }
        // アルファが最大値ならそのまま
        else if (rowP[i].alpha >= max_chan) {
            continue;
        }

        // 修正: 浮動小数点除算を行う（先にキャスト）
        PF_FpLong v = static_cast<PF_FpLong>(max_chan) / static_cast<PF_FpLong>(rowP[i].alpha);

        PF_FpLong r = rowP[i].red * v;
        if (r < 0.0) r = 0.0; else if (r > max_chan) r = max_chan;
        PF_FpLong g = rowP[i].green * v;
        if (g < 0.0) g = 0.0; else if (g > max_chan) g = max_chan;
        PF_FpLong b = rowP[i].blue * v;
        if (b < 0.0) b = 0.0; else if (b > max_chan) b = max_chan;

        rowP[i].red = static_cast<CH>(r);
        rowP[i].green = static_cast<CH>(g);
        rowP[i].blue = static_cast<CH>(b);
    }

    return err;
}
// 8-bit用
static PF_Err UnMultSub8(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultT<PF_Pixel8, A_u_char>(refcon, y, PF_MAX_CHAN8);
}

// 16-bit用
static PF_Err UnMultSub16(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultT<PF_Pixel16, A_u_short>(refcon, y, PF_MAX_CHAN16);
}

// 32-bit用 (Float)
static PF_Err UnMultSubFloat(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    // 32bitの場合、max_chanは 1.0f
    return UnMultT<PF_PixelFloat, PF_FpShort>(refcon, y, 1.0f);
}

PF_Err UnMult8(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultSub8(refcon, thread_idx, y, itrt);
}
PF_Err UnMult16(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultSub16(refcon, thread_idx, y, itrt);
}
PF_Err UnMult32(void* refcon, A_long thread_idx, A_long y, A_long itrt) {
    return UnMultSubFloat(refcon, thread_idx, y, itrt);
}



static PF_Err MultImpl(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
	PF_Boolean isUnMult
)
{
    PF_Err err = PF_Err_NONE;
 
    MultInfo bi;
    bi.in_data = in_dataP;
    bi.world = worldP;
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
        if (isUnMult) {
            ERR(iter_scope->iterate_generic(worldP->height, &bi, UnMultSub8));
        }
        else {
            ERR(iter_scope->iterate_generic(worldP->height, &bi, Mult8));
		}
        break;

    case PF_PixelFormat_ARGB64:
        if (isUnMult) {
            ERR(iter_scope->iterate_generic(worldP->height, &bi, UnMultSub16));
        }
        else {
            ERR(iter_scope->iterate_generic(worldP->height, &bi, Mult16));
        }
        break;

    case PF_PixelFormat_ARGB128:
        if (isUnMult) {
            ERR(iter_scope->iterate_generic(worldP->height, &bi, UnMultSubFloat));
        }
        else {
            ERR(iter_scope->iterate_generic(worldP->height, &bi, MultFloat));
        }
        break;
    }
    return err;
}
PF_Err Mult(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    PF_Boolean isUnMult
)
{
    return MultImpl(in_dataP, out_dataP, worldP, isUnMult);
}
static PF_Err MultImpl(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_Boolean isUnMult
)
{
    PF_Err err = PF_Err_NONE;
    MultInfo bi;
    bi.in_data = in_dataP;
    bi.world = worldP;
    bi.width = worldP->width;
    bi.height = worldP->height;
    bi.rowbytes = worldP->rowbytes;
    
    switch (pixelFormat) {
    case PF_PixelFormat_ARGB32:
        if (isUnMult) {
            ERR(suitesP->Iterate8Suite1()->iterate_generic(worldP->height, &bi, UnMultSub8));
        }
        else {
            ERR(suitesP->Iterate8Suite1()->iterate_generic(worldP->height, &bi, Mult8));
        }
        break;

    case PF_PixelFormat_ARGB64:
        if (isUnMult) {
            ERR(suitesP->Iterate8Suite1()->iterate_generic(worldP->height, &bi, UnMultSub16));
        }
        else {
            ERR(suitesP->Iterate8Suite1()->iterate_generic(worldP->height, &bi, Mult16));
        }
        break;

    case PF_PixelFormat_ARGB128:
        if (isUnMult) {
            ERR(suitesP->Iterate8Suite1()->iterate_generic(worldP->height, &bi, UnMultSubFloat));
        }
        else {
            ERR(suitesP->Iterate8Suite1()->iterate_generic(worldP->height, &bi, MultFloat));
        }
        break;
    }
    return err;
}


PF_Err Mult(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    PF_Boolean isUnMult
)
{
    return MultImpl(in_dataP,  worldP, pixelFormat, suitesP, isUnMult);
}