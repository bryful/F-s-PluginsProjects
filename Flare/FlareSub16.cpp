#include "Flare.h"

//==================================================================================
/*

*/
static PF_Err ToHarfSize(CFsGraph* src)
{
	PF_Err			err = PF_Err_NONE;
	A_long ow = src->width();
	A_long owt = src->widthTrue();
	A_long oh = src->height();

	A_long wh = ow / 2;
	A_long hh = oh / 2;
	PF_Pixel16* data = (PF_Pixel16*)src->data();
	PF_Pixel16 bl = { 0,0,0,0 };

	//まず水平方向
	for (A_long y = 0; y < oh; y++)
	{
		A_long ypos = y * owt;
		for (A_long x = 0; x < wh; x++)
		{
			PF_Pixel16 c0, c1,c2;
			c0 = data[ypos + 2 * x + 0];
			c1 = data[ypos + 2 * x + 1];
			c2.red = (A_u_short)(((long)c0.red + (long)c1.red) / 2);
			c2.green = (A_u_short)(((long)c0.green + (long)c1.green) / 2);
			c2.blue = (A_u_short)(((long)c0.blue + (long)c1.blue) / 2);
			c2.alpha = (A_u_short)(((long)c0.alpha + (long)c1.alpha) / 2);
			data[ypos + x] = c2;

		}
		for (A_long x = wh; x < ow; x++)
		{
			data[ypos + x] = bl;
		}

	}
	//垂直
	for (A_long x = 0; x < ow; x++)
	{
		A_long ypos = 0;
		A_long ypos2 = 0;

		for (A_long y = 0; y < hh; y++)
		{
			PF_Pixel16 c0, c1, c2;
			c0 = data[x + ypos];
			ypos += owt;
			c1 = data[x + ypos];
			ypos += owt;
			c2.red = (A_u_short)(((long)c0.red + (long)c1.red) / 2);
			c2.green = (A_u_short)(((long)c0.green + (long)c1.green) / 2);
			c2.blue = (A_u_short)(((long)c0.blue + (long)c1.blue) / 2);
			c2.alpha = (A_u_short)(((long)c0.alpha + (long)c1.alpha) / 2);
			data[x +ypos2] = c2;
			ypos2 += owt;

		}
		for (A_long y = hh; y < oh; y++)
		{
			data[x + ypos2] = bl;
			ypos2 += owt;
		}
	}


	return err;
}
//=========================================================================================
static PF_Err ToDoubleSize(CFsGraph* src,PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	A_long ow = src->width();
	A_long owt = src->widthTrue();
	A_long oh = src->height();

	A_long hw = ow / 2;
	A_long hh = oh / 2;
	PF_Pixel16* data = (PF_Pixel16*)src->data();
	PF_Pixel16 bl = { 0,0,0,0 };
	PF_Pixel16* scanline = *(PF_Pixel16**)bufH;

	//まず水平方向
	for (A_long y = 0; y < oh; y++)
	{
		A_long ypos = y * owt;
		//スキャンラインへ転送
		for (A_long x = 0; x < hw; x++) scanline[x] = data[x + ypos];
		for (A_long x = 0; x < hw; x++)
		{
			A_long x1 = x; if (x1 >= hw) x1 = hw-1;
			A_long x2 = x+1; if (x2 >= hw) x2 = hw-1;
			PF_Pixel16 c0 = scanline[x1];
			PF_Pixel16 c1 = scanline[x2];
			PF_Pixel16 c2;
			if (x1 == x2) {
				c2 = c0;
			}
			else {
				c2.red = (A_u_short)(((long)c0.red + (long)c1.red) / 2);
				c2.green = (A_u_short)(((long)c0.green + (long)c1.green) / 2);
				c2.blue = (A_u_short)(((long)c0.blue + (long)c1.blue) / 2);
				c2.alpha = (A_u_short)(((long)c0.alpha + (long)c1.alpha) / 2);
			}
			x2 = 2 * x;
			data[x2 + ypos] = c0;
			x2++;
			if (x2 < ow) {
				data[x2 + ypos] = c2;
			}
		}

	}
	//垂直
	for (A_long x = 0; x < ow; x++)
	{
		A_long ypos = 0;
		A_long ypos2 = 0;

		//スキャンラインへ転送
		for (A_long y = 0; y < hh; y++)
		{
			scanline[y] = data[x + ypos];
			ypos += owt;
		}
		ypos = 0;
		for (A_long y = 0; y < hh; y++)
		{
			A_long y1 = y; if (y1 >= hh) y1 = hh - 1;
			A_long y2 = y + 1; if (y2 >= hh) y2 = hh - 1;
			PF_Pixel16 c0 = scanline[y1];
			PF_Pixel16 c1 = scanline[y2];
			PF_Pixel16 c2;
			if (y1 == y2) {
				c2 = c0;
			}
			else {
				c2.red = (A_u_short)(((long)c0.red + (long)c1.red) / 2);
				c2.green = (A_u_short)(((long)c0.green + (long)c1.green) / 2);
				c2.blue = (A_u_short)(((long)c0.blue + (long)c1.blue) / 2);
				c2.alpha = (A_u_short)(((long)c0.alpha + (long)c1.alpha) / 2);
			}
			data[x + ypos] = c0;
			ypos += owt;
			if (y * 2 + 1 < oh) {
				data[x + ypos] = c2;
			}
			ypos += owt;

		}

	}

	return err;
}
//==================================================================================
PF_Err RevRedChannel16(CFsGraph *g)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16* data = (PF_Pixel16*)g->data();
	A_long wt = g->widthTrue();
	A_long w = g->width() / 2;
	A_long h = g->height() / 2;

	A_long ypos = 0;
	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			A_long xx = x + ypos;
			data[xx].red = PF_MAX_CHAN16 - data[xx].red;
			//data[xx].green = PF_MAX_CHAN8 - data[xx].green;
			//data[xx].blue = PF_MAX_CHAN8 - data[xx].blue;
			//data[xx].alpha = PF_MAX_CHAN8 - data[xx].alpha;
		}
		ypos += wt;
	}

	return err;
}
//==================================================================================
PF_Err MaskedApha16(CFsGraph *g)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16* data = (PF_Pixel16*)g->data();
	A_long wt = g->widthTrue();
	A_long w = g->width() / 2;
	A_long h = g->height() / 2;

	A_long ypos = 0;
	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			A_long xx = x + ypos;
			A_long a = PF_MAX_CHAN16- data[xx].green;
			data[xx].red = RoundShort( data[xx].red - a);
		}
		ypos += wt;
	}

	return err;
}
//==================================================================================
static PF_Err EqualRedChannel(CFsGraph *g)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16* data = (PF_Pixel16*)g->data();
	A_long wt = g->widthTrue();
	A_long w = g->width() / 2;
	A_long h = g->height() / 2;

	A_long ypos = 0;
	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			A_long xx = x + ypos;
			data[xx].green = data[xx].red;
			data[xx].blue = data[xx].red;
			data[xx].alpha = data[xx].red;
		}
		ypos += wt;
	}

	return err;
}
static PF_Err Colored(CFsGraph *g, FlareInfo16 * infoP)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16*data = (PF_Pixel16*)g->data();
	A_long wt = g->widthTrue();
	A_long w = g->width() / 2;
	A_long h = g->height() / 2;

	A_long ypos = 0;
	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			A_long xx = x + ypos;
			A_u_short a = data[xx].red;
			double aa = (double)a / PF_MAX_CHAN16;
			data[xx].red = RoundShortFpLong((double)infoP->color.red *aa);
			data[xx].green = RoundShortFpLong((double)infoP->color.green *aa);
			data[xx].blue = RoundShortFpLong((double)infoP->color.blue *aa);
			data[xx].alpha = a;
		}
		ypos += wt;
	}

	return err;
}
//==================================================================================
PF_Err RedToGreen16(CFsGraph *g)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16* data = (PF_Pixel16*)g->data();
	A_long wt = g->widthTrue();
	A_long w = g->width() / 2;
	A_long h = g->height() / 2;

	A_long ypos = 0;
	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			A_long xx = x + ypos;
			A_u_short c = data[xx].green;
			data[xx].green = data[xx].red;
			data[xx].red = c;
		}
		ypos += wt;
	}

	return err;
}

