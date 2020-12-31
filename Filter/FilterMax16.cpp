#include "Filter.h"

static PF_Err MaxAllHor(PF_EffectWorldPtr g, A_long max, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel16* data = (PF_Pixel16*)g->data;

	A_long w = g->width / 2;
	A_long wt = g->rowbytes / sizeof(PF_Pixel16);
	A_long h = g->height / 2;

	PF_Pixel16* scanline = *(PF_Pixel16**)bufH;
	A_u_short* scanlineLv = (A_u_short *)(scanline + wt);

	A_long target = 0;
	for (A_long y = 0; y < h; y++)
	{
		A_long ypos = y * wt;

		A_u_short mv = 0;
		//scanline mvで最大値を記憶しておく
		for (A_long i = 0; i < w; i++)
		{
			scanline[i] = data[ypos + i];
			scanlineLv[i] = RoundShort(((A_long)data[ypos + i].red + (A_long)data[ypos + i].green + (A_long)data[ypos + i].blue) / 3);
			if (mv < scanlineLv[i]) mv = scanlineLv[i];
		}
		if (mv <= 0) continue; // 最大値が０なら必要なし

		for (A_long x = 0; x < w; x++)
		{
			A_u_short t = 0;
			PF_Pixel16 c = { 0,0,0,0 };
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
static PF_Err MaxAllVur(PF_EffectWorldPtr g, A_long max, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16* data = (PF_Pixel16*)g->data;
	A_long w = g->width / 2;
	A_long wt = g->rowbytes / sizeof(PF_Pixel16);
	A_long h = g->height / 2;

	PF_Pixel16* scanline = *(PF_Pixel16**)bufH;
	A_u_short* scanlineLv = (A_u_short *)(scanline + wt);


	A_long target = 0;
	for (A_long x = 0; x < w; x++)
	{

		A_u_short mv = 0;
		A_long ypos = 0;
		//scanline mvで最大値を記憶しておく
		for (A_long i = 0; i < h; i++)
		{
			A_long p = x + ypos;
			scanline[i] = data[p];
			scanlineLv[i] = RoundShort(((A_long)data[p].red + (A_long)data[p].green + (A_long)data[p].blue) / 3);
			if (mv < scanlineLv[i]) mv = scanlineLv[i];
			ypos += wt;
		}
		if (mv <= 0) continue; // 最大値が０なら必要なし
		ypos = 0;
		for (A_long y = 0; y < h; y++)
		{
			A_u_short t = 0;
			PF_Pixel16 c = { 0,0,0,0 };
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
PF_Err Max16(PF_EffectWorldPtr world, A_long max, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	if (max == 0) return err;
	MaxAllHor(world, max, bufH);
	MaxAllVur(world, max, bufH);

	return err;
}
