

#include "ShineParallel.h"


// **********************************************************
/*in画像からbuf1へ画像を縮小コピー
* 4点中一番明るい色を選ぶ
*/
// **********************************************************
#pragma region Copy in to buf1


PF_Err CopyIntoBuf1_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_Pixel p[4];
			p[0] = PMat8(infoP->inP->GetPix8(2*x, 2*y));
			p[1] = PMat8(infoP->inP->GetPix8(2 * x + 1, 2 * y));
			p[2] = PMat8(infoP->inP->GetPix8(2 * x, 2 * y + 1));
			p[3] = PMat8(infoP->inP->GetPix8(2 * x + 1, 2 * y + 1));


			A_long vmax = p[0].red+ p[0].green + p[0].blue;
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				A_long vv = p[i].red + p[i].green + p[i].blue;
				if (vv > vmax)
				{
					idx = i;
					vmax = vv;
				}
			}
			p[idx].alpha = PF_MAX_CHAN8;
			infoP->bufP1->SetPix8(x, y, p[idx]);
		}

	}



	return err;
}
// **********************************************************
PF_Err CopyIntoBuf1_16(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_Pixel16 p[4];
			p[0] = PMat16(infoP->inP->GetPix16(2 * x, 2 * y));
			p[1] = PMat16(infoP->inP->GetPix16(2 * x + 1, 2 * y));
			p[2] = PMat16(infoP->inP->GetPix16(2 * x, 2 * y + 1));
			p[3] = PMat16(infoP->inP->GetPix16(2 * x + 1, 2 * y + 1));


			PF_FpLong vmax = (PF_FpLong)p[0].red/PF_MAX_CHAN16 + (PF_FpLong)p[0].green/ PF_MAX_CHAN16 + (PF_FpLong)p[0].blue/ PF_MAX_CHAN16;
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				PF_FpLong vv = (PF_FpLong)p[i].red / PF_MAX_CHAN16 + (PF_FpLong)p[i].green / PF_MAX_CHAN16 + (PF_FpLong)p[i].blue / PF_MAX_CHAN16;
				if (vv > vmax)
				{
					idx = i;
					vmax = vv;
				}
			}
			p[idx].alpha = PF_MAX_CHAN16;
			infoP->bufP1->SetPix16(x, y, p[idx]);
		}

	}



	return err;
}
PF_Err CopyIntoBuf1_32(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_PixelFloat p[4];
			p[0] = PMat32(infoP->inP->GetPix32(2 * x, 2 * y));
			p[1] = PMat32(infoP->inP->GetPix32(2 * x + 1, 2 * y));
			p[2] = PMat32(infoP->inP->GetPix32(2 * x, 2 * y + 1));
			p[3] = PMat32(infoP->inP->GetPix32(2 * x + 1, 2 * y + 1));


			PF_FpLong vmax = (PF_FpLong)p[0].red + (PF_FpLong)p[0].green + (PF_FpLong)p[0].blue;
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				PF_FpLong vv = (PF_FpLong)p[i].red + (PF_FpLong)p[i].green + (PF_FpLong)p[i].blue;
				if (vv > vmax)
				{
					idx = i;
					vmax = vv;
				}
			}
			p[idx].alpha = 1;
			infoP->bufP1->SetPix32(x, y, p[idx]);
		}

	}



	return err;
}
#pragma endregion

// **********************************************************
/*in画像からbuf1へ画像を縮小コピー
* 4点中一番明るい色を選ぶ グレー
*/
// **********************************************************
#pragma region CopyG in to buf1


