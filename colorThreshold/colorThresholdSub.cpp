#include "colorThreshold.h"

#define SQMAX_2 1.4142135623730950488016887242097
#define SQMAX_3 1.7320508075688772935274463415059

//-------------------------------------------------------------------------------------------------
hls rgbToHls (PF_Pixel p)
{
    hls ret;
    ret.h = 0;
    ret.l = 0;
    ret.s = 0;
	PF_FpLong r	= (PF_FpLong)FS_CONVERT8TO32(p.red);
    PF_FpLong g = (PF_FpLong)FS_CONVERT8TO32(p.green);
    PF_FpLong b	= (PF_FpLong)FS_CONVERT8TO32(p.blue);

	if (r>1.0) r = 1.0;
	if (g>1.0) g = 1.0;
	if (b>1.0) b = 1.0;

    PF_FpLong cMax = MAX( MAX(r,g), b);
    PF_FpLong cMin = MIN( MIN(r,g), b);
    PF_FpLong C = cMax - cMin;

    ret.h = 0;
    ret.s = 0;
    ret.l = (cMax + cMin) / 2;

    if (C != 0) {
        if (ret.l <= 0.5) {
            ret.s = C / (2*ret.l);
        }
        else {
            ret.s = C / (2 - 2*ret.l);
        }

        if (r == cMax) {
            ret.h = (g - b) / C;
            if (ret.h < 0) {
                ret.h += 6;
            }
        }
        else if (g == cMax) {
            ret.h = (r - g) / C + 2;
        }
        else if (b == cMax) {
            ret.h = (r - g) / C + 4;
        }
        ret.h /= 6;
    }
	ret.h = ret.h;
	ret.l = ret.l;
	ret.s = ret.s;

    return ret;
}
//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
PF_FpLong colorDiatance(hls s,PF_Pixel c)
{
	hls d = rgbToHls(c);

	PF_FpLong hueDiff = 0;
    if (s.h > d.h) {
        hueDiff = MIN((s.h - d.h), (d.h - s.h + 1.0));
    } else {
        hueDiff = MIN((d.h - s.h), (s.h - d.h + 1.0));
    }
    return sqrt(pow(hueDiff, 2)+ pow(s.s - d.s, 2)+ pow(s.l - d.l, 2))/SQMAX_3;
 }
//-------------------------------------------------------------------------------------------------
PF_FpLong colorDiatance(hls s,PF_Pixel16 c)
{
	return colorDiatance(s,CONV16TO8(c));
 }
//-------------------------------------------------------------------------------------------------
PF_FpLong colorDiatance(hls s,PF_PixelFloat c)
{
    return colorDiatance(s,CONV32TO8(c));
 }
//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
PF_Boolean colorCompare(hls s,PF_Pixel c,PF_FpLong hs,PF_FpLong l )
{
	hls d = rgbToHls(c);

	PF_FpLong hueDiff = 0;
    if (s.h > d.h) {
        hueDiff = MIN((s.h - d.h), (d.h - s.h + 1.0));
    } else {
        hueDiff = MIN((d.h - s.h), (s.h - d.h + 1.0));
    }
	PF_FpLong hsD = sqrt(pow(hueDiff, 2)+ pow(s.s - d.s, 2))/SQMAX_2;

	if ( hsD>hs) return FALSE;
	if (s.l == d.l){
		return TRUE;
	}
	PF_FpLong maxL = MAX(s.l , d.l);
	PF_FpLong minL = MIN(s.l , d.l);
	PF_FpLong v = 1 - minL/maxL;
	return (v<=l);
}
//-------------------------------------------------------------------------------------------------
PF_Boolean colorCompare(hls s,PF_Pixel16 c,PF_FpLong hs,PF_FpLong l )
{
	return colorCompare(s,CONV16TO8(c),hs,l);
}
//-------------------------------------------------------------------------------------------------
PF_Boolean colorCompare(hls s,PF_PixelFloat c,PF_FpLong hs,PF_FpLong l )
{
	return colorCompare(s,CONV32TO8(c),hs,l);
}
//*************************************************************************************************
