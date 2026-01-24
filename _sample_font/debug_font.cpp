#include "debug_font.h"
#include "debug_font_1424.h"

#define FONT_WIDTH_ 14
#define FONT_HEIGHT_ 24
#define FONT_WIDTH_OFFSET_ 1
#define _CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

inline PF_Pixel16 _PX_8to16(PF_Pixel c)
{
    PF_Pixel16 r;
    r.alpha = (A_u_short)((((A_long)(c.alpha)*PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8);
    r.red = (A_u_short)((((A_long)(c.red) * PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8);
    r.green = (A_u_short)((((A_long)(c.green) * PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8);
    r.blue = (A_u_short)((((A_long)(c.blue) * PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8);
    return r;
}
inline PF_PixelFloat _PX_8to32(PF_Pixel c)
{
    PF_PixelFloat r;
    r.alpha = (PF_FpShort)c.alpha/PF_MAX_CHAN8;
    r.red = (PF_FpShort)c.red/PF_MAX_CHAN8;
    r.green = (PF_FpShort)c.green / PF_MAX_CHAN8;
    r.blue = (PF_FpShort)c.blue / PF_MAX_CHAN8;
    return r;
}
/**
 * テンプレートによる共通描画ロジック
 * T: PF_Pixel8, PF_Pixel16, PF_PixelFloat のいずれか
 */
template <typename T>
static void DrawDebugCharT(
    PF_EffectWorld* world,
    int x_off,
    int y_off,
    unsigned char c,
    const T* color)
{
    if (c < 0x21 || c > 0x7E) return;
    const unsigned short* glyph = debug_font_1424[c - 0x21];

    for (int y = 0; y < FONT_HEIGHT_; y++) {
        int ty = y_off + y;
        if (ty < 0 || ty >= world->height) continue;

        // 行の先頭ポインタをバイト(char*)で取得
        char* row_base = (char*)world->data + (ty * world->rowbytes);
        // T 型のポインタに変換（まだオフセットは加えない）
        T* p_row = reinterpret_cast<T*>(row_base);


        unsigned short row_data = glyph[y];

        for (int x = 0; x < FONT_WIDTH_; x++) {
            if (row_data & (0x8000 >> x)) {
                int tx = x_off + x;
                if (tx >= 0 && tx < world->width) {
                    p_row[tx] = *color;  // x_off + x の位置に書き込む
                }
            }
        }
    }
}
/**
 * 深度を判別して描画を振り分けるエントリーポイント
 */
static void DrawDebugStringImple(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    int x,
    int y,
    const char* str,
    PF_Pixel color
)
{
    PF_WorldSuite2* ws2P;
    PF_PixelFormat pf;
    AEFX_AcquireSuite(in_dataP,
        out_dataP,
        kPFWorldSuite,
        kPFWorldSuiteVersion2,
        "Couldn't load suite.",
        (void**)&(ws2P));
    ws2P->PF_GetPixelFormat(worldP, &pf);

    switch (pf)
    {
    case PF_PixelFormat_ARGB32:
    {
        while (*str) {
            if (*str != ' ') DrawDebugCharT<PF_Pixel8>(worldP, x, y, *str, &color);
            x += FONT_WIDTH_ - FONT_WIDTH_OFFSET_;
            str++;
        }
        break;
    }
    case PF_PixelFormat_ARGB64:
    {
        PF_Pixel16 color16 = _PX_8to16(color);
        while (*str) {
            if (*str != ' ') DrawDebugCharT<PF_Pixel16>(worldP, x, y, *str, &color16);
            x += FONT_WIDTH_- FONT_WIDTH_OFFSET_;
            str++;
        }
        break;
    }
    case PF_PixelFormat_ARGB128:
    {
        PF_PixelFloat color32 = _PX_8to32(color);  // _PX_8to32 を使用
        while (*str) {
            if (*str != ' ') DrawDebugCharT<PF_PixelFloat>(worldP, x, y, *str, &color32);
            x += FONT_WIDTH_ - FONT_WIDTH_OFFSET_;  // FONT_WIDTH_ に変更（一貫性のため）
            str++;
        }
        break;
    }
    default:
        break;
    }


}


void DrawDebugString(
    PF_InData* in_dataP,
    PF_OutData* out_dataP,
    PF_EffectWorld* worldP,
    int x,
    int y,
    const char* str,
    PF_Pixel color
)
{
	DrawDebugStringImple(in_dataP, out_dataP, worldP, x, y, str, color);
}