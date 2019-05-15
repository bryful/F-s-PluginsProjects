#include "Star.h"
//***********************************************************************************
//-----------------------------------------------------------------------------------
inline PF_Pixel MixPx8(PF_Pixel s,PF_Pixel d,PF_FpShort a)
{
	
	// 1 - ( ( 1 - Pa ) * ( 1 - Pb ) )
	//   ( Pb + Pa - Pa*Pb  ) 
	
	PF_Pixel ret;
	A_u_char a2;
	a2 = (A_u_char)(d.red * a);
	ret.red		= RoundByteLong(s.red + a2 - (s.red * a2)/PF_MAX_CHAN8); 
	a2 = (A_u_char)(d.green * a);
	ret.green	= RoundByteLong(s.green + a2 - (s.green * a2)/PF_MAX_CHAN8); 
	a2 = (A_u_char)(d.blue * a);
	ret.blue	= RoundByteLong(s.blue + a2 - (s.blue * a2)/PF_MAX_CHAN8); 
	ret.alpha = s.alpha;
	return ret;
}
//-----------------------------------------------------------------------------------
inline PF_Pixel16 MixPx16(PF_Pixel16 s,PF_Pixel16 d,PF_FpShort a)
{
	
	// 1 - ( ( 1 - Pa ) * ( 1 - Pb ) )
	//   ( Pb + Pa - Pa*Pb  ) 
	
	PF_Pixel16 ret;
	A_u_short a2;
	a2 = (A_u_short)(d.red * a);
	ret.red		= RoundShortFpLong(s.red + a2 - (s.red * a2)/PF_MAX_CHAN16); 
	a2 = (A_u_short)(d.green * a);
	ret.green	= RoundShortFpLong(s.green + a2 - (s.green * a2)/PF_MAX_CHAN16); 
	a2 = (A_u_short)(d.blue * a);
	ret.blue	= RoundShortFpLong(s.blue + a2 - (s.blue * a2)/PF_MAX_CHAN16); 
	ret.alpha = s.alpha;
	return ret;
}
//-----------------------------------------------------------------------------------
inline PF_PixelFloat MixPx32(PF_PixelFloat s,PF_PixelFloat d,PF_FpShort a)
{
	
	// 1 - ( ( 1 - Pa ) * ( 1 - Pb ) )
	//   ( Pb + Pa - Pa*Pb  ) 
	
	PF_PixelFloat ret;
	PF_FpShort a2;
	a2 = (PF_FpShort)(d.red * a);
	ret.red		= RoundFpShortDouble(s.red + a2 - (s.red * a2)); 
	a2 = (PF_FpShort)(d.green * a);
	ret.green	= RoundFpShortDouble(s.green + a2 - (s.green * a2)); 
	a2 = (PF_FpShort)(d.blue * a);
	ret.blue	= RoundFpShortDouble(s.blue + a2 - (s.blue * a2)); 
	ret.alpha = s.alpha;
	return ret;
}
//***********************************************************************************
//-----------------------------------------------------------------------------------
inline PF_Boolean pset8(CFsGraph *g, PF_Fixed x,PF_Fixed y, PF_Pixel p)
{
	PF_Boolean ret = FALSE;
	A_long x0 = x >>16;
	A_long y0 = y >>16;
	A_long x1 = x0+1;
	A_long y1 = y0+1;
	A_long w = g->width();
	A_long h = g->height();
	if ((x0<-2)||(y0<-2)|| (x0>=w)||(y0>=h)||(x1<0)||(y1<0)) return FALSE;

	PF_FpShort xd1 = (PF_FpShort)(x &0xFFFF)/65536;
	PF_FpShort yd1 = (PF_FpShort)(y &0xFFFF)/65536;
	PF_FpShort xd0 = 1-xd1;
	PF_FpShort yd0 = 1-yd1;
	PF_Pixel *data;
	data = (PF_Pixel *)g->data();
	A_long adr = x0 + y0 * g->widthTrue();
	A_long ww = g->widthTrue() * h; 
	if ( (xd0==1)&&(yd0=1)){
		if (( adr>=0)&&(adr<ww)) {
			data[adr] = MixPx8(data[adr],p,1);
			ret = TRUE;
		}
	}else{
		if ( (x0>=0)&&(y0>=0)){
			if (( adr>=0)&&(adr<ww)) {
				data[adr] = MixPx8(data[adr],p,xd0*yd0);
				ret = TRUE;
			}
		}
		adr++;
		if ( (x1<w)&&(y0>=0)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx8(data[adr],p,xd1*yd0);
				ret = TRUE;
			}
		}
		adr+= g->widthTrue();
		if ( (x1<w)&&(y1<h)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx8(data[adr],p,xd1*yd1);
				ret = TRUE;
			}
		}
		adr--;
		if ( (x>=0)&&(y1<h)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx8(data[adr],p,xd0*yd1);
				ret = TRUE;
			}
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------------
inline PF_Boolean pset16(CFsGraph *g, PF_Fixed x,PF_Fixed y, PF_Pixel16 p)
{
	PF_Boolean ret = FALSE;
	A_long x0 = x >>16;
	A_long y0 = y >>16;
	A_long x1 = x0+1;
	A_long y1 = y0+1;
	A_long w = g->width();
	A_long h = g->height();
	if ((x0<-2)||(y0<-2)|| (x0>=w)||(y0>=h)||(x1<0)||(y1<0)) return FALSE;

	PF_FpShort xd1 = (PF_FpShort)(x &0xFFFF)/65536;
	PF_FpShort yd1 = (PF_FpShort)(y &0xFFFF)/65536;
	PF_FpShort xd0 = 1-xd1;
	PF_FpShort yd0 = 1-yd1;
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)g->data();
	A_long adr = x0 + y0 * g->widthTrue();
	A_long ww = g->widthTrue() * h; 
	if ( (xd0==1)&&(yd0=1)){
		if (( adr>=0)&&(adr<ww)) {
			data[adr] = MixPx16(data[adr],p,1);
			ret = TRUE;
		}
	}else{
		if ( (x0>=0)&&(y0>=0)){
			if (( adr>=0)&&(adr<ww)) {
				data[adr] = MixPx16(data[adr],p,xd0*yd0);
				ret = TRUE;
			}
		}
		adr++;
		if ( (x1<w)&&(y0>=0)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx16(data[adr],p,xd1*yd0);
				ret = TRUE;
			}
		}
		adr+= g->widthTrue();
		if ( (x1<w)&&(y1<h)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx16(data[adr],p,xd1*yd1);
				ret = TRUE;
			}
		}
		adr--;
		if ( (x>=0)&&(y1<h)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx16(data[adr],p,xd0*yd1);
				ret = TRUE;
			}
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------------
inline PF_Boolean pset32(CFsGraph *g, PF_Fixed x,PF_Fixed y, PF_PixelFloat p)
{
	PF_Boolean ret = FALSE;
	A_long x0 = x >>16;
	A_long y0 = y >>16;
	A_long x1 = x0+1;
	A_long y1 = y0+1;
	A_long w = g->width();
	A_long h = g->height();
	if ((x0<-2)||(y0<-2)|| (x0>=w)||(y0>=h)||(x1<0)||(y1<0)) return FALSE;

	PF_FpShort xd1 = (PF_FpShort)(x &0xFFFF)/65536;
	PF_FpShort yd1 = (PF_FpShort)(y &0xFFFF)/65536;
	PF_FpShort xd0 = 1-xd1;
	PF_FpShort yd0 = 1-yd1;
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)g->data();
	A_long adr = x0 + y0 * g->widthTrue();
	A_long ww = g->widthTrue() * h; 
	if ( (xd0==1)&&(yd0=1)){
		if (( adr>=0)&&(adr<ww)) {
			data[adr] = MixPx32(data[adr],p,1);
			ret = TRUE;
		}
	}else{
		if ( (x0>=0)&&(y0>=0)){
			if (( adr>=0)&&(adr<ww)) {
				data[adr] = MixPx32(data[adr],p,xd0*yd0);
				ret = TRUE;
			}
		}
		adr++;
		if ( (x1<w)&&(y0>=0)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx32(data[adr],p,xd1*yd0);
				ret = TRUE;
			}
		}
		adr+= g->widthTrue();
		if ( (x1<w)&&(y1<h)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx32(data[adr],p,xd1*yd1);
				ret = TRUE;
			}
		}
		adr--;
		if ( (x>=0)&&(y1<h)){
			if (( adr>=0)&&(adr<ww)){
				data[adr] = MixPx32(data[adr],p,xd0*yd1);
				ret = TRUE;
			}
		}
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------
PF_Pixel calcColorTableOnly8(ColorTableInfo8 *ci)
{
	PF_Pixel ret = {0,0,0,0};
	if ( (ci->length<=0)|| ( ci->strong<=0)||(ci->length <= ci->value)) return ret;
		
	PF_FpLong op = ci->strong * (PF_FpLong)( ci->length - ci->value) / (PF_FpLong)ci->length;

	ret.blue	= RoundByteFpLong(ci->col1.blue  *op);
	ret.green	= RoundByteFpLong(ci->col1.green *op);
	ret.red		= RoundByteFpLong(ci->col1.red   *op);
	return ret;
	
}
//-------------------------------------------------------------------------------------------------
PF_Pixel16 calcColorTableOnly16(ColorTableInfo16 *ci)
{
	PF_Pixel16 ret = {0,0,0,0};
	if ( (ci->length<=0)|| ( ci->strong<=0)||(ci->length <= ci->value)) return ret;
		
	PF_FpLong op = ci->strong * (PF_FpLong)( ci->length - ci->value) / (PF_FpLong)ci->length;

	ret.blue	= RoundShortFpLong(ci->col1.blue  *op);
	ret.green	= RoundShortFpLong(ci->col1.green *op);
	ret.red		= RoundShortFpLong(ci->col1.red   *op);
	return ret;
	
}
//-------------------------------------------------------------------------------------------------
PF_PixelFloat calcColorTableOnly32(ColorTableInfo32 *ci)
{
	PF_PixelFloat ret = {0,0,0,0};
	if ( (ci->length<=0)|| ( ci->strong<=0)||(ci->length <= ci->value)) return ret;
		
	PF_FpLong op = ci->strong * (PF_FpLong)( ci->length - ci->value) / (PF_FpLong)ci->length;

	ret.blue	= RoundFpShortDouble(ci->col1.blue  *op);
	ret.green	= RoundFpShortDouble(ci->col1.green *op);
	ret.red		= RoundFpShortDouble(ci->col1.red   *op);
	return ret;
	
}
//-------------------------------------------------------------------------------------------------
PF_Pixel calcColorTable8(ColorTableInfo8 *ci)
{
	PF_Pixel ret = {0,0,0,0};
	if ( (ci->length<=0)|| ( ci->strong<=0)||(ci->length <= ci->value)) return ret;

	PF_FpLong op = ci->strong * (PF_FpLong)( ci->length - ci->value) / (PF_FpLong)ci->length;

	A_long bd = (A_long)(ci->length * ci->border);
	if ( (bd<=0)||(ci->value  >= bd)){
		ret.blue	= RoundByteFpLong(ci->col2.blue  *op);
		ret.green	= RoundByteFpLong(ci->col2.green *op);
		ret.red		= RoundByteFpLong(ci->col2.red   *op);
	}else{
		PF_FpLong d2 =  (PF_FpLong)ci->value / (PF_FpLong)bd;
		PF_FpLong d1 =  1 - d2;
		ret.blue	= RoundByteFpLong((ci->col1.blue  * d1 + ci->col2.blue  * d2)*op);
		ret.green	= RoundByteFpLong((ci->col1.green * d1 + ci->col2.green * d2)*op);
		ret.red		= RoundByteFpLong((ci->col1.red   * d1 + ci->col2.red   * d2)*op);
	}
	return ret;

}
//-------------------------------------------------------------------------------------------------
PF_Pixel16 calcColorTable16(ColorTableInfo16 *ci)
{
	PF_Pixel16 ret = {0,0,0,0};
	if ( (ci->length<=0)|| ( ci->strong<=0)||(ci->length <= ci->value)) return ret;

	PF_FpLong op = ci->strong * (PF_FpLong)( ci->length - ci->value) / (PF_FpLong)ci->length;

	A_long bd = (A_long)(ci->length * ci->border);
	if ( (bd<=0)||(ci->value  >= bd)){
		ret.blue	= RoundShortFpLong(ci->col2.blue  *op);
		ret.green	= RoundShortFpLong(ci->col2.green *op);
		ret.red		= RoundShortFpLong(ci->col2.red   *op);
	}else{
		PF_FpLong d2 =  (PF_FpLong)ci->value / (PF_FpLong)bd;
		PF_FpLong d1 =  1 - d2;
		ret.blue	= RoundShortFpLong((ci->col1.blue  * d1 + ci->col2.blue  * d2)*op);
		ret.green	= RoundShortFpLong((ci->col1.green * d1 + ci->col2.green * d2)*op);
		ret.red		= RoundShortFpLong((ci->col1.red   * d1 + ci->col2.red   * d2)*op);
	}
	return ret;

}
//-------------------------------------------------------------------------------------------------
PF_PixelFloat calcColorTable32(ColorTableInfo32 *ci)
{
	PF_PixelFloat ret = {0,0,0,0};
	if ( (ci->length<=0)|| ( ci->strong<=0)||(ci->length <= ci->value)) return ret;

	PF_FpLong op = ci->strong * (PF_FpLong)( ci->length - ci->value) / (PF_FpLong)ci->length;

	A_long bd = (A_long)(ci->length * ci->border);
	if ( (bd<=0)||(ci->value  >= bd)){
		ret.blue	= RoundFpShortDouble(ci->col2.blue  *op);
		ret.green	= RoundFpShortDouble(ci->col2.green *op);
		ret.red		= RoundFpShortDouble(ci->col2.red   *op);
	}else{
		PF_FpLong d2 =  (PF_FpLong)ci->value / (PF_FpLong)bd;
		PF_FpLong d1 =  1 - d2;
		ret.blue	= RoundFpShortDouble((ci->col1.blue  * d1 + ci->col2.blue  * d2)*op);
		ret.green	= RoundFpShortDouble((ci->col1.green * d1 + ci->col2.green * d2)*op);
		ret.red		= RoundFpShortDouble((ci->col1.red   * d1 + ci->col2.red   * d2)*op);
	}
	return ret;

}
PF_Pixel (*calcColor8)(ColorTableInfo8 *ci);
PF_Pixel16 (*calcColor16)(ColorTableInfo16 *ci);
PF_PixelFloat (*calcColor32)(ColorTableInfo32 *ci);

