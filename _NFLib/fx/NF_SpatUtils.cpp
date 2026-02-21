#include "NF_SpatUtils.h"

inline std::vector<A_u_char> GetSpatPattern(int index) {
    if (index < 0 || index >= SPT_COUNT) {
        return std::vector<A_u_char>();
    }
    return std::vector<A_u_char>(
        SPT[index],
        SPT[index] + (SPT_WIDTH * SPT_HEIGHT)
    );
}

std::vector<std::vector<A_u_char>> GetDefSpatData(float scale) {
    std::vector<std::vector<A_u_char>> result;
    result.reserve(SPT_COUNT);

    for (int i = 0; i < SPT_COUNT; ++i) {
        if(scale==1.0f){
            result.push_back(GetSpatPattern(i));
        }
        else {
            std::vector<A_u_char> ss = ScaleSpatBilinear(GetSpatPattern(i), scale);
            result.push_back(ss);
        }
    }
    return result;
}

// ピクセルタイプごとの変換Traits
template<typename PixelType>
struct PixelTraits;

template<>
struct PixelTraits<PF_Pixel> {
    static constexpr float AlphaScale = 255.0f;
    static constexpr float GreenScale = 255.0f;
    static constexpr float ColorScale = 255.0f;
    
    static inline float GetAlpha(const PF_Pixel& px) { return px.alpha / AlphaScale; }
    static inline float GetGreen(const PF_Pixel& px) { return (float)px.green; }
    
    static inline void BlendPixel(PF_Pixel& dst, float srcR, float srcG, float srcB, float srcA, float alpha) {
        float invAlpha = 1.0f - alpha;
        dst.red = (A_u_char)AE_CLAMP(srcR * alpha + dst.red * invAlpha + 0.5f, 0, 255);
        dst.green = (A_u_char)AE_CLAMP(srcG * alpha + dst.green * invAlpha + 0.5f, 0, 255);
        dst.blue = (A_u_char)AE_CLAMP(srcB * alpha + dst.blue * invAlpha + 0.5f, 0, 255);
        dst.alpha = (A_u_char)AE_CLAMP(srcA * alpha + dst.alpha * invAlpha + 0.5f, 0, 255);
    }
};

template<>
struct PixelTraits<PF_Pixel16> {
    static constexpr float AlphaScale = 32768.0f;
    static constexpr float GreenScale = 32768.0f;
    static constexpr float ColorScale = 32768.0f / 255.0f;
    
    static inline float GetAlpha(const PF_Pixel16& px) { return px.alpha / AlphaScale; }
    static inline float GetGreen(const PF_Pixel16& px) { return px.green / GreenScale * 255.0f; }
    
    static inline void BlendPixel(PF_Pixel16& dst, float srcR, float srcG, float srcB, float srcA, float alpha) {
        float invAlpha = 1.0f - alpha;
        // 入力色は0-255なので32768スケールに変換
        float srcR16 = srcR * ColorScale;
        float srcG16 = srcG * ColorScale;
        float srcB16 = srcB * ColorScale;
        float srcA16 = srcA * ColorScale;
        
        dst.red = (A_u_short)AE_CLAMP(srcR16 * alpha + dst.red * invAlpha + 0.5f, 0, 32768);
        dst.green = (A_u_short)AE_CLAMP(srcG16 * alpha + dst.green * invAlpha + 0.5f, 0, 32768);
        dst.blue = (A_u_short)AE_CLAMP(srcB16 * alpha + dst.blue * invAlpha + 0.5f, 0, 32768);
        dst.alpha = (A_u_short)AE_CLAMP(srcA16 * alpha + dst.alpha * invAlpha + 0.5f, 0, 32768);
    }
};

template<>
struct PixelTraits<PF_PixelFloat> {
    static constexpr float AlphaScale = 1.0f;
    static constexpr float GreenScale = 1.0f;
    static constexpr float ColorScale = 1.0f/255.0f;
    
    static inline float GetAlpha(const PF_PixelFloat& px) { return px.alpha; }
    static inline float GetGreen(const PF_PixelFloat& px) { return px.green * 255.0f; }
    
