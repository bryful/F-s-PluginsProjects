#include "NF_Blend.h"

// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}
template <typename T> struct PixelTraits;

template <> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char channel_type;
};

template <> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short channel_type;
};

template <> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort channel_type;
};

// --- 5. 共通フィルタテンプレート ---
template <typename T>
static PF_Err BlendBehindT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	PF_FpLong max_val = GetMaxChannel<T>();
	PF_FpLong a;
	PF_FpLong r;
	PF_FpLong g;
	PF_FpLong b;
	
	if (outP->alpha == 0) {
		*outP = *inP;
		return PF_Err_NONE;
	}else if (inP->alpha == 0|| outP->alpha == max_val) {
		return PF_Err_NONE;
	}else if (inP->alpha == max_val) {
		PF_FpLong av = (PF_FpLong)outP->alpha / max_val;
		r = (PF_FpLong)outP->red * av + (PF_FpLong)inP->red *(1-av);
		g = (PF_FpLong)outP->green *av + (PF_FpLong)inP->green * (1 - av);
		b = (PF_FpLong)outP->blue *av + (PF_FpLong)inP->blue * (1 - av);
		a = max_val;
	}
	else {

		PF_FpLong src_a = (PF_FpLong)inP->alpha / max_val;
		PF_FpLong dst_a = (PF_FpLong)outP->alpha / max_val;
		a = src_a * (1 - dst_a) + dst_a ;
		r = ((PF_FpLong)inP->red * src_a * (1 - dst_a)) / a + (PF_FpLong)outP->red * dst_a;
		g = ((PF_FpLong)inP->green * src_a * (1 - dst_a)) / a + (PF_FpLong)outP->green * dst_a;
		b = ((PF_FpLong)inP->blue * src_a * (1 - dst_a)) / a + (PF_FpLong)outP->blue * dst_a;
		a *= max_val;
	}

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(r);
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(g);
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(b);
		outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(a);

	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(r, 0, max_val));
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(g, 0, max_val));
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(b, 0, max_val));
		outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(a, 0, max_val));
	}


	return PF_Err_NONE;
}

// 8-bit用
static PF_Err BlendBehind8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return BlendBehindT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err BlendBehind16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return BlendBehindT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err BlendBehindFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return BlendBehindT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err BlendBehind(
	PF_InData* in_dataP,
	PF_EffectWorld* src,
	PF_EffectWorld* dst,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP
)
{
	PF_Err err = PF_Err_NONE;

	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			dst->width,
			src,
			NULL,
			NULL,
			BlendBehindFloat,
			dst);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(
			in_dataP,
			0,
			dst->width,
			src,
			NULL,
			NULL,
			BlendBehind16,
			dst);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(
			in_dataP,
			0,
			dst->width,
			src,
			NULL,
			NULL,
			BlendBehind8,
			dst);
		break;
	}
	return err;
}