//==================================================================================
static PF_Err
BaseCopyImage(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP = reinterpret_cast<ParamInfo16*>(refcon);
	
	double a = (double)inP->alpha / PF_MAX_CHAN16;
	a *= infoP->BaseOpacity;
	outP->red = RoundShortFpLong((double)inP->red*a);
	outP->green = RoundShortFpLong((double)inP->green*a);
	outP->blue = RoundShortFpLong((double)inP->blue*a);
	outP->alpha = RoundShortFpLong((double)PF_MAX_CHAN16*a);

	return err;
}
//==================================================================================
static PF_Err
BaseCopyWhiteBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP = reinterpret_cast<ParamInfo16*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;
		return err;
	}
	double a = (double)inP->alpha / (double)PF_MAX_CHAN16;
	a *= infoP->BaseOpacity;
	if (a > 1) a = 1;
	else if (a < 0) a = 0;

	A_long rgb = ((A_long)inP->red + (A_long)inP->green + (A_long)inP->blue) / 3;
	//反転
	rgb = (PF_MAX_CHAN16 - rgb);
	if (rgb < 0) rgb = 0;
	else if (rgb > PF_MAX_CHAN16) rgb = PF_MAX_CHAN16;

	if (a >= 1)
	{
		outP->red = (A_u_short)(rgb * infoP->BaseColor.red / PF_MAX_CHAN16);
		outP->green = (A_u_short)(rgb * infoP->BaseColor.green / PF_MAX_CHAN16);
		outP->blue = (A_u_short)(rgb * infoP->BaseColor.blue / PF_MAX_CHAN16);
		outP->alpha = (A_u_short)rgb;
	}
	else {
		double v = (double)rgb*a;
		outP->red = RoundShortFpLong(v * (double)infoP->BaseColor.red / PF_MAX_CHAN16);
		outP->green = RoundShortFpLong(v * (double)infoP->BaseColor.green / PF_MAX_CHAN16);
		outP->blue = RoundShortFpLong(v * (double)infoP->BaseColor.blue / PF_MAX_CHAN16);
		outP->alpha = RoundShortFpLong(v);
	}
	return err;

}

