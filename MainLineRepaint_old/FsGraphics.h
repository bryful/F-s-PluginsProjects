#ifndef FsGraphics_H
#define FsGraphics_H

#pragma once

#include "AEConfig.h"

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif
//*******************************************************************************************
/*
	描画用のクラス
	EffectWorldとin_dataから作成
*/
//*******************************************************************************************
class CFsGraph{
private:
	PF_Boolean			m_Enabled;
	PF_InData			*m_in_data;
	PF_EffectWorld	*m_world;
	PF_PixelPtr			m_data;
	PF_Boolean			m_is16Bit;

	long						m_width;
	long						m_height;
	long						m_widthTrue;
	long						m_offsetWidth;
	//--------------------------------------------------------------------
	// paint start 
	typedef struct  {
		long Xleft;		/* 領域左端のX座標 */
		long Xright;		/* 領域右端のX座標 */
		long Y;					/* 領域のY座標 */
		long Yparent;	/* 親ラインのY座標 */
	} paintQue;

	typedef struct  {
		PF_Pixel		targetCol;
		PF_Pixel16	targetCol16;
		long				max;
		long				start;
		long				last;
		paintQue		now;
		paintQue		*buf;
	}paintPrm;

	PF_Boolean paint_cmp8(PF_Pixel s,PF_Pixel d){ return ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue) );}
	PF_Boolean paint_cmp16(PF_Pixel16 s,PF_Pixel16 d){return ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue));}
	PF_Pixel paint_getPix8(long x,long y){ PF_Pixel *data =(PF_Pixel *)m_data;	return data[x + y * m_widthTrue];};
	PF_Pixel16 paint_getPix16(long x,long y){ PF_Pixel16 *data =(PF_Pixel16 *)m_data; return data[x + y * m_widthTrue];};
	void paint_scan_line8(paintPrm *prm);
	void paint_scan_line16(paintPrm *prm);
	// paint end 
	//--------------------------------------------------------------------
//protected:

