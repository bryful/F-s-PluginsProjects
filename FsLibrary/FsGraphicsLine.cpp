#include "FsGraphics.h"

//******************************************************************************
void CFsGraph::XLine8(A_long x0,A_long x1,A_long y,PF_Pixel col)
{
	PF_Pixel *data = (PF_Pixel *)m_data;
	A_long w	= m_width;
	A_long wt	= m_widthTrue;
	A_long h	= m_height;
	A_long xx0 = x0;
	A_long xx1 = x1;
	A_long yy  = y;
	A_long target,i,j;
	A_long hh = lineHeiht-1;
	if (hh<0) return;
	PF_Pixel c	= col;

	//クリッピング
	if ( ((yy+hh)<0)||((yy-hh)>=h) ) return;
	
	//起点と終点の確認
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if ( (xx0>=w) || (xx1<0) ) return;
	if (xx0<0) xx0=0;
	if (xx1>=w) xx1=w-1;
	for (j=0;j<=hh;j++) {
		yy = y+j;
		if ( (yy>=0)&&(yy<h) ) {
			target = xx0 + yy * wt;
			for (i=xx0;i<=xx1;i++) {
				data[target]= PixelBlend8(data[target],c);
				target++;
			}
		}
	}
}
//******************************************************************************
void CFsGraph::XLine16(A_long x0,A_long x1,A_long y,PF_Pixel16 col)
{
	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	A_long w	= m_width;
	A_long wt	= m_widthTrue;
	A_long h	= m_height;
	A_long xx0 = x0;
	A_long xx1 = x1;
	A_long yy  = y;
	A_long target,i,j;
	A_long hh = lineHeiht-1;
	if (hh<0) return;
	PF_Pixel16 c	= col;

	//クリッピング
	if ( ((yy+hh)<0)||((yy-hh)>=h) ) return;
	
	//起点と終点の確認
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if ( (xx0>=w) || (xx1<0) ) return;
	if (xx0<0) xx0=0;
	if (xx1>=w) xx1=w-1;
	for (j=0;j<=hh;j++) {
		yy = y+j;
		if ( (yy>=0)&&(yy<h) ) {
			target = xx0 + yy * wt;
			for (i=xx0;i<=xx1;i++) {
				data[target]= PixelBlend16(data[target],c);
				target++;
			}
		}
	}
}
//******************************************************************************
void CFsGraph::XLine32(A_long x0,A_long x1,A_long y,PF_PixelFloat col)
{
	PF_PixelFloat *data = (PF_PixelFloat *)m_data;
	A_long w	= m_width;
	A_long wt	= m_widthTrue;
	A_long h	= m_height;
	A_long xx0 = x0;
	A_long xx1 = x1;
	A_long yy  = y;
	A_long target,i,j;
	A_long hh = lineHeiht-1;
	if (hh<0) return;
	PF_PixelFloat c	= col;

	//クリッピング
	if ( ((yy+hh)<0)||((yy-hh)>=h) ) return;
	
	//起点と終点の確認
	if (xx0>xx1) swapLong(&xx0,&xx1);
	if ( (xx0>=w) || (xx1<0) ) return;
	if (xx0<0) xx0=0;
	if (xx1>=w) xx1=w-1;
	for (j=0;j<=hh;j++) {
		yy = y+j;
		if ( (yy>=0)&&(yy<h) ) {
			target = xx0 + yy * wt;
			for (i=xx0;i<=xx1;i++) {
				data[target]= PixelBlend32(data[target],c);
				target++;
			}
		}
	}
}
//******************************************************************************
void CFsGraph::YLine8(A_long x,A_long y0,A_long y1,PF_Pixel col)
{
	PF_Pixel *data = (PF_Pixel *)m_data;
	A_long w		= m_width;
	A_long wt		= m_widthTrue;
	A_long h		= m_height;
	A_long xx		= x;
	A_long yy0	= y0;
	A_long yy1	= y1;
	A_long target,i,j;
	PF_Pixel c	= col;
	A_long hh = lineHeiht-1;
	if (hh<0) return;

	//クリッピング
	if ( ((xx+hh)<0)||((xx-hh)>=w) ) return;
	
	//起点と終点の確認
	if (yy0>yy1)  swapLong(&yy0,&yy1);
	if ( (yy0>=h) || (yy1<0) ) return;
	if (yy0<0) yy0=0;
	if (yy1>=h) yy1=h-1;

	for (j=0;j<=hh;j++){
		xx = x+j;
		if ( (xx>=0)&&(xx<w) ) {
			target = xx + yy0 * wt;
			for (i=yy0;i<=yy1;i++) {
				data[target]= PixelBlend8(data[target],c);
				target+=wt;
			}
		}
	}
}
//******************************************************************************
void CFsGraph::YLine16(A_long x,A_long y0,A_long y1,PF_Pixel16 col)
{
	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	A_long w		= m_width;
	A_long wt		= m_widthTrue;
	A_long h		= m_height;
	A_long xx		= x;
	A_long yy0	= y0;
	A_long yy1	= y1;
	A_long target,i,j;
	PF_Pixel16 c	= col;
	A_long hh = lineHeiht-1;
	if (hh<0) return;

	//クリッピング
	if ( ((xx+hh)<0)||((xx-hh)>=w) ) return;
	
	//起点と終点の確認
	if (yy0>yy1)  swapLong(&yy0,&yy1);
	if ( (yy0>=h) || (yy1<0) ) return;
	if (yy0<0) yy0=0;
	if (yy1>=h) yy1=h-1;

	for (j=0;j<=hh;j++){
		xx = x+j;
		if ( (xx>=0)&&(xx<w) ) {
			target = xx + yy0 * wt;
			for (i=yy0;i<=yy1;i++) {
				data[target]= PixelBlend16(data[target],c);
				target+=wt;
			}
		}
	}
}
//******************************************************************************
void CFsGraph::YLine32(A_long x,A_long y0,A_long y1,PF_PixelFloat col)
{
	PF_PixelFloat *data = (PF_PixelFloat *)m_data;
	A_long w		= m_width;
	A_long wt		= m_widthTrue;
	A_long h		= m_height;
	A_long xx		= x;
	A_long yy0	= y0;
	A_long yy1	= y1;
	A_long target,i,j;
	PF_PixelFloat c	= col;
	A_long hh = lineHeiht-1;
	if (hh<0) return;

	//クリッピング
	if ( ((xx+hh)<0)||((xx-hh)>=w) ) return;
	
	//起点と終点の確認
	if (yy0>yy1)  swapLong(&yy0,&yy1);
	if ( (yy0>=h) || (yy1<0) ) return;
	if (yy0<0) yy0=0;
	if (yy1>=h) yy1=h-1;

	for (j=0;j<=hh;j++){
		xx = x+j;
		if ( (xx>=0)&&(xx<w) ) {
			target = xx + yy0 * wt;
			for (i=yy0;i<=yy1;i++) {
				data[target]= PixelBlend32(data[target],c);
				target+=wt;
			}
		}
	}
}//******************************************************************************
void CFsGraph::Line8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel col)
{
  A_long E;
  A_long dx,dy,sx,sy,i;

	PF_Pixel *data = (PF_Pixel *)m_data;
	A_long w = m_width;
	A_long h = m_height;
	PF_Pixel c	= col;
	
	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	A_long x = x0;
	A_long y = y0;
	/* 傾きが1以下の場合 */
	if( dx >= dy ) {
		E = -dx;
		for( i = 0; i <= dx; i++ ) {
			setPixelBlend8(x,y,c);
			x += sx;
			E += 2 * dy;
			if( E >= 0 ) {
				y += sy;
				E -= 2 * dx;
			}
		}
	/* 傾きが1より大きい場合 */
	} else {
		E = -dy;
	  for( i = 0; i <= dy; i++ ) {
			setPixelBlend8(x,y,c);
			y += sy;
			E += 2 * dx;
			if( E >= 0 ) {
				x += sx;
				E -= 2 * dy;
			}
		}
	}
}

