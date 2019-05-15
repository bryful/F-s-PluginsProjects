#include "TouchDraw.h"

typedef struct {
	PF_Pixel	color;
	PF_Boolean	org;
}copyAlpha8Param;
typedef struct {
	PF_Pixel16 color;
	PF_Boolean	org;
}copyAlpha16Param;
typedef struct {
	PF_PixelFloat color;
	PF_Boolean	org;
}copyAlpha32Param;


#define	TARGET_CHANNEL	green
//-------------------------------------------------------------------------------------------------
static PF_Err 
copyAlphaSub8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	copyAlpha8Param *	infoP		= reinterpret_cast<copyAlpha8Param*>(refcon);

	PF_Pixel p;
	p = infoP->color;
	p.alpha = outP->TARGET_CHANNEL;
	if ( infoP->org){
		*outP = PixelBlend8(*inP,p);
	}else{
		*outP = p;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
copyAlphaSub16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	copyAlpha16Param *	infoP		= reinterpret_cast<copyAlpha16Param*>(refcon);

	PF_Pixel16 p;
	p = infoP->color;
	p.alpha = outP->TARGET_CHANNEL;
	if ( infoP->org){
		*outP = PixelBlend16(*inP,p);
	}else{
		*outP = p;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
copyAlphaSub32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	copyAlpha32Param *	infoP		= reinterpret_cast<copyAlpha32Param*>(refcon);

	PF_PixelFloat p;
	p = infoP->color;
	p.alpha = outP->TARGET_CHANNEL;
	if ( infoP->org){
		*outP = PixelBlend32(*inP,p);
	}else{
		*outP = p;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err copyAlpha(CFsAE *ae , PF_Pixel color, PF_Boolean org)
{
	PF_Err	err = PF_Err_NONE;
	
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		copyAlpha32Param cp32;
		cp32.color	= CONV8TO32(color);
		cp32.org	= org;
		ERR(ae->iterate32((refconType)&cp32,copyAlphaSub32));
		break;
	case PF_PixelFormat_ARGB64:
		copyAlpha16Param cp16;
		cp16.color	= CONV8TO16(color);
		cp16.org	= org;
		ERR(ae->iterate16((refconType)&cp16,copyAlphaSub16));
		break;
	case PF_PixelFormat_ARGB32:
		copyAlpha8Param cp8;
		cp8.color	= color;
		cp8.org		= org;
		ERR(ae->iterate8((refconType)&cp8,copyAlphaSub8));
		break;
	}
	return err;
}
