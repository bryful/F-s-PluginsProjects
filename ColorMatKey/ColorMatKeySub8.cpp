#include "ColorMatKey.h"



//-------------------------------------------------------------------------------------------------
// 加算合成
inline void AddPxL(GInfo *gi, PF_Pixel p)
{
	PF_Pixel *data = (PF_Pixel *)gi->data;

	A_long pos = gi->xL + gi->yL*gi->widthTrue;
	data[pos].red = RoundByteLong((A_long)data[pos].red + (A_long)p.red);
	data[pos].green = RoundByteLong((A_long)data[pos].green + (A_long)p.green);
	data[pos].blue = RoundByteLong((A_long)data[pos].blue + (A_long)p.blue);
	data[pos].alpha = RoundByteLong((A_long)data[pos].alpha + (A_long)p.alpha);

}
//-------------------------------------------------------------------------------------------------
// ピクセルの濃度調整
inline PF_Pixel PxPer(PF_Pixel p, PF_FpLong per)
{
	PF_Pixel r;
	r.red = RoundByteFpLong( p.red * per);
	r.green = RoundByteFpLong(p.green * per);
	r.blue = RoundByteFpLong(p.blue * per);
	r.alpha = RoundByteFpLong(p.alpha * per);
	return r;
}
//-------------------------------------------------------------------------------------------------
//小数点対応の加算合成
inline void AddPxD(GInfo *gi, PF_Pixel p)
{

	A_long x0, x1, y0, y1;
	x0 = (A_long)gi->xD; x1 = x0 + 1;
	y0 = (A_long)gi->yD; y1 = y0 + 1;

	PF_FpLong ax1 = gi->xD - (PF_FpLong)x0;
	PF_FpLong ay1 = gi->yD - (PF_FpLong)y0;
	PF_FpLong ax0 = 1.0 - ax1;
	PF_FpLong ay0 = 1.0 - ay1;

	gi->xL = x0;
	gi->yL = y0;
	AddPxL(gi, PxPer(p, ax0 * ay0));
	gi->xL = x0;
	gi->yL = y1;
	AddPxL(gi, PxPer(p, ax0* ay1));
	gi->xL = x1;
	gi->yL = y0;
	AddPxL(gi, PxPer(p, ax1* ay0));
	gi->xL = x1;
	gi->yL = y1;
	AddPxL(gi, PxPer(p, ax1* ay1));

}
//-------------------------------------------------------------------------------------------------
static PF_Err
MakePixelTable(PF_Pixel *tbl, A_long len, PF_Pixel col,PF_FpLong per, PF_FpLong br)
{

	PF_FpLong p = per;
	for (A_long i = 0; i < len; i++)
	{
		p = per * (1 - (PF_FpLong)i/ (PF_FpLong)len) ;
		tbl[i] = PxPer(col, p);
		A_long a = (A_long)((PF_FpLong)tbl[i].alpha*br);
		tbl[i].red = RoundByteLong(tbl[i].red + a);
		tbl[i].green = RoundByteLong(tbl[i].green + a);
		tbl[i].blue = RoundByteLong(tbl[i].blue + a);

	}
}

//-------------------------------------------------------------------------------------------------
static PF_Err
TargetPixel8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	infoP = reinterpret_cast<ParamInfo8*>(refcon);


	PF_Pixel8 c = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
	A_long v = 0, r, g, b;
	r = (A_long)inP->red * (A_long)inP->alpha / PF_MAX_CHAN8;
	g = (A_long)inP->green * (A_long)inP->alpha / PF_MAX_CHAN8;
	b = (A_long)inP->blue * (A_long)inP->alpha / PF_MAX_CHAN8;


	PF_InData		*in_data = infoP->in_data;
	PF_PixelFloat pf;
	double v2;

	switch (infoP->info.target_kind)
	{
	case 2:
		v = (r + g + b) / 3;
		break;
	case 3:
		pf.red = (PF_FpShort)r / PF_MAX_CHAN8;
		pf.green = (PF_FpShort)g / PF_MAX_CHAN8;
		pf.blue = (PF_FpShort)b / PF_MAX_CHAN8;
		pf.alpha = 1.0;

		LABA lab = RgbToLab(pf);
		//色の距離測定
		v2 = 1.0 - (PF_SQRT(
			PF_POW(lab.L - infoP->taget_lab.L, 2)
			+ PF_POW(lab.A - infoP->taget_lab.A, 2)
			+ PF_POW(lab.B - infoP->taget_lab.B, 2))
			/ PF_SQRT(3)
			);
		v = (A_long)(v2 * PF_MAX_CHAN8 + 0.5);
		
		//HLSA hls = RGBtoHLS(pf);
		//色の距離測定
		/*
		v2 = 1.0 - (PF_SQRT(
			PF_POW(hls.H - infoP->target_hls.H, 2)
			+ PF_POW(hls.L - infoP->target_hls.L, 2)
			+ PF_POW(hls.S - infoP->target_hls.S, 2))
			/ PF_SQRT(3)
			);
		v = (A_long)(v2 * PF_MAX_CHAN8 + 0.5);
		*/
		break;
	case 1:
	default:
		v = MAX(MAX(r, g), b);
		break;
	}
	//------------
	if (infoP->target_softness == 0) {
		if (v >= infoP->target_border) {
			v = PF_MAX_CHAN8;
		}
		else {
			v = 0;
		}
	}
	else {
		if (v <= infoP->target_start) {
			v = 0;
		}
		else if (v >= infoP->target_border) {
			v = PF_MAX_CHAN8;
		}
		else {
			v = (v - infoP->target_start)*PF_MAX_CHAN8 / (infoP->target_softness);
		}

	}


	c.alpha = RoundByteLong(v);

	c.red = RoundByteLong(inP->red * v / PF_MAX_CHAN8);
	c.green = RoundByteLong(inP->green * v / PF_MAX_CHAN8);
	c.blue = RoundByteLong(inP->blue * v / PF_MAX_CHAN8);

	*outP = c;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
