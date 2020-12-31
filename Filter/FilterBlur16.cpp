#include "Filter.h"

//==================================================================================
static PF_Err MakeTbl(CAE *ae, A_long blur, A_long *tbl)
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
static PF_Err BlurHor(PF_EffectWorldPtr g, A_long blue, PF_Pixel16 *scanline, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data;

	A_long w = g->width/2;
	A_long wt = g->rowbytes /sizeof(PF_Pixel16);
	A_long h = g->height/2;

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

static PF_Err BlurVur(PF_EffectWorldPtr g, A_long blue, PF_Pixel16 *scanline, A_long *tbl)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data;

	A_long w = g->width / 2;
	A_long wt = g->rowbytes /sizeof(PF_Pixel16);
	A_long h = g->height / 2;

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


PF_Err Blur16(CAE *ae,  PF_EffectWorldPtr world, A_long blur, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	if (blur == 0) return err;
	PF_InData *in_data;
	in_data = ae->in_data;

	//scanline
	PF_Pixel16* scanline = *(PF_Pixel16**)bufH;

	A_long w = world->width;
	if (w < world->height) w = world->height;
	//table
	A_long * tbl = (A_long *)(scanline + w);

	blur = (A_long)PF_CEIL(blur);
	if (blur < 1) blur = 1;
	MakeTbl(ae, blur, tbl);

	BlurHor(world, blur, scanline,tbl);
	BlurVur(world, blur, scanline, tbl);

	return err;
}