public:
	//PF_Err paint16(long x, long y,PF_Pixel col);
	PF_Err paint16(long x, long y,PF_Pixel16 col);
	PF_Err paint8(long x, long y,PF_Pixel col);
	//-----------------------
	CFsGraph( PF_EffectWorld *world, PF_InData *in_data);
	//~CFsGraph();
	//-----------------------
	PF_Boolean Enabled(){ return m_Enabled;}
	PF_Boolean is16Bit(){ return m_is16Bit;}
	long width(){ return m_width;}
	long height(){ return m_height;}
	long widthTrue(){ return m_widthTrue;}
	long offsetWidth(){ return m_offsetWidth;}
	PF_PixelPtr data() {return m_data;}
	//-----------------------
	//塗りつぶし
	//PF_Err paint(long x, long y,PF_Pixel col);
	//-----------------------
	//AlphaBlend
	inline PF_Pixel PixelBlend8(PF_Pixel src,PF_Pixel dst)
	{
		PF_Pixel rr = {0,0,0,0};
		PF_Pixel ss = src;
		PF_Pixel dd = dst;

		if ( (ss.alpha == 0)||(dd.alpha==PF_MAX_CHAN8) ) return dd;
		if (dd.alpha == 0) return ss;
		
		long as2 = ( (PF_MAX_CHAN8+1) - dd.alpha) * ss.alpha >> 8;
		long ad2 = dd.alpha + as2;

		if (ad2>PF_MAX_CHAN8) ad2 = PF_MAX_CHAN8;

		if (ad2<=0) {
			//０の除算を防ぐ
			return rr;
		}else{
			long r = ( dd.red   * dd.alpha + ss.red   * as2 );
			long g = ( dd.green * dd.alpha + ss.green * as2 );
			long b = ( dd.blue  * dd.alpha + ss.blue  * as2 );
			//除算をなるべく早くする。無駄かな？
			switch(ad2){
			case 1:break;
			case 2:  r = r >>1;g = g >>1;b = b >>1;break;
			case 4:  r = r >>2;g = g >>2;b = b >>2;break;
			case 8:  r = r >>3;g = g >>3;b = b >>3;break;
			case 16: r = r >>4;g = g >>4;b = b >>4;break;
			case 32: r = r >>5;g = g >>5;b = b >>5;break;
			case 64: r = r >>6;g = g >>6;b = b >>6;break;
			case 128:r = r >>7;g = g >>7;b = b >>7;break;
			default:
					r/=ad2; g/=ad2; b/=ad2;
			}

			if (r>PF_MAX_CHAN8) r = PF_MAX_CHAN8;
			if (g>PF_MAX_CHAN8) g = PF_MAX_CHAN8;
			if (b>PF_MAX_CHAN8) b = PF_MAX_CHAN8;

			rr.alpha	= (unsigned char)ad2;
			rr.red		= (unsigned char)r;
			rr.green	= (unsigned char)g;
			rr.blue		= (unsigned char)b;
			return rr;
		}
	}
	inline PF_Pixel16 PixelBlend16(PF_Pixel16 src,PF_Pixel16 dst)
	{
		PF_Pixel16 rr = {0,0,0,0};
		PF_Pixel16 ss = src;
		PF_Pixel16 dd = dst;

		if ( (ss.alpha == 0)||(dd.alpha==PF_MAX_CHAN8) ) return dd;
		if (dd.alpha == 0) return ss;
		long as2 = ( PF_MAX_CHAN16 - dd.alpha) * ss.alpha >> 15;
		long ad2 = dd.alpha + as2;

		if (ad2>PF_MAX_CHAN16) ad2 = PF_MAX_CHAN16;

		if (ad2<=0) {
			return rr;
		}else{
			long r = ( dd.red   * dd.alpha + ss.red   * as2 );
			long g = ( dd.green * dd.alpha + ss.green * as2 );
			long b = ( dd.blue  * dd.alpha + ss.blue  * as2 );
			//除算をなるべく早くする。無駄かな？
			switch(ad2){
			case 1:break;
			case 2:  r = r >>1;g = g >>1;b = b >>1;break;
			case 4:  r = r >>2;g = g >>2;b = b >>2;break;
			case 8:  r = r >>3;g = g >>3;b = b >>3;break;
			case 16: r = r >>4;g = g >>4;b = b >>4;break;
			case 32: r = r >>5;g = g >>5;b = b >>5;break;
			case 64: r = r >>6;g = g >>6;b = b >>6;break;
			case 128:r = r >>7;g = g >>7;b = b >>7;break;
			case 256:r = r >>8;g = g >>8;b = b >>8;break;
			case 512:r = r >>9;g = g >>9;b = b >>9;break;
			case 1024:r= r >>10;g = g >>10;b = b >>10;break;
			case 2048:r= r >>11;g = g >>11;b = b >>11;break;
			case 4096:r= r >>12;g = g >>12;b = b >>12;break;
			case 8192:r= r >>13;g = g >>13;b = b >>13;break;
			case 16384:r=r >>14;g = g >>14;b = b >>14;break;
			case 32768:r=r >>15;g = g >>15;b = b >>15;break;
			default:
					r/=ad2; g/=ad2; b/=ad2;
			}

			if (r>PF_MAX_CHAN16) r = PF_MAX_CHAN16;
			if (g>PF_MAX_CHAN16) g = PF_MAX_CHAN16;
			if (b>PF_MAX_CHAN16) b = PF_MAX_CHAN16;

			rr.alpha	= (unsigned short)ad2;
			rr.red		= (unsigned short)r;
			rr.green	= (unsigned short)g;
			rr.blue		= (unsigned short)b;
			return rr;
		}
	}
	//-----------------------
	//point/pset
	inline PF_Pixel getPixel8_F(long x, long y){
		long xx = x;
		if (xx<0) { xx=0; }else if (xx>=m_width) {xx = m_width -1;}
		long yy = y;
		if (yy<0) { yy=0; }else if (yy>=m_height) {yy = m_height -1;}

		PF_Pixel *data = (PF_Pixel *)m_data;
		return data[xx + yy * m_widthTrue];
	}

	inline PF_Pixel getPixel8(long x, long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_Pixel r ={0,0,0,0};
			return r;
		}else{
			PF_Pixel *data = (PF_Pixel *)m_data;
			return data[x + y * m_widthTrue];
		}
	}
	inline PF_Pixel getPixel8_W(long x, long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_Pixel r ={PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8};
			return r;
		}else{
			PF_Pixel *data = (PF_Pixel *)m_data;
			return data[x + y * m_widthTrue];
		}
	}
	inline PF_Pixel16 getPixel16_F(long x, long y){
		long xx = x;
		if (xx<0) { xx=0; }else if (xx>=m_width) {xx = m_width -1;}
		long yy = y;
		if (yy<0) { yy=0; }else if (yy>=m_height) {yy = m_height -1;}

		PF_Pixel16 *data = (PF_Pixel16 *)m_data;

		return data[xx + yy * m_widthTrue];
	}
	inline PF_Pixel16 getPixel16(long x, long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_Pixel16 r ={0,0,0,0};
			return r;
		}else{
			PF_Pixel16 *data = (PF_Pixel16 *)m_data;
			return data[x + y * m_widthTrue];
		}
	}
	inline PF_Pixel16 getPixel16_W(long x, long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_Pixel16 r ={PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16};
			return r;
		}else{
			PF_Pixel16 *data = (PF_Pixel16 *)m_data;
			return data[x + y * m_widthTrue];
		}
	}
	inline void setPixel8(long x, long y,PF_Pixel col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel *data = (PF_Pixel *)m_data;
		data[x + y * m_widthTrue] = col;
	}

	inline void setPixel16(long x, long y,PF_Pixel16 col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel16 *data = (PF_Pixel16 *)m_data;
		data[x + y * m_widthTrue] = col;
	}
	inline void setPixelBlend8(long x, long y,PF_Pixel col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel *data = (PF_Pixel *)m_data;
		PF_Pixel s;
		long adr = x + y * m_widthTrue;
		s =data[adr]; 
		data[adr] = PixelBlend8(s,col);
	}
	inline void setPixelBlend16(long x, long y,PF_Pixel16 col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel16 *data = (PF_Pixel16 *)m_data;
		PF_Pixel16 s;
		long adr = x + y * m_widthTrue;
		s = data[adr]; 
		data[adr] = PixelBlend16(s,col);
	}
	//-----------------------
	// Line幅
	//これが影響するのは今のところ
	// XLine YLine Boxのみ
	long lineHeiht;
	//-----------------------
	//Line
	void XLine8(long x0,long x1,long y,PF_Pixel col);
	void XLine16(long x0,long x1,long y,PF_Pixel16 col);
	void YLine8(long x0,long y0,long y1,PF_Pixel col);
	void YLine16(long x0,long y0,long y1,PF_Pixel16 col);
	void Line8(long x0,long y0,long x1,long y1,PF_Pixel col);
	void Line16(long x0,long y0,long x1,long y1,PF_Pixel16 col);
	//-----------------------
	//Box
	void boxFill8(long x0,long y0,long x1,long y1,PF_Pixel col);
	void boxFill16(long x0,long y0,long x1,long y1,PF_Pixel16 col);
	void box8(long x0,long y0,long x1,long y1,PF_Pixel colu,PF_Pixel cold);
	void box8(long x0,long y0,long x1,long y1,PF_Pixel col){box8(x0,y0,x1,y1,col,col);}
	void box16(long x0,long y0,long x1,long y1,PF_Pixel16 colu,PF_Pixel16 cold);
	void box16(long x0,long y0,long x1,long y1,PF_Pixel16 col){box16(x0,y0,x1,y1,col,col);}
	//-----------------------
	//circle
	void circle8(long x ,long y, long r,PF_Pixel col);
	void circle16(long x ,long y, long r,PF_Pixel16 col);
	void circleFill8(long x ,long y, long r,PF_Pixel col);
	void circleFill16(long x ,long y, long r,PF_Pixel16 col);
	//-----------------------
	//画面消去
	PF_Err clear(){ PF_InData *in_data = m_in_data; return PF_FILL(NULL, NULL, m_world);}
	PF_Err clear(PF_Pixel col){	PF_InData *in_data = m_in_data; return PF_FILL(&col, NULL, m_world);}
	//-----------------------
	void swapLong(long *s, long *d){
		long temp;
		temp = *s;
		*s =*d;
		*d = temp;
	}
};
//******************************************************************************
// FsGraphicsBlend.cpp
// Blend関数
// perは(0L<<16)から(1L<<16)の間
//******************************************************************************
PF_Err FsBlend8(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per);
PF_Err FsBlend16(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per);

