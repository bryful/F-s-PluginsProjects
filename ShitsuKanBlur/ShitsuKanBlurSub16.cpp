#include "ShitsuKanBlur.h"

static PF_Pixel16(*GetPX16)(ParamInfo16 *infoP, A_long x, A_long y);

//-------------------------------------------------------------------------------------------------
//ぼかし用のガウステーブル
static PF_Boolean makeTbl16(ParamInfo16 *infoP)
{
	PF_InData *in_data;
	in_data = infoP->in_data;
	A_long range;
	if (infoP->IsTagetHi) range = infoP->info.hi_blur;
	else range = infoP->info.sw_blur;

	if (range <= 0) return FALSE;
	double zone = (double)range / 3;
	for (A_long i = 0; i <= range; i++) {
		infoP->blurTable[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone)) * 0x100);
	}
	return TRUE;
}
//----------------------------------------------------------------------------
//Greenのみを水平ぼかし
void horBlurG16(ParamInfo16 *infoP)
{
	A_long blur = 0;
	if (infoP->IsTagetHi) blur = infoP->info.hi_blur;
	else blur = infoP->info.sw_blur;
	if (blur <= 0) return;

	A_long wt = infoP->widthTrue;
	A_u_short *scanline = infoP->scanline;
	A_u_short *scanlineM = infoP->scanlineM;
	PF_Pixel16 *data = infoP->data;
	A_long hor = 0;
	A_long ix, i;
	A_long x = 0, y = 0;
	A_long count, sg, gauss;

	hor = 0;
	for (y = 0; y < infoP->height; y++)
	{
		hor = wt * y;
		//スキャンラインへコピー
		for (x = 0; x < infoP->width; x++) {
			scanline[x] = data[x + hor].green;
			scanlineM[x] = data[x + hor].red;
		}
		for (x = 0; x < infoP->width; x++) {
			if (scanlineM[x] == PF_MAX_CHAN16)
			{
				count = sg = gauss = 0;
				//真ん中
				gauss = infoP->blurTable[0];
				sg += (A_long)scanline[x] * gauss;
				count += gauss;
				//左
				for (i = 1; i <= blur; i++)
				{
					ix = x - i;
					if (ix < 0) break;
					if (scanlineM[ix] > 0) {
						gauss = infoP->blurTable[i];
						sg += (A_long)scanline[ix] * gauss;
						count += gauss;
					}
				}
				//右
				for (i = 1; i <= blur; i++)
				{
					ix = x + i;
					if (ix >= infoP->width) break;
					if (scanlineM[ix] > 0) {
						gauss = infoP->blurTable[i];
						sg += (A_long)scanline[ix] * gauss;
						count += gauss;
					}
				}
				if (count > 0) {
					data[hor + x].green = RoundShort(sg / count);
				}
			}
			else {
				data[hor + x].green = 0;
			}
		}
	}

}
//----------------------------------------------------------------------------
void vurBlurG16(ParamInfo16 *infoP)
{
	A_long blur = 0;
	if (infoP->IsTagetHi) blur = infoP->info.hi_blur;
	else blur = infoP->info.sw_blur;
	if (blur <= 0) return;

	A_long wt = infoP->widthTrue;

	A_u_short *scanline = infoP->scanline;
	A_u_short *scanlineM = infoP->scanlineM;
	PF_Pixel16 *data = infoP->data;
	A_long hor = 0;
	A_long iy, i;
	A_long x = 0, y = 0;
	A_long count, sg, gauss;
	hor = 0;
	for (x = 0; x < infoP->width; x++)
	{
		//スキャンラインへコピー
		hor = 0;
		for (y = 0; y < infoP->height; y++) {
			scanline[y] = data[x + hor].green;
			scanlineM[y] = data[x + hor].red;
			hor += wt;
		}
		hor = 0;
		for (y = 0; y < infoP->height; y++) {
			if (scanlineM[y] == PF_MAX_CHAN16)
			{
				count = sg = gauss = 0;
				//真ん中
				gauss = infoP->blurTable[0];
				sg += (A_long)scanline[y] * gauss;
				count += gauss;
				//左
				for (i = 1; i <= blur; i++)
				{
					iy = y - i;
					if (iy < 0) break;
					if (scanlineM[iy] > 0) {
						gauss = infoP->blurTable[i];
						sg += (A_long)scanline[iy] * gauss;
						count += gauss;
					}
				}
				//右
				for (i = 1; i <= blur; i++)
				{
					iy = y + i;
					if (iy >= infoP->height) break;
					if (scanlineM[iy] > 0) {
						gauss = infoP->blurTable[i];
						sg += (A_long)scanline[iy] * gauss;
						count += gauss;
					}
				}
				if (count > 0) {
					data[hor + x].green = RoundShort(sg / count);
				}
			}
			else {
				data[hor + x].green = 0;
			}
			hor += wt;
		}
	}

}
//-------------------------------------------------------------------------------------------------
static PF_Pixel16 GetPX16_NOCLIP(ParamInfo16 *infoP, A_long x, A_long y)
{
	if ((x < 0) || (y < 0) || (x > infoP->width2) || (y > infoP->height2)) {
		PF_Pixel16 p = { 0,0,0,0 };
		return p;
	}
	else {
		PF_Pixel16 *data;
		data = infoP->data;
		return data[x + y * infoP->widthTrue];
	}
}
//-------------------------------------------------------------------------------------------------
static PF_Pixel16 GetPX16_CLIP(ParamInfo16 *infoP, A_long x, A_long y)
{
	A_long xx = x;
	A_long yy = y;
	if (xx < 0) xx = 0;
	else if (xx > infoP->width2) xx = infoP->width2;

	if (yy < 0) yy = 0;
	else if (yy > infoP->height2) yy = infoP->height2;

	PF_Pixel16 *data;
	data = infoP->data;
	return data[xx + yy * infoP->widthTrue];
}
//-------------------------------------------------------------------------------------------------
static PF_Err
Iterate16(
	CFsAE *ae, 
	ParamInfo16 *niP,
	PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16 *in, PF_Pixel16 *out)
)
{
	PF_Err			err = PF_Err_NONE;

	A_long w = ae->in->width();
	A_long h = ae->in->height();

	A_long owi = ae->in->offsetWidth();
	A_long owo = ae->out->offsetWidth();

	PF_Pixel16 *odata;
	PF_Pixel16 *idata;

	idata = (PF_Pixel16*)ae->in->data();
	odata = (PF_Pixel16*)ae->out->data();

	for (A_long yL = 0; yL < h; yL++) {
		for (A_long xL = 0; xL < w; xL++) {
			pix_fn((refconType)niP, xL, yL, idata, odata);
			idata++;
			odata++;
		}
		idata += owi;
		odata += owo;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
GetTargetPixels16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);


	PF_Pixel8 s = CONV16TO8(*inP);
	PF_Pixel16 onC = { PF_MAX_CHAN16,0,0,0 };
	PF_Pixel16 exC = { PF_HALF_CHAN16,0,0,0 };
	PF_Pixel16 offC = { 0,0,0,0 };

	A_long b = 0;
	for (int i = 0; i < niP->info.Color_Max; i++)
	{
		if (compPix8(s, niP->info.Color[i]) == TRUE)
		{
			b = 1;
			break;
		}
	}
	if (b == 0) {
		for (int i = 0; i < niP->info.EXColor_Max; i++)
		{
			if (compPix8(s, niP->info.EXColor[i]) == TRUE)
			{
				b = 2;
				break;
			}
		}
	}
	if (b == 1) {
		*outP = onC;
		niP->TargetCount += 1;
	}
	else if (b==2){
		*outP = exC;
	}
	else {
		*outP = offC;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
AlphaTargetToGreen16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	if (outP->alpha != 0) {
		outP->green = PF_MAX_CHAN16;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
SwapGB16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	A_u_short tmp = outP->green;
	outP->green = outP->blue;
	outP->blue = tmp;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
RedToGreen16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	outP->green = outP->red;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
AlphaON16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	outP->alpha = PF_MAX_CHAN16;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaskedGBFromA16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;

	if (outP->alpha != PF_MAX_CHAN16) {
		outP->green = 0;
		outP->blue = 0;
	}
	outP->red = 0;


	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err
MaxHorG2R16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);

	PF_Boolean b = FALSE;
	if (outP->green == 0)
	{
		PF_Pixel16 *d;
		if (xL > 0) {
			d = outP - 1;
			if (d->green > 0) b = true;
		}
		if (b == FALSE) {
			if (xL < niP->width2) {
				d = outP + 1;
				if (d->green > 0) b = true;
			}
		}

	}
	else {
		b = TRUE;
	}
	if (b == TRUE) {
		outP->red = PF_MAX_CHAN16;
	}
	else {
		outP->red = 0;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxVurG2R16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);

	PF_Boolean b = FALSE;
	if (outP->green == 0)
	{
		PF_Pixel16 *d;
		if (yL > 0) {
			d = outP - niP->widthTrue;
			if (d->green > 0) b = true;
		}
		if (b == FALSE) {
			if (yL < niP->height2) {
				d = outP + niP->widthTrue;
				if (d->green > 0) b = true;
			}
		}

	}
	else {
		b = TRUE;
	}
	if (b == TRUE) {
		outP->red = PF_MAX_CHAN16;
	}
	else {
		outP->red = 0;
	}


	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err
MoveG2R16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);


	PF_Pixel16 c = *outP;

	PF_Pixel16 p1 = GetPX16(niP, xL + niP->hi_x_offset, yL + niP->hi_y_offset);
	PF_Pixel16 p2 = GetPX16(niP, xL - niP->sw_x_offset, yL - niP->sw_y_offset);


	//PF_MAX_CHAN16 = 32768 0b10000000_00000000
	//だったので
	A_u_short r = 0;
	if (outP->green > 0) r |= 0x7F00;
	if (p1.green > 0) r |= 0xF0;
	if (p2.green > 0) r |= 0xF;

	outP->red = r;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
SplitG2RGB16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);

	A_u_short moto = (outP->green & 0xF00) >> 8;
	A_u_short up = (outP->green & 0xF0) >> 4;
	A_u_short down = outP->green & 0x0F;

	if (moto > 0) {
		outP->red = PF_MAX_CHAN16;
	}
	else {
		outP->red = 0;
	}

	if (moto - up > 0) {
		outP->green= PF_MAX_CHAN16;
	}
	else {
		outP->green = 0;
	}
	if (moto - down > 0) {
		outP->blue = PF_MAX_CHAN16;
	}
	else {
		outP->blue = 0;
	}

	

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
Noise16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);


	A_long v = (A_long)(niP->info.noise * PF_MAX_CHAN16);
	A_long rnd = v - (v * 2 * F_RAND() / F_RAND_MAX);
	if (rnd != 0) {
		if (outP->alpha > 0) {
			outP->green = RoundShort(outP->green + rnd);
			outP->blue = RoundShort(outP->blue + rnd);
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
CopyTagetEX16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);

	PF_Pixel16 c = { 0,0,0,0 };
	if (outP->alpha != PF_MAX_CHAN16)
	{
		PF_Boolean b = TRUE;
		if (niP->info.white == TRUE)
		{
			if ((inP->red == PF_MAX_CHAN16) && (inP->green == PF_MAX_CHAN16) && (inP->blue == PF_MAX_CHAN16)) {
				b = FALSE;
			}
		}
		if (b == TRUE) {
			*outP = *inP;
		}
		else {
			*outP = c;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
GreenToAlpha16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);
	
	PF_Pixel16 c = niP->MaskColor16;
	c.alpha = outP->green;;
	*outP = c;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
BlueToAlpha16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);

	PF_Pixel16 c = niP->MaskColor16;
	c.alpha = outP->blue;;
	*outP = c;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong
LvSet16(PF_FpLong src, PF_FpLong lv, PF_FpLong alpha)
{
	PF_FpLong ret = 0;

	if ((lv == 0) || (alpha == 0)) {
		return src;
	}
	PF_FpLong r = 0;
	if (lv < 0) {
		r = src * lv;
	}
	else if (lv > 0)
	{
		r = (PF_MAX_CHAN16 - src) *lv;
	}
	if (alpha != 1) {
		r *= alpha;
	}

	ret = src + r;


	return ret;
}


static PF_Err
ColorSet16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP = reinterpret_cast<ParamInfo16*>(refcon);

	//hi
	PF_FpLong r, g, b;

	if (outP->alpha == PF_MAX_CHAN16) {
		PF_FpLong a = (PF_FpLong)outP->green / PF_MAX_CHAN16;

		r = (PF_FpLong)inP->red;
		g = (PF_FpLong)inP->green;
		b = (PF_FpLong)inP->blue;
		if (a > 0) {
			r = LvSet16(r, niP->info.hi_lv, a);
			g = LvSet16(g, niP->info.hi_lv, a);
			b = LvSet16(b, niP->info.hi_lv, a);

			r = LvSet16(r, niP->info.hi_r, a);
			g = LvSet16(g, niP->info.hi_g, a);
			b = LvSet16(b, niP->info.hi_b, a);
		}
		a = (PF_FpLong)outP->blue / PF_MAX_CHAN16;
		if (a > 0) {
			r = LvSet16(r, niP->info.sw_lv, a);
			g = LvSet16(g, niP->info.sw_lv, a);
			b = LvSet16(b, niP->info.sw_lv, a);

			r = LvSet16(r, niP->info.sw_r, a);
			g = LvSet16(g, niP->info.sw_g, a);
			b = LvSet16(b, niP->info.sw_b, a);
		}

		outP->red = RoundShort((A_long)(r + 0.5));
		outP->green = RoundShort((A_long)(g + 0.5));
		outP->blue = RoundShort((A_long)(b + 0.5));
	}
	else {
		outP->alpha = 0;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
debug16(CFsAE *ae, ParamInfo16 *pi)
{
	PF_Err			err = PF_Err_NONE;

	A_long a;
	a = pi->hi_x_offset; if (a < 0) a *= -1;
	pi->data[0].alpha = RoundShortFpLong(a);
	a = pi->hi_y_offset; if (a < 0) a *= -1;
	pi->data[1].alpha = RoundShortFpLong(a);
	a = pi->sw_x_offset; if (a < 0) a *= -1;
	pi->data[2].alpha = RoundShortFpLong(a);
	a = pi->sw_y_offset; if (a < 0) a *= -1;
	pi->data[3].alpha = RoundShortFpLong(a);

	a = pi->info.hi_length >> 16;
	pi->data[4].alpha = RoundShortFpLong(a);
	a = pi->info.sw_length >> 16;
	pi->data[5].alpha = RoundShortFpLong(a);


	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err ToParam16(CFsAE *ae, ParamInfo *infoP, ParamInfo16 *pi)
{
	PF_Err	err = PF_Err_NONE;

	infoP->TagetCount = 0;

	pi->width = ae->out->width();
	pi->width2 = pi->width - 1;
	pi->widthTrue = ae->out->widthTrue();
	pi->height = ae->out->height();
	pi->height2 = pi->height - 1;
	pi->data = (PF_Pixel16 *)ae->out->data();
	pi->in_data = ae->in_data;

	pi->info.Color_Max = infoP->Color_Max;
	pi->info.EXColor_Max = infoP->EXColor_Max;

	for (int i = 0; i < pi->info.Color_Max; i++) pi->info.Color[i] = infoP->Color[i];
	for (int i = 0; i < pi->info.EXColor_Max; i++) pi->info.EXColor[i] = infoP->EXColor[i];

	pi->info.noise = infoP->noise;
	pi->info.draw = infoP->draw;
	pi->info.edge = infoP->edge;

	pi->scanlineSize = pi->width;
	if (pi->scanlineSize < pi->height) pi->scanlineSize = pi->height;


	pi->info.hi_length = ae->downScaleNoClip(infoP->hi_length);
	pi->info.hi_blur = ae->downScaleNoClip(infoP->hi_blur);
	if (pi->info.hi_length <= 0) pi->info.hi_blur = 0;

	pi->info.sw_length = ae->downScaleNoClip(infoP->sw_length);
	pi->info.sw_blur = ae->downScaleNoClip(infoP->sw_blur);
	if (pi->info.sw_length <= 0) pi->info.sw_blur = 0;

	pi->blurSize = pi->info.hi_blur;
	if (pi->blurSize < pi->info.sw_blur) pi->blurSize = pi->info.sw_blur;


	CRotCalc rot(ae->in_data);

	A_long LM = ae->downScale(LENGTH_MIN);

	if (pi->info.hi_length > 0) {
		rot.SetRotLength(infoP->angle, pi->info.hi_length);
		pi->hi_x_offset = (A_long)((double)(rot.x()) / 65536.0 + 0.5);
		pi->hi_y_offset = (A_long)((double)(rot.y()) / 65536.0 + 0.5);
		if (pi->hi_x_offset > 0) pi->hi_x_offset += LM;
		else if (pi->hi_x_offset < 0)pi->hi_x_offset -= LM;
		if (pi->hi_y_offset > 0) pi->hi_y_offset += LM;
		else if (pi->hi_y_offset < 0)pi->hi_y_offset -= LM;
	}
	else {
		pi->hi_x_offset = 0;
		pi->hi_y_offset = 0;

	}
	if (pi->info.sw_length > 0) {
		rot.SetRotLength(infoP->angle, pi->info.sw_length);
		pi->sw_x_offset = (A_long)((double)(rot.x()) / 65536.0 + 0.5);
		pi->sw_y_offset = (A_long)((double)(rot.y()) / 65536.0 + 0.5);
		if (pi->sw_x_offset > 0) pi->sw_x_offset += LM;
		else if (pi->sw_x_offset < 0)pi->sw_x_offset -= LM;
		if (pi->sw_y_offset > 0) pi->sw_y_offset += LM;
		else if (pi->sw_y_offset < 0)pi->sw_y_offset -= LM;
	}
	else {
		pi->sw_x_offset = 0;
		pi->sw_y_offset = 0;

	}
	pi->info.hi_lv = infoP->hi_lv;
	pi->info.hi_r = infoP->hi_r;
	pi->info.hi_g = infoP->hi_g;
	pi->info.hi_b = infoP->hi_b;

	pi->info.sw_lv = infoP->sw_lv;
	pi->info.sw_r = infoP->sw_r;
	pi->info.sw_g = infoP->sw_g;
	pi->info.sw_b = infoP->sw_b;


	pi->MaskColor16 = CONV8TO16(infoP->MaskColor);
	pi->info.white = infoP->white;

	return err;
}
	//-------------------------------------------------------------------------------------------------
PF_Err SitsuKanSub16(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if (ae->out->Enabled() == FALSE)
	{
		return PF_Err_INVALID_CALLBACK;
	}
	ParamInfo16 pi;

	ToParam16(ae, infoP, &pi);


	if (infoP->edge == TRUE) {
		GetPX16 = GetPX16_NOCLIP;
	}
	else {
		GetPX16 = GetPX16_CLIP;
	}

	//ターゲットをalphaへ抽出
	pi.TargetCount = 0;
	ERR(ae->iterate16((refconType)&pi, GetTargetPixels16));

	if (pi.TargetCount > 0) {

		//Max
		//alphaをgreenへ
		ERR(ae->iterate16((refconType)&pi, AlphaTargetToGreen16));


		A_long ll = ae->downScale(LENGTH_MIN);
		if ((pi.info.hi_length > 0) || (pi.info.sw_length > 0)) {
			
			//LENGTH_MIN回繰り返す
			for (int i = 0; i < ll; i++) {
				ERR(ae->iterate16((refconType)&pi, MaxHorG2R16));
				ERR(ae->iterate16((refconType)&pi, RedToGreen16));
				ERR(ae->iterate16((refconType)&pi, MaxVurG2R16));
				ERR(ae->iterate16((refconType)&pi, RedToGreen16));
			}
			
			//ずらし
			ERR(ae->iterate16((refconType)&pi, MoveG2R16));
			ERR(ae->iterate16((refconType)&pi, RedToGreen16));
			ERR(ae->iterate16((refconType)&pi, SplitG2RGB16));
			//ぼかし
			if ((pi.info.hi_blur > 0) || (pi.info.sw_blur > 0)) {
				pi.scanlineH = ae->NewHandle(pi.scanlineSize * sizeof(A_u_short) * 4 + pi.blurSize*sizeof(A_long) + 100);

				if (pi.scanlineH != NULL) {
					ae->LockHandle(pi.scanlineH);
					pi.scanline = *(A_u_short**)pi.scanlineH;
					pi.scanlineM = pi.scanline + pi.scanlineSize * 2;
					pi.blurTable = (A_long*)(pi.scanline + pi.scanlineSize * 4);
					//hi
					if (pi.info.hi_blur > 0) {
						pi.IsTagetHi = TRUE;
						if (makeTbl16(&pi) == TRUE) {
							horBlurG16(&pi);
							vurBlurG16(&pi);

							A_long b = pi.info.hi_blur;
							pi.info.hi_blur /= 4;
							if (makeTbl16(&pi) == TRUE) {
								horBlurG16(&pi);
								vurBlurG16(&pi);
							}
							pi.info.hi_blur = b;

						}
					}
					//sw

					if (pi.info.sw_blur > 0) {
						pi.IsTagetHi = FALSE;
						if (makeTbl16(&pi) == TRUE) {
							(Iterate16(ae, &pi, SwapGB16));
							horBlurG16(&pi);
							vurBlurG16(&pi);
							A_long b = infoP->sw_blur;
							pi.info.sw_blur /= 4;
							if (makeTbl16(&pi) == TRUE) {
								horBlurG16(&pi);
								vurBlurG16(&pi);
							}
							pi.info.sw_blur = b;
							(Iterate16(ae, &pi, SwapGB16));
						}
					}

					ae->UnlockHandle(pi.scanlineH);
					ae->DisposeHandle(pi.scanlineH);
					pi.scanlineH = NULL;
				}
			}
		}
		
		//ERR(ae->iterate16((refconType)&pi, MaskedGBFromA16));
		(Iterate16(ae, &pi, MaskedGBFromA16));
		if (pi.info.noise > 0) {
			F_SRAND(ae->frame());
			//ERR(ae->iterate8((refconType)&pi, Noise8));
			(Iterate16(ae, &pi, Noise16));
		}
		if ((pi.info.draw == 1) || (pi.info.draw == 2)) {
			//ERR(ae->iterate8((refconType)&pi, ColorSet8));
			(Iterate16(ae, &pi, ColorSet16));
		}
		

	}
	
	if (pi.info.draw == 1) {
		//ERR(ae->iterate8((refconType)&pi, CopyTagetEX));
		(Iterate16(ae, &pi, CopyTagetEX16));
	}
	else if (pi.info.draw == 3)
	{
		//ERR(ae->iterate16((refconType)&pi, GreenToAlpha16));
		(Iterate16(ae, &pi, GreenToAlpha16));
	}
	else if (pi.info.draw == 4)
	{
		//ERR(ae->iterate16((refconType)&pi, BlueToAlpha16));
		(Iterate16(ae, &pi, BlueToAlpha16));
	}

	//debug16(ae, &pi);

	return err;

}
/*

まずAlphaにTargetエリアのマスクを作成


*/
//-------------------------------------------------------------------------------------------------