static
PF_Err
BaseCopyBlackBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP = reinterpret_cast<ParamInfo16*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;
		return err;
	}
	double a = (double)inP->alpha / (double)PF_MAX_CHAN16;
	a *= infoP->BaseOpacity;
	if (a > 1) a = 1;
	else if (a < 0) a = 0;

	A_long rgb = (inP->red + inP->green + inP->blue) / 3;
	if (rgb < 0) rgb = 0;
	else if (rgb > PF_MAX_CHAN16) rgb = PF_MAX_CHAN16;

	if (a >= 1)
	{
		outP->red = (A_u_short)(rgb * infoP->BaseColor.red / PF_MAX_CHAN16);
		outP->green = (A_u_short)(rgb * infoP->BaseColor.green / PF_MAX_CHAN16);
		outP->blue = (A_u_short)(rgb * infoP->BaseColor.blue / PF_MAX_CHAN16);
		outP->alpha = (A_u_short)rgb;
	}
	else {
		double v = (double)rgb*a;
		outP->red = RoundShortFpLong(v * (double)infoP->BaseColor.red / PF_MAX_CHAN16);
		outP->green = RoundShortFpLong(v * (double)infoP->BaseColor.green / PF_MAX_CHAN16);
		outP->blue = RoundShortFpLong(v * (double)infoP->BaseColor.blue / PF_MAX_CHAN16);
		outP->alpha = RoundShortFpLong(v);
	}
	return err;
}
static PF_Err
BaseCopyAlphaBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP = reinterpret_cast<ParamInfo16*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;
	}
	else if ((inP->alpha >= PF_MAX_CHAN16) && (infoP->BaseOpacity >= 1))
	{
		PF_Pixel16 r2 = { PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16 };
		*outP = infoP->BaseColor;

	}
	else {
		double a =((double)inP->alpha/ PF_MAX_CHAN16) * infoP->BaseOpacity;

		outP->red = RoundShortFpLong((double)infoP->BaseColor.red *a);
		outP->green = RoundShortFpLong((double)infoP->BaseColor.green *a);
		outP->blue = RoundShortFpLong((double)infoP->BaseColor.blue *a);
		outP->alpha = RoundShortFpLong((double)PF_MAX_CHAN16 *a);

	}



	return err;
}
//==================================================================================
static PF_Err
WhiteBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;
		return err;
	}
	double a = (double)inP->alpha / (double)PF_MAX_CHAN16;
	a *= infoP->opacitry;
	if (a > 1) a = 1;
	else if (a < 0) a = 0;

	A_long rgb = ((A_long)inP->red + (A_long)inP->green + (A_long)inP->blue) / 3;
	//反転
	rgb = (PF_MAX_CHAN16 - rgb);
	if (rgb < 0) rgb = 0;
	else if (rgb > PF_MAX_CHAN16) rgb = PF_MAX_CHAN16;

	if (a >= 1)
	{
		outP->red = (A_u_short)rgb;
		outP->green = (A_u_short)rgb;
		outP->blue = (A_u_short)rgb;
		outP->alpha = (A_u_short)rgb;
	}
	else {
		A_u_short v = RoundShortFpLong((double)rgb*a);
		outP->red = v;
		outP->green = v;
		outP->blue = v;
		outP->alpha = v;
	}
	return err;
}