void makeTable8(ParamInfo8 *infoP)
{
	for ( A_long i=0; i< infoP->colTable.length; i++){
		infoP->colTable.value = i;
		infoP->tbl[i] = calcColor8(&infoP->colTable);
	}
}
void makeTable16(ParamInfo16 *infoP)
{
	for ( A_long i=0; i< infoP->colTable.length; i++){
		infoP->colTable.value = i;
		infoP->tbl[i] = calcColor16(&infoP->colTable);
	}
}
void makeTable32(ParamInfo32 *infoP)
{
	for ( A_long i=0; i< infoP->colTable.length; i++){
		infoP->colTable.value = i;
		infoP->tbl[i] = calcColor32(&infoP->colTable);
	}
}
//-------------------------------------------------------------------------------------------------
void DrawLine8(ParamInfo8 *infoP)
{
	
	CRotCalc c(infoP->in_data);
	c.SetRotLength(infoP->colTable.angle,infoP->colTable.length<<16);
	PF_Fixed dx = c.x() / infoP->colTable.length;
	PF_Fixed dy = c.y() / infoP->colTable.length;
	PF_Fixed x = infoP->x <<16;
	PF_Fixed y = infoP->y <<16;
	PF_Pixel *tbl = (PF_Pixel *)infoP->tbl;
	PF_Fixed w = infoP->g->width() << 16;
	PF_Fixed h = infoP->g->height() << 16;
	for ( A_long i=0; i<infoP->colTable.length; i++){
		if (pset8(infoP->g,x,y,tbl[i])==FALSE) break;
		x+=dx;
		y+=dy;
		if ( x >= w)break;
		if ( y >= h)break;
	}
	x = infoP->x <<16;
	y = infoP->y <<16;
	for ( A_long i=0; i<infoP->colTable.length; i++){
		if (pset8(infoP->g,x,y,tbl[i])==FALSE) break;
		x-=dx;
		y-=dy;
		if ( x <-1)break;
		if ( y <-1)break;
	}
}
//-------------------------------------------------------------------------------------------------
void DrawLine16(ParamInfo16 *infoP)
{
	
	CRotCalc c(infoP->in_data);
	c.SetRotLength(infoP->colTable.angle,infoP->colTable.length<<16);
	PF_Fixed dx = c.x() / infoP->colTable.length;
	PF_Fixed dy = c.y() / infoP->colTable.length;
	PF_Fixed x = infoP->x <<16;
	PF_Fixed y = infoP->y <<16;
	PF_Pixel16 *tbl = (PF_Pixel16 *)infoP->tbl;
	PF_Fixed w = infoP->g->width() << 16;
	PF_Fixed h = infoP->g->height() << 16;
	for ( A_long i=0; i<infoP->colTable.length; i++){
		if (pset16(infoP->g,x,y,tbl[i])==FALSE) break;
		x+=dx;
		y+=dy;
		if ( x >= w)break;
		if ( y >= h)break;
	}
	x = infoP->x <<16;
	y = infoP->y <<16;
	for ( A_long i=0; i<infoP->colTable.length; i++){
		if (pset16(infoP->g,x,y,tbl[i])==FALSE) break;
		x-=dx;
		y-=dy;
		if ( x <-1)break;
		if ( y <-1)break;
	}
}
//-------------------------------------------------------------------------------------------------
void DrawLine32(ParamInfo32 *infoP)
{
	
	CRotCalc c(infoP->in_data);
	c.SetRotLength(infoP->colTable.angle,infoP->colTable.length<<16);
	PF_Fixed dx = c.x() / infoP->colTable.length;
	PF_Fixed dy = c.y() / infoP->colTable.length;
	PF_Fixed x = infoP->x <<16;
	PF_Fixed y = infoP->y <<16;
	PF_PixelFloat *tbl = (PF_PixelFloat *)infoP->tbl;
	PF_Fixed w = infoP->g->width() << 16;
	PF_Fixed h = infoP->g->height() << 16;
	for ( A_long i=0; i<infoP->colTable.length; i++){
		if (pset32(infoP->g,x,y,tbl[i])==FALSE) break;
		x+=dx;
		y+=dy;
		if ( x >= w)break;
		if ( y >= h)break;
	}
	x = infoP->x <<16;
	y = infoP->y <<16;
	for ( A_long i=0; i<infoP->colTable.length; i++){
		if (pset32(infoP->g,x,y,tbl[i])==FALSE) break;
		x-=dx;
		y-=dy;
		if ( x <-1)break;
		if ( y <-1)break;
	}
}
//-------------------------------------------------------------------------------------------------
PF_Err StarDrawSub8(ParamInfo8 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long x,y;
	PF_Pixel bl = {0,0,0,0};

	PF_Pixel *data;
	data = infoP->g->data();
	A_long target =0;
	for ( y=0;y <infoP->g->height(); y++){
		infoP->y = y;
		for ( x=0; x<infoP->g->width(); x++){
			infoP->x = x;
			A_long base = (A_long)data[target].alpha;
			if (base>0){
				for ( A_long i=0 ; i<4; i++){
					infoP->colTable.angle	= infoP->line[i].angle;
					infoP->colTable.length	= infoP->line[i].length * base /PF_MAX_CHAN8;
					infoP->colTable.strong	= infoP->line[i].strong * (PF_FpShort)base/PF_MAX_CHAN8;
					if ( (infoP->colTable.length>0)&&(infoP->colTable.strong>0)){
						makeTable8(infoP);
						DrawLine8(infoP);
					}
				}

			}
			target++;
		}
		target += infoP->g->offsetWidth();
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarDrawSub16(ParamInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long x,y;

	PF_Pixel16 *data;
	data = (PF_Pixel16 *)infoP->g->data();
	A_long target =0;
	for ( y=0;y <infoP->g->height(); y++){
		infoP->y = y;
		for ( x=0; x<infoP->g->width(); x++){
			infoP->x = x;
			A_long base = (A_long)data[target].alpha;
			if (base>0){
				for ( A_long i=0 ; i<4; i++){
					infoP->colTable.angle	= infoP->line[i].angle;
					infoP->colTable.length	= infoP->line[i].length * base /PF_MAX_CHAN16;
					infoP->colTable.strong	= infoP->line[i].strong * (PF_FpShort)base/PF_MAX_CHAN16;
					if ( (infoP->colTable.length>0)&&(infoP->colTable.strong>0)){
						makeTable16(infoP);
						DrawLine16(infoP);
					}
				}

			}
			target++;
		}
		target += infoP->g->offsetWidth();
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarDrawSub32(ParamInfo32 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long x,y;

	PF_PixelFloat *data;
	data = (PF_PixelFloat *)infoP->g->data();
	A_long target =0;
	for ( y=0;y <infoP->g->height(); y++){
		infoP->y = y;
		for ( x=0; x<infoP->g->width(); x++){
			infoP->x = x;
			PF_FpShort base = data[target].alpha;
			if (base>0){
				for ( A_long i=0 ; i<4; i++){
					infoP->colTable.angle	= infoP->line[i].angle;
					infoP->colTable.length	= (A_long)(infoP->line[i].length * base);
					infoP->colTable.strong	= infoP->line[i].strong * base;
					if ( (infoP->colTable.length>0)&&(infoP->colTable.strong>0)){
						makeTable32(infoP);
						DrawLine32(infoP);
					}
				}

			}
			target++;
		}
		target += infoP->g->offsetWidth();
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarDrawSubScreen8(ParamInfo8 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long x,y;

	PF_Pixel *data;
	data = infoP->g->data();
	A_long target =0;
	for ( y=0;y <infoP->g->height(); y++){
		infoP->y = y;
		for ( x=0; x<infoP->g->width(); x++){
			infoP->x = x;
			PF_Pixel c = data[target];
			A_long base = (A_long)c.alpha;
			if (base>0){
				infoP->colTable.col1 = c;
				infoP->colTable.col2 = c;
				for ( A_long i=0 ; i<4; i++){
					infoP->colTable.angle	= infoP->line[i].angle;
					infoP->colTable.length	= infoP->line[i].length * base /PF_MAX_CHAN8;
					infoP->colTable.strong	= infoP->line[i].strong * (PF_FpShort)base/PF_MAX_CHAN8;
					if ( (infoP->colTable.length>0)&&(infoP->colTable.strong>0)){
						makeTable8(infoP);
						DrawLine8(infoP);
					}
				}

			}
			target++;
		}
		target += infoP->g->offsetWidth();
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarDrawSubScreen16(ParamInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long x,y;

	PF_Pixel16 *data;
	data = (PF_Pixel16 *)infoP->g->data();
	A_long target =0;
	for ( y=0;y <infoP->g->height(); y++){
		infoP->y = y;
		for ( x=0; x<infoP->g->width(); x++){
			infoP->x = x;
			PF_Pixel16 c = data[target];
			A_long base = (A_long)c.alpha;
			if (base>0){
				infoP->colTable.col1 = c;
				infoP->colTable.col2 = c;
				for ( A_long i=0 ; i<4; i++){
					infoP->colTable.angle	= infoP->line[i].angle;
					infoP->colTable.length	= (A_long)(infoP->line[i].length * base/PF_MAX_CHAN16);
					infoP->colTable.strong	= infoP->line[i].strong * (PF_FpShort)base/PF_MAX_CHAN16;
					if ( (infoP->colTable.length>0)&&(infoP->colTable.strong>0)){
						makeTable16(infoP);
						DrawLine16(infoP);
					}
				}

			}
			target++;
		}
		target += infoP->g->offsetWidth();
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarDrawSubScreen32(ParamInfo32 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long x,y;

	PF_PixelFloat *data;
	data = (PF_PixelFloat *)infoP->g->data();
	A_long target =0;
	for ( y=0;y <infoP->g->height(); y++){
		infoP->y = y;
		for ( x=0; x<infoP->g->width(); x++){
			infoP->x = x;
			PF_PixelFloat c = data[target];
			PF_FpShort base = c.alpha;
			if (base>0){
				infoP->colTable.col1 = c;
				infoP->colTable.col2 = c;
				for ( A_long i=0 ; i<4; i++){
					infoP->colTable.angle	= infoP->line[i].angle;
					infoP->colTable.length	= (A_long)(infoP->line[i].length * base);
					infoP->colTable.strong	= infoP->line[i].strong * base;
					if ( (infoP->colTable.length>0)&&(infoP->colTable.strong>0)){
						makeTable32(infoP);
						DrawLine32(infoP);
					}
				}

			}
			target++;
		}
		target += infoP->g->offsetWidth();
	}

	return err;
}
#define TABLE_SIZE	2500
//-------------------------------------------------------------------------------------------------
PF_Err StarDraw (CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	CFsBuffer buf(ae->in_data);
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(buf.Create(TABLE_SIZE * sizeof(PF_PixelFloat)));
		if (!err){
			ParamInfo32 info32;
			switch(infoP->colTable.kind)
			{
			case skAtoB:
			case skBtoA:
				calcColor32 = calcColorTable32;
				break;
			case skAonly:
			case skBonly:
			case skScreen:
				calcColor32 = calcColorTableOnly32;
				break;
			}
			info32.in_data	= ae->in_data;
			info32.g			= ae->out;
			for ( A_long i=0; i<4; i++){
				info32.line[i].length	= infoP->line[i].length;
				info32.line[i].angle	= infoP->line[i].angle;
				info32.line[i].strong	= infoP->line[i].strong;
			}

			info32.colTable.border		=  infoP->colTable.border;
			info32.colTable.col1		= CONV8TO32(infoP->colTable.col1);
			info32.colTable.col2		= CONV8TO32(infoP->colTable.col2);
			info32.colTable.in_data		= ae->in_data;
			info32.colTable.length = 0;
			info32.colTable.strong = 0;
			info32.colTable.value = 0; 

			info32.tbl				= buf.bufPixelFloat();
			//buf.Lock();
			if ( infoP->colTable.kind == skScreen){
				StarDrawSubScreen32(&info32);
			}else{
				StarDrawSub32(&info32);
			}
			buf.Dispose();
		}
		break;
	case PF_PixelFormat_ARGB64:
		ERR(buf.Create(TABLE_SIZE * sizeof(PF_Pixel16)));
		if (!err){
			ParamInfo16 info16;
			switch(infoP->colTable.kind)
			{
			case skAtoB:
			case skBtoA:
				calcColor16 = calcColorTable16;
				break;
			case skAonly:
			case skBonly:
			case skScreen:
				calcColor16 = calcColorTableOnly16;
				break;
			}
			info16.in_data	= ae->in_data;
			info16.g			= ae->out;
			for ( A_long i=0; i<4; i++){
				info16.line[i].length	= infoP->line[i].length;
				info16.line[i].angle	= infoP->line[i].angle;
				info16.line[i].strong	= infoP->line[i].strong;
			}

			info16.colTable.border		=  infoP->colTable.border;
			info16.colTable.col1		= CONV8TO16(infoP->colTable.col1);
			info16.colTable.col2		= CONV8TO16(infoP->colTable.col2);
			info16.colTable.in_data		= ae->in_data;
			info16.colTable.length = 0;
			info16.colTable.strong = 0;
			info16.colTable.value = 0; 

			info16.tbl				= buf.bufPixel16();
			//buf.Lock();
			if ( infoP->colTable.kind == skScreen){
				StarDrawSubScreen16(&info16);
			}else{
				StarDrawSub16(&info16);
			}
			buf.Dispose();
		}
		break;
	case PF_PixelFormat_ARGB32:
		ERR(buf.Create(TABLE_SIZE * sizeof(PF_Pixel)));
		if (!err){
			ParamInfo8 info8;
			switch(infoP->colTable.kind)
			{
			case skAtoB:
			case skBtoA:
				calcColor8 = calcColorTable8;
				break;
			case skAonly:
			case skBonly:
			case skScreen:
				calcColor8 = calcColorTableOnly8;
				break;
			}
			info8.in_data	= ae->in_data;
			info8.g			= ae->out;
			for ( A_long i=0; i<4; i++){
				info8.line[i].length	= infoP->line[i].length;
				info8.line[i].angle		= infoP->line[i].angle;
				info8.line[i].strong	= infoP->line[i].strong;
			}

			info8.colTable.border	= infoP->colTable.border;
			info8.colTable.col1		= infoP->colTable.col1;
			info8.colTable.col2		= infoP->colTable.col2;
			info8.colTable.in_data	= ae->in_data;
			info8.colTable.length = 0;
			info8.colTable.strong = 0;
			info8.colTable.value = 0; 

			info8.tbl				= buf.bufPixel();
			//buf.Lock();
			if ( infoP->colTable.kind == skScreen){
				StarDrawSubScreen8(&info8);
			}else{
				StarDrawSub8(&info8);
			}
			buf.Dispose();
		}
		break;
	}
	return err;
}
