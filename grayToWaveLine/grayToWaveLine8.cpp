#include "grayToWaveLine.h"


typedef struct LineInfo8 {
	A_long		x0;
	A_long		y0;
	A_long		x1;
	A_long		y1;
	PF_Pixel	col;
	PF_Pixel*	data;
	A_long		width;
	A_long		widthTrue;
	A_long		height;
	A_long		lineHeight;

} LineInfo8, * LineInfo8P, ** LineInfo8H;

//*****************************************************************************
static PF_Err Line8(LineInfo8 *li)
{
	PF_Err	err = PF_Err_NONE;

	A_long E;
	A_long dx, dy, sx, sy;


	sx = (li->x1 > li->x0) ? 1 : -1;
	dx = (li->x1 > li->x0) ? li->x1 - li->x0 : li->x0 - li->x1;
	sy = (li->y1 > li->y0) ? 1 : -1;
	dy = (li->y1 > li->y0) ? li->y1 - li->y0 : li->y0 - li->y1;

	A_long x = li->x0;
	A_long y = li->y0;
	/* 傾きが1以下の場合 */
	if (dx >= dy) {
		E = -dx;
		for (A_long i = 0; i <= dx; i++) {

			for (A_long lh = 0; lh < li->lineHeight; lh++)
			{
				A_long y2 = y + lh;
				if ((x >= 0) && (x < li->width) && (y2 >= 0) && (y2< li->height))
				{
					li->data[x + y2 * li->widthTrue] = li->col;
				}

			}
			x += sx;
			E += 2 * dy;
			if (E >= 0) {
				y += sy;
				E -= 2 * dx;
			}
		}
		/* 傾きが1より大きい場合 */
	}
	else {
		E = -dy;
		for (A_long i = 0; i <= dy; i++) {
			for (A_long lh = 0; lh < li->lineHeight; lh++)
			{
				A_long y2 = y + lh;
				if ((x >= 0) && (x < li->width) && (y2 >= 0) && (y2 < li->height))
				{
					li->data[x + y2 * li->widthTrue] = li->col;
				}

			}
			y += sy;
			E += 2 * dx;
			if (E >= 0) {
				x += sx;
				E -= 2 * dy;
			}
		}
	}
	return err;
}
//*****************************************************************************
static PF_Err drawGraph8(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;
	LineInfo8 li;
	li.data = (PF_Pixel *)ae->out->data();
	li.width = ae->out->width();
	li.widthTrue = ae->out->widthTrue();
	li.height = ae->out->height();
	li.col = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
	li.lineHeight = infoP->LineHeight;


	if ((infoP->mode == MODE_ALL) || (infoP->mode == MODE_BASE)) {
		li.col = infoP->Base_Color;
		for (A_long yc = 0; yc < infoP->SYCount; yc++)
		{
			A_long xPos = infoP->Draw_X + yc * infoP->Draw_X_Offset;
			A_long yPos = infoP->Draw_Y + yc * infoP->Draw_Y_Offset;

			A_long target = infoP->SXCount * yc;

			double yoffset = (double)infoP->Graph_Tilt / (double)infoP->SXCount;

			li.x0 = xPos;
			li.x1 = xPos + infoP->SXCount * infoP->Graph_XScale;

			li.y0 = yPos;
			li.y1 = yPos + infoP->Graph_Tilt;
			Line8(&li);

		}
	}


	if ((infoP->mode == MODE_ALL) || (infoP->mode == MODE_WAVE)) {
		li.col = infoP->Graph_Color;
		for (A_long yc = 0; yc < infoP->SYCount; yc++)
		{
			A_long xPos = infoP->Draw_X + yc * infoP->Draw_X_Offset;
			A_long yPos = infoP->Draw_Y + yc * infoP->Draw_Y_Offset;

			A_long target = infoP->SXCount * yc;

			double yoffset = (double)infoP->Graph_Tilt / (double)infoP->SXCount;

			for (A_long xc = 1; xc < infoP->SXCount; xc++)
			{
				li.x0 = xPos + (xc - 1) * infoP->Graph_XScale;
				li.x1 = xPos + (xc)*infoP->Graph_XScale;

				double y0 = (double)yPos - ((double)infoP->Graph_YMax * infoP->SampleData[target + xc - 1]);
				y0 += (double)infoP->Graph_Tilt * ((double)xc - 1) / (double)infoP->SXCount;
				li.y0 = (A_long)(y0 + 0.5);
				double y1 = (double)yPos - ((double)infoP->Graph_YMax * infoP->SampleData[target + xc]);
				y1 += (double)infoP->Graph_Tilt * (double)xc / (double)infoP->SXCount;
				li.y1 = (A_long)(y1 + 0.5);

				Line8(&li);
			}

		}
	}
	return err;

}
//*****************************************************************************
static PF_Err sampling8(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;

	PF_Pixel* D = ae->output->data;
	A_long Wt = ae->out->widthTrue();
	A_long Wo = ae->out->offsetWidth();
	A_long W = ae->out->width();
	A_long H = ae->out->height();


	//縦方向の処理
	A_long sy = H / infoP->SYCount; //縦方向のサンプリング数
	if (H <= infoP->SYCount) {
		//何もしない
	}
	else {
		for (A_long yy = 0; yy < infoP->SYCount; yy++) {
			A_long target = yy*sy*Wt;
			A_long target2 = yy * Wt;
			for (A_long x = 0; x < W; x++)
			{
				A_long v = 0;
				A_long target3 = target;
				for (A_long y = 0; y < sy; y++)
				{
					v += (A_long)D[target3].red;
					target3 += Wt;
				}
				v = (A_long)((double)v / (double)sy + 0.5);
				if (v > PF_MAX_CHAN8) v = PF_MAX_CHAN8;
				D[target2].red = 
				D[target2].blue = 
				D[target2].green = (A_u_char)v;
				target++;
				target2++;
			}
		}
	}
	//横方向の処理
	A_long sx = W / infoP->SXCount; //横方向のサンプリング数
	if (W <= infoP->SXCount) {
		//何もしない
	}
	else {
		for (A_long yy = 0; yy < infoP->SYCount; yy++) {
			A_long target = yy * Wt;
			A_long target2 = target;
			A_long target3 = target;
			for (A_long x = 0; x < infoP->SXCount; x++)
			{
				A_long v = 0;
				for (A_long xx = 0; xx < sx; xx++)
				{
					v += (A_long)D[target3].green;
					target3++;
				}
				v = (A_long)((double)v / (double)sx + 0.5);
				if (v > PF_MAX_CHAN8) v = PF_MAX_CHAN8;
				D[target2].red =
				D[target2].blue =
				D[target2].green = (A_u_char)v;
				target2++;
			}
		}
	}

	for (A_long y = 0; y < infoP->SYCount; y++)
	{
		A_long target = y * Wt;
		A_long target2 = y * infoP->SXCount;
		for (A_long x = 0; x < infoP->SXCount; x++)
		{
			double v = (double)D[target].red / PF_MAX_CHAN8;
			target++;
			infoP->SampleData[target2] = v;
			target2++;


		}

	}


	return err;

}
//*****************************************************************************
static PF_Err gray8(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;

	PF_Pixel* iD = (PF_Pixel8*)ae->input->data;
	A_long iWt = ae->in->widthTrue();
	A_long iWo = ae->in->offsetWidth();
	A_long iW = ae->in->width();
	A_long iH = ae->in->height();

	PF_Pixel* oD = (PF_Pixel8*)ae->output->data;
	A_long oWt = ae->out->widthTrue();
	A_long oWo = ae->out->offsetWidth();
	A_long oW = ae->out->width();
	A_long oH = ae->out->height();

	A_long iTarget = 0;
	A_long oTarget = 0;

	for (A_long y = 0; y < iH; y++) {
		for (A_long x = 0; x < iW; x++) {
			double  a = (double)iD[iTarget].red * 0.29891 + (double)iD[iTarget].green * 0.58661 + (double)iD[iTarget].blue * 0.11448;
			if (iD[iTarget].alpha < PF_MAX_CHAN8) {
				a *= (double)iD[iTarget].alpha / PF_MAX_CHAN8;
			}
			A_long aa = (A_long)(a + 0.5);
			if (aa > PF_MAX_CHAN8) aa = PF_MAX_CHAN8;
			oD[oTarget].alpha = PF_MAX_CHAN8;
			oD[oTarget].red = oD[oTarget].green = oD[oTarget].blue = (A_u_char)aa;
			iTarget++;
			oTarget++;
		}
		iTarget += iWo;
		oTarget += oWo;
	}



	return err;

}
//*****************************************************************************
PF_Err exec8(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;

	infoP->SampleDataH = ae->NewHandle(sizeof(PF_FpLong) * infoP->SXCount * infoP->SYCount * 2);
	if (!infoP->SampleDataH) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	infoP->SampleData = *(PF_FpLong**)infoP->SampleDataH;


	if ((infoP->mode == MODE_ALL) || (infoP->mode == MODE_WAVE)) {
		ERR(gray8(ae, infoP));
		ERR(sampling8(ae, infoP));
	}
	ERR(ae->out->clear());
	ERR(drawGraph8(ae, infoP));

	if (infoP->SampleDataH != NULL) {
		ae->DisposeHandle(infoP->SampleDataH);
		infoP->SampleDataH = NULL;
	}
	return err;
}
