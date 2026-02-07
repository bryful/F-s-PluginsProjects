#include "_SkeletonFilter.h"

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
static PF_Err FilterImageT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	FilterInfo* infoP = reinterpret_cast<FilterInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	// 色の加算処理
	PF_FpLong r = (PF_FpLong)outP->red + (infoP->r * max_val);
	PF_FpLong g = (PF_FpLong)outP->green + (infoP->g * max_val);
	PF_FpLong b = (PF_FpLong)outP->blue + (infoP->b * max_val);

	// ノイズ処理
	if (infoP->noise > 0) {
		PF_FpLong rf =(PF_FpLong)hash_float( xL , yL,infoP->noise_offset); // 座標に基づくハッシュでランダム値を変化させる
		PF_FpLong random_factor = (rf * 2.0f) - 1.0f; // -1.0 ~ 1.0 に変換
		PF_FpLong vv = random_factor * infoP->noise * max_val;
		r += vv;
		g += vv;
		b += vv;
	}

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(r);
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(g);
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(b);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(r, 0, max_val));
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(g, 0, max_val));
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(b, 0, max_val));
	}

	//outP->alpha = inP->alpha; // アルファはそのまま維持

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err FilterImage8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return FilterImageT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err FilterImage16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP) 
{
	return FilterImageT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err FilterImageFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return FilterImageT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

static PF_Err FilterImageImpl(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	FilterInfo *infoP
)
{
	PF_Err err = PF_Err_NONE;
		
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			worldP->width,
			worldP,
			NULL,
			infoP,
			FilterImageFloat,
			worldP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(
			in_dataP,
			0,
			worldP->width,
			worldP,
			NULL,
			infoP,
			FilterImage16,
			worldP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(
			in_dataP,
			0,
			worldP->width,
			worldP,
			NULL,
			infoP,
			FilterImage8,
			worldP);
		break;
	}
	return err;
}
PF_Err FilterImage(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	FilterInfo* infoP
)
{
	return FilterImageImpl(in_dataP, worldP, pixelFormat, suitesP, infoP);
}