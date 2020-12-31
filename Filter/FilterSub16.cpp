#include "Filter.h"


PF_Err ToHarfSize16(PF_EffectWorldPtr world)
{
	PF_Err			err = PF_Err_NONE;
	A_long ow = world->width;
	A_long owt = world->rowbytes / sizeof(PF_Pixel16);
	A_long oh = world->height;

	A_long wh = ow / 2;
	A_long hh = oh / 2;
	PF_Pixel16* data = (PF_Pixel16*)world->data;
	PF_Pixel16 bl = { 0,0,0,0 };

	//まず水平方向
	for (A_long y = 0; y < oh; y++)
	{
		A_long ypos = y * owt;
		for (A_long x = 0; x < wh; x++)
		{
			PF_Pixel16 c0, c1, c2;
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
			data[x + ypos2] = c2;
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
PF_Err ToDoubleSize16(PF_EffectWorldPtr world, PF_Handle bufH)
{
	PF_Err			err = PF_Err_NONE;
	A_long ow = world->width;
	A_long owt = world->rowbytes / sizeof(PF_Pixel16);
	A_long oh = world->height;

	A_long hw = ow / 2;
	A_long hh = oh / 2;
	PF_Pixel16* data = (PF_Pixel16*)world->data;
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
			A_long x1 = x; if (x1 >= hw) x1 = hw - 1;
			A_long x2 = x + 1; if (x2 >= hw) x2 = hw - 1;
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
// ****************************************************************************************
//=========================================================================================
typedef struct ExtractInfo
{
	A_long start;
	A_long last;
	A_long soft;
}ExtractInfo, *ExtractInfoP, **ExtractInfoH;

//=========================================================================================
PF_Err
ExtractHiSub(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*px)
{
	PF_Err			err = PF_Err_NONE;
	ExtractInfo *	infoP = reinterpret_cast<ExtractInfo*>(refcon);
	PF_Pixel16 bl = { 0,0,0,0 };

	A_long v = (px->red + px->green + px->blue) / 3;

	if (v<= infoP->start)
	{
		*px = bl;
	}
	else if (v < infoP->last)
	{
		A_long p = PF_MAX_CHAN16 *(v - infoP->start) / infoP->soft;
		px->red = RoundShort((A_long)px->red * p / PF_MAX_CHAN16);
		px->green = RoundShort((A_long)px->green * p / PF_MAX_CHAN16);
		px->blue = RoundShort((A_long)px->blue * p / PF_MAX_CHAN16);
		px->alpha = RoundShort((A_long)px->alpha * p / PF_MAX_CHAN16);
	}
	return err;
}

//=========================================================================================
PF_Err ExtractHi16(PF_EffectWorldPtr world, PF_FpLong wp, PF_FpLong sf)
{
	PF_Err			err = PF_Err_NONE;
	ExtractInfo ei;
	ei.last = (A_long)(wp * PF_MAX_CHAN16 + 0.5);
	ei.soft = (A_long)(sf * PF_MAX_CHAN16 + 0.5);
	ei.start = ei.last - ei.soft;

	ERR(iterateWorld16(&ei, world, ExtractHiSub));

	return err;
}
//=========================================================================================
PF_Err
ExtractLoSub(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*px)
{
	PF_Err			err = PF_Err_NONE;
	ExtractInfo *	infoP = reinterpret_cast<ExtractInfo*>(refcon);
	PF_Pixel16 bl = { 0,0,0,0 };

	A_long v = (px->red + px->green + px->blue) / 3;

	if (v >= infoP->last)
	{
		*px = bl;
	}
	else if (v > infoP->start)
	{
		A_long p = PF_MAX_CHAN16 * (infoP->last -v) / infoP->soft;
		px->red = RoundShort((A_long)px->red * p / PF_MAX_CHAN16);
		px->green = RoundShort((A_long)px->green * p / PF_MAX_CHAN16);
		px->blue = RoundShort((A_long)px->blue * p / PF_MAX_CHAN16);
		px->alpha = RoundShort((A_long)px->alpha * p / PF_MAX_CHAN16);

	}
	return err;
}
//=========================================================================================
PF_Err ExtractLo16(PF_EffectWorldPtr world, PF_FpLong bp, PF_FpLong sf)
{
	PF_Err			err = PF_Err_NONE;
	ExtractInfo ei;
	ei.start = (A_long)(bp * PF_MAX_CHAN16 + 0.5);
	ei.soft = (A_long)(sf  * PF_MAX_CHAN16 + 0.5);
	ei.last = ei.start + ei.soft;

	ERR(iterateWorld16(&ei, world, ExtractLoSub));

	return err;
}
//=========================================================================================
//=========================================================================================
PF_Err
RevSub(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*px)
{
	PF_Err			err = PF_Err_NONE;
	px->alpha = PF_MAX_CHAN16 - px->alpha;
	px->red = PF_MAX_CHAN16 - px->red;
	px->green = PF_MAX_CHAN16 - px->green;
	px->blue = PF_MAX_CHAN16 - px->blue;
	return err;
}
//=========================================================================================
PF_Err Rev16(PF_EffectWorldPtr world)
{
	PF_Err			err = PF_Err_NONE;

	ERR(iterateWorld16(NULL, world, RevSub));

	return err;
}
//=========================================================================================
typedef struct BrightInfo
{
	A_long brightness;
}BrightInfo, *BrightInfoP, **BrightInfoH;
//=========================================================================================
PF_Err
BrightnessSub(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*px)
{
	PF_Err			err = PF_Err_NONE;
	BrightInfo *	infoP = reinterpret_cast<BrightInfo*>(refcon);
	PF_Pixel16 bl = { 0,0,0,0 };

	px->red = RoundShort(px->red + infoP->brightness);
	px->green = RoundShort(px->green + infoP->brightness);
	px->blue = RoundShort(px->blue + infoP->brightness);

	
	return err;
}
//=========================================================================================
PF_Err Brightness16(PF_EffectWorldPtr world,PF_FpLong brigtness)
{
	PF_Err			err = PF_Err_NONE;
	BrightInfo bi;
	bi.brightness = (A_long)(PF_MAX_CHAN16 * brigtness);
	ERR(iterateWorld16(&bi, world, BrightnessSub));
	return err;
}

