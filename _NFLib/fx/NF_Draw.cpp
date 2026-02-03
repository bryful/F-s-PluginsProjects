#include "NF_Draw.h"


// *********************************************************************************
template <typename P>
static PF_Err DrawLine(
    PF_EffectWorld* world,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    P color)
{
    A_long width = world->width;
    A_long height = world->height;
    A_long rowbytes = world->rowbytes;
    char* data_ptr = reinterpret_cast<char*>(world->data);

    // 距離の絶対値
    A_long dx = std::abs(x1 - x0);
    A_long dy = std::abs(y1 - y0);

    // 進む方向 (1 or -1)
    A_long sx = (x0 < x1) ? 1 : -1;
    A_long sy = (y0 < y1) ? 1 : -1;

    // 誤差の初期値
    A_long err = dx - dy;

    while (true) {
        // 境界チェックを行ってからピクセルを書き込み
        if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
            P* pixel = reinterpret_cast<P*>(data_ptr + (y0 * rowbytes) + (x0 * sizeof(P)));
            *pixel = color;
        }

        // 終点に達したら終了
        if (x0 == x1 && y0 == y1) break;

        A_long e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return PF_Err_NONE;
}
static PF_Err DrawLineImpl(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
	PF_Pixel fill_color
)
{
	PF_Err err = PF_Err_NONE;

    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB128:
        PF_PixelFloat fill_color_float;
		fill_color_float = NF_Pixel8To32(fill_color);
        err = DrawLine<PF_PixelFloat>(worldP, x0, y0, x1, y1, fill_color_float);
        break;
    case PF_PixelFormat_ARGB64:
        PF_Pixel16 fill_color_16;
        fill_color_16 = NF_Pixel8To16(fill_color);
        err = DrawLine<PF_Pixel16>(worldP, x0, y0, x1, y1, fill_color_16);
        break;
    case PF_PixelFormat_ARGB32:
        err = DrawLine<PF_Pixel>(worldP, x0, y0, x1, y1, fill_color);
        break;

    }
    return err;
}


PF_Err DrawLine(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel fill_color
)
{
    return DrawLineImpl(worldP, pixelFormat, x0,y0,x1,y1,fill_color);
}
// *********************************************************************************
/**
 * 矩形の枠線を描画 (DrawBox)
 */
template <typename P>
static PF_Err DrawBox(
    PF_EffectWorld* world,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    P color)
{
    // 座標の正規化 (x0, y0 を左上に)
    if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);

    // 水平線 2本
    for (A_long x = x0; x <= x1; ++x) {
        // 上辺
        if (x >= 0 && x < world->width && y0 >= 0 && y0 < world->height) {
            P* p = reinterpret_cast<P*>(reinterpret_cast<char*>(world->data) + (y0 * world->rowbytes) + (x * sizeof(P)));
            *p = color;
        }
        // 下辺
        if (x >= 0 && x < world->width && y1 >= 0 && y1 < world->height) {
            P* p = reinterpret_cast<P*>(reinterpret_cast<char*>(world->data) + (y1 * world->rowbytes) + (x * sizeof(P)));
            *p = color;
        }
    }

    // 垂直線 2本 (角の重複を避けるなら y0+1 から y1-1)
    for (A_long y = y0 + 1; y < y1; ++y) {
        // 左辺
        if (y >= 0 && y < world->height && x0 >= 0 && x0 < world->width) {
            P* p = reinterpret_cast<P*>(reinterpret_cast<char*>(world->data) + (y * world->rowbytes) + (x0 * sizeof(P)));
            *p = color;
        }
        // 右辺
        if (y >= 0 && y < world->height && x1 >= 0 && x1 < world->width) {
            P* p = reinterpret_cast<P*>(reinterpret_cast<char*>(world->data) + (y * world->rowbytes) + (x1 * sizeof(P)));
            *p = color;
        }
    }

    return PF_Err_NONE;
}
static PF_Err DrawBoxImpl(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel color
)
{
    PF_Err err = PF_Err_NONE;

    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB128:
        PF_PixelFloat color_float;
        color_float = NF_Pixel8To32(color);
        err = DrawBox<PF_PixelFloat>(world, x0, y0, x1, y1, color_float);
        break;
    case PF_PixelFormat_ARGB64:
        PF_Pixel16 color_16;
        color_16 = NF_Pixel8To16(color);
        err = DrawBox<PF_Pixel16>(world, x0, y0, x1, y1, color_16);
        break;
    case PF_PixelFormat_ARGB32:
        err = DrawBox<PF_Pixel>(world, x0, y0, x1, y1, color);
        break;

    }
    return err;
}
PF_Err DrawBox(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel color
)
{
    return DrawBoxImpl(world, pixelFormat, x0, y0, x1, y1, color);
}
// *********************************************************************************
/**
 * 塗りつぶされた矩形を描画 (FillBox)
 * パフォーマンスのため、行単位のスキャンループで書き込みます。
 */
