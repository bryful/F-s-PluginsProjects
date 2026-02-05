#include "Max_Kasumi.h"

//-------------------------------------------------------------------------------------------------
static PF_Err
Rev(CFsAE *ae, ParamInfo32 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_PixelFloat *data = pi->data;

	A_long pos = 0;
	for (A_long y = 0; y < pi->height; y++)
	{
		for (A_long x = 0; x < pi->width; x++)
		{
			PF_FpShort r = data[pos].red;
			PF_FpShort g = data[pos].green;
			PF_FpShort b = data[pos].blue;
			PF_FpShort a = data[pos].alpha;
			if (r > 1.0) r = 1.0;
			if (g > 1.0) g = 1.0;
			if (b > 1.0) b = 1.0;
			if (a > 1.0) a = 1.0;
			data[pos].red = (PF_FpShort)(1.0 - r);
			data[pos].green = (PF_FpShort)(11.0 - g);
			data[pos].blue = (PF_FpShort)(11.0 - b);
			data[pos].alpha = (PF_FpShort)(11.0 - a);
			pos++;
		}
		pos += pi->widthOffset;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxHorRGB32(CFsAE *ae, ParamInfo32 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_PixelFloat *data = pi->data;

	PF_FpShort a;
	PF_FpShort r, g, b, v,p;
	PF_FpShort r1, g1, b1, v1;
	PF_FpShort r2, g2, b2, v2;
	A_long hor;
	for (A_long y = 0; y < pi->height; y++)
	{
		hor = y * pi->widthTrue;
		for (A_long x = 0; x < pi->width; x++) {
			pi->scanline[x] = data[x + hor];
		}
		for (A_long x = 0; x < pi->width; x++) {
			r = pi->scanline[x].red;
			g = pi->scanline[x].green;
			b = pi->scanline[x].blue;
			a = pi->scanline[x].alpha;

			v = r + g + b;

			r1 = r2 = r;
			g1 = g2 = g;
			b1 = b2 = b;

			if (x > 0) {
				r1 = pi->scanline[x - 1].red;
				g1 = pi->scanline[x - 1].green;
				b1 = pi->scanline[x - 1].blue;
				v1 = r1 + g1 + b1;
			}
			if (x < pi->width - 1) {
				r2 = pi->scanline[x + 1].red;
				g2 = pi->scanline[x + 1].green;
				b2 = pi->scanline[x + 1].blue;
				v2 = r2 + g2 + b2;
			}
			if (v1 < v2) {
				r1 = r2;
				g1 = g2;
				b1 = b2;
				v1 = v2;
			}
			if (v <v1) {
				p = r1 / 4;
				r = r + p - r * p;
				if (r > r1) r = r1;
				p = g1 / 4;
				g = g + p - g * p;
				if (g > g1) g = g1;
				p = b1 / 4;
				b = b + p - b * p;
				if (b > b1) b = b1;

				PF_PixelFloat c;
				c.red = (PF_FpShort)r;
				c.green = (PF_FpShort)g;
				c.blue = (PF_FpShort)b;
				c.alpha = a;
				data[x + hor] = c;
			}

		}

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxVerRGB32(CFsAE *ae, ParamInfo32 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_PixelFloat *data = pi->data;

	PF_FpShort a;
	PF_FpShort r, g, b, v, p;
	PF_FpShort r1, g1, b1, v1;
	PF_FpShort r2, g2, b2, v2;

	A_long hor;

	for (A_long x = 0; x < pi->width; x++)
	{
		hor = 0;
		for (A_long y = 0; y < pi->height; y++) {
			pi->scanline[y] = data[x + hor];
			hor += pi->widthTrue;
		}

		hor = 0;
		for (A_long y = 0; y < pi->height; y++) {
			r = pi->scanline[y].red;
			g = pi->scanline[y].green;
			b = pi->scanline[y].blue;
			a = pi->scanline[y].alpha;

			v = r + g + b;


			r1 = r2 = r;
			g1 = g2 = g;
			b1 = b2 = b;

			if (y > 0) {
				r1 = pi->scanline[y - 1].red;
				g1 = pi->scanline[y - 1].green;
				b1 = pi->scanline[y - 1].blue;
				v1 = r1 + g1 + b1;
			}
			if (y < pi->height - 1) {
				r2 = pi->scanline[y + 1].red;
				g2 = pi->scanline[y + 1].green;
				b2 = pi->scanline[y + 1].blue;
				v2 = r2 + g2 + b2;
			}
			if (v1 < v2) {
				r1 = r2;
				g1 = g2;
				b1 = b2;
				v1 = v2;
			}
			if (v < v1) {
				p = r1 / 4;
				r = r + p - r * p;
				if (r > r1) r = r1;
				p = g1 / 4;
				g = g + p - g * p;
				if (g > g1) g = g1;
				p = b1 / 4;
				b = b + p - b * p;
				if (b > b1) b = b1;
				PF_PixelFloat c;
				c.red = (PF_FpShort)r;
				c.green = (PF_FpShort)g;
				c.blue = (PF_FpShort)b;
				c.alpha = a;
				data[x + hor] = c;
			}
			hor += pi->widthTrue;

		}

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxHorA32(CFsAE *ae, ParamInfo32 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_PixelFloat *data = pi->data;

	PF_FpShort a, p;
	PF_FpShort r, g, b;
	PF_FpShort a1, a2;
	A_long hor;

	for (A_long y = 0; y < pi->height; y++)
	{
		hor = y * pi->widthTrue;
		for (A_long x = 0; x < pi->width; x++) {
			pi->scanline[x] = data[x + hor];
		}
		for (A_long x = 0; x < pi->width; x++) {
			r = pi->scanline[x].red;
			g = pi->scanline[x].green;
			b = pi->scanline[x].blue;
			a = pi->scanline[x].alpha;

			a1 = a2 = a;

			if (x > 0) {
				a1 = pi->scanline[x - 1].alpha;
			}
			if (x < pi->width - 1) {
				a2 = pi->scanline[x + 1].alpha;
			}
			if (a1 < a2) {
				a1 = a2;
			}
			if (a < a1) {
				p = a1 / 4;
				a = a + p - a * p;
				if (a > a1) a = a1;

				PF_PixelFloat c;
				c.red = r;
				c.green = g;
				c.blue = b;
				c.alpha = (PF_FpShort)a;
				data[x + hor] = c;
			}

		}

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxVerA32(CFsAE *ae, ParamInfo32 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_PixelFloat *data = pi->data;

	PF_FpShort a, p;
	PF_FpShort r, g, b;
	PF_FpShort a1, a2;
	A_long hor;

	for (A_long x = 0; x < pi->width; x++)
	{
		hor = 0;
		for (A_long y = 0; y < pi->height; y++) {
			pi->scanline[y] = data[x + hor];
			hor += pi->widthTrue;
		}
		hor = 0;
		for (A_long y = 0; y < pi->height; y++) {
			r = pi->scanline[y].red;
			g = pi->scanline[y].green;
			b = pi->scanline[y].blue;
			a = pi->scanline[y].alpha;

			a1 = a2 = a;

			if (x > 0) {
				a1 = pi->scanline[y - 1].alpha;
			}
			if (x < pi->width - 1) {
				a2 = pi->scanline[y + 1].alpha;
			}
			if (a1 < a2) {
				a1 = a2;
			}
			if (a < a1) {
				p = a1 / 4;
				a = a + p - a * p;
				if (a > a1) a = a1;

				PF_PixelFloat c;
				c.red = r;
				c.green = g;
				c.blue = b;
				c.alpha = (PF_FpShort)a;
				data[x + hor] = c;
			}
			hor += pi->widthTrue;

		}

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err ToParam32(CFsAE *ae, ParamInfo *infoP, ParamInfo32 *pi)
{
	PF_Err	err = PF_Err_NONE;


	pi->width = ae->out->width();
	pi->widthTrue = ae->out->widthTrue();
	pi->widthOffset = ae->out->offsetWidth();
	pi->height = ae->out->height();
	pi->data = (PF_PixelFloat *)ae->out->data();
	pi->in_data = ae->in_data;

	pi->bufSize = pi->width;
	if (pi->bufSize < pi->height) pi->bufSize = pi->height;

	pi->info = *infoP;



	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err Exec32(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if (ae->out->Enabled() == FALSE)
	{
		return PF_Err_INVALID_CALLBACK;
	}
	ParamInfo32 pi;

	ToParam32(ae, infoP, &pi);
	if (pi.info.max <= 0) {
		return err;
	}

	pi.bufH = ae->NewHandle(pi.bufSize * sizeof(PF_PixelFloat) * 2);
	if (pi.bufH == NULL) {
		err = PF_Err_OUT_OF_MEMORY;
		return err;
	}
	pi.scanline = *(PF_PixelFloat**)pi.bufH;

	
	ae->out->toBlackMat32();
	if (pi.info.minus)Rev(ae, &pi);

	if ((pi.info.ch == 1) || (pi.info.ch == 2)) {
		for (A_long i = 0; i < pi.info.max; i++)
		{
			if ((pi.info.dir == 1) || (pi.info.dir == 2)) {
				MaxHorRGB32(ae, &pi);
			}
			if ((pi.info.dir == 1) || (pi.info.dir == 3)) {
				MaxVerRGB32(ae, &pi);
			}
		}
	}
	if ((pi.info.ch == 1) || (pi.info.ch == 3)) {
		for (A_long i = 0; i < pi.info.max; i++)
		{
			if ((pi.info.dir == 1) || (pi.info.dir == 2)) {
				MaxHorA32(ae, &pi);
			}
			if ((pi.info.dir == 1) || (pi.info.dir == 3)) {
				MaxVerA32(ae, &pi);
			}
		}
	}


	if (pi.info.minus)Rev(ae, &pi);
	ae->out->SetMatMode(MAT::blackMat);
	ae->out->fromBlackMat32();

	if (pi.bufH != NULL) {
		ae->DisposeHandle(pi.bufH);
		pi.bufH = NULL;
	}


	return err;

}
