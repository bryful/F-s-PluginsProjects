#include "NF_LogicGray.h"
#include <math.h>

// 内部データ構造
typedef struct {
	PF_FpLong	H;
	PF_FpLong	L;
	PF_FpLong	S;
	PF_FpLong	A;
}HLSA;

typedef struct {
	PF_FpLong	L;
	PF_FpLong	A;
	PF_FpLong	B;
	PF_FpShort	alpha;
}LABA;

// 内部関数定義
static HLSA RGBtoHLS(PF_PixelFloat c)
{
	HLSA hls;
	hls.A = c.alpha;

	PF_FpLong R = c.red;
	PF_FpLong G = c.green;
	PF_FpLong B = c.blue;

	PF_FpLong cMax = MAX(MAX(R, G), B);
	PF_FpLong cMin = MIN(MIN(R, G), B);
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

		if (R == cMax) {
			hls.H = (G - B) / C;
			if (hls.H < 0) {
				hls.H += 6;
			}
		}
		else if (G == cMax) {
			hls.H = (B - R) / C + 2;
		}
		else if (B == cMax) {
			hls.H = (R - G) / C + 4;
		}
		hls.H /= 6;
	}

	return hls;
}

static LABA RgbToLab(PF_PixelFloat rgb)
{
	double sr = rgb.red;
	if (sr < 0.04045)
		sr /= 12.92;
	else
		sr = pow((sr + 0.055) / 1.055, 2.4);

	double sg = rgb.green;
	if (sg < 0.04045)
		sg /= 12.92;
	else
		sg = pow((sg + 0.055) / 1.055, 2.4);

	double sb = rgb.blue;
	if (sb < 0.04045)
		sb /= 12.92;
	else
		sb = pow((sb + 0.055) / 1.055, 2.4);

	// XYZに変換
	double x = 0.412453 * sr + 0.357580 * sg + 0.180423 * sb;
	double y = 0.212671 * sr + 0.715160 * sg + 0.072169 * sb;
	double z = 0.019334 * sr + 0.119193 * sg + 0.950227 * sb;

	x /= 0.950456;
	z /= 1.088754;

	// Labに変換
	LABA lab;
	double fy;
	if (y > 0.008856) {
		fy = pow(y, 1. / 3);
		lab.L = 1.16 * fy;
	}
	else {
		fy = 7.787 * y + 16. / 116;
		lab.L = 9.033 * y;
	}
	lab.L -= 0.16;
	double fx = x > 0.008856 ? pow(x, 1. / 3) : 7.787 * x + 16. / 116;
	double fz = z > 0.008856 ? pow(z, 1. / 3) : 7.787 * z + 16. / 116;

	lab.A = 5 * (fx - fy);
	lab.B = 2 * (fy - fz);
	lab.alpha = rgb.alpha;

	return lab;
}

