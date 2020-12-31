#include "FsBlend.h"


typedef struct OpacityInfo
{
	A_long opacity;
}OpacityInfo, *OpacityInfoP, **OpacityInfoH;



PF_Pixel16(*BlendPixel16_sub)(PF_Pixel16 under, PF_Pixel16 upper,A_long par);

// *********************************************************************************************************
/*
	通常合成

	S*(PF_MAX_CHAN16- A)/PF_MAX_CHAN16 + D*A/PF_MAX_CHAN16
	(S*(PF_MAX_CHAN16- A)+ D*A)/PF_MAX_CHAN16
	(S*PF_MAX_CHAN16- S*A+ D*A)/PF_MAX_CHAN16
	(S*PF_MAX_CHAN16 - (S-D)*A)/PF_MAX_CHAN16
	S -(S-D)*A/PF_MAX_CHAN16
*/
#define BLEND_NORMAL16(s,d,a) (RoundShort((A_long)s - ( (A_long)s - (A_long)d)*(A_long)a / PF_MAX_CHAN16))

/*
	スクリーン合成合成
	PF_MAX_CHAN16 - (PF_MAX_CHAN16 - A)*(PF_MAX_CHAN16 - b)/PF_MAX_CHAN16
	PF_MAX_CHAN16 - (PF_MAX_CHAN16*(PF_MAX_CHAN16 - b) - A*(PF_MAX_CHAN16 - b))/PF_MAX_CHAN16
	PF_MAX_CHAN16 - ((PF_MAX_CHAN16*PF_MAX_CHAN16 - PF_MAX_CHAN16*b) - (A*PF_MAX_CHAN16 - A*b))/PF_MAX_CHAN16
	PF_MAX_CHAN16 - (PF_MAX_CHAN16*PF_MAX_CHAN16 - PF_MAX_CHAN16*b - A*PF_MAX_CHAN16 + A*b)/PF_MAX_CHAN16
	PF_MAX_CHAN16 - (PF_MAX_CHAN16*PF_MAX_CHAN16/PF_MAX_CHAN16 - PF_MAX_CHAN16*b/PF_MAX_CHAN16 - A*PF_MAX_CHAN16/PF_MAX_CHAN16 + A*b/PF_MAX_CHAN16)
	PF_MAX_CHAN16 - (PF_MAX_CHAN16 - b - A + A*b/PF_MAX_CHAN16)
	PF_MAX_CHAN16 - PF_MAX_CHAN16 + b + A - A*b/PF_MAX_CHAN16

	b + A - A*b/PF_MAX_CHAN16

	PF_MAX_CHAN16 - 2*(PF_MAX_CHAN16 - A)*(PF_MAX_CHAN16 - B)/PF_MAX_CHAN16
	PF_MAX_CHAN16 - 2*(PF_MAX_CHAN16*(PF_MAX_CHAN16 - B) - A*(PF_MAX_CHAN16 - B))/PF_MAX_CHAN16
	PF_MAX_CHAN16 - 2*((PF_MAX_CHAN16*PF_MAX_CHAN16 - PF_MAX_CHAN16*B) - A*PF_MAX_CHAN16 + A*B)/PF_MAX_CHAN16
	PF_MAX_CHAN16 - 2*(PF_MAX_CHAN16*PF_MAX_CHAN16 - PF_MAX_CHAN16*B - A*PF_MAX_CHAN16 + A*B)/PF_MAX_CHAN16
	PF_MAX_CHAN16 - (2*PF_MAX_CHAN16*PF_MAX_CHAN16 - 2*PF_MAX_CHAN16*B - 2*A*PF_MAX_CHAN16 + A*B)/PF_MAX_CHAN16
	PF_MAX_CHAN16 - (2*PF_MAX_CHAN16 - 2*B - 2*A + A*B/PF_MAX_CHAN16)

	 2*(A+B) - A*B/PF_MAX_CHAN16 -PF_MAX_CHAN16

	*/
#define BLEND_SCREEN16(s,d) (RoundShort((A_long)s + (A_long)d - ((A_long)s * (A_long)d / PF_MAX_CHAN16)))
/*
	乗算合成
	A*B/PF_MAX_CHAN16
*/
#define BLEND_MULT16(s,d) (RoundShort((A_long)s * (A_long)d / PF_MAX_CHAN16))

