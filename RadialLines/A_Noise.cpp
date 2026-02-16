#include "RadialLines.h"
typedef struct ANoiseInfo {
	PF_FpLong	noise;
	A_long		seed;

} ANoiseInfo;
// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}

// --- 5. 共通フィルタテンプレート ---
template <typename T>
static PF_Err A_NoiseImageT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	ANoiseInfo* infoP = reinterpret_cast<ANoiseInfo*>(refcon);
	if(infoP->noise <= 0) {
		return PF_Err_NONE;
	}
	PF_FpLong rnd = (PF_FpLong)hash_float(xL, yL, infoP->seed);
	if (rnd < infoP->noise) {
		PF_FpLong max_val = GetMaxChannel<T>();
		// 色の加算処理
		PF_FpLong aa = (PF_FpLong)hash_float(xL, yL, infoP->seed + 10) * max_val; // 座標に基づくハッシュでランダム値を変化させる
		PF_FpLong aaa = AE_CLAMP(outP->alpha - aa, 0, max_val);
		outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(aaa);
	}

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err A_NoiseImage8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return A_NoiseImageT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err A_NoiseImage16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return A_NoiseImageT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err A_NoiseImageFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return A_NoiseImageT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err A_NoiseImage(
	NF_AE* ae,
	PF_FpLong noise,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;
	ANoiseInfo info{ noise, seed };
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		err = ae->suitesP->IterateFloatSuite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->output,
			NULL,
			&info,
			A_NoiseImageFloat,
			ae->output);
		break;
	case PF_PixelFormat_ARGB64:
		err = ae->suitesP->Iterate16Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->output,
			NULL,
			&info,
			A_NoiseImage16,
			ae->output);
		break;
	case PF_PixelFormat_ARGB32:
		err = ae->suitesP->Iterate8Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->output,
			NULL,
			&info,
			A_NoiseImage8,
			ae->output);
		break;
	}
	return err;
}
typedef struct PrmTbl {
	PF_PixelPtr	data;		//画像
	A_long 		width;		//画像の横幅
	A_long 		height;		//画像の高さ
	A_long 		w;			//ターゲット
	A_long 		h;
	A_long 		target;

	PF_Boolean	animFlag;	//
	A_long 		anim;

	A_long		frame;
	A_long		seed;

	A_long 		value;
	PF_Fixed	length;
	PF_Boolean	colorFlag;

	PF_Fixed	block_value;
	PF_Fixed	block_length;
	A_long 		block_width;
	A_long 		block_height;
	PF_Boolean	block_colorFlag;

} PrmTbl;
// テンプレート化されたノイズメイン関数
template <typename T, typename T_COMP>
static void ANoiseMainT(NF_AE* ae, A_long count, A_long seed, float max_val)
{
	T* data = (T*)ae->output->data;

	for (A_long idx = 0; idx < count; idx++) {
		A_long x = (A_long)(hash_float(0, idx, seed) * (ae->outputInfo.width - 2) + 1);
		A_long y = (A_long)(hash_float(10, idx, seed) * (ae->outputInfo.height - 2) + 1);
		A_long t = x + y * ae->outputInfo.widthTrue;

		PF_FpLong arnd = hash_float(x, y, seed) * max_val/2;
		if (data[t].alpha / max_val > 0.5) continue;
		data[t].alpha = (T_COMP)AE_CLAMP(data[t].alpha - arnd, 0, max_val);

		PF_FpLong arnd2 = arnd / 2;
		PF_FpLong arnd4 = arnd / 4;

		// 左
		A_long t2 = t - 1;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd2, 0, max_val);

		// 右
		t2 = t + 1;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd2, 0, max_val);

		// 上
		t2 = t - ae->outputInfo.widthTrue;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd2, 0, max_val);

		// 下
		t2 = t + ae->outputInfo.widthTrue;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd2, 0, max_val);

		// 左上
		t2 = t - 1 - ae->outputInfo.widthTrue;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd4, 0, max_val);

		// 右上
		t2 = t + 1 - ae->outputInfo.widthTrue;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd4, 0, max_val);

		// 右下
		t2 = t + 1 + ae->outputInfo.widthTrue;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd4, 0, max_val);

		// 左下
		t2 = t - 1 + ae->outputInfo.widthTrue;
		data[t2].alpha = (T_COMP)AE_CLAMP(data[t2].alpha - arnd4, 0, max_val);
	}
}

// ラッパー関数
static void ANoiseMain8(NF_AE* ae, A_long count, A_long seed)
{
	ANoiseMainT<PF_Pixel8, A_u_char>(ae, count, seed, 255.0f);
}

static void ANoiseMain16(NF_AE* ae, A_long count, A_long seed)
{
	ANoiseMainT<PF_Pixel16, A_u_short>(ae, count, seed, 32768.0f);
}

static void ANoiseMain32(NF_AE* ae, A_long count, A_long seed)
{
	ANoiseMainT<PF_PixelFloat, PF_FpShort>(ae, count, seed, 1.0f);
}

// 呼び出し側の関数（例）
PF_Err ApplyANoise(NF_AE* ae, A_long count, A_long seed)
{
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		ANoiseMain32(ae, count, seed);
		break;
	case PF_PixelFormat_ARGB64:
		ANoiseMain16(ae, count, seed);
		break;
	case PF_PixelFormat_ARGB32:
		ANoiseMain8(ae, count, seed);
		break;
	}
	return PF_Err_NONE;
}