//==================================================================================
static
PF_Err
BlackBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;
		return err;
	}
	double a = (double)inP->alpha / (double)PF_MAX_CHAN16;
	a *= infoP->opacitry;
	if (a > 1) a = 1;
	else if (a < 0) a = 0;

	A_long rgb = (inP->red + inP->green + inP->blue) / 3;
	if (rgb < 0) rgb = 0;
	else if (rgb > PF_MAX_CHAN16) rgb = PF_MAX_CHAN16;

	if (a >= 1)
	{
		outP->red = (A_u_short)rgb;
		outP->green = (A_u_short)rgb;
		outP->blue = (A_u_short)rgb;
		outP->alpha = (A_u_short)rgb;
	}
	else {
		A_u_short v = RoundShortFpLong((double)rgb*a);
		outP->red = v;
		outP->green = v;
		outP->blue = v;
		outP->alpha = v;
	}
	return err;
}
static PF_Err
AlphaBase(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);

	if (inP->alpha <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;
	}
	else if ((inP->alpha >= PF_MAX_CHAN16) && (infoP->opacitry >= 1))
	{
		PF_Pixel16 r2 = { PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16 };
		*outP = r2;

	}
	else {
		A_u_short a = RoundShortFpLong((double)inP->alpha *infoP->opacitry);

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
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);

	double a = (double)inP->alpha / PF_MAX_CHAN16;
	a *= infoP->opacitry;
	if (a <= 0)
	{
		PF_Pixel16 r = { 0,0,0,0 };
		*outP = r;

	}
	else {
		outP->red = RoundShortFpLong(inP->red * a);
		outP->green = RoundShortFpLong(inP->green * a);
		outP->blue = RoundShortFpLong(inP->blue * a);
		outP->alpha = RoundShortFpLong(inP->alpha * a);
	}


	return err;
}


//==================================================================================
static PF_Err
BlendNormal(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);
	
	double a = inP->alpha * infoP->opacitry;
	if (a == 0) return err;
	if (a >= PF_MAX_CHAN16) {
		*outP = *inP;
	}else {
		double r = inP->red *infoP->opacitry;
		double g = inP->green *infoP->opacitry;
		double b = inP->blue *infoP->opacitry;

		outP->alpha = RoundShortFpLong(outP->alpha*(1 - a / PF_MAX_CHAN16) + a);
		outP->red = RoundShortFpLong(outP->red*(1 - a / PF_MAX_CHAN16) + r);
		outP->green = RoundShortFpLong(outP->green*(1 - a / PF_MAX_CHAN16) + g);
		outP->blue = RoundShortFpLong(outP->blue*(1 - a / PF_MAX_CHAN16) + b);

	}


	
	return err;
}
//==================================================================================
static PF_Err
BlendScreen(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);
	outP->red = RoundShort((A_long)inP->red + (A_long)outP->red - ((A_long)inP->red * (A_long)outP->red) / PF_MAX_CHAN16);
	outP->green = RoundShort((A_long)inP->green + (A_long)outP->green - ((A_long)inP->green * (A_long)outP->green) / PF_MAX_CHAN16);
	outP->blue = RoundShort((A_long)inP->blue + (A_long)outP->blue - ((A_long)inP->blue * (A_long)outP->blue) / PF_MAX_CHAN16);
	outP->alpha = RoundShort((A_long)inP->alpha + (A_long)outP->alpha - ((A_long)inP->alpha * (A_long)outP->alpha) / PF_MAX_CHAN16);

	return err;
}
//==================================================================================
static PF_Err
BlendAdd(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FlareInfo16 *	infoP = reinterpret_cast<FlareInfo16*>(refcon);
	outP->red = RoundShort((A_long)inP->red + (A_long)outP->red);
	outP->green = RoundShort((A_long)inP->green + (A_long)outP->green);
	outP->blue = RoundShort((A_long)inP->blue + (A_long)outP->blue);
	outP->alpha = RoundShort((A_long)inP->alpha + (A_long)outP->alpha);

	return err;
}

