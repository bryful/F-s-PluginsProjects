#include "NF_SelectPixels.h"


// --- ビット深度ごとの型変換を吸収するヘルパー ---
inline PF_Pixel NF_To8(PF_Pixel c) { return c; }
inline PF_Pixel NF_To8(PF_Pixel16 c) { return NF_Pixel16To8(c); }
inline PF_Pixel NF_To8(PF_PixelFloat c) { return NF_Pixel32To8(c); }

template <typename T> struct ECTraits;

template <> struct ECTraits<PF_Pixel8> {
	typedef A_u_char comp;
	static inline comp half() { return 128; }
	static inline comp max_C() { return 255; }
	//static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 255); }
};

template <> struct ECTraits<PF_Pixel16> {
	typedef A_u_short comp;
	static inline comp half() { return 16384; } // AE 16bitは32768が上限
	static inline comp max_C() { return 32768; }
	//static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 32768); }
};

template <> struct ECTraits<PF_PixelFloat> {
	typedef PF_FpShort comp;
	static inline comp half() { return 0.5f; }
	static inline comp max_C() { return 1.0f; }
	//static inline comp clamp(float v) { return (comp)v; } // Floatは基本クランプ不要
};

// テンプレート用の情報を保持する構造体
template <typename T>
struct SPInfo {
	A_long		mode;
	A_long		count;
	PF_Pixel	target_colors[SP_COLOR_TABLE_MAX];
	//T			new_colors[SP_COLOR_TABLE_MAX];
	//T			newColor;
};

//-------------------------------------------------------------------------------------------------
// テンプレート化したピクセルサンプリング関数
//-------------------------------------------------------------------------------------------------
template <typename T>
static PF_Err
SelectPixels_Template(
	void* refcon,
	A_long		xL,
	A_long		yL,
	T* inP,
	T* outP)
{
	PF_Err			err = PF_Err_NONE;
	SPInfo<T>* infoP = reinterpret_cast<SPInfo<T>*>(refcon);
	typedef ECTraits<T> Traits;
	//if (!infoP) return PF_Err_INVALID_CALLBACK_PARAM;

	// オーバーロードされた NF_To8 により、T の型に応じた関数が呼ばれます
	PF_Pixel src = NF_To8(*inP);
	int idx = -1;
	if (infoP->count > 0) {
		// 置換対象の色が無い場合は処理しない
		for (A_long i = 0; i < infoP->count; i++) {
			if (
				src.red == infoP->target_colors[i].red &&
				src.green == infoP->target_colors[i].green &&
				src.blue == infoP->target_colors[i].blue
				) {
				idx = i;
				break;
			}
		}
	}
	else {
		if (inP->alpha == Traits::max_C()) {
			idx = 0;
		}
	}
	if (idx < 0) {
		*outP = { 0,0,0,0 };
	}
	else if (infoP->mode == SelectPixelMode::SP_MODE_ORIGINAL)
	{
		*outP = *inP;
	}
	else if (infoP->mode == SelectPixelMode::SP_MODE_MASK)
	{
		outP->red = Traits::max_C();
		outP->green = Traits::max_C();
		outP->blue = Traits::max_C();
		outP->alpha = Traits::max_C();
	}
	
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err SelectPixelsExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long count,
	PF_Pixel* colors,
	A_long mode
)
{
	PF_Err err = PF_Err_NONE;
	SPInfo<PF_Pixel> info8;
	AEFX_CLR_STRUCT(info8);
	info8.mode = mode;
	if (mode < SelectPixelMode::SP_MODE_ORIGINAL) mode = SelectPixelMode::SP_MODE_ORIGINAL;
	else if (mode > SelectPixelMode::SP_MODE_MASK) mode = SelectPixelMode::SP_MODE_MASK;

	int cnt = count;
	if (cnt > SP_COLOR_TABLE_MAX) cnt = SP_COLOR_TABLE_MAX;
	if (cnt > 0) {
		for (int i = 0; i < cnt; i++)
		{
			info8.target_colors[i] = colors[i];
		}
		info8.count = cnt;
	}
	else {
		for (int i = 0; i < cnt; i++)
		{
			info8.target_colors[i] = {0,0,0,0};
		}
		info8.count = 0;
	}

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
	{
		suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			output->height,
			input, 
			NULL,
			&info8, 
			SelectPixels_Template<PF_PixelFloat>, 
			output);
	}
	break;

	case PF_PixelFormat_ARGB64:
	{
		suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			output->height,
			input,
			NULL,
			&info8,
			SelectPixels_Template<PF_Pixel16>,
			output);
	}
	break;

	case PF_PixelFormat_ARGB32:
	{
		suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			output->height,
			input,
			NULL,
			&info8,
			SelectPixels_Template<PF_Pixel>,
			output);
	}
	break;
	}
	return err;
}