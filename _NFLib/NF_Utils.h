
#pragma once
#ifndef NFUTILS_H
#define NFUTILS_H

#include "AE_SDK.h"

#ifndef AE_CLAMP
#define AE_CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (MIN) : ((VAL) > (MAX) ? (MAX) : (VAL)))
#endif


//xorShift
#define F_SRAND(s) (init_xorShift(s))
#define F_RAND() (xorShift()) 
#define F_RAND_MAX 0x7FFF
#define F_RANDR(min,max) ( min + ((max - min)*xorShift()/F_RAND_MAX)) 

#define F_RAND_D() (xorShiftDouble())

#define F_RAND_D2(min,max) (min + ((max - min) * xorShiftDouble()))

#define F_RAND_D1()		(2 * xorShiftDouble() -1)

#define F_RAND2(min,max) ((min) + (A_long)( (double)F_RAND()*((double)(max) - (double)(min) +1.0f )/(double)(1.0f+F_RAND_MAX))) 
#define F_RAND3(max) ((A_long)( (double)F_RAND()*((double)(max) +1.0f )/(1.0f+F_RAND_MAX) ) )




#define	FIX2FLT(X)	((double)(X) / 65536.0)
#define	FLT2FIX(F)	((PF_Fixed)((F) * 65536 + (((F) < 0) ? -0.5 : 0.5)))
#define F_ABS(x) ((x) >= 0 ? (x) : -(x))

#ifndef F_SWAP
#define F_SWAP(a, b) { auto _tmp = (a); (a) = (b); (b) = _tmp; }
#endif
#ifndef F_MAX
#define F_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef F_MIN
#define F_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif
//変数の型変換

