#include "Threshold.h"
// -- - 1. 型解決用のトレイト(PixelTraits) -- -
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
// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0f;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0f;
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
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	if(infoP->mode==1)
	{
		// Lumaモード
		if (infoP->enabled[CHAN_LUMA]) {
			PF_FpLong luma = (0.2126 * outP->red + 0.7152 * outP->green + 0.0722 * outP->blue) / max_val;
			if (luma >= infoP->level[CHAN_LUMA]) {
				outP->red = outP->green = outP->blue = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				outP->red = outP->green = outP->blue = static_cast<typename PixelTraits<T>::channel_type>(0);
			}
		}
	}
	else {
		if (infoP->enabled[CHAN_R])
		{
			PF_FpLong level = (PF_FpLong)outP->red / max_val;
			if (level >= infoP->level[CHAN_R]) {
				outP->red = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				outP->red = static_cast<typename PixelTraits<T>::channel_type>(0);
			}
		}
		if (infoP->enabled[CHAN_G])
		{
			PF_FpLong level = (PF_FpLong)outP->green / max_val;
			if (level >= infoP->level[CHAN_G]) {
				outP->green = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				outP->green = static_cast<typename PixelTraits<T>::channel_type>(0);
			}
		}
		if (infoP->enabled[CHAN_B])
		{
			PF_FpLong level = (PF_FpLong)outP->blue / max_val;
			if (level >= infoP->level[CHAN_B]) {
				outP->blue = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				outP->blue = static_cast<typename PixelTraits<T>::channel_type>(0);
			}
		}
	}

	
	if (infoP->enabled[CHAN_A])
	{
		PF_FpLong level = (PF_FpLong)outP->alpha / max_val;
		if (level >= infoP->level[CHAN_A]) {
			outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
		}
		else {
			outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(0);
		}
	}
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

PF_Err FilterImage(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	ParamInfo *infoP
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