PF_Err CopyIntoBuf1G_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong p[4];
			p[0] = gray8(PMat8(infoP->inP->GetPix8(2 * x, 2 * y)));
			p[1] = gray8(PMat8(infoP->inP->GetPix8(2 * x + 1, 2 * y)));
			p[2] = gray8(PMat8(infoP->inP->GetPix8(2 * x, 2 * y + 1)));
			p[3] = gray8(PMat8(infoP->inP->GetPix8(2 * x + 1, 2 * y + 1)));


			PF_FpLong vmax = p[0];
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				if (p[i] > vmax)
				{
					idx = i;
					vmax = p[i];
				}
			}
			PF_Pixel c = { PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8 };
			c.alpha = RoundByteFpLong(vmax);
			infoP->bufP1->SetPix8(x, y, c);
		}

	}



	return err;
}
// **********************************************************
PF_Err CopyIntoBuf1G_16(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong p[4];
			p[0] = gray16(PMat16(infoP->inP->GetPix16(2 * x, 2 * y)));
			p[1] = gray16(PMat16(infoP->inP->GetPix16(2 * x + 1, 2 * y)));
			p[2] = gray16(PMat16(infoP->inP->GetPix16(2 * x, 2 * y + 1)));
			p[3] = gray16(PMat16(infoP->inP->GetPix16(2 * x + 1, 2 * y + 1)));


			PF_FpLong vmax = p[0];
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				if (p[i] > vmax)
				{
					idx = i;
					vmax = p[i];
				}
			}
			PF_Pixel16 c = { PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16 };
			c.alpha = RoundShortFpLong(vmax);
			infoP->bufP1->SetPix16(x, y, c);
		}

	}


	return err;
}
PF_Err CopyIntoBuf1G_32(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong p[4];
			p[0] = gray32(PMat32(infoP->inP->GetPix32(2 * x, 2 * y)));
			p[1] = gray32(PMat32(infoP->inP->GetPix32(2 * x + 1, 2 * y)));
			p[2] = gray32(PMat32(infoP->inP->GetPix32(2 * x, 2 * y + 1)));
			p[3] = gray32(PMat32(infoP->inP->GetPix32(2 * x + 1, 2 * y + 1)));


			PF_FpLong vmax = p[0];
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				if (p[i] > vmax)
				{
					idx = i;
					vmax = p[i];
				}
			}
			PF_PixelFloat c = { 1,1,1,1 };
			c.alpha = RoundFpShortDouble(vmax);
			infoP->bufP1->SetPix32(x, y, c);
		}

	}


	return err;
}
#pragma endregion

PF_Err CopyBuf1ToDst_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP1->width();
	A_long h2 = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix8(
				x, y,
				infoP->bufP1->GetPixD8(xx, yy)
			);
		}
	}
	return err;
}
PF_Err CopyBuf1ToDst_16(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP1->width();
	A_long h2 = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix16(
				x, y,
				infoP->bufP1->GetPixD16(xx, yy)
			);
		}
	}
	return err;
}
PF_Err CopyBuf1ToDst_32(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP1->width();
	A_long h2 = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix32(
				x, y,
				infoP->bufP1->GetPixD32(xx, yy)
			);
		}
	}
	return err;
}
// **********************************************************
/*buf2画像からoutへ画像を拡大コピー
* 4点中一番明るい色を選ぶ
*/
// **********************************************************
#pragma region Copy buf2 to out


PF_Err CopyBuf2ToDst_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP2->width();
	A_long h2 = infoP->bufP2->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix8(
				x, y,
				infoP->bufP2->GetPixD8(xx, yy)
			);
		}
	}
	return err;
}
PF_Err CopyBuf2ToDst_16(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP2->width();
	A_long h2 = infoP->bufP2->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix16(
				x, y,
				infoP->bufP2->GetPixD16(xx, yy)
			);
		}
	}
	return err;
}
PF_Err CopyBuf2ToDst_32(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP2->width();
	A_long h2 = infoP->bufP2->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix32(
				x, y,
				infoP->bufP2->GetPixD32(xx, yy)
			);
		}
	}
	return err;
}
#pragma endregion



// **********************************************************
/*　shineの実態
*/
// **********************************************************
#pragma region sub