    static inline void BlendPixel(PF_PixelFloat& dst, float srcR, float srcG, float srcB, float srcA, float alpha) {
        float invAlpha = 1.0f - alpha;
        // 入力色は0-255なので0-1スケールに変換
        float srcRf = srcR / 255.0f;
        float srcGf = srcG / 255.0f;
        float srcBf = srcB / 255.0f;
        float srcAf = srcA / 255.0f;
        
        dst.red = AE_CLAMP(srcRf * alpha + dst.red * invAlpha, 0.0f, 1.0f);
        dst.green = AE_CLAMP(srcGf * alpha + dst.green * invAlpha, 0.0f, 1.0f);
        dst.blue = AE_CLAMP(srcBf * alpha + dst.blue * invAlpha, 0.0f, 1.0f);
        dst.alpha = AE_CLAMP(srcAf * alpha + dst.alpha * invAlpha, 0.0f, 1.0f);
    }
};

// テンプレート関数による統一実装
template<typename PixelType>
static std::vector<std::vector<A_u_char>> GetSpatDataFromWorldT
(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long spatWidth,
    A_long spatHeight
)
{
    //横のパターン数
    A_long pX = world->width / spatWidth;
    //縦のパターン数
    A_long pY = world->height / spatHeight;
    std::vector<std::vector<A_u_char>> spatData(pY*pX, std::vector<A_u_char>(spatWidth * spatHeight));
    PixelType* data = (PixelType*)world->data;
    A_long widthTrue = world->rowbytes / sizeof(PixelType);
    using Traits = PixelTraits<PixelType>;
    A_long idx = 0;
    for (A_long yIdx = 0; yIdx < pY; ++yIdx) {
        A_long y = yIdx * spatHeight;
        for (A_long xIdx = 0; xIdx < pX; ++xIdx) {
            A_long x = xIdx * spatWidth;
            for (A_long j = 0; j < spatHeight; ++j) {
                for (A_long i = 0; i < spatWidth; ++i) {
                    PixelType& px = data[(y+j) * widthTrue + x+i];
                    float alpha = Traits::GetAlpha(px);
                    float green = Traits::GetGreen(px);
                    A_u_char v = (A_u_char)AE_CLAMP((green * alpha + 0.5), 0, 255);
                    spatData[idx][j * spatWidth + i] = v;
                }
            }
            idx++;
        }
    }
    return spatData;
}

/**
 * 外部公開用関数
 */
std::vector<std::vector<A_u_char>> GetSpatDataFromWorld(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long spatWidth,
    A_long spatHeight
)
{
    if (pixelFormat == PF_PixelFormat_ARGB128) {
        return GetSpatDataFromWorldT<PF_PixelFloat>(world, pixelFormat, spatWidth, spatHeight);
    }
    else if (pixelFormat == PF_PixelFormat_ARGB64) {
        return GetSpatDataFromWorldT<PF_Pixel16>(world, pixelFormat, spatWidth, spatHeight);
    }
    else {
        return GetSpatDataFromWorldT<PF_Pixel>(world, pixelFormat, spatWidth, spatHeight);
    }
}

/**
 * Spatデータを使用してWorldにブレンド合成する（テンプレート版）
 *
 * @param world 合成先のワールド
 * @param widthTrue ワールドの実際の幅（rowbytes / sizeof(PixelType)）
 * @param x 合成中心X座標
 * @param y 合成中心Y座標
 * @param spat Spatデータ（1次元配列、各要素0-255がアルファ値として機能）
 * @param col 合成する色（RGB値は0-255）
 * @param opacity 全体の不透明度（0.0 = 完全透明, 1.0 = 完全不透明）
 */