//******************************************************************************
void CFsGraph::Line16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 col)
{
  A_long E;
  A_long dx,dy,sx,sy,i;

	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	A_long w = m_width;
	A_long h = m_height;
	PF_Pixel16 c	= col;
	
	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	A_long x = x0;
	A_long y = y0;
	/* 傾きが1以下の場合 */
	if( dx >= dy ) {
		E = -dx;
		for( i = 0; i <= dx; i++ ) {
			setPixelBlend16(x,y,c);
			x += sx;
			E += 2 * dy;
			if( E >= 0 ) {
				y += sy;
				E -= 2 * dx;
			}
		}
	/* 傾きが1より大きい場合 */
	} else {
		E = -dy;
	  for( i = 0; i <= dy; i++ ) {
			setPixelBlend16(x,y,c);
			y += sy;
			E += 2 * dx;
			if( E >= 0 ) {
				x += sx;
				E -= 2 * dy;
			}
		}
	}
}
//******************************************************************************
void CFsGraph::Line32(A_long x0,A_long y0,A_long x1,A_long y1,PF_PixelFloat col)
{
  A_long E;
  A_long dx,dy,sx,sy,i;

	PF_PixelFloat *data = (PF_PixelFloat *)m_data;
	A_long w = m_width;
	A_long h = m_height;
	PF_PixelFloat c	= col;
	
	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	A_long x = x0;
	A_long y = y0;
	/* 傾きが1以下の場合 */
	if( dx >= dy ) {
		E = -dx;
		for( i = 0; i <= dx; i++ ) {
			setPixelBlend32(x,y,c);
			x += sx;
			E += 2 * dy;
			if( E >= 0 ) {
				y += sy;
				E -= 2 * dx;
			}
		}
	/* 傾きが1より大きい場合 */
	} else {
		E = -dy;
	  for( i = 0; i <= dy; i++ ) {
			setPixelBlend32(x,y,c);
			y += sy;
			E += 2 * dx;
			if( E >= 0 ) {
				x += sx;
				E -= 2 * dy;
			}
		}
	}
}
