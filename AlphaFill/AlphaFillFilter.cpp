#include "AlphaFill.h"

// *******************************************************************************
// チャンネル型を取得するヘルパー
template <typename T>
struct ChannelTypeHelper {
	using type = A_u_char;
};

template <>
struct ChannelTypeHelper<PF_Pixel16> {
	using type = A_u_short;
};

template <>
struct ChannelTypeHelper<PF_PixelFloat> {
	using type = PF_FpShort;
};

// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}

// --- 5. 共通フィルタテンプレート ---
template <typename T>
static PF_Err AlphaFillT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	using ChannelType = typename ChannelTypeHelper<T>::type;
	
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	if(outP->alpha == 0) {
		// 型に応じて適切な色を取得
		if (std::is_same<T, PF_PixelFloat>::value) {
			*outP = *reinterpret_cast<T*>(&infoP->colorFloat);
		}else if (std::is_same<T, PF_Pixel16>::value) {
			*outP = *reinterpret_cast<T*>(&infoP->color16);
		}
		else {
			*outP = *reinterpret_cast<T*>(&infoP->color);
		}
		return PF_Err_NONE;
	}else if (outP->alpha == max_val) {
		return PF_Err_NONE;
	}
	
	// 型に応じて適切な色を取得
	T col;
	if (std::is_same<T, PF_PixelFloat>::value) {
		col = *reinterpret_cast<T*>(&infoP->colorFloat);
	} else if (std::is_same<T, PF_Pixel16>::value) {
		col = *reinterpret_cast<T*>(&infoP->color16);
	} else {
		col = *reinterpret_cast<T*>(&infoP->color);
	}
	
	PF_FpLong aov = static_cast<PF_FpLong>(outP->alpha) / max_val;
	PF_FpLong ro = static_cast<PF_FpLong>(outP->red) * aov 
		+ static_cast<PF_FpLong>(col.red) *(1-aov);
	PF_FpLong go = static_cast<PF_FpLong>(outP->green) * aov 
		+ static_cast<PF_FpLong>(col.green) * (1 - aov);
	PF_FpLong bo = static_cast<PF_FpLong>(outP->blue) * aov 
		+ static_cast<PF_FpLong>(col.blue) * (1 - aov);


	// 出力処理
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<ChannelType>(ro);
		outP->green = static_cast<ChannelType>(go);
		outP->blue = static_cast<ChannelType>(bo);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<ChannelType>(AE_CLAMP(ro, 0, max_val));
		outP->green = static_cast<ChannelType>(AE_CLAMP(go, 0, max_val));
		outP->blue = static_cast<ChannelType>(AE_CLAMP(bo, 0, max_val));
	}

	outP->alpha = static_cast<ChannelType>(max_val);

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err AlphaFill8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return AlphaFillT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err AlphaFill16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return AlphaFillT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err AlphaFillFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return AlphaFillT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err AlphaFillExec(
	NF_AE* ae,
	ParamInfo *infoP
)
{
	PF_Err err = PF_Err_NONE;
		
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		ERR(ae->suitesP->IterateFloatSuite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			infoP,
			AlphaFillFloat,
			ae->output));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->suitesP->Iterate16Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			infoP,
			AlphaFill16,
			ae->output));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->suitesP->Iterate8Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			infoP,
			AlphaFill8,
			ae->output));
		break;
	}
	return err;
}
