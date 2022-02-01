#pragma once
#ifndef FSSCREENSHAKE_H
#define FSSCREENSHAKE_H

#include "Fs.h"


#include "FsUtils.h"

//
typedef struct getPixelPrm{
	A_long	widthTrue;
	A_long	width;
	A_long	height;
	PF_PixelPtr data;
} getPixelPrm;

typedef struct channelShiftPrm{
	PF_FixedPoint	a;
	PF_FixedPoint	b;
	PF_FixedPoint	g;
	PF_FixedPoint	r;
} channelShiftPrm;

PF_Pixel (*getPixel8_sub)(getPixelPrm *prm,A_long x,A_long y);
PF_Pixel16 (*getPixel16_sub)(getPixelPrm *prm,A_long x,A_long y);
PF_PixelFloat (*getPixel32_sub)(getPixelPrm *prm,A_long x,A_long y);

PF_Pixel (*getPixel8_CS_sub)(getPixelPrm *prm,channelShiftPrm *sft);
PF_Pixel16 (*getPixel16_CS_sub)(getPixelPrm *prm,channelShiftPrm *sft);
PF_PixelFloat (*getPixel32_CS_sub)(getPixelPrm *prm,channelShiftPrm *sft);

//-------------------------------------------------------------------------------------------------
//縁は透明
inline PF_Pixel getPixel8_none(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel blank ={0,0,0,0};
	PF_Pixel *data;
	data = (PF_Pixel *)prm->data;
	if (x<0) {
		return blank;
	}else if (x>=prm->width){
		return blank;
	}else if (y<0) {
		return blank;
	}else if (y>=prm->height) {
		return blank;
	}
	return data[x + y * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel16 getPixel16_none(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel16 blank ={0,0,0,0};
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)prm->data;
	if (x<0) {
		return blank;
	}else if (x>=prm->width){
		return blank;
	}else if (y<0) {
		return blank;
	}else if (y>=prm->height) {
		return blank;
	}
	return data[x + y * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat getPixel32_none(getPixelPrm *prm,A_long x,A_long y)
{
	PF_PixelFloat blank ={0,0,0,0};
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)prm->data;
	if (x<0) {
		return blank;
	}else if (x>=prm->width){
		return blank;
	}else if (y<0) {
		return blank;
	}else if (y>=prm->height) {
		return blank;
	}
	return data[x + y * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//縁は塗りつぶし
inline PF_Pixel getPixel8_fill(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel *data;
	data = (PF_Pixel *)prm->data;
	A_long xx,yy;
	if (x<0) { xx=0;}
	else if (x>=prm->width) { xx = prm->width -1;}
	else { xx = x; }
	
	if (y<0) { yy=0;}
	else if (y>=prm->height) { yy = prm->height -1;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
///-------------------------------------------------------------------------------------------------
//縁は塗りつぶし
inline PF_Pixel16 getPixel16_fill(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)prm->data;
	A_long xx,yy;
	if (x<0) { xx=0;}
	else if (x>=prm->width) { xx = prm->width -1;}
	else { xx = x; }
	
	if (y<0) { yy=0;}
	else if (y>=prm->height) { yy = prm->height -1;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
///-------------------------------------------------------------------------------------------------
//縁は塗りつぶし
inline PF_PixelFloat getPixel32_fill(getPixelPrm *prm,A_long x,A_long y)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)prm->data;
	A_long xx,yy;
	if (x<0) { xx=0;}
	else if (x>=prm->width) { xx = prm->width -1;}
	else { xx = x; }
	
	if (y<0) { yy=0;}
	else if (y>=prm->height) { yy = prm->height -1;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//繰り返し
inline PF_Pixel getPixel8_rep(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel *data;
	data = (PF_Pixel *)prm->data;
	A_long xx,yy;
	
	if (x<0) { xx = x + prm->width;}
	else if (x>=prm->width) { xx = x - prm->width;}
	else { xx = x; }
	
	if (y<0) { yy = y + prm->height;}
	else if (y>=prm->height) { yy = y - prm->height;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//繰り返し
inline PF_Pixel16 getPixel16_rep(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)prm->data;
	A_long xx,yy;
	
	if (x<0) { xx = x + prm->width;}
	else if (x>=prm->width) { xx = x - prm->width;}
	else { xx = x; }
	
	if (y<0) { yy = y + prm->height;}
	else if (y>=prm->height) { yy = y - prm->height;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//繰り返し
inline PF_PixelFloat getPixel32_rep(getPixelPrm *prm,A_long x,A_long y)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)prm->data;
	A_long xx,yy;
	
	if (x<0) { xx = x + prm->width;}
	else if (x>=prm->width) { xx = x - prm->width;}
	else { xx = x; }
	
	if (y<0) { yy = y + prm->height;}
	else if (y>=prm->height) { yy = y - prm->height;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//繰り返し
inline PF_Pixel getPixel8_mirror(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel *data;
	data = (PF_Pixel *)prm->data;
	A_long xx,yy;
	
	if (x<0) { xx = x * -1;}
	else if (x>=prm->width) { xx = prm->width - (x - prm->width)-1;xx  %= prm->width;}
	else { xx = x ; }
	

	if (y<0) { yy = y * -1;}
	else if (y>=prm->height) { yy = prm->height - (y - prm->height)-1; yy %= prm->height;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//繰り返し
inline PF_Pixel16 getPixel16_mirror(getPixelPrm *prm,A_long x,A_long y)
{
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)prm->data;
	A_long xx,yy;
	
	if (x<0) { xx = x * -1;}
	else if (x>=prm->width) { xx = prm->width - (x - prm->width)-1;xx  %= prm->width;}
	else { xx = x ; }
	

	if (y<0) { yy = y * -1;}
	else if (y>=prm->height) { yy = prm->height - (y - prm->height)-1; yy %= prm->height;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
//繰り返し
inline PF_PixelFloat getPixel32_mirror(getPixelPrm *prm,A_long x,A_long y)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)prm->data;
	A_long xx,yy;
	
	if (x<0) { xx = x * -1;}
	else if (x>=prm->width) { xx = prm->width - (x - prm->width)-1;xx  %= prm->width;}
	else { xx = x ; }
	

	if (y<0) { yy = y * -1;}
	else if (y>=prm->height) { yy = prm->height - (y - prm->height)-1; yy %= prm->height;}
	else { yy = y; }

	return data[xx + yy * prm->widthTrue];
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel getPixel8(getPixelPrm *prm,PF_Fixed x,PF_Fixed y)
{
	PF_Pixel p[4];
	PF_Pixel r;
	A_long xx,yy,dx1,dy1,dx2,dy2,temp;
	xx = x >>16;
	yy = y >>16;
	dx1 = (x & 0xFFFF) >>8;
	dy1 = (y & 0xFFFF) >>8;
	dx2 = (1L<<8) - dx1;
	dy2 = (1L<<8) - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getPixel8_sub(prm,xx,yy);
	}

	p[0] = getPixel8_sub(prm,xx  ,yy  );
	p[1] = getPixel8_sub(prm,xx+1,yy  );
	p[2] = getPixel8_sub(prm,xx  ,yy+1);
	p[3] = getPixel8_sub(prm,xx+1,yy+1);
	
	temp = ( (p[0].alpha * dx2 * dy2) + (p[1].alpha * dx1 * dy2) + (p[2].alpha * dx2 * dy1) + (p[3].alpha * dx1 * dy1) ) >>(16);
	if ( temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
	r.alpha = (unsigned char)temp;
	temp = ( (p[0].red * dx2 * dy2) + (p[1].red * dx1 * dy2) + (p[2].red * dx2 * dy1) + (p[3].red * dx1 * dy1) ) >>(16);
	if ( temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
	r.red = (unsigned char)temp;
	temp = ( (p[0].green * dx2 * dy2) + (p[1].green * dx1 * dy2) + (p[2].green * dx2 * dy1) + (p[3].green * dx1 * dy1) ) >>(16);
	if ( temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
	r.green = (unsigned char)temp;
	temp = ( (p[0].blue * dx2 * dy2) + (p[1].blue * dx1 * dy2) + (p[2].blue * dx2 * dy1) + (p[3].blue * dx1 * dy1) ) >>(16);
	if ( temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
	r.blue = (unsigned char)temp;

	return r;
}


//-------------------------------------------------------------------------------------------------
inline PF_Pixel16 getPixel16(getPixelPrm *prm,PF_Fixed x,PF_Fixed y)
{
	PF_Pixel16 p[4];
	PF_Pixel16 r;
	A_long xx,yy,dx1,dy1,dx2,dy2,temp;
	xx = x >>16;
	yy = y >>16;
	dx1 = (x & 0xFFFF) >>8;
	dy1 = (y & 0xFFFF) >>8;
	dx2 = (1L<<8) - dx1;
	dy2 = (1L<<8) - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		return getPixel16_sub(prm,xx,yy);
	}

	p[0] = getPixel16_sub(prm,xx  ,yy  );
	p[1] = getPixel16_sub(prm,xx+1,yy  );
	p[2] = getPixel16_sub(prm,xx  ,yy+1);
	p[3] = getPixel16_sub(prm,xx+1,yy+1);
	
	temp = (p[0].alpha * dx2 * dy2 >>16 ) + (p[1].alpha * dx1 * dy2 >>16 ) + (p[2].alpha * dx2 * dy1 >>16) + (p[3].alpha * dx1 * dy1>>16 );
	if ( temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
	r.alpha = (unsigned short)temp;
	temp = (p[0].red * dx2 * dy2 >>16) + (p[1].red * dx1 * dy2 >>16) + (p[2].red * dx2 * dy1>>16) + (p[3].red * dx1 * dy1>>16);
	if ( temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
	r.red = (unsigned short)temp;
	temp = (p[0].green * dx2 * dy2 >>16) + (p[1].green * dx1 * dy2>>16) + (p[2].green * dx2 * dy1>>16) + (p[3].green * dx1 * dy1>>16);
	if ( temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
	r.green = (unsigned short)temp;
	temp = (p[0].blue * dx2 * dy2 >> 16) + (p[1].blue * dx1 * dy2 >>16 ) + (p[2].blue * dx2 * dy1 >>16) + (p[3].blue * dx1 * dy1 >> 16);
	if ( temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
	r.blue = (unsigned short)temp;

	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat getPixel32(getPixelPrm *prm,PF_Fixed x,PF_Fixed y)
{
	PF_PixelFloat p[4];
	PF_PixelFloat r;
	PF_FpShort temp;
	
	A_long xx = x >>16;
	A_long yy = y >>16;
	
	PF_FpShort dx1 = (PF_FpShort)((PF_FpShort)(x & 0xFFFF)/65536.0);
	PF_FpShort dy1 = (PF_FpShort)((PF_FpShort)(y & 0xFFFF)/65536.0);
	PF_FpShort dx2 = (PF_FpShort)(1.0 - dx1);
	PF_FpShort dy2 = (PF_FpShort)(1.0 - dy1);

	if ( (dx1==0)&&(dy1==0) ) {
		return getPixel32_sub(prm,xx,yy);
	}
	PF_FpShort b[4];
	b[0] = dx2 * dy2;
	b[1] = dx1 * dy2;
	b[2] = dx2 * dy1;
	b[3] = dx1 * dy1;

	p[0] = getPixel32_sub(prm,xx  ,yy  );
	p[1] = getPixel32_sub(prm,xx+1,yy  );
	p[2] = getPixel32_sub(prm,xx  ,yy+1);
	p[3] = getPixel32_sub(prm,xx+1,yy+1);

	temp = (p[0].alpha * b[0]) + (p[1].alpha * b[1]) + (p[2].alpha * b[2]) + (p[3].alpha * b[3]);
	r.alpha = temp;
	
	temp = (p[0].red * b[0]) + (p[1].red * b[1]) + (p[2].red * b[2]) + (p[3].red * b[3]);
	r.red = temp;
	
	temp = (p[0].green * b[0]) + (p[1].green * b[1]) + (p[2].green * b[2]) + (p[3].green * b[3]);
	r.green = temp;
	
	temp = (p[0].blue * b[0]) + (p[1].blue * b[1]) + (p[2].blue * b[2]) + (p[3].blue * b[3]);
	r.blue = temp;

	return r;
}
//********************************************************************
//-------------------------------------------------------------------------------------------------
inline PF_Pixel getPixel8_CS(getPixelPrm *prm,channelShiftPrm *sht)
{
	PF_Pixel p[4];
	PF_Pixel r;
	A_long xx,yy;

	PF_FpLong dx1,dy1,dx2,dy2;
	PF_Fixed x,y;
	//alpha---------------------------------------
	x = sht->a.x; 
	y = sht->a.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.alpha = getPixel8_sub(prm,xx,yy).alpha;
	}else{
		p[0] = getPixel8_sub(prm,xx  ,yy  );
		p[1] = getPixel8_sub(prm,xx+1,yy  );
		p[2] = getPixel8_sub(prm,xx  ,yy+1);
		p[3] = getPixel8_sub(prm,xx+1,yy+1);
	
		r.alpha = RoundByteFpLong( (p[0].alpha * dx2 * dy2) + (p[1].alpha * dx1 * dy2) + (p[2].alpha * dx2 * dy1) + (p[3].alpha * dx1 * dy1) );
	}
	//red---------------------------------------
	x = sht->r.x; 
	y = sht->r.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.red = getPixel8_sub(prm,xx,yy).red;
	}else{
		p[0] = getPixel8_sub(prm,xx  ,yy  );
		p[1] = getPixel8_sub(prm,xx+1,yy  );
		p[2] = getPixel8_sub(prm,xx  ,yy+1);
		p[3] = getPixel8_sub(prm,xx+1,yy+1);
	
		r.red =RoundByteFpLong( (p[0].red * dx2 * dy2) + (p[1].red * dx1 * dy2) + (p[2].red * dx2 * dy1) + (p[3].red * dx1 * dy1) );
	}
	//green---------------------------------------
	x = sht->g.x; 
	y = sht->g.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.green = getPixel8_sub(prm,xx,yy).green;
	}else{
		p[0] = getPixel8_sub(prm,xx  ,yy  );
		p[1] = getPixel8_sub(prm,xx+1,yy  );
		p[2] = getPixel8_sub(prm,xx  ,yy+1);
		p[3] = getPixel8_sub(prm,xx+1,yy+1);
	
		r.green =RoundByteFpLong( (p[0].green * dx2 * dy2) + (p[1].green * dx1 * dy2) + (p[2].green * dx2 * dy1) + (p[3].green * dx1 * dy1) );
	}
	//blue---------------------------------------
	x = sht->b.x; 
	y = sht->b.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.blue = getPixel8_sub(prm,xx,yy).blue;
	}else{
		p[0] = getPixel8_sub(prm,xx  ,yy  );
		p[1] = getPixel8_sub(prm,xx+1,yy  );
		p[2] = getPixel8_sub(prm,xx  ,yy+1);
		p[3] = getPixel8_sub(prm,xx+1,yy+1);
	
		r.blue =RoundByteFpLong( (p[0].blue * dx2 * dy2) + (p[1].blue * dx1 * dy2) + (p[2].blue * dx2 * dy1) + (p[3].blue * dx1 * dy1) );
	}


	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel16 getPixel16_CS(getPixelPrm *prm,channelShiftPrm *sht)
{
	PF_Pixel16 p[4];
	PF_Pixel16 r;
	A_long xx,yy;

	PF_FpLong dx1,dy1,dx2,dy2;
	PF_Fixed x,y;
	//alpha---------------------------------------
	x = sht->a.x; 
	y = sht->a.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.alpha = getPixel16_sub(prm,xx,yy).alpha;
	}else{
		p[0] = getPixel16_sub(prm,xx  ,yy  );
		p[1] = getPixel16_sub(prm,xx+1,yy  );
		p[2] = getPixel16_sub(prm,xx  ,yy+1);
		p[3] = getPixel16_sub(prm,xx+1,yy+1);
	
		r.alpha = RoundShortFpLong( (p[0].alpha * dx2 * dy2) + (p[1].alpha * dx1 * dy2) + (p[2].alpha * dx2 * dy1) + (p[3].alpha * dx1 * dy1) );
	}
	//red---------------------------------------
	x = sht->r.x; 
	y = sht->r.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.red = getPixel16_sub(prm,xx,yy).red;
	}else{
		p[0] = getPixel16_sub(prm,xx  ,yy  );
		p[1] = getPixel16_sub(prm,xx+1,yy  );
		p[2] = getPixel16_sub(prm,xx  ,yy+1);
		p[3] = getPixel16_sub(prm,xx+1,yy+1);
	
		r.red =RoundShortFpLong( (p[0].red * dx2 * dy2) + (p[1].red * dx1 * dy2) + (p[2].red * dx2 * dy1) + (p[3].red * dx1 * dy1) );
	}
	//green---------------------------------------
	x = sht->g.x; 
	y = sht->g.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.green = getPixel16_sub(prm,xx,yy).green;
	}else{
		p[0] = getPixel16_sub(prm,xx  ,yy  );
		p[1] = getPixel16_sub(prm,xx+1,yy  );
		p[2] = getPixel16_sub(prm,xx  ,yy+1);
		p[3] = getPixel16_sub(prm,xx+1,yy+1);
	
		r.green =RoundShortFpLong( (p[0].green * dx2 * dy2) + (p[1].green * dx1 * dy2) + (p[2].green * dx2 * dy1) + (p[3].green * dx1 * dy1) );
	}
	//blue---------------------------------------
	x = sht->b.x; 
	y = sht->b.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.blue = getPixel16_sub(prm,xx,yy).blue;
	}else{
		p[0] = getPixel16_sub(prm,xx  ,yy  );
		p[1] = getPixel16_sub(prm,xx+1,yy  );
		p[2] = getPixel16_sub(prm,xx  ,yy+1);
		p[3] = getPixel16_sub(prm,xx+1,yy+1);
	
		r.blue =RoundShortFpLong( (p[0].blue * dx2 * dy2) + (p[1].blue * dx1 * dy2) + (p[2].blue * dx2 * dy1) + (p[3].blue * dx1 * dy1) );
	}


	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat getPixel32_CS(getPixelPrm *prm,channelShiftPrm *sht)
{
	PF_PixelFloat p[4];
	PF_PixelFloat r;
	A_long xx,yy;

	PF_FpLong dx1,dy1,dx2,dy2;
	PF_Fixed x,y;
	//alpha---------------------------------------
	x = sht->a.x; 
	y = sht->a.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536.0;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536.0;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.alpha = getPixel32_sub(prm,xx,yy).alpha;
	}else{
		p[0] = getPixel32_sub(prm,xx  ,yy  );
		p[1] = getPixel32_sub(prm,xx+1,yy  );
		p[2] = getPixel32_sub(prm,xx  ,yy+1);
		p[3] = getPixel32_sub(prm,xx+1,yy+1);
	
		r.alpha = RoundFpShortDouble( (p[0].alpha * dx2 * dy2) + (p[1].alpha * dx1 * dy2) + (p[2].alpha * dx2 * dy1) + (p[3].alpha * dx1 * dy1) );
	}
	//red---------------------------------------
	x = sht->r.x; 
	y = sht->r.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.red = getPixel32_sub(prm,xx,yy).red;
	}else{
		p[0] = getPixel32_sub(prm,xx  ,yy  );
		p[1] = getPixel32_sub(prm,xx+1,yy  );
		p[2] = getPixel32_sub(prm,xx  ,yy+1);
		p[3] = getPixel32_sub(prm,xx+1,yy+1);
	
		r.red =RoundFpShortDouble( (p[0].red * dx2 * dy2) + (p[1].red * dx1 * dy2) + (p[2].red * dx2 * dy1) + (p[3].red * dx1 * dy1) );
	}
	//green---------------------------------------
	x = sht->g.x; 
	y = sht->g.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.green = getPixel32_sub(prm,xx,yy).green;
	}else{
		p[0] = getPixel32_sub(prm,xx  ,yy  );
		p[1] = getPixel32_sub(prm,xx+1,yy  );
		p[2] = getPixel32_sub(prm,xx  ,yy+1);
		p[3] = getPixel32_sub(prm,xx+1,yy+1);
	
		r.green =RoundFpShortDouble( (p[0].green * dx2 * dy2) + (p[1].green * dx1 * dy2) + (p[2].green * dx2 * dy1) + (p[3].green * dx1 * dy1) );
	}
	//blue---------------------------------------
	x = sht->b.x; 
	y = sht->b.y; 
	xx = x >>16;
	yy = y >>16;
	dx1 = (PF_FpLong)(x & 0xFFFF)/65536;
	dy1 = (PF_FpLong)(y & 0xFFFF)/65536;
	dx2 = 1.0 - dx1;
	dy2 = 1.0 - dy1;

	if ( (dx1==0)&&(dy1==0) ) {
		r.blue = getPixel32_sub(prm,xx,yy).blue;
	}else{
		p[0] = getPixel32_sub(prm,xx  ,yy  );
		p[1] = getPixel32_sub(prm,xx+1,yy  );
		p[2] = getPixel32_sub(prm,xx  ,yy+1);
		p[3] = getPixel32_sub(prm,xx+1,yy+1);
	
		r.blue =RoundFpShortDouble( (p[0].blue * dx2 * dy2) + (p[1].blue * dx1 * dy2) + (p[2].blue * dx2 * dy1) + (p[3].blue * dx1 * dy1) );
	}


	return r;
}
//----------------------------------------------------------------------------------------
inline channelShiftPrm addChannelShiftPrm(PF_Fixed X,PF_Fixed Y,channelShiftPrm *cs)
{
	channelShiftPrm ret;
	ret.a.x = X + cs->a.x;
	ret.a.y = Y + cs->a.y;

	ret.r.x = X + cs->r.x;
	ret.r.y = Y + cs->r.y;

	ret.g.x = X + cs->g.x;
	ret.g.y = Y + cs->g.y;

	ret.b.x = X + cs->b.x;
	ret.b.y = Y + cs->b.y;

	return ret;
}



#endif
