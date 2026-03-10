#include "noiseFilter.h"
struct NoiseInfo {
	float	noise;
	PF_Boolean	isColor;
	A_long		seed;

};


// *******************************************************************************

// --- 5. 共通フィルタテンプレート ---
template <typename T,typename CT,CT max_val>
static PF_Err NoiseT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	NoiseInfo* infoP = reinterpret_cast<NoiseInfo*>(refcon);

	// ノイズ処理
	if (infoP->noise > 0) {
		float r = (float)outP->red / max_val;
		float g = (float)outP->green / max_val;
		float b = (float)outP->blue / max_val;

		float ar = 0.0f;
		float ag = 0.0f;
		float ab = 0.0f;

		ar =infoP->noise - 2.0f*(float)hash_float( xL , yL,infoP->seed)* infoP->noise; // 座標に基づくハッシュでランダム値を変化させる
		if (infoP->isColor) {
			ag = infoP->noise - 2.0f * (float)hash_float(xL, yL, infoP->seed +1000) * infoP->noise; // 座標に基づくハッシュでランダム値を変化させる
			ab = infoP->noise - 2.0f * (float)hash_float(xL, yL, infoP->seed +3000) * infoP->noise; // 座標に基づくハッシュでランダム値を変化させる
		}
		else {
			ag = ar; // モノクロノイズの場合は同じ値を使用
			ab = ar;
		}
		// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
		outP->red = static_cast<CT>AE_CLAMP((r + ar)*max_val, 0, max_val);
		outP->green = static_cast<CT>AE_CLAMP((g + ag) * max_val, 0, max_val);
		outP->blue = static_cast<CT>AE_CLAMP((b + ab) * max_val, 0, max_val);
	}

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err Noise8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return NoiseT<PF_Pixel8,A_u_char,255>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err Noise16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return NoiseT<PF_Pixel16,A_u_short,PF_MAX_CHAN16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err NoiseFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return NoiseT<PF_PixelFloat,PF_FpShort,1.0f>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err NoiseExec(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	float noise,
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
			output,
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
			output,
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
			output,
			NULL,
			&info,
			Noise8,
			output);
		break;
	}
	return err;
}
