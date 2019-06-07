#include "Flare.h"




static PF_Err
WhiteBase(
	refconType		refcon,
	A_long		xL,
A_long		yL,
PF_Pixel8	*inP,
PF_Pixel8	*outP)
{
PF_Err			err = PF_Err_NONE;
FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);

if (inP->alpha <= 0)
{
	PF_Pixel r = { 0,0,0,0 };
	*outP = r;
	return err;
}
double a = (double)inP->alpha / (double)PF_MAX_CHAN8;
a *= infoP->opacitry;

A_long rgb = ((A_long)inP->red + (A_long)inP->green + (A_long)inP->blue) / 3;
//”½“]
rgb = (PF_MAX_CHAN8 - rgb);
if (rgb < 0) rgb = 0;
else if (rgb > PF_MAX_CHAN8) rgb = PF_MAX_CHAN8;

if (a > 1) a = 1;
if (a >= 1)
{
	outP->red = (A_u_char)rgb;
	outP->green = (A_u_char)rgb;
	outP->blue = (A_u_char)rgb;
	outP->alpha = PF_MAX_CHAN8;
}
else {
	A_u_char v = RoundByteDouble((double)rgb*a);
	outP->red = v;
	outP->green = v;
	outP->blue = v;
	outP->alpha = RoundByteDouble(PF_MAX_CHAN8*a);
}
return err;
}

//-------------------------------------------------------------
static
PF_Err
BlackBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel r = { 0,0,0,0 };
		*outP = r;
		return err;
	}
	double a = (double)inP->alpha / (double)PF_MAX_CHAN8;
	a *= infoP->opacitry;

	A_long rgb = (inP->red + inP->green + inP->blue) / 3;
	if (rgb < 0) rgb = 0;
	else if (rgb > PF_MAX_CHAN8) rgb = PF_MAX_CHAN8;

	if (a > 1) a = 1;
	if (a >= 1)
	{
		outP->red = (A_u_char)rgb;
		outP->green = (A_u_char)rgb;
		outP->blue = (A_u_char)rgb;
		outP->alpha = PF_MAX_CHAN8;
	}
	else {
		A_u_char v = RoundByteDouble((double)rgb*a);
		outP->red = v;
		outP->green = v;
		outP->blue = v;
		outP->alpha = RoundByteDouble(PF_MAX_CHAN8*a);
	}
	return err;
}
static PF_Err
AlphaBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel r = { 0,0,0,0 };
		*outP = r;
	}
	else if ((inP->alpha >= PF_MAX_CHAN8) && (infoP->opacitry >= 1))
	{
		PF_Pixel r2 = { PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8 };
		*outP = r2;

	}
	else {
		A_u_char a = RoundByteDouble((double)inP->alpha *infoP->opacitry);

		outP->red = a;
		outP->green = a;
		outP->blue = a;
		outP->alpha = a;

	}



	return err;
}
static PF_Err
ImageBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);

	double a = (double)inP->alpha / PF_MAX_CHAN8;
	a *= infoP->opacitry;
	if (a <= 0)
	{
		PF_Pixel r = { 0,0,0,0 };
		*outP = r;

	}
	else {
		outP->red = RoundByteDouble(inP->red * a);
		outP->green = RoundByteDouble(inP->green * a);
		outP->blue = RoundByteDouble(inP->blue * a);
		outP->alpha = RoundByteDouble(inP->alpha * a);
	}


	return err;
}


