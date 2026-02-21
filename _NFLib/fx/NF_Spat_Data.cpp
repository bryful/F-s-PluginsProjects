#include "NF_Spat_Data.h"

/**
 * 決定論的なハッシュ関数 (0.0 - 1.0)
 */
inline float hash_01(int x, int y, int seed) {
    uint32_t n = (x + 12345) * 374761393 + (y + 67890) * 668265263 + (seed + 13579) * 1103515245;
    n = (n ^ (n >> 13)) * 1274126177;
    return (float)(n & 0x7fffffff) / 2147483647.0f;
}

/**
 * 内部処理用テンプレート
 */
template <typename T>
void DrawSpatAccentT(
    PF_EffectWorld* world,
    int count,
    float countLoLimit,
    bool is_color,
    float intensity_mult,
    int seed)
{
    if (intensity_mult <= 0.0f) return;
    int width = world->width;
    int height = world->height;

    // --- 1. 描画数の決定 ---
    int min_count = static_cast<int>((float)count * countLoLimit);
    int range = count - min_count;
    float count_rand = hash_01(seed, 0, 777);
    int actual_count = min_count + (range > 0 ? (int)(count_rand * (float)range) : 0);
	if (actual_count <= 0) return;


    float max_chan;
    if constexpr (std::is_same_v<T, PF_Pixel8>)      max_chan = 255.0f;
    else if constexpr (std::is_same_v<T, PF_Pixel16>)     max_chan = 32768.0f;
    else if constexpr (std::is_same_v<T, PF_PixelFloat>)  max_chan = 1.0f;

    for (int i = 0; i < actual_count; ++i) {

        // --- 2. 各個体のパラメータ決定 ---
        int idx = (int)(hash_01(i, 100, seed) * SPT_COUNT);
        idx = (idx < 0) ? 0 : (idx >= SPT_COUNT ? SPT_COUNT - 1 : idx);
        int spat_size = SPT_SIZE(idx);

        int cx = (int)(hash_01(i, 200, seed) * width);
        int cy = (int)(hash_01(i, 300, seed) * height);
        int x_start = cx - (spat_size / 2);
        int y_start = cy - (spat_size / 2);

        float r_f, g_f, b_f;
        if (is_color) {
            r_f = hash_01(i, 400, seed);
            g_f = hash_01(i, 500, seed);
            b_f = hash_01(i, 600, seed);
        }
        else {
            r_f = g_f = b_f = hash_01(i, 700, seed);
        }

        // --- ★今回のポイント：極性（Polarity）の決定 ---
        // 50%の確率で 1.0f (加算)、50%の確率で -1.0f (減算) にする
        float polarity = (hash_01(i, 900, seed) > 0.5f) ? 1.0f : -1.0f;

        // 強度に極性を掛ける
        float intensity = hash_01(i, 800, seed) * intensity_mult * max_chan * polarity;

        // --- 3. 描画ループ ---
        for (int py = 0; py < spat_size; ++py) {
            int wy = y_start + py;
            if (wy < 0 || wy >= height) continue;

            T* row_ptr = (T*)((char*)world->data + (wy * world->rowbytes));

            for (int px = 0; px < spat_size; ++px) {
                int wx = x_start + px;
                if (wx < 0 || wx >= width) continue;

                float spat_alpha = (float)SPT_VALUE(idx, px, py) / 255.0f;
                if (spat_alpha <= 0.0f) continue;

                float add_r = spat_alpha * r_f * intensity;
                float add_g = spat_alpha * g_f * intensity;
                float add_b = spat_alpha * b_f * intensity;

                T* pixel = &row_ptr[wx];

                // クランプしながら加減算 (add_r がマイナスの場合は減算になる)
                if constexpr (std::is_same_v<T, PF_Pixel8>) {
                    pixel->red = (A_u_char)AE_CLAMP((float)pixel->red + add_r, 0.0f, max_chan);
                    pixel->green = (A_u_char)AE_CLAMP((float)pixel->green + add_g, 0.0f, max_chan);
                    pixel->blue = (A_u_char)AE_CLAMP((float)pixel->blue + add_b, 0.0f, max_chan);
                }
                else if constexpr (std::is_same_v<T, PF_Pixel16>) {
                    pixel->red = (A_u_short)AE_CLAMP((float)pixel->red + add_r, 0.0f, max_chan);
                    pixel->green = (A_u_short)AE_CLAMP((float)pixel->green + add_g, 0.0f, max_chan);
                    pixel->blue = (A_u_short)AE_CLAMP((float)pixel->blue + add_b, 0.0f, max_chan);
                }
                else {
                    pixel->red = (float)AE_CLAMP(pixel->red + add_r, 0.0f, max_chan);
                    pixel->green = (float)AE_CLAMP(pixel->green + add_g, 0.0f, max_chan);
                    pixel->blue = (float)AE_CLAMP(pixel->blue + add_b, 0.0f, max_chan);
                }
            }
        }
    }
}

