#include "FsGraphics.h"

//******************************************************************************
void CFsGraph::circle8(A_long x ,A_long y, A_long r,PF_Pixel col)
{
	//void circle(FsGrfPrm *prm, A_long cx ,A_long cy, A_long r)
	PF_Pixel c = col;
	A_long ix,iy,px,py,F;
	A_long xx = x;
	A_long yy = y;
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
void CFsGraph::circle16(A_long x ,A_long y, A_long r,PF_Pixel16 col)
{
	//void circle(FsGrfPrm *prm, A_long cx ,A_long cy, A_long r)
	PF_Pixel16 c = col;
	A_long ix,iy,px,py,F;
	A_long xx = x;
	A_long yy = y;
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
void CFsGraph::circle32(A_long x ,A_long y, A_long r,PF_PixelFloat col)
{
	//void circle(FsGrfPrm *prm, A_long cx ,A_long cy, A_long r)
	PF_PixelFloat c = col;
	A_long ix,iy,px,py,F;
	A_long xx = x;
	A_long yy = y;
	if (r<=0) return;

	ix = r;
	iy = 0;
	F = -2 * r + 3;
	while ( ix >= iy ) {
		px = xx + ix; py = yy + iy; setPixelBlend32(px,py,c);
		px = xx - ix; py = yy + iy; setPixelBlend32(px,py,c);
		px = xx + ix; py = yy - iy; setPixelBlend32(px,py,c);
		px = xx - ix; py = yy - iy; setPixelBlend32(px,py,c);
		px = xx + iy; py = yy + ix; setPixelBlend32(px,py,c);
		px = xx - iy; py = yy + ix; setPixelBlend32(px,py,c);
		px = xx + iy; py = yy - ix; setPixelBlend32(px,py,c);
		px = xx - iy; py = yy - ix; setPixelBlend32(px,py,c);
		if ( F >= 0 ) {
			ix--;
			F -= 4 * ix;
		}
		iy++;
		F += 4 * iy + 2;
	}

}//******************************************************************************
void CFsGraph::circleFill8(A_long x ,A_long y, A_long r,PF_Pixel col)
{
	PF_Pixel c = col;
	A_long bak = lineHeiht;
	lineHeiht =1;
	A_long xx = x;
	A_long yy = y;

	A_long ix,iy,F;
	A_long px0,px1,py0,py1;

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
		A_long k = iy;
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
void CFsGraph::circleFill16(A_long x ,A_long y, A_long r,PF_Pixel16 col)
{
	PF_Pixel16 c = col;
	A_long bak = lineHeiht;
	lineHeiht =1;
	A_long xx = x;
	A_long yy = y;

	A_long ix,iy,F;
	A_long px0,px1,py0,py1;

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
		A_long k = iy;
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
void CFsGraph::circleFill32(A_long x ,A_long y, A_long r,PF_PixelFloat col)
{
	PF_PixelFloat c = col;
	A_long bak = lineHeiht;
	lineHeiht =1;
	A_long xx = x;
	A_long yy = y;

	A_long ix,iy,F;
	A_long px0,px1,py0,py1;

	if (r>0){

		ix = r;
		iy = 0;
		F = -2 * r + 3;
		while (  ix > iy  ) {
			px0	=xx + ix;
			px1	=xx - ix;
			py0 =yy + iy;
			XLine32(px0,px1,py0,c);
			if (iy>0){
				px0	=xx + ix;
				px1	=xx - ix;
				py0 =yy - iy;
				XLine32(px0,px1,py0,c);
			}
			if ( F >= 0 ) {
				ix--;
				F -= 4 * ix;
			}
			iy++;
			F += 4 * iy + 2;
		}
		A_long k = iy;
		ix = r;
		iy = 0;
		F = -2 * r + 3;
		while (  ix > iy  ) {
			px0	=xx + iy;
			py0	=yy - ix;
			py1	=yy - k;
			YLine32(px0,py0,py1,c);
			py0	=yy + ix;
			py1	=yy + k;
			YLine32(px0,py0,py1,c);
			if (iy>0) {
				px0	=xx - iy;
				py0	=yy - ix;
				py1	=yy - k;
				YLine32(px0,py0,py1,c);
				py0	=yy + ix;
				py1	=yy + k;
				YLine32(px0,py0,py1,c);
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
