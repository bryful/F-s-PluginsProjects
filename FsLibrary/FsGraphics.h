#pragma once
#ifndef FsGraphics_H
#define FsGraphics_H
//*******************************************************************************************
#include "Fs.h"
#include "FsUtils.h"


typedef struct FsMosaicParam{
	A_long		sizeX;
	A_long		sizeY;
	PF_FixedPoint	pos;
	PF_FpShort	rand;
	PF_Boolean	isRandGray;
	PF_Boolean	isFrameRand;
}FsMosaicParam, *FsMosaicParamP, **FsMosaicParamH;

enum{
	FsFLAT_TOPLEFT =0,
	FsFLAT_TOPRIGHT,
	FsFLAT_BOTTOMLEFT,
	FsFLAT_BOTTOMRIGHT,
};

enum
{
	FsMax_MODE_HOR_VER =1,
	FsMax_MODE_HORIZON =2,
	FsMax_MODE_VERTUAL =3
};

namespace MAT
{
	enum
	{
		none =0,
		blackMat,
		whiteMat
	};
}
typedef  A_long ScanLineMode;
namespace SCANLINE
{
	/// 1:水平&垂直 2: 水平 3:垂直
	enum
	{
		HorAndVer = 1,
		Hor,
		Ver
	};
}
typedef  A_long TargetChannelMode;

namespace TARGET_CHANNEL
{
	enum{
		rgbAndAlpha = 1,
		rgb,
		alpha
	};
}
typedef	A_long FsMax_MODE;

typedef struct {
	A_long x;
	A_long y;
	A_long sizeX;
	A_long sizeY;
}FsFlatParam;
	
typedef struct {
	A_long a;
	A_long r;
	A_long g;
	A_long b;
}FsPixelRand;

typedef struct {
	PF_FpShort a;
	PF_FpShort r;
	PF_FpShort g;
	PF_FpShort b;
}FsPixelRandFloat;


typedef struct GInfo {
	PF_InData	*in_data;
	A_long		width;
	A_long		widthTrue;
	A_long		offsetWidth;
	A_long		height;
	PF_PixelPtr	data;
	PF_FpLong	xD;
	PF_FpLong	yD;
}GInfo;



	

//*******************************************************************************************
/*
	描画用のクラス
	EffectWorldとin_dataから作成
*/
//*******************************************************************************************
class CFsGraph{
private:
	PF_Boolean			m_Enabled = FALSE;
	PF_InData			*m_in_data = NULL;
	PF_EffectWorld* m_world = NULL;;
	PF_PixelPtr			m_data = NULL;
	PF_PixelFormat		m_format = PF_PixelFormat_INVALID;
	A_long				m_frame = 0;

	A_long				m_width = 0;
	A_long				m_height = 0;
	A_long				m_widthTrue = 0;
	A_long				m_offsetWidth = 0;
	A_long				m_mat = MAT::none;

	A_long* m_vurTbl = NULL;
	PF_Handle	m_vurTblH = NULL;

	//--------------------------------------------------------------------
	// paint start 
	typedef struct  {
		A_long Xleft;		/* 領域左端のX座標 */
		A_long Xright;		/* 領域右端のX座標 */
		A_long Y;					/* 領域のY座標 */
		A_long Yparent;	/* 親ラインのY座標 */
	} paintQue;

	typedef struct  {
		PF_Pixel		targetCol;
		PF_Pixel16		targetCol16;
		PF_PixelFloat	targetCol32;
		A_long			max;
		A_long			start;
		A_long			last;
		paintQue		now;
		paintQue		*buf;
	}paintPrm;
	
	typedef struct blurPrm
	{
		A_long		blur;
		A_long		tbl[513];
		PF_PixelPtr	scanline;
	}blurPrm;

	typedef struct MiniMaxPrm
	{
		ScanLineMode	mode;
		PF_PixelPtr		scanline;	//スキャンラインバッファー
		A_long			*level;		//画像の明るさ
	
		A_long			value;	//Max/Minの値
		PF_Boolean		minusFlag;
	}MiniMaxPrm;

	PF_Boolean paint_cmp8(PF_Pixel s,PF_Pixel d){ return ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue) );}
	PF_Boolean paint_cmp16(PF_Pixel16 s,PF_Pixel16 d){return ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue));}
	PF_Boolean paint_cmp32(PF_PixelFloat s,PF_PixelFloat d){return ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue));}

	PF_Pixel paint_getPix8(A_long x,A_long y){ PF_Pixel *data =(PF_Pixel *)m_data;	return data[x + y * m_widthTrue];};
	PF_Pixel16 paint_getPix16(A_long x,A_long y){ PF_Pixel16 *data =(PF_Pixel16 *)m_data; return data[x + y * m_widthTrue];};
	PF_PixelFloat paint_getPix32(A_long x,A_long y){ PF_PixelFloat *data =(PF_PixelFloat *)m_data; return data[x + y * m_widthTrue];};

	void paint_scan_line8(paintPrm *prm);
	void paint_scan_line16(paintPrm *prm);
	void paint_scan_line32(paintPrm *prm);
	// paint end



	//--------------------------------------------------------------------