//比較(明)
#define BLEND_LIGHTEN(s,d) ((s) > (d) ? (s) : (d))
//比較(暗)
#define BLEND_DARKEN(s,d) ((s) < (d) ? (s) : (d))

//加算
#define BLEND_ADD16(s,d) (RoundShort((A_long)s  + (A_long)d))
//減算
#define BLEND_SUB16(s,d) (RoundShort((A_long)s  - (A_long)d))

//overlay
/*
	PF_MAX_CHAN16 − 2*(PF_MAX_CHAN16 − d)*(PF_MAX_CHAN16 − s)/PF_MAX_CHAN16
	PF_MAX_CHAN16 − 2*((PF_MAX_CHAN16 − d)*PF_MAX_CHAN16 − (PF_MAX_CHAN16 − d)*s)/PF_MAX_CHAN16
	PF_MAX_CHAN16 − 2*((PF_MAX_CHAN16*PF_MAX_CHAN16 − d*PF_MAX_CHAN16) − (PF_MAX_CHAN16*s − d*s))/PF_MAX_CHAN16
	PF_MAX_CHAN16 − 2*(PF_MAX_CHAN16*PF_MAX_CHAN16 − d*PF_MAX_CHAN16 − PF_MAX_CHAN16*s + d*s)/PF_MAX_CHAN16

	PF_MAX_CHAN16 − (2*PF_MAX_CHAN16*PF_MAX_CHAN16 − 2*d*PF_MAX_CHAN16 − 2*PF_MAX_CHAN16*s + 2*d*s)/PF_MAX_CHAN16

	PF_MAX_CHAN16 + 2*PF_MAX_CHAN16 + 2*d + 2*s - 2*d*s/PF_MAX_CHAN16
	2*(d + s) - 2*d*s/PF_MAX_CHAN16 - PF_MAX_CHAN16

	1−2*(1−RGB1)*(1−RGB2)
	PF_MAX_CHAN16−2*(PF_MAX_CHAN16−s)*(PF_MAX_CHAN16−d)/PF_MAX_CHAN16
	PF_MAX_CHAN16−(2*PF_MAX_CHAN16*(PF_MAX_CHAN16−s) − 2*d*(PF_MAX_CHAN16−s))/PF_MAX_CHAN16
	PF_MAX_CHAN16−((2*PF_MAX_CHAN16*PF_MAX_CHAN16−2*PF_MAX_CHAN16*s) − (2*d*PF_MAX_CHAN16 − 2*d*s))/PF_MAX_CHAN16
	PF_MAX_CHAN16−(2*PF_MAX_CHAN16*PF_MAX_CHAN16−2*PF_MAX_CHAN16*s − 2*d*PF_MAX_CHAN16 + 2*d*s)/PF_MAX_CHAN16
	PF_MAX_CHAN16−(2*PF_MAX_CHAN16 − 2*s − 2*d + 2*d*s/PF_MAX_CHAN16)
	PF_MAX_CHAN16− 2*PF_MAX_CHAN16 + 2*s + 2*d - 2*d*s/PF_MAX_CHAN16
	 2*(s + d - d*s/PF_MAX_CHAN16) -PF_MAX_CHAN16

*/
#define BLEND_OVERRAY16(s,d) ((s) < (PF_HALF_CHAN16) ?\
 (RoundShort((A_long)s * (A_long)d *2 / PF_MAX_CHAN16))\
:(RoundShort( 2* ((A_long)s + (A_long)d - ((A_long)s * (A_long)d / PF_MAX_CHAN16 )) - PF_MAX_CHAN16)))
/*

*/

// ************************************************************************************
PF_Pixel16 BlendNormal16(PF_Pixel16 under, PF_Pixel16 upper, A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <=0)
	{
		r = upper;
	}
	else 
	{
		r.alpha = BLEND_SCREEN16(under.alpha, upper.alpha);
		r.red = BLEND_NORMAL16(under.red, upper.red, upper.alpha);
		r.green = BLEND_NORMAL16(under.green, upper.green, upper.alpha);
		r.blue = BLEND_NORMAL16(under.blue, upper.blue, upper.alpha);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha,par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);

	}
	return r;
}
// *********************************************************************************************************

