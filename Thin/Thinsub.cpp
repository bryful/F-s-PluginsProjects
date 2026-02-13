#pragma once
#include "Thin.h"
#include <thread>
#include <vector>
#include <algorithm>
//-------------------------------------------------------
typedef struct thinInfo8 {
    A_long		w;
    A_long		wt;
    A_long		wt2;
    A_long		h;
    A_long		offset;
    PF_Handle	scanlineH;
    PF_Pixel* scanline;
    PF_Pixel* data;

    PF_Boolean	white;
    PF_Boolean	alphaZero;
    PF_Boolean	edge;

    A_long		target_color_count;
    PF_Pixel	color8[TARGET_COLOR_MAX];
    A_long	    level;
    A_long		nowX;
    A_long		nowY;

} thinInfo8;
typedef struct thinBak8 {
    PF_Boolean	drawFlag;
    PF_Pixel	color;
    A_long		dir;
} thinBak8;

typedef struct thinInfo16 {
    A_long		w;
    A_long		wt;
    A_long		wt2;
    A_long		h;
    A_long		offset;
    PF_Handle	scanlineH;
    PF_Pixel16* scanline;
    PF_Pixel16* data;

    PF_Boolean	white;
    PF_Boolean	alphaZero;
    PF_Boolean	edge;

    A_long		target_color_count;
    PF_Pixel	color8[TARGET_COLOR_MAX];
    A_long		level;
    A_long		nowX;
    A_long		nowY;

} thinInfo16;

typedef struct thinBak16 {
    PF_Boolean	drawFlag;
    PF_Pixel16	color;
    A_long		dir;
} thinBak16;

typedef struct thinInfo32 {
    A_long		w;
    A_long		wt;
    A_long		wt2;
    A_long		h;
    A_long		offset;
    PF_Handle	scanlineH;
    PF_PixelFloat* scanline;
    PF_PixelFloat* data;

    PF_Boolean	white;
    PF_Boolean	alphaZero;
    PF_Boolean	edge;

    A_long		target_color_count;
    PF_Pixel	color8[TARGET_COLOR_MAX];
    A_long		level;
    A_long		nowX;
    A_long		nowY;

} thinInfo32;
typedef struct thinBak32 {
    PF_Boolean	drawFlag;
    PF_PixelFloat	color;
    A_long		dir;
} thinBak32;


// ピクセルタイプトレイト
template<typename PixelType>
struct PixelTraits;

template<>
struct PixelTraits<PF_Pixel> {
    typedef thinInfo8 InfoType;
    typedef thinBak8 BakType;
    static constexpr A_u_char MAX_CHANNEL = PF_MAX_CHAN8;

    static PF_Pixel ConvertFrom8bit(const PF_Pixel& p) { return p; }
    static PF_Pixel ConvertTo8bit(const PF_Pixel& p) { return p; }
};

template<>
struct PixelTraits<PF_Pixel16> {
    typedef thinInfo16 InfoType;
    typedef thinBak16 BakType;
    static constexpr A_u_short MAX_CHANNEL = PF_MAX_CHAN16;

    static PF_Pixel16 ConvertFrom8bit(const PF_Pixel& p) {
        return NF_Pixel8To16(p);
    }
    static PF_Pixel ConvertTo8bit(const PF_Pixel16& p) {
        return NF_Pixel16To8(p);
    }
};

template<>
struct PixelTraits<PF_PixelFloat> {
    typedef thinInfo32 InfoType;
    typedef thinBak32 BakType;
    static constexpr float MAX_CHANNEL = 1.0f;

    static PF_PixelFloat ConvertFrom8bit(const PF_Pixel& p) {
        return NF_Pixel8To32(p);
    }
    static PF_Pixel ConvertTo8bit(const PF_PixelFloat& p) {
        return NF_Pixel32To8(p);
    }
};

