#include "Star_Colorful.h"



//-------------------------------------------------------------------------------------------------
//画像の色を獲得
inline PF_Pixel16 GetPX(GInfo *gi, A_long x, A_long y)
{
	PF_Pixel16 *data = (PF_Pixel16 *)gi->data;

	A_long xx = x;
	if (xx < 0) xx = 0; else if (xx >= gi->width) xx = gi->width - 1;
	A_long yy = y;
	if (yy < 0) yy = 0; else if (yy >= gi->height) xx = gi->height - 1;
	return data[xx + yy * gi->widthTrue];

}

//-------------------------------------------------------------------------------------------------
//スクリーン合成の基本部分
inline A_u_short scr(A_u_short s, A_u_short d)
{
	A_long v = (A_long)d + (A_long)s - (A_long)s * (A_long)d / PF_MAX_CHAN16;
	if (v < 0) v = 0; else if (v > PF_MAX_CHAN16) v = PF_MAX_CHAN16;
	return (A_u_short)v;
}
//-------------------------------------------------------------------------------------------------
// ピクセルのスクリーン合成
inline void AddPxL(GInfo *gi, A_long xL, A_long yL, PF_Pixel16 p)
{
	if ((xL < 0) || (xL >= gi->width) || (yL < 0) || (yL >= gi->height)) {
		return;
	}

	PF_Pixel16 *data = (PF_Pixel16 *)gi->data;

	A_long pos = xL + yL*gi->widthTrue;
	data[pos].red = scr(data[pos].red ,p.red);
	data[pos].green = scr(data[pos].green ,p.green);
	data[pos].blue = scr(data[pos].blue, p.blue);
	data[pos].alpha = scr(data[pos].alpha , p.alpha);

}
//-------------------------------------------------------------------------------------------------
// ピクセルの濃度調整
inline PF_Pixel16 PxPer(PF_Pixel16 p, PF_FpLong per)
{
	PF_Pixel16 r;
	r.red = RoundShortFpLong( (PF_FpLong)p.red * per);
	r.green = RoundShortFpLong((PF_FpLong)p.green * per);
	r.blue = RoundShortFpLong((PF_FpLong)p.blue * per);
	r.alpha = RoundShortFpLong((PF_FpLong)p.alpha * per);
	return r;
}
//-------------------------------------------------------------------------------------------------
//小数点対応のスクリーン合成
inline void AddPxD(GInfo *gi, PF_Pixel16 p)
{



	A_long x0, x1, y0, y1;
	x0 = (A_long)gi->xD; x1 = x0 + 1;
	y0 = (A_long)gi->yD; y1 = y0 + 1;

	PF_FpLong ax1 = gi->xD - (PF_FpLong)x0;
	PF_FpLong ay1 = gi->yD - (PF_FpLong)y0;
	PF_FpLong ax0 = 1.0 - ax1;
	PF_FpLong ay0 = 1.0 - ay1;

	AddPxL(gi, x0, y0, PxPer(p, ax0 * ay0));
	AddPxL(gi, x0, y1, PxPer(p, ax0 * ay1));
	AddPxL(gi, x1, y0, PxPer(p, ax1 * ay0));
	AddPxL(gi, x1, y1, PxPer(p, ax1 * ay1));

}
//-------------------------------------------------------------------------------------------------
static PF_Err
BlendScreen(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	outP->alpha = scr(outP->alpha, inP->alpha);

	PF_FpLong a = (PF_FpLong)inP->alpha/PF_MAX_CHAN16;

	outP->red = scr(outP->red, (A_u_short)((PF_FpLong)inP->red*a));
	outP->green = scr(outP->green, (A_u_short)((PF_FpLong)inP->green*a));
	outP->blue = scr(outP->blue, (A_u_short)((PF_FpLong)inP->blue*a));

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err
BlendAdd(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	outP->alpha = scr(outP->alpha, inP->alpha);

	PF_FpLong a = (PF_FpLong)inP->alpha / PF_MAX_CHAN16;

	outP->red = RoundShort((A_long)outP->red + (A_long)((PF_FpLong)inP->red*a));
	outP->green = RoundShort((A_long)outP->green + (A_long)((PF_FpLong)inP->green*a));
	outP->blue = RoundShort((A_long)outP->blue + (A_long)((PF_FpLong)inP->blue*a));

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err
BlendNormal(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	PF_FpLong ar = 1 - (PF_FpLong)outP->alpha/ PF_MAX_CHAN16;
	outP->alpha = scr(outP->alpha, inP->alpha);

	PF_FpLong a = (PF_FpLong)inP->alpha / PF_MAX_CHAN16;
	PF_FpLong r, g, b;
	r = ((PF_FpLong)inP->red*a)*ar;
	g = ((PF_FpLong)inP->green*a)*ar;
	b = ((PF_FpLong)inP->blue*a)*ar;

	outP->red = RoundShortFpLong((PF_FpLong)outP->red + r );
	outP->green = RoundShortFpLong((PF_FpLong)outP->green + g);
	outP->blue = RoundShortFpLong((PF_FpLong)outP->blue + b);

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err
TargetPixel(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP = reinterpret_cast<ParamInfo16*>(refcon);


	PF_Pixel16 c = { PF_MAX_CHAN16,PF_MAX_CHAN16, PF_MAX_CHAN16, PF_MAX_CHAN16 };
	PF_FpShort  r, g, b, a;
	PF_FpLong v = 0;
	a = (PF_FpShort)inP->alpha / PF_MAX_CHAN16;
	r = (PF_FpShort)inP->red * a / PF_MAX_CHAN16;
	g = (PF_FpShort)inP->green * a / PF_MAX_CHAN16;
	b = (PF_FpShort)inP->blue * a / PF_MAX_CHAN16;


	PF_InData		*in_data = infoP->gout.in_data;
	PF_PixelFloat pf;

	switch (infoP->info.target_kind)
	{
	case 2:
		v = (r + g + b) / 3;
		break;
	case 3:
		v = (0.29891 * r) + (0.58661 * g) + (0.11448 * b);
		break;
	case 4:
		pf.red = r;
		pf.green = g;
		pf.blue = b;
		pf.alpha = a;

		LABA lab = RgbToLab(pf);
		//色の距離測定
		v = 1.0 - (PF_SQRT(
			PF_POW(lab.L - infoP->taget_lab.L, 2)
			+ PF_POW(lab.A - infoP->taget_lab.A, 2)
			+ PF_POW(lab.B - infoP->taget_lab.B, 2))
			/ PF_SQRT(3)
			);
		break;
	case 1:
	default:
		v = MAX(MAX(r, g), b);
		break;
	}
	//------------
	if (infoP->target_softness == 0) {
		if (v >= infoP->target_border) {
			v = 1.0;
		}
		else {
			v = 0;
		}
	}
	else {
		if (v <= infoP->target_start) {
			v = 0;
		}
		else if (v >= infoP->target_border) {
			v = 1.0;
		}
		else {
			v = (v - infoP->target_start) / (infoP->target_softness);
		}

	}

	a = (PF_FpShort)v;
	if (a < 0)a = 0; else if (a > 1) a = 1;

	c.alpha = RoundShort((A_long)(a*PF_MAX_CHAN16 + 0.5));

	c.red = RoundShort((A_long)((double)inP->red * a+0.5));
	c.green = RoundShort((A_long)((double)inP->green * a + 0.5));
	c.blue = RoundShort((A_long)((double)inP->blue * a + 0.5));


	*outP = c;

	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err
CopyAtoTemp(ParamInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16 * dataO = (PF_Pixel16 *)infoP->gout.data;
	PF_Pixel16 * dataT = (PF_Pixel16 *)infoP->gtmp.data;

	A_long horO = 0;
	A_long horT = 0;
	for (A_long y = 0; y < infoP->gout.height; y++)
	{
		horO = y * infoP->gout.widthTrue;
		horT = y * infoP->gtmp.widthTrue;
		for (A_long x = 0; x < infoP->gout.width; x++)
		{
			A_u_short a = dataO[x + horO].alpha;
			dataT[x + horT].alpha = PF_MAX_CHAN16;
			dataT[x + horT].red = a;
			dataT[x + horT].green = a;
			dataT[x + horT].blue = a;
		}
	}


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TempRev(ParamInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gi = infoP->gtmp;

	PF_Pixel16 * data = (PF_Pixel16 *)gi.data;

	A_long hor = 0;
	for (A_long y = 0; y < gi.height; y++)
	{
		for (A_long x = 0; x < gi.width; x++)
		{
			A_long pos = x + hor;
			data[pos].red = PF_MAX_CHAN16 - data[pos].red;
			data[pos].green = PF_MAX_CHAN16 - data[pos].green;
		}
		hor += gi.widthTrue;
	}


	return err;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
static PF_Err
TempSub(ParamInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	GInfo gi = infoP->gtmp;

	PF_Pixel16 * data = (PF_Pixel16 *)gi.data;

	A_long hor = 0, pos = 0;

	for (A_long y = 0; y < gi.height; y++)
	{
		for (A_long x = 0; x < gi.width; x++)
		{
			pos = x + hor;
			data[pos].green = PF_MAX_CHAN16 - data[pos].green;
			data[pos].red = RoundShort(data[pos].blue - data[pos].green);
		}
		hor += gi.widthTrue;
	}


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxHor(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gi = pi->gtmp;

	PF_Pixel16 *data = (PF_Pixel16 *)gi.data;
	A_long mx1 = 0;
	A_long mx2 = 0;

	A_long org,pos;
	A_long hor=0;
	for (A_long y = 0; y < gi.height; y++)
	{
		for (A_long x = 0; x < gi.width; x++) 
		{
			pos = x + hor;
			org = data[pos].red;
			mx1 = mx2 = 0;
			if (x > 0) {
				mx1 = data[pos -1].red;
			}
			if (x < gi.width-1) {
				mx2 = data[pos + 1].red;
			}
			if (mx1 < mx2) {
				mx1 = mx2;
			}
			if (org < mx1) {
				org += mx1/2;
				if (org > mx1) org = mx1;
				data[pos].green = RoundShort(org);
			}

		}
		//redに戻す
		for (A_long x = 0; x < gi.width; x++)
		{
			pos = x + hor;
			data[pos].red = data[pos].green;
		}
		hor += gi.widthTrue;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxVer(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gi = pi->gtmp;

	PF_Pixel16 *data = (PF_Pixel16 *)gi.data;
	A_long mx1 = 0;
	A_long mx2 = 0;

	A_long org, pos;
	A_long hor = 0;
	for (A_long x = 0; x < gi.width; x++) 
	{
		hor = 0;
		for (A_long y = 0; y < gi.height; y++)
		{ 
			pos = x + hor;
			org = data[pos].red;
			mx1 = mx2 = 0;
			if (y > 0) {
				mx1 = data[pos - gi.widthTrue].red;
			}
			if (y < gi.height - 1) {
				mx2 = data[pos + gi.widthTrue].red;
			}
			if (mx1 < mx2) {
				mx1 = mx2;
			}
			if (org < mx1) {
				org += mx1 / 2;
				if (org > mx1) org = mx1;
				data[pos].green = RoundShort(org);
			}
			hor += gi.widthTrue;

		}
		//redに戻す
		hor = 0;
		for (A_long y = 0; y < gi.height; y++)
		{
			pos = x + hor;
			data[pos].red = data[pos].green;
			hor += gi.widthTrue;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static void
MakePixelTable(PF_Pixel16 *tbl, A_long len, PF_Pixel16 col, PF_FpLong per, PF_FpLong br)
{

	if ((len <= 0) || (per <= 0))
	{
		return;
	}


	PF_FpLong p = per;
	for (A_long i = 0; i < len; i++)
	{
		p = per * (1 - (PF_FpLong)i / (PF_FpLong)len)*0.5;
		tbl[i] = PxPer(col, p);
	}

	A_long len2 = len / 2;
	if ((len2 <= 0) || (br <= 0)) return;
	for (A_long i = 0; i < len2; i++)
	{
		A_long r, g, b;
		r = PF_MAX_CHAN16 - tbl[i].red;
		g = PF_MAX_CHAN16 - tbl[i].green;
		b = PF_MAX_CHAN16 - tbl[i].blue;

		tbl[i].red = RoundShort(tbl[i].red + (A_long)((PF_FpLong)r * br * (len2 - i) / len2));
		tbl[i].green = RoundShort(tbl[i].green + (A_long)((PF_FpLong)g * br * (len2 - i) / len2));
		tbl[i].blue = RoundShort(tbl[i].blue + (A_long)((PF_FpLong)b * br * (len2 - i) / len2));

	}

}
//-------------------------------------------------------------------------------------------------
static PF_Err StarDrawColor(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gt = pi->gtmp;
	GInfo go = pi->gout;
	GInfo go2 = pi->gout;
	GInfo go3 = pi->gout;
	GInfo go4 = pi->gout;

	PF_Pixel16 *dataT = (PF_Pixel16 *)gt.data;

	MakePixelTable(pi->horTable, pi->hor.len, pi->color, pi->hor.opa, pi->info.brigthness);
	MakePixelTable(pi->verTable, pi->ver.len, pi->color, pi->ver.opa, pi->info.brigthness);
	MakePixelTable(pi->diaTable, pi->dia.len, pi->color, pi->dia.opa, pi->info.brigthness);


	A_long posT = 0;
	A_long lv = 0;
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			lv = dataT[posT].red;
			posT++;
			if (lv <= 0) continue;

			//垂直
			if ((pi->ver.len > 0) && (pi->ver.opa > 0))
			{
				A_long vl = pi->ver.len * lv / PF_MAX_CHAN16;
				if (vl > 0)
				{
					A_long d = pi->ver.len - vl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->ver.len; i++)
					{
						AddPxD(&go, pi->verTable[i]);
						AddPxD(&go2, pi->verTable[i]);
						go.xD += pi->ver.addX;
						go.yD += pi->ver.addY;
						go2.xD -= pi->ver.addX;
						go2.yD -= pi->ver.addY;
					}
				}

			}
			//水平
			if ((pi->hor.len > 0) && (pi->hor.opa > 0))
			{
				A_long hl = pi->hor.len * lv / PF_MAX_CHAN16;
				if (hl > 0)
				{
					A_long d = pi->hor.len - hl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->hor.len; i++)
					{
						AddPxD(&go, pi->horTable[i]);
						AddPxD(&go2, pi->horTable[i]);
						go.xD += pi->hor.addX;
						go.yD += pi->hor.addY;
						go2.xD -= pi->hor.addX;
						go2.yD -= pi->hor.addY;
					}
				}
			}
			//斜め
			if ((pi->dia.len > 0) && (pi->dia.opa > 0))
			{
				A_long dl = pi->dia.len * lv / PF_MAX_CHAN16;
				if (dl > 0)
				{
					A_long d = pi->dia.len - dl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					go3.xD = (PF_FpLong)x;
					go3.yD = (PF_FpLong)y;
					go4.xD = (PF_FpLong)x;
					go4.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->dia.len; i++)
					{
						AddPxD(&go, pi->diaTable[i]);
						AddPxD(&go2, pi->diaTable[i]);
						AddPxD(&go3, pi->diaTable[i]);
						AddPxD(&go4, pi->diaTable[i]);
						go.xD += pi->dia.addX;
						go.yD += pi->dia.addY;
						go2.xD -= pi->dia.addX;
						go2.yD -= pi->dia.addY;
						go3.xD += pi->dia2.addX;
						go3.yD += pi->dia2.addY;
						go4.xD -= pi->dia2.addX;
						go4.yD -= pi->dia2.addY;
					}
				}
			}

		}
		posT += gt.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err CenterDraw(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gt = pi->gtmp;
	GInfo go = pi->gout;

	PF_Pixel16 *dataO = (PF_Pixel16 *)go.data;
	PF_Pixel16 *dataT = (PF_Pixel16 *)gt.data;


	A_long posO = 0;
	A_long posT = 0;
	A_long lv = 0;

	PF_FpLong r = pi->color.red;
	PF_FpLong g = pi->color.green;
	PF_FpLong b = pi->color.blue;
	r = (r + (PF_MAX_CHAN16 - r)*pi->info.brigthness);
	g = (g + (PF_MAX_CHAN16 - g)*pi->info.brigthness);
	b = (b + (PF_MAX_CHAN16 - b)*pi->info.brigthness);
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			PF_FpLong v = (PF_FpLong)dataT[posT].green / PF_MAX_CHAN16;
			dataO[posO].red = RoundShortFpLong(dataO[posO].red + r * v);
			dataO[posO].green = RoundShortFpLong(dataO[posO].green + g * v);
			dataO[posO].blue = RoundShortFpLong(dataO[posO].blue + b * v);
			dataO[posO].alpha = RoundShortFpLong( (A_long)dataO[posO].alpha + (A_long)dataT[posT].green);

			posO++;
			posT++;

		}
		posT += gt.offsetWidth;
		posO += go.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err blur(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo go = pi->gout;

	PF_Pixel16 *data = (PF_Pixel16 *)go.data;

	A_long pos = 0;
	A_long lv = 0;
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			PF_FpLong r = 0, g = 0, b = 0, a = 0;
			PF_Pixel16 c;
			c = GetPX(&go, x - 1, y - 1);
			r += (PF_FpLong)c.red /32;
			g += (PF_FpLong)c.green/32;
			b += (PF_FpLong)c.blue / 32;
			a += (PF_FpLong)c.alpha / 32;

			c = GetPX(&go, x, y - 1);
			r += (PF_FpLong)c.red / 8;
			g += (PF_FpLong)c.green / 8;
			b += (PF_FpLong)c.blue / 8;
			a += (PF_FpLong)c.alpha / 8;

			c = GetPX(&go, x+1, y - 1);
			r += (PF_FpLong)c.red / 32;
			g += (PF_FpLong)c.green / 32;
			b += (PF_FpLong)c.blue / 32;
			a += (PF_FpLong)c.alpha / 32;

			c = GetPX(&go, x - 1, y);
			r += (PF_FpLong)c.red / 8;
			g += (PF_FpLong)c.green / 8;
			b += (PF_FpLong)c.blue / 8;
			a += (PF_FpLong)c.alpha / 8;

			//c = GetPX(&go, x, y);
			c = data[pos];
			r += (PF_FpLong)c.red * 4 / 8;
			g += (PF_FpLong)c.green * 4 / 8;
			b += (PF_FpLong)c.blue * 4 / 8;
			a += (PF_FpLong)c.alpha * 4 / 8;
			/*r += (PF_FpLong)c.red * 3 / 8;
			g += (PF_FpLong)c.green * 3 / 8;
			b += (PF_FpLong)c.blue * 3 / 8;
			a += (PF_FpLong)c.alpha * 3 / 8;*/

			c = GetPX(&go, x+1, y);
			r += (PF_FpLong)c.red / 8;
			g += (PF_FpLong)c.green / 8;
			b += (PF_FpLong)c.blue / 8;
			a += (PF_FpLong)c.alpha / 8;

			c = GetPX(&go, x - 1, y+1);
			r += (PF_FpLong)c.red / 32;
			g += (PF_FpLong)c.green / 32;
			b += (PF_FpLong)c.blue / 32;
			a += (PF_FpLong)c.alpha / 32;

			c = GetPX(&go, x - 0, y + 1);
			r += (PF_FpLong)c.red / 8;
			g += (PF_FpLong)c.green / 8;
			b += (PF_FpLong)c.blue / 8;
			a += (PF_FpLong)c.alpha / 8;

			c = GetPX(&go, x + 1, y + 1);
			r += (PF_FpLong)c.red / 32;
			g += (PF_FpLong)c.green / 32;
			b += (PF_FpLong)c.blue / 32;
			a += (PF_FpLong)c.alpha / 32;

			c.red = RoundShortFpLong(r);
			c.green = RoundShortFpLong(g);
			c.blue = RoundShortFpLong(b);
			c.alpha = RoundShortFpLong(a);

			data[pos] = c;
			pos++;

		}
		pos += go.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err blurTempRed(ParamInfo16 *pi)
{
#define DEC1 16
#define DEC2 32
	PF_Err			err = PF_Err_NONE;

	GInfo gt = pi->gtmp;

	PF_Pixel16 *data = (PF_Pixel16 *)gt.data;

	A_long pos = 0;
	A_long lv = 0;
	for (A_long y = 0; y < gt.height; y++)
	{
		for (A_long x = 0; x < gt.width; x++)
		{
			PF_FpLong r = 0;
			PF_Pixel16 c;
			c = GetPX(&gt, x - 1, y - 1);
			r += (PF_FpLong)c.red / DEC2;

			c = GetPX(&gt, x, y - 1);
			r += (PF_FpLong)c.red / DEC1;

			c = GetPX(&gt, x + 1, y - 1);
			r += (PF_FpLong)c.red / DEC2;

			c = GetPX(&gt, x - 1, y);
			r += (PF_FpLong)c.red / DEC1;

			//c = GetPX(&go, x, y);
			c = data[pos];
			r += (PF_FpLong)c.red*2/3;

			c = GetPX(&gt, x + 1, y);
			r += (PF_FpLong)c.red / DEC1;

			c = GetPX(&gt, x - 1, y + 1);
			r += (PF_FpLong)c.red / DEC2;

			c = GetPX(&gt, x - 0, y + 1);
			r += (PF_FpLong)c.red / DEC1;

			c = GetPX(&gt, x + 1, y + 1);
			r += (PF_FpLong)c.red / DEC2;


			data[pos].red = RoundShortFpLong(r);
			pos++;

		}
		pos += gt.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err ScreenTemp(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gi = pi->gin;
	GInfo gt = pi->gtmp;

	PF_Pixel16 *dataI = (PF_Pixel16 *)gi.data;
	PF_Pixel16 *dataT = (PF_Pixel16 *)gt.data;

	A_long posI = 0;
	A_long posT = 0;
	for (A_long y = 0; y < gt.height; y++)
	{
		for (A_long x = 0; x < gt.width; x++)
		{
			PF_Pixel16 pt = dataT[posT];

			dataT[posT] = dataI[posI];
			dataT[posT].alpha = pt.red;
			posI++;
			posT++;

		}
		posI += gi.offsetWidth;
		posT += gt.offsetWidth;
	}


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err StarDrawScreen(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gt = pi->gtmp;
	GInfo go = pi->gout;
	GInfo go2 = pi->gout;
	GInfo go3 = pi->gout;
	GInfo go4 = pi->gout;

	PF_Pixel16 *dataT = (PF_Pixel16 *)gt.data;

	

	A_long posT = 0;
	A_long lv = 0;
	PF_FpLong lvb = 0;
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			PF_Pixel16 c = dataT[posT];
			lv = c.alpha;
			lvb = (PF_FpLong)c.alpha / PF_MAX_CHAN16;
			posT++;
			if (lv <= 0) continue;

			//垂直
			if ((pi->ver.len > 0) && (pi->ver.opa > 0))
			{
				A_long vl = pi->ver.len * lv / PF_MAX_CHAN16;
				if (vl > 0)
				{
					MakePixelTable(pi->verTable, 
						pi->ver.len,
						c, 
						pi->ver.opa*lvb,
						pi->info.brigthness);
					A_long d = pi->ver.len - vl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->ver.len; i++)
					{
						AddPxD(&go, pi->verTable[i]);
						AddPxD(&go2, pi->verTable[i]);
						go.xD += pi->ver.addX;
						go.yD += pi->ver.addY;
						go2.xD -= pi->ver.addX;
						go2.yD -= pi->ver.addY;
					}
				}

			}
			//水平
			if ((pi->hor.len > 0) && (pi->hor.opa > 0))
			{
				A_long hl = pi->hor.len * lv / PF_MAX_CHAN16;
				if (hl > 0)
				{
					MakePixelTable(pi->horTable,
						pi->hor.len,
						c,
						pi->hor.opa*lvb,
						pi->info.brigthness);
					A_long d = pi->hor.len - hl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->hor.len; i++)
					{
						AddPxD(&go, pi->horTable[i]);
						AddPxD(&go2, pi->horTable[i]);
						go.xD += pi->hor.addX;
						go.yD += pi->hor.addY;
						go2.xD -= pi->hor.addX;
						go2.yD -= pi->hor.addY;
					}
				}
			}
			//斜め
			if ((pi->dia.len > 0) && (pi->dia.opa > 0))
			{
				A_long dl = pi->dia.len * lv / PF_MAX_CHAN16;
				if (dl > 0)
				{
					MakePixelTable(pi->diaTable,
						pi->dia.len,
						c,
						pi->dia.opa*lvb,
						pi->info.brigthness);
					A_long d = pi->dia.len - dl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					go3.xD = (PF_FpLong)x;
					go3.yD = (PF_FpLong)y;
					go4.xD = (PF_FpLong)x;
					go4.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->dia.len; i++)
					{
						AddPxD(&go, pi->diaTable[i]);
						AddPxD(&go2, pi->diaTable[i]);
						AddPxD(&go3, pi->diaTable[i]);
						AddPxD(&go4, pi->diaTable[i]);
						go.xD += pi->dia.addX;
						go.yD += pi->dia.addY;
						go2.xD -= pi->dia.addX;
						go2.yD -= pi->dia.addY;
						go3.xD += pi->dia2.addX;
						go3.yD += pi->dia2.addY;
						go4.xD -= pi->dia2.addX;
						go4.yD -= pi->dia2.addY;
					}
				}
			}

		}
		posT += gt.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static void
MakeRainbowTable(PF_Pixel16 *tbl, A_long len,PF_FpLong ro, PF_FpLong rd, PF_FpLong per, PF_FpLong br)
{

	if ((len <= 0) || (per <= 0))
	{
		return;
	}


	PF_FpLong p = per;
	HLSA hls;
	for (A_long i = 0; i < len; i++)
	{
		p = per * (1 - (PF_FpLong)i / (PF_FpLong)len)*0.5;
		A_long v = (A_long)((ro - rd * i) * 65536);
		v %= 65536;
		if (v < 0) v += 65536;
		hls.H = (PF_FpLong)v/65536;
		hls.S = 1;
		hls.L = 0.5;
		hls.A = 1.0;
		PF_Pixel16 c = CONV32TO16(HLStoRGB(hls));
		tbl[i] = PxPer(c, p);
	}

	A_long len2 = len / 2;
	if ((len2 <= 0) || (br <= 0)) return;
	for (A_long i = 0; i < len2; i++)
	{
		A_long r, g, b;
		r = PF_MAX_CHAN16 - tbl[i].red;
		g = PF_MAX_CHAN16 - tbl[i].green;
		b = PF_MAX_CHAN16 - tbl[i].blue;

		tbl[i].red = RoundShort(tbl[i].red + (A_long)((PF_FpLong)r * br * (len2 - i) / len2));
		tbl[i].green = RoundShort(tbl[i].green + (A_long)((PF_FpLong)g * br * (len2 - i) / len2));
		tbl[i].blue = RoundShort(tbl[i].blue + (A_long)((PF_FpLong)b * br * (len2 - i) / len2));

	}

}
//-------------------------------------------------------------------------------------------------
static PF_Err StarDrawRainbow(ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gt = pi->gtmp;
	GInfo go = pi->gout;
	GInfo go2 = pi->gout;
	GInfo go3 = pi->gout;
	GInfo go4 = pi->gout;

	PF_Pixel16 *dataT = (PF_Pixel16 *)gt.data;

	MakeRainbowTable(pi->verTable, pi->ver.len, pi->info.rainbowOffset, pi->info.rainbowDelta, pi->ver.opa, pi->info.brigthness);
	MakeRainbowTable(pi->horTable, pi->hor.len, pi->info.rainbowOffset, pi->info.rainbowDelta, pi->hor.opa, pi->info.brigthness);
	MakeRainbowTable(pi->diaTable, pi->dia.len, pi->info.rainbowOffset, pi->info.rainbowDelta, pi->dia.opa, pi->info.brigthness);


	A_long posT = 0;
	A_long lv = 0;
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			lv = dataT[posT].red;
			posT++;
			if (lv <= 0) continue;

			//垂直
			if ((pi->ver.len > 0) && (pi->ver.opa > 0))
			{
				A_long vl = pi->ver.len * lv / PF_MAX_CHAN16;
				if (vl > 0)
				{
					A_long d = pi->ver.len - vl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->ver.len; i++)
					{
						AddPxD(&go, pi->verTable[i]);
						AddPxD(&go2, pi->verTable[i]);
						go.xD += pi->ver.addX;
						go.yD += pi->ver.addY;
						go2.xD -= pi->ver.addX;
						go2.yD -= pi->ver.addY;
					}
				}

			}
			//水平
			if ((pi->hor.len > 0) && (pi->hor.opa > 0))
			{
				A_long hl = pi->hor.len * lv / PF_MAX_CHAN16;
				if (hl > 0)
				{
					A_long d = pi->hor.len - hl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->hor.len; i++)
					{
						AddPxD(&go, pi->horTable[i]);
						AddPxD(&go2, pi->horTable[i]);
						go.xD += pi->hor.addX;
						go.yD += pi->hor.addY;
						go2.xD -= pi->hor.addX;
						go2.yD -= pi->hor.addY;
					}
				}
			}
			//斜め
			if ((pi->dia.len > 0) && (pi->dia.opa > 0))
			{
				A_long dl = pi->dia.len * lv / PF_MAX_CHAN16;
				if (dl > 0)
				{
					A_long d = pi->dia.len - dl;
					go.xD = (PF_FpLong)x;
					go.yD = (PF_FpLong)y;
					go2.xD = (PF_FpLong)x;
					go2.yD = (PF_FpLong)y;
					go3.xD = (PF_FpLong)x;
					go3.yD = (PF_FpLong)y;
					go4.xD = (PF_FpLong)x;
					go4.yD = (PF_FpLong)y;
					for (A_long i = d; i < pi->dia.len; i++)
					{
						AddPxD(&go, pi->diaTable[i]);
						AddPxD(&go2, pi->diaTable[i]);
						AddPxD(&go3, pi->diaTable[i]);
						AddPxD(&go4, pi->diaTable[i]);
						go.xD += pi->dia.addX;
						go.yD += pi->dia.addY;
						go2.xD -= pi->dia.addX;
						go2.yD -= pi->dia.addY;
						go3.xD += pi->dia2.addX;
						go3.yD += pi->dia2.addY;
						go4.xD -= pi->dia2.addX;
						go4.yD -= pi->dia2.addY;
					}
				}
			}

		}
		posT += gt.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err ToParam(CFsAE *ae, ParamInfo *infoP, ParamInfo16 *pi)
{
	PF_Err	err = PF_Err_NONE;

	
	ae->in->ToGInfo(&pi->gin);
	ae->out->ToGInfo(&pi->gout);
	ae->tmp->ToGInfo(&pi->gtmp);

	pi->info = *infoP;

	pi->color = CONV8TO16(infoP->color);

	PF_PixelFloat pf = CONV8TO32(pi->info.target_color);
	pi->taget_lab = RgbToLab(pf);

	pi->target_border = infoP->target_border;
	pi->target_softness =infoP->target_softness;
	pi->target_start = pi->target_border - pi->target_softness;
	if (pi->target_start < 0) {
		pi->target_start = 0;
		pi->target_softness = pi->target_border;
	}

	pi->hor.len = (A_long)((double)pi->info.horLength * pi->info.length + 0.5);
	pi->ver.len = (A_long)((double)pi->info.verLength * pi->info.length + 0.5);
	pi->dia.len = (A_long)((double)pi->info.diaLength * pi->info.length + 0.5);
	pi->dia2.len = pi->dia.len;

	pi->hor.opa = pi->info.horOpacity * pi->info.opacity;
	pi->ver.opa = pi->info.verOpacity * pi->info.opacity;
	pi->dia.opa = pi->info.diaOpacity * pi->info.opacity;
	pi->dia2.opa = pi->dia.opa;

	pi->rot = pi->info.rot;
	if ((pi->info.autoRolling == TRUE) && (pi->info.rollingSpeed != 0))
	{
		double time = ((double)ae->in_data->current_time / (double)ae->in_data->time_scale);
		pi->rot += (pi->info.rollingSpeed*time);


	}

	pi->hor.rot = pi->rot + pi->info.horRot;
	pi->ver.rot = pi->rot + pi->info.verRot;
	pi->dia.rot = pi->rot + pi->info.diaRot;
	pi->dia2.rot = pi->rot + pi->info.diaRot2;

	pi->hor.rot = CRot::RoundRot(pi->hor.rot);
	pi->ver.rot = CRot::RoundRot(pi->ver.rot);
	pi->dia.rot = CRot::RoundRot(pi->dia.rot);
	pi->dia2.rot = CRot::RoundRot(pi->dia2.rot);


	CRot rot(ae->in_data);
	if (pi->hor.len > 0) {
		rot.SetRotLength(pi->hor.rot, pi->hor.len);
		pi->hor.addX = rot.x() / pi->hor.len;
		pi->hor.addY = rot.y() / pi->hor.len;
	}
	else
	{
		pi->hor.addX = 0;
		pi->hor.addY = 0;

	}

	if (pi->ver.len > 0) {
		rot.SetRotLength(pi->ver.rot, pi->ver.len);
		pi->ver.addX = rot.x() / pi->ver.len;
		pi->ver.addY = rot.y() / pi->ver.len;
	}
	else {
		pi->ver.addX = 0;
		pi->ver.addY = 0;
	}
	if (pi->dia.len > 0) {
		rot.SetRotLength(pi->dia.rot, pi->dia.len);
		pi->dia.addX = rot.x() / pi->dia.len;
		pi->dia.addY = rot.y() / pi->dia.len;

		rot.SetRotLength(pi->dia2.rot, pi->dia2.len);
		pi->dia2.addX = rot.x() / pi->dia2.len;
		pi->dia2.addY = rot.y() / pi->dia2.len;
	}
	else {
		pi->dia.addX = 0;
		pi->dia.addY = 0;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarExec16(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if (ae->out->Enabled() == FALSE)
	{
		return PF_Err_INVALID_CALLBACK;
	}
	ae->NewTmpWorld();
	ParamInfo16 pi;

	ToParam(ae, infoP, &pi);
	pi.bufH = ae->NewHandle( (pi.hor.len + pi.ver.len + pi.dia.len)* sizeof(PF_Pixel16)+ 1024);
	if (pi.bufH == NULL) {
		err = PF_Err_OUT_OF_MEMORY;
		return err;
	}
	pi.horTable = *(PF_Pixel16**)(pi.bufH);
	pi.verTable = (pi.horTable + pi.hor.len);
	pi.diaTable = (pi.verTable + pi.ver.len);
	//ターゲットエリア
	ERR(ae->iterate16((refconType)&pi, TargetPixel));
	ae->out->SetMatMode(MAT::blackMat);
	if (pi.info.target_maskDraw==TRUE) {
		ae->out->fromBlackMat16();
		return err;
	}
	else {

		CopyAtoTemp(&pi);
		TempRev(&pi);
		for (A_long i = 0; i < 3; i++) {
			ERR(MaxHor( &pi));
			ERR(MaxVer( &pi));
		}
		TempSub(&pi);
		blurTempRed(&pi);

		switch (pi.info.color_kind)
		{
		case 3:
			ae->out->clear();
			StarDrawRainbow(&pi);
			break;
		case 2:
			ScreenTemp(&pi);
			StarDrawScreen(&pi);
			break;
		case 1:
		default:
			ae->out->clear();
			StarDrawColor(&pi);
			CenterDraw(&pi);
			break;
		}
		blur(&pi);
		switch (pi.info.blend)
		{
		case 4:
			ERR(ae->iterate16((refconType)&pi, BlendNormal));
			break;
		case 3:
			ERR(ae->iterate16((refconType)&pi, BlendAdd));
			break;
		case 2:
			ERR(ae->iterate16((refconType)&pi, BlendScreen));
			break;
		case 1:
		default:
			break;
		}

		ae->out->fromBlackMat16();

	}

	if (pi.bufH != NULL) {
		ae->DisposeHandle(pi.bufH);
		pi.bufH = NULL;
	}

	return err;

}