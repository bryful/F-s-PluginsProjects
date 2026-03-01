#include "Star.h"

//typedef 
struct SBInfo {
	A_long		color_mode;        // 1: Original, 2: Custom
	PF_PixelFloat color32;     // 0.0-1.0 normalized
	std::vector<std::vector<float>>* mask;
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
template <typename T,typename CT>
static PF_Err StarBlendT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	SBInfo* infoP = reinterpret_cast<SBInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	float aa = infoP->mask->at(yL).at(xL);
	if (aa <= 0.0f) {
		return PF_Err_NONE;
	}
	PF_FpLong r = 0;
	PF_FpLong g = 0;
	PF_FpLong b = 0;
	PF_FpLong a = 0;
	if (infoP->color_mode == 1)
	{
		r = (PF_FpLong)inP->red/ max_val;
		g = (PF_FpLong)inP->green / max_val;
		b = (PF_FpLong)inP->blue / max_val;
	}
	else {
		r = infoP->color32.red;
		g = infoP->color32.green;
		b = infoP->color32.blue;
	}
	
	r = AE_CLAMP(aa * (1 + r) * 0.5, 0, 1.0f);
	g = AE_CLAMP(aa * (1 + g) * 0.5, 0, 1.0f);
	b = AE_CLAMP(aa * (1 + b) * 0.5, 0, 1.0f);
	a = aa;
	PF_FpLong ro = (PF_FpLong)outP->red / max_val;
	PF_FpLong go = (PF_FpLong)outP->green / max_val;
	PF_FpLong bo = (PF_FpLong)outP->blue / max_val;
	PF_FpLong ao = (PF_FpLong)outP->alpha / max_val;
	r = ro + r;// -ro * r;
	if (r > 1.0f) r = 1.0f;
	g = go + g;// -go * g;
	if (g > 1.0f) g = 1.0f;
	b = bo + b;// -bo * b;
	if (b > 1.0f) b = 1.0f;
	a = ao + a;// -ao * a;
	if (a > 1.0f) a = 1.0f;

	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<CT>(r);
		outP->green = static_cast<CT>(g);
		outP->blue = static_cast<CT>(b);
		outP->alpha = static_cast<CT>(a);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<CT>(AE_CLAMP(r* max_val, 0, max_val));
		outP->green = static_cast<CT>(AE_CLAMP(g * max_val, 0, max_val));
		outP->blue = static_cast<CT>(AE_CLAMP(b * max_val, 0, max_val));
		outP->alpha = static_cast<CT>(AE_CLAMP(a * max_val, 0, max_val));

	}

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err StarBlend8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return StarBlendT<PF_Pixel8, A_u_char>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err StarBlend16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return StarBlendT<PF_Pixel16,A_u_short>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err StarBlendFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return StarBlendT<PF_PixelFloat,PF_FpShort>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err StarBlend(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<std::vector<float>> *mask
)
{
	PF_Err err = PF_Err_NONE;
	SBInfo sbInfo;
	AEFX_CLR_STRUCT(sbInfo);
	sbInfo.mask = mask;
	sbInfo.color_mode = infoP->color_mode;
	sbInfo.color32 = infoP->cross_color;

	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		err = ae->suitesP->IterateFloatSuite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			&sbInfo,
			StarBlendFloat,
			ae->output);
		break;
	case PF_PixelFormat_ARGB64:
		err = ae->suitesP->Iterate16Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			&sbInfo,
			StarBlend16,
			ae->output);
		break;
	case PF_PixelFormat_ARGB32:
		err = ae->suitesP->Iterate8Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			&sbInfo,
			StarBlend8,
			ae->output);
		break;
	}
	return err;
}