template<typename PixelType>
static void BlendSpatToWorldT(
    PF_EffectWorld* world,
    A_long widthTrue,
    A_long x,
    A_long y,
    const std::vector<A_u_char>& spat,
    float colR,
    float colG,
    float colB,
    float colA,
    float opacity
)
{
    if (spat.empty() || spat.size() < 2*2) return;
    if (opacity <= 0.0f) return;
    
    A_long sz = (A_long)sqrt((double)spat.size());
    opacity = AE_CLAMP(opacity, 0.0f, 1.0f);
    
    PixelType* data = (PixelType*)world->data;
    using Traits = PixelTraits<PixelType>;
    
    // 中心座標を左上座標に変換
    A_long startX = x - sz / 2;
    A_long startY = y - sz / 2;
    
    for (A_long j = 0; j < sz; ++j) {
        A_long yPos = startY + j;
        if (yPos < 0 || yPos >= world->height) continue;
        
        for (A_long i = 0; i < sz; ++i) {
            A_long xPos = startX + i;
            if (xPos < 0 || xPos >= world->width) continue;
            
            A_u_char spatAlpha = spat[j * sz + i];
            if (spatAlpha == 0) continue;
            
            PixelType& dstPixel = data[yPos * widthTrue + xPos];
            
            float alpha = (spatAlpha / 255.0f) * opacity;
            Traits::BlendPixel(dstPixel, colR, colG, colB, colA, alpha);
        }
    }
}

/**
 * Spatデータを使用してWorldにブレンド合成する（8bit専用・互換性のため残す）
 */
void BlendSpatToWorld8(
    PF_EffectWorld* world,
    A_long widthTrue,
    A_long x,
    A_long y,
    const std::vector<A_u_char>& spat,
    const PF_Pixel& col,
    float opacity
)
{
    BlendSpatToWorldT<PF_Pixel>(
        world, widthTrue, x, y, spat,
        (float)col.red, (float)col.green, (float)col.blue, (float)col.alpha,
        opacity
    );
}
void BlendSpatToWorld16(
    PF_EffectWorld* world,
    A_long widthTrue,
    A_long x,
    A_long y,
    const std::vector<A_u_char>& spat,
    const PF_Pixel& col,
    float opacity
)
{
    BlendSpatToWorldT<PF_Pixel16>(
        world, widthTrue, x, y, spat,
        (float)col.red, (float)col.green, (float)col.blue, (float)col.alpha,
        opacity
    );
}
void BlendSpatToWorldFloat(
    PF_EffectWorld* world,
    A_long widthTrue,
    A_long x,
    A_long y,
    const std::vector<A_u_char>& spat,
    const PF_Pixel& col,
    float opacity
)
{
    BlendSpatToWorldT<PF_PixelFloat>(
        world, widthTrue, x, y, spat,
        (float)col.red, (float)col.green, (float)col.blue, (float)col.alpha,
        opacity
    );
}
/**
 * Spatデータを使用してWorldにブレンド合成する（全ビット深度対応）
 */
void BlendSpatToWorld(
    PF_EffectWorld* world,
    PF_PixelFormat pixelFormat,
    A_long x,
    A_long y,
    const std::vector<A_u_char>& spat,
    const PF_Pixel& col,
    float opacity
)
{
    A_long widthTrue = 0;
    
    if (pixelFormat == PF_PixelFormat_ARGB128) {
        widthTrue = world->rowbytes / sizeof(PF_PixelFloat);
        BlendSpatToWorldT<PF_PixelFloat>(
            world, widthTrue, x, y, spat,
            (float)col.red, (float)col.green, (float)col.blue, (float)col.alpha,
            opacity
        );
    }
    else if (pixelFormat == PF_PixelFormat_ARGB64) {
        widthTrue = world->rowbytes / sizeof(PF_Pixel16);
        BlendSpatToWorldT<PF_Pixel16>(
            world, widthTrue, x, y, spat,
            (float)col.red, (float)col.green, (float)col.blue, (float)col.alpha,
            opacity
        );
    }
    else {
        widthTrue = world->rowbytes / sizeof(PF_Pixel);
        BlendSpatToWorldT<PF_Pixel>(
            world, widthTrue, x, y, spat,
            (float)col.red, (float)col.green, (float)col.blue, (float)col.alpha,
            opacity
        );
    }
}