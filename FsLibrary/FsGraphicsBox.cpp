#include "FsGraphics.h"


//-------------------------------------------------------------------
void CFsGraph::boxFill8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel col)
{
	PF_Pixel *data = (PF_Pixel *)m_data;
	A_long w		= m_width;
	A_long wt		= m_widthTrue;
	A_long wo;
	A_long h		= m_height;
	A_long xx0	= x0;
	A_long xx1	= x1;
	A_long yy0	= y0;
	A_long yy1	= y1;
	A_long target,i,j;
	PF_Pixel c = col;
	//
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if (yy0>yy1) swapLong(&yy0,&yy1);
	
	if ( (xx0>=w) || (yy0>=h) || (xx1<0) || (yy1<0) ) return;

	if (xx0<0) xx0=0;
	if (yy0<0) yy0=0;
	if (xx1>=w) xx1=w-1;
	if (yy1>=h) yy1=h-1;

	wo = wt -(xx1-xx0+1);
	target = xx0 + yy0 * wt;
	for (j=yy0; j<=yy1; j++){
		for (i=xx0; i<=xx1; i++){
			data[target]= PixelBlend8(data[target],c);
			target++;
		}
		target += wo;
	}

}
//******************************************************************************
void CFsGraph::boxFill16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 col)
{
	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	A_long w		= m_width;
	A_long wt		= m_widthTrue;
	A_long wo;
	A_long h		= m_height;
	A_long xx0	= x0;
	A_long xx1	= x1;
	A_long yy0	= y0;
	A_long yy1	= y1;
	A_long target,i,j;
	PF_Pixel16 c = col;
	//
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if (yy0>yy1) swapLong(&yy0,&yy1);
	
	if ( (xx0>=w) || (yy0>=h) || (xx1<0) || (yy1<0) ) return;

	if (xx0<0) xx0=0;
	if (yy0<0) yy0=0;
	if (xx1>=w) xx1=w-1;
	if (yy1>=h) yy1=h-1;

	target = xx0 + yy0 * wt;
	wo = wt -(xx1-xx0+1);
	for (j=yy0; j<=yy1; j++){
		for (i=xx0; i<=xx1; i++){
			data[target]= PixelBlend16(data[target],c);
			target++;
		}
		target += wo;
	}

}
//******************************************************************************
void CFsGraph::boxFill32(A_long x0,A_long y0,A_long x1,A_long y1,PF_PixelFloat col)
{
	PF_PixelFloat *data = (PF_PixelFloat *)m_data;
	A_long w		= m_width;
	A_long wt		= m_widthTrue;
	A_long wo;
	A_long h		= m_height;
	A_long xx0	= x0;
	A_long xx1	= x1;
	A_long yy0	= y0;
	A_long yy1	= y1;
	A_long target,i,j;
	PF_PixelFloat c = col;
	//
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if (yy0>yy1) swapLong(&yy0,&yy1);
	
	if ( (xx0>=w) || (yy0>=h) || (xx1<0) || (yy1<0) ) return;

	if (xx0<0) xx0=0;
	if (yy0<0) yy0=0;
	if (xx1>=w) xx1=w-1;
	if (yy1>=h) yy1=h-1;

	target = xx0 + yy0 * wt;
	wo = wt -(xx1-xx0+1);
	for (j=yy0; j<=yy1; j++){
		for (i=xx0; i<=xx1; i++){
			data[target]= PixelBlend32(data[target],c);
			target++;
		}
		target += wo;
	}

}//******************************************************************************
void CFsGraph::box8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel colu,PF_Pixel cold)
{
	A_long bak = lineHeiht;
	lineHeiht = 1;
	A_long hh = bak;
	A_long xx0 = x0;
	A_long yy0 = y0;
	A_long xx1 = x1;
	A_long yy1 = y1;
	A_long i;
	
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if (yy0>yy1) swapLong(&yy0,&yy1);

	if ( (xx1-xx0) < (hh*2) ){
		hh = (xx1-xx0) / 2;
	}

	if ( (yy1-yy0) < (hh*2) ){
		hh = (yy1-yy0) / 2;
	}
	hh = hh -1;

	if (hh>=0) {
		for (i=0; i<=hh;i++){ 
			//上
			XLine8(xx0,xx1,yy0,colu);
			//左
			YLine8(xx0,yy0+1,yy1-1,colu);

			//下
			XLine8(xx0,xx1,yy1,cold);
			//右
			YLine8(xx1,yy0+1,yy1-1,cold);
			xx0++;
			yy0++;
			xx1--;
			yy1--;
		}
	}
	lineHeiht = bak;

}
//******************************************************************************
void CFsGraph::box16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 colu,PF_Pixel16 cold)
{
	A_long bak = lineHeiht;
	lineHeiht = 1;
	A_long hh = bak;
	A_long xx0 = x0;
	A_long yy0 = y0;
	A_long xx1 = x1;
	A_long yy1 = y1;
	A_long i;
	
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if (yy0>yy1) swapLong(&yy0,&yy1);

	if ( (xx1-xx0) < (hh*2) ){
		hh = (xx1-xx0) / 2;
	}

	if ( (yy1-yy0) < (hh*2) ){
		hh = (yy1-yy0) / 2;
	}
	hh = hh -1;

	if (hh>=0) {
		for (i=0; i<=hh;i++){ 
			//上
			XLine16(xx0,xx1,yy0,colu);
			//左
			YLine16(xx0,yy0+1,yy1-1,colu);

			//下
			XLine16(xx0,xx1,yy1,cold);
			//右
			YLine16(xx1,yy0+1,yy1-1,cold);
			xx0++;
			yy0++;
			xx1--;
			yy1--;
		}
	}
	lineHeiht = bak;


}
//******************************************************************************
void CFsGraph::box32(A_long x0,A_long y0,A_long x1,A_long y1,PF_PixelFloat colu,PF_PixelFloat cold)
{
	A_long bak = lineHeiht;
	lineHeiht = 1;
	A_long hh = bak;
	A_long xx0 = x0;
	A_long yy0 = y0;
	A_long xx1 = x1;
	A_long yy1 = y1;
	A_long i;
	
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if (yy0>yy1) swapLong(&yy0,&yy1);

	if ( (xx1-xx0) < (hh*2) ){
		hh = (xx1-xx0) / 2;
	}

	if ( (yy1-yy0) < (hh*2) ){
		hh = (yy1-yy0) / 2;
	}
	hh = hh -1;

	if (hh>=0) {
		for (i=0; i<=hh;i++){ 
			//上
			XLine32(xx0,xx1,yy0,colu);
			//左
			YLine32(xx0,yy0+1,yy1-1,colu);

			//下
			XLine32(xx0,xx1,yy1,cold);
			//右
			YLine32(xx1,yy0+1,yy1-1,cold);
			xx0++;
			yy0++;
			xx1--;
			yy1--;
		}
	}
	lineHeiht = bak;


}
//******************************************************************************
