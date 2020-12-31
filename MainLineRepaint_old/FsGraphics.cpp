#include "FsGraphics.h"


inline void swapLong(long *s,long *d)
{
	long temp = *s;
	*d = *s;
	*s = temp;
}
//******************************************************************************
/*
	CFsGraph
	描画用のクラス
*/
//******************************************************************************
CFsGraph::CFsGraph(
		PF_EffectWorld *world, 
		PF_InData *in_data)
{
	//メンバの初期化
	m_Enabled = FALSE;
	m_in_data	= NULL;
	m_world		= NULL;
	m_data		= NULL;
	m_is16Bit	= FALSE;

	m_width			= 0;
	m_widthTrue	= 0;
	m_height		= 0;
	m_offsetWidth	= 0;
	
	lineHeiht		= 1;

	if ( (world==NULL)||(in_data==NULL) ) return;
	m_in_data = in_data;
	m_world		= world;
	m_data		= world->data;
	m_is16Bit	= PF_WORLD_IS_DEEP( m_world );
	m_width		= m_world->width;
	m_height	= m_world->height;
	if (m_is16Bit==TRUE){
		m_widthTrue	= m_world->rowbytes / sizeof(PF_Pixel16);
	}else{
		m_widthTrue	= m_world->rowbytes / sizeof(PF_Pixel);
	}
	m_offsetWidth	= m_widthTrue - m_width;

	m_Enabled	= TRUE;
}
//******************************************************************************
void CFsGraph::XLine8(long x0,long x1,long y,PF_Pixel col)
{
	PF_Pixel *data = (PF_Pixel *)m_data;
	long w	= m_width;
	long wt	= m_widthTrue;
	long h	= m_height;
	long xx0 = x0;
	long xx1 = x1;
	long yy  = y;
	long target,i,j;
	long hh = lineHeiht-1;
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
void CFsGraph::XLine16(long x0,long x1,long y,PF_Pixel16 col)
{
	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	long w	= m_width;
	long wt	= m_widthTrue;
	long h	= m_height;
	long xx0 = x0;
	long xx1 = x1;
	long yy  = y;
	long target,i,j;
	long hh = lineHeiht-1;
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
void CFsGraph::YLine8(long x,long y0,long y1,PF_Pixel col)
{
	PF_Pixel *data = (PF_Pixel *)m_data;
	long w		= m_width;
	long wt		= m_widthTrue;
	long h		= m_height;
	long xx		= x;
	long yy0	= y0;
	long yy1	= y1;
	long target,i,j;
	PF_Pixel c	= col;
	long hh = lineHeiht-1;
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
void CFsGraph::YLine16(long x,long y0,long y1,PF_Pixel16 col)
{
	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	long w		= m_width;
	long wt		= m_widthTrue;
	long h		= m_height;
	long xx		= x;
	long yy0	= y0;
	long yy1	= y1;
	long target,i,j;
	PF_Pixel16 c	= col;
	long hh = lineHeiht-1;
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
void CFsGraph::Line8(long x0,long y0,long x1,long y1,PF_Pixel col)
{
  long E;
  long dx,dy,sx,sy,i;

	PF_Pixel *data = (PF_Pixel *)m_data;
	long w = m_width;
	long h = m_height;
	PF_Pixel c	= col;
	
	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	long x = x0;
	long y = y0;
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
void CFsGraph::Line16(long x0,long y0,long x1,long y1,PF_Pixel16 col)
{
  long E;
  long dx,dy,sx,sy,i;

	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	long w = m_width;
	long h = m_height;
	PF_Pixel16 c	= col;
	
	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	long x = x0;
	long y = y0;
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
void CFsGraph::boxFill8(long x0,long y0,long x1,long y1,PF_Pixel col)
{
	PF_Pixel *data = (PF_Pixel *)m_data;
	long w		= m_width;
	long wt		= m_widthTrue;
	long wo;
	long h		= m_height;
	long xx0	= x0;
	long xx1	= x1;
	long yy0	= y0;
	long yy1	= y1;
	long target,i,j;
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
void CFsGraph::boxFill16(long x0,long y0,long x1,long y1,PF_Pixel16 col)
{
	PF_Pixel16 *data = (PF_Pixel16 *)m_data;
	long w		= m_width;
	long wt		= m_widthTrue;
	long wo;
	long h		= m_height;
	long xx0	= x0;
	long xx1	= x1;
	long yy0	= y0;
	long yy1	= y1;
	long target,i,j;
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
void CFsGraph::circle8(long x ,long y, long r,PF_Pixel col)
{
	//void circle(FsGrfPrm *prm, long cx ,long cy, long r)
	PF_Pixel c = col;
	long ix,iy,px,py,F;
	long xx = x;
	long yy = y;
	if (r<=0) return;
	ix = r;
	iy = 0;
	F = -2 * r + 3;
	while ( ix >= iy ) {
		px = xx + ix; py = yy + iy; setPixelBlend8(px,py,c);
		px = xx - ix; py = yy + iy; setPixelBlend8(px,py,c);
		px = xx + ix; py = yy - iy; setPixelBlend8(px,py,c);
		px = xx - ix; py = yy - iy; setPixelBlend8(px,py,c);
		px = xx + iy; py = yy + ix; setPixelBlend8(px,py,c);
		px = xx - iy; py = yy + ix; setPixelBlend8(px,py,c);
		px = xx + iy; py = yy - ix; setPixelBlend8(px,py,c);
		px = xx - iy; py = yy - ix; setPixelBlend8(px,py,c);
		if ( F >= 0 ) {
			ix--;
			F -= 4 * ix;
		}
		iy++;
		F += 4 * iy + 2;
	}

}
//******************************************************************************
void CFsGraph::circle16(long x ,long y, long r,PF_Pixel16 col)
{
	//void circle(FsGrfPrm *prm, long cx ,long cy, long r)
	PF_Pixel16 c = col;
	long ix,iy,px,py,F;
	long xx = x;
	long yy = y;
	if (r<=0) return;

	ix = r;
	iy = 0;
	F = -2 * r + 3;
	while ( ix >= iy ) {
		px = xx + ix; py = yy + iy; setPixelBlend16(px,py,c);
		px = xx - ix; py = yy + iy; setPixelBlend16(px,py,c);
		px = xx + ix; py = yy - iy; setPixelBlend16(px,py,c);
		px = xx - ix; py = yy - iy; setPixelBlend16(px,py,c);
		px = xx + iy; py = yy + ix; setPixelBlend16(px,py,c);
		px = xx - iy; py = yy + ix; setPixelBlend16(px,py,c);
		px = xx + iy; py = yy - ix; setPixelBlend16(px,py,c);
		px = xx - iy; py = yy - ix; setPixelBlend16(px,py,c);
		if ( F >= 0 ) {
			ix--;
			F -= 4 * ix;
		}
		iy++;
		F += 4 * iy + 2;
	}

}
//******************************************************************************
void CFsGraph::circleFill8(long x ,long y, long r,PF_Pixel col)
{
	PF_Pixel c = col;
	long bak = lineHeiht;
	lineHeiht =1;
	long xx = x;
	long yy = y;

	long ix,iy,F;
	long px0,px1,py0,py1;

	if (r>0) {

		ix = r;
		iy = 0;
		F = -2 * r + 3;
		while (  ix > iy  ) {
			px0	=xx + ix;
			px1	=xx - ix;
			py0 =yy + iy;
			XLine8(px0,px1,py0,c);

			if ( iy>0){
				px0	=xx + ix;
				px1	=xx - ix;
				py0 =yy - iy;
				XLine8(px0,px1,py0,c);
				}
			if ( F >= 0 ) {
				ix--;
				F -= 4 * ix;
			}
			iy++;
			F += 4 * iy + 2;
		}
		long k = iy;
		ix = r;
		iy = 0;
		F = -2 * r + 3;
		while (  ix > iy  ) {
			px0	=xx + iy;
			py0	=yy - ix;
			py1	=yy - k;
			YLine8(px0,py0,py1,c);
			py0	=yy + ix;
			py1	=yy + k;
			YLine8(px0,py0,py1,c);

			if (iy>0){
				px0	=xx - iy;
				py0	=yy - ix;
				py1	=yy - k;
				YLine8(px0,py0,py1,c);
				py0	=yy + ix;
				py1	=yy + k;
				YLine8(px0,py0,py1,c);
			}
			if ( F >= 0 ) {
				ix--;
				F -= 4 * ix;
			}
			iy++;
			F += 4 * iy + 2;
		}

	}
	lineHeiht =bak;
}
//******************************************************************************
void CFsGraph::circleFill16(long x ,long y, long r,PF_Pixel16 col)
{
	PF_Pixel16 c = col;
	long bak = lineHeiht;
	lineHeiht =1;
	long xx = x;
	long yy = y;

	long ix,iy,F;
	long px0,px1,py0,py1;

	if (r>0){

		ix = r;
		iy = 0;
		F = -2 * r + 3;
		while (  ix > iy  ) {
			px0	=xx + ix;
			px1	=xx - ix;
			py0 =yy + iy;
			XLine16(px0,px1,py0,c);
			if (iy>0){
				px0	=xx + ix;
				px1	=xx - ix;
				py0 =yy - iy;
				XLine16(px0,px1,py0,c);
			}
			if ( F >= 0 ) {
				ix--;
				F -= 4 * ix;
			}
			iy++;
			F += 4 * iy + 2;
		}
		long k = iy;
		ix = r;
		iy = 0;
		F = -2 * r + 3;
		while (  ix > iy  ) {
			px0	=xx + iy;
			py0	=yy - ix;
			py1	=yy - k;
			YLine16(px0,py0,py1,c);
			py0	=yy + ix;
			py1	=yy + k;
			YLine16(px0,py0,py1,c);
			if (iy>0) {
				px0	=xx - iy;
				py0	=yy - ix;
				py1	=yy - k;
				YLine16(px0,py0,py1,c);
				py0	=yy + ix;
				py1	=yy + k;
				YLine16(px0,py0,py1,c);
			}
			if ( F >= 0 ) {
				ix--;
				F -= 4 * ix;
			}
			iy++;
			F += 4 * iy + 2;
		}

	}
	lineHeiht = bak;
}
//******************************************************************************
void CFsGraph::box8(long x0,long y0,long x1,long y1,PF_Pixel colu,PF_Pixel cold)
{
	long bak = lineHeiht;
	lineHeiht = 1;
	long hh = bak;
	long xx0 = x0;
	long yy0 = y0;
	long xx1 = x1;
	long yy1 = y1;
	long i;
	
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
void CFsGraph::box16(long x0,long y0,long x1,long y1,PF_Pixel16 colu,PF_Pixel16 cold)
{
	long bak = lineHeiht;
	lineHeiht = 1;
	long hh = bak;
	long xx0 = x0;
	long yy0 = y0;
	long xx1 = x1;
	long yy1 = y1;
	long i;
	
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
