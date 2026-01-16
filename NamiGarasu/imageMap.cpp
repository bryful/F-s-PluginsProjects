#include "NamiGarasu.h"

//-------------------------------------------------------------------------------------------------
PF_Pixel getP8_sub(ScrInfo *infoP,A_long x, A_long y)
{
	PF_Pixel *data;
	data = (PF_Pixel *)infoP->data;
	
	if (x<0) x=0; else if(x>=infoP->width) x =infoP->width -1; 
	if (y<0) y=0; else if(y>=infoP->height) y =infoP->height -1; 

	return data[x + y * infoP->widthTrue];

}
//-------------------------------------------------------------------------------------------------
PF_Pixel16 getP16_sub(ScrInfo *infoP,A_long x, A_long y)
{
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)infoP->data;
	
	if (x<0) x=0; else if(x>=infoP->width) x =infoP->width -1; 
	if (y<0) y=0; else if(y>=infoP->height) y =infoP->height -1; 

	return data[x + y * infoP->widthTrue];

}
//-------------------------------------------------------------------------------------------------
PF_Pixel32 getP32_sub(ScrInfo *infoP,A_long x, A_long y)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)infoP->data;
	
	if (x<0) x=0; else if(x>=infoP->width) x =infoP->width -1; 
	if (y<0) y=0; else if(y>=infoP->height) y =infoP->height -1; 

	return data[x + y * infoP->widthTrue];

}
//-------------------------------------------------------------------------------------------------
PF_Pixel getP8(ScrInfo *infoP,PF_Fixed x,PF_Fixed y)
{
	PF_Pixel p[4];
	A_long xx,yy,dx1,dy1,dx2,dy2;
	xx = x >>16;
	yy = y >>16;
	dx1 = (x & 0xFFFF) >>8;
	dy1 = (y & 0xFFFF) >>8;
	dx2 = (1L<<8) - dx1;
	dy2 = (1L<<8) - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getP8_sub(infoP,xx,yy);
	}

	p[0] = getP8_sub(infoP,xx  ,yy  );
	p[1] = getP8_sub(infoP,xx+1,yy  );
	p[2] = getP8_sub(infoP,xx  ,yy+1);
	p[3] = getP8_sub(infoP,xx+1,yy+1);
	
	PF_Pixel ret;
	ret.alpha	= RoundByteLong(( ((A_long)p[0].alpha * dx2 * dy2) + ((A_long)p[1].alpha * dx1 * dy2) + ((A_long)p[2].alpha * dx2 * dy1) + ((A_long)p[3].alpha * dx1 * dy1) ) >>(16));
	ret.red		= RoundByteLong(( ((A_long)p[0].red * dx2 * dy2) + ((A_long)p[1].red * dx1 * dy2) + ((A_long)p[2].red * dx2 * dy1) + ((A_long)p[3].red * dx1 * dy1) ) >>(16));
	ret.green	= RoundByteLong(( ((A_long)p[0].green * dx2 * dy2) + ((A_long)p[1].green * dx1 * dy2) + ((A_long)p[2].green * dx2 * dy1) + ((A_long)p[3].green * dx1 * dy1) ) >>(16));
	ret.blue	= RoundByteLong(( ((A_long)p[0].blue * dx2 * dy2) + ((A_long)p[1].blue * dx1 * dy2) + ((A_long)p[2].blue * dx2 * dy1) + ((A_long)p[3].blue * dx1 * dy1) ) >>(16));

	

	return ret;
}
//-------------------------------------------------------------------------------------------------
PF_Pixel16 getP16(ScrInfo *infoP,PF_Fixed x,PF_Fixed y)
{
	PF_Pixel16 p[4];
	A_long xx,yy;
	double dx1,dy1,dx2,dy2;
	xx = x >>16;
	yy = y >>16;
	dx1 = ((double)(x & 0xFFFF))/65536.0;
	dy1 = ((double)(y & 0xFFFF))/65536.0;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getP16_sub(infoP,xx,yy);
	}

	p[0] = getP16_sub(infoP,xx  ,yy  );
	p[1] = getP16_sub(infoP,xx+1,yy  );
	p[2] = getP16_sub(infoP,xx  ,yy+1);
	p[3] = getP16_sub(infoP,xx+1,yy+1);
	
	PF_Pixel16 ret;
	ret.alpha	= RoundShortFpLong(( ((double)p[0].alpha * dx2 * dy2) + ((double)p[1].alpha * dx1 * dy2) + ((double)p[2].alpha * dx2 * dy1) + ((double)p[3].alpha * dx1 * dy1) ) );
	ret.red		= RoundShortFpLong(( ((double)p[0].red   * dx2 * dy2) + ((double)p[1].red   * dx1 * dy2) + ((double)p[2].red   * dx2 * dy1) + ((double)p[3].red   * dx1 * dy1) ) );
	ret.green	= RoundShortFpLong(( ((double)p[0].green * dx2 * dy2) + ((double)p[1].green * dx1 * dy2) + ((double)p[2].green * dx2 * dy1) + ((double)p[3].green * dx1 * dy1) ) );
	ret.blue	= RoundShortFpLong(( ((double)p[0].blue  * dx2 * dy2) + ((double)p[1].blue  * dx1 * dy2) + ((double)p[2].blue  * dx2 * dy1) + ((double)p[3].blue  * dx1 * dy1) ) );
	//ret.blue = PF_MAX_CHAN16;


	return ret;
}
//-------------------------------------------------------------------------------------------------
PF_Pixel32 getP32(ScrInfo *infoP,PF_Fixed x,PF_Fixed y)
{
	PF_PixelFloat p[4];
	A_long xx,yy;
	double dx1,dy1,dx2,dy2;
	xx = x >>16;
	yy = y >>16;
	dx1 = ((double)(x & 0xFFFF)) / (double)(1L<<16);
	dy1 = ((double)(y & 0xFFFF)) / (double)(1L<<16);
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getP32_sub(infoP,xx,yy);
	}

	p[0] = getP32_sub(infoP,xx  ,yy  );
	p[1] = getP32_sub(infoP,xx+1,yy  );
	p[2] = getP32_sub(infoP,xx  ,yy+1);
	p[3] = getP32_sub(infoP,xx+1,yy+1);
	
	PF_PixelFloat ret;
	ret.alpha	= RoundFpShortDouble(( ((double)p[0].alpha * dx2 * dy2) + ((double)p[1].alpha * dx1 * dy2) + ((double)p[2].alpha * dx2 * dy1) + ((double)p[3].alpha * dx1 * dy1) ) );
	ret.red		= RoundFpShortDouble(( ((double)p[0].red   * dx2 * dy2) + ((double)p[1].red   * dx1 * dy2) + ((double)p[2].red   * dx2 * dy1) + ((double)p[3].red   * dx1 * dy1) ) );
	ret.green	= RoundFpShortDouble(( ((double)p[0].green * dx2 * dy2) + ((double)p[1].green * dx1 * dy2) + ((double)p[2].green * dx2 * dy1) + ((double)p[3].green * dx1 * dy1) ) );
	ret.blue	= RoundFpShortDouble(( ((double)p[0].blue  * dx2 * dy2) + ((double)p[1].blue  * dx1 * dy2) + ((double)p[2].blue  * dx2 * dy1) + ((double)p[3].blue  * dx1 * dy1) ) );

	return ret;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
