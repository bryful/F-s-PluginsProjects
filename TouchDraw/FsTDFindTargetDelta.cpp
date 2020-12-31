#include "TouchDraw.h"


typedef struct {
	A_long		widthTrue;
	A_long		width;
	A_long		height;
	A_long		lv;
	PF_Boolean	isAlpha;
} DeltaSubParam;

//#define TARGET_DELTA	green
#define TARGET_DELTA	blue
//-------------------------------------------------------------------------------------------------
inline A_long SubtrucPixelAlpha8(PF_Pixel s, PF_Pixel d)
{
	return (A_long)F_ABS(s.alpha - d.alpha);
}
//-------------------------------------------------------------------------------------------------
inline A_long SubtrucPixelAlpha16(PF_Pixel16 s, PF_Pixel16 d)
{
	return (A_long)FS_CONVERT16TO8(F_ABS(s.alpha - d.alpha));
}
//-------------------------------------------------------------------------------------------------
inline A_long SubtrucPixelAlpha32(PF_PixelFloat s, PF_PixelFloat d)
{
	return FS_CONVERT32TO8(PF_MAX_CHAN8 * F_ABS(s.alpha - d.alpha));
}
//-------------------------------------------------------------------------------------------------
inline A_long SubtrucPixel8(PF_Pixel s, PF_Pixel d)
{
	A_long v =0;
	/*
	v += F_ABS(s.red  *s.alpha/PF_MAX_CHAN8 - d.red  *d.alpha/PF_MAX_CHAN8);
	v += F_ABS(s.green*s.alpha/PF_MAX_CHAN8 - d.green*d.alpha/PF_MAX_CHAN8);
	v += F_ABS(s.blue *s.alpha/PF_MAX_CHAN8 - d.blue *d.alpha/PF_MAX_CHAN8);
	*/
	v += F_ABS(s.red  *s.alpha - d.red  *d.alpha)/PF_MAX_CHAN8;
	v += F_ABS(s.green*s.alpha - d.green*d.alpha)/PF_MAX_CHAN8;
	v += F_ABS(s.blue *s.alpha - d.blue *d.alpha)/PF_MAX_CHAN8;

	v = v/3;
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	return (A_long)v;
}
//-------------------------------------------------------------------------------------------------
inline A_long SubtrucPixel16(PF_Pixel16 s, PF_Pixel16 d)
{
	A_long v =0;
	v += F_ABS(s.red*s.alpha/PF_MAX_CHAN16 - d.red*d.alpha/PF_MAX_CHAN16);
	v += F_ABS(s.green*s.alpha/PF_MAX_CHAN16 - d.green*d.alpha/PF_MAX_CHAN16);
	v += F_ABS(s.blue*s.alpha/PF_MAX_CHAN16 - d.blue*d.alpha/PF_MAX_CHAN16);

	v = FS_CONVERT16TO8(v/3);
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	return (A_long)v;
}
//-------------------------------------------------------------------------------------------------
inline A_long SubtrucPixel32(PF_PixelFloat s, PF_PixelFloat d)
{
	PF_FpLong v =0;
	PF_FpLong a =0;

	a = F_ABS(s.red*s.alpha - d.red*d.alpha);
	if ( a>1) a = 1;
	v +=a;

	a = F_ABS(s.green*s.alpha - d.green*d.alpha);
	if ( a>1) a = 1;
	v +=a;

	a = F_ABS(s.blue*s.alpha - d.blue*d.alpha);
	if ( a>1) a = 1;
	v +=a;
	v = FS_CONVERT32TO8(v/3);
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	return (A_long)v;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static PF_Err 
DeltaSub8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	DeltaSubParam *	infoP		= reinterpret_cast<DeltaSubParam*>(refcon);

	A_long v,mv,mx;
	v =0;
	mv =0;
	mx = PF_MAX_CHAN8;//A_u_char で識別
	
	if (mv<mx) if (xL<=infoP->width) {
		if (infoP->isAlpha==TRUE){
			v = SubtrucPixelAlpha8(*inP, inP[1]);
		}else{
			v = SubtrucPixel8(*inP, inP[1]);
		}
		if (mv<v) mv=v; 
	}
	
	
	if (mv<mx) if (yL<=infoP->height) {
		PF_Pixel *g = inP;
		g += infoP->widthTrue;
		if (infoP->isAlpha==TRUE){
			v = SubtrucPixelAlpha8(*inP, *g);
		}else{
			v = SubtrucPixel8(*inP, *g);
		}
		if (mv<v) mv=v; 
	}
	
	//outP->blue = outP->green = outP->red = outP->alpha = 0;
	if ( mv>infoP->lv){
		outP->TARGET_DELTA = PF_MAX_CHAN8;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
DeltaSub16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	DeltaSubParam *	infoP		= reinterpret_cast<DeltaSubParam*>(refcon);

	A_long v,mv,mx;
	v =0;
	mv =0;
	mx = PF_MAX_CHAN8;//A_u_char で識別
	
	if (mv<mx) if (xL<=infoP->width) {
		if (infoP->isAlpha==TRUE){
			v = SubtrucPixelAlpha16(*inP, inP[1]);
		}else{
			v = SubtrucPixel16(*inP, inP[1]);
		}
		if (mv<v) mv=v; 
	}
	
	if (mv<mx) if (yL<=infoP->height) {
		if (infoP->isAlpha==TRUE){
			v = SubtrucPixelAlpha16(*inP, inP[infoP->widthTrue]);
		}else{
			v = SubtrucPixel16(*inP, inP[infoP->widthTrue]);
		}
		if (mv<v) mv=v; 
	}
	if ( mv>infoP->lv){
		outP->TARGET_DELTA = PF_MAX_CHAN16;
	}else{
		outP->TARGET_DELTA = 0;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
DeltaSub32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	DeltaSubParam *	infoP		= reinterpret_cast<DeltaSubParam*>(refcon);

	A_long v,mv,mx;
	v =0;
	mv =0;
	mx = PF_MAX_CHAN8;//A_u_char で識別
	if (mv<mx) if (xL<=infoP->width) {
		if (infoP->isAlpha==TRUE){
			v = SubtrucPixelAlpha32(*inP, inP[1]);
		}else{
			v = SubtrucPixel32(*inP, inP[1]);
		}
		if (mv<v) mv=v; 
	}
	if (mv<mx) if (yL<=infoP->height) {
		if (infoP->isAlpha==TRUE){
			v = SubtrucPixelAlpha32(*inP, inP[infoP->widthTrue]);
		}else{
			v = SubtrucPixel32(*inP, inP[infoP->widthTrue]);
		}
		if (mv<v) mv=v; 
	}
	if ( mv>infoP->lv){
		outP->TARGET_DELTA = 1;
	}else{
		outP->TARGET_DELTA = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err TargetDelta_8(CFsAE *ae , A_long delta_range,PF_Boolean isMask)
{
	PF_Err	err = PF_Err_NONE;
	DeltaSubParam info;
	info.width = ae->out->width()-2;
	info.height = ae->out->height()-2;
	info.lv		= (A_long)(PF_MAX_CHAN8 * delta_range >> 16);
	info.isAlpha	= isMask;
	info.widthTrue = ae->in->widthTrue(); 
	err = ae->iterate8((refconType)&info,DeltaSub8);
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err TargetDelta_16(CFsAE *ae , A_long delta_range,PF_Boolean isMask)
{
	PF_Err	err = PF_Err_NONE;
	DeltaSubParam info;
	info.width = ae->out->width()-2;
	info.height = ae->out->height()-2;
	info.lv		= (A_long)(PF_MAX_CHAN8 * delta_range >> 16);
	info.isAlpha	= isMask;
	info.widthTrue = ae->in->widthTrue(); 
	err = ae->iterate16((refconType)&info,DeltaSub16);
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err TargetDelta_32(CFsAE *ae , A_long delta_range,PF_Boolean isMask)
{
	PF_Err	err = PF_Err_NONE;
	DeltaSubParam info;
	info.width = ae->out->width()-2;
	info.height = ae->out->height()-2;
	info.lv		= (A_long)(PF_MAX_CHAN8 * delta_range >> 16);
	info.isAlpha	= isMask;
	info.widthTrue = ae->in->widthTrue(); 
	err = ae->iterate32((refconType)&info,DeltaSub32);
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindTargetDeltaBright(CFsAE *ae , A_long delta_range)
{
	PF_Err	err = PF_Err_NONE;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		err = TargetDelta_32(ae,delta_range,FALSE);
		break;
	case PF_PixelFormat_ARGB64:
		err = TargetDelta_16(ae,delta_range,FALSE);
		break;
	case PF_PixelFormat_ARGB32:
		err = TargetDelta_8(ae,delta_range,FALSE);
		break;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindTargetDeltaAlpha(CFsAE *ae , A_long delta_range)
{
	PF_Err	err = PF_Err_NONE;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		err = TargetDelta_32(ae,delta_range,TRUE);
		break;
	case PF_PixelFormat_ARGB64:
		err = TargetDelta_16(ae,delta_range,TRUE);
		break;
	case PF_PixelFormat_ARGB32:
		err = TargetDelta_8(ae,delta_range,TRUE);
		break;
	}
	return err;
}