AlphaON(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	infoP = reinterpret_cast<ParamInfo8*>(refcon);


	outP->alpha = PF_MAX_CHAN8;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
RevCopyAtoG(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	infoP = reinterpret_cast<ParamInfo8*>(refcon);

	PF_Pixel c = { 0,0,0,0 };
	c.green = PF_MAX_CHAN8 - outP->alpha;
	c.alpha = outP->alpha;

	*outP = c;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxHor8(CFsAE *ae, ParamInfo8 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel *data = pi->data;
	A_long mx1 = 0;
	A_long mx2 = 0;

	A_long org;
	A_long hor;
	for (A_long y = 0; y < pi->height; y++)
	{
		hor = y * pi->widthTrue;
		for (A_long x = 0; x < pi->width; x++) {
			pi->scanline[x] = data[x + hor].green;
		}
		for (A_long x = 0; x < pi->width; x++) {
			org = pi->scanline[x];
			mx1 = mx2 = 0;
			if (x > 0) {
				mx1 = pi->scanline[x-1];
			}
			if (x < pi->width-1) {
				mx2 = pi->scanline[x + 1];
			}
			if (mx1 < mx2) {
				mx1 = mx2;
			}
			if (org < mx1) {
				org += mx1/2;
				if (org > mx1) org = mx1;
				data[x + hor].green = RoundByteLong(org);
			}

		}

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
MaxVer8(CFsAE *ae, ParamInfo8 *pi)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel *data = pi->data;
	A_long mx1 = 0;
	A_long mx2 = 0;

	A_long org;
	A_long hor;
	for (A_long x = 0; x < pi->width; x++)
	{
		hor = 0;
		for (A_long y = 0; y < pi->height; y++) {
			pi->scanline[y] = data[x + hor].green;
			hor += pi->widthTrue;
		}
		hor = 0;
		for (A_long y = 0; y < pi->height; y++) {
			org = pi->scanline[y];
			mx1 = mx2 = 0;
			if (y > 0) {
				mx1 = pi->scanline[y - 1];
			}
			if (y < pi->height - 1) {
				mx2 = pi->scanline[y + 1];
			}
			if (mx1 < mx2) {
				mx1 = mx2;
			}
			if (org < mx1) {
				org += mx1 / 2;
				if (org > mx1) org = mx1;
				data[x + hor].green = RoundByteLong(org);
			}
			hor += pi->widthTrue;

		}

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
RevSubGtoA(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	infoP = reinterpret_cast<ParamInfo8*>(refcon);

	PF_Pixel c = { PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8 };

	A_long g = outP->green;
	A_long a = outP->alpha;

	c.alpha = RoundByteLong(a - (PF_MAX_CHAN8 - g));


	*outP = c;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err ToParam8(CFsAE *ae, ParamInfo *infoP, ParamInfo8 *pi)
{
	PF_Err	err = PF_Err_NONE;


	pi->width = ae->out->width();
	pi->widthTrue = ae->out->widthTrue();
	pi->height = ae->out->height();
	pi->data = (PF_Pixel8 *)ae->out->data();
	pi->in_data = ae->in_data;

	pi->bufSize = pi->width;
	if (pi->bufSize < pi->height) pi->bufSize = pi->height;
	
	pi->info = *infoP;
	pi->info.target_color = infoP->target_color;

	PF_PixelFloat pf = CONV8TO32(pi->info.target_color);
	pi->taget_lab = RgbToLab(pf);

	pi->target_border = (A_long)(infoP->target_border*PF_MAX_CHAN8);
	pi->target_softness = (A_long)(infoP->target_softness*PF_MAX_CHAN8);
	pi->target_start = pi->target_border - pi->target_softness;
	if (pi->target_start < 0) {
		pi->target_start = 0;
		pi->target_softness = pi->target_border - pi->target_start;
	}

	pi->si.lenHor = (A_long)((double)pi->info.horLength * pi->info.length + 0.5);
	pi->si.lenVer = (A_long)((double)pi->info.verLength * pi->info.length + 0.5);
	pi->si.lenDia = (A_long)((double)pi->info.diaLength * pi->info.length + 0.5);
	pi->si.horOpa = pi->info.horOpacity * pi->info.opacity;
	pi->si.verOpa = pi->info.verOpacity * pi->info.opacity;
	pi->si.diaOpa = pi->info.diaOpacity * pi->info.opacity;

	pi->si.lenMax = pi->si.lenHor;
	if (pi->si.lenMax > pi->si.lenVer) pi->si.lenMax = pi->si.lenVer;
	if (pi->si.lenMax > pi->si.lenDia) pi->si.lenMax = pi->si.lenDia;

	pi->si.rot = pi->info.rot;
	if (pi->info.autoRolling == TRUE)
	{
		double time = ((double)ae->in_data->current_time / (double)ae->in_data->time_scale);
		pi->si.rot = (PF_Fixed)(pi->info.rollingSpeed*time * 65536.0);


	}
	pi->si.rot %= (360 * 65536);
	if (pi->si.rot < 0) {
		pi->si.rot += (360 * 65536);
	}


	CRotCalc rot(ae->in_data);
	rot.SetRotLength(pi->si.rot, pi->si.lenHor << 16);
	pi->si.horAddX = rot.xFLT() / pi->si.lenHor;
	pi->si.horAddY = rot.yFLT() / pi->si.lenHor;

	rot.SetRotLength(pi->si.rot- 90*65536, pi->si.lenVer << 16);
	pi->si.verAddX = rot.xFLT() / pi->si.lenVer;
	pi->si.verAddY = rot.yFLT() / pi->si.lenVer;

	rot.SetRotLength(pi->si.rot - 45 * 65536, pi->si.lenDia << 16);
	pi->si.diaAddX = rot.xFLT() / pi->si.lenDia;
	pi->si.diaAddY = rot.yFLT() / pi->si.lenDia;

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err StarExec8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if (ae->out->Enabled() == FALSE)
	{
		return PF_Err_INVALID_CALLBACK;
	}
	ParamInfo8 pi;

	ToParam8(ae, infoP, &pi);
	/*
	pi.bufH = ae->NewHandle(pi.bufSize * sizeof(A_u_char)  + (pi.si.lenHor+ pi.si.lenVer + pi.si.lenDia)* sizeof(PF_Pixel)+ 1024);
	if (pi.bufH == NULL) {
		err = PF_Err_OUT_OF_MEMORY;
		return err;
	}
	pi.scanline = *(A_u_char**)pi.bufH;
	pi.horTable = *(PF_Pixel**)(pi.scanline + pi.bufSize);
	pi.verTable = (pi.horTable + pi.si.lenHor);
	pi.diaTable = (pi.verTable + pi.si.lenVer);
	*/
	//ターゲットエリア
	ERR(ae->iterate8((refconType)&pi, TargetPixel8));
	if (pi.info.target_maskDraw) {
		ae->out->fromBlackMat8();
		return err;
	}
	
	/**
	//Gへ反転してコピー
	ERR(ae->iterate8((refconType)&pi, RevCopyAtoG));
	//拡張
	ERR(MaxHor8(ae, &pi));
	ERR(MaxVer8(ae, &pi));
	ERR(MaxHor8(ae, &pi));
	ERR(MaxVer8(ae, &pi));
	ERR(MaxHor8(ae, &pi));
	ERR(MaxVer8(ae, &pi));
	ERR(MaxHor8(ae, &pi));
	ERR(MaxVer8(ae, &pi));
	ERR(ae->iterate8((refconType)&pi, RevSubGtoA));
	*/


	//デバッグ用
	//ERR(ae->iterate8((refconType)&pi, AlphaON));

	/*if (pi.bufH != NULL) {
		ae->DisposeHandle(pi.bufH);
		pi.bufH = NULL;
	}*/

	return err;

}