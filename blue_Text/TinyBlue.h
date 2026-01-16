#pragma once
#ifndef TINY_BLUE_H
#define TINY_BLUE_H

#include "../FsLibrary/Fs.h"
#include "Fs_Target.h"
#include "../FsLibrary/FsAE.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

// ブラー・抽出用共通構造体
typedef struct {
	PF_EffectWorld* src;
	A_long radius;
} BlurInfo;
// ビット深度ごとの計算用テンプレート構造体（型による違いを吸収）
template <typename P, typename COMP>
struct PixelTrait {
	// P: ピクセル構造体 (PF_Pixel8等)
	// COMP: 計算用型 (A_u_longやfloat等)
};
#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

PF_Err TinyBlue(CFsAE* ae, A_long bl);

#endif // TINY_BLUE_H