static PF_Err
sub8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	A_long vlen = (A_long)((double)infoP->length / 2 + 0.5);
	if (vlen == 0)
	{
		return err;
	}
	//bufがハーフサイズなので位置補正
	PF_FpLong cx = infoP->pos.x / 2;
	PF_FpLong cy = infoP->pos.y / 2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	//lenDターゲットからの距離
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	// len ターゲットからの距離 long
	A_long len = (A_long)(lenD + 0.5);
	if (len <= 0)
	{
		outP->red = RoundByteDouble((PF_FpLong)inP->red * infoP->strong);
		outP->green = RoundByteDouble((PF_FpLong)inP->green * infoP->strong);
		outP->blue = RoundByteDouble((PF_FpLong)inP->blue * infoP->strong);
		outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
		return err;
	}


	A_long len2 = vlen;

	if (len2 > len)
	{
		len2 = len;
	}

	PF_FpLong r = (PF_FpLong)inP->red;
	PF_FpLong g = (PF_FpLong)inP->green;
	PF_FpLong b = (PF_FpLong)inP->blue;

	if (len2 > 0) {

		PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)len;
		PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)len;

		PF_FpLong xD = (PF_FpLong)xL;
		PF_FpLong yD = (PF_FpLong)yL;
		xD += dx;
		yD += dy;
		for (A_long i = 1; i < len2; i++)
		{
			PF_Pixel c = infoP->bufP1->GetPixD8(xD, yD);
			PF_FpLong av = 1;
			av = ((PF_FpLong)len2 - (PF_FpLong)i) / ((PF_FpLong)len2);
			r += (PF_FpLong)c.red * av;
			g += (PF_FpLong)c.green * av;
			b += (PF_FpLong)c.blue * av;
			xD += dx;
			yD += dy;
		}
		
		r = r * infoP->strong / (PF_FpLong)len2;
		g = g * infoP->strong / (PF_FpLong)len2;
		b = b * infoP->strong / (PF_FpLong)len2;
	}

	
	outP->red = RoundByteDouble(r);
	outP->green = RoundByteDouble(g);
	outP->blue = RoundByteDouble(b);
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
sub16(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	A_long vlen = (A_long)((double)infoP->length / 2 + 0.5);
	if (vlen == 0)
	{
		return err;
	}
	//bufがハーフサイズなので位置補正
	PF_FpLong cx = infoP->pos.x / 2;
	PF_FpLong cy = infoP->pos.y / 2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	//lenDターゲットからの距離
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	// len ターゲットからの距離 long
	A_long len = (A_long)(lenD + 0.5);
	if (len <= 0)
	{
		outP->red = RoundShortFpLong((PF_FpLong)inP->red * infoP->strong);
		outP->green = RoundShortFpLong((PF_FpLong)inP->green * infoP->strong);
		outP->blue = RoundShortFpLong((PF_FpLong)inP->blue * infoP->strong);
		outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
		return err;
	}


	A_long len2 = vlen;

	if (len2 > len)
	{
		len2 = len;
	}

	PF_FpLong r = (PF_FpLong)inP->red;
	PF_FpLong g = (PF_FpLong)inP->green;
	PF_FpLong b = (PF_FpLong)inP->blue;

	if (len2 > 0) {

		PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)len;
		PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)len;

		PF_FpLong xD = (PF_FpLong)xL;
		PF_FpLong yD = (PF_FpLong)yL;
		xD += dx;
		yD += dy;
		for (A_long i = 1; i < len2; i++)
		{
			PF_Pixel16 c = infoP->bufP1->GetPixD16(xD, yD);
			PF_FpLong av = 1;
			av = ((PF_FpLong)len2 - (PF_FpLong)i) / ((PF_FpLong)len2);
			r += (PF_FpLong)c.red * av;
			g += (PF_FpLong)c.green * av;
			b += (PF_FpLong)c.blue * av;
			xD += dx;
			yD += dy;
		}

		r = r * infoP->strong / (PF_FpLong)len2;
		g = g * infoP->strong / (PF_FpLong)len2;
		b = b * infoP->strong / (PF_FpLong)len2;
	}


	outP->red = RoundShortFpLong(r);
	outP->green = RoundShortFpLong(g);
	outP->blue = RoundShortFpLong(b);
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
sub32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	A_long vlen = (A_long)((double)infoP->length / 2 + 0.5);
	if (vlen == 0)
	{
		return err;
	}
	//bufがハーフサイズなので位置補正
	PF_FpLong cx = infoP->pos.x / 2;
	PF_FpLong cy = infoP->pos.y / 2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	//lenDターゲットからの距離
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	// len ターゲットからの距離 long
	A_long len = (A_long)(lenD + 0.5);
	if (len <= 0)
	{
		outP->red = RoundFpShortDouble((PF_FpLong)inP->red * infoP->strong);
		outP->green = RoundFpShortDouble((PF_FpLong)inP->green * infoP->strong);
		outP->blue = RoundFpShortDouble((PF_FpLong)inP->blue * infoP->strong);
		outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
		return err;
	}


	A_long len2 = vlen;

	if (len2 > len)
	{
		len2 = len;
	}

	PF_FpLong r = (PF_FpLong)inP->red;
	PF_FpLong g = (PF_FpLong)inP->green;
	PF_FpLong b = (PF_FpLong)inP->blue;

	if (len2 > 0) {

		PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)len;
		PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)len;

		PF_FpLong xD = (PF_FpLong)xL;
		PF_FpLong yD = (PF_FpLong)yL;
		xD += dx;
		yD += dy;
		for (A_long i = 1; i < len2; i++)
		{
			PF_PixelFloat c = infoP->bufP1->GetPixD32(xD, yD);
			PF_FpLong av = 1;
			av = ((PF_FpLong)len2 - (PF_FpLong)i) / ((PF_FpLong)len2);
			r += (PF_FpLong)c.red * av;
			g += (PF_FpLong)c.green * av;
			b += (PF_FpLong)c.blue * av;
			xD += dx;
			yD += dy;
		}

		r = r * infoP->strong / (PF_FpLong)len2;
		g = g * infoP->strong / (PF_FpLong)len2;
		b = b * infoP->strong / (PF_FpLong)len2;
	}


	outP->red = RoundFpShortDouble(r);
	outP->green = RoundFpShortDouble(g);
	outP->blue = RoundFpShortDouble(b);
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
	return err;
}
#pragma endregion