// DrawSpatAccent（外部公開用）は変更なし...

/**
 * 外部公開用関数
 */
PF_Err DrawSpatAccent(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    int count,
    float countLoLimit,
    bool is_color,
    float intensity_max,
    int seed)
{
    if (pixelFormat == PF_PixelFormat_ARGB128) {
        DrawSpatAccentT<PF_PixelFloat>(world, count, countLoLimit, is_color, intensity_max, seed);
    }
    else if (pixelFormat == PF_PixelFormat_ARGB64) {
        DrawSpatAccentT<PF_Pixel16>(world, count, countLoLimit, is_color, intensity_max, seed);
    }
    else {
        DrawSpatAccentT<PF_Pixel8>(world, count, countLoLimit, is_color, intensity_max, seed);
    }
    return PF_Err_NONE;
}
/**
 * Spatデータを使用してWorldにブレンド合成する（8bit専用）
 *
 * @param world 合成先のワールド
 * @param widthTrue ワールドの実際の幅（rowbytes / sizeof(PF_Pixel)）
 * @param x 合成開始X座標
 * @param y 合成開始Y座標
 * @param spat 32x32サイズのSpatデータ（各要素0-255がアルファ値として機能）
 * @param col 合成する色
 */
static void BlendSpatToWorld8(
    PF_EffectWorld* world,
    A_long widthTrue,
    A_long x,
    A_long y,
    const std::vector<std::vector<A_u_char>>& spat,
    const PF_Pixel& col
)
{
    if (spat.empty() || spat[0].empty()) return;

    A_long spatHeight = static_cast<A_long>(spat.size());
    A_long spatWidth = static_cast<A_long>(spat[0].size());

    PF_Pixel* data = (PF_Pixel*)world->data;

    for (A_long j = 0; j < spatHeight; ++j) {
        A_long yPos = y + j;
        // 範囲チェック
        if (yPos < 0 || yPos >= world->height) continue;

        for (A_long i = 0; i < spatWidth; ++i) {
            A_long xPos = x + i;
            // 範囲チェック
            if (xPos < 0 || xPos >= world->width) continue;

            // Spatデータからアルファ値を取得（0-255）
            A_u_char spatAlpha = spat[j][i];
            if (spatAlpha == 0) continue; // 完全透明ならスキップ

            // ワールド上のピクセルを取得
            PF_Pixel& dstPixel = data[yPos * widthTrue + xPos];

            // アルファブレンド係数（0.0-1.0）
            float alpha = spatAlpha / 255.0f;
            float invAlpha = 1.0f - alpha;

            // ブレンド合成: result = src * alpha + dst * (1 - alpha)
            dstPixel.red = (A_u_char)AE_CLAMP(col.red * alpha + dstPixel.red * invAlpha + 0.5f, 0, 255);
            dstPixel.green = (A_u_char)AE_CLAMP(col.green * alpha + dstPixel.green * invAlpha + 0.5f, 0, 255);
            dstPixel.blue = (A_u_char)AE_CLAMP(col.blue * alpha + dstPixel.blue * invAlpha + 0.5f, 0, 255);

            // アルファチャンネルもブレンド
            dstPixel.alpha = (A_u_char)AE_CLAMP(col.alpha * alpha + dstPixel.alpha * invAlpha + 0.5f, 0, 255);
        }
    }
}