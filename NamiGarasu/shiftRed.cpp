#include "NamiGarasu.h"



//-------------------------------------------------------------------------------------------------
A_u_char getRed8_sub(ScrInfo *infoP,A_long x, A_long y)
{
	PF_Pixel *data;
	data = (PF_Pixel *)infoP->data;
	
	if (x<0) { x = x + infoP->width;}
	else if (x>=infoP->width) { x = x - infoP->width;}
	
	if (y<0) { y = y + infoP->height;}
	else if (y>=infoP->height) { y = y - infoP->height;}

	return data[x + y * infoP->widthTrue].red;

}
//-------------------------------------------------------------------------------------------------
A_u_short getRed16_sub(ScrInfo *infoP,A_long x, A_long y)
{
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)infoP->data;
	
	if (x<0) { x = x + infoP->width;}
	else if (x>=infoP->width) { x = x - infoP->width;}
	
	if (y<0) { y = y + infoP->height;}
	else if (y>=infoP->height) { y = y - infoP->height;}

	return data[x + y * infoP->widthTrue].red;

}
//-------------------------------------------------------------------------------------------------
PF_FpShort getRed32_sub(ScrInfo *infoP,A_long x, A_long y)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)infoP->data;
	
	if (x<0) { x = x + infoP->width;}
	else if (x>=infoP->width) { x = x - infoP->width;}
	
	if (y<0) { y = y + infoP->height;}
	else if (y>=infoP->height) { y = y - infoP->height;}

	return data[x + y * infoP->widthTrue].red;

}
//-------------------------------------------------------------------------------------------------
A_u_char getRed8(ScrInfo *infoP,PF_Fixed x,PF_Fixed y)
{
	A_long p[4];
	A_long xx,yy,dx1,dy1,dx2,dy2;
	xx = x >>16;
	yy = y >>16;
	dx1 = (x & 0xFFFF) >>8;
	dy1 = (y & 0xFFFF) >>8;
	dx2 = (1L<<8) - dx1;
	dy2 = (1L<<8) - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getRed8_sub(infoP,xx,yy);
	}

	p[0] = (A_long)getRed8_sub(infoP,xx  ,yy  );
	p[1] = (A_long)getRed8_sub(infoP,xx+1,yy  );
	p[2] = (A_long)getRed8_sub(infoP,xx  ,yy+1);
	p[3] = (A_long)getRed8_sub(infoP,xx+1,yy+1);
	
	A_long ret = ( (p[0] * dx2 * dy2) + (p[1] * dx1 * dy2) + (p[2] * dx2 * dy1) + (p[3] * dx1 * dy1) ) >>(16);
	if ( ret>PF_MAX_CHAN8) { ret=PF_MAX_CHAN8;}

	return (A_u_char)ret;
}
//-------------------------------------------------------------------------------------------------
A_u_short getRed16(ScrInfo *infoP,PF_Fixed x,PF_Fixed y)
{
	A_long p[4];
	A_long xx,yy;
	double dx1,dy1,dx2,dy2;
	xx = x >>16;
	yy = y >>16;
	dx1 = ((double)(x & 0xFFFF))/65536.0;
	dy1 = ((double)(y & 0xFFFF))/65536.0;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getRed16_sub(infoP,xx,yy);
	}

	p[0] = (A_long)getRed16_sub(infoP,xx  ,yy  );
	p[1] = (A_long)getRed16_sub(infoP,xx+1,yy  );
	p[2] = (A_long)getRed16_sub(infoP,xx  ,yy+1);
	p[3] = (A_long)getRed16_sub(infoP,xx+1,yy+1);
	
	return RoundShortFpLong( ((double)p[0] * dx2 * dy2) + ((double)p[1] * dx1 * dy2) + ((double)p[2] * dx2 * dy1) + ((double)p[3] * dx1 * dy1) );
}
//-------------------------------------------------------------------------------------------------
PF_FpShort getRed32(ScrInfo *infoP,PF_Fixed x,PF_Fixed y)
{
	double p[4];
	A_long xx,yy;
	double dx1,dy1,dx2,dy2;
	xx = x >>16;
	yy = y >>16;
	dx1 = ((double)(x & 0xFFFF))/65536;
	dy1 = ((double)(y & 0xFFFF))/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getRed32_sub(infoP,xx,yy);
	}

	p[0] = (double)getRed32_sub(infoP,xx  ,yy  );
	p[1] = (double)getRed32_sub(infoP,xx+1,yy  );
	p[2] = (double)getRed32_sub(infoP,xx  ,yy+1);
	p[3] = (double)getRed32_sub(infoP,xx+1,yy+1);
	
	double ret = ( (p[0] * dx2 * dy2) + (p[1] * dx1 * dy2) + (p[2] * dx2 * dy1) + (p[3] * dx1 * dy1) );
	if ( ret>1.0) { ret = 1.0;}

	return (PF_FpShort)ret;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