//*****************************************************************************
template<typename PixelType>
inline PF_Boolean CompPx(typename PixelTraits<PixelType>::InfoType* ti, PixelType p)
{
    PF_Boolean ret = FALSE;
    PF_Pixel p8 = PixelTraits<PixelType>::ConvertTo8bit(p);

    for (A_long i = 0; i < ti->target_color_count; i++) {
        if ((F_ABS((A_long)ti->color8[i].red - (A_long)p8.red) <= ti->level)
            && (F_ABS((A_long)ti->color8[i].green - (A_long)p8.green) <= ti->level)
            && (F_ABS((A_long)ti->color8[i].blue - (A_long)p8.blue) <= ti->level)) {
            ret = TRUE;
            break;
        }
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
inline PF_Boolean CompPxs(typename PixelTraits<PixelType>::InfoType* ti,
    PixelType p1, PixelType p2, PixelType p3)
{
    PF_Boolean ret = FALSE;
    PF_Pixel pp1 = PixelTraits<PixelType>::ConvertTo8bit(p1);
    PF_Pixel pp2 = PixelTraits<PixelType>::ConvertTo8bit(p2);
    PF_Pixel pp3 = PixelTraits<PixelType>::ConvertTo8bit(p3);

    if ((F_ABS((A_long)pp1.red - (A_long)pp2.red) <= ti->level)
        && (F_ABS((A_long)pp1.green - (A_long)pp2.green) <= ti->level)
        && (F_ABS((A_long)pp1.blue - (A_long)pp2.blue) <= ti->level)) {
        if ((F_ABS((A_long)pp1.red - (A_long)pp3.red) <= ti->level)
            && (F_ABS((A_long)pp1.green - (A_long)pp3.green) <= ti->level)
            && (F_ABS((A_long)pp1.blue - (A_long)pp3.blue) <= ti->level)) {
            ret = TRUE;
        }
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
inline PF_Boolean CompPxs2(typename PixelTraits<PixelType>::InfoType* ti,
    PixelType p1, PixelType p2)
{
    PF_Boolean ret = FALSE;
    PF_Pixel pp1 = PixelTraits<PixelType>::ConvertTo8bit(p1);
    PF_Pixel pp2 = PixelTraits<PixelType>::ConvertTo8bit(p2);

    if ((F_ABS((A_long)pp1.red - (A_long)pp2.red) <= ti->level)
        && (F_ABS((A_long)pp1.green - (A_long)pp2.green) <= ti->level)
        && (F_ABS((A_long)pp1.blue - (A_long)pp2.blue) <= ti->level)) {
        ret = TRUE;
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
inline void scanlineCopy(typename PixelTraits<PixelType>::InfoType* ti, A_long y)
{
    A_long v = 0;
    if (y == 0) {
        for (int i = 0; i < ti->w; i++) {
            ti->scanline[i] = ti->data[i];
            ti->scanline[i + ti->wt] = ti->data[i];
            ti->scanline[i + ti->wt2] = ti->data[i + ti->wt];
        }
    }
    else {
        for (int i = 0; i < ti->w; i++) {
            ti->scanline[i] = ti->scanline[i + ti->wt];
            ti->scanline[i + ti->wt] = ti->scanline[i + ti->wt2];
        }
        if (y < ti->h - 1) {
            v = (y + 1) * ti->wt;
            for (int i = 0; i < ti->w; i++) {
                ti->scanline[i + ti->wt2] = ti->data[i + v];
            }
        }
    }
}

//*****************************************************************************
// マルチスレッド対応: スレッドローカルなscanlineCopy
//*****************************************************************************
template<typename PixelType>
inline void scanlineCopyLocal(typename PixelTraits<PixelType>::InfoType* ti,
    PixelType* localScanline, A_long y)
{
    A_long v = 0;
    if (y == 0) {
        for (int i = 0; i < ti->w; i++) {
            localScanline[i] = ti->data[i];
            localScanline[i + ti->wt] = ti->data[i];
            localScanline[i + ti->wt2] = ti->data[i + ti->wt];
        }
    }
    else {
        for (int i = 0; i < ti->w; i++) {
            localScanline[i] = localScanline[i + ti->wt];
            localScanline[i + ti->wt] = localScanline[i + ti->wt2];
        }
        if (y < ti->h - 1) {
            v = (y + 1) * ti->wt;
            for (int i = 0; i < ti->w; i++) {
                localScanline[i + ti->wt2] = ti->data[i + v];
            }
        }
    }
}

//*****************************************************************************
template<typename PixelType>
inline PixelType getScanLine(typename PixelTraits<PixelType>::InfoType* ti,
    A_long offsetX, A_long offsetY)
{
    A_long xx = ti->nowX + offsetX;
    A_long yy = 1 + offsetY;
    if (xx < 0) {
        xx = 0;
    }
    else if (xx >= ti->w) {
        xx = ti->w - 1;
    }
    if (yy < 0) {
        yy = 0;
    }
    else if (yy > 2) {
        yy = 2;
    }
    return ti->scanline[xx + yy * ti->wt];
}

//*****************************************************************************
// マルチスレッド対応: スレッドローカルなgetScanLine（nowX, nowYをパラメータとして受け取る）
//*****************************************************************************
template<typename PixelType>
inline PixelType getScanLineLocal(typename PixelTraits<PixelType>::InfoType* ti,
    PixelType* localScanline, A_long nowX, A_long offsetX, A_long offsetY)
{
    A_long xx = nowX + offsetX;
    A_long yy = 1 + offsetY;
    if (xx < 0) {
        xx = 0;
    }
    else if (xx >= ti->w) {
        xx = ti->w - 1;
    }
    if (yy < 0) {
        yy = 0;
    }
    else if (yy > 2) {
        yy = 2;
    }
    return localScanline[xx + yy * ti->wt];
}

//*****************************************************************************
template<typename PixelType>
inline PF_Boolean getData(typename PixelTraits<PixelType>::InfoType* ti,
    A_long offsetX, A_long offsetY)
{
    A_long xx = ti->nowX + offsetX;
    A_long yy = ti->nowY + offsetY;
    if (xx < 0) {
        xx = 0;
    }
    else if (xx >= ti->w) {
        xx = ti->w - 1;
    }
    if (yy < 0) {
        yy = 0;
    }
    else if (yy >= ti->h) {
        yy = ti->h - 1;
    }
    PixelType p = ti->data[xx + yy * ti->wt];
    return CompPx<PixelType>(ti, p);
}

//*****************************************************************************
// マルチスレッド対応: getDataLocal（nowX, nowYをパラメータとして受け取る）
//*****************************************************************************
template<typename PixelType>
inline PF_Boolean getDataLocal(typename PixelTraits<PixelType>::InfoType* ti,
    A_long nowX, A_long nowY, A_long offsetX, A_long offsetY)
{
    A_long xx = nowX + offsetX;
    A_long yy = nowY + offsetY;
    if (xx < 0) {
        xx = 0;
    }
    else if (xx >= ti->w) {
        xx = ti->w - 1;
    }
    if (yy < 0) {
        yy = 0;
    }
    else if (yy >= ti->h) {
        yy = ti->h - 1;
    }
    PixelType p = ti->data[xx + yy * ti->wt];
    return CompPx<PixelType>(ti, p);
}

//*****************************************************************************
template<typename PixelType>
inline PixelType minColor(PixelType s, PixelType d)
{
    PF_Pixel s8 = PixelTraits<PixelType>::ConvertTo8bit(s);
    PF_Pixel d8 = PixelTraits<PixelType>::ConvertTo8bit(d);

    A_long ss = (A_long)s8.red + (A_long)s8.green + (A_long)s8.blue;
    A_long dd = (A_long)d8.red + (A_long)d8.green + (A_long)d8.blue;

    if (ss < dd) {
        return s;
    }
    else {
        return d;
    }
}

//*****************************************************************************
// ピクセルスキップ判定（高速化用）
//*****************************************************************************
template<typename PixelType>
inline PF_Boolean shouldSkipPixel(typename PixelTraits<PixelType>::InfoType* ti, const PixelType& color)
{
    if (!ti->white && !ti->alphaZero) return FALSE;

    PF_Pixel c8 = PixelTraits<PixelType>::ConvertTo8bit(color);

    if (ti->white) {
        if ((c8.blue == PF_MAX_CHAN8) && (c8.green == PF_MAX_CHAN8)
            && (c8.red == PF_MAX_CHAN8) && (c8.alpha == PF_MAX_CHAN8)) {
            return TRUE;
        }
    }
    if (ti->alphaZero) {
        if (c8.alpha == 0) return TRUE;
    }
    return FALSE;
}

//*****************************************************************************
template<typename PixelType>
inline typename PixelTraits<PixelType>::BakType getPatEdge(typename PixelTraits<PixelType>::InfoType* ti)
{
    typedef typename PixelTraits<PixelType>::BakType ThinBak;
    ThinBak ret;
    ret.drawFlag = FALSE;

    PixelType table[8];
    table[0] = getScanLine<PixelType>(ti, -1, -1);
    table[1] = getScanLine<PixelType>(ti, 0, -1);
    table[2] = getScanLine<PixelType>(ti, 1, -1);
    table[3] = getScanLine<PixelType>(ti, -1, 0);
    table[4] = getScanLine<PixelType>(ti, 1, 0);
    table[5] = getScanLine<PixelType>(ti, -1, 1);
    table[6] = getScanLine<PixelType>(ti, 0, 1);
    table[7] = getScanLine<PixelType>(ti, 1, 1);

    A_long pat = 0;
    A_long patCount = 0;
    for (A_long i = 0; i < 8; i++) {
        pat <<= 1;
        if (CompPx<PixelType>(ti, table[i])) {
            pat = pat | 1;
            patCount++;
        }
    }

    if ((pat == 0x8F) || (pat == 0x8E)) {
        if (CompPxs<PixelType>(ti, table[1], table[2], table[3]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
        return ret;
    }
    else if ((pat == 0x6D) || (pat == 0x4D)) {
        if (CompPxs<PixelType>(ti, table[0], table[3], table[6]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[0];
        }
        return ret;
    }
    else if ((pat == 0xF1) || (pat == 0x71)) {
        if (CompPxs<PixelType>(ti, table[4], table[5], table[6]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[4];
        }
        return ret;
    }
    else if ((pat == 0x86) || (pat == 0xB2)) {
        if (CompPxs<PixelType>(ti, table[1], table[4], table[7]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
        return ret;
    }
    else if ((pat == 0x35) || (pat == 0x33)) {
        if (CompPxs<PixelType>(ti, table[0], table[1], table[4]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[0];
        }
        return ret;
    }
    else if ((pat == 0xD5) || (pat == 0x55)) {
        if (CompPxs<PixelType>(ti, table[2], table[4], table[6]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[2];
        }
        return ret;
    }
    else if ((pat == 0xEC) || (pat == 0xCC)) {
        if (CompPxs<PixelType>(ti, table[3], table[6], table[7]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[3];
        }
        return ret;
    }
    else if ((pat == 0xAB) || (pat == 0xAA)) {
        if (CompPxs<PixelType>(ti, table[1], table[3], table[5]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
        return ret;
    }
    else if ((pat == 0xED) || (pat == 0xCD)) {
        if (CompPxs2<PixelType>(ti, table[3], table[6]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[3];
        }
        return ret;
    }
    else if ((pat == 0xAF) || (pat == 0xAE)) {
        if (CompPxs2<PixelType>(ti, table[1], table[3]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
        return ret;
    }
    else if ((pat == 0xB7) || (pat == 0xB3)) {
        if (CompPxs2<PixelType>(ti, table[1], table[4]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
        return ret;
    }
    else if ((pat == 0xF5) || (pat == 0x75)) {
        if (CompPxs2<PixelType>(ti, table[4], table[6]) == TRUE) {
            ret.drawFlag = TRUE;
            ret.color = table[4];
        }
    }
    return ret;
}

//*****************************************************************************
// マルチスレッド対応: getPatEdgeLocal
//*****************************************************************************
template<typename PixelType>
inline typename PixelTraits<PixelType>::BakType getPatEdgeLocal(
    typename PixelTraits<PixelType>::InfoType* ti, PixelType* localScanline, A_long nowX)
{
    typedef typename PixelTraits<PixelType>::BakType ThinBak;
    ThinBak ret;
    ret.drawFlag = FALSE;

    PixelType table[8];
    table[0] = getScanLineLocal<PixelType>(ti, localScanline, nowX, -1, -1);
    table[1] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 0, -1);
    table[2] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 1, -1);
    table[3] = getScanLineLocal<PixelType>(ti, localScanline, nowX, -1, 0);
    table[4] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 1, 0);
    table[5] = getScanLineLocal<PixelType>(ti, localScanline, nowX, -1, 1);
    table[6] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 0, 1);
    table[7] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 1, 1);

    A_long pat = 0;
    for (A_long i = 0; i < 8; i++) {
        pat <<= 1;
        if (CompPx<PixelType>(ti, table[i])) {
            pat = pat | 1;
        }
    }

    if ((pat == 0x8F) || (pat == 0x8E)) {
        if (CompPxs<PixelType>(ti, table[1], table[2], table[3])) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
    }
    else if ((pat == 0x6D) || (pat == 0x4D)) {
        if (CompPxs<PixelType>(ti, table[0], table[3], table[6])) {
            ret.drawFlag = TRUE;
            ret.color = table[0];
        }
    }
    else if ((pat == 0xF1) || (pat == 0x71)) {
        if (CompPxs<PixelType>(ti, table[4], table[5], table[6])) {
            ret.drawFlag = TRUE;
            ret.color = table[4];
        }
    }
    else if ((pat == 0x86) || (pat == 0xB2)) {
        if (CompPxs<PixelType>(ti, table[1], table[4], table[7])) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
    }
    else if ((pat == 0x35) || (pat == 0x33)) {
        if (CompPxs<PixelType>(ti, table[0], table[1], table[4])) {
            ret.drawFlag = TRUE;
            ret.color = table[0];
        }
    }
    else if ((pat == 0xD5) || (pat == 0x55)) {
        if (CompPxs<PixelType>(ti, table[2], table[4], table[6])) {
            ret.drawFlag = TRUE;
            ret.color = table[2];
        }
    }
    else if ((pat == 0xEC) || (pat == 0xCC)) {
        if (CompPxs<PixelType>(ti, table[3], table[6], table[7])) {
            ret.drawFlag = TRUE;
            ret.color = table[3];
        }
    }
    else if ((pat == 0xAB) || (pat == 0xAA)) {
        if (CompPxs<PixelType>(ti, table[1], table[3], table[5])) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
    }
    else if ((pat == 0xED) || (pat == 0xCD)) {
        if (CompPxs2<PixelType>(ti, table[3], table[6])) {
            ret.drawFlag = TRUE;
            ret.color = table[3];
        }
    }
    else if ((pat == 0xAF) || (pat == 0xAE)) {
        if (CompPxs2<PixelType>(ti, table[1], table[3])) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
    }
    else if ((pat == 0xB7) || (pat == 0xB3)) {
        if (CompPxs2<PixelType>(ti, table[1], table[4])) {
            ret.drawFlag = TRUE;
            ret.color = table[1];
        }
    }
    else if ((pat == 0xF5) || (pat == 0x75)) {
        if (CompPxs2<PixelType>(ti, table[4], table[6])) {
            ret.drawFlag = TRUE;
            ret.color = table[4];
        }
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
inline typename PixelTraits<PixelType>::BakType getPatDot(typename PixelTraits<PixelType>::InfoType* ti)
{
    typedef typename PixelTraits<PixelType>::BakType ThinBak;
    ThinBak ret;
    ret.drawFlag = FALSE;

    PixelType table[8];
    table[0] = getScanLine<PixelType>(ti, -1, -1);
    table[1] = getScanLine<PixelType>(ti, 0, -1);
    table[2] = getScanLine<PixelType>(ti, 1, -1);
    table[3] = getScanLine<PixelType>(ti, -1, 0);
    table[4] = getScanLine<PixelType>(ti, 1, 0);
    table[5] = getScanLine<PixelType>(ti, -1, 1);
    table[6] = getScanLine<PixelType>(ti, 0, 1);
    table[7] = getScanLine<PixelType>(ti, 1, 1);

    A_long pat = 0;
    A_long patCount = 0;
    for (A_long i = 0; i < 8; i++) {
        pat <<= 1;
        if (CompPx<PixelType>(ti, table[i])) {
            pat = pat | 1;
            patCount++;
        }
    }

    if (pat == 0XE0) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[3], minColor<PixelType>(table[4], table[6]));
        return ret;
    }
    else if (pat == 0X29) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[1], minColor<PixelType>(table[3], table[6]));
        return ret;
    }
    else if (pat == 0X07) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[1], minColor<PixelType>(table[3], table[4]));
        return ret;
    }
    else if (pat == 0X94) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[1], minColor<PixelType>(table[4], table[6]));
        return ret;
    }
    return ret;
}

//*****************************************************************************
// マルチスレッド対応: getPatDotLocal
//*****************************************************************************
template<typename PixelType>
inline typename PixelTraits<PixelType>::BakType getPatDotLocal(
    typename PixelTraits<PixelType>::InfoType* ti, PixelType* localScanline, A_long nowX)
{
    typedef typename PixelTraits<PixelType>::BakType ThinBak;
    ThinBak ret;
    ret.drawFlag = FALSE;

    PixelType table[8];
    table[0] = getScanLineLocal<PixelType>(ti, localScanline, nowX, -1, -1);
    table[1] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 0, -1);
    table[2] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 1, -1);
    table[3] = getScanLineLocal<PixelType>(ti, localScanline, nowX, -1, 0);
    table[4] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 1, 0);
    table[5] = getScanLineLocal<PixelType>(ti, localScanline, nowX, -1, 1);
    table[6] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 0, 1);
    table[7] = getScanLineLocal<PixelType>(ti, localScanline, nowX, 1, 1);

    A_long pat = 0;
    for (A_long i = 0; i < 8; i++) {
        pat <<= 1;
        if (CompPx<PixelType>(ti, table[i])) {
            pat = pat | 1;
        }
    }

    if (pat == 0XE0) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[3], minColor<PixelType>(table[4], table[6]));
        return ret;
    }
    else if (pat == 0X29) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[1], minColor<PixelType>(table[3], table[6]));
        return ret;
    }
    else if (pat == 0X07) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[1], minColor<PixelType>(table[3], table[4]));
        return ret;
    }
    else if (pat == 0X94) {
        ret.drawFlag = TRUE;
        ret.color = minColor<PixelType>(table[1], minColor<PixelType>(table[4], table[6]));
        return ret;
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
inline typename PixelTraits<PixelType>::BakType getPatDot2nd(typename PixelTraits<PixelType>::InfoType* ti)
{
    typedef typename PixelTraits<PixelType>::BakType ThinBak;
    ThinBak ret;
    ret.drawFlag = FALSE;
    ret.dir = -1;

    PixelType table[8];
    table[0] = getScanLine<PixelType>(ti, -1, -1);
    table[1] = getScanLine<PixelType>(ti, 0, -1);
    table[2] = getScanLine<PixelType>(ti, 1, -1);
    table[3] = getScanLine<PixelType>(ti, -1, 0);
    table[4] = getScanLine<PixelType>(ti, 1, 0);
    table[5] = getScanLine<PixelType>(ti, -1, 1);
    table[6] = getScanLine<PixelType>(ti, 0, 1);
    table[7] = getScanLine<PixelType>(ti, 1, 1);

    A_long pat = 0;
    A_long patCount = 0;
    for (A_long i = 0; i < 8; i++) {
        pat <<= 1;
        if (CompPx<PixelType>(ti, table[i])) {
            pat = pat | 1;
            patCount++;
        }
    }

    if ((pat & 0x1F) == 0X1D) {
        ret.drawFlag = TRUE;
        ret.color = table[3];
        ret.dir = 2;
        return ret;
    }
    else if ((pat & 0xD6) == 0XC6) {
        ret.drawFlag = TRUE;
        ret.color = table[0];
        ret.dir = 3;
        return ret;
    }
    else if ((pat & 0xF8) == 0XB8) {
        ret.drawFlag = TRUE;
        ret.color = table[0];
        ret.dir = 0;
        return ret;
    }
    else if ((pat & 0x6B) == 0X63) {
        ret.drawFlag = TRUE;
        ret.color = table[1];
        ret.dir = 1;
        return ret;
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
inline typename PixelTraits<PixelType>::BakType getPat(typename PixelTraits<PixelType>::InfoType* ti)
{
    typedef typename PixelTraits<PixelType>::BakType ThinBak;
    ThinBak ret;
    ret.drawFlag = FALSE;

    PixelType table[8];
    table[0] = getScanLine<PixelType>(ti, -1, -1);
    table[1] = getScanLine<PixelType>(ti, 0, -1);
    table[2] = getScanLine<PixelType>(ti, 1, -1);
    table[3] = getScanLine<PixelType>(ti, -1, 0);
    table[4] = getScanLine<PixelType>(ti, 1, 0);
    table[5] = getScanLine<PixelType>(ti, -1, 1);
    table[6] = getScanLine<PixelType>(ti, 0, 1);
    table[7] = getScanLine<PixelType>(ti, 1, 1);

    A_long pat = 0;
    A_long patCount = 0;
    for (A_long i = 0; i < 8; i++) {
        pat <<= 1;
        if (CompPx<PixelType>(ti, table[i])) {
            pat = pat | 1;
            patCount++;
        }
    }

    if (patCount > 7) {
        return ret;
    }
    else if (patCount <= 0) {
        ret.drawFlag = TRUE;
        PixelType c = minColor<PixelType>(table[1], table[3]);
        c = minColor<PixelType>(c, table[4]);
        c = minColor<PixelType>(c, table[6]);
        ret.color = c;
        return ret;
    }
    else if (patCount == 7) {
        if (pat == 0xBF) {
            if (getData<PixelType>(ti, 0, 1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[1];
            }
            return ret;
        }
        if (pat == 0xEF) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[3];
            }
            return ret;
        }
        if (pat == 0xFD) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[6];
            }
            return ret;
        }
        if (pat == 0x7F) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[4];
            }
            return ret;
        }
    }
    else if (patCount == 6) {
        if ((pat == 0x9F) || (pat == 0x3F)) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[1];
            }
            return ret;
        }
        else if ((pat == 0x6F) || (pat == 0xEB)) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[3];
            }
            return ret;
        }
        else if ((pat == 0xF9) || (pat == 0xFC)) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[6];
            }
            return ret;
        }
        else if ((pat == 0xF6) || (pat == 0xD7)) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[4];
            }
            return ret;
        }
    }
    else if (patCount == 5) {
        if (pat == 0xF8) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[6];
            }
            return ret;
        }
        if (pat == 0x1F) {
            if (getData<PixelType>(ti, 0, 1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[1];
            }
            return ret;
        }
        else if (pat == 0x6B) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[3];
            }
            return ret;
        }
        else if (pat == 0xD6) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[4];
            }
            return ret;
        }
        else if (pat == 0x97) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[1], table[4]);
            return ret;
        }
        else if (pat == 0x2F) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[1], table[3]);
            return ret;
        }
        else if (pat == 0xE9) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[3], table[6]);
            return ret;
        }
        else if (pat == 0xF4) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[4], table[6]);
            return ret;
        }
        else if (pat == 0x5D) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[6];
            }
            return ret;
        }
        else if (pat == 0x73) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[4];
            }
            return ret;
        }
        else if (pat == 0xBA) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[1];
            }
            return ret;
        }
        else if (pat == 0xCE) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = table[3];
            }
            return ret;
        }
    }
    else if (patCount == 4) {
        if ((pat == 0x0F) || (pat == 0x2B)) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[1], table[3]);
            return ret;
        }
        else if ((pat == 0x69) || (pat == 0xE8)) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[3], table[6]);
            return ret;
        }
        else if ((pat == 0xF0) || (pat == 0xD4)) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[4], table[6]);
            return ret;
        }
        else if ((pat == 0x96) || (pat == 0x17)) {
            ret.drawFlag = TRUE;
            ret.color = minColor<PixelType>(table[1], table[4]);
            return ret;
        }
        else if (pat == 0xC9) {
            if ((getData<PixelType>(ti, 0, -1) == TRUE) && (getData<PixelType>(ti, 1, 0) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[3], table[6]);
            }
            return ret;
        }
        else if (pat == 0x74) {
            if ((getData<PixelType>(ti, 0, -1) == TRUE) && (getData<PixelType>(ti, -1, 0) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[4], table[6]);
            }
            return ret;
        }
        else if (pat == 0x93) {
            if ((getData<PixelType>(ti, -1, 0) == TRUE) && (getData<PixelType>(ti, 0, 1) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[4]);
            }
            return ret;
        }
        else if (pat == 0x2E) {
            if ((getData<PixelType>(ti, 1, 0) == TRUE) && (getData<PixelType>(ti, 0, 1) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[3]);
            }
            return ret;
        }
    }
    else if (patCount == 3) {
        if (pat == 0xE0) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(minColor<PixelType>(table[3], table[4]), table[6]);
            }
            return ret;
        }
        else if (pat == 0x94) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(minColor<PixelType>(table[1], table[4]), table[6]);
            }
            return ret;
        }
        else if (pat == 0x07) {
            if (getData<PixelType>(ti, 0, 1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(minColor<PixelType>(table[1], table[3]), table[4]);
            }
            return ret;
        }
        else if (pat == 0x29) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(minColor<PixelType>(table[1], table[3]), table[6]);
            }
            return ret;
        }
        else if (pat == 0x70) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[4], table[6]);
            }
            return ret;
        }
        else if (pat == 0x13) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[4], table[1]);
            }
            return ret;
        }
        else if (pat == 0x2A) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[3]);
            }
            return ret;
        }
        else if (pat == 0x92) {
            if (getData<PixelType>(ti, -1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[4]);
            }
            return ret;
        }
        else if (pat == 0xC8) {
            if (getData<PixelType>(ti, 0, -1) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[3], table[6]);
            }
            return ret;
        }
        else if (pat == 0x0E) {
            if (getData<PixelType>(ti, 1, 0) == TRUE) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[3]);
            }
            return ret;
        }
        if (pat == 0xD0) {
            if ((getData<PixelType>(ti, 0, -1) == TRUE) && (getData<PixelType>(ti, -1, 0) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[4], table[6]);
            }
            return ret;
        }
        if (pat == 0x16) {
            if ((getData<PixelType>(ti, -1, 0) == TRUE) && (getData<PixelType>(ti, 0, 1) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[4]);
            }
            return ret;
        }
        if (pat == 0x0B) {
            if ((getData<PixelType>(ti, 1, 0) == TRUE) && (getData<PixelType>(ti, 0, 1) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[3]);
            }
            return ret;
        }
    }
    else if (patCount == 2) {
        if (pat == 0x50) {
            if ((getData<PixelType>(ti, 0, -1) == TRUE) && (getData<PixelType>(ti, -1, 0) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[4], table[6]);
            }
            return ret;
        }
        else if (pat == 0x12) {
            if ((getData<PixelType>(ti, -1, 0) == TRUE) && (getData<PixelType>(ti, 0, 1) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[4]);
            }
            return ret;
        }
        else if (pat == 0x0A) {
            if ((getData<PixelType>(ti, 1, 0) == TRUE) && (getData<PixelType>(ti, 0, 1) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[1], table[3]);
            }
            return ret;
        }
        else if (pat == 0x48) {
            if ((getData<PixelType>(ti, 0, -1) == TRUE) && (getData<PixelType>(ti, 1, 0) == TRUE)) {
                ret.drawFlag = TRUE;
                ret.color = minColor<PixelType>(table[3], table[6]);
            }
            return ret;
        }
    }
    return ret;
}