#pragma region subG

static PF_Err
subG8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	A_long vlen = (A_long)((PF_FpLong)infoP->length / 2+0.5);
	if (vlen <= 0)
	{
		return err;
	}


	//bufがハーフサイズなので位置補正
	PF_FpLong cx = infoP->pos.x / 2;
	PF_FpLong cy = infoP->pos.y / 2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	//lenDターゲットからの距離
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	// len ターゲットからの距離 long
	A_long len = (A_long)(lenD + 0.5);
	if (len <= 0) {
		*outP = infoP->Color;
		outP->alpha = RoundByteFpLong((PF_FpLong)inP->alpha * infoP->strong);
		return err;
	}


	A_long len2 = vlen;
	if (len2 > len)
	{
		len2 = len;
	}

	PF_FpLong a = (PF_FpLong)inP->alpha;
	if (len2 > 0) {

		PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)len;
		PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)len;

		PF_FpLong xD = (PF_FpLong)xL;
		PF_FpLong yD = (PF_FpLong)yL;
		xD += dx;
		yD += dy;
		for (A_long i = 1; i < len2; i++)
		{
			PF_Pixel c = infoP->bufP1->GetPixDA8(xD, yD);
			PF_FpLong av = 1;
			av = ((PF_FpLong)len2 - (PF_FpLong)i) / ((PF_FpLong)len2);
			a += (PF_FpLong)c.alpha * av;
			xD += dx;
			yD += dy;
		}

	}
	*outP = infoP->Color;
	outP->alpha = RoundByteFpLong(a*infoP->strong / (PF_FpLong)len2);
	return err;
}
static PF_Err
subG16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	A_long vlen = (A_long)((PF_FpLong)infoP->length / 2 + 0.5);
	if (vlen <= 0)
	{
		return err;
	}


	//bufがハーフサイズなので位置補正
	PF_FpLong cx = infoP->pos.x / 2;
	PF_FpLong cy = infoP->pos.y / 2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	//lenDターゲットからの距離
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	// len ターゲットからの距離 long
	A_long len = (A_long)(lenD + 0.5);
	if (len <= 0) {
		*outP = infoP->Color16;
		outP->alpha = RoundShortFpLong((PF_FpLong)inP->alpha * infoP->strong);
		return err;
	}


	A_long len2 = vlen;
	if (len2 > len)
	{
		len2 = len;
	}

	PF_FpLong a = (PF_FpLong)inP->alpha;
	if (len2 > 0) {

		PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)len;
		PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)len;

		PF_FpLong xD = (PF_FpLong)xL;
		PF_FpLong yD = (PF_FpLong)yL;
		xD += dx;
		yD += dy;
		for (A_long i = 1; i < len2; i++)
		{
			PF_Pixel16 c = infoP->bufP1->GetPixDA16(xD, yD);
			PF_FpLong av = 1;
			av = ((PF_FpLong)len2 - (PF_FpLong)i) / ((PF_FpLong)len2);
			a += (PF_FpLong)c.alpha * av;
			xD += dx;
			yD += dy;
		}

	}
	*outP = infoP->Color16;
	outP->alpha = RoundShortFpLong(a * infoP->strong / (PF_FpLong)len2);
	return err;
}
static PF_Err
subG32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	A_long vlen = (A_long)((PF_FpLong)infoP->length / 2 + 0.5);
	if (vlen <= 0)
	{
		return err;
	}


	//bufがハーフサイズなので位置補正
	PF_FpLong cx = infoP->pos.x / 2;
	PF_FpLong cy = infoP->pos.y / 2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	//lenDターゲットからの距離
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	// len ターゲットからの距離 long
	A_long len = (A_long)(lenD + 0.5);
	if (len <= 0) {
		*outP = infoP->Color32;
		outP->alpha = RoundFpShortDouble((PF_FpLong)inP->alpha * infoP->strong);
		return err;
	}


	A_long len2 = vlen;
	if (len2 > len)
	{
		len2 = len;
	}

	PF_FpLong a = (PF_FpLong)inP->alpha;
	if (len2 > 0) {

		PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)len;
		PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)len;

		PF_FpLong xD = (PF_FpLong)xL;
		PF_FpLong yD = (PF_FpLong)yL;
		xD += dx;
		yD += dy;
		for (A_long i = 1; i < len2; i++)
		{
			PF_PixelFloat c = infoP->bufP1->GetPixDA32(xD, yD);
			PF_FpLong av = 1;
			av = ((PF_FpLong)len2 - (PF_FpLong)i) / ((PF_FpLong)len2);
			a += (PF_FpLong)c.alpha * av;
			xD += dx;
			yD += dy;
		}
		a = a * infoP->strong / (PF_FpLong)len2;
	}
	*outP = infoP->Color32;
	outP->alpha = RoundFpShortDouble(a);
	return err;
}
#pragma endregion
static PF_Err
subScr8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel c = *inP;
	PF_FpLong v = (PF_FpLong)c.alpha/PF_MAX_CHAN8;
	c.red = RoundByteFpLong((PF_FpLong)c.red * v);
	c.green = RoundByteFpLong((PF_FpLong)c.green * v);
	c.blue = RoundByteFpLong((PF_FpLong)c.blue * v);


	outP->alpha = ScrBlend8( outP->alpha ,c.alpha);
	outP->red = ScrBlend8(outP->red, c.red);
	outP->green = ScrBlend8(outP->green, c.green);
	outP->blue = ScrBlend8(outP->blue, c.blue);


	return err;
}
static PF_Err
subScr16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 c = *inP;
	PF_FpLong v = (PF_FpLong)c.alpha / PF_MAX_CHAN16;
	c.red = RoundShortFpLong((PF_FpLong)c.red * v);
	c.green = RoundShortFpLong((PF_FpLong)c.green * v);
	c.blue = RoundShortFpLong((PF_FpLong)c.blue * v);


	outP->alpha = ScrBlend16(outP->alpha, c.alpha);
	outP->red = ScrBlend16(outP->red, c.red);
	outP->green = ScrBlend16(outP->green, c.green);
	outP->blue = ScrBlend16(outP->blue, c.blue);


	return err;
}
static PF_Err
subScr32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat c = *inP;
	PF_FpLong v = (PF_FpLong)c.alpha;
	c.red = RoundFpShortDouble((PF_FpLong)c.red * v);
	c.green = RoundFpShortDouble((PF_FpLong)c.green * v);
	c.blue = RoundFpShortDouble((PF_FpLong)c.blue * v);


	outP->alpha = ScrBlend32(outP->alpha, c.alpha);
	outP->red = ScrBlend32(outP->red, c.red);
	outP->green = ScrBlend32(outP->green, c.green);
	outP->blue = ScrBlend32(outP->blue, c.blue);


	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************

