#pragma once
#ifndef NOISE_ENGINE_H
#define NOISE_ENGINE_H

#include "..\_NFLib\AE_SDK.h"
#include "..\_NFLib\NF_Utils.h"


#include <cmath>
#include <cstdint>
#include <algorithm>
#include <cstdint>
#include <vector>

class FastSimplexNoise {
private:
    uint8_t p[512];
    static inline float grad(int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
    static inline float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static inline float lerp(float t, float a, float b) { return a + t * (b - a); }

public:
    FastSimplexNoise() {
        uint8_t permutation[] = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
        for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
    }

    float eval(float x, float y, float z) const {
        int X = (int)std::floor(x) & 255;
        int Y = (int)std::floor(y) & 255;
        int Z = (int)std::floor(z) & 255;
        x -= std::floor(x); y -= std::floor(y); z -= std::floor(z);
        float u = fade(x), v = fade(y), w = fade(z);
        int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;
        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)),
            lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
            lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)),
                lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }

    float fbm(float x, float y, float z, int octaves) const {
        float result = 0.0f, amp = 1.0f, freq = 1.0f, maxV = 0.0f;
        for (int i = 0; i < octaves; i++) {
            result += eval(x * freq, y * freq, z * freq) * amp;
            maxV += amp; amp *= 0.5f; freq *= 2.0f;
        }
        return result / maxV;
    }
};

class NoiseLUT {
private:
    std::vector<float> table;
    const int size = 256;
    const int mask = 255;

public:
    NoiseLUT() : table(256 * 256) {}

    // シーケンス開始時に一度だけ計算
    void Refresh(const FastSimplexNoise& engine, float evolution, int octaves) {
        float noise_step = 0.1f; // LUT内でのノイズの解像度
        const float offset = 0.1234f;
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                //table[(y << 8) | x] = engine.fbm((float)x / size, (float)y / size, evolution, octaves);
                table[(y << 8) | x] = engine.fbm(((float)x+offset)/8.0f, ((float)y+offset) / 8.0f, evolution, octaves);
                //table[(y << 8) | x] = engine.fbm((float)x * noise_step, (float)y * noise_step, evolution, octaves);
            }
        }
    }

    // 座標を指定してLUTから高速取得（バイリニア補間なしの高速版）
    inline float GetValue(int x, int y) const {
        return table[((y & mask) << 8) | (x & mask)];
    }

    // より滑らかなサンプリングが必要な場合の補間版
    float GetValueSmooth(float x, float y) const {
        int x1 = (int)std::floor(x);
        int y1 = (int)std::floor(y);
        float fx = x - x1;
        float fy = y - y1;

        float p11 = GetValue(x1, y1);
        float p21 = GetValue(x1 + 1, y1);
        float p12 = GetValue(x1, y1 + 1);
        float p22 = GetValue(x1 + 1, y1 + 1);

        return (1 - fx) * (1 - fy) * p11 + fx * (1 - fy) * p21 + (1 - fx) * fy * p12 + fx * fy * p22;
    }
};

PF_Err SimplexNoise
(
    PF_InData* in_dataP,
    PF_EffectWorld* input,
    PF_EffectWorld* output,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    float evolution,
    int octaves,
    float scale
    );


#endif