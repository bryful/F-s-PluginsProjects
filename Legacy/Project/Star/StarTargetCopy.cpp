//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "Star.h"


//-----------------------------------------------------------------------------------
static PF_Err 
TargetCopy8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	A_u_char mp = MAX(inP->red,MAX(inP->green,inP->blue));

	outP->blue = outP->green = outP->red =
	outP->alpha = RoundByteLong( mp * inP->alpha / PF_MAX_CHAN8);
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
TargetCopy16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	A_u_short mp = MAX(inP->red,MAX(inP->green,inP->blue));

	outP->blue = outP->green = outP->red =
	outP->alpha = RoundShort( mp * inP->alpha / PF_MAX_CHAN16);
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
TargetCopy32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	PF_FpShort mp = MAX(inP->red,MAX(inP->green,inP->blue));

	outP->blue = outP->green = outP->red =
	outP->alpha = RoundFpShortDouble( mp * inP->alpha);
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err TargetCopy (CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32(NULL,TargetCopy32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16(NULL,TargetCopy16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8(NULL,TargetCopy8));
		break;
	}
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
ScreenCopy8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	PF_Pixel r;
	r.red	= RoundByteLong( inP->red * inP->alpha /PF_MAX_CHAN8);
	r.green	= RoundByteLong( inP->green * inP->alpha /PF_MAX_CHAN8);
	r.blue	= RoundByteLong( inP->blue * inP->alpha /PF_MAX_CHAN8);

	r.alpha = outP->alpha;
	*outP = r;

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
ScreenCopy16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	PF_Pixel16 r;
	r.red	= RoundShort( inP->red		* inP->alpha /PF_MAX_CHAN16);
	r.green	= RoundShort( inP->green	* inP->alpha /PF_MAX_CHAN16);
	r.blue	= RoundShort( inP->blue		* inP->alpha /PF_MAX_CHAN16);

	r.alpha = outP->alpha;
	*outP = r;

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
ScreenCopy32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	PF_PixelFloat r;
	r.red	= RoundFpShortDouble( inP->red * inP->alpha);
	r.green	= RoundFpShortDouble( inP->green * inP->alpha);
	r.blue	= RoundFpShortDouble( inP->blue * inP->alpha);

	r.alpha = outP->alpha;
	*outP = r;

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err ScreenCopy (CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32(NULL,ScreenCopy32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16(NULL,ScreenCopy16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8(NULL,ScreenCopy8));
		break;
	}
	return err;
}
typedef struct
{
	PF_Pixel		c8;
	PF_Pixel16		c16;
	PF_PixelFloat	c32;
}ColorFillInfo;
#define FillColor_TARGET red
//-----------------------------------------------------------------------------------
static PF_Err 
FillColor8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ColorFillInfo *	infoP		= reinterpret_cast<ColorFillInfo*>(refcon);
	
	PF_Pixel r;
	PF_Pixel p = infoP->c8;
	r.red	= RoundByteLong( p.red		* outP->FillColor_TARGET /PF_MAX_CHAN8);
	r.green	= RoundByteLong( p.green	* outP->FillColor_TARGET /PF_MAX_CHAN8);
	r.blue	= RoundByteLong( p.blue		* outP->FillColor_TARGET /PF_MAX_CHAN8);

	r.alpha = outP->alpha;
	*outP = r;

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FillColor16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ColorFillInfo *	infoP		= reinterpret_cast<ColorFillInfo*>(refcon);
	
	PF_Pixel16 r;
	PF_Pixel16 p = infoP->c16;
	r.red	= RoundShort( p.red		* outP->FillColor_TARGET /PF_MAX_CHAN16);
	r.green	= RoundShort( p.green	* outP->FillColor_TARGET /PF_MAX_CHAN16);
	r.blue	= RoundShort( p.blue	* outP->FillColor_TARGET /PF_MAX_CHAN16);

	r.alpha = outP->alpha;
	*outP = r;

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FillColor32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ColorFillInfo *	infoP		= reinterpret_cast<ColorFillInfo*>(refcon);
	
	PF_PixelFloat r;
	PF_PixelFloat p = infoP->c32;
	r.red	= RoundFpShortDouble( p.red		* outP->FillColor_TARGET);
	r.green	= RoundFpShortDouble( p.green	* outP->FillColor_TARGET);
	r.blue	= RoundFpShortDouble( p.blue	* outP->FillColor_TARGET);

	r.alpha = outP->alpha;
	*outP = r;

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FillColorCopy (CFsAE *ae,PF_Pixel c)
{
	PF_Err	err = PF_Err_NONE;

	ColorFillInfo info;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		info.c32 = CONV8TO32(c);
		ERR(ae->iterate32((refconType)&info,FillColor32));
		break;
	case PF_PixelFormat_ARGB64:
		info.c16 = CONV8TO16(c);
		ERR(ae->iterate16((refconType)&info,FillColor16));
		break;
	case PF_PixelFormat_ARGB32:
		info.c8 = c;
		ERR(ae->iterate8((refconType)&info,FillColor8));
		break;
	}
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
AlphaMake8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	outP->alpha = MAX(outP->blue,MAX(outP->green,outP->red));
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
AlphaMake16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	outP->alpha = MAX(outP->blue,MAX(outP->green,outP->red));
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
AlphaMake32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	outP->alpha = MAX(outP->blue,MAX(outP->green,outP->red));
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err AlphaMake (CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32(NULL,AlphaMake32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16(NULL,AlphaMake16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8(NULL,AlphaMake8));
		break;
	}
	return err;
}