#pragma region Params


PF_Err ShineParallel::ParamsSetup(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP)
{
	PF_Err err = PF_Err_NONE;
	Init();
	m_cmd = PF_Cmd_PARAMS_SETUP;
	in_data = in_dataP;
	out_data = out_dataP;
	PF_ParamDef		def;
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ROT, 45, ID_Rot);

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LENGTH,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_LENGTH
	);		
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_STRONG,	//Name
		0,						//VALID_MIN
		1000,					//VALID_MAX
		0,						//SLIDER_MIN
		200,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_STRONG
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_ISCOLOR,
		STR_ON,
		FALSE,
		0,
		ID_ISCOLOR
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COLOR,
		0xFF,
		0xFF,
		0xFF,
		ID_COLOR
	);
	//----------------------------------------------------------------


					
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err ShineParallel::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	infoP->offsetX = 0;
	infoP->offsetY = 0;
	infoP->length = 0;
	infoP->strong = 1;

	PF_Fixed r = 0;
	ERR(GetANGLE_FIXED(ID_Rot, &r));
	if (!err)
	{
		r = r % (360<<16);
		if (r < 0) infoP->rot += (360 << 16);
		infoP->rot = (PF_FpLong)r / 65536.0;
	}

	ERR(GetADD(ID_LENGTH, &infoP->length));
	if (!err)
	{
		if (infoP->length < 0) infoP->length = 0;
		if (infoP->length > 0)
		{
			CRot  cr = CRot(in_data);
			cr.SetLength(infoP->length);
			cr.SetRot(infoP->rot);
			infoP->offsetX = cr.x();
			infoP->offsetY = cr.y();

		}
	}
	ERR(GetFLOAT(ID_STRONG, &infoP->strong));
	if (!err)
	{
		infoP->strong /= 100;
		if (infoP->strong < 0) infoP->strong = 0;


	}


	ERR(GetCHECKBOX(ID_ISCOLOR, &infoP->isColor));
	ERR(GetCOLOR(ID_COLOR, &infoP->Color));
	if (!err)
	{
		infoP->Color16 = CONV8TO16(infoP->Color);
		infoP->Color32 = CONV8TO32(infoP->Color);
	}

	return err;
};
#pragma endregion