PF_Pixel16 BlendScreen16(PF_Pixel16 under, PF_Pixel16 upper,A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		r.alpha = BLEND_SCREEN16(under.alpha, upper.alpha);
		r.red = BLEND_SCREEN16(under.red, upper.red);
		r.green = BLEND_SCREEN16(under.green, upper.green);
		r.blue = BLEND_SCREEN16(under.blue, upper.blue);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);

	}
	return r;

}
// *********************************************************************************************************

PF_Pixel16 BlendMult16(PF_Pixel16 under, PF_Pixel16 upper,A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		r.alpha = BLEND_SCREEN16(under.alpha, upper.alpha);
		r.red = BLEND_MULT16(under.red, upper.red);
		r.green = BLEND_MULT16(under.green, upper.green);
		r.blue = BLEND_MULT16(under.blue, upper.blue);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);

	}
	return r;
}
// *********************************************************************************************************
PF_Pixel16 BlendLighten16(PF_Pixel16 under, PF_Pixel16 upper, A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		r.alpha = BLEND_SCREEN16(under.alpha, upper.alpha);
		r.red = BLEND_LIGHTEN(under.red, upper.red);
		r.green = BLEND_LIGHTEN(under.green, upper.green);
		r.blue = BLEND_LIGHTEN(under.blue, upper.blue);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);

	}
	return r;
}
// *********************************************************************************************************
PF_Pixel16 BlendDarken16(PF_Pixel16 under, PF_Pixel16 upper,A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		r.alpha = BLEND_SCREEN16(under.alpha, upper.alpha);
		r.red = BLEND_DARKEN(under.red, upper.red);
		r.green = BLEND_DARKEN(under.green, upper.green);
		r.blue = BLEND_DARKEN(under.blue, upper.blue);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);
	}
	return r;
}
// *********************************************************************************************************
PF_Pixel16 BlendAdd16(PF_Pixel16 under, PF_Pixel16 upper, A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		r.alpha = BLEND_SCREEN16(under.alpha, upper.alpha);
		r.red = BLEND_ADD16(under.red, upper.red);
		r.green = BLEND_ADD16(under.green, upper.green);
		r.blue = BLEND_ADD16(under.blue, upper.blue);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);
	}
	return r;
}
// *********************************************************************************************************
PF_Pixel16 BlendOveray16(PF_Pixel16 under, PF_Pixel16 upper, A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		r.alpha = BLEND_MULT16(under.alpha, upper.alpha);
		r.red = BLEND_OVERRAY16(under.red, upper.red);
		r.green = BLEND_OVERRAY16(under.green, upper.green);
		r.blue = BLEND_OVERRAY16(under.blue, upper.blue);
	}
	if (par < PF_MAX_CHAN16)
	{
		r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
		r.red = BLEND_NORMAL16(under.red, r.red, par);
		r.green = BLEND_NORMAL16(under.green, r.green, par);
		r.blue = BLEND_NORMAL16(under.blue, r.blue, par);
	}
	return r;
}
// *********************************************************************************************************
PF_Pixel16 BlendLighterColor16(PF_Pixel16 under, PF_Pixel16 upper, A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		A_long underM = (A_long)under.red + (A_long)under.green + (A_long)under.blue;
		A_long upperM = (A_long)upper.red + (A_long)upper.green + (A_long)upper.blue;

		if (underM > upperM)
		{
			r = under;
		}
		else {
			r = upper;
			if (par < PF_MAX_CHAN16)
			{
				r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
				r.red = BLEND_NORMAL16(under.red, r.red, par);
				r.green = BLEND_NORMAL16(under.green, r.green, par);
				r.blue = BLEND_NORMAL16(under.blue, r.blue, par);
			}
		}
	}
	return r;
}
// *********************************************************************************************************
PF_Pixel16 BlendDarkerColor16(PF_Pixel16 under, PF_Pixel16 upper, A_long par)
{
	PF_Pixel16 r = { 0,0,0,0 };
	if ((par <= 0) || (upper.alpha <= 0))
	{
		return under;
	}

	if (under.alpha <= 0)
	{
		r = upper;
	}
	else
	{
		A_long underM = (A_long)under.red + (A_long)under.green + (A_long)under.blue;
		A_long upperM = (A_long)upper.red + (A_long)upper.green + (A_long)upper.blue;

		if (underM < upperM)
		{
			r = under;
		}
		else {
			r = upper;
			if (par < PF_MAX_CHAN16)
			{
				r.alpha = BLEND_NORMAL16(under.alpha, r.alpha, par);
				r.red = BLEND_NORMAL16(under.red, r.red, par);
				r.green = BLEND_NORMAL16(under.green, r.green, par);
				r.blue = BLEND_NORMAL16(under.blue, r.blue, par);
			}
		}
	}
	return r;
}
// *********************************************************************************************************
PF_Err Blend16To08Sub(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel* outP)
{
	PF_Err		err = PF_Err_NONE;
	OpacityInfo *	infoP = reinterpret_cast<OpacityInfo*>(refcon);

	PF_Pixel16 outP2 = CONV8TO16(*outP);
	outP2 = BlendPixel16_sub(outP2, *inP,infoP->opacity);
	*outP = CONV16TO8(outP2);
	return err;
}
// *********************************************************************************************************
PF_Err Blend16To16Sub(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel16* outP)
{
	PF_Err		err = PF_Err_NONE;
	OpacityInfo *	infoP = reinterpret_cast<OpacityInfo*>(refcon);
	*outP = BlendPixel16_sub(*outP, *inP, infoP->opacity);
	return err;
}
// *********************************************************************************************************
PF_Err Blend16To32Sub(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_PixelFloat* outP)
{
	PF_Err		err = PF_Err_NONE;
	OpacityInfo *	infoP = reinterpret_cast<OpacityInfo*>(refcon);
	PF_Pixel16 outP2 = CONV32TO16(*outP);
	outP2 = BlendPixel16_sub(outP2, *inP, infoP->opacity);
	*outP = CONV16TO32(outP2);
	return err;
}
// *********************************************************************************************************
PF_Err FuncSelector(A_long mode)
{
	PF_Err		err = PF_Err_NONE;
	switch (mode)
	{
	case BLEND_MODE::Screen:
		BlendPixel16_sub = BlendScreen16;
		break;
	case BLEND_MODE::Multiply:
		BlendPixel16_sub = BlendMult16;
		break;
	case BLEND_MODE::Lighten:
		BlendPixel16_sub = BlendLighten16;
		break;
	case BLEND_MODE::Darken:
		BlendPixel16_sub = BlendDarken16;
		break;
	case BLEND_MODE::Add:
		BlendPixel16_sub = BlendAdd16;
		break;
	case BLEND_MODE::Overlay:
		BlendPixel16_sub = BlendOveray16;
		break;
	case BLEND_MODE::DarkerColor:
		BlendPixel16_sub = BlendDarkerColor16;
		break;
	case BLEND_MODE::LighterColor:
		BlendPixel16_sub = BlendLighterColor16;
		break;
	case BLEND_MODE::Normal:
	default:
		BlendPixel16_sub = BlendNormal16;
		break;
	}
	return err;
}
// *********************************************************************************************************
PF_Err Blend16To08(PF_EffectWorldPtr src, PF_EffectWorldPtr dst, A_long blendMode,PF_FpLong par)
{
	PF_Err		err = PF_Err_NONE;
	FuncSelector(blendMode);
	OpacityInfo oi;
	oi.opacity = (A_long)(PF_MAX_CHAN16 * par + 0.5);
	ERR(iterateWorld16to08(&oi, src, dst, Blend16To08Sub));
	return err;
}
// *********************************************************************************************************
PF_Err Blend16To16(PF_EffectWorldPtr src, PF_EffectWorldPtr dst, A_long blendMode, PF_FpLong par)
{
	PF_Err		err = PF_Err_NONE;

	FuncSelector(blendMode);
	OpacityInfo oi;
	oi.opacity = (A_long)(PF_MAX_CHAN16 * par + 0.5);
	ERR(iterateWorld16to16(&oi, src, dst, Blend16To16Sub));
	return err;
}
// *********************************************************************************************************
PF_Err Blend16To32(PF_EffectWorldPtr src, PF_EffectWorldPtr dst, A_long blendMode, PF_FpLong par)
{
	PF_Err		err = PF_Err_NONE;

	FuncSelector(blendMode);
	OpacityInfo oi;
	oi.opacity = (A_long)(PF_MAX_CHAN16 * par + 0.5);
	ERR(iterateWorld16to32(&oi, src, dst, Blend16To32Sub));
	return err;
}
// *********************************************************************************************************
