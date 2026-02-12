#include "WhiteKeyingFilter.h"
typedef struct FilterInfo {
	A_long		level;
	PF_Boolean	reverse;
	PF_Pixel	target_color;
} FilterInfo;

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
	if (outP->alpha == 0) {
		// もともと透明なピクセルは処理しない
		*outP = { 0,0,0,0 };
		return PF_Err_NONE;
	}
	PF_FpLong max_val = GetMaxChannel<T>();
	PF_Pixel src;
	PF_Pixel target = infoP->target_color;
	if (std::is_same<T, PF_PixelFloat>::value) {
		src = NF_Pixel32To8(*reinterpret_cast<PF_PixelFloat*>(outP));
	}
	else if (std::is_same<T, PF_Pixel16>::value) {
		src = NF_Pixel16To8(*reinterpret_cast<PF_Pixel16*>(outP));
	}
	else {
		src = *reinterpret_cast<PF_Pixel8*>(inP);
	}
	PF_FpLong col = 0;
	PF_Boolean match = FALSE;

	if (infoP->level <= 0) {
		match = ((src.red == target.red) &&
			(src.green == target.green) &&
			(src.blue == target.blue));
	}
	else {
		match = (ABS(src.red - target.red) <= infoP->level &&
			ABS(src.green - target.green) <= infoP->level &&
			ABS(src.blue - target.blue) <= infoP->level );
	}
	if(match)
	{
		// キーイング対象色に近い場合、透明にする
		col = 0;
	}
	else {
		col = max_val;
	}
	// リバースオプション
	if (infoP->reverse) {
		col = max_val - col;
	}
	if(col==0) {
		// 完全に透明にする場合はRGBも0にする
		outP->red = 0;
		outP->green = 0;
		outP->blue = 0;
	}
	outP->alpha = static_cast<decltype(outP->alpha)>(col);

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
	PF_Pixel col,
	PF_FpLong level,
	PF_Boolean reverse
)
{
	PF_Err err = PF_Err_NONE;
		
	FilterInfo info;
	info.target_color = col;
	info.level = (A_long)(level * (255/16) + 0.5);
	info.reverse = reverse;

	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			worldP->width,
			worldP,
			NULL,
			&info,
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
			&info,
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
			&info,
			FilterImage8,
			worldP);
		break;
	}
	return err;
}