template <typename P>
static PF_Err FillBox(
    PF_EffectWorld* world,
    int x0, int y0,
    int x1, int y1,
    P color)
{
    // 座標の正規化
    if (x0 > x1) F_SWAP(x0, x1);
    if (y0 > y1) F_SWAP(y0, y1);

    // クリップ（画面外を描画しないよう制限）
    int left = max(x0, 0);
    int top = max(y0, 0);
    int right = min(x1, (int)world->width - 1);
    int bottom =min(y1, (int)world->height - 1);

    if (left > right || top > bottom) return PF_Err_NONE;

    for (int y = top; y <= bottom; ++y) {
        P* p_row = reinterpret_cast<P*>(reinterpret_cast<char*>(world->data) + (y * world->rowbytes));
        for (int x = left; x <= right; ++x) {
            p_row[x] = color;
        }
    }

    return PF_Err_NONE;
}
static PF_Err FillBoxImpl(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel color
)
{
    PF_Err err = PF_Err_NONE;

    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB128:
        PF_PixelFloat color_float;
        color_float = NF_Pixel8To32(color);
        err = FillBox<PF_PixelFloat>(world, x0, y0, x1, y1, color_float);
        break;
    case PF_PixelFormat_ARGB64:
        PF_Pixel16 color_16;
        color_16 = NF_Pixel8To16(color);
        err = FillBox<PF_Pixel16>(world, x0, y0, x1, y1, color_16);
        break;
    case PF_PixelFormat_ARGB32:
        err = FillBox<PF_Pixel>(world, x0, y0, x1, y1, color);
        break;

    }
    return err;
}
PF_Err FillBox(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x0, A_long y0,
    A_long x1, A_long y1,
    PF_Pixel color
)
{
    return FillBoxImpl(world, pixelFormat, x0, y0, x1, y1, color);
}
// *********************************************************************************
/**
 * 円の枠線を描画 (DrawCircle)
 */
template <typename P>
static PF_Err DrawCircleT(
    PF_EffectWorld* world,
    int centerX, int centerY,
    int radius,
    P color)
{
    if (radius < 0) return PF_Err_NONE;

    int x = radius;
    int y = 0;
    int d = 1 - radius; // 判定変数

    auto plot = [&](int px, int py) {
        if (px >= 0 && px < (int)world->width && py >= 0 && py < (int)world->height) {
            P* p = reinterpret_cast<P*>(reinterpret_cast<char*>(world->data) + (py * world->rowbytes) + (px * sizeof(P)));
            *p = color;
        }
        };

    while (x >= y) {
        // 8つの対称点に描画
        plot(centerX + x, centerY + y);
        plot(centerX + y, centerY + x);
        plot(centerX - y, centerY + x);
        plot(centerX - x, centerY + y);
        plot(centerX - x, centerY - y);
        plot(centerX - y, centerY - x);
        plot(centerX + y, centerY - x);
        plot(centerX + x, centerY - y);
        y++;
        if (d < 0) {
            d += 2 * y + 1;
        }
        else {
            x--;
            d += 2 * (y - x) + 1;
        }
    }
    return PF_Err_NONE;
}
PF_Err DrawCircle(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    PF_Point pos,
	A_long radius,
    PF_Pixel color
)
{
    PF_Err err = PF_Err_NONE;

    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB128:
        PF_PixelFloat color_float;
        color_float = NF_Pixel8To32(color);
        err = DrawCircleT<PF_PixelFloat>(world, pos.x, pos.y, radius, color_float);
        break;
    case PF_PixelFormat_ARGB64:
        PF_Pixel16 color_16;
        color_16 = NF_Pixel8To16(color);
        err = DrawCircleT<PF_Pixel16>(world, pos.x, pos.y, radius, color_16);
        break;
    case PF_PixelFormat_ARGB32:
        err = DrawCircleT<PF_Pixel>(world, pos.x, pos.y, radius, color);
        break;

    }
    return err;
}
/**
 * 塗りつぶされた円を描画 (FillCircle)
 * 水平スキャンラインとして描画するため非常に高速です。
 */
template <typename P>
static PF_Err FillCircleT(
    PF_EffectWorld* world,
    int centerX, int centerY,
    int radius,
    P color)
{
    if (radius < 0) return PF_Err_NONE;

    int x = radius;
    int y = 0;
    int d = 1 - radius;

    int width = world->width;
    int height = world->height;
    int rowbytes = world->rowbytes;
    char* data_ptr = reinterpret_cast<char*>(world->data);

    // 水平線を引くヘルパー
    auto drawScanline = [&](int lx, int rx, int py) {
        if (py < 0 || py >= height) return;
        if (lx > rx) F_SWAP(lx, rx);
        int left = (lx > 0) ? lx : 0;
        int right = (rx < width - 1) ? rx : width - 1;

        if (left <= right) {
            P* p_row = reinterpret_cast<P*>(data_ptr + (py * rowbytes));
            for (int i = left; i <= right; ++i) {
                p_row[i] = color;
            }
        }
        };

    while (x >= y) {
        // 上下に対称な 4 本のスキャンラインを引く
        drawScanline(centerX - x, centerX + x, centerY + y);
        drawScanline(centerX - x, centerX + x, centerY - y);
        drawScanline(centerX - y, centerX + y, centerY + x);
        drawScanline(centerX - y, centerX + y, centerY - x);

        y++;
        if (d < 0) {
            d += 2 * y + 1;
        }
        else {
            x--;
            d += 2 * (y - x) + 1;
        }
    }
    return PF_Err_NONE;
}
PF_Err FillCircle(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    PF_Point pos,
    A_long radius,
    PF_Pixel color
)
{
    PF_Err err = PF_Err_NONE;

    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB128:
        PF_PixelFloat color_float;
        color_float = NF_Pixel8To32(color);
        err = FillCircleT<PF_PixelFloat>(world, pos.x, pos.y, radius, color_float);
        break;
    case PF_PixelFormat_ARGB64:
        PF_Pixel16 color_16;
        color_16 = NF_Pixel8To16(color);
        err = FillCircleT<PF_Pixel16>(world, pos.x, pos.y, radius, color_16);
        break;
    case PF_PixelFormat_ARGB32:
        err = FillCircleT<PF_Pixel>(world, pos.x, pos.y, radius, color);
        break;

    }
    return err;
}