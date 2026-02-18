#include "Star.h"


// 輝度計算ヘルパー (RGBの最大値を基準にする仕様)
template <typename T>
inline PF_FpLong GetLuminance(T* p) {
	PF_FpLong r = (PF_FpLong)p->red;
	PF_FpLong g = (PF_FpLong)p->green;
	PF_FpLong b = (PF_FpLong)p->blue;
	// RGBの最大値を輝度とする（仕様に基づき）
	PF_FpLong max_val = (r > g) ? r : g;
	return (max_val > b) ? max_val : b;
}

// ピクセルごとの抽出処理
template <typename T>
static PF_Err ExtractLumaT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用

	PF_FpLong aa = (PF_FpLong)inP->alpha/max_chan;
	PF_FpLong r = (PF_FpLong)inP->red * aa;
	PF_FpLong g = (PF_FpLong)inP->green * aa;
	PF_FpLong b = (PF_FpLong)inP->blue * aa;
	*outP = *inP; // 元のピクセルをコピー
	PF_FpLong max_val = AE_CLAMP(MAX(MAX(r,g),b),0,max_chan);
	outP->alpha = (typename PixelTraits<T>::channel_type)max_val;
	outP->red = (typename PixelTraits<T>::channel_type)r;
	outP->green = (typename PixelTraits<T>::channel_type)g;
	outP->blue = (typename PixelTraits<T>::channel_type)b;

	/*

	// 現在のピクセルの輝度取得 (0.0 - 1.0)
	PF_FpLong luma = GetLuminance<T>(inP) / max_chan;
	PF_FpLong alpha = (PF_FpLong)inP->alpha / max_chan;


	outP->alpha = (typename PixelTraits<T>::channel_type)(max_val);

	// 閾値による重み計算 (Linear Step)
	PF_FpLong weight = 0;
	if (luma >= infoP->thresh_high) {
		weight = 1.0;
	}
	else if (luma > infoP->thresh_low) {
		weight = (luma - infoP->thresh_low) / (infoP->thresh_high - infoP->thresh_low);
	}

	// アルファを考慮
	weight *= alpha;

	if (weight > 0) {
		if (infoP->color_mode == 2) { // Custom Color
			outP->red = (typename PixelTraits<T>::channel_type)(infoP->cross_color.red * max_chan * weight);
			outP->green = (typename PixelTraits<T>::channel_type)(infoP->cross_color.green * max_chan * weight);
			outP->blue = (typename PixelTraits<T>::channel_type)(infoP->cross_color.blue * max_chan * weight);
		}
		else { // Original Color
			outP->red = (typename PixelTraits<T>::channel_type)(inP->red * weight);
			outP->green = (typename PixelTraits<T>::channel_type)(inP->green * weight);
			outP->blue = (typename PixelTraits<T>::channel_type)(inP->blue * weight);
		}
		outP->alpha = (typename PixelTraits<T>::channel_type)(max_chan * weight);
	}
	else {
		outP->red = outP->green = outP->blue = outP->alpha = 0;
	}
	*/
	return PF_Err_NONE;
}

PF_Err ExtractLumaMask(
	PF_InData* in_dataP,
	PF_EffectWorld* inputP,
	PF_EffectWorld* maskP, // 書き出し先（事前にNewWorldしたもの）
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(in_dataP, 0, inputP->height, inputP, NULL, infoP, ExtractLumaT<PF_PixelFloat>, maskP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(in_dataP, 0, inputP->height, inputP, NULL, infoP, ExtractLumaT<PF_Pixel16>, maskP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(in_dataP, 0, inputP->height, inputP, NULL, infoP, ExtractLumaT<PF_Pixel8>, maskP);
		break;
	}
	return err;
}
template <typename T>
void BuildLitPixelListT(PF_EffectWorld* mask, std::vector<LitPixel>& list) {
	PF_FpLong max_chan = GetMaxChannel<T>();
	for (A_long y = 0; y < mask->height; y++) {
		T* row = reinterpret_cast<T*>((char*)mask->data + (y * mask->rowbytes));
		for (A_long x = 0; x < mask->width; x++) {
			if (row[x].alpha > 0) {
				LitPixel lp;
				lp.x = x;
				lp.y = y;
				lp.color.red = (PF_FpShort)row[x].red / max_chan;
				lp.color.green = (PF_FpShort)row[x].green / max_chan;
				lp.color.blue = (PF_FpShort)row[x].blue / max_chan;
				lp.color.alpha = (PF_FpShort)row[x].alpha / max_chan;
				list.push_back(lp);
			}
		}
	}
}