//==================================================================================
/*
Matを元に戻す
*/
static PF_Err fromBlackMat16(CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;
	A_long target = 0;
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)ae->output->data;
	A_long w = ae->out->width();
	A_long offset = ae->out->offsetWidth();
	A_long h = ae->out->height();


	for (A_long j = 0; j < h; j++) {
		for (A_long i = 0; i < w; i++) {
			PF_Pixel16 p = data[target];
			if (p.alpha < PF_MAX_CHAN16) {
				if (p.alpha <= 0) {
					p.blue = p.green = p.red = 0;
				}
				else if (p.alpha < PF_MAX_CHAN16) {
					p.blue = RoundShort((A_long)p.blue * PF_MAX_CHAN16 / (A_long)p.alpha);
					p.green = RoundShort((A_long)p.green * PF_MAX_CHAN16 / (A_long)p.alpha);
					p.red = RoundShort((A_long)p.red * PF_MAX_CHAN16 / (A_long)p.alpha);
				}
				data[target] = p;
			}
			target++;
		}
		target += offset;
	}
	return err;
}
//==================================================================================
PF_Err Exec16(CFsAE *ae, ParamInfo16 *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData *in_data;
	in_data = ae->in_data;

	ERR(ae->NewTmpWorld());
	ERR(ae->out->clear());
	ERR(ae->tmp->clear());
	
	A_long w = ae->out->width();
	if (w < ae->out->height())w = ae->out->height();

	PF_Handle bufH = PF_NEW_HANDLE(sizeof(PF_Pixel16)*w*2);
	if (bufH == NULL) return PF_Err_OUT_OF_MEMORY;

	if (infoP->BaseOn == TRUE) {
		switch (infoP->mode)
		{
		case MODE::Image: // image
			ERR(ae->iterate16((refconType)infoP, BaseCopyImage));
			break;
		case MODE::Alpha: // alpha
			ERR(ae->iterate16((refconType)infoP, BaseCopyAlphaBase));
			break;
		case MODE::BlackbasedMask: // blackbase
			ERR(ae->iterate16((refconType)infoP, BaseCopyBlackBase));
			break;
		case MODE::WhitebasedMask: // whitebase
		default:
			ERR(ae->iterate16((refconType)infoP, BaseCopyWhiteBase));
			break;
		}
	}


	//本番
	for (int i = 0; i < PCOUNT; i++)
	{
		ae->tmp->clear();
		if (infoP->flareInfo[i].enabled == FALSE) continue;
		if (infoP->flareInfo[i].opacitry <=0) continue;
	
		infoP->flareInfo[i].bufH = bufH;

		switch (infoP->mode)
		{
		case MODE::Image: // image
			ERR(ae->iterate16InputToTemp((refconType)&infoP->flareInfo[i], ImageBase));
			break;
		case MODE::Alpha: // alpha
			ERR(ae->iterate16InputToTemp((refconType)&infoP->flareInfo[i], AlphaBase));
			break;
		case MODE::BlackbasedMask: // blackbase
			ERR(ae->iterate16InputToTemp((refconType)&infoP->flareInfo[i], BlackBase));
			break;
		case MODE::WhitebasedMask: // whitebase
		default:
			ERR(ae->iterate16InputToTemp((refconType)&infoP->flareInfo[i], WhiteBase));
			break;
		}
		//画像を半分に
		ERR(ToHarfSize(ae->tmp));

		if ((infoP->flareInfo[i].rev != REVMODE::None) && (infoP->mode != MODE::Image))
		{
			ERR(RevRedChannel16(ae->tmp));
		}

		if ((infoP->flareInfo[i].border == TRUE)&& (infoP->mode != MODE::Image)&& (infoP->flareInfo[i].rev == REVMODE::None))
		{
			ERR(Border16(ae, &infoP->flareInfo[i]));
		}

		//maxの処理
		if (infoP->mode == MODE::Image) {
			ERR(MaxAll16(ae, &infoP->flareInfo[i]));
			ERR(BlurAll16(ae, &infoP->flareInfo[i]));

		}
		else {
			ERR(MaxRed16(ae, &infoP->flareInfo[i]));
			ERR(BlurRed16(ae, &infoP->flareInfo[i]));
			if ((infoP->flareInfo[i].rev == REVMODE::ReverseAndOriginalAlpha))
			{
				ERR(MaskedApha16(ae->tmp));
			}
			ERR(Colored(ae->tmp, &infoP->flareInfo[i]));
		}
		//画像サイズを元に戻す
		ERR(ToDoubleSize(ae->tmp,bufH));

		//outputへ戻す
		switch (infoP->flareInfo[i].blend)
		{
		case BLEND::Screen:
			ERR(ae->iterate16TempToOutput((refconType)&infoP->flareInfo[i], BlendScreen));
			break;
		case BLEND::Add:
			ERR(ae->iterate16TempToOutput((refconType)&infoP->flareInfo[i], BlendAdd));
			break;
		case BLEND::Normal:
		default:
			ERR(ae->iterate16TempToOutput((refconType)&infoP->flareInfo[i], BlendNormal));
			break;
		}
	}

	ERR(fromBlackMat16(ae));
	
	if (bufH != NULL)
	{
		PF_DISPOSE_HANDLE(bufH);
	}

	
	return err;
}
 