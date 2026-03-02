#include "NF_VectorMask.h"

template <typename T>
struct THInfo {
	PF_EffectWorld* world;
	A_long width;
	A_long widthTrue;
	A_long height;
	A_long rowbytes;
	T target_color;
	float level;
	std::vector<std::vector<float>>* mask;
};

template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}
// -- - 1. 型解決用のトレイト(PixelTraits) -- -
template <typename T> struct PixelTraits;

template <> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char CT;
	static inline CT half() { return 128; }
	static inline CT max_chan() { return 255; }
	static inline CT clamp(float v) { return (CT)AE_CLAMP(v + 0.5f, 0, 255); }
};

template <> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short CT;
	static inline CT half() { return 16384; } // AE 16bitは32768が上限
	static inline CT max_chan() { return 32768; }
	static inline CT clamp(float v) { return (CT)AE_CLAMP(v + 0.5f, 0, 32768); }
};

template <> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort CT;
	static inline CT half() { return 0.5f; }
	static inline CT max_chan() { return 1.0f; }
	static inline CT clamp(float v) { return (CT)v; } // Floatは基本クランプ不要
};

// ***********************************************************************************
// ***********************************************************************************
template <typename T,typename channelType>
static PF_Err DrawMaskT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	THInfo<T>* infoP = reinterpret_cast<THInfo<T>*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用


	T* outRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	for (A_long x = 0; x < infoP->width; x++) {
		//float luma = *(infoP->mask->data() + y * infoP->width + x); // マスクから輝度を取得
		float luma = (float)infoP->mask->at(y).at(x); // マスクから輝度を取得
		T* outP = &outRow[x];
		if (std::is_same<T, PF_PixelFloat>::value) {
			// 32bit float の場合はクランプ不要
			outP->red = static_cast<channelType>(luma);
			outP->green = static_cast<channelType>(luma);
			outP->blue = static_cast<channelType>(luma);
		}
		else {
			// 整数型の場合はクランプしてから適切な型にキャスト
			outP->red = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
			outP->green = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
			outP->blue = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
		}
		outP->alpha = static_cast<channelType>(max_chan);

	}






	return PF_Err_NONE;
}
PF_Err DrawMask(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask
)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
	{
		THInfo <PF_PixelFloat>info;
		AEFX_CLR_STRUCT(info);
		info.width = output->width;
		info.height = output->height;
		info.rowbytes = output->rowbytes;
		info.world = output;
		info.mask = mask;
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_PixelFloat, PF_FpShort>));
		break;
	}
	case PF_PixelFormat_ARGB64:
	{
		THInfo <PF_PixelFloat>info;
		AEFX_CLR_STRUCT(info);
		info.width = output->width;
		info.height = output->height;
		info.rowbytes = output->rowbytes;
		info.world = output;
		info.mask = mask;
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_Pixel16, A_u_short>));
		break;
	}
	case PF_PixelFormat_ARGB32:
	{
		THInfo <PF_PixelFloat>info;
		AEFX_CLR_STRUCT(info);
		info.width = output->width;
		info.height = output->height;
		info.rowbytes = output->rowbytes;
		info.world = output;
		info.mask = mask;
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_Pixel, A_u_char>));
		break;
	}
	}
	return err;
}
// ***********************************************************************************
// ***********************************************************************************
template <typename T, typename channelType>
static PF_Err DrawColorMaskT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	THInfo<T>* infoP = reinterpret_cast<THInfo<T>*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>();

	T* outRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	for (A_long x = 0; x < infoP->width; x++) {
		float luma = (float)infoP->mask->at(y).at(x);
		T* outP = &outRow[x];

		if (luma >= 1.0f)
		{
			*outP = infoP->target_color;
		}
		else if (luma <= 0.0f) {
			// 何もしない
		}else if (outP->alpha == 0) {
			// 出力が完全に透明なら、マスクの値をそのまま適用
			outP->red   = static_cast<channelType>(AE_CLAMP(infoP->target_color.red, 0, max_chan));
			outP->green = static_cast<channelType>(AE_CLAMP(infoP->target_color.green, 0, max_chan));
			outP->blue  = static_cast<channelType>(AE_CLAMP(infoP->target_color.blue, 0, max_chan));
			outP->alpha = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
		}	
		else {
			// 正規化値 [0,1] のままブレンド
			float a  = luma;
			float r  = (float)infoP->target_color.red   / (float)max_chan;
			float g  = (float)infoP->target_color.green / (float)max_chan;
			float b  = (float)infoP->target_color.blue  / (float)max_chan;

			float ao = (float)outP->alpha / (float)max_chan;
			float ro = (float)outP->red   / (float)max_chan;
			float go = (float)outP->green / (float)max_chan;
			float bo = (float)outP->blue / (float)max_chan;

			// Porter-Duff "over" — a は [0,1] のまま計算
			float a_new = a + ao - a * ao;
			float r_new = r * a + ro * (1.0f - a);
			float g_new = g * a + go * (1.0f - a);
			float b_new = b * a + bo * (1.0f - a);

			float aj = 1/a_new;
			// 最後にまとめてスケール & クランプ
			outP->red = static_cast<channelType>(AE_CLAMP(r_new * max_chan, 0, max_chan));
			outP->green = static_cast<channelType>(AE_CLAMP(g_new * max_chan, 0, max_chan));
			outP->blue  = static_cast<channelType>(AE_CLAMP(b_new * max_chan, 0, max_chan));
			outP->alpha = static_cast<channelType>(AE_CLAMP(a_new * max_chan, 0, max_chan));
		}
	}

	return PF_Err_NONE;
}
PF_Err DrawColorMask(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask,
	PF_Pixel target_color
)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
	{
		THInfo <PF_PixelFloat>info;
		AEFX_CLR_STRUCT(info);
		info.width = output->width;
		info.height = output->height;
		info.rowbytes = output->rowbytes;
		info.world = output;
		info.mask = mask;
		info.target_color = NF_Pixel8To32(target_color);
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawColorMaskT<PF_PixelFloat, PF_FpShort>));
		break;
	}
	case PF_PixelFormat_ARGB64:
	{
		THInfo <PF_Pixel16>info;
		AEFX_CLR_STRUCT(info);
		info.width = output->width;
		info.height = output->height;
		info.rowbytes = output->rowbytes;
		info.world = output;
		info.mask = mask;
		info.target_color = NF_Pixel8To16(target_color);
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawColorMaskT<PF_Pixel16, A_u_short>));
		break;
	}
	case PF_PixelFormat_ARGB32:
	{
		THInfo <PF_Pixel8>info;
		AEFX_CLR_STRUCT(info);
		info.width = output->width;
		info.height = output->height;
		info.rowbytes = output->rowbytes;
		info.world = output;
		info.mask = mask;
		info.target_color = target_color;
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawColorMaskT<PF_Pixel, A_u_char>));
		break;
	}
	}
	return err;
}
