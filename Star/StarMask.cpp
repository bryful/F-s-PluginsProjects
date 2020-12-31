
#include "Star.h"
typedef struct MinParam
{
	A_long		*scanline;
	PF_FpShort	*scanlineD;
	A_long		scanlineLength;	//scanlineの長さ
	A_long		value;	//Max/Minの値
	
	A_long		target;	//scanlineのターゲットインデックス
	A_long		maxIndex;
}MinParam;


//----------------------------------------------------------------------------
static A_long getMin(MinParam *mm)
{
	A_long start = mm->target - mm->value;
	if ( start<0) start = 0;
	A_long end = mm->target + mm->value;
	if ( end >= mm->scanlineLength) end = mm->scanlineLength-1;
	if ( (mm->maxIndex>=start)&&(mm->maxIndex<=end))
	{
		if (mm->scanline[end] <=mm->scanline[mm->maxIndex]){
			mm->maxIndex = end;
			return mm->scanline[end];
		}else{
			return mm->scanline[mm->maxIndex];
		}
	}
	A_long idx = start;
	for ( A_long i=start; i<=end; i++)
	{
		if ( mm->scanline[i] <=0){
			idx = i;
			break;
		}else{
			if ( mm->scanline[i] < mm->scanline[idx]) idx = i;
		}
	}
	mm->maxIndex =idx;
	return mm->scanline[idx];
} 
//----------------------------------------------------------------------------
static PF_FpShort getMinD(MinParam *mm)
{
	A_long start = mm->target - mm->value;
	if ( start<0) start = 0;
	A_long end = mm->target + mm->value;
	if ( end >= mm->scanlineLength) end = mm->scanlineLength-1;
	if ( (mm->maxIndex>=start)&&(mm->maxIndex<=end))
	{
		if (mm->scanlineD[end] <=mm->scanlineD[mm->maxIndex]){
			mm->maxIndex = end;
			return mm->scanlineD[end];
		}else{
			return mm->scanlineD[mm->maxIndex];
		}
	}
	A_long idx = start;
	for ( A_long i=start; i<=end; i++)
	{
		if ( mm->scanlineD[i] <=0){
			idx = i;
			break;
		}else{
			if ( mm->scanlineD[i] < mm->scanlineD[idx]) idx = i;
		}
	}
	mm->maxIndex =idx;
	return mm->scanlineD[idx];
} 
//-------------------------------------------------------------------------------------------------
void MinMask8(CFsAE *ae,A_long min)
{
	PF_Err err = PF_Err_NONE;
	MinParam mm;
	mm.value = min;
	A_long w = ae->out->width();
	if ( ae->out->width()<ae->out->height()) w = ae->out->height();
	CFsBuffer scanline = ae->NewBuffer(w*sizeof(A_long));
	err = scanline.err();
	if (!err){
		PF_Pixel *data;
		data = (PF_Pixel *)ae->out->data();
		mm.scanline	= scanline.bufA_long();
		mm.scanlineLength = ae->out->width();
		//水平方向
		A_long hor =0; 
		A_long x,y;
		for (y=0; y< ae->out->height(); y++)
		{
			hor = ae->out->widthTrue() * y;
			//スキャンライン分転送
			for (x=0; x<ae->out->width(); x++){
				mm.scanline[x]	= (A_long)data[hor + x].alpha;
			}
			mm.maxIndex = -1;
			for (x=0; x<ae->out->width(); x++){
				mm.target = x;
				data[hor + x].alpha = (A_u_char)getMin(&mm);
			}
		}
	
		mm.scanlineLength =  ae->out->height();
		for ( x=0; x<ae->out->width(); x++)
		{
			hor = x;
			for ( y=0; y<ae->out->height(); y++){
				mm.scanline[y] = data[hor].alpha;
				hor += ae->out->widthTrue();
			}
			hor = x;
			mm.maxIndex = -1;
			for ( y=0; y<ae->out->height(); y++){
				mm.target = y;
				data[hor].alpha = (A_u_char)getMin(&mm);
				hor += ae->out->widthTrue();
			}
		}
		scanline.Dispose();
	}
}
//-------------------------------------------------------------------------------------------------
void MinMask16(CFsAE *ae,A_long min)
{
	PF_Err err = PF_Err_NONE;
	MinParam mm;
	mm.value = min;
	A_long w = ae->out->width();
	if ( ae->out->width()<ae->out->height()) w = ae->out->height();
	CFsBuffer scanline = ae->NewBuffer(w*sizeof(A_long));
	err = scanline.err();
	if (!err){
		PF_Pixel16 *data;
		data = (PF_Pixel16 *)ae->out->data();
		mm.scanline	= scanline.bufA_long();

		mm.scanlineLength = ae->out->width();
		//水平方向
		A_long hor =0; 
		A_long x,y;
		for (y=0; y< ae->out->height(); y++)
		{
			hor = ae->out->widthTrue() * y;
			//スキャンライン分転送
			for (x=0; x<ae->out->width(); x++){
				mm.scanline[x]	= (A_long)data[hor + x].alpha;
			}
			mm.maxIndex = -1;
			for (x=0; x<ae->out->width(); x++){
				mm.target = x;
				data[hor + x].alpha = (A_u_short)getMin(&mm);
			}
		}
	
		mm.scanlineLength =  ae->out->height();
		for ( x=0; x<ae->out->width(); x++)
		{
			hor = x;
			for ( y=0; y<ae->out->height(); y++){
				mm.scanline[y] = data[hor].alpha;
				hor += ae->out->widthTrue();
			}
			hor = x;
			mm.maxIndex = -1;
			for ( y=0; y<ae->out->height(); y++){
				mm.target = y;
				data[hor].alpha = (A_u_short)getMin(&mm);
				hor += ae->out->widthTrue();
			}
		}
		scanline.Dispose();
	}
}
//-------------------------------------------------------------------------------------------------
void MinMask32(CFsAE *ae,A_long min)
{
	PF_Err err = PF_Err_NONE;
	MinParam mm;
	mm.value = min;
	A_long w = ae->out->width();
	if ( ae->out->width()<ae->out->height()) w = ae->out->height();
	CFsBuffer scanline = ae->NewBuffer(w*sizeof(PF_FpShort));
	err = scanline.err();
	if (!err){
		PF_PixelFloat *data;
		data = (PF_PixelFloat *)ae->out->data();
		mm.scanlineD = scanline.bufFpShort();

		mm.scanlineLength = ae->out->width();
		//水平方向
		A_long hor =0; 
		A_long x,y;
		for (y=0; y< ae->out->height(); y++)
		{
			hor = ae->out->widthTrue() * y;
			//スキャンライン分転送
			for (x=0; x<ae->out->width(); x++){
				mm.scanlineD[x]	= (PF_FpShort)data[hor + x].alpha;
			}
			mm.maxIndex = -1;
			for (x=0; x<ae->out->width(); x++){
				mm.target = x;
				data[hor + x].alpha = getMinD(&mm);
			}
		}
	
		mm.scanlineLength =  ae->out->height();
		for ( x=0; x<ae->out->width(); x++)
		{
			hor = x;
			for ( y=0; y<ae->out->height(); y++){
				mm.scanlineD[y] = data[hor].alpha;
				hor += ae->out->widthTrue();
			}
			hor = x;
			mm.maxIndex = -1;
			for ( y=0; y<ae->out->height(); y++){
				mm.target = y;
				data[hor].alpha =  getMinD(&mm);
				hor += ae->out->widthTrue();
			}
		}
		scanline.Dispose();
	}
}
//-------------------------------------------------------------------------------------------------
void MinMask(CFsAE *ae,A_long min)
{
	if ( min<=0) return;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		MinMask32(ae,min);
		break;
	case PF_PixelFormat_ARGB64:
		MinMask16(ae,min);
		break;
	case PF_PixelFormat_ARGB32:
		MinMask8(ae,min);
		break;
	}

}
//-----------------------------------------------------------------------------------
static PF_Err 
SubtrackAlpha8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	outP->alpha = RoundByteLong( outP->red - outP->alpha);	
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
SubtrackAlpha16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	outP->alpha = RoundShort( outP->red - outP->alpha);	
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
SubtrackAlpha32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	outP->alpha = RoundFpShort( outP->red - outP->alpha);	
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err SubtrackAlpha (CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32(NULL,SubtrackAlpha32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16(NULL,SubtrackAlpha16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8(NULL,SubtrackAlpha8));
		break;
	}
	return err;
}
