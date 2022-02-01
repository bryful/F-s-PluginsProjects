#include "Extract_Edge.h"

//-------------------------------------------------------------------------------------------------
//画像の色を獲得
inline PF_Pixel GetPX(GInfo *gi, A_long x, A_long y)
{
	PF_Pixel *data = (PF_Pixel *)gi->data;

	A_long xx = x;
	if (xx < 0) xx = 0; else if (xx >= gi->width) xx = gi->width - 1;
	A_long yy = y;
	if (yy < 0) yy = 0; else if (yy >= gi->height) xx = gi->height - 1;
	return data[xx + yy * gi->widthTrue];

}

//-------------------------------------------------------------------------------------------------
static PF_Err
ToGray(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	infoP = reinterpret_cast<ParamInfo8*>(refcon);

	PF_FpLong r, g, b, a;
	a = (PF_FpLong)inP->alpha / PF_MAX_CHAN8;
	r = (PF_FpLong)inP->red * a / PF_MAX_CHAN8;
	g = (PF_FpLong)inP->green * a / PF_MAX_CHAN8;
	b = (PF_FpLong)inP->blue * a / PF_MAX_CHAN8;

	PF_PixelFloat c;
	c.alpha = RoundFpShortDouble(a);
	c.red = RoundFpShortDouble(r);
	c.green = RoundFpShortDouble(g);
	c.blue = RoundFpShortDouble(b);

	LABA lab = RgbToLab(c);

	outP->red = RoundByteLong((A_long)(lab.L * PF_MAX_CHAN8 + 0.5));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err EdgeR2G(ParamInfo8 *infoP)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gi = infoP->gin;
	GInfo go = infoP->gout;

	PF_Pixel8 *data = (PF_Pixel8 *)go.data;
	A_long pos = 0;
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			PF_FpLong v1 = (PF_FpLong)data[pos].red / PF_MAX_CHAN8;
			PF_FpLong v2 = (PF_FpLong)GetPX(&go, x+1, y).red / PF_MAX_CHAN8;
			PF_FpLong v3 = (PF_FpLong)GetPX(&go, x, y+1).red / PF_MAX_CHAN8;
			PF_FpLong d = ABS(v1 - v2) + ABS(v2 - v3);

			if (infoP->info.HiLo==0)
			{ 
				if (d >= infoP->info.HiLevel) {
					d = 1.0;
				}
				else {
					d = 0;
				}
			}else if (d <= infoP->info.LoLevel) {
				d = 0;
			}
			else if (d >= infoP->info.HiLevel)
			{
				d = 1.0;
			}
			else {
				d = (d - infoP->info.LoLevel) / infoP->info.HiLo;
			}


			data[pos].green = RoundByteLong((A_long)(d*PF_MAX_CHAN8 + 0.5));
			pos++;
		}
		pos += go.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err AlphaToRed(ParamInfo8 *infoP)
{
	PF_Err			err = PF_Err_NONE;

	GInfo gi = infoP->gin;
	GInfo go = infoP->gout;

	PF_Pixel8 *dataI = (PF_Pixel8 *)gi.data;
	PF_Pixel8 *dataO = (PF_Pixel8 *)go.data;
	A_long posI = 0;
	A_long posO = 0;
	for (A_long y = 0; y < go.height; y++)
	{
		for (A_long x = 0; x < go.width; x++)
		{
			dataO[posO].red = dataI[posI].alpha;
			posI++;
			posO++;
		}
		posI += gi.offsetWidth;
		posO += go.offsetWidth;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err ToParam(CFsAE *ae, ParamInfo *infoP, ParamInfo8 *pi)
{
	PF_Err	err = PF_Err_NONE;

	pi->info = *infoP;

	ae->in->ToGInfo(&pi->gin);
	ae->out->ToGInfo(&pi->gout);




	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err Exec8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ParamInfo8 pi;
	ToParam(ae, infoP, &pi);

	ae->CopyInToOut();
	ERR(ae->iterate8((refconType)&pi, ToGray));
	EdgeR2G(&pi);
	//VerSub(&pi);
	return err;

}