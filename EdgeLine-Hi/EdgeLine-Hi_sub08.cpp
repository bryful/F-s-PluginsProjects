//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "EdgeLine-Hi.h"

PF_Err EdgeLineHor8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->xscan == 0) return err;
	PF_Pixel* data;
	data = (PF_Pixel*)ae->out->data();
	PF_Pixel* scanline;
	scanline = *(PF_Pixel8**)infoP->scanlineH;
	A_long x = 0;
	A_long y = 0;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();

	PF_Pixel target = infoP->target_color;
	PF_Pixel sample = infoP->sample_color;
	A_u_char lv = RoundByteDouble((double)PF_MAX_CHAN8 * infoP->level + 0.5);
	PF_Pixel draw = infoP->draw_color;

	A_long length = infoP->xscan;
	A_long minus = 1;
	if (length < 0) {
		length *= -1;
		minus = -1;
	}
	A_long adr = 0;
	A_long yAdr = 0;
	//PF_Pixel WHT = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
	//PF_Pixel BL = { PF_MAX_CHAN8,0, 0, 0 };
	for (A_long y = 0; y < h; y++)
	{

		for (A_long x = 0; x < w; x++) 
			scanline[x] = data[yAdr + x];
		for (A_long x = 0; x < w; x++) {

			if (compPix8Lv(target, scanline[x], lv)==TRUE)
			{
				for (A_long x2 = 1; x2 <= length; x2++)
				{
					A_long xx = x + (x2*minus);
					if ((xx < 0) || (xx >= w)) break;
					if (compPix8Lv(sample, scanline[xx], lv)==TRUE)
					{
						data[yAdr + x] = draw;
						break;
					}
					else if(infoP->GiveUpSoon==TRUE)
					{
						if (compPix8Lv(target, scanline[xx], lv) == FALSE)
							break;
					}
				}
			}
		}
		yAdr += wt;

	}
		return err;
}
PF_Err EdgeLineVur8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->xscan == 0) return err;
	PF_Pixel* data;
	data = (PF_Pixel*)ae->out->data();
	PF_Pixel* scanline;
	scanline = *(PF_Pixel8**)infoP->scanlineH;
	A_long x = 0;
	A_long y = 0;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();

	PF_Pixel target = infoP->target_color;
	PF_Pixel sample = infoP->sample_color;
	A_u_char lv = RoundByteDouble((double)PF_MAX_CHAN8 * infoP->level + 0.5);
	PF_Pixel draw = infoP->draw_color;

	A_long length = infoP->yscan;
	A_long minus = 1;
	if (length < 0) {
		length *= -1;
		minus = -1;
	}
	A_long adr = 0;
	A_long yAdr = 0;
	for (A_long x = 0; x < w; x++)
	{
		yAdr = x;
		for (A_long y = 0; y < h; y++) {
			scanline[y] = data[yAdr];
			yAdr += wt;
		}
		yAdr = x;
		for (A_long y = 0; y < h; y++) 
		{
			if (compPix8Lv(target, scanline[y], lv))
			{
				for (A_long y2 = 1; y2 < length; y2++)
				{
					A_long yy = y + (y2*minus);
					if ((yy < 0) || (yy >= w)) break;
					if (compPix8Lv(sample, scanline[yy], lv))
					{
						data[yAdr] = draw;
						break;
					}
					else if (infoP->GiveUpSoon == TRUE)
					{
						if (compPix8Lv(target, scanline[yy], lv) == FALSE)
							break;
					}
				}
			}
			yAdr += wt;
		}

	}
	return err;
}
//----------------------------------------------------------------------------

PF_Err EdgeLineSub8(CFsAE *ae, ParamInfo *infoP)
{
	PF_InData *in_data;
	in_data = ae->in_data;

	PF_Err err = PF_Err_NONE;
	
	//スキャンラインバッファーの確保
	A_long w = ae->out->width();
	if (ae->out->height() > w) w = ae->out->height();
	infoP->scanlineH = PF_NEW_HANDLE(w * sizeof(PF_Pixel) * 2); //念のため2倍サイズ確保
	if (!infoP->scanlineH) return PF_Err_OUT_OF_MEMORY;

	EdgeLineHor8(ae, infoP);
	EdgeLineVur8(ae, infoP);

	PF_DISPOSE_HANDLE(infoP->scanlineH);
	
	return err;
}
//----------------------------------------------------------------------------