//-----------------------------------------------------------------------------------
inline A_u_char RoundByteLong(A_long x)
{
	return (A_u_char)AE_CLAMP(x, 0, PF_MAX_CHAN8);
}
//-----------------------------------------------------------------------------------
inline A_u_char RoundByteFpLong(PF_FpLong x)
{
	return (A_u_char)AE_CLAMP(x, 0, PF_MAX_CHAN8);
}
//-----------------------------------------------------------------------------------
inline A_u_char RoundByteFpShort(PF_FpShort x)
{
	return (A_u_char)AE_CLAMP(x, 0, PF_MAX_CHAN8);
}
//-----------------------------------------------------------------------------------
inline A_u_char RoundByteDouble(double x)
{
	return (A_u_char)AE_CLAMP(x, 0, PF_MAX_CHAN8);
}
//-----------------------------------------------------------------------------------
inline A_u_short RoundShort(A_long x)
{
	return (A_u_short)AE_CLAMP(x, 0, PF_MAX_CHAN16);
}
//-----------------------------------------------------------------------------------
inline A_u_short RoundShortFpLong(PF_FpLong x)
{
	return (A_u_short)AE_CLAMP(x, 0, PF_MAX_CHAN16);
}
//-----------------------------------------------------------------------------------
inline PF_FpShort RoundFpShort(PF_FpShort x)
{
	return (PF_FpShort)AE_CLAMP(x, 0, 32);
}
//-----------------------------------------------------------------------------------
inline PF_FpShort RoundFpShortDouble(PF_FpLong x)
{
	return (PF_FpShort)AE_CLAMP(x, 0, 32);
}
//-----------------------------------------------------------------------------------
inline PF_FpShort RoundFpShort2(PF_FpLong x)
{
	return (PF_FpShort)AE_CLAMP(x, 0, 1);
}
//-----------------------------------------------------------------------------------
//***********************************************************************************
/*
	乱数
	xorShiftに関しては以下のHPの記事を参考にしました。

	★小川暇つぶしチラ裏　2008年10月31日　ランダム
	http://ogawa-sankinkoutai.seesaa.net/article/108848981.html

	★銀天随筆集　Xorshift の初期化は
	http://d.hatena.ne.jp/gintenlabo/20100930/1285859540
*/
// 符号なし32bit整数を用いたハッシュ
static unsigned int hash_int(unsigned int x, unsigned int y, unsigned int seed) {
	unsigned int h = seed ^ (x * 2654435761U) ^ (y * 2246822519U);
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

// 0.0f ～ 1.0f の範囲に正規化
static float hash_float(int x, int y, int seed) {
	return (float)hash_int((unsigned int)x, (unsigned int)y, (unsigned int)seed) / (float)0xFFFFFFFFU;
}
//***********************************************************************************
// xorShift用のグローバル変数
static A_u_long gSeed128[8];
//-------------------------------------------------------------------
static A_u_long xorShift()
{
	A_u_long t = ( gSeed128[ 0 ] ^ ( gSeed128[ 0 ] << 11 ) );
	gSeed128[ 0 ] = gSeed128[ 1 ];
	gSeed128[ 1 ] = gSeed128[ 2 ];
	gSeed128[ 2 ] = gSeed128[ 3 ];
	A_u_long ret =  (gSeed128[ 3 ] = ( gSeed128[ 3 ] ^ ( gSeed128[ 3 ] >> 19 ) ) ^ ( t ^ ( t >> 8 ) ) );
	ret = (ret>>16) & 0x7FFF;
	return ret;

}
//-------------------------------------------------------------------
static double xorShiftDouble()
{
	A_u_long t = ( gSeed128[ 0 ] ^ ( gSeed128[ 0 ] << 11 ) );
	gSeed128[ 0 ] = gSeed128[ 1 ];
	gSeed128[ 1 ] = gSeed128[ 2 ];
	gSeed128[ 2 ] = gSeed128[ 3 ];
	A_u_long ret =  (gSeed128[ 3 ] = ( gSeed128[ 3 ] ^ ( gSeed128[ 3 ] >> 19 ) ) ^ ( t ^ ( t >> 8 ) ) );
	ret = ret & 0x7FFFFFFF;

	return (double)ret / 0x7FFFFFFF;

}
//-------------------------------------------------------------------
static void xorShiftPush()
{
	gSeed128[4] = gSeed128[0];
	gSeed128[5] = gSeed128[1];
	gSeed128[6] = gSeed128[2];
	gSeed128[7] = gSeed128[3];
}
//-------------------------------------------------------------------
static void xorShiftPop()
{
	gSeed128[0] = gSeed128[4];
	gSeed128[1] = gSeed128[5];
	gSeed128[2] = gSeed128[6];
	gSeed128[3] = gSeed128[7];
}
//-------------------------------------------------------------------
static void init_xorShift(A_u_long s)
{
	A_u_long ss = s + 100;
	for ( A_u_long i=1; i<=4;i++)
	{
		gSeed128[ i - 1 ] = ss = 1812433253U * ( ss ^ ( ss >> 30 ) ) + i;
	}
}
//-------------------------------------------------------------------
static A_u_long gSeed128R[8];
//-------------------------------------------------------------------
static A_u_long xorShiftR()
{
	A_u_long t = (gSeed128R[0] ^ (gSeed128R[0] << 11));
	gSeed128R[0] = gSeed128R[1];
	gSeed128R[1] = gSeed128R[2];
	gSeed128R[2] = gSeed128R[3];
	A_u_long ret = (gSeed128R[3] = (gSeed128R[3] ^ (gSeed128R[3] >> 19)) ^ (t ^ (t >> 8)));
	ret = (ret >> 16) & 0x7FFF;
	return ret;

}
//-------------------------------------------------------------------
static double xorShiftRDouble()
{
	A_u_long t = (gSeed128R[0] ^ (gSeed128R[0] << 11));
	gSeed128R[0] = gSeed128R[1];
	gSeed128R[1] = gSeed128R[2];
	gSeed128R[2] = gSeed128R[3];
	A_u_long ret = (gSeed128R[3] = (gSeed128R[3] ^ (gSeed128R[3] >> 19)) ^ (t ^ (t >> 8)));
	ret = ret & 0x7FFFFFFF;

	return (double)ret / 0x7FFFFFFF;

}
//-------------------------------------------------------------------
static void xorShiftRPush()
{
	gSeed128R[4] = gSeed128R[0];
	gSeed128R[5] = gSeed128R[1];
	gSeed128R[6] = gSeed128R[2];
	gSeed128R[7] = gSeed128R[3];
}
//-------------------------------------------------------------------
static void xorShiftRPop()
{
	gSeed128R[0] = gSeed128R[4];
	gSeed128R[1] = gSeed128R[5];
	gSeed128R[2] = gSeed128R[6];
	gSeed128R[3] = gSeed128R[7];
}
//-------------------------------------------------------------------
static void init_xorShiftR(A_u_long s)
{
	A_u_long ss = s + 100;
	for (A_u_long i = 1; i <= 4; i++)
	{
		gSeed128R[i - 1] = ss = 1812433253U * (ss ^ (ss >> 30)) + i;
	}
}
//-------------------------------------------------------------------
static A_u_long gSeed128M[8];
//-------------------------------------------------------------------
static A_u_long xorShiftM()
{
	A_u_long t = (gSeed128M[0] ^ (gSeed128M[0] << 11));
	gSeed128M[0] = gSeed128M[1];
	gSeed128M[1] = gSeed128M[2];
	gSeed128M[2] = gSeed128M[3];
	A_u_long ret = (gSeed128M[3] = (gSeed128M[3] ^ (gSeed128M[3] >> 19)) ^ (t ^ (t >> 8)));
	ret = (ret >> 16) & 0x7FFF;
	return ret;

}
//-------------------------------------------------------------------
static double xorShiftMDouble()
{
	A_u_long t = (gSeed128M[0] ^ (gSeed128M[0] << 11));
	gSeed128M[0] = gSeed128M[1];
	gSeed128M[1] = gSeed128M[2];
	gSeed128M[2] = gSeed128M[3];
	A_u_long ret = (gSeed128M[3] = (gSeed128M[3] ^ (gSeed128M[3] >> 19)) ^ (t ^ (t >> 8)));
	ret = ret & 0x7FFFFFFF;

	return (double)ret / 0x7FFFFFFF;

}
//-------------------------------------------------------------------
static void xorShiftMPush()
{
	gSeed128M[4] = gSeed128M[0];
	gSeed128M[5] = gSeed128M[1];
	gSeed128M[6] = gSeed128M[2];
	gSeed128M[7] = gSeed128M[3];
}
//-------------------------------------------------------------------
static void xorShiftMPop()
{
	gSeed128M[0] = gSeed128M[4];
	gSeed128M[1] = gSeed128M[5];
	gSeed128M[2] = gSeed128M[6];
	gSeed128M[3] = gSeed128M[7];
}
//-------------------------------------------------------------------
static void init_xorShiftM(A_u_long s)
{
	A_u_long ss = s + 100;
	for (A_u_long i = 1; i <= 4; i++)
	{
		gSeed128M[i - 1] = ss = 1812433253U * (ss ^ (ss >> 30)) + i;
	}
}
//***********************************************************************************
/*
	ピクセル変換
*/
//***********************************************************************************
#define NF_CONVERT8TO16(A)		( (((A_long)(A) * PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8 )
#define NF_CONVERT8TO32(A)      ((double)((long)((double)A*10000.0/(double)PF_MAX_CHAN8 + 0.5))/10000.0)
//#define NF_CONVERT8TO32(A)      ((double)(A)/PF_MAX_CHAN8)

#define NF_CONVERT16TO8(A)		( (((A_long)(A) * PF_MAX_CHAN8) + PF_HALF_CHAN16) / PF_MAX_CHAN16)
#define NF_CONVERT16TO32(A)      ((double)((long)((double)A*10000.0/(double)PF_MAX_CHAN16 + 0.5))/10000.0)
//#define NF_CONVERT16TO32(A)      ((double)(A)/PF_MAX_CHAN16)

#define NF_CONVERT32TO8(A)		(  (long)((A)*PF_MAX_CHAN8+0.5) )
#define NF_CONVERT32TO16(A)		(  NF_CONVERT8TO16(NF_CONVERT32TO8(A)) )
//#define FS_CONVERT32TO16(A)		( (long)(PF_MAX_CHAN16*(A)+0.5) )

#define NF_SHORT(A)				((double)(long)((double)A*10000.0 + 0.5)/10000.0)

//-----------------------------------------------------------------------------------
inline PF_Pixel16 NF_Pixel8To16(PF_Pixel c)
{
	PF_Pixel16 r;
	r.alpha		= RoundShortFpLong(NF_CONVERT8TO16(c.alpha));
	r.red		= RoundShortFpLong(NF_CONVERT8TO16(c.red));
	r.green		= RoundShortFpLong(NF_CONVERT8TO16(c.green));
	r.blue		= RoundShortFpLong(NF_CONVERT8TO16(c.blue));
	return r;
}
#define C16TO8(A)		(RoundByteFpLong( (((A_long)(A) * PF_MAX_CHAN8) + PF_HALF_CHAN16) / PF_MAX_CHAN16))
//-----------------------------------------------------------------------------------
inline PF_Pixel NF_Pixel16To8(PF_Pixel16 c)
{

	PF_Pixel r;
	r.alpha		= RoundByteFpLong(NF_CONVERT16TO8(c.alpha));
	r.red		= RoundByteFpLong(NF_CONVERT16TO8(c.red));
	r.green		= RoundByteFpLong(NF_CONVERT16TO8(c.green));
	r.blue		= RoundByteFpLong(NF_CONVERT16TO8(c.blue));
	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat NF_Pixel8To32(PF_Pixel c)
{
	PF_PixelFloat r;
	r.alpha		= (PF_FpShort)(NF_CONVERT8TO32(c.alpha));
	r.red		= (PF_FpShort)(NF_CONVERT8TO32(c.red));
	r.green		= (PF_FpShort)(NF_CONVERT8TO32(c.green));
	r.blue		= (PF_FpShort)(NF_CONVERT8TO32(c.blue));
	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat NF_Pixel6To32(PF_Pixel16 c)
{
	PF_PixelFloat r;
	r.alpha		= (PF_FpShort)(NF_CONVERT16TO32(c.alpha));
	r.red		= (PF_FpShort)(NF_CONVERT16TO32(c.red));
	r.green		= (PF_FpShort)(NF_CONVERT16TO32(c.green));
	r.blue		= (PF_FpShort)(NF_CONVERT16TO32(c.blue));
	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel NF_Pixel32To8(PF_PixelFloat c)
{
	PF_Pixel r;
	r.alpha		= RoundByteLong(NF_CONVERT32TO8(c.alpha));
	r.red		= RoundByteLong(NF_CONVERT32TO8(c.red));
	r.green		= RoundByteLong(NF_CONVERT32TO8(c.green));
	r.blue		= RoundByteLong(NF_CONVERT32TO8(c.blue));
	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel16 NF_Pixel32To16(PF_PixelFloat c)
{
	PF_Pixel16 r;
	r.alpha		= RoundShortFpLong(NF_CONVERT32TO16(c.alpha));
	r.red		= RoundShortFpLong(NF_CONVERT32TO16(c.red));
	r.green		= RoundShortFpLong(NF_CONVERT32TO16(c.green));
	r.blue		= RoundShortFpLong(NF_CONVERT32TO16(c.blue));
	return r;
}
//-------------------------------------------------------------------------------------------------
//*************************************************************************************************
//ピクセルの比較
//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix8(PF_Pixel s,PF_Pixel d)
{
	return ( (s.blue==d.blue)&&(s.green==d.green)&&(s.red==d.red) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix8Lv(PF_Pixel s,PF_Pixel d,A_u_char lv)
{
	return ( (F_ABS(s.blue-d.blue)<=lv)&&(F_ABS(s.green-d.green)<=lv)&&(F_ABS(s.red-d.red)<=lv) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix16(PF_Pixel16 s,PF_Pixel16 d)
{
	return ( (s.blue==d.blue)&&(s.green==d.green)&&(s.red==d.red) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix16Lv(PF_Pixel16 s,PF_Pixel16 d,A_u_short lv)
{
	return ( (F_ABS(s.blue-d.blue)<=lv)&&(F_ABS(s.green-d.green)<=lv)&&(F_ABS(s.red-d.red)<=lv) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix32(PF_PixelFloat s,PF_PixelFloat d)
{
	return ( (s.blue==d.blue)&&(s.green==d.green)&&(s.red==d.red) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix32Lv(PF_PixelFloat s,PF_PixelFloat d,PF_FpShort lv)
{
	return ( (F_ABS(s.blue-d.blue)<=lv)&&(F_ABS(s.green-d.green)<=lv)&&(F_ABS(s.red-d.red)<=lv) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix16_8(PF_Pixel16 s,PF_Pixel d)
{
	PF_Pixel ss;
	ss = NF_Pixel16To8(s);
	return ( (ss.blue==d.blue)&&(ss.green==d.green)&&(ss.red==d.red) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix16_8Lv(PF_Pixel16 s,PF_Pixel d,A_u_char lv)
{
	PF_Pixel ss;
	ss = NF_Pixel16To8(s);
	return ( (F_ABS(ss.blue-d.blue)<=lv)&&(F_ABS(ss.green-d.green)<=lv)&&(F_ABS(ss.red-d.red)<=lv) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix32_8Lv(PF_PixelFloat s,PF_Pixel d,A_u_char lv)
{
	PF_Pixel ss;
	ss = NF_Pixel32To8(s);
	return ( (F_ABS(ss.blue-d.blue)<=lv)&&(F_ABS(ss.green-d.green)<=lv)&&(F_ABS(ss.red-d.red)<=lv) );
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix32_8(PF_PixelFloat s,PF_Pixel d)
{
	PF_Pixel ss;
	ss = NF_Pixel32To8(s);
	return ( (ss.blue==d.blue)&&(ss.green==d.green)&&(ss.red==d.red) );
}
//-------------------------------------------------------------------------------------------------
inline A_u_char Pixel8Level(PF_Pixel s)
{
	//77 150 29
	/*
	A_long v = (s.red * 77 + s.green + s.blue * 29) /256;
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	return (A_u_char)v;
	*/
		
	double v = ( 0.29891 * (double)s.red) + ( 0.58661 * (double)s.green) + ( 0.11448 * (double)s.blue);
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	return (A_u_char)v;
	
}
//-------------------------------------------------------------------------------------------------
inline A_u_long Pixel8LevelA(PF_Pixel s)
{
	//77 150 29
	/*
	A_long v = (s.red * 77 + s.green + s.blue * 29) /256;
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	return (A_u_char)v;
	*/
		
	double v = ( 0.29891 * (double)s.red) + ( 0.58661 * (double)s.green) + ( 0.11448 * (double)s.blue);
	if ( v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	v = ((v+1) *256) + s.alpha;
	return (A_u_long)v;
	
}
//-------------------------------------------------------------------------------------------------
inline PF_FpShort Pixel8LevelFloat(PF_Pixel s)
{
	return (PF_FpShort)( (( 0.29891 * (double)s.red) + ( 0.58661 * (double)s.green) + ( 0.11448 * (double)s.blue))/PF_MAX_CHAN8);

}
//-------------------------------------------------------------------------------------------------
inline A_u_short Pixel16Level(PF_Pixel16 s)
{
	double v = ( 0.29891 * (double)s.red) + ( 0.58661 * (double)s.green) + ( 0.11448 * (double)s.blue);
	if ( v>PF_MAX_CHAN16) v = PF_MAX_CHAN16;
	return (A_u_short)v;

}
//-------------------------------------------------------------------------------------------------
inline PF_FpShort Pixel16LevelFloat(PF_Pixel16 s)
{
	return (PF_FpShort)( (( 0.29891 * (double)s.red) + ( 0.58661 * (double)s.green) + ( 0.11448 * (double)s.blue))/PF_MAX_CHAN16);
}
//-------------------------------------------------------------------------------------------------
inline PF_FpShort Pixel32Level(PF_PixelFloat s)
{
	PF_FpLong v = ( 0.29891 * s.red) + ( 0.58661 * s.green) + ( 0.11448 * s.blue);
	return (PF_FpShort)v;
}
//*************************************************************************************************
template <typename T>
struct PixelTraits;

template <> struct PixelTraits<A_u_char> { // 8-bit
	enum { MAX = PF_MAX_CHAN8, HALF = PF_HALF_CHAN8, SHIFT = 8 };
	typedef PF_Pixel PixelType;
};

template <> struct PixelTraits<A_u_short> { // 16-bit
	enum { MAX = PF_MAX_CHAN16, HALF = PF_HALF_CHAN16, SHIFT = 15 };
	typedef PF_Pixel16 PixelType;
};

template <> struct PixelTraits<PF_FpShort> { // 32-bit Float
	// Floatの場合は1.0が最大
	typedef PF_PixelFloat PixelType;
};
// ピクセルのアルファブレンド（共通実装）
/*
// 8bitの場合
PF_Pixel res = PixelAlphaBlendGeneric<A_u_char>(s_px, d_px);

// 16bitの場合
PF_Pixel16 res16 = PixelAlphaBlendGeneric<A_u_short>(s_px16, d_px16);

// 32bitの場合
PF_PixelFloat resF = PixelAlphaBlendGeneric<PF_FpShort>(s_pxF, d_pxF);
*/
template <typename T>
inline typename PixelTraits<T>::PixelType
PixelAlphaBlendGeneric(
	typename PixelTraits<T>::PixelType src,
	typename PixelTraits<T>::PixelType dst)
{
	using PType = typename PixelTraits<T>::PixelType;
	PType rr = { 0, 0, 0, 0 };

	if constexpr (std::is_floating_point_v<T>) {
		// --- 32-bit Float (0.0 - 1.0) の処理 ---
		if (src.alpha <= 0.0f || dst.alpha >= 1.0f) return dst;
		if (dst.alpha <= 0.0f) return src;

		float as2 = (1.0f - dst.alpha) * src.alpha;
		float ad2 = dst.alpha + as2;

		if (ad2 <= 0.0f) return rr;

		rr.alpha = min(ad2, 1.0f);
		rr.red = (dst.red * dst.alpha + src.red * as2) / ad2;
		rr.green = (dst.green * dst.alpha + src.green * as2) / ad2;
		rr.blue = (dst.blue * dst.alpha + src.blue * as2) / ad2;

		return rr;
	}
	else {
		// --- 8/16-bit Integer の処理 ---
		const A_long max_val = PixelTraits<T>::MAX;
		const A_long shift = PixelTraits<T>::SHIFT;

		if (src.alpha == 0 || dst.alpha == max_val) return dst;
		if (dst.alpha == 0) return src;

		// as2 = ((MAX + 1) - dst.alpha) * src.alpha / (MAX + 1)
		// 16bitの場合は32768を超える計算になるため A_longlong (64bit) を推奨
		long long as2 = (long long)((max_val + 1) - dst.alpha) * src.alpha >> shift;
		long long ad2 = (long long)dst.alpha + as2;

		if (ad2 > max_val) ad2 = max_val;
		if (ad2 <= 0) return rr;

		long long r = ((long long)dst.red * dst.alpha + (long long)src.red * as2) / ad2;
		long long g = ((long long)dst.green * dst.alpha + (long long)src.green * as2) / ad2;
		long long b = ((long long)dst.blue * dst.alpha + (long long)src.blue * as2) / ad2;

		rr.alpha = (T)ad2;
		rr.red = (T)min(r, (long long)max_val);
		rr.green = (T)min(g, (long long)max_val);
		rr.blue = (T)min(b, (long long)max_val);

		return rr;
	}
}
/*
//AlphaBlend
inline PF_Pixel PixelAlphaBlend8(PF_Pixel src,PF_Pixel dst)
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
inline PF_Pixel16 PixelAlphaBlend16(PF_Pixel16 src,PF_Pixel16 dst)
{
	PF_Pixel16 rr = {0,0,0,0};
	PF_Pixel16 ss = src;
	PF_Pixel16 dd = dst;

	if ( (ss.alpha == 0)||(dd.alpha==PF_MAX_CHAN16) ) return dd;
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
inline PF_PixelFloat PixelAlphaBlend32(PF_PixelFloat src,PF_PixelFloat dst)
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
*/
//*************************************************************************************************
inline PF_Pixel ToPremultiplied8(PF_Pixel col)
{
	PF_Pixel ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha == PF_MAX_CHAN8)
	{
		ret = col;
	}
	else {
		PF_FpLong a = (PF_FpLong)col.alpha / PF_MAX_CHAN8;
		ret.red = RoundByteFpLong((PF_FpLong)col.red * a +0.5);
		ret.green = RoundByteFpLong((PF_FpLong)col.green * a + 0.5);
		ret.blue = RoundByteFpLong((PF_FpLong)col.blue * a +0.5);
		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel16 ToPremultiplied16(PF_Pixel16 col)
{
	PF_Pixel16 ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha == PF_MAX_CHAN16)
	{
		ret = col;
	}
	else {
		PF_FpLong a = (PF_FpLong)col.alpha / PF_MAX_CHAN16;
		ret.red = RoundShortFpLong((PF_FpLong)col.red * a + 0.5);
		ret.green = RoundShortFpLong((PF_FpLong)col.green * a + 0.5);
		ret.blue = RoundShortFpLong((PF_FpLong)col.blue * a + 0.5);
		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel32 ToPremultiplied32(PF_Pixel32 col)
{
	PF_Pixel32 ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >=1.0)
	{
		col.alpha = 1.0;
		ret = col;
	}
	else {
		ret.red = RoundFpShort(col.red * col.alpha);
		ret.green = RoundFpShort(col.green * col.alpha);
		ret.blue = RoundFpShort(col.blue * col.alpha);
		ret.alpha = col.alpha;
	}
	return ret;
}
//*************************************************************************************************
inline PF_Pixel ToPremultipliedW8(PF_Pixel col)
{
	PF_Pixel ret = { 0,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8 };
	if (col.alpha <= 0)
	{
		ret.alpha = 0;
		ret.blue = ret.green = ret.red = PF_MAX_CHAN8;
	}
	else if (col.alpha >= PF_MAX_CHAN8)
	{
		ret = col;
	}
	else {
		PF_FpLong a = (PF_FpLong)col.alpha / PF_MAX_CHAN8;
		PF_FpLong b = 1 - a;

		ret.red = RoundByteFpLong(PF_MAX_CHAN8 * b + (PF_FpLong)col.red * a);
		ret.green = RoundByteFpLong(PF_MAX_CHAN8 * b + (PF_FpLong)col.green * a);
		ret.blue = RoundByteFpLong(PF_MAX_CHAN8 * b + (PF_FpLong)col.blue * a);
		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel16 ToPremultipliedW16(PF_Pixel16 col)
{
	PF_Pixel16 ret = { 0,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16 };
	if (col.alpha <= 0)
	{
	}
	else if (col.alpha >= PF_MAX_CHAN16)
	{
		ret = col;
	}
	else {
		PF_FpLong a = (PF_FpLong)col.alpha / PF_MAX_CHAN16;
		PF_FpLong b = 1 - a;

		ret.red = RoundShortFpLong(PF_MAX_CHAN16 * b + (PF_FpLong)col.red * a);
		ret.green = RoundShortFpLong(PF_MAX_CHAN16 * b + (PF_FpLong)col.green * a);
		ret.blue = RoundShortFpLong(PF_MAX_CHAN16 * b + (PF_FpLong)col.blue * a);
		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel32 ToPremultipliedW32(PF_Pixel32 col)
{
	PF_Pixel32 ret = { 0,1,1,1 };
	if (col.alpha <= 0)
	{
	}
	else if (col.alpha >= 1)
	{
		col.alpha = 1;
		ret = col;
	}
	else {
		PF_FpLong a = (PF_FpLong)col.alpha;
		if (a > 1) a = 1;
		PF_FpLong b = 1 - a;

		ret.red = RoundFpShortDouble(b + (PF_FpLong)col.red * a);
		ret.green = RoundFpShortDouble(b + (PF_FpLong)col.green * a);
		ret.blue = RoundFpShortDouble(b + (PF_FpLong)col.blue * a);
		ret.alpha = (PF_FpShort)a;
	}
	return ret;
}

//*************************************************************************************************
inline PF_Pixel ToStraight8(PF_Pixel col)
{
	PF_Pixel ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >= PF_MAX_CHAN8)
	{
		ret = col;
	}
	else
	{
		PF_FpLong a = PF_MAX_CHAN8 / (PF_FpLong)col.alpha;
		ret.red = RoundByteFpLong( ((PF_FpLong)col.red - PF_MAX_CHAN8) / (PF_FpLong)col.alpha);


		ret.green = RoundByteFpLong((PF_FpLong)col.green * a + 0.5);
		ret.blue = RoundByteFpLong((PF_FpLong)col.blue * a + 0.5);
		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel16 ToStraight16(PF_Pixel16 col)
{
	PF_Pixel16 ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >= PF_MAX_CHAN16)
	{
		ret = col;
	}
	else
	{
		PF_FpLong a = PF_MAX_CHAN16 / (PF_FpLong)col.alpha;
		ret.red = RoundShortFpLong((PF_FpLong)col.red * a + 0.5);
		ret.green = RoundShortFpLong((PF_FpLong)col.green * a + 0.5);
		ret.blue = RoundShortFpLong((PF_FpLong)col.blue * a + 0.5);
		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel32 ToStraight32(PF_Pixel32 col)
{
	PF_Pixel32 ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >= 1)
	{
		col.alpha = 1;
		ret = col;
	}
	else
	{
		PF_FpLong a = 1 / (PF_FpLong)col.alpha;
		ret.red = RoundFpShortDouble((PF_FpLong)col.red * a);
		ret.green = RoundFpShortDouble((PF_FpLong)col.green * a);
		ret.blue = RoundFpShortDouble((PF_FpLong)col.blue * a);
		ret.alpha = col.alpha;
	}
	return ret;
}

inline PF_Pixel ToStraightW8(PF_Pixel col)
{
	PF_Pixel ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >= PF_MAX_CHAN8)
	{
		ret = col;
	}
	else
	{
		double r = (PF_FpLong)col.red / PF_MAX_CHAN8;
		double g = (PF_FpLong)col.green / PF_MAX_CHAN8;
		double b = (PF_FpLong)col.blue / PF_MAX_CHAN8;
		double a = (PF_FpLong)col.alpha / PF_MAX_CHAN8;

		double a2 = 1 / a;
		double a3 = 1 - a;

		ret.red   = RoundByteFpLong((r - a3) * a2 * PF_MAX_CHAN8);
		ret.green = RoundByteFpLong((g - a3) * a2 * PF_MAX_CHAN8);
		ret.blue  = RoundByteFpLong((b - a3) * a2 * PF_MAX_CHAN8);

		ret.alpha = col.alpha;
	}
	return ret;
}
inline PF_Pixel16 ToStraightW16(PF_Pixel16 col)
{
	PF_Pixel16 ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >= PF_MAX_CHAN16)
	{
		ret = col;
	}
	else
	{
		double r = (PF_FpLong)col.red / PF_MAX_CHAN16;
		double g = (PF_FpLong)col.green / PF_MAX_CHAN16;
		double b = (PF_FpLong)col.blue / PF_MAX_CHAN16;
		double a = (PF_FpLong)col.alpha / PF_MAX_CHAN16;

		double a2 = 1 / a;
		double a3 = 1 - a;

		ret.red = RoundShortFpLong((r - a3) * a2 * PF_MAX_CHAN16);
		ret.green = RoundShortFpLong((g - a3) * a2 * PF_MAX_CHAN16);
		ret.blue = RoundShortFpLong((b - a3) * a2 * PF_MAX_CHAN16);

		ret.alpha = col.alpha;
	}
	return ret;
}

inline PF_Pixel32 ToStraightW32(PF_Pixel32 col)
{
	PF_Pixel32 ret = { 0,0,0,0 };
	if (col.alpha == 0)
	{
	}
	else if (col.alpha >= 1)
	{
		col.alpha = 1;
		ret = col;
	}
	else
	{
		double r = (PF_FpLong)col.red ;
		double g = (PF_FpLong)col.green;
		double b = (PF_FpLong)col.blue;
		double a = (PF_FpLong)col.alpha;

		double a2 = 1 / a;
		double a3 = 1 - a;

		ret.red = RoundFpShortDouble((r - a3) * a2);
		ret.green = RoundFpShortDouble((g - a3) * a2);
		ret.blue = RoundFpShortDouble((b - a3) * a2);
		ret.alpha = col.alpha;
	}
	return ret;
}


inline PF_FpLong gray8(PF_Pixel p)
{
	return (0.29891 * (PF_FpLong)p.red) + (0.58661 * (PF_FpLong)p.green) + (0.11448 * (PF_FpLong)p.blue);

}
inline PF_FpLong gray16(PF_Pixel16 p)
{
	return (0.29891 * (PF_FpLong)p.red) + (0.58661 * (PF_FpLong)p.green) + (0.11448 * (PF_FpLong)p.blue);

}
inline PF_FpLong gray32(PF_PixelFloat p)
{
	return (0.29891 * (PF_FpLong)p.red) + (0.58661 * (PF_FpLong)p.green) + (0.11448 * (PF_FpLong)p.blue);

}
//*************************************************************************************************
inline A_u_char ScrBlend8(A_u_char c0, A_u_char c1)
{
	return RoundByteLong((A_u_long)c0 + (A_u_long)c1 - (A_u_long)c0 * (A_u_long)c1/PF_MAX_CHAN8);
}
inline A_u_short ScrBlend16(A_u_short c0, A_u_short c1)
{
	return RoundShort((A_u_long)c0 + (A_u_long)c1 - (A_u_long)c0 * (A_u_long)c1 / PF_MAX_CHAN16);
}
inline PF_FpShort ScrBlend32(PF_FpShort c0, PF_FpShort c1)
{
	return RoundFpShortDouble((double)c0 + (double)c1 - (double)c0 * (double)c1);
}
//*************************************************************************************************
//ミリ計算
//*************************************************************************************************
inline PF_Fixed mm2px(PF_Fixed mm,PF_Fixed dpi)
{
	double ret;
	ret = ((double)mm/65536) * ((double)dpi/65536) / 25.4;
	return FLT2FIX(ret);
}
inline PF_Fixed px2mm(PF_Fixed px,PF_Fixed dpi)
{
	double ret;
	ret = ((double)px/65536) * 25.4 / ((double)dpi/65536);
	return FLT2FIX(ret);
}
//*************************************************************rot************************************
//座標計算
//*************************************************************************************************
inline PF_FpLong pointLength(PF_InData* in_data,PF_FpLong x0, PF_FpLong y0, PF_FpLong x1, PF_FpLong y1)
{
	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	return PF_SQRT( PF_POW(x1 - x0, 2) + PF_POW(y1 - y0, 2));
}	
//*************************************************************rot************************************
//角度計算
//*************************************************************************************************

inline  A_FloatPoint ROT_LENGTH(PF_InData* in_data,PF_FpLong r, PF_FpLong l, A_FloatPoint p)
{
	A_FloatPoint ret{ 0,0 };
	PF_FpLong r2 = r - 90;
	ret.x = (l * PF_COS(r2 * PF_PI / 180)) + p.x;
	ret.y = (l * PF_SIN(r2 * PF_PI / 180)) + p.y;
	return ret;
}
inline  A_FloatPoint POS_SCALE(A_FloatPoint p, A_FloatPoint c, PF_FpLong sc)
{
	A_FloatPoint ret = p;
	if (sc != 1) 
	{
		ret.x = (p.x - c.x) * sc + c.x;
		ret.y = (p.y - c.y) * sc + c.y;
	}
	return ret;
}

class CRotCalc
{
protected:
	PF_InData		*in_data;
	PF_Fixed		m_rot;
	PF_Fixed		m_length;
	PF_FixedPoint	m_pos;

public:
	//------------------------------
	void Init()
	{
		in_data = NULL;
		m_rot =
		m_length =
		m_pos.x =
		m_pos.y = 0;
	}
	//------------------------------
	CRotCalc(PF_InData *in_dataP)
	{
		Init();
		in_data = in_dataP;
	}
	//------------------------------
	PF_Fixed	rot() { return m_rot;}
	PF_Fixed	length() { return m_length;}
	PF_Fixed	x() { return m_pos.x;}
	PF_Fixed	y() { return m_pos.y;}
	PF_FixedPoint	pos() { return m_pos;}
	double		rotFLT() { return FIX2FLT(m_rot);}
	double		lengthFLT() { return FIX2FLT(m_length);}
	double		xFLT() { return FIX2FLT(m_pos.x);}
	double		yFLT() { return FIX2FLT(m_pos.y);}
	//------------------------------
	PF_Fixed RoundRot(PF_Fixed r)
	{
		PF_Fixed rr = r % (360L<<16);
		if ( rr<0) rr +=(360L<<16);
		return rr;
	}
	//------------------------------
	PF_FixedPoint SetRotLength(PF_Fixed r, PF_Fixed len)
	{
		m_rot = r;
		m_length = len;
		if (len != 0){
			PF_Fixed rr = RoundRot(r);
			double r2 = FIX2FLT(rr);
			double v2 = FIX2FLT(len);
			double x = 0;
			double y = 0;
			if ( (rr>=(0L<<16))&&(rr<(90L<<16)) ) {
				x =PF_SIN((PF_PI/180)*r2)*v2;
				y =PF_COS((PF_PI/180)*r2)*v2*-1;

			}else if ( (rr>=(90L<<16))&&(rr<(180L<<16)) ) {
				x =PF_COS((PF_PI/180)*(r2-90))*v2;
				y =PF_SIN((PF_PI/180)*(r2-90))*v2;

			}else if ( (rr>=(180L<<16))&&(rr<(270L<<16)) ) {
				x =PF_SIN((PF_PI/180)*(r2-180))*v2*-1;
				y =PF_COS((PF_PI/180)*(r2-180))*v2;

			}else if ( (rr>=(270L<<16))&&(rr<(360L<<16)) ) {
				x =PF_COS((PF_PI/180)*(r2-270))*v2*-1;
				y =PF_SIN((PF_PI/180)*(r2-270))*v2*-1;
			}
			m_pos.x = FLT2FIX(x);
			m_pos.y = FLT2FIX(y);
		}else{
			m_pos.x = 0;
			m_pos.y = 0;
		}
		return m_pos;
	}	
	//------------------------------
	PF_FixedPoint SetRot(PF_Fixed r) { return SetRotLength(r, m_length);}
	PF_FixedPoint SetLength(PF_Fixed len) { return SetRotLength(m_rot,len);}
	//------------------------------
	//------------------------------
	void SetPos(PF_Fixed x, PF_Fixed y)
	{
		m_pos.x = x;
		m_pos.y = y;
		if ( (x==0)&&(y==0) ){
			m_rot=0; m_length =0;
		}else{
			double xx = FIX2FLT(x);
			double yy = FIX2FLT(y);
			double rr = 90 - PF_ATAN2(yy,xx) * 180 /PF_PI;
			m_rot = FLT2FIX(rr) % (360L<<16);
			if (m_rot<0) m_rot += 360L<<16;

			xx = F_ABS(xx);
			yy = F_ABS(yy);
			double ll = PF_SQRT( PF_POW(xx,2) +PF_POW(yy,2));
			m_length = FLT2FIX(ll);
		}
	}
	//------------------------------
	void SetX(PF_Fixed x)
	{
		SetPos(x,m_pos.y);
	}
	//------------------------------
	void SetY(PF_Fixed y)
	{
		SetPos(m_pos.x,y);
	}
	//------------------------------
};
//*******************************************************
/*
inline void swapLong(A_long *s,A_long *d)
{
	A_long temp = *s;
	*s = *d;
	*d = temp;
}
*/
//*******************************************************
template<class T> 
inline void swapLong( T* s, T* d)
{
	T temp = *s;
	*s = *d;
	*d = temp;
}
	//*******************************************************
inline PF_Fixed RoundAngle360(PF_Fixed f)
{
	PF_Fixed ret = f;
	ret %= (360L<<16);
	if ( ret<0) ret += (360L<<16);
	return ret;
}
//*******************************************************

//*************************************************************rot************************************
//角度計算
//*************************************************************************************************
inline PF_Boolean CrossPointer(
	A_FloatPoint s0,
	A_FloatPoint s1,
	A_FloatPoint e0,
	A_FloatPoint e1,
	A_FloatPoint* p)
{
	PF_Boolean ret = FALSE;
	p->x = 0;
	p->y = 0;


	PF_FpLong dx0 = s1.x - s0.x;
	PF_FpLong dy0 = s1.y - s0.y;
	PF_FpLong dx1 = e1.x - e0.x;
	PF_FpLong dy1 = e1.y - e0.y;

	PF_FpLong dBunbo = dx0 * dy1 - dy0 * dx1;

	if (dBunbo == 0) return ret;
	PF_FpLong ax = e0.x - s0.x;
	PF_FpLong ay = e0.y - s0.y;

	PF_FpLong dr = (dy1 * ax - dx1 * ay) / dBunbo;
	//PF_FpLong ds = (dy0 * ax - dx0 * ay) / dBunbo;

	p->x = s0.x + dr * dx0;
	p->y = s0.y + dr * dy0;
	ret = TRUE;

	return ret;
}
class CRot
{
protected:
	PF_InData		*in_data = NULL;
	PF_FpLong		m_rot =0;
	PF_FpLong		m_length =0;
	PF_FpLong		m_x =0;
	PF_FpLong		m_y = 0;

	
public:
	//------------------------------
	void Init()
	{
		in_data = NULL;
		m_rot =
		m_length =
		m_x =
		m_y = 0;
	}
	//------------------------------
	CRot(PF_InData *in_dataP)
	{
		Init();
		in_data = in_dataP;
	}
	//------------------------------
	PF_FpLong	rot() { return m_rot; }
	PF_FpLong	length() { return m_length; }
	PF_FpLong	x() { return m_x; }
	PF_FpLong	y() { return m_y; }
	//------------------------------
	static PF_FpLong RoundRot(PF_FpLong r)
	{
		A_long rr = (A_long)(r * 65536 +0.5);
		rr = rr % (360*65536);
		if (rr < 0) rr += (360 * 65536);
		return (PF_FpLong)rr/65536;
	}
	//------------------------------
	void SetRotLength(PF_FpLong r, PF_FpLong len)
	{
		m_rot = r;
		m_length = len;
		if (len != 0) {
			PF_FpLong rr = RoundRot(r);
			double r2 = rr;
			double v2 = len;
			double x = 0;
			double y = 0;
			if ((rr >= 0.0) && (rr < 90.0)) {
				x = PF_SIN((PF_PI / 180)*r2)*v2;
				y = PF_COS((PF_PI / 180)*r2)*v2*-1;

			}
			else if ((rr >= 90) && (rr < 180)) {
				x = PF_COS((PF_PI / 180)*(r2 - 90))*v2;
				y = PF_SIN((PF_PI / 180)*(r2 - 90))*v2;

			}
			else if ((rr >= 180) && (rr < 270)) {
				x = PF_SIN((PF_PI / 180)*(r2 - 180))*v2*-1;
				y = PF_COS((PF_PI / 180)*(r2 - 180))*v2;

			}
			else if ((rr >= 270) && (rr < 360)) {
				x = PF_COS((PF_PI / 180)*(r2 - 270))*v2*-1;
				y = PF_SIN((PF_PI / 180)*(r2 - 270))*v2*-1;
			}
			m_x = x;
			m_y = y;
		}
		else {
			m_x = 0;
			m_y = 0;
		}
	}
	//------------------------------
	void SetRot(PF_FpLong r) { return SetRotLength(r, m_length); }
	void SetLength(PF_FpLong len) { return SetRotLength(m_rot, len); }
	//------------------------------
	//------------------------------
	void SetXY(PF_FpLong x, PF_FpLong y)
	{
		m_x = x;
		m_y = y;
		if ((x == 0) && (y == 0)) {
			m_rot = 0; m_length = 0;
		}
		else {
			double xx = x;
			double yy = y;
			double rr = 90 - PF_ATAN2(yy, xx) * 180 / PF_PI;
			m_rot = RoundRot( rr);

			double ll = PF_SQRT(PF_POW(xx, 2) + PF_POW(yy, 2));
			m_length = ll;
		}
	}
	//------------------------------
	void SetX(PF_FpLong x)
	{
		SetXY(x, m_y);
	}
	//------------------------------
	void SetY(PF_FpLong y)
	{
		SetXY(m_x, y);
	}
	//------------------------------
};
#endif
