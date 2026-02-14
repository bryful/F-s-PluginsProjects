#include "NF_Noise.h"
#include <sstream>


struct Span {
    int x1, x2, y, dy;
};

/**
 * 各ピクセル型を 8bit 相当の整数値に正規化して比較する
 */
template <typename P>
static inline bool IsSameColorDownsampled(const P* p1, const P* p2) {
    if constexpr (std::is_same<P, PF_PixelFloat>::value) {
        // 32bit Float: 0.0-1.0 を 0-255 に変換して比較
        // (クランプ処理を含めるとより安全です)
        auto quick_downsample = [](float f) -> int {
            return static_cast<int>(f * 255.0f + 0.5f);
            };
        return (quick_downsample(p1->alpha) == quick_downsample(p2->alpha) &&
            quick_downsample(p1->red) == quick_downsample(p2->red) &&
            quick_downsample(p1->green) == quick_downsample(p2->green) &&
            quick_downsample(p1->blue) == quick_downsample(p2->blue));
    }
    else if constexpr (std::is_same<P, PF_Pixel16>::value) {
        // 16bit: 0-32768 を 0-255 に変換 (>> 7)
        return ((p1->alpha >> 7) == (p2->alpha >> 7) &&
            (p1->red >> 7) == (p2->red >> 7) &&
            (p1->green >> 7) == (p2->green >> 7) &&
            (p1->blue >> 7) == (p2->blue >> 7));
    }
    else {
        // 8bit: そのまま比較
        return (p1->alpha == p2->alpha && p1->red == p2->red &&
            p1->green == p2->green && p1->blue == p2->blue);
    }
}

/**
 * パフォーマンス重視のスキャンライン・フィリング
 */
template <typename P>
static PF_Err FloodFillScanline(
    PF_EffectWorld* output,
    int startX,
    int startY,
    P fill_color)
{
    if (startX < 0 || startX >= output->width || startY < 0 || startY >= output->height) return PF_Err_NONE;

    int width = output->width;
    int height = output->height;
    int rowbytes = output->rowbytes;
    char* data_ptr = reinterpret_cast<char*>(output->data);

    P* start_p = reinterpret_cast<P*>(data_ptr + (startY * rowbytes) + (startX * sizeof(P)));
    P target_color = *start_p;

    // 塗りつぶし色自体も判定基準（減色後）で一致していたら無限ループ防止で抜ける
    if (IsSameColorDownsampled(&target_color, &fill_color)) return PF_Err_NONE;

    std::vector<Span> stack;
    stack.reserve(height);
    stack.push_back({ startX, startX, startY, 1 });
    stack.push_back({ startX, startX, startY - 1, -1 });

    while (!stack.empty()) {
        Span s = stack.back();
        stack.pop_back();

        if (s.y < 0 || s.y >= height) continue;

        P* p_row = reinterpret_cast<P*>(data_ptr + (s.y * rowbytes));

        int x = s.x1;
        while (x >= 0 && IsSameColorDownsampled(&p_row[x], &target_color)) {
            p_row[x] = fill_color; // 書き込みは元の精度を維持
            x--;
        }

        if (x >= s.x1) continue;

        int left = x + 1;
        int right;
        x = s.x1 + 1;

        while (true) {
            while (x < width && IsSameColorDownsampled(&p_row[x], &target_color)) {
                p_row[x] = fill_color;
                x++;
            }
            right = x - 1;

            auto check_neighbor = [&](int next_y, int next_dy) {
                if (next_y < 0 || next_y >= height) return;
                P* p_next = reinterpret_cast<P*>(data_ptr + (next_y * rowbytes));
                bool in_span = false;
                int span_start = 0;
                for (int i = left; i <= right; i++) {
                    bool match = IsSameColorDownsampled(&p_next[i], &target_color);
                    if (!in_span && match) {
                        span_start = i;
                        in_span = true;
                    }
                    else if (in_span && !match) {
                        stack.push_back({ span_start, i - 1, next_y, next_dy });
                        in_span = false;
                    }
                }
                if (in_span) stack.push_back({ span_start, right, next_y, next_dy });
                };

            check_neighbor(s.y + s.dy, s.dy);
            check_neighbor(s.y - s.dy, -s.dy);

            while (x <= s.x2 && x < width && !IsSameColorDownsampled(&p_row[x], &target_color)) x++;
            if (x > s.x2 || x >= width) break;
            left = x;
        }
    }
    return PF_Err_NONE;
}
PF_Err Paint(
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    A_long xL,
    A_long yL,
    PF_Pixel fill_color
)
{
    PF_Err err = PF_Err_NONE;

   

    switch (pixelFormat)
    {
    case PF_PixelFormat_ARGB128:
        PF_PixelFloat fill_color_float;
        fill_color_float = NF_Pixel8To32(fill_color);
        err = FloodFillScanline<PF_PixelFloat>(worldP, xL, yL, fill_color_float);
        break;
    case PF_PixelFormat_ARGB64:
        PF_Pixel16 fill_color_16;
        fill_color_16 = NF_Pixel8To16(fill_color);
        err = FloodFillScanline<PF_Pixel16>(worldP, xL, yL, fill_color_16);
        break;
    case PF_PixelFormat_ARGB32:
        err = FloodFillScanline<PF_Pixel8>(worldP, xL, yL, fill_color);
        break;

    }
    return err;
}

