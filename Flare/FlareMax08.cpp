#include "Flare.h"

//==================================================================================
static PF_Err MaxRedHor(CFsGraph *g, A_long max, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel* data = (PF_Pixel*)g->data();

	A_long w = g->width()/2;
	A_long wt = g->widthTrue();
	A_long h = g->height()/2;

	A_u_char* scanline = *(A_u_char**)bufH;

	A_long target = 0;
	for (A_long y = 0; y < h; y++)
	{
		A_long ypos = y * wt;

		A_u_char mv = 0;
		//scanline mvで最大値を記憶しておく
		for (A_long i = 0; i < w; i++) 
		{
			scanline[i] = data[ypos + i].red;
			if (mv < scanline[i]) mv = scanline[i];
		}
		if (mv <= 0) continue; // 最大値が０なら必要なし

		for (A_long x = 0; x < w; x++)
		{
			A_u_char t = 0;
			for (A_long i = -max; i <= max; i++)
			{
				A_long x2 = x + i;
				if (x2 < 0) continue;
				if (x2 >= w) break;
				if (t<scanline[x2] )
				{
					t = scanline[x2];
				}
				if (t >= mv) break;
			}
			data[ypos + x].red = t;
			//data[ypos + x].green = t;
			//data[ypos + x].blue = t;
			//data[ypos + x].alpha = t;

		}
	}
	return err;
}
static PF_Err MaxRedVur(CFsGraph *g, A_long max,PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel* data = (PF_Pixel*)g->data();
		A_long w = g->width() / 2;
	A_long wt = g->widthTrue();
	A_long h = g->height() / 2;

	A_u_char* scanline = *(A_u_char**)bufH;


	A_long target = 0;
	for (A_long x = 0; x < w; x++)
	{

		A_u_char mv = 0;
		A_long ypos = 0;
		//scanline mvで最大値を記憶しておく
		for (A_long i = 0; i < h; i++)
		{
			scanline[i] = data[x + ypos].red;
			if (mv < scanline[i]) mv = scanline[i];
			ypos += wt;
		}
		if (mv <= 0) continue; // 最大値が０なら必要なし
		ypos = 0;
		for (A_long y = 0; y < h; y++)
		{
			A_u_char t = 0;
			for (A_long i = -max; i <= max; i++)
			{
				A_long y2 = y + i;
				if ((y2 >= 0) && (y2 < h))
				{
					if (t < scanline[y2])
					{
						t = scanline[y2];
					}
					if (t >= mv) break;

				}
			}
			data[ypos + x].red = t;
			//data[ypos + x].green = t;
			//data[ypos + x].blue = t;
			//data[ypos + x].alpha = t;
			ypos += wt;
		}
	}
	return err;
}
//==================================================================================
/*redチャンネルのみ対応
*/
PF_Err MaxRed08(CFsAE *ae, FlareInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long max = infoP->max;
	if (max == 0) return err;
	MaxRedHor(ae->tmp, max,infoP->bufH);
	MaxRedVur(ae->tmp, max, infoP->bufH);

	return err;
}
//==================================================================================
static PF_Err BorderFin(CFsGraph *g)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel* data = g->data();
	A_long wt = g->widthTrue();
	A_long w = g->width() / 2;
	A_long h = g->height() / 2;

	A_long ypos = 0;
	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			A_long xx = x + ypos;
			data[xx].red = RoundByteLong( (long)data[xx].green - (long)data[xx].red);
		}
		ypos += wt;
	}

	return err;
}
PF_Err Border08(CFsAE *ae, FlareInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;

	A_long inside = infoP->inside;
	A_long outside = infoP->outside;
	//if ((inside <= 0) && (outside <= 0)) return err;

	if (outside > 0) {
		MaxRedHor(ae->tmp, outside, infoP->bufH);
		MaxRedVur(ae->tmp, outside, infoP->bufH);
	}
	RedToGreen08(ae->tmp);
	if (inside > 0)
	{
		RevRedChannel08(ae->tmp);
		MaxRedHor(ae->tmp, inside, infoP->bufH);
		MaxRedVur(ae->tmp, inside, infoP->bufH);
		RevRedChannel08(ae->tmp);
	}
	BorderFin(ae->tmp);

	return err;
}

static PF_Err MaxAllHor(CFsGraph *g, A_long max, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel* data = (PF_Pixel*)g->data();

	A_long w = g->width() / 2;
	A_long wt = g->widthTrue();
	A_long h = g->height() / 2;

	PF_Pixel* scanline = *(PF_Pixel**)bufH;
	A_u_char* scanlineLv = (A_u_char *)(scanline + wt);

	A_long target = 0;
	for (A_long y = 0; y < h; y++)
	{
		A_long ypos = y * wt;

		A_u_char mv = 0;
		//scanline mvで最大値を記憶しておく
		for (A_long i = 0; i < w; i++)
		{
			scanline[i] = data[ypos + i];
			scanlineLv[i] = RoundByteLong(((A_long)data[ypos + i].red + (A_long)data[ypos + i].green + (A_long)data[ypos + i].blue) / 3);
			if (mv < scanlineLv[i]) mv = scanlineLv[i];
		}
		if (mv <= 0) continue; // 最大値が０なら必要なし

		for (A_long x = 0; x < w; x++)
		{
			A_u_char t = 0;
			PF_Pixel c = { 0,0,0,0 };
			for (A_long i = -max; i <= max; i++)
			{
				A_long x2 = x + i;
				if (x2 < 0) continue;
				if (x2 >= w) break;
				if (t < scanlineLv[x2])
				{
					t = scanlineLv[x2];
					c = scanline[x2];
				}
				if (t >= mv) break;
			}
			data[ypos + x] = c;

		}
	}
	return err;
}
static PF_Err MaxAllVur(CFsGraph *g, A_long max, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel* data = (PF_Pixel*)g->data();
	A_long w = g->width() / 2;
	A_long wt = g->widthTrue();
	A_long h = g->height() / 2;

	PF_Pixel* scanline = *(PF_Pixel**)bufH;
	A_u_char* scanlineLv = (A_u_char *)(scanline + wt);


	A_long target = 0;
	for (A_long x = 0; x < w; x++)
	{

		A_u_char mv = 0;
		A_long ypos = 0;
		//scanline mvで最大値を記憶しておく
		for (A_long i = 0; i < h; i++)
		{
			A_long p = x + ypos;
			scanline[i] = data[p];
			scanlineLv[i] = RoundByteLong(((A_long)data[p].red + (A_long)data[p].green + (A_long)data[p].blue) / 3);
			if (mv < scanlineLv[i]) mv = scanlineLv[i];
			ypos += wt;
		}
		if (mv <= 0) continue; // 最大値が０なら必要なし
		ypos = 0;
		for (A_long y = 0; y < h; y++)
		{
			A_u_char t = 0;
			PF_Pixel c = { 0,0,0,0 };
			for (A_long i = -max; i <= max; i++)
			{
				A_long y2 = y + i;
				if ((y2 >= 0) && (y2 < h))
				{
					if (t < scanlineLv[y2])
					{
						t = scanlineLv[y2];
						c = scanline[y2];
					}
					if (t >= mv) break;

				}
			}
			data[ypos + x] = c;
			ypos += wt;
		}
	}
	return err;
}
PF_Err MaxAll08(CFsAE *ae, FlareInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long max = infoP->max;
	if (max == 0) return err;
	MaxAllHor(ae->tmp, max, infoP->bufH);
	MaxAllVur(ae->tmp, max, infoP->bufH);

	return err;
}