//*****************************************************************************
template<typename PixelType>
PF_Err thinExec(NF_AE* ae, ParamInfo* infoP)
{
    typedef typename PixelTraits<PixelType>::InfoType ThinInfo;
    typedef typename PixelTraits<PixelType>::BakType ThinBak;

    PF_Err err = PF_Err_NONE;
    ThinInfo ti;

    ti.target_color_count = infoP->target_color_count;
    if (ti.target_color_count <= 0) return err;

    for (A_long i = 0; i < infoP->target_color_count; i++) {
        //ti.color[i] = PixelTraits<PixelType>::ConvertFrom8bit(infoP->color[i]);
        ti.color8[i] = infoP->target_color[i];
    }

    ti.level = (A_long)((double)PF_MAX_CHAN8 * infoP->target_level + 0.5);
    ti.w = ae->outputInfo.width;
    ti.wt = ae->outputInfo.widthTrue;
    ti.wt2 = ti.wt * 2;
    ti.h = ae->outputInfo.height;
    ti.offset = ti.wt - ti.w;

    ti.scanlineH = ae->NewHandle(ti.wt * sizeof(PixelType) * 4);
    if (!ti.scanlineH) return PF_Err_INTERNAL_STRUCT_DAMAGED;

    ti.scanline = *(PixelType**)ti.scanlineH;
    ti.data = (PixelType*)ae->output->data;
    ti.white = infoP->white;
    ti.alphaZero = infoP->alphaZero;
    ti.edge = infoP->edge;

    // マルチスレッド処理用のパラメータ
    const unsigned int numThreads = MAX(1u, MIN(std::thread::hardware_concurrency(), 
                                                           static_cast<unsigned int>(ti.h / 8)));
    const bool useMultiThread = (ti.h >= 64 && numThreads > 1);

    for (A_long vv = 0; vv < infoP->thin_value; vv++) {
        // エッジパス
        if (ti.edge == TRUE) {
            if (useMultiThread) {
                std::vector<std::thread> threads;
                A_long rowsPerThread = ti.h / numThreads;

                for (unsigned int t = 0; t < numThreads; t++) {
                    A_long startRow = t * rowsPerThread;
                    A_long endRow = (t == numThreads - 1) ? ti.h : (t + 1) * rowsPerThread;

                    threads.emplace_back([&ti, startRow, endRow]() {
                        std::vector<PixelType> localScanline(ti.wt * 3);
                        for (A_long j = startRow; j < endRow; j++) {
                            scanlineCopyLocal<PixelType>(&ti, localScanline.data(), j);
                            A_long now = j * ti.wt;

                            for (A_long i = 0; i < ti.w; i++) {
                                PixelType p = getScanLineLocal<PixelType>(&ti, localScanline.data(), i, 0, 0);
                                if (CompPx<PixelType>(&ti, p)) {
                                    ThinBak tb = getPatEdgeLocal<PixelType>(&ti, localScanline.data(), i);
                                    if (tb.drawFlag && !shouldSkipPixel<PixelType>(&ti, tb.color)) {
                                        ti.data[now] = tb.color;
                                    }
                                }
                                now++;
                            }
                        }
                    });
                }

                for (auto& thread : threads) {
                    thread.join();
                }
            }
            else {
                A_long now = 0;
                for (A_long j = 0; j < ti.h; j++) {
                    ti.nowY = j;
                    scanlineCopy<PixelType>(&ti, j);
                    for (A_long i = 0; i < ti.w; i++) {
                        ti.nowX = i;
                        PixelType p = getScanLine<PixelType>(&ti, 0, 0);
                        if (CompPx<PixelType>(&ti, p)) {
                            ThinBak tb = getPatEdge<PixelType>(&ti);
                            if (tb.drawFlag && !shouldSkipPixel<PixelType>(&ti, tb.color)) {
                                ti.data[now] = tb.color;
                            }
                        }
                        now++;
                    }
                    now += ti.offset;
                }
            }
        }

        // メインパス（getPatはgetDataを使用するため、シングルスレッドで実行）
        A_long now = 0;
        for (A_long j = 0; j < ti.h; j++) {
            ti.nowY = j;
            scanlineCopy<PixelType>(&ti, j);
            for (A_long i = 0; i < ti.w; i++) {
                ti.nowX = i;
                PixelType p = getScanLine<PixelType>(&ti, 0, 0);
                if (CompPx<PixelType>(&ti, p)) {
                    ThinBak tb = getPat<PixelType>(&ti);
                    if (tb.drawFlag && !shouldSkipPixel<PixelType>(&ti, tb.color)) {
                        ti.data[now] = tb.color;
                    }
                }
                now++;
            }
            now += ti.offset;
        }

        // ドットパス（getPatDotもシングルスレッドで実行）
        now = 0;
        for (A_long j = 0; j < ti.h; j++) {
            ti.nowY = j;
            scanlineCopy<PixelType>(&ti, j);
            for (A_long i = 0; i < ti.w; i++) {
                ti.nowX = i;
                PixelType p = getScanLine<PixelType>(&ti, 0, 0);
                if (CompPx<PixelType>(&ti, p)) {
                    ThinBak tb = getPatDot<PixelType>(&ti);
                    if (tb.drawFlag && !shouldSkipPixel<PixelType>(&ti, tb.color)) {
                        ti.data[now] = tb.color;
                    }
                }
                now++;
            }
            now += ti.offset;
        }

        // ドット第2パス（シングルスレッドのみ - データ依存性があるため）
       now = 0;
        for (A_long j = 0; j < ti.h; j++) {
            ti.nowY = j;
            scanlineCopy<PixelType>(&ti, j);
            for (A_long i = 0; i < ti.w; i++) {
                ti.nowX = i;
                PixelType p = getScanLine<PixelType>(&ti, 0, 0);
                A_long now2 = now;
                if (CompPx<PixelType>(&ti, p)) {
                    ThinBak tb = getPatDot2nd<PixelType>(&ti);

                    if (tb.drawFlag) {
                        switch (tb.dir) {
                        case 0:
                            if (ti.nowY > 0) {
                                now2 = now - ti.wt;
                            }
                            else {
                                tb.drawFlag = FALSE;
                            }
                            break;
                        case 1:
                            if (ti.nowX < (ti.w - 1)) {
                                now2 = now + 1;
                            }
                            else {
                                tb.drawFlag = FALSE;
                            }
                            break;
                        case 2:
                            if (ti.nowY < (ti.h - 1)) {
                                now2 = now + ti.wt;
                            }
                            else {
                                tb.drawFlag = FALSE;
                            }
                            break;
                        case 3:
                            if (ti.nowX > 0) {
                                now2 = now - 1;
                            }
                            else {
                                tb.drawFlag = FALSE;
                            }
                            break;
                        }
                    }
                    if (tb.drawFlag) {
                        PixelType p2 = ti.data[now2];
                        if (!shouldSkipPixel<PixelType>(&ti, p2)) {
                            ti.data[now2] = tb.color;
                        }
                    }
                }
                now++;
            }
            now += ti.offset;
        }
    }

    if (ti.scanlineH != NULL) {
        ae->DisposeHandle(ti.scanlineH);
        ti.scanlineH = NULL;
    }

    return err;
}

PF_Err thinExec8(NF_AE* ae, ParamInfo* infoP)
{
    return thinExec<PF_Pixel>(ae, infoP);
}
PF_Err thinExec16(NF_AE* ae, ParamInfo* infoP)
{
    return thinExec<PF_Pixel16>(ae, infoP);
}
PF_Err thinExec32(NF_AE* ae, ParamInfo* infoP)
{
    return thinExec<PF_PixelFloat>(ae, infoP);
}