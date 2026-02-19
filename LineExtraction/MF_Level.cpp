#include "LineExtraction.h"
// 内部用コンテキスト
struct LevelInfo {
	PF_FpLong lo;
	PF_FpLong hi;
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
static PF_Err LevelT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	LevelInfo* infoP = reinterpret_cast<LevelInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	// 色の加算処理
	PF_FpLong a = (PF_FpLong)outP->alpha / max_val;
	if (infoP->hi <= infoP->lo)return PF_Err_NONE; // hi <= lo の場合は処理しない
	if (a < infoP->lo) {
		a = 0;
	}
	else if (a >= infoP->hi) {
		a = 1;
	}
	else {
		a = (a - infoP->lo) / (infoP->hi - infoP->lo);
	}
	

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(a);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(a* max_val, 0, max_val));
	}

	//outP->alpha = inP->alpha; // アルファはそのまま維持

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err Level8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return LevelT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err Level16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return LevelT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err LevelFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return LevelT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err LevelExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_FpLong hi,
	PF_FpLong lo
)
{
	PF_Err err = PF_Err_NONE;
	
	LevelInfo info;
	info.hi = hi;
	info.lo = lo;
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			output->width,
			output,
			NULL,
			&info,
			LevelFloat,
			output);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(
			in_dataP,
			0,
			output->width,
			output,
			NULL,
			&info,
			Level16,
			output);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(
			in_dataP,
			0,
			output->width,
			output,
			NULL,
			&info,
			Level8,
			output);
		break;
	}
	return err;
}
