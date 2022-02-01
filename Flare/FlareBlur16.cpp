#include "Flare.h"

//==================================================================================
static PF_Err BlurRedHor(CFsGraph *g, A_long blue, A_u_short *scanline, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data();

	A_long w = g->width()/2;
	A_long wt = g->widthTrue();
	A_long h = g->height()/2;

	A_long adr = 0;
	for (A_long y = 0; y < h; y++)
	{
		//スキャンラインへ転送
		for (A_long x = 0; x < w; x++) {
			scanline[x] = data[adr + x].red;
		}
		for (A_long x = 0; x <w; x++) {
			A_long count = 0;
			A_long gauss = 0;
			A_long r = 0;
			//真ん中
			gauss = tbl[0];
			if (scanline[x] > 0) {
				r += scanline[x] * gauss;
			}
			count += gauss;
			for (A_long i = 1; i <= blue; i++)
			{
				A_long il = x - i;
				gauss = tbl[i];
				if (il >= 0) {
					if (scanline[il] > 0) {
						r += scanline[il] * gauss;
					}
					count += gauss;
				}
				A_long ir = x + i;
				if (ir < w) {
					if (scanline[ir] > 0) {
						r += scanline[ir] * gauss;
					}
					count += gauss;
				}
			}
			if (count > 0) {
				data[adr + x].red = RoundShort(r / count);
			}
		}
		adr += wt;
	}
	return err;
}
static PF_Err BlurRedVur(CFsGraph *g, A_long blue, A_u_short *scanline, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data();

	A_long w = g->width()/2;
	A_long wt = g->widthTrue();
	A_long h = g->height()/2;

	A_long adr = 0;
	for (A_long x = 0; x < w; x++)
	{
		//スキャンラインへ転送
		adr = 0;
		for (A_long y = 0; y < h; y++) {
			scanline[y] = data[x + adr].red;
			adr += wt;
		}
		adr = x;
		for (A_long y = 0; y < h; y++) {
			A_long count = 0;
			A_long gauss = 0;
			A_long r = 0;
			//真ん中
			gauss = tbl[0];
			if (scanline[y] > 0) {
				r += scanline[y] * gauss;
			}
			count += gauss;
			for (A_long i = 1; i <= blue; i++)
			{
				A_long il = y - i;
				gauss = tbl[i];
				if (il >= 0) {
					if (scanline[il] > 0) {
						r += scanline[il] * gauss;
					}
					count += gauss;
				}
				A_long ir = y + i;
				if (ir < h) {
					if (scanline[ir] > 0) {
						r += scanline[ir] * gauss;
					}
					count += gauss;
				}
			}
			if (count > 0) {
				data[adr].red = RoundShort(r / count);
			}
			adr += wt;
		}
	}
	return err;
}
//==================================================================================
static PF_Err MakeTbl(CFsAE *ae, A_long blur, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;
	PF_InData *in_data;
	in_data = ae->in_data;

	if (blur == 0) return err;
	double zone = (double)blur / 3;
	for (A_long i = 0; i <= blur; i++) {
		tbl[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone)) * 0x100);
	}
	return err;

}
//==================================================================================
/*redチャンネルのみ対応
*/

PF_Err BlurRed16(CFsAE *ae, FlareInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long blur = infoP->blur;
	if (blur == 0) return err;
	PF_InData *in_data;
	in_data = ae->in_data;

	//scanline
	A_u_short* scanline = *(A_u_short**)infoP->bufH;

	A_long w = ae->tmp->width();
	if (w < ae->tmp->height()) w = ae->tmp->height();
	//table
	A_long * tbl = (A_long *)(scanline + w);

	blur = (A_long)PF_CEIL(blur);
	if (blur < 1) blur = 1;
	MakeTbl(ae, blur, tbl);

	BlurRedHor(ae->tmp, blur, scanline, tbl);
	BlurRedVur(ae->tmp, blur, scanline, tbl);

	return err;
}


