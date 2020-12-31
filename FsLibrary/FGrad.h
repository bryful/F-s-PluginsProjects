#pragma once
#ifndef FGrad_H
#define FGrad_H

#include "Fs.h"
#include "FsUtils.h"


#ifndef LINEINFO_H
#define LINEINFO_H
typedef struct LineInfo {
	A_FloatPoint	s;
	A_FloatPoint	e;

} LineInfo, * LineInfoP, ** LineInfoH;
#endif

#ifndef GRADINFO_H
#define GRADINFO_H
typedef struct GradInfo {
	A_FloatPoint start;
	A_FloatPoint last;
	PF_FpLong	length;
	PF_FpLong	dx;
	PF_FpLong	dy;
	PF_FpLong	rad;
	PF_FpLong	ax;
	PF_FpLong	ay;
	LineInfo	sV;
	LineInfo	eV;
	PF_PixelPtr	startColP;
	PF_PixelPtr	lastColP;

} GradInfo, * GradInfoP, ** GradInfoH;
#endif

class FGrad
{
private:
	//アドレス計算テーブル・ポイントテーブル用
	PF_Handle	m_bufH = NULL;

	// アドレス計算テーブル
	A_long* m_vurTbl = NULL;


	A_long		m_width = 0;
	A_long		m_height = 0;
	A_long		m_widthTrue = 0;
	A_long		m_offsetWidth = 0;
	PF_PixelPtr m_data = NULL;
	PF_PixelFormat	m_format = PF_PixelFormat_ARGB32;

public:
	PF_InData* in_data = NULL;
	PF_EffectWorld* world = NULL;

	A_long	width() { return m_width; }
	A_long	height() { return m_height; }
	A_long	widthTrue() { return  m_widthTrue; }
	A_long	offsetWidth() { m_offsetWidth; }
	PF_Pixel* data8() { return (PF_Pixel*)m_data; }
	PF_Pixel16* data16() { return (PF_Pixel16*)m_data; }
	PF_PixelFloat* data32() { return (PF_PixelFloat*)m_data; }
	PF_PixelFormat pixelFormat() { return m_format; }

