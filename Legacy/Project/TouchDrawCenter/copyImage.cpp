#include "TouchDrawCenter.h"
//--------------------------------------------------------------------------------
inline PF_Pixel MixPx8(PF_Pixel s,PF_Pixel d)
{
	PF_Pixel rr = {0,0,0,0};
	PF_Pixel ss = s;
	PF_Pixel dd = d;

	if ( (ss.alpha == 0)||(dd.alpha==PF_MAX_CHAN8) ) return dd;
	if (dd.alpha == 0) return ss;
	A_long as2 = ( (PF_MAX_CHAN8+1) - dd.alpha) * ss.alpha >> 8;
	A_long ad2 = dd.alpha + as2;

	if (ad2>PF_MAX_CHAN8) ad2 = PF_MAX_CHAN8;

	if (ad2<=0) {
		return rr;
	}else{
		A_long r = ( dd.red   * dd.alpha + ss.red   * as2 );
		A_long g = ( dd.green * dd.alpha + ss.green * as2 );
		A_long b = ( dd.blue  * dd.alpha + ss.blue  * as2 );

		r/=ad2; g/=ad2; b/=ad2;

		rr.alpha	= (A_u_char)ad2;
		rr.red		= RoundByteLong(r);
		rr.green	= RoundByteLong(g);
		rr.blue		= RoundByteLong(b);
		return rr;
	}
}
//--------------------------------------------------------------------------------
inline PF_Pixel16 MixPx16(PF_Pixel16 s,PF_Pixel16 d)
{
	PF_Pixel16 rr = {0,0,0,0};
	PF_Pixel16 ss = s;
	PF_Pixel16 dd = d;

	if ( (ss.alpha == 0)||(dd.alpha==PF_MAX_CHAN16) ) return dd;
	if (dd.alpha == 0) return ss;
	A_long as2 = ( (PF_MAX_CHAN16) - dd.alpha) * ss.alpha >> 15;
	A_long ad2 = dd.alpha + as2;

	if (ad2>PF_MAX_CHAN16) ad2 = PF_MAX_CHAN16;

	if (ad2<=0) {
		return rr;
	}else{
		A_long r = ( dd.red   * dd.alpha + ss.red   * as2 );
		A_long g = ( dd.green * dd.alpha + ss.green * as2 );
		A_long b = ( dd.blue  * dd.alpha + ss.blue  * as2 );

		r/=ad2; g/=ad2; b/=ad2;

		rr.alpha	= (A_u_short)ad2;
		rr.red		= RoundShort(r);
		rr.green	= RoundShort(g);
		rr.blue		= RoundShort(b);
		return rr;
	}
}
//--------------------------------------------------------------------------------
inline PF_PixelFloat MixPx32(PF_PixelFloat s,PF_PixelFloat d)
{
	PF_PixelFloat rr = {0,0,0,0};
	PF_PixelFloat ss = s;
	PF_PixelFloat dd = d;

	if ( (ss.alpha == 0)||(dd.alpha==1) ) return dd;
	if (dd.alpha == 0) return ss;
	PF_FpLong as2 = ( 1 - dd.alpha) * ss.alpha;
	PF_FpLong ad2 = dd.alpha + as2;

	if (ad2>1) ad2 = 1;

	if (ad2<=0) {
		return rr;
	}else{
		PF_FpLong r = ( dd.red   * dd.alpha + ss.red   * as2 );
		PF_FpLong g = ( dd.green * dd.alpha + ss.green * as2 );
		PF_FpLong b = ( dd.blue  * dd.alpha + ss.blue  * as2 );

		r/=ad2; g/=ad2; b/=ad2;

		rr.alpha	= (PF_FpShort)ad2;
		rr.red		= RoundFpShortDouble(r);
		rr.green	= RoundFpShortDouble(g);
		rr.blue		= RoundFpShortDouble(b);
		return rr;
	}
}
//--------------------------------------------------------------------------------
PF_Err 
CopyImage8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	copyImageInfo8 *	niP		= reinterpret_cast<copyImageInfo8*>(refcon);
					
	if (niP){
		PF_Pixel p;
		p.blue	= niP->col.blue;
		p.green	= niP->col.green;
		p.red	= niP->col.red;
		p.alpha = outP->alpha;
		PF_Pixel pp;
		pp.alpha	= inP->alpha;
		pp.green	= inP->green;
		pp.blue		= inP->blue;
		pp.red		= inP->red;

		if ( niP->org==TRUE) p = MixPx8(pp,p);

		outP->alpha	= p.alpha;
		outP->red	= p.red;
		outP->green	= p.green;
		outP->blue	= p.blue;
	}

	return err;
}
//--------------------------------------------------------------------------------
PF_Err 
CopyImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	copyImageInfo16 *	niP		= reinterpret_cast<copyImageInfo16*>(refcon);
					
	if (niP){
		PF_Pixel16 p;
		p.blue	= niP->col.blue;
		p.green	= niP->col.green;
		p.red	= niP->col.red;
		p.alpha = outP->alpha;
		PF_Pixel16 pp;
		pp.alpha	= inP->alpha;
		pp.green	= inP->green;
		pp.blue		= inP->blue;
		pp.red		= inP->red;

		if ( niP->org==TRUE) p = MixPx16(pp,p);

		outP->alpha	= p.alpha;
		outP->red	= p.red;
		outP->green	= p.green;
		outP->blue	= p.blue;
	}

	return err;
}
//--------------------------------------------------------------------------------
PF_Err 
CopyImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	copyImageInfo32 *	niP		= reinterpret_cast<copyImageInfo32*>(refcon);
					
	if (niP){
		PF_PixelFloat p;
		p.blue	= niP->col.blue;
		p.green	= niP->col.green;
		p.red	= niP->col.red;
		p.alpha = outP->alpha;
		PF_PixelFloat pp;
		pp.alpha	= inP->alpha;
		pp.green	= inP->green;
		pp.blue		= inP->blue;
		pp.red		= inP->red;

		if ( niP->org==TRUE) p = MixPx32(pp,p);

		outP->alpha	= p.alpha;
		outP->red	= p.red;
		outP->green	= p.green;
		outP->blue	= p.blue;
	}

	return err;
}
//--------------------------------------------------------------------------------

