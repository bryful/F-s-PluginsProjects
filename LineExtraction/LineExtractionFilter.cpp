#include "LineExtractionFilter.h"
typedef struct {
	PF_Pixel			color;
	PF_Pixel16			color16;
	PF_PixelFloat		color32;

} FiltInfo;

// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}

// --- 5. 共通フィルタテンプレート ---
template <typename T, typename ChannelType>
static PF_Err FilterImageT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	//FiltInfo* infoP = reinterpret_cast<FiltInfo*>(refcon);
	ChannelType maxChannel = (ChannelType)GetMaxChannel<T>();
	
	PF_FpLong a = outP->alpha;
	PF_FpLong av = (PF_FpLong)outP->alpha / maxChannel;
	PF_FpLong mx = (PF_FpLong)MAX(outP->red, MAX(outP->green, outP->blue));
	PF_FpLong mn = (PF_FpLong)MIN(outP->red, MIN(outP->green, outP->blue));

	PF_FpLong lv = AE_CLAMP(av * (mx+mn)/2, 0, maxChannel);



	/*
	ChannelType alpha = inP->alpha;
	if (infoP->isWhite) {
		if (inP->red == maxChannel && inP->green == maxChannel && inP->blue == maxChannel) {
			alpha = 0;
		}
	}
	*/

	outP->alpha = (ChannelType)maxChannel;
	outP->red = (ChannelType)lv;
	outP->green = (ChannelType)lv;
	outP->blue = (ChannelType)lv;
	return PF_Err_NONE;
}

// 8-bit用
static PF_Err FilterImage8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return FilterImageT<PF_Pixel8,A_u_char>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err FilterImage16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP) 
{
	return FilterImageT<PF_Pixel16,A_u_short>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err FilterImageFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return FilterImageT<PF_PixelFloat,PF_FpShort>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err FilterImage(
	PF_InData* in_dataP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP
)
{
	PF_Err err = PF_Err_NONE;
	FiltInfo info;
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterImageFloat,
			outputP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterImage16,
			outputP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterImage8,
			outputP);
		break;
	}
	return err;
}
// *******************************************************************************
// --- 5. 共通フィルタテンプレート ---
template <typename T,typename ChannelType>
static PF_Err FilterLastImageT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	FilterInfo* infoP = static_cast<FilterInfo*>(refcon);

	PF_FpLong maxChannel = GetMaxChannel<T>();
	outP->alpha =  (ChannelType)AE_CLAMP((PF_FpLong)outP->red - (PF_FpLong)outP->green,0, maxChannel);

	if (outP->alpha == 0) {
		outP->red = 0;
		outP->green = 0;
		outP->blue = 0;
	}
	else {
		if constexpr (std::is_same<T, PF_Pixel8>::value) {
			outP->red = infoP->color.red;
			outP->green = infoP->color.green;
			outP->blue = infoP->color.blue;
		}
		else if constexpr (std::is_same<T, PF_Pixel16>::value)
		{
			outP->red = infoP->color16.red;
			outP->green = infoP->color16.green;
			outP->blue = infoP->color16.blue;
		}
		else {
			outP->red = infoP->color32.red;
			outP->green = infoP->color32.green;
			outP->blue = infoP->color32.blue;
		}
	}

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err FilterLastImage8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return FilterLastImageT<PF_Pixel8,A_u_char>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err FilterLastImage16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return FilterLastImageT<PF_Pixel16,A_u_short>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err FilterLastImageFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return FilterLastImageT<PF_PixelFloat,PF_FpShort>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err FilterLastImage(
	PF_InData* in_dataP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_Pixel color
)
{
	PF_Err err = PF_Err_NONE;

	FilterInfo info;
	info.color = color;
	info.color16 = NF_Pixel8To16(color);
	info.color32 = NF_Pixel8To32(color);

	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterLastImageFloat,
			outputP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterLastImage16,
			outputP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterLastImage8,
			outputP);
		break;
	}
	return err;
}
// *******************************************************************************
// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー


// --- 5. 共通フィルタテンプレート ---
template <typename T, typename ChannelType>
static PF_Err FilterSubImageT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	FiltInfo* infoP = reinterpret_cast<FiltInfo*>(refcon);
	ChannelType maxChannel = (ChannelType)GetMaxChannel<T>();

	PF_FpLong a = ABS(outP->red - outP->green);
	outP->alpha = (ChannelType)AE_CLAMP(a, 0, maxChannel);
	if constexpr (std::is_same<T, PF_Pixel8>::value) {
		outP->red = infoP->color.red;
		outP->green = infoP->color.green;
		outP->blue = infoP->color.blue;
	}
	else if constexpr (std::is_same<T, PF_Pixel16>::value)
	{
		outP->red = infoP->color16.red;
		outP->green = infoP->color16.green;
		outP->blue = infoP->color16.blue;
	}
	else {
		outP->red = infoP->color32.red;
		outP->green = infoP->color32.green;
		outP->blue = infoP->color32.blue;
	}
	return PF_Err_NONE;
}

// 8-bit用
static PF_Err FilterSubImage8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return FilterSubImageT<PF_Pixel8, A_u_char>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err FilterSubImage16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return FilterSubImageT<PF_Pixel16, A_u_short>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err FilterSubImageFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return FilterSubImageT<PF_PixelFloat, PF_FpShort>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err FilterSubImage(
	PF_InData* in_dataP,
	PF_EffectWorld* outputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	PF_Pixel color
)
{
	PF_Err err = PF_Err_NONE;
	FiltInfo info;
	info.color = color;
	info.color16 = NF_Pixel8To16(color);
	info.color32 = NF_Pixel8To32(color);
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterSubImageFloat,
			outputP);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterSubImage16,
			outputP);
		break;
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite1()->iterate(
			in_dataP,
			0,
			outputP->width,
			outputP,
			NULL,
			&info,
			FilterSubImage8,
			outputP);
		break;
	}
	return err;
}
// *******************************************************************************