//**********************************************************************************************
// テンプレート化されたGMIX関数
template<typename PixelType, typename ChanType, ChanType MaxChan>
inline PixelType GMIX(PixelType* s, auto v, PF_FpShort p)
{
	PixelType r;
	if constexpr (std::is_same_v<ChanType, PF_FpShort>) {
		if (v < 0) v = 0;
		else if (v > 1.0) v = 1.0;
	} else {
		if (v < 0) v = 0;
		else if (v > MaxChan) v = MaxChan;
	}
	
	if (p <= 0) {
		r.alpha = s->alpha;
		r.red = r.green = r.blue = (ChanType)v;
	}
	else {
		PF_FpShort d0 = p;
		PF_FpShort d1 = 1 - p;
		r.alpha = s->alpha;
		
		if constexpr (std::is_same_v<ChanType, A_u_char>) {
			r.red = RoundByteFpLong((PF_FpShort)s->red * d0 + (PF_FpShort)v * d1);
			r.green = RoundByteFpLong((PF_FpShort)s->green * d0 + (PF_FpShort)v * d1);
			r.blue = RoundByteFpLong((PF_FpShort)s->blue * d0 + (PF_FpShort)v * d1);
		}
		else if constexpr (std::is_same_v<ChanType, A_u_short>) {
			r.red = RoundShortFpLong((PF_FpShort)s->red * d0 + (PF_FpShort)v * d1);
			r.green = RoundShortFpLong((PF_FpShort)s->green * d0 + (PF_FpShort)v * d1);
			r.blue = RoundShortFpLong((PF_FpShort)s->blue * d0 + (PF_FpShort)v * d1);
		}
		else {
			r.red = RoundFpShortDouble((PF_FpShort)s->red * d0 + (PF_FpShort)v * d1);
			r.green = RoundFpShortDouble((PF_FpShort)s->green * d0 + (PF_FpShort)v * d1);
			r.blue = RoundFpShortDouble((PF_FpShort)s->blue * d0 + (PF_FpShort)v * d1);
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------
// YUVモード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_yuv(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = (0.29891 * (PF_FpLong)inP->red) + (0.58661 * (PF_FpLong)inP->green) + (0.11448 * (PF_FpLong)inP->blue);
		if constexpr (!std::is_same_v<ChanType, PF_FpShort>) {
			v = (decltype(v))v;
		}
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// HLSモード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_hls(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = (MAX(MAX(inP->red, inP->green), inP->blue) + MIN(MIN(inP->red, inP->green), inP->blue));
		if constexpr (std::is_same_v<ChanType, A_u_char>) {
			v = v >> 1;
		} else if constexpr (std::is_same_v<ChanType, A_u_short>) {
			v = v >> 1;
		} else {
			v = v / 2;
		}
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// RGB平均モード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_rgb(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = (inP->red + inP->green + inP->blue) / 3;
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// Rチャンネルモード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_r(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = inP->red;
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// Gチャンネルモード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_g(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = inP->green;
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// Bチャンネルモード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_b(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = inP->blue;
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// RGB最大値モード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_rgbmax(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		auto v = MAX(MAX(inP->red, inP->green), inP->blue);
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
// LABモード
template<typename PixelType, typename ChanType, ChanType MaxChan>
static PF_Err ToGrayFunc_lab(
	refconType refcon,
	A_long xL,
	A_long yL,
	PixelType* inP,
	PixelType* outP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpShort* org_blend = reinterpret_cast<PF_FpShort*>(refcon);
	if (org_blend) {
		PF_PixelFloat c;
		if constexpr (std::is_same_v<PixelType, PF_Pixel8>) {
			c = NF_Pixel8To32(*inP);
		} else if constexpr (std::is_same_v<PixelType, PF_Pixel16>) {
			c = NF_Pixel16To32(*inP);
		} else {
			c = *inP;
		}
		LABA lab = RgbToLab(c);
		
		auto v = lab.L;
		if constexpr (std::is_same_v<ChanType, A_u_char>) {
			v = v * PF_MAX_CHAN8;
		} else if constexpr (std::is_same_v<ChanType, A_u_short>) {
			v = v * PF_MAX_CHAN16;
		}
		
		*outP = GMIX<PixelType, ChanType, MaxChan>(inP, v, *org_blend);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
/**
 * LogicalGrayMain - グレースケール変換のメイン処理関数
 */
PF_Err LogicalGrayMain(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long gray_mode,
	PF_FpShort org_blend)
{
	PF_Err err = PF_Err_NONE;

	if (org_blend >= 1.0) {
		return PF_Err_NONE;
	}

	try {
		switch (pixelFormat)
		{
		case PF_PixelFormat_ARGB128:
			switch (gray_mode) {
			case 1: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_yuv<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 2: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_hls<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 3: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_rgb<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 4: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_r<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 5: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_g<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 6: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_b<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 7: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_rgbmax<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			case 8: ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_lab<PF_PixelFloat, PF_FpShort, (PF_FpShort)1.0>, worldP)); break;
			}
			break;
		case PF_PixelFormat_ARGB64:
			switch (gray_mode) {
			case 1: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_yuv<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 2: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_hls<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 3: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_rgb<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 4: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_r<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 5: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_g<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 6: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_b<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 7: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_rgbmax<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			case 8: ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_lab<PF_Pixel16, A_u_short, PF_MAX_CHAN16>, worldP)); break;
			}
			break;
		case PF_PixelFormat_ARGB32:
			switch (gray_mode) {
			case 1: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_yuv<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 2: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_hls<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 3: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_rgb<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 4: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_r<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 5: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_g<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 6: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_b<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 7: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_rgbmax<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			case 8: ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, worldP->height, worldP, nullptr, (refconType)&org_blend, ToGrayFunc_lab<PF_Pixel8, A_u_char, PF_MAX_CHAN8>, worldP)); break;
			}
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}

	return err;
}