	// ***************************************************************
	FGrad(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	)
	{
		in_data = ind;
		world = wld;
		m_format = fmt;


		if (wld != NULL) {
			m_data = wld->data;
			m_width = wld->width;
			m_height = wld->height;

			switch (m_format)
			{
			case PF_PixelFormat_ARGB128:
				m_widthTrue = wld->rowbytes / sizeof(PF_Pixel32);
				break;
			case PF_PixelFormat_ARGB64:
				m_widthTrue = wld->rowbytes / sizeof(PF_Pixel16);
				break;
			default:
			case PF_PixelFormat_ARGB32:
				m_widthTrue = wld->rowbytes / sizeof(PF_Pixel8);
				break;
			}
			m_offsetWidth = m_widthTrue - m_width;


			A_long bufLen = m_height * sizeof(A_long);
			bufLen *= 2;
			m_bufH = PF_NEW_HANDLE(bufLen);
			if (m_bufH) {
				m_vurTbl = *(A_long**)m_bufH;
				PF_LOCK_HANDLE(m_bufH);
				for (A_long i = 0; i < m_height; i++)
				{
					m_vurTbl[i] = i * m_widthTrue;
				}

			}
		}
	}
	// ***************************************************************
	~FGrad()
	{
		if (m_bufH != NULL)
		{
			PF_UNLOCK_HANDLE(m_bufH);
			PF_DISPOSE_HANDLE(m_bufH);
			m_bufH = NULL;
		}
	}
private:
	// ***************************************************************
	void execX8(GradInfo gi)
	{
		PF_Pixel* data = data8();

		LineInfo xl;
		xl.s.x = -8000;
		xl.e.x = 8000;
		xl.s.y = 0;
		xl.e.y = 0;
		A_FloatPoint s0;

		PF_Pixel* sp = (PF_Pixel*)gi.startColP;
		PF_Pixel* lp = (PF_Pixel*)gi.lastColP;
		PF_Pixel* sp1 = (PF_Pixel*)gi.startColP;
		PF_Pixel* lp1 = (PF_Pixel*)gi.lastColP;
		for (A_long y = 0; y < m_height; y++)
		{
			xl.s.y = y;
			xl.e.y = y;
			CrossPoint(xl, gi.sV, &s0);
			A_long x0 = (A_long)(s0.x +0.5);
			CrossPoint(xl, gi.eV, &s0);
			A_long x1 = (A_long)(s0.x + 0.5);

			if (x0 > x1) {
				A_long t = x0;
				x0 = x1;
				x1 = t;
				sp1 = (PF_Pixel*)gi.lastColP;
				lp1 = (PF_Pixel*)gi.startColP;
			}

			for (A_long x = 0; x < m_width; x++)
			{
				A_u_char a = data[x + m_vurTbl[y]].alpha;
				if ( a == 0) continue;
				PF_Pixel col;
				if (x < x0) {
					col = *sp1;
				}
				else if (x > x1) {
					col = *lp1;
				}
				else {
					LineInfo gl;
					gl.s.x = x - gi.ax;
					gl.s.y = y - gi.ay;
					gl.e.x = x + gi.ax;
					gl.e.y = y + gi.ay;
					A_FloatPoint p0,p1,p2;
					p0.x = x;
					p0.y = y;
					CrossPoint(gl, gi.sV, &p1);
					CrossPoint(gl, gi.eV, &p2);
					PF_FpLong v1 = PF_HYPOT((p1.x - p0.x), (p1.y - p0.y)) / gi.length;
					PF_FpLong v2 = PF_HYPOT((p2.x - p0.x), (p2.y - p0.y)) / gi.length;
					col.red = RoundByteFpLong(((PF_FpLong)sp->red * v2) + ((PF_FpLong)lp->red * v1));
					col.green = RoundByteFpLong(((PF_FpLong)sp->green * v2) + ((PF_FpLong)lp->green * v1));
					col.blue = RoundByteFpLong(((PF_FpLong)sp->blue * v2) + ((PF_FpLong)lp->blue * v1));
				}
				col.alpha = a;
				data[x + m_vurTbl[y]] = col;

			}
		}
	}
	// ***************************************************************
	void execY8(GradInfo gi)
	{
		PF_Pixel* data = data8();

		LineInfo yl;
		yl.s.x = 0;
		yl.e.x = 0;
		yl.s.y = -8000;
		yl.e.y = 8000;
		A_FloatPoint s0;

		PF_Pixel* sp = (PF_Pixel*)gi.startColP;
		PF_Pixel* lp = (PF_Pixel*)gi.lastColP;
		PF_Pixel* sp1 = (PF_Pixel*)gi.startColP;
		PF_Pixel* lp1 = (PF_Pixel*)gi.lastColP;
		for (A_long x = 0; x < m_width; x++)
		{
			yl.s.x = x;
			yl.e.x = x;
			CrossPoint(yl, gi.sV, &s0);
			A_long y0 = (A_long)(s0.y + 0.5);
			CrossPoint(yl, gi.eV, &s0);
			A_long y1 = (A_long)(s0.y + 0.5);

			if (y0 > y1) {
				A_long t = y0;
				y0 = y1;
				y1 = t;
				sp1 = (PF_Pixel*)gi.lastColP;
				lp1 = (PF_Pixel*)gi.startColP;
			}

			for (A_long y = 0; y < m_height; y++)
			{
				A_u_char a = data[x + m_vurTbl[y]].alpha;
				if (a == 0) continue;
				PF_Pixel col;
				if (y < y0) {
					col = *sp1;
				}
				else if (y > y1) {
					col = *lp1;
				}
				else {
					LineInfo gl;
					gl.s.x = x - gi.ax;
					gl.s.y = y - gi.ay;
					gl.e.x = x + gi.ax;
					gl.e.y = y + gi.ay;
					A_FloatPoint p0, p1, p2;
					p0.x = x;
					p0.y = y;
					CrossPoint(gl, gi.sV, &p1);
					CrossPoint(gl, gi.eV, &p2);
					PF_FpLong v1 = PF_HYPOT((p1.x - p0.x), (p1.y - p0.y)) / gi.length;
					PF_FpLong v2 = PF_HYPOT((p2.x - p0.x), (p2.y - p0.y)) / gi.length;
					col.red = RoundByteFpLong(((PF_FpLong)sp->red * v2) + ((PF_FpLong)lp->red * v1));
					col.green = RoundByteFpLong(((PF_FpLong)sp->green * v2) + ((PF_FpLong)lp->green * v1));
					col.blue = RoundByteFpLong(((PF_FpLong)sp->blue * v2) + ((PF_FpLong)lp->blue * v1));
				}
				col.alpha = a;
				data[x + m_vurTbl[y]] = col;

			}
		}
	}// ***************************************************************
	// ***************************************************************
	void execX16(GradInfo gi)
	{
		PF_Pixel16* data = data16();

		LineInfo xl;
		xl.s.x = -8000;
		xl.e.x = 8000;
		xl.s.y = 0;
		xl.e.y = 0;
		A_FloatPoint s0;

		PF_Pixel16* sp = (PF_Pixel16*)gi.startColP;
		PF_Pixel16* lp = (PF_Pixel16*)gi.lastColP;
		PF_Pixel16* sp1 = (PF_Pixel16*)gi.startColP;
		PF_Pixel16* lp1 = (PF_Pixel16*)gi.lastColP;
		for (A_long y = 0; y < m_height; y++)
		{
			xl.s.y = y;
			xl.e.y = y;
			CrossPoint(xl, gi.sV, &s0);
			A_long x0 = (A_long)(s0.x + 0.5);
			CrossPoint(xl, gi.eV, &s0);
			A_long x1 = (A_long)(s0.x + 0.5);

			if (x0 > x1) {
				A_long t = x0;
				x0 = x1;
				x1 = t;
				sp1 = (PF_Pixel16*)gi.lastColP;
				lp1 = (PF_Pixel16*)gi.startColP;
			}

			for (A_long x = 0; x < m_width; x++)
			{
				A_u_short a = data[x + m_vurTbl[y]].alpha;
				if (a == 0) continue;
				PF_Pixel16 col;

				if (x < x0) {
					col = *sp1;
				}
				else if (x > x1) {
					col = *lp1;
				}
				else {
					LineInfo gl;
					gl.s.x = x - gi.ax;
					gl.s.y = y - gi.ay;
					gl.e.x = x + gi.ax;
					gl.e.y = y + gi.ay;
					A_FloatPoint p0, p1, p2;
					p0.x = x;
					p0.y = y;
					CrossPoint(gl, gi.sV, &p1);
					CrossPoint(gl, gi.eV, &p2);
					PF_FpLong v1 = PF_HYPOT((p1.x - p0.x), (p1.y - p0.y)) / gi.length;
					PF_FpLong v2 = PF_HYPOT((p2.x - p0.x), (p2.y - p0.y)) / gi.length;
					col.red = RoundShortFpLong(((PF_FpLong)sp->red * v2) + ((PF_FpLong)lp->red * v1));
					col.green = RoundShortFpLong(((PF_FpLong)sp->green * v2) + ((PF_FpLong)lp->green * v1));
					col.blue = RoundShortFpLong(((PF_FpLong)sp->blue * v2) + ((PF_FpLong)lp->blue * v1));
				}
				col.alpha = a;
				data[x + m_vurTbl[y]] = col;

			}
		}
	}
	// ***************************************************************
	void execY16(GradInfo gi)
	{
		PF_Pixel16* data = data16();

		LineInfo yl;
		yl.s.x = 0;
		yl.e.x = 0;
		yl.s.y = -8000;
		yl.e.y = 8000;
		A_FloatPoint s0;

		PF_Pixel16* sp = (PF_Pixel16*)gi.startColP;
		PF_Pixel16* lp = (PF_Pixel16*)gi.lastColP;
		PF_Pixel16* sp1 = (PF_Pixel16*)gi.startColP;
		PF_Pixel16* lp1 = (PF_Pixel16*)gi.lastColP;
		for (A_long x = 0; x < m_width; x++)
		{
			yl.s.x = x;
			yl.e.x = x;
			CrossPoint(yl, gi.sV, &s0);
			A_long y0 = (A_long)(s0.y + 0.5);
			CrossPoint(yl, gi.eV, &s0);
			A_long y1 = (A_long)(s0.y + 0.5);

			if (y0 > y1) {
				A_long t = y0;
				y0 = y1;
				y1 = t;
				sp1 = (PF_Pixel16*)gi.lastColP;
				lp1 = (PF_Pixel16*)gi.startColP;
			}

			for (A_long y = 0; y < m_height; y++)
			{
				A_u_short a = data[x + m_vurTbl[y]].alpha;
				if (a == 0) continue;
				PF_Pixel16 col;

				if (y < y0) {
					col = *sp1;
				}
				else if (y > y1) {
					col = *lp1;
				}
				else {
					LineInfo gl;
					gl.s.x = x - gi.ax;
					gl.s.y = y - gi.ay;
					gl.e.x = x + gi.ax;
					gl.e.y = y + gi.ay;
					A_FloatPoint p0, p1, p2;
					p0.x = x;
					p0.y = y;
					CrossPoint(gl, gi.sV, &p1);
					CrossPoint(gl, gi.eV, &p2);
					PF_FpLong v1 = PF_HYPOT((p1.x - p0.x), (p1.y - p0.y)) / gi.length;
					PF_FpLong v2 = PF_HYPOT((p2.x - p0.x), (p2.y - p0.y)) / gi.length;
					col.red = RoundShortFpLong(((PF_FpLong)sp->red * v2) + ((PF_FpLong)lp->red * v1));
					col.green = RoundShortFpLong(((PF_FpLong)sp->green * v2) + ((PF_FpLong)lp->green * v1));
					col.blue = RoundShortFpLong(((PF_FpLong)sp->blue * v2) + ((PF_FpLong)lp->blue * v1));
				}
				col.alpha = a;
				data[x + m_vurTbl[y]] = col;

			}
		}
	}// ***************************************************************
	// ***************************************************************
	void execX32(GradInfo gi)
	{
		PF_PixelFloat* data = data32();

		LineInfo xl;
		xl.s.x = -8000;
		xl.e.x = 8000;
		xl.s.y = 0;
		xl.e.y = 0;
		A_FloatPoint s0;

		PF_PixelFloat* sp = (PF_PixelFloat*)gi.startColP;
		PF_PixelFloat* lp = (PF_PixelFloat*)gi.lastColP;
		PF_PixelFloat* sp1 = (PF_PixelFloat*)gi.startColP;
		PF_PixelFloat* lp1 = (PF_PixelFloat*)gi.lastColP;
		for (A_long y = 0; y < m_height; y++)
		{
			xl.s.y = y;
			xl.e.y = y;
			CrossPoint(xl, gi.sV, &s0);
			A_long x0 = (A_long)(s0.x + 0.5);
			CrossPoint(xl, gi.eV, &s0);
			A_long x1 = (A_long)(s0.x + 0.5);

			if (x0 > x1) {
				A_long t = x0;
				x0 = x1;
				x1 = t;
				sp1 = (PF_PixelFloat*)gi.lastColP;
				lp1 = (PF_PixelFloat*)gi.startColP;
			}

			for (A_long x = 0; x < m_width; x++)
			{
				PF_FpShort a = data[x + m_vurTbl[y]].alpha;
				if (a == 0) continue;
				PF_PixelFloat col;

				if (x < x0) {
					col = *sp1;
				}
				else if (x > x1) {
					col = *lp1;
				}
				else {
					LineInfo gl;
					gl.s.x = x - gi.ax;
					gl.s.y = y - gi.ay;
					gl.e.x = x + gi.ax;
					gl.e.y = y + gi.ay;
					A_FloatPoint p0, p1, p2;
					p0.x = x;
					p0.y = y;
					CrossPoint(gl, gi.sV, &p1);
					CrossPoint(gl, gi.eV, &p2);
					PF_FpLong v1 = PF_HYPOT((p1.x - p0.x), (p1.y - p0.y)) / gi.length;
					PF_FpLong v2 = PF_HYPOT((p2.x - p0.x), (p2.y - p0.y)) / gi.length;
					col.red = RoundFpShortDouble(((PF_FpLong)sp->red * v2) + ((PF_FpLong)lp->red * v1));
					col.green = RoundFpShortDouble(((PF_FpLong)sp->green * v2) + ((PF_FpLong)lp->green * v1));
					col.blue = RoundFpShortDouble(((PF_FpLong)sp->blue * v2) + ((PF_FpLong)lp->blue * v1));
				}
				col.alpha = a;
				data[x + m_vurTbl[y]] = col;

			}
		}
	}
	// ***************************************************************
	void execY32(GradInfo gi)
	{
		PF_PixelFloat* data = data32();

		LineInfo yl;
		yl.s.x = 0;
		yl.e.x = 0;
		yl.s.y = -8000;
		yl.e.y = 8000;
		A_FloatPoint s0;

		PF_PixelFloat* sp = (PF_PixelFloat*)gi.startColP;
		PF_PixelFloat* lp = (PF_PixelFloat*)gi.lastColP;
		PF_PixelFloat* sp1 = (PF_PixelFloat*)gi.startColP;
		PF_PixelFloat* lp1 = (PF_PixelFloat*)gi.lastColP;
		for (A_long x = 0; x < m_width; x++)
		{
			yl.s.x = x;
			yl.e.x = x;
			CrossPoint(yl, gi.sV, &s0);
			A_long y0 = (A_long)(s0.y + 0.5);
			CrossPoint(yl, gi.eV, &s0);
			A_long y1 = (A_long)(s0.y + 0.5);

			if (y0 > y1) {
				A_long t = y0;
				y0 = y1;
				y1 = t;
				sp1 = (PF_PixelFloat*)gi.lastColP;
				lp1 = (PF_PixelFloat*)gi.startColP;
			}

			for (A_long y = 0; y < m_height; y++)
			{
				PF_FpShort a = data[x + m_vurTbl[y]].alpha;
				if (a == 0) continue;
				PF_PixelFloat col;

				if (y < y0) {
					col = *sp1;
				}
				else if (y > y1) {
					col = *lp1;
				}
				else {
					LineInfo gl;
					gl.s.x = x - gi.ax;
					gl.s.y = y - gi.ay;
					gl.e.x = x + gi.ax;
					gl.e.y = y + gi.ay;
					A_FloatPoint p0, p1, p2;
					p0.x = x;
					p0.y = y;
					CrossPoint(gl, gi.sV, &p1);
					CrossPoint(gl, gi.eV, &p2);
					PF_FpLong v1 = PF_HYPOT((p1.x - p0.x), (p1.y - p0.y)) / gi.length;
					PF_FpLong v2 = PF_HYPOT((p2.x - p0.x), (p2.y - p0.y)) / gi.length;
					col.red = RoundFpShortDouble(((PF_FpLong)sp->red * v2) + ((PF_FpLong)lp->red * v1));
					col.green = RoundFpShortDouble(((PF_FpLong)sp->green * v2) + ((PF_FpLong)lp->green * v1));
					col.blue = RoundFpShortDouble(((PF_FpLong)sp->blue * v2) + ((PF_FpLong)lp->blue * v1));
				}
				col.alpha = a;
				data[x + m_vurTbl[y]] = col;

			}
		}
	}// ***************************************************************
public:
	void exec(A_FloatPoint start, A_FloatPoint last,PF_Pixel stc, PF_Pixel ltc )
	{
		GradInfo gi;
		gi.start = start;
		gi.last = last;
		gi.dx = gi.last.x - gi.start.x;
		gi.dy = gi.last.y - gi.start.y;
		gi.rad = PF_ATAN2(gi.dy, gi.dx);
		gi.ax = gi.dx*10;
		gi.ay = gi.dy*10;

		gi.length = PF_HYPOT(gi.dx, gi.dy);

		PF_FpLong rd2 = gi.rad + 90 * PF_PI / 180;

		PF_FpLong px0 = 8000 * PF_COS(rd2);
		PF_FpLong py0 = 8000 * PF_SIN(rd2);

		gi.sV.s.x = gi.start.x - px0;
		gi.sV.s.y = gi.start.y - py0;
		gi.sV.e.x = gi.start.x + px0;
		gi.sV.e.y = gi.start.y + py0;

		gi.eV.s.x = gi.last.x - px0;
		gi.eV.s.y = gi.last.y - py0;
		gi.eV.e.x = gi.last.x + px0;
		gi.eV.e.y = gi.last.y + py0;


		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			PF_PixelFloat sc32 = CONV8TO32(stc);
			PF_PixelFloat lc32 = CONV8TO32(ltc);
			gi.startColP = (PF_PixelPtr)&sc32;
			gi.lastColP = (PF_PixelPtr)&lc32;
			if (F_ABS(gi.dx) >= F_ABS(gi.dy))
			{
				execX32(gi);
			}
			else {
				execY32(gi);
			}
			break;
		case PF_PixelFormat_ARGB64:
			PF_Pixel16 sc16 = CONV8TO16(stc);
			PF_Pixel16 lc16 = CONV8TO16(ltc);
			gi.startColP = (PF_PixelPtr)&sc16;
			gi.lastColP = (PF_PixelPtr)&lc16;
			if (F_ABS(gi.dx) >= F_ABS(gi.dy))
			{
				execX16(gi);
			}
			else {
				execY16(gi);
			}
			break;
		default:
		case PF_PixelFormat_ARGB32:
			PF_Pixel sc = stc;
			PF_Pixel lc = ltc;
			gi.startColP = (PF_PixelPtr)&sc;
			gi.lastColP = (PF_PixelPtr)&lc;
			if (F_ABS(gi.dx) >= F_ABS(gi.dy))
			{
				execX8(gi);
			}
			else {
				execY8(gi);
			}
			break;
		}


	}
	// ***************************************************************
	static PF_Boolean CrossPoint(LineInfo line0, LineInfo line1, A_FloatPoint *p)
	{
		PF_Boolean ret = FALSE;
		p->x = 0;
		p->y = 0;


		PF_FpLong dx0 = line0.e.x - line0.s.x;
		PF_FpLong dy0 = line0.e.y - line0.s.y;
		PF_FpLong dx1 = line1.e.x - line1.s.x;
		PF_FpLong dy1 = line1.e.y - line1.s.y;

		PF_FpLong dBunbo = dx0 * dy1 - dy0  * dx1;

		if (dBunbo == 0) return ret;
		PF_FpLong ax = line1.s.x - line0.s.x;
		PF_FpLong ay = line1.s.y - line0.s.y;

		PF_FpLong dr = (dy1 * ax - dx1 * ay) / dBunbo;
		PF_FpLong ds = (dy0 * ax - dx0 * ay) / dBunbo;
		
		p->x = line0.s.x + dr * dx0;
		p->y = line0.s.y + dr * dy0;
		ret = TRUE;

		return ret;
	}

};
#endif