static PF_Err BlurAllHor(CFsGraph *g, A_long blue, PF_Pixel16 *scanline, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data();

	A_long w = g->width()/2;
	A_long wt = g->widthTrue();
	A_long h = g->height()/2;

	A_long adr = 0;
	for (A_long y = 0; y < h; y++)
	{
		//スキャンラインへ転送
		for (A_long x = 0; x < w; x++) {
			scanline[x] = data[adr + x];
		}
		for (A_long x = 0; x <w; x++) {
			A_long count = 0;
			A_long gauss = 0;
			A_long r = 0;
			A_long g = 0;
			A_long b = 0;
			A_long a = 0;
			//真ん中
			gauss = tbl[0];
			if (scanline[x].alpha > 0) {
				r += scanline[x].red * gauss;
				g += scanline[x].green * gauss;
				b += scanline[x].blue * gauss;
				a += scanline[x].alpha * gauss;
			}
			count += gauss;
			for (A_long i = 1; i <= blue; i++)
			{
				A_long il = x - i;
				gauss = tbl[i];
				if (il >= 0) {
					if (scanline[il].alpha > 0) {
						r += scanline[il].red * gauss;
						g += scanline[il].green * gauss;
						b += scanline[il].blue * gauss;
						a += scanline[il].alpha * gauss;
					}
					count += gauss;
				}
				A_long ir = x + i;
				if (ir < w) {
					if (scanline[ir].alpha > 0) {
						r += scanline[ir].red * gauss;
						g += scanline[ir].green * gauss;
						b += scanline[ir].blue * gauss;
						a += scanline[ir].alpha * gauss;
					}
					count += gauss;
				}
			}
			if (count > 0) {
				data[adr + x].red = RoundShort(r / count);
				data[adr + x].green = RoundShort(g / count);
				data[adr + x].blue = RoundShort(b / count);
				data[adr + x].alpha = RoundShort(a / count);
			}
		}
		adr += wt;
	}
	return err;
}

static PF_Err BlurAllVur(CFsGraph *g, A_long blue, PF_Pixel16 *scanline, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data();

	A_long w = g->width() / 2;
	A_long wt = g->widthTrue();
	A_long h = g->height() / 2;

	A_long adr = 0;
	for (A_long x = 0; x < w; x++)
	{
		//スキャンラインへ転送
		adr = 0;
		for (A_long y = 0; y < h; y++) {
			scanline[y] = data[x + adr];
			adr += wt;
		}
		adr = x;
		for (A_long y = 0; y < h; y++) {
			A_long count = 0;
			A_long gauss = 0;
			A_long r = 0;
			A_long g = 0;
			A_long b = 0;
			A_long a = 0;
			//真ん中
			gauss = tbl[0];
			if (scanline[y].alpha > 0) {
				r += scanline[y].red * gauss;
				g += scanline[y].green * gauss;
				b += scanline[y].blue * gauss;
				a += scanline[y].alpha * gauss;
			}
			count += gauss;
			for (A_long i = 1; i <= blue; i++)
			{
				A_long il = y - i;
				gauss = tbl[i];
				if (il >= 0) {
					if (scanline[il].alpha > 0) {
						r += scanline[il].red * gauss;
						g += scanline[il].green * gauss;
						b += scanline[il].blue * gauss;
						a += scanline[il].alpha * gauss;
					}
					count += gauss;
				}
				A_long ir = y + i;
				if (ir < h) {
					if (scanline[ir].alpha > 0) {
						r += scanline[ir].red * gauss;
						g += scanline[ir].green * gauss;
						b += scanline[ir].blue * gauss;
						a += scanline[ir].alpha * gauss;
					}
					count += gauss;
				}
			}
			if (count > 0) {
				data[adr].red = RoundShort(r / count);
				data[adr].green = RoundShort(g / count);
				data[adr].blue = RoundShort(b / count);
				data[adr].alpha = RoundShort(a / count);
			}
			adr += wt;
		}
	}
	return err;
}


PF_Err BlurAll16(CFsAE *ae, FlareInfo16 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long blur = infoP->blur;
	if (blur == 0) return err;
	PF_InData *in_data;
	in_data = ae->in_data;

	//scanline
	PF_Pixel16* scanline = *(PF_Pixel16**)infoP->bufH;

	A_long w = ae->tmp->width();
	if (w < ae->tmp->height()) w = ae->tmp->height();
	//table
	A_long * tbl = (A_long *)(scanline + w);

	blur = (A_long)PF_CEIL(blur);
	if (blur < 1) blur = 1;
	MakeTbl(ae, blur, tbl);

	BlurAllHor(ae->tmp, blur, scanline,tbl);
	BlurAllVur(ae->tmp, blur, scanline, tbl);

	return err;
}