//-------------------------------------------------------------
static PF_Err
Normal(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);
	
	PF_Pixel rr = { 0,0,0,0 };
	PF_Pixel ss = *outP;
	PF_Pixel dd = *inP;

	if ((ss.alpha == 0) || (dd.alpha == PF_MAX_CHAN8))
	{
		*outP = dd;
	}
	else if (dd.alpha > 0)
	{
		A_long as2 = (ss.alpha - dd.alpha* ss.alpha/ PF_MAX_CHAN8) ;
		A_long ad2 = dd.alpha + as2;

		if (ad2 > PF_MAX_CHAN8) ad2 = PF_MAX_CHAN8;

		if (ad2 <= 0) {
			*outP = rr;
		}
		else 
		{
			A_long r = (dd.red   * dd.alpha + ss.red   * as2);
			A_long g = (dd.green * dd.alpha + ss.green * as2);
			A_long b = (dd.blue  * dd.alpha + ss.blue  * as2);
			r /= ad2; g /= ad2; b /= ad2;

			if (r > PF_MAX_CHAN8) r = PF_MAX_CHAN8;
			if (g > PF_MAX_CHAN8) g = PF_MAX_CHAN8;
			if (b > PF_MAX_CHAN8) b = PF_MAX_CHAN8;

			outP->alpha = (A_u_char)ad2;
			outP->red = (A_u_char)r;
			outP->green = (A_u_char)g;
			outP->blue = (A_u_char)b;
			
		}
	}

	
	return err;
}
//-------------------------------------------------------------
static PF_Err
BlendScreen(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);
	outP->red = RoundByteLong((A_long)inP->red + (A_long)outP->red - ((A_long)inP->red * (A_long)outP->red) / PF_MAX_CHAN8);
	outP->green = RoundByteLong((A_long)inP->green + (A_long)outP->green - ((A_long)inP->green * (A_long)outP->green) / PF_MAX_CHAN8);
	outP->blue = RoundByteLong((A_long)inP->blue + (A_long)outP->blue - ((A_long)inP->blue * (A_long)outP->blue) / PF_MAX_CHAN8);
	outP->alpha = RoundByteLong((A_long)inP->alpha + (A_long)outP->alpha - ((A_long)inP->alpha * (A_long)outP->alpha) / PF_MAX_CHAN8);

	return err;
}
//-------------------------------------------------------------
static PF_Err
BlendAdd(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo *	infoP = reinterpret_cast<FlareInfo*>(refcon);
	outP->red = RoundByteLong((A_long)inP->red + (A_long)outP->red);
	outP->green = RoundByteLong((A_long)inP->green + (A_long)outP->green);
	outP->blue = RoundByteLong((A_long)inP->blue + (A_long)outP->blue);
	outP->alpha = RoundByteLong((A_long)inP->alpha + (A_long)outP->alpha);

	return err;
}
//-------------------------------------------------------------
PF_Err
Equal(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	*outP = *inP;

	return err;
}

// ------------------------------------------------------ -
/*
Mat‚ðŒ³‚É–ß‚·
*/
static PF_Err fromBlackMat8(CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;
	A_long target = 0;
	PF_Pixel *data;
	data = (PF_Pixel *)ae->output->data;
	A_long w = ae->out->width();
	A_long offset = ae->out->offsetWidth();
	A_long h = ae->out->height();


	for (A_long j = 0; j < h; j++) {
		for (A_long i = 0; i < w; i++) {
			PF_Pixel p = data[target];
			if (p.alpha < PF_MAX_CHAN8) {
				if (p.alpha <= 0) {
					p.blue = p.green = p.red = 0;
				}
				else if (p.alpha < PF_MAX_CHAN8) {
					p.blue = RoundByteLong((A_long)p.blue * PF_MAX_CHAN8 / (A_long)p.alpha);
					p.green = RoundByteLong((A_long)p.green * PF_MAX_CHAN8 / (A_long)p.alpha);
					p.red = RoundByteLong((A_long)p.red * PF_MAX_CHAN8 / (A_long)p.alpha);
				}
				data[target] = p;
			}
			target++;
		}
		target += offset;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
 PF_Err Exec08(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ERR(ae->NewTmpWorld());

	//–{”Ô
	for (int i = 0; i < PCOUNT; i++)
	{
		ae->tmp->clear();
		if (infoP->flareInfo[i].enabled == FALSE) continue;
		if (infoP->flareInfo[i].opacitry <=0) continue;

		//tmp‚Ö
		switch (infoP->flareInfo[i].mode)
		{
		case 4: // image
			ERR(ae->iterate8InputToTemp((refconType)&infoP->flareInfo[i], ImageBase));
			break;
		case 3: // alpha
			ERR(ae->iterate8InputToTemp((refconType)&infoP->flareInfo[i], AlphaBase));
			break;
		case 2: // blackbase
			ERR(ae->iterate8InputToTemp((refconType)&infoP->flareInfo[i], BlackBase));
			break;
		case 1: // whitebase
		default:
			ERR(ae->iterate8InputToTemp((refconType)&infoP->flareInfo[i], WhiteBase));
			break;
		}

		//output‚Ö–ß‚·
		switch (infoP->flareInfo[i].blend)
		{
		case 1:
			ERR(ae->iterate8TempToOutput((refconType)&infoP->flareInfo[i], BlendScreen));
			break;
		case 2:
			ERR(ae->iterate8TempToOutput((refconType)&infoP->flareInfo[i], BlendAdd));
			break;
		case 3:
		default:
			ERR(ae->iterate8TempToOutput((refconType)&infoP->flareInfo[i], Normal));
			break;
		}

	}

	ERR(fromBlackMat8(ae));
	return err;
}
 