// **********************************************************
PF_Err ShineParallel::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;


	infoP->inP = new NFsWorld(input, in_data, pixelFormat());
	infoP->outP = new NFsWorld(output, in_data, pixelFormat());

	infoP->length = (A_long)((PF_FpLong)infoP->length * ParamDownScaleX() + 0.5);

	if ((infoP->length <= 0) || (infoP->strong <= 0))
	{
		//infoP->outP->Copy(infoP->inP);
	}
	else {
		PF_EffectWorld buf1 = NewEffectWorld(infoP->outP->width() / 2, infoP->outP->height() / 2, pixelFormat());
		PF_EffectWorld buf2 = NewEffectWorld(infoP->outP->width() / 2, infoP->outP->height() / 2, pixelFormat());
		infoP->bufP1 = new NFsWorld(&buf1, in_data, pixelFormat());
		infoP->bufP2 = new NFsWorld(&buf2, in_data, pixelFormat());
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			if (infoP->isColor)
			{
				CopyIntoBuf1G_32(infoP);
				iterate32(infoP->bufP1->world, (void*)infoP, subG32, infoP->bufP2->world);
				CopyBuf2ToDst_32(infoP);
			}
			else {
				CopyIntoBuf1_32(infoP);
				iterate32(infoP->bufP1->world, (void*)infoP, sub32, infoP->bufP2->world);
				CopyBuf2ToDst_32(infoP);
				infoP->outP->FromMat();
			}
			break;
		case PF_PixelFormat_ARGB64:
			if (infoP->isColor)
			{
				CopyIntoBuf1G_16(infoP);
				iterate16(infoP->bufP1->world, (void*)infoP, subG16, infoP->bufP2->world);
				CopyBuf2ToDst_16(infoP);
			}
			else {
				CopyIntoBuf1_16(infoP);
				iterate16(infoP->bufP1->world, (void*)infoP, sub16, infoP->bufP2->world);
				CopyBuf2ToDst_16(infoP);
				infoP->outP->FromMat();
			}
			break;
		case PF_PixelFormat_ARGB32:
			if (infoP->isColor)
			{
				CopyIntoBuf1G_8(infoP);
				iterate8(infoP->bufP1->world, (void*)infoP, subG8, infoP->bufP2->world);
				CopyBuf2ToDst_8(infoP);
			}
			else {
				CopyIntoBuf1_8(infoP);
				iterate8(infoP->bufP1->world, (void*)infoP, sub8, infoP->bufP2->world);
				CopyBuf2ToDst_8(infoP);

				infoP->outP->FromMat();
			}
			break;
		default:
			break;
		}

		delete infoP->bufP1;
		delete infoP->bufP2;
	}
	delete infoP->inP;
	delete infoP->outP;
	return err;

};
// **********************************************************