//******************************************************************************

//******************************************************************************
/*
アルファーブレンド関数
以下の記事をそのまま流用。

参考元
悟茶辞苑ッ -ごちゃたんファンクラブくらぶ-
http://d.hatena.ne.jp/GOCHA/20071027/alphablend

の2007-10-27 土曜日「RGBAのアルファブレンドに感激 -続・レイヤー処理- 」の記事

A(s): 合成元（下のレイヤー）の不透明度 
A(d): 合成先（上のレイヤー）の不透明度 
A(s'): 合成元（下のレイヤー）の合成量 
A(d'): 合成結果の不透明度 

A(s') = (1.0 - A(d)) * A(s);
A(d') = A(d) + A(s');

A(d') = A(d) + (1.0 - A(d)) * A(s)

c(s): 合成元（下のレイヤー）の輝度（RGBそれぞれの値に対応） 
c(d): 合成先（上のレイヤー）の輝度（RGBそれぞれの値に対応） 
c(d'): 合成結果の輝度（RGBそれぞれの値に対応） 
c(d') = c(d) * (A(d) / A(d')) + c(s) * (A(s') / A(d'))

A(d')による除算部分をくくって変形すると、

c(d') = (c(d) * A(d) + c(s) * A(s')) / A(d');


*/
//******************************************************************************
#endif