imageMap8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ShiftInfo *	niP		= reinterpret_cast<ShiftInfo*>(refcon);

	double shft = ((double)outP->alpha - PF_HALF_CHAN8)/PF_HALF_CHAN8;
	PF_Fixed sx = (PF_Fixed)(niP->shift_x * shft);
	PF_Fixed sy = (PF_Fixed)(niP->shift_y * shft);



	if ((sx==0)&&(sy==0)){
		*outP = getP8_sub(&niP->scr,xL,yL);	
	}else{
		sx = (xL<<16) - sx;
		sy = (yL<<16) - sy;
		A_long a = (A_long)(((double)outP->alpha - PF_HALF_CHAN8)  * niP->level);
		*outP = getP8(&niP->scr,sx,sy);
		if (niP->level>0){
			outP->blue	= RoundByteLong(outP->blue + a);
			outP->green	= RoundByteLong(outP->green + a);
			outP->red	= RoundByteLong(outP->red + a);
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
imageMap16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ShiftInfo *	niP		= reinterpret_cast<ShiftInfo*>(refcon);

	double shft = ((double)outP->alpha - PF_HALF_CHAN16)/PF_HALF_CHAN16;
	PF_Fixed sx = (PF_Fixed)(niP->shift_x * shft);
	PF_Fixed sy = (PF_Fixed)(niP->shift_y * shft);


	if ((sx==0)&&(sy==0)){
		*outP = getP16_sub(&niP->scr,xL,yL);	
	}else{
		sx = (xL<<16) - sx;
		sy = (yL<<16) - sy;
		A_long a = (A_long)(((double)outP->alpha - PF_HALF_CHAN16)  * niP->level);
		*outP = getP16(&niP->scr,sx,sy);
		if (niP->level>0){
			outP->blue	= RoundShort(outP->blue + a);
			outP->green	= RoundShort(outP->green + a);
			outP->red	= RoundShort(outP->red + a);
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
imageMap32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ShiftInfo *	niP		= reinterpret_cast<ShiftInfo*>(refcon);

	double d = (double)outP->alpha;
	if (d>1.0) d = 1.0;
	double shft = ( d - PF_HALF_CHAN32)*2;
	PF_Fixed sx = (PF_Fixed)(niP->shift_x * shft);
	PF_Fixed sy = (PF_Fixed)(niP->shift_y * shft);




	if ((sx==0)&&(sy==0)){
		*outP = getP32_sub(&niP->scr,xL,yL);	
	}else{
		sx = (xL<<16) - sx;
		sy = (yL<<16) - sy;
		double a = ((double)outP->alpha - PF_HALF_CHAN32)  * niP->level;
		*outP = getP32(&niP->scr,sx,sy);
		if (niP->level>0){
			outP->blue	= RoundFpShortDouble(outP->blue + a);
			outP->green	= RoundFpShortDouble(outP->green + a);
			outP->red	= RoundFpShortDouble(outP->red + a);
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------

