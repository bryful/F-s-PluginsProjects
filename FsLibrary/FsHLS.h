#pragma once
#ifndef FsHLS_H
#define FsHLS_H
//*******************************************************************************************
#include "Fs.h"

#include "FsUtils.h"

#include <math.h>

typedef struct {
	PF_FpLong	H;
	PF_FpLong	L;
	PF_FpLong	S;
	PF_FpLong	A;
}HLSA;

typedef struct {
	PF_FpLong	L;
	PF_FpLong	A;
	PF_FpLong	B;
	PF_FpShort	alpha;

}LABA;

typedef struct {
	PF_FpLong	Y;
	PF_FpLong	U;
	PF_FpLong	V;
	PF_FpShort	alpha;

}YUVA;


typedef struct {
	PF_FpLong hs;
	PF_FpLong l;
}COL_DIS;

#define HLSMAX (1.0)
#define UNDEFINED	(HLSMAX*2/3)
#define RGBMAX		PF_MAX_CHAN8 
#define RGBMAX16	PF_MAX_CHAN16 
#define RGBMAX32	(1.0)

static HLSA RGBtoHLS(PF_PixelFloat c)
{
	HLSA hls;

	hls.A = c.alpha;

	PF_FpLong R = c.red;
	PF_FpLong G = c.green;
	PF_FpLong B = c.blue;

	PF_FpLong cMax = MAX( MAX(R,G), B);
	PF_FpLong cMin = MIN( MIN(R,G), B);
    PF_FpLong C = cMax - cMin; 
    
    hls.H = 0;
    hls.S = 0;
    hls.L = (cMax + cMin) / 2;
    
    if (C != 0) {
        if (hls.L <= 0.5) {
            hls.S = C / (2*hls.L);
        }
        else {
            hls.S = C / (2 - 2*hls.L);
        }
        
        if (R == cMax) {
            hls.H = (G - B) / C;
            if (hls.H < 0) {
                hls.H += 6;
            }
        }
        else if (G == cMax) {
            hls.H = (B - R) / C + 2;
        }
        else if (B == cMax) {
            hls.H = (R - G) / C + 4;
        }
        hls.H /= 6;
    }
    
    return hls;
}
//-------------------------------------------------------------------------------------------------
inline PF_FpLong hsl_value(PF_FpLong hh, PF_FpLong C, PF_FpLong m)
{
    PF_FpLong val;
    
    if (hh < 0) {
        hh += 6;
    }
    else if (hh >= 6) {
        hh -= 6;
    }
    
    if (hh < 1) {
        val = C * hh;
    }
    else if (hh < 3) {
        val = C;
    }
    else if (hh < 4) {
        val = C * (4 - hh);
    }
    else {
        val = 0;
    }
    val += m;
    
    return val;
}
//-------------------------------------------------------------------------------------------------
static PF_PixelFloat HLStoRGB(HLSA hls)
{
	PF_FpLong R =0, G =0, B = 0;
    PF_FpLong C ,m;

    if (hls.S == 0) {
        R = G = B = hls.L;
    }
    else {
        if (hls.L <= 0.5) {
            C = 2*hls.L * hls.S;
        }
        else {
            C = (2 - 2*hls.L) * hls.S;
        }
        m = hls.L - C/2;
        
        R = hsl_value(6*hls.H + 2, C, m);
        G = hsl_value(6*hls.H,     C, m);
        B = hsl_value(6*hls.H - 2, C, m);
    }
    
    PF_PixelFloat r;
    r.red	= (PF_FpShort)R;
    r.green = (PF_FpShort)G;
    r.blue	= (PF_FpShort)B;
	r.alpha	= (PF_FpShort)hls.A;
    return r;
}
#define SQMAX2 1.4142135623730950488016887242097
#define SQMAX3 1.7320508075688772935274463415059
//-------------------------------------------------------------------------------------------------
static PF_FpLong pixelDistane(PF_PixelFloat p0,PF_PixelFloat p1)
{
	HLSA s = RGBtoHLS(p0);
	HLSA d = RGBtoHLS(p1);
	PF_FpLong hueDiff = 0;
    if (s.H > d.H) {
		hueDiff = MIN((s.H - d.H), (d.H - s.H + 1.0));
    } else {
        hueDiff = MIN((d.H - s.H), (s.H - d.H + 1.0));
    }
	return (sqrt(pow((double)hueDiff, 2)+ pow((double)s.S - (double)d.S, 2) + pow((double)s.L - (double)d.L, 2))/SQMAX3) ;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong pixelDistane(PF_Pixel p0,PF_Pixel p1)
{
	return pixelDistane(CONV8TO32(p0),CONV8TO32(p1));
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong pixelDistane(PF_Pixel16 p0,PF_Pixel16 p1)
{
	return pixelDistane(CONV16TO32(p0),CONV16TO32(p1));
}
//-------------------------------------------------------------------------------------------------
static COL_DIS pixelDistaneHSL(PF_PixelFloat p0,PF_PixelFloat p1)
{
	COL_DIS ret;

	HLSA s = RGBtoHLS(p0);
	HLSA d = RGBtoHLS(p1);
	PF_FpLong hueDiff = 0;
    if (s.H > d.H) {
		hueDiff = MIN((s.H - d.H), (d.H - s.H + 1.0));
    } else {
        hueDiff = MIN((d.H - s.H), (s.H - d.H + 1.0));
    }
	ret.hs = (sqrt(pow((double)hueDiff, 2)+ pow((double)s.S - (double)d.S, 2)  )/SQMAX2) ;
	if (s.L != d.L) {
		PF_FpLong maxL = MAX(s.L , d.L);
		PF_FpLong minL = MIN(s.L , d.L);
		ret.l = 1 - minL/maxL;
	}else{
		ret.l = 0;
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static COL_DIS pixelDistaneHSL(PF_Pixel p0,PF_Pixel p1)
{
	return pixelDistaneHSL(CONV8TO32(p0),CONV8TO32(p1));
}
//-------------------------------------------------------------------------------------------------
static COL_DIS pixelDistaneHSL(PF_Pixel16 p0,PF_Pixel16 p1)
{
	return pixelDistaneHSL(CONV16TO32(p0),CONV16TO32(p1));
}
//-------------------------------------------------------------------------------------------------
static LABA RgbToLab(PF_PixelFloat rgb)
{
	double sr = rgb.red;
	if (sr < 0.04045)
		sr /= 12.92;
	else
		sr = pow((sr + 0.055) / 1.055, 2.4);

	double sg = rgb.green;
	if (sg < 0.04045)
		sg /= 12.92;
	else
		sg = pow((sg + 0.055) / 1.055, 2.4);

	double sb = rgb.blue;
	if (sb < 0.04045)
		sb /= 12.92;
	else
		sb = pow((sb + 0.055) / 1.055, 2.4);

	// XYZに変換
	double x = 0.412453*sr + 0.357580*sg + 0.180423*sb;
	double y = 0.212671*sr + 0.715160*sg + 0.072169*sb;
	double z = 0.019334*sr + 0.119193*sg + 0.950227*sb;

	x /= 0.950456;
	z /= 1.088754;

	// Labに変換
	LABA lab;
	double fy;
	if (y > 0.008856) {
		fy = pow(y, 1. / 3);
		lab.L = 1.16*fy;
	}
	else {
		fy = 7.787*y + 16. / 116;
		lab.L = 9.033*y;
	}
	lab.L -= 0.16;
	double fx = x > 0.008856 ? pow(x, 1. / 3) : 7.787*x + 16. / 116;
	double fz = z > 0.008856 ? pow(z, 1. / 3) : 7.787*z + 16. / 116;

	lab.A = 5 * (fx - fy);
	lab.B = 2 * (fy - fz);
	lab.alpha = rgb.alpha;

	return lab;

}
//-------------------------------------------------------------------------------------------------
static PF_PixelFloat Lab2Rgb(LABA lab)

{
	lab.L *= 100;
	lab.A *= 100;
	lab.B *= 100;
	double x, y, z;
	// XYZに変換
	if (lab.L < 7.9996) {
		y = lab.L / 903.3;
		x = 0.950456*(lab.A / 3893.5 + y);
		z = 1.088754*(y - lab.B / 1557.4);
	}
	else {
		y = pow((lab.L + 16) / 116, 3.);
		x = 0.950456*pow((lab.L + 16) / 116 + lab.A / 500, 3);
		z = 1.088754*pow((lab.L + 16) / 116 - lab.B / 200, 3);
	}

	// sRGBに変換
	double r, g, b;
	r = 3.241*x - 1.5374*y - 0.4986*z;
	g = -0.9692*x + 1.876*y + 0.0416*z;
	b = 0.0556*x - 0.2040*y + 1.057*z;

	// linear RGBに変換
	if (r > 0.00313080728)
		r = pow(r, 10. / 24)*1.055 - 0.055;
	else
		r *= 12.92;

	if (g > 0.00313080728)
		g = pow(g, 10. / 24)*1.055 - 0.055;
	else
		g *= 12.92;

	if (b > 0.00313080728)
		b = pow(b, 10. / 24)*1.055 - 0.055;
	else
		b *= 12.92;


	PF_PixelFloat bgr;
	bgr.red = (PF_FpShort)(MAX(MIN(b, 1), 0));
	bgr.green = (PF_FpShort)(MAX(MIN(g, 1), 0));
	bgr.blue = (PF_FpShort)(MAX(MIN(r, 1), 0));
	bgr.alpha = lab.alpha;

	return bgr;
}
//-------------------------------------------------------------------------------------------------
static YUVA RgbToYuv(PF_PixelFloat rgb)
{
	YUVA yuv;
	yuv.Y = (0.299 * rgb.red + 0.587 * rgb.green + 0.114 * rgb.blue);
	if (yuv.Y < 0) yuv.Y = 0; else if (yuv.Y > 1) yuv.Y = 1;
	yuv.U = (-0.169  * rgb.red - 0.331 * rgb.green + 0.500 * rgb.blue) + 0.5;
	if (yuv.U < 0) yuv.U = 0; else if (yuv.U > 1) yuv.U = 1;
	yuv.V = (0.500  * rgb.red - 0.419 * rgb.green - 0.081 * rgb.blue) + 0.5;
	if (yuv.V < 0) yuv.V = 0; else if (yuv.V > 1) yuv.V = 1;
	yuv.alpha = rgb.alpha;

	return yuv;

}
//-------------------------------------------------------------------------------------------------
static PF_PixelFloat YuvToRgb(YUVA yuv)
{
	PF_PixelFloat rgb;
	PF_FpLong u2, v2;
	u2 = yuv.U - 0.5;
	v2 = yuv.V - 0.5;
	rgb.red = RoundFpShort2(1.000*yuv.Y + 1.402 *v2);
	rgb.green = RoundFpShort2(1.000*yuv.Y - 0.344*u2 - 0.714*v2);
	rgb.blue = RoundFpShort2(1.000*yuv.Y + 1.772*u2);
	rgb.alpha = yuv.alpha;

	return rgb;
}
#endif