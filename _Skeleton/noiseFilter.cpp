#include "noiseFilter.h"
struct NoiseInfo {
	PF_FpLong	noise;
	PF_Boolean	isColor;
	A_long		seed;

};
// -- - 1. 型解決用のトレイト(PixelTraits) -- -
template <typename T> struct PixelTraits;

template <> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char CT;
	static inline CT half() { return 128; }
	static inline CT max_chan() { return 255; }
	static inline CT clamp(float v) { return (CT)AE_CLAMP(v + 0.5f, 0, 255); }
};

template <> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short CT;
	static inline CT half() { return 16384; } // AE 16bitは32768が上限
	static inline CT max_chan() { return 32768; }
	static inline CT clamp(float v) { return (CT)AE_CLAMP(v + 0.5f, 0, 32768); }
};

template <> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort CT;
	static inline CT half() { return 0.5f; }
	static inline CT max_chan() { return 1.0f; }
	static inline CT clamp(float v) { return (CT)v; } // Floatは基本クランプ不要
};

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
static PF_Err NoiseT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	NoiseInfo* infoP = reinterpret_cast<NoiseInfo*>(refcon);
	//PF_FpLong max_val = GetMaxChannel<T>();
	PF_FpLong max_val = (PF_FpLong)PixelTraits<T>::max_chan();

	// ノイズ処理
	if (infoP->noise > 0) {
		PF_FpLong r = (PF_FpLong)outP->red / max_val;
		PF_FpLong g = (PF_FpLong)outP->green / max_val;
		PF_FpLong b = (PF_FpLong)outP->blue / max_val;

		PF_FpLong ar = 0;
		PF_FpLong ag = 0;
		PF_FpLong ab = 0;

		ar =infoP->noise - 2*(PF_FpLong)hash_float( xL , yL,infoP->seed)* infoP->noise; // 座標に基づくハッシュでランダム値を変化させる
		if (infoP->isColor) {
			ag = infoP->noise - 2 * (PF_FpLong)hash_float(xL, yL, infoP->seed +1000) * infoP->noise; // 座標に基づくハッシュでランダム値を変化させる
			ab = infoP->noise - 2 * (PF_FpLong)hash_float(xL, yL, infoP->seed +3000) * infoP->noise; // 座標に基づくハッシュでランダム値を変化させる
		}
		else {
			ag = ar; // モノクロノイズの場合は同じ値を使用
			ab = ar;
		}
		// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
		if (std::is_same<T, PF_PixelFloat>::value) {
			outP->red = static_cast<typename PixelTraits<T>::CT>AE_CLAMP((r + ar),0,5);
			outP->green = static_cast<typename PixelTraits<T>::CT>AE_CLAMP((g + ag), 0, 5);
			outP->blue = static_cast<typename PixelTraits<T>::CT>AE_CLAMP((b + ab), 0, 5);
		}
		else {
			// 整数型の場合はクランプしてから適切な型にキャスト
			outP->red = static_cast<typename PixelTraits<T>::CT>(AE_CLAMP((r + ar) * max_val, 0, max_val));
			outP->green = static_cast<typename PixelTraits<T>::CT>(AE_CLAMP((g + ag) * max_val, 0, max_val));
			outP->blue = static_cast<typename PixelTraits<T>::CT>(AE_CLAMP((b + ab) * max_val, 0, max_val));
		}
	}

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err Noise8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return NoiseT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err Noise16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return NoiseT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err NoiseFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return NoiseT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err NoiseExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_FpLong noise,
	PF_Boolean isColor,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;
	NoiseInfo info;
	AEFX_CLR_STRUCT(info);
	info.noise = noise;
	info.isColor = isColor;
	info.seed = seed;	
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			output->width,
			input,
			NULL,
			&info,
			NoiseFloat,
			output);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			output->width,
			input,
			NULL,
			&info,
			Noise16,
			output);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			output->width,
			input,
			NULL,
			&info,
			Noise8,
			output);
		break;
	}
	return err;
}
