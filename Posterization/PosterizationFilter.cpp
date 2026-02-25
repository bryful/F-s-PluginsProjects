#include "PosterizationFilter.h"
typedef struct FilterInfo {
	A_long level;

} FilterInfo;

#define COLOR_TABLE_MAX	15

typedef struct PosInfo {
	A_long level;
	A_long color_count;
	PF_Pixel colors08[COLOR_TABLE_MAX];
	PF_Pixel16 colors16[COLOR_TABLE_MAX];
	PF_PixelFloat colors32[COLOR_TABLE_MAX];
	A_long mode;
	PF_Pixel bcolors08;
	PF_Pixel16 bcolors16;
	PF_PixelFloat bcolors32;

} PosInfo;

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
static PF_Err PosterizationExecT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	FilterInfo* infoP = reinterpret_cast<FilterInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();
	A_long LevelMax = infoP->level - 1;
	// 色の加算処理
	PF_FpLong av = (PF_FpLong)outP->alpha / max_val; // アルファを0.0-1.0に正規化
	PF_FpLong r = (PF_FpLong)outP->red * av / max_val;
	PF_FpLong g = (PF_FpLong)outP->green * av / max_val;
	PF_FpLong b = (PF_FpLong)outP->blue * av / max_val;
	PF_FpLong v = AE_CLAMP((0.29891 * r + 0.58661 * g + 0.11448 * b),0,1.0f);
	A_long level = (A_long)(v*(PF_FpLong)(LevelMax) + 0.5);
	PF_FpLong rlevel = ((PF_FpLong)level / (PF_FpLong)(LevelMax))*max_val;

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(rlevel);
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(rlevel);
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(rlevel);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(rlevel, 0, max_val));
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(rlevel, 0, max_val));
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(rlevel, 0, max_val));
	}
	outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err PosterizationExec8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return PosterizationExecT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err PosterizationExec16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP) 
{
	return PosterizationExecT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err PosterizationExecFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return PosterizationExecT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err PosterizationExec(
	PF_InData* in_dataP,
	//PF_EffectWorld* inputP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long level
)
{
	PF_Err err = PF_Err_NONE;
	FilterInfo info;
	AEFX_CLR_STRUCT(info);
	info.level = level;
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			PosterizationExecFloat,
			outputP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			PosterizationExec16,
			outputP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			PosterizationExec8,
			outputP);
		break;
	}
	return err;
}
// ********************************************************************************
// ********************************************************************************
// --- 5. 共通フィルタテンプレート ---
template <typename T>
static PF_Err PostExecT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	PosInfo* infoP = reinterpret_cast<PosInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();
	A_long LevelMax = infoP->level - 1;

	A_long border = -1;
	if( infoP->mode == 2|| infoP->mode == 3) {
		if (ABS(((float)outP->green - (float)outP->blue) / max_val) >= 0.01f)
		{
			border = 1;
		}
	}

	A_long ColorIndex = -1;
	T c0 = { 0,0,0 ,0 };
	if (infoP->mode == 1 || infoP->mode == 2) {
		if (border == -1) {
			border = 0;
			if (infoP->color_count > 0) {
				ColorIndex = (A_long)((PF_FpLong)LevelMax * (PF_FpLong)outP->red / max_val + 0.5);
			}
			else {
				c0.red = outP->red;
				c0.green = outP->red;
				c0.blue = outP->red;
				c0.alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
		}
	}

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if constexpr (std::is_same<T, PF_PixelFloat>::value) {
		if (border==1) {
			*outP = infoP->bcolors32;
			outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
		}else if( border == 0) {
			if (ColorIndex >= 0) {
				*outP = infoP->colors32[ColorIndex];
				outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				*outP = c0;
				outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
		}
		else {
			*outP = { 0,0,0,0 };
		}
	}
	else if constexpr (std::is_same<T, PF_Pixel16>::value) {
		if (border == 1) {
			*outP = infoP->bcolors16;
			outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
		}
		else if (border == 0) {
			if (ColorIndex >= 0) {
				*outP = infoP->colors16[ColorIndex];
				outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				*outP = c0;
				outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
		}
		else {
			*outP = { 0,0,0,0 };
		}
	}
	else {
		if (border == 1) {
			*outP = infoP->bcolors08;
			outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
		}
		else if (border == 0) {
			if (ColorIndex >= 0) {
				*outP = infoP->colors08[ColorIndex];
				outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
			else {
				*outP = c0;
				outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);
			}
		}
		else {
			*outP = { 0,0,0,0 };
		}
	
	}
	//outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_val);

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err PostExec8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return PostExecT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err PostExec16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return PostExecT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err PostExecFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return PostExecT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}
PF_Err PostExec(
	PF_InData* in_dataP,
	//PF_EffectWorld* inputP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long level,
	A_long color_count,
	PF_Pixel *colors,
	A_long mode,
	PF_Pixel bcolors
	)
{
	PF_Err err = PF_Err_NONE;
	PosInfo info;
	AEFX_CLR_STRUCT(info);
	info.level = level;
	info.color_count = color_count;
	info.mode = mode;
	if (color_count > 0) {
		for (int i = 0; i < color_count; i++) {
			info.colors08[i] = colors[i];
			info.colors16[i] = NF_Pixel8To16(colors[i]);
			info.colors32[i] = NF_Pixel8To32(colors[i]);
		}
	}
	info.bcolors08 = bcolors;
	info.bcolors16 = NF_Pixel8To16(bcolors);
	info.bcolors32 = NF_Pixel8To32(bcolors);
	
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			PostExecFloat,
			outputP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			PostExec16,
			outputP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			PostExec8,
			outputP);
		break;
	}
	return err;
}