shiftRed8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ShiftInfo *	niP		= reinterpret_cast<ShiftInfo*>(refcon);

	A_u_char ret = PF_HALF_CHAN8;
	if ((niP->shift_x==0)&&(niP->shift_y==0)){
		ret = getRed8_sub(&niP->scr,xL,yL);	
	}else{
		PF_Fixed sx = (xL<<16) - niP->shift_x;
		PF_Fixed sy = (yL<<16) - niP->shift_y;
		ret = getRed8(&niP->scr,sx,sy);

	}
	if (ret != PF_HALF_CHAN8){
		//outP->alpha = RoundByteLong((A_long)outP->alpha + (A_long)ret - PF_HALF_CHAN8);
		A_long base = outP->alpha;
		if (ret<PF_HALF_CHAN8){
			ret =RoundByteLong( 2 * base * (A_long)ret/ PF_MAX_CHAN8);
		}else{
			A_long b = PF_MAX_CHAN8 - base;
			A_long r = PF_MAX_CHAN8 - (A_long)ret;
			r =RoundByteLong( 2 * b * r/ PF_MAX_CHAN8);
			ret = RoundByteLong(PF_MAX_CHAN8 - r);
		}
		outP->alpha =ret;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
shiftRed16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ShiftInfo *	niP		= reinterpret_cast<ShiftInfo*>(refcon);

	A_u_short ret = PF_HALF_CHAN16;
	if ((niP->shift_x==0)&&(niP->shift_y==0)){
		ret = getRed16_sub(&niP->scr,xL,yL);	
	}else{
		PF_Fixed sx = (xL<<16) - niP->shift_x;
		PF_Fixed sy = (yL<<16) - niP->shift_y;
		ret = getRed16(&niP->scr,sx,sy);

	}
	if (ret != PF_HALF_CHAN16){
		//outP->alpha = RoundByteLong((A_long)outP->alpha + (A_long)ret - PF_HALF_CHAN8);
		A_long base = outP->alpha;
		if (ret<PF_HALF_CHAN16){
			ret =RoundShort( 2 * base * (A_long)ret/ PF_MAX_CHAN16);
		}else{
			A_long b = PF_MAX_CHAN16 - base;
			A_long r = PF_MAX_CHAN16 - (A_long)ret;
			r =RoundShort( 2 * b * r/ PF_MAX_CHAN16);
			ret = RoundShort(PF_MAX_CHAN16 - r);
		}
		outP->alpha =ret;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
shiftRed32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ShiftInfo *	niP		= reinterpret_cast<ShiftInfo*>(refcon);

	double ret = PF_HALF_CHAN32;
	if ((niP->shift_x==0)&&(niP->shift_y==0)){
		ret = getRed32_sub(&niP->scr,xL,yL);	
	}else{
		PF_Fixed sx = (xL<<16) - niP->shift_x;
		PF_Fixed sy = (yL<<16) - niP->shift_y;
		ret = getRed32(&niP->scr,sx,sy);

	}
	if (ret != PF_HALF_CHAN32){
		//outP->alpha = RoundByteLong((A_long)outP->alpha + (A_long)ret - PF_HALF_CHAN8);
		double base = outP->alpha;
		if (base>1.0) base = 1.0;
		if (ret<PF_HALF_CHAN32){
			ret = 2 * base * ret;
		}else{
			double b = 1.0 - base;
			double r = 1.0 - ret;
			ret = 1.0 - 2 * b * r;
		}
		if (ret<0) ret = 0;
		else if( ret>1.0) ret = 1.0;
		outP->alpha =(PF_FpShort)ret;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