protected:
	PF_Err paint8(A_long x, A_long y,PF_Pixel col);
	PF_Err paint16(A_long x, A_long y,PF_Pixel col);
	PF_Err paint32(A_long x, A_long y,PF_Pixel col);

	PF_Err mosaic8(FsMosaicParam *mp);
	PF_Err mosaic16(FsMosaicParam *mp);
	PF_Err mosaic32(FsMosaicParam *mp);




	void verBlur8(blurPrm *sub);
	void horBlur8(blurPrm *sub);
	void verBlur16(blurPrm *sub);
	void horBlur16(blurPrm *sub);
	void verBlur32(blurPrm *sub);
	void horBlur32(blurPrm *sub);
public:
	//-----------------------
	CFsGraph( 
		PF_EffectWorld *world, 
		PF_InData *in_data,
		PF_PixelFormat	format = PF_PixelFormat_ARGB32
		);
	//-----------------------
	//-----------------------
	~CFsGraph();
	//-----------------------
	PF_Boolean Enabled(){ return m_Enabled;}
	
	PF_Boolean is8Bit(){ return (m_format == PF_PixelFormat_ARGB32);}
	PF_Boolean is16Bit(){ return (m_format == PF_PixelFormat_ARGB64);}
	PF_Boolean is32Bit(){ return (m_format == PF_PixelFormat_ARGB128);}

	PF_PixelFormat pixelFormat() { return m_format;}
	
	A_long width(){ return m_width;}
	A_long height(){ return m_height;}
	A_long widthTrue(){ return m_widthTrue;}
	A_long offsetWidth(){ return m_offsetWidth;}
	PF_PixelPtr data() { return m_data;}
	PF_Pixel* data8() { return (PF_Pixel*)m_data; }
	PF_Pixel16* data16() { return (PF_Pixel16*)m_data; }
	PF_PixelFloat* data32() { return (PF_PixelFloat*)m_data; }
	//-----------------------
	//塗りつぶし
	PF_Err paint(A_long x, A_long y,PF_Pixel col);
	PF_Err paint(PF_FixedPoint pos,PF_Pixel col);
	//-----------------------
	PF_Err flat8(FsFlatParam *fp,FsPixelRand *pr); 
	PF_Err flat16(FsFlatParam *fp,FsPixelRand *pr); 
	PF_Err flat32(FsFlatParam *fp,FsPixelRandFloat *pr); 

	PF_Err mosaic(FsMosaicParam *mp);
	//-----------------------
	/*
	PF_Err Max_rgb8(A_long value,ScanLineMode mode);
	PF_Err Max_rgb16(A_long value,ScanLineMode mode);
	PF_Err Max_rgb32(A_long value,ScanLineMode mode);
	PF_Err Max_alpha8(A_long value,ScanLineMode mode);
	PF_Err Max_alpha16(A_long value,ScanLineMode mode);
	PF_Err Max_alpha32(A_long value,ScanLineMode mode);

	PF_Err Max_rgb(A_long value,ScanLineMode mode); // 1:水平&垂直 2: 水平 3:垂直
	PF_Err Max_alpha(A_long value,ScanLineMode mode); // 1:水平&垂直 2: 水平 3:垂直
	*/

	void Minimax_rgb8(MiniMaxPrm *prm);
	void Minimax_alpha8(MiniMaxPrm *prm);
	PF_Err Minimax8(A_long value,ScanLineMode mode = SCANLINE::HorAndVer,TargetChannelMode target = TARGET_CHANNEL::rgbAndAlpha);
	
	PF_Err MinimaxA8(A_long value);
	PF_Err MinimaxA16(A_long value);
	PF_Err MinimaxA32(A_long value);
	
	void Minimax_rgb16(MiniMaxPrm *prm);
	void Minimax_alpha16(MiniMaxPrm *prm);
	PF_Err Minimax16(A_long value,ScanLineMode mode = SCANLINE::HorAndVer,TargetChannelMode target = TARGET_CHANNEL::rgbAndAlpha);

	void Minimax_rgb32(MiniMaxPrm *prm);
	void Minimax_alpha32(MiniMaxPrm *prm);
	PF_Err Minimax32(A_long value,ScanLineMode mode = SCANLINE::HorAndVer,TargetChannelMode target = TARGET_CHANNEL::rgbAndAlpha);
	
	PF_Err Minimax(A_long value,ScanLineMode mode = SCANLINE::HorAndVer,TargetChannelMode target = TARGET_CHANNEL::rgbAndAlpha);
	
	//-----------------------
	PF_Err blur8(A_long value,ScanLineMode mode);
	PF_Err gblur8(PF_FpLong value,ScanLineMode mode = SCANLINE::HorAndVer);
	PF_Err gblur16(PF_FpLong value,ScanLineMode mode = SCANLINE::HorAndVer);
	PF_Err gblur32(PF_FpLong value,ScanLineMode mode = SCANLINE::HorAndVer);
	//-----------------------
	void toBlackMat8();
	void toBlackMat16();
	void toBlackMat32();
	
	void fromBlackMat8();
	void fromBlackMat16();
	void fromBlackMat32();

	void toStraightMat8();
	void toStraightMat16();
	void toStraightMat32();

	void toWhiteMat8();
	void toWhiteMat16();
	void toWhiteMat32();

	void fromWhiteMat8();
	void fromWhiteMat16();
	void fromWhiteMat32();

	void SetMatMode(A_long m) {
		m_mat = m;
	}
	//-----------------------
	//AlphaBlend
	inline PF_Pixel PixelBlend8(PF_Pixel src,PF_Pixel dst)
	{
		PF_Pixel rr = {0,0,0,0};
		PF_Pixel ss = src;
		PF_Pixel dd = dst;

		if ( (ss.alpha == 0)||(dd.alpha==PF_MAX_CHAN8) ) return dd;
		if (dd.alpha == 0) return ss;
		
		A_long as2 = ( (PF_MAX_CHAN8+1) - dd.alpha) * ss.alpha >> 8;
		A_long ad2 = dd.alpha + as2;

		if (ad2>PF_MAX_CHAN8) ad2 = PF_MAX_CHAN8;

		if (ad2<=0) {
			//０の除算を防ぐ
			return rr;
		}else{
			A_long r = ( dd.red   * dd.alpha + ss.red   * as2 );
			A_long g = ( dd.green * dd.alpha + ss.green * as2 );
			A_long b = ( dd.blue  * dd.alpha + ss.blue  * as2 );
			r/=ad2; g/=ad2; b/=ad2;

			if (r>PF_MAX_CHAN8) r = PF_MAX_CHAN8;
			if (g>PF_MAX_CHAN8) g = PF_MAX_CHAN8;
			if (b>PF_MAX_CHAN8) b = PF_MAX_CHAN8;

			rr.alpha	= (A_u_char)ad2;
			rr.red		= (A_u_char)r;
			rr.green	= (A_u_char)g;
			rr.blue		= (A_u_char)b;
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
		A_long as2 = ( PF_MAX_CHAN16 - dd.alpha) * ss.alpha >> 15;
		A_long ad2 = dd.alpha + as2;

		if (ad2>PF_MAX_CHAN16) ad2 = PF_MAX_CHAN16;

		if (ad2<=0) {
			return rr;
		}else{
			A_long r = ( dd.red   * dd.alpha + ss.red   * as2 );
			A_long g = ( dd.green * dd.alpha + ss.green * as2 );
			A_long b = ( dd.blue  * dd.alpha + ss.blue  * as2 );
			r/=ad2; g/=ad2; b/=ad2;

			if (r>PF_MAX_CHAN16) r = PF_MAX_CHAN16;
			if (g>PF_MAX_CHAN16) g = PF_MAX_CHAN16;
			if (b>PF_MAX_CHAN16) b = PF_MAX_CHAN16;

			rr.alpha	= (A_u_short)ad2;
			rr.red		= (A_u_short)r;
			rr.green	= (A_u_short)g;
			rr.blue		= (A_u_short)b;
			return rr;
		}
	}
	inline PF_PixelFloat PixelBlend32(PF_PixelFloat src,PF_PixelFloat dst)
	{
		PF_PixelFloat rr = {0,0,0,0};
		PF_PixelFloat ss = src;
		PF_PixelFloat dd = dst;

		if ( (ss.alpha == 0)||(dd.alpha>=1.0) ) return dd;
		if (dd.alpha == 0) return ss;
		PF_FpShort as2 = ( 1 - dd.alpha) * ss.alpha;
		PF_FpShort ad2 = dd.alpha + as2;

		if (ad2>1.0) ad2 = 1.0;

		if (ad2<=0) {
			return rr;
		}else{
			PF_FpShort r = ( dd.red   * dd.alpha + ss.red   * as2 );
			PF_FpShort g = ( dd.green * dd.alpha + ss.green * as2 );
			PF_FpShort b = ( dd.blue  * dd.alpha + ss.blue  * as2 );
			r/=ad2; g/=ad2; b/=ad2;

			if (r>1.0) r = 1.0;
			if (g>1.0) g = 1.0;
			if (b>1.0) b = 1.0;

			rr.alpha	= ad2;
			rr.red		= r;
			rr.green	= g;
			rr.blue		= b;
			return rr;
		}
	}
	//-----------------------
	//point/pset
	inline PF_Pixel getPixel8_F(A_long x, A_long y){
		A_long xx = x;
		if (xx<0) { xx=0; }else if (xx>=m_width) {xx = m_width -1;}
		A_long yy = y;
		if (yy<0) { yy=0; }else if (yy>=m_height) {yy = m_height -1;}

		PF_Pixel *data = (PF_Pixel *)m_data;
		return data[xx + yy * m_widthTrue];
	}

	//-----------------------
	inline PF_Pixel getPixel8(A_long x, A_long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_Pixel r ={0,0,0,0};
			return r;
		}else{
			PF_Pixel *data = (PF_Pixel *)m_data;
			return data[x + y * m_widthTrue];
		}
	}
	//-----------------------
	//point/pset
	inline PF_Pixel getPixelFloat8(PF_FpLong x, PF_FpLong y){
		A_long x0 = (A_long)x;
		A_long x1 = x0 + 1;
		A_long y0 = (A_long)y;
		A_long y1 = y0 + 1;

		double dx1 = x -(double)x0;
		double dx0 = 1.0 - dx1;
		double dy1 = y -(double)y0;
		double dy0 = 1.0 - dy1;

		PF_Pixel p[4];
		PF_Pixel ret;
		p[0] = getPixel8_F(x0,y0);
		p[1] = getPixel8_F(x1,y0);
		p[2] = getPixel8_F(x0,y1);
		p[3] = getPixel8_F(x1,y1);

		ret.alpha = RoundByteLong( 
			(A_long)( 
			  (double)(p[0].alpha) * dx0 *dy0  
			+ (double)(p[1].alpha) * dx1 *dy0  
			+ (double)(p[2].alpha) * dx0 *dy1 
			+ (double)(p[3].alpha) * dx1 *dy1 +0.5));

		ret.red = RoundByteLong( 
			(A_long)( 
			  (double)(p[0].red) * dx0 *dy0  
			+ (double)(p[1].red) * dx1 *dy0  
			+ (double)(p[2].red) * dx0 *dy1 
			+ (double)(p[3].red) * dx1 *dy1  +0.5));

		ret.green = RoundByteLong( 
			(A_long)( 
			  (double)(p[0].green) * dx0 *dy0  
			+ (double)(p[1].green) * dx1 *dy0  
			+ (double)(p[2].green) * dx0 *dy1 
			+ (double)(p[3].green) * dx1 *dy1  +0.5));

		ret.blue = RoundByteLong( 
			(A_long)( 
			  (double)(p[0].blue) * dx0 *dy0  
			+ (double)(p[1].blue) * dx1 *dy0  
			+ (double)(p[2].blue) * dx0 *dy1 
			+ (double)(p[3].blue) * dx1 *dy1  +0.5));

		return ret;
	}
	//-----------------------
	//point/pset
	inline PF_Pixel16 getPixelFloat16(PF_FpLong x, PF_FpLong y){
		A_long x0 = (A_long)x;
		A_long x1 = x0 + 1;
		A_long y0 = (A_long)y;
		A_long y1 = y0 + 1;

		double dx1 = x -(double)x0;
		double dx0 = 1.0 - dx1;
		double dy1 = y -(double)y0;
		double dy0 = 1.0 - dy1;

		PF_Pixel16 p[4];
		PF_Pixel16 ret;
		p[0] = getPixel16_F(x0,y0);
		p[1] = getPixel16_F(x1,y0);
		p[2] = getPixel16_F(x0,y1);
		p[3] = getPixel16_F(x1,y1);

		ret.alpha = RoundShort( 
			(A_long)( 
			  (double)(p[0].alpha) * dx0 *dy0  
			+ (double)(p[1].alpha) * dx1 *dy0  
			+ (double)(p[2].alpha) * dx1 *dy0 
			+ (double)(p[3].alpha) * dx1 *dy1 +0.5));

		ret.red = RoundShort( 
			(A_long)( 
			  (double)(p[0].red) * dx0 *dy0  
			+ (double)(p[1].red) * dx1 *dy0  
			+ (double)(p[2].red) * dx0 *dy1 
			+ (double)(p[3].red) * dx1 *dy1  +0.5));

		ret.green = RoundShort( 
			(A_long)( 
			  (double)(p[0].green) * dx0 *dy0  
			+ (double)(p[1].green) * dx1 *dy0  
			+ (double)(p[2].green) * dx0 *dy1 
			+ (double)(p[3].green) * dx1 *dy1  +0.5));

		ret.blue = RoundShort( 
			(A_long)( 
			  (double)(p[0].blue) * dx0 *dy0  
			+ (double)(p[1].blue) * dx1 *dy0  
			+ (double)(p[2].blue) * dx0 *dy1 
			+ (double)(p[3].blue) * dx1 *dy1  +0.5));

		return ret;
	}
	//-----------------------
	//point/pset
	inline PF_Pixel32 getPixelFloat32(PF_FpLong x, PF_FpLong y) {
		A_long x0 = (A_long)x;
		A_long x1 = x0 + 1;
		A_long y0 = (A_long)y;
		A_long y1 = y0 + 1;

		double dx1 = x - (double)x0;
		double dx0 = 1.0 - dx1;
		double dy1 = y - (double)y0;
		double dy0 = 1.0 - dy1;

		PF_Pixel32 p[4];
		PF_Pixel32 ret;
		p[0] = getPixel32_F(x0, y0);
		p[1] = getPixel32_F(x1, y0);
		p[2] = getPixel32_F(x0, y1);
		p[3] = getPixel32_F(x1, y1);

		ret.alpha = RoundFpShortDouble(
			(
				(double)(p[0].alpha) * dx0 * dy0
				+ (double)(p[1].alpha) * dx1 * dy0
				+ (double)(p[2].alpha) * dx1 * dy0
				+ (double)(p[3].alpha) * dx1 * dy1 ));

		ret.red = RoundFpShortDouble(
			(
				(double)(p[0].red) * dx0 * dy0
				+ (double)(p[1].red) * dx1 * dy0
				+ (double)(p[2].red) * dx0 * dy1
				+ (double)(p[3].red) * dx1 * dy1 ));

		ret.green = RoundFpShortDouble(
			(
				(double)(p[0].green) * dx0 * dy0
				+ (double)(p[1].green) * dx1 * dy0
				+ (double)(p[2].green) * dx0 * dy1
				+ (double)(p[3].green) * dx1 * dy1 ));

		ret.blue = RoundFpShortDouble(
			(
				(double)(p[0].blue) * dx0 * dy0
				+ (double)(p[1].blue) * dx1 * dy0
				+ (double)(p[2].blue) * dx0 * dy1
				+ (double)(p[3].blue) * dx1 * dy1 ));

		return ret;
	}
	//-----------------------
	inline PF_Pixel16 getPixel16_F(A_long x, A_long y){
		A_long xx = x;
		if (xx<0) { xx=0; }else if (xx>=m_width) {xx = m_width -1;}
		A_long yy = y;
		if (yy<0) { yy=0; }else if (yy>=m_height) {yy = m_height -1;}

		PF_Pixel16 *data = (PF_Pixel16 *)m_data;

		return data[xx + yy * m_widthTrue];
	}

	//-----------------------
	inline PF_Pixel16 getPixel16(A_long x, A_long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_Pixel16 r ={0,0,0,0};
			return r;
		}else{
			PF_Pixel16 *data = (PF_Pixel16 *)m_data;
			return data[x + y * m_widthTrue];
		}
	}
	//-----------------------
	inline PF_PixelFloat getPixel32_F(A_long x, A_long y){
		A_long xx = x;
		if (xx<0) { xx=0; }else if (xx>=m_width) {xx = m_width -1;}
		A_long yy = y;
		if (yy<0) { yy=0; }else if (yy>=m_height) {yy = m_height -1;}

		PF_PixelFloat *data = (PF_PixelFloat *)m_data;

		return data[xx + yy * m_widthTrue];
	}

	//-----------------------
	inline PF_PixelFloat getPixel32(A_long x, A_long y){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) {
			PF_PixelFloat r ={0,0,0,0};
			return r;
		}else{
			PF_PixelFloat *data = (PF_PixelFloat *)m_data;

			return data[x + y * m_widthTrue];
		}
	}

	//-----------------------
	inline void setPixel8(A_long x, A_long y,PF_Pixel col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel *data = (PF_Pixel *)m_data;
		data[x + y * m_widthTrue] = col;
	}

	//-----------------------
	inline void setPixel16(A_long x, A_long y,PF_Pixel16 col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel16 *data = (PF_Pixel16 *)m_data;
		data[x + y * m_widthTrue] = col;
	}
	//-----------------------
	inline void setPixel32(A_long x, A_long y,PF_PixelFloat col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_PixelFloat *data = (PF_PixelFloat *)m_data;
		data[x + y * m_widthTrue] = col;
	}

	//-----------------------
	inline void setPixelBlend8(A_long x, A_long y,PF_Pixel col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel *data = (PF_Pixel *)m_data;
		PF_Pixel s;
		A_long adr = x + y * m_widthTrue;
		s =data[adr]; 
		data[adr] = PixelBlend8(s,col);
	}
	//-----------------------
	inline void setPixelBlend16(A_long x, A_long y,PF_Pixel16 col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_Pixel16 *data = (PF_Pixel16 *)m_data;
		PF_Pixel16 s;
		A_long adr = x + y * m_widthTrue;
		s = data[adr]; 
		data[adr] = PixelBlend16(s,col);
	}
	//-----------------------
	inline void setPixelBlend32(A_long x, A_long y,PF_PixelFloat col){
		if ( (x<0) || (y<0) || (x>=m_width) || (y>=m_height) ) return;
		PF_PixelFloat *data = (PF_PixelFloat *)m_data;
		PF_PixelFloat s;
		A_long adr = x + y * m_widthTrue;
		s = data[adr]; 
		data[adr] = PixelBlend32(s,col);
	}
	//**************************************************************************
	inline PF_Pixel scrBlend8(PF_Pixel s, PF_Pixel d)
	{
		d.alpha = RoundByteLong((A_long)((double)s.alpha + (double)d.alpha - ((double)s.alpha * (double)d.alpha / PF_MAX_CHAN8) + 0.5));
		d.red   = RoundByteLong((A_long)((double)s.red   + (double)d.red   - ((double)s.red   * (double)d.red   / PF_MAX_CHAN8) + 0.5));
		d.green = RoundByteLong((A_long)((double)s.green + (double)d.green - ((double)s.green * (double)d.green / PF_MAX_CHAN8) + 0.5));
		d.blue  = RoundByteLong((A_long)((double)s.blue  + (double)d.blue  - ((double)s.blue  * (double)d.blue  / PF_MAX_CHAN8) + 0.5));
		return d;
	}
	inline PF_Pixel16 scrBlend16(PF_Pixel16 s, PF_Pixel16 d)
	{
		d.alpha = RoundShort((A_long)((double)s.alpha + (double)d.alpha - ((double)s.alpha * (double)d.alpha / PF_MAX_CHAN16) + 0.5));
		d.red   = RoundShort((A_long)((double)s.red   + (double)d.red   - ((double)s.red   * (double)d.red   / PF_MAX_CHAN16) + 0.5));
		d.green = RoundShort((A_long)((double)s.green + (double)d.green - ((double)s.green * (double)d.green / PF_MAX_CHAN16) + 0.5));
		d.blue  = RoundShort((A_long)((double)s.blue  + (double)d.blue  - ((double)s.blue  * (double)d.blue  / PF_MAX_CHAN16) + 0.5));
		return d;
	}
	inline PF_Pixel pxPer8(PF_Pixel s,PF_FpLong per)
	{
		PF_Pixel d;
		d.alpha = RoundByteLong((A_long)((double)s.alpha * per + 0.5));
		d.red   = RoundByteLong((A_long)((double)s.red   * per + 0.5));
		d.green = RoundByteLong((A_long)((double)s.green * per + 0.5));
		d.blue  = RoundByteLong((A_long)((double)s.blue  * per + 0.5));
		return d;
	}
	inline PF_Pixel16 pxPer16(PF_Pixel16 s,PF_FpLong per)
	{
		PF_Pixel16 d;
		d.alpha = RoundShort((A_long)((double)s.alpha * per + 0.5));
		d.red   = RoundShort((A_long)((double)s.red   * per + 0.5));
		d.green = RoundShort((A_long)((double)s.green * per + 0.5));
		d.blue  = RoundShort((A_long)((double)s.blue  * per + 0.5));
		return d;
	}
	//--------------
	inline PF_Boolean setPixelScreenF8(PF_FpLong x, PF_FpLong y,PF_Pixel col)
	{
		PF_Boolean ret = FALSE;
		A_long x0 = (A_long)x;
		A_long x1 = x0+1;
		A_long y0 = (A_long)y;
		A_long y1 = y0+1;

		double dx1 = x - (double)x0; 
		double dx0 = 1.0 - dx1; 

		double dy1 = y - (double)y0; 
		double dy0 = 1.0 - dy1; 

		PF_Pixel *data = (PF_Pixel *)m_data;
		A_long adr = x0 + y0 * m_widthTrue;

		if ( (x0>=0)&&(x0<m_width)&&(y0>=0)&&(y0<m_height)){
			data[adr] = scrBlend8(data[adr],pxPer8(col,dx0*dy0));
			ret = TRUE;
		}
		adr++;
		if ( (x1>=0)&&(x1<m_width)&&(y0>=0)&&(y0<m_height)){
			data[adr] = scrBlend8(data[adr],pxPer8(col,dx1*dy0));
			ret = TRUE;
		}
		adr+= m_widthTrue -1;
		if ( (x0>=0)&&(x0<m_width)&&(y1>=0)&&(y1<m_height)){
			data[adr] = scrBlend8(data[adr],pxPer8(col,dx0*dy1));
			ret = TRUE;
		}
		adr++;
		if ( (x1>=0)&&(x1<m_width)&&(y1>=0)&&(y1<m_height) ){
			data[adr] = scrBlend8(data[adr],pxPer8(col,dx1*dy1));
			ret = TRUE;
		}
		return ret;
	}
	//--------------
	inline PF_Boolean setPixelScreenF16(PF_FpLong x, PF_FpLong y,PF_Pixel16 col)
	{
		PF_Boolean ret = FALSE;
		A_long x0 = (A_long)x;
		A_long x1 = x0+1;
		A_long y0 = (A_long)y;
		A_long y1 = y0+1;

		double dx1 = x - (double)x0; 
		double dx0 = 1.0 - dx1; 

		double dy1 = y - (double)y0; 
		double dy0 = 1.0 - dy1; 

		PF_Pixel16 *data = (PF_Pixel16 *)m_data;
		A_long adr = x0 + y0 * m_widthTrue;

		if ( (x0>=0)&&(x0<m_width)&&(y0>=0)&&(y0<m_height)){
			data[adr] = scrBlend16(data[adr],pxPer16(col,dx0*dy0));
			ret = TRUE;
		}
		adr++;
		if ( (x1>=0)&&(x1<m_width)&&(y0>=0)&&(y0<m_height)){
			data[adr] = scrBlend16(data[adr],pxPer16(col,dx1*dy0));
			ret = TRUE;
		}
		adr+= m_widthTrue -1;
		if ( (x0>=0)&&(x0<m_width)&&(y1>=0)&&(y1<m_height)){
			data[adr] = scrBlend16(data[adr],pxPer16(col,dx0*dy1));
			ret = TRUE;
		}
		adr++;
		if ( (x1>=0)&&(x1<m_width)&&(y1>=0)&&(y1<m_height) ){
			data[adr] = scrBlend16(data[adr],pxPer16(col,dx1*dy1));
			ret = TRUE;
		}
		return ret;
	}
	//**************************************************************************
	inline PF_Pixel screen8(PF_Pixel s, PF_Pixel d, double per)
	{
			double r,g,b,a;
			a = (double)d.alpha * per;
			if (a<=0) return s;
			r = (double)d.red   * per;
			g = (double)d.green * per;
			b = (double)d.blue  * per;
			d.red   = RoundByteLong((A_long)((double)s.red   + r - (double)s.red   * r / PF_MAX_CHAN8 +0.5));   		
			d.green = RoundByteLong((A_long)((double)s.green + g - (double)s.green * g / PF_MAX_CHAN8 +0.5));   		
			d.blue  = RoundByteLong((A_long)((double)s.blue  + g - (double)s.blue  * b / PF_MAX_CHAN8 +0.5));   		
			d.alpha = RoundByteLong((A_long)((double)s.alpha + a - (double)s.alpha * a / PF_MAX_CHAN8 +0.5));
			return d;
	}
	//**************************************************************************
	inline PF_Pixel16 screen16(PF_Pixel16 s, PF_Pixel16 d, double per)
	{
			double r,g,b,a;
			a = (double)d.alpha * per;
			if (a<=0) return s;
			r = (double)d.red   * per;
			g = (double)d.green * per;
			b = (double)d.blue  * per;
			d.red   = RoundShort((A_long)((double)s.red   + r - (double)s.red   * r / PF_MAX_CHAN16 +0.5));   		
			d.green = RoundShort((A_long)((double)s.green + g - (double)s.green * g / PF_MAX_CHAN16 +0.5));   		
			d.blue  = RoundShort((A_long)((double)s.blue  + g - (double)s.blue  * b / PF_MAX_CHAN16 +0.5));   		
			d.alpha = RoundShort((A_long)((double)s.alpha + a - (double)s.alpha * a / PF_MAX_CHAN16 +0.5));
			return d;
	}
	//**************************************************************************
	
	//**************************************************************;************
	//-----------------------
	// Line幅
	//これが影響するのは今のところ
	// XLine YLine Boxのみ
	A_long lineHeiht = 1;
	//-----------------------
	//Line
	void XLine8(A_long x0,A_long x1,A_long y,PF_Pixel col);
	void XLine16(A_long x0,A_long x1,A_long y,PF_Pixel16 col);
	void XLine32(A_long x0,A_long x1,A_long y,PF_PixelFloat col);
	void XLine(A_long x0,A_long x1,A_long y,PF_Pixel col)
	{
		switch(m_format)
		{
		case PF_PixelFormat_ARGB128:
			PF_PixelFloat col32;
			col32= CONV8TO32(col);
			XLine32(x0,x1,y,col32);
			break;
		case PF_PixelFormat_ARGB64:
			PF_Pixel16 col16;
			col16 = CONV8TO16(col);
			XLine16(x0,x1,y,col16);
			break;
		case PF_PixelFormat_ARGB32:
			XLine8(x0,x1,y,col);
			break;
		}
	}

	void YLine8(A_long x0,A_long y0,A_long y1,PF_Pixel col);
	void YLine16(A_long x0,A_long y0,A_long y1,PF_Pixel16 col);
	void YLine32(A_long x0,A_long y0,A_long y1,PF_PixelFloat col);
	void YLine(A_long x0,A_long y0,A_long y1,PF_Pixel col)
	{
		switch(m_format)
		{
		case PF_PixelFormat_ARGB128:
			PF_PixelFloat col32;
			col32= CONV8TO32(col);
			YLine32(x0,y0,y1,col32);
			break;
		case PF_PixelFormat_ARGB64:
			PF_Pixel16 col16;
			col16 = CONV8TO16(col);
			YLine16(x0,y0,y1,col16);
			break;
		case PF_PixelFormat_ARGB32:
			YLine8(x0,y0,y1,col);
			break;
		}
	}

	void Line8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel col);
	void Line16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 col);
	void Line32(A_long x0,A_long y0,A_long x1,A_long y1,PF_PixelFloat col);
	void Line(A_long x0, A_long y0, A_long x1, A_long y1, PF_Pixel col)
	{
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			PF_PixelFloat col32; col32 = CONV8TO32(col);
			Line32(x0, y0, x1, y1, col32);
			break;
		case PF_PixelFormat_ARGB64:
			PF_Pixel16 col16; col16 = CONV8TO16(col);
			Line16(x0, y0, x1, y1, col16);
			break;
		case PF_PixelFormat_ARGB32:
			Line8(x0, y0, x1, y1, col);
			break;
		}
	}
	//-----------------------
	//Box
	void boxFill8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel col);
	void boxFill16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 col);
	void boxFill32(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel32 col);
	void box8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel colu,PF_Pixel cold);
	void box8(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel col){box8(x0,y0,x1,y1,col,col);}
	void box16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 colu,PF_Pixel16 cold);
	void box16(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel16 col){box16(x0,y0,x1,y1,col,col);}
	void box32(A_long x0,A_long y0,A_long x1,A_long y1,PF_PixelFloat colu,PF_PixelFloat cold);
	void box32(A_long x0,A_long y0,A_long x1,A_long y1,PF_PixelFloat col){box32(x0,y0,x1,y1,col,col);}
	void box(A_long x0,A_long y0,A_long x1,A_long y1,PF_Pixel colu,PF_Pixel cold)
	{
		switch(m_format)
		{
		case PF_PixelFormat_ARGB128:
			PF_PixelFloat colu32; colu32 = CONV8TO32(colu);
			PF_PixelFloat cold32; cold32 = CONV8TO32(cold);
			box32(x0,y0,x1,y1,colu32,cold32);
			break;
		case PF_PixelFormat_ARGB64:
			PF_Pixel16 colu16; colu16 = CONV8TO16(colu);
			PF_Pixel16 cold16; cold16 = CONV8TO16(cold);
			box16(x0,y0,x1,y1,colu16,cold16);
			break;
		case PF_PixelFormat_ARGB32:
			box8(x0,y0,x1,y1,colu,cold);
			break;
		}
	}
	//-----------------------
	//circle
	void circle8(A_long x ,A_long y, A_long r,PF_Pixel col);
	void circle16(A_long x ,A_long y, A_long r,PF_Pixel16 col);
	void circle32(A_long x ,A_long y, A_long r,PF_PixelFloat col);
	void circleFill8(A_long x ,A_long y, A_long r,PF_Pixel col);
	void circleFill16(A_long x ,A_long y, A_long r,PF_Pixel16 col);
	void circleFill32(A_long x ,A_long y, A_long r,PF_PixelFloat col);
	//-----------------------
	//画面消去
	PF_Err clear(){ PF_InData *in_data = m_in_data; return PF_FILL(NULL, NULL, m_world);}
	PF_Err clear(PF_Pixel col){	PF_InData *in_data = m_in_data; return PF_FILL(&col, NULL, m_world);}
	//-----------------------
	//******************************************************************************
	void ToGInfo(GInfo *gi)
	{
		gi->in_data = m_in_data;
		gi->width = m_width;
		gi->widthTrue = m_widthTrue;
		gi->offsetWidth = m_offsetWidth;
		gi->height = m_height;
		gi->data = m_data;
	}
};
//******************************************************************************
// FsGraphicsBlend.cpp
// Blend関数
// perは(0L<<16)から(1L<<16)の間
//******************************************************************************
PF_Err FsBlend8(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per);
PF_Err FsBlend16(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per);
PF_Err FsBlend32(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per);

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
