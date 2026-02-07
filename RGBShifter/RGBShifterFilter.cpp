#include "RGBShifter.h"

typedef struct {
	PF_FpLong	Y;
	PF_FpLong	U;
	PF_FpLong	V;
}YUV;

// *******************************************************************************
inline YUV RgbToYuv(PF_FpLong r, PF_FpLong g, PF_FpLong b)
{
	YUV yuv;
	yuv.Y = AE_CLAMP((0.299 * r + 0.587 * g + 0.114 * b),0,1.0);
	yuv.U = AE_CLAMP((-0.168736 * r - 0.331264 * g + 0.500 * b) + 0.5,0,1.0);
	yuv.V = AE_CLAMP((0.500 * r - 0.418688 * g - 0.081312 * b) + 0.5,0,1.0);
	return yuv;
}

inline void YuvToRgb(YUV yuv,PF_FpLong *r, PF_FpLong* g, PF_FpLong* b )
{
	PF_FpLong u2, v2;
	u2 = yuv.U - 0.5;
	v2 = yuv.V - 0.5;
	*r = (PF_FpLong)AE_CLAMP(yuv.Y + 1.402 * v2,0,1);
	*g = (PF_FpLong)AE_CLAMP(yuv.Y - 0.344136 * u2 - 0.714136 * v2, 0, 1);
	*b = (PF_FpLong)AE_CLAMP(yuv.Y + 1.772 * u2, 0, 1);
}

// 共通処理をテンプレート化
template<typename PixelType>
static PF_Err RGBShiftCommon(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PixelType* inP,
	PixelType* outP,
	PF_FpLong	maxChan)
{
	PF_Err err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	
	if (inP->alpha == 0) {
		*outP = {0,0,0,0};
		return err;
	}

	PF_FpLong r, g, b, r2, g2, b2;
	YUV yuv, yuv2;
	
	r2 = r = (PF_FpLong)inP->red / maxChan;
	g2 = g = (PF_FpLong)inP->green / maxChan;
	b2 = b = (PF_FpLong)inP->blue / maxChan;

	if (infoP->mode == 1) { // RGBシフト
		if (infoP->r_shift > 0.0f) r2 = r + (1.0f - r) * infoP->r_shift;
		else if (infoP->r_shift < 0.0f) r2 = r + r * infoP->r_shift;
		
		if (infoP->g_shift > 0.0f) g2 = g + (1.0f - g) * infoP->g_shift;
		else if (infoP->g_shift < 0.0f) g2 = g + g * infoP->g_shift;
		
		if (infoP->b_shift > 0.0f) b2 = b + (1.0f - b) * infoP->b_shift;
		else if (infoP->b_shift < 0.0f) b2 = b + b * infoP->b_shift;
	}
	else if (infoP->mode == 2) { // YUVシフト
		yuv = RgbToYuv(r, g, b);
		yuv2 = yuv;
		
		if (infoP->y_shift > 0.0f) yuv2.Y = yuv.Y + (1.0f - yuv.Y) * infoP->y_shift;
		else if (infoP->y_shift < 0.0f) yuv2.Y = yuv.Y + yuv.Y * infoP->y_shift;
		
		if (infoP->u_shift > 0.0f) yuv2.U = yuv.U + (1.0f - yuv.U) * infoP->u_shift;
		else if (infoP->u_shift < 0.0f) yuv2.U = yuv.U + yuv.U * infoP->u_shift;
		
		if (infoP->v_shift > 0.0f) yuv2.V = yuv.V + (1.0f - yuv.V) * infoP->v_shift;
		else if (infoP->v_shift < 0.0f) yuv2.V = yuv.V + yuv.V * infoP->v_shift;
		
		YuvToRgb(yuv2, &r2, &g2, &b2);
	}
	else if (infoP->mode == 3) { // Target RGB
		yuv = RgbToYuv(r, g, b);
		yuv2.Y = yuv.Y;
		yuv2.U = infoP->target_u;
		yuv2.V = infoP->target_v;
		YuvToRgb(yuv2, &r2, &g2, &b2);
	}

	// 型に応じた出力変換
	if constexpr (std::is_same<PixelType, PF_Pixel8>::value) {
		outP->red = (A_u_char)(AE_CLAMP(r2,0,1.0) * PF_MAX_CHAN8 + 0.5);
		outP->green = (A_u_char)(AE_CLAMP(g2,0,1.0) * PF_MAX_CHAN8 + 0.5);
		outP->blue = (A_u_char)(AE_CLAMP(b2,0,1.0) * PF_MAX_CHAN8 + 0.5);
	}
	else if constexpr (std::is_same<PixelType, PF_Pixel16>::value) {
		outP->red = NF_CONVERT32TO16(AE_CLAMP(r2,0,1.0));
		outP->green = NF_CONVERT32TO16(AE_CLAMP(g2,0,1.0));
		outP->blue = NF_CONVERT32TO16(AE_CLAMP(b2,0,1.0));
	}
	else { // PF_PixelFloat
		outP->red = (PF_FpShort)AE_CLAMP(r2,0,1.0);
		outP->green = (PF_FpShort)AE_CLAMP(g2,0,1.0);
		outP->blue = (PF_FpShort)AE_CLAMP(b2,0,1.0);
	}
	
	outP->alpha = inP->alpha;
	return err;
}

//-----------------------------------------------------------------------------------
static PF_Err RGBShift8(refconType refcon, A_long xL, A_long yL, PF_Pixel8* inP, PF_Pixel8* outP)
{
	return RGBShiftCommon(refcon, xL, yL, inP, outP, (PF_FpLong)PF_MAX_CHAN8);
}

static PF_Err RGBShift16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return RGBShiftCommon(refcon, xL, yL, inP, outP, (PF_FpLong)PF_MAX_CHAN16);
}

static PF_Err RGBShift32(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return RGBShiftCommon(refcon, xL, yL, inP, outP, 1.0);
}

// *******************************************************************************
PF_Err RGBShiftFX(NF_AE* ae, ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	if (infoP->mode == 1 && infoP->r_shift == 0.0f && infoP->g_shift == 0.0f && infoP->b_shift == 0.0f) {
		return err;
	}
	else if (infoP->mode == 2 && infoP->y_shift == 0.0f && infoP->u_shift == 0.0f && infoP->v_shift == 0.0f) {
		return err;
	}
	
	PF_FpLong rTarget, gTarget, bTarget;
	rTarget = (PF_FpLong)infoP->target_color.red / PF_MAX_CHAN8;
	gTarget = (PF_FpLong)infoP->target_color.green / PF_MAX_CHAN8;
	bTarget = (PF_FpLong)infoP->target_color.blue / PF_MAX_CHAN8;
	YUV yuvTarget = RgbToYuv(rTarget, gTarget, bTarget);
	infoP->target_u = yuvTarget.U;
	infoP->target_v = yuvTarget.V;

	switch (ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		err = ae->iterate32(ae->input, reinterpret_cast<refconType>(infoP), RGBShift32, ae->output);
		break;
	case PF_PixelFormat_ARGB64:
		err = ae->iterate16(ae->input, reinterpret_cast<refconType>(infoP), RGBShift16, ae->output);
		break;
	case PF_PixelFormat_ARGB32:
		err = ae->iterate8(ae->input, reinterpret_cast<refconType>(infoP), RGBShift8, ae->output);
		break;
	}

	return err;
}
