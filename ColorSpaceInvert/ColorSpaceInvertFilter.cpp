//#include "ColorSpaceInvertFilter.h"
#include "ColorSpaceInvert.h"
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
typedef struct {
	PF_FpLong	Y;
	PF_FpLong	U;
	PF_FpLong	V;
}YUV;
typedef struct {
	PF_FpLong	H;
	PF_FpLong	L;
	PF_FpLong	S;
}HLS;
// *******************************************************************************
inline YUV RgbToYuv(PF_FpLong r, PF_FpLong g, PF_FpLong b)
{
	YUV yuv;
	yuv.Y = AE_CLAMP((0.299 * r + 0.587 * g + 0.114 * b), 0, 1.0);
	yuv.U = AE_CLAMP((-0.168736 * r - 0.331264 * g + 0.500 * b) + 0.5, 0, 1.0);
	yuv.V = AE_CLAMP((0.500 * r - 0.418688 * g - 0.081312 * b) + 0.5, 0, 1.0);
	return yuv;
}

inline void YuvToRgb(YUV yuv, PF_FpLong* r, PF_FpLong* g, PF_FpLong* b)
{
	PF_FpLong u2, v2;
	u2 = yuv.U - 0.5;
	v2 = yuv.V - 0.5;
	*r = (PF_FpLong)AE_CLAMP(yuv.Y + 1.402 * v2, 0, 1);
	*g = (PF_FpLong)AE_CLAMP(yuv.Y - 0.344136 * u2 - 0.714136 * v2, 0, 1);
	*b = (PF_FpLong)AE_CLAMP(yuv.Y + 1.772 * u2, 0, 1);
}
#define HLSMAX (1.0)
#define UNDEFINED	(HLSMAX*2/3)
#define RGBMAX		PF_MAX_CHAN8 
#define RGBMAX16	PF_MAX_CHAN16 
#define RGBMAX32	(1.0)

inline HLS RGBtoHLS(PF_FpLong r, PF_FpLong g, PF_FpLong b)
{
	HLS hls;

	PF_FpLong cMax = MAX(MAX(r, g), b);
	PF_FpLong cMin = MIN(MIN(r, g), b);
	PF_FpLong C = cMax - cMin;

	hls.H = 0;
	hls.S = 0;
	hls.L = (cMax + cMin) / 2;

	if (C != 0) {
		if (hls.L <= 0.5) {
			hls.S = C / (2 * hls.L);
		}
		else {
			hls.S = C / (2 - 2 * hls.L);
		}

		if (r == cMax) {
			hls.H = (g - b) / C;
			if (hls.H < 0) {
				hls.H += 6;
			}
		}
		else if (g == cMax) {
			hls.H = (b - r) / C + 2;
		}
		else if (b == cMax) {
			hls.H = (r - g) / C + 4;
		}
		hls.H /= 6;
	}

	return hls;
}
//-------------------------------------------------------------------------------------------------
inline PF_FpLong hsl_value(PF_FpLong hh, PF_FpLong C, PF_FpLong m)
{
	PF_FpLong val;

	if (hh < 0) {
		hh += 6;
	}
	else if (hh >= 6) {
		hh -= 6;
	}

	if (hh < 1) {
		val = C * hh;
	}
	else if (hh < 3) {
		val = C;
	}
	else if (hh < 4) {
		val = C * (4 - hh);
	}
	else {
		val = 0;
	}
	val += m;

	return val;
}
//-------------------------------------------------------------------------------------------------
inline void HLStoRGB(HLS hls, PF_FpLong *R, PF_FpLong *G, PF_FpLong *B)
{
	//*R = 0, *G = 0, *B = 0;
	PF_FpLong C, m;

	if (hls.S == 0) {
		*R = *G = *B = hls.L;
	}
	else {
		if (hls.L <= 0.5) {
			C = 2 * hls.L * hls.S;
		}
		else {
			C = (2 - 2 * hls.L) * hls.S;
		}
		m = hls.L - C / 2;

		*R = hsl_value(6 * hls.H + 2, C, m);
		*G = hsl_value(6 * hls.H, C, m);
		*B = hsl_value(6 * hls.H - 2, C, m);
	}

}
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
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();
	/*
	if (xL == 0 && yL == 0) {
		char msg[256];
		sprintf_s(msg, "mode=%d, value=%f, hue_rot_ccw=%d, blend=%f\n",
			infoP->mode, infoP->value, infoP->hue_rot_ccw, infoP->blend);
		OutputDebugStringA(msg);
	}
	*/
	PF_FpLong r, g, b, r2, g2, b2;

	if (inP->alpha == 0) {
		// 完全に透明なピクセルは処理せずにそのままコピー
		*outP = {0,0,0,0};
		return PF_Err_NONE;
	}

	r2 = r = (PF_FpLong)inP->red / max_val;
	g2 = g = (PF_FpLong)inP->green / max_val;
	b2 = b = (PF_FpLong)inP->blue / max_val;
	PF_FpLong rv = 1 - infoP->value;
	if(infoP->mode ==1) {
		r2 = r * rv + (1.0f - r) * infoP->value;
		g2 = g * rv + (1.0f - g) * infoP->value;
		b2 = b * rv + (1.0f - b) * infoP->value;
	}else if (infoP->mode == 2) {
		YUV yuv = RgbToYuv(r, g, b);
		yuv.Y = yuv.Y*rv + (1.0f - yuv.Y) * infoP->value;
		YuvToRgb(yuv, &r2, &g2, &b2);
	}
	else if (infoP->mode == 3) {
		HLS hls = RGBtoHLS(r, g, b);
		PF_FpLong l = 0;
		l = hls.L * rv + (1.0f - hls.L) * infoP->value;
		HLStoRGB({ hls.H, l, hls.S }, &r2, &g2, &b2);
	}

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(r2);
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(g2);
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(b2);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(r2*max_val, 0, max_val));
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(g2 * max_val, 0, max_val));
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(b2 * max_val, 0, max_val));
	}

	outP->alpha = inP->alpha; // アルファはそのまま維持

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
	NF_AE *ae,
	ParamInfo *infoP
)
{
	PF_Err err = PF_Err_NONE;
		
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		err = ae->suitesP->IterateFloatSuite2()->iterate(
			ae->in_data,
			0,
			ae->output->height,
			ae->input,
			NULL,
			reinterpret_cast<refconType>(infoP),
			FilterImageFloat,
			ae->output);
		break;
	case PF_PixelFormat_ARGB64:
		err = ae->suitesP->Iterate16Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->height,
			ae->input,
			NULL,
			reinterpret_cast<refconType>(infoP),
			FilterImage16,
			ae->output);
		break;
	case PF_PixelFormat_ARGB32:
		err = ae->suitesP->Iterate8Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->height,
			ae->input,
			NULL,
			reinterpret_cast<refconType>(infoP),
			FilterImage8,
			ae->output);
		break;
	}
	return err;
}
