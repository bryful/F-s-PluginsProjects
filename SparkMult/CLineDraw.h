#pragma once
#ifndef CLineDraw_H
#define CLineDraw_H

#include "CPointInfo.h"

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"


#ifndef PointInfo_H
#define PointInfo_H
typedef struct PointInfo {
	A_LPoint	p;
	PF_FpLong	s;

} PointInfo, * PointInfoP, ** PointInfoH;
#endif

#ifndef POINT_TABLE_SIZE_H
#define POINT_TABLE_SIZE_H
#define POINT_TABLE_SIZE 1024
#endif


class CLineDraw
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

	//関数ポインタ用の宣言
	typedef void (CLineDraw::* XLINE_FUNC)(PF_FpLong x0, PF_FpLong x1, A_long y);
	XLINE_FUNC m_XLINE_FUNC = NULL;
	//A_long x, PF_FpLong y0, PF_FpLong y1
	typedef void (CLineDraw::* YLINE_FUNC)(A_long x, PF_FpLong y0, PF_FpLong y1);
	YLINE_FUNC m_YLINE_FUNC = NULL;

public:
	PF_InData* in_data = NULL;
	PF_EffectWorld* world = NULL;
	CPointInfo Points;

	A_long	width() { return m_width; }
	A_long	height() { return m_height; }
	A_long	widthTrue() { return  m_widthTrue; }
	A_long	offsetWidth() { m_offsetWidth; }
	PF_Pixel* data8() { return (PF_Pixel*)m_data; }
	PF_Pixel16* data16() { return (PF_Pixel16*)m_data; }
	PF_PixelFloat* data32() { return (PF_PixelFloat*)m_data; }
	PF_PixelFormat pixelFormat() { return m_format; }

private:
#pragma region BlendPX


	// ***************************************************************
	inline  void BlendPX8(A_long x, A_long y, A_u_char v)
	{
		if ((x >= 0) && (x < m_width) && (y >= 0) && (y < m_height))
		{
			A_long p = x + m_vurTbl[y];
			PF_Pixel* data = (PF_Pixel*)m_data;
			data[p].red = RoundByteLong((long)data[p].red * v / PF_MAX_CHAN8);
		}
	}
	// ***************************************************************
	inline  void BlendPX16(A_long x, A_long y, A_u_short v)
	{
		if ((x >= 0) && (x < m_width) && (y >= 0) && (y < m_height))
		{
			A_long p = x + m_vurTbl[y];
			PF_Pixel16* data = (PF_Pixel16*)m_data;
			data[p].red = RoundShort((long)data[p].red * v / PF_MAX_CHAN16);
		}
	}
	// ***************************************************************
	inline  void BlendPX32(A_long x, A_long y, PF_FpShort v)
	{
		if ((x >= 0) && (x < m_width) && (y >= 0) && (y < m_height))
		{
			A_long p = x + m_vurTbl[y];
			PF_PixelFloat* data = (PF_PixelFloat*)m_data;
			data[p].red = RoundFpShort(data[p].red * v);
		}
	}
#pragma endregion

	// **************************************************************
#pragma region Xline
	void Xline8(PF_FpLong x0, PF_FpLong x1, A_long y)
	{
		if (x0 > x1) {
			PF_FpLong temp = x0;
			x0 = x1;
			x1 = temp;
		}
		PF_FpLong dx = x1 - x0;
		if (dx <= 0) { return; }

		A_long xi0 = (A_long)x0;
		PF_FpLong xv0 = 1.0 - (x0 - (PF_FpLong)xi0);
		A_long xi1 = (A_long)x1;
		PF_FpLong xv1 = (x1 - (PF_FpLong)xi1);

		A_long dxi = xi1 - xi0;
		if (dxi == 0) {
			BlendPX8(xi0, y, (A_u_char)((1 - xv0) * PF_MAX_CHAN8));
		}
		else {
			BlendPX8(xi0, y, (A_u_char)((1 - xv0) * PF_MAX_CHAN8));
			BlendPX8(xi1, y, (A_u_char)((1 - xv1) * PF_MAX_CHAN8));
			if (dxi == 1) {
			}
			else if (dxi == 2) {
				BlendPX8(xi0 + 1, y, 0);
			}
			else {
				for (A_long i = xi0 + 1; i <= xi1 - 1; i++)
				{
					BlendPX8(i, y, 0);
				}
			}
		}
	}
	// **************************************************************
	void Xline16(PF_FpLong x0, PF_FpLong x1, A_long y)
	{
		if (x0 > x1) {
			PF_FpLong temp = x0;
			x0 = x1;
			x1 = temp;
		}
		PF_FpLong dx = x1 - x0;
		if (dx <= 0) { return; }

		A_long xi0 = (A_long)x0;
		PF_FpLong xv0 = 1.0 - (x0 - (PF_FpLong)xi0);
		A_long xi1 = (A_long)x1;
		PF_FpLong xv1 = (x1 - (PF_FpLong)xi1);

		A_long dxi = xi1 - xi0;
		if (dxi == 0) {
			BlendPX16(xi0, y, (A_u_short)((1 - xv0) * PF_MAX_CHAN16));
		}
		else {
			BlendPX16(xi0, y, (A_u_short)((1 - xv0) * PF_MAX_CHAN16));
			BlendPX16(xi1, y, (A_u_short)((1 - xv1) * PF_MAX_CHAN16));
			if (dxi == 1) {
			}
			else if (dxi == 2) {
				BlendPX16(xi0 + 1, y, 0);
			}
			else {
				for (A_long i = xi0 + 1; i <= xi1 - 1; i++)
				{
					BlendPX16(i, y, 0);
				}
			}
		}
	}
	// **************************************************************
	void Xline32(PF_FpLong x0, PF_FpLong x1, A_long y)
	{
		if (x0 > x1) {
			PF_FpLong temp = x0;
			x0 = x1;
			x1 = temp;
		}
		PF_FpLong dx = x1 - x0;
		if (dx <= 0) { return; }

		A_long xi0 = (A_long)x0;
		PF_FpLong xv0 = 1.0 - (x0 - (PF_FpLong)xi0);
		A_long xi1 = (A_long)x1;
		PF_FpLong xv1 = (x1 - (PF_FpLong)xi1);

		A_long dxi = xi1 - xi0;
		if (dxi == 0) {
			BlendPX32(xi0, y, (PF_FpShort)((1 - xv0)));
		}
		else {
			BlendPX32(xi0, y, (PF_FpShort)((1 - xv0)));
			BlendPX32(xi1, y, (PF_FpShort)((1 - xv1)));
			if (dxi == 1) {
			}
			else if (dxi == 2) {
				BlendPX32(xi0 + 1, y, 0);
			}
			else {
				for (A_long i = xi0 + 1; i <= xi1 - 1; i++)
				{
					BlendPX32(i, y, 0);
				}
			}
		}
	}
#pragma endregion
	// ***************************************************************
#pragma region Yline
	void Yline8(A_long x, PF_FpLong y0, PF_FpLong y1)
	{
		if (y0 > y1) {
			PF_FpLong temp = y0;
			y0 = y1;
			y1 = temp;
		}
		PF_FpLong dy = y1 - y0;
		if (dy <= 0) { return; }

		A_long yi0 = (A_long)y0;
		PF_FpLong yv0 = 1.0 - (y0 - (PF_FpLong)yi0);
		A_long yi1 = (A_long)y1;
		PF_FpLong yv1 = (y1 - (PF_FpLong)yi1);

		A_long dyi = yi1 - yi0;
		if (dyi == 0) {
			BlendPX8(x, yi0, (A_u_char)((1 - yv0) * PF_MAX_CHAN8));
		}
		else {
			BlendPX8(x, yi0, (A_u_char)((1 - yv0) * PF_MAX_CHAN8));
			BlendPX8(x, yi1, (A_u_char)((1 - yv1) * PF_MAX_CHAN8));
			if (dyi == 1) {
			}
			else if (dyi == 2) {
				BlendPX8(x, yi0 + 1, 0);
			}
			else {
				for (A_long i = yi0 + 1; i <= yi1 - 1; i++)
				{
					BlendPX8(x, i, 0);
				}
			}
		}
	}
	void Yline16(A_long x, PF_FpLong y0, PF_FpLong y1)
	{
		if (y0 > y1) {
			PF_FpLong temp = y0;
			y0 = y1;
			y1 = temp;
		}
		PF_FpLong dy = y1 - y0;
		if (dy <= 0) { return; }

		A_long yi0 = (A_long)y0;
		PF_FpLong yv0 = 1.0 - (y0 - (PF_FpLong)yi0);
		A_long yi1 = (A_long)y1;
		PF_FpLong yv1 = (y1 - (PF_FpLong)yi1);

		A_long dyi = yi1 - yi0;
		if (dyi == 0) {
			BlendPX16(x, yi0, (A_u_short)((1 - yv0) * PF_MAX_CHAN16));
		}
		else {
			BlendPX16(x, yi0, (A_u_short)((1 - yv0) * PF_MAX_CHAN16));
			BlendPX16(x, yi1, (A_u_short)((1 - yv1) * PF_MAX_CHAN16));
			if (dyi == 1) {
			}
			else if (dyi == 2) {
				BlendPX16(x, yi0 + 1, 0);
			}
			else {
				for (A_long i = yi0 + 1; i <= yi1 - 1; i++)
				{
					BlendPX16(x, i, 0);
				}
			}
		}
	}
	void Yline32(A_long x, PF_FpLong y0, PF_FpLong y1)
	{
		if (y0 > y1) {
			PF_FpLong temp = y0;
			y0 = y1;
			y1 = temp;
		}
		PF_FpLong dy = y1 - y0;
		if (dy <= 0) { return; }

		A_long yi0 = (A_long)y0;
		PF_FpLong yv0 = 1.0 - (y0 - (PF_FpLong)yi0);
		A_long yi1 = (A_long)y1;
		PF_FpLong yv1 = (y1 - (PF_FpLong)yi1);

		A_long dyi = yi1 - yi0;
		if (dyi == 0) {
			BlendPX32(x, yi0, (A_u_short)((1 - yv0)));
		}
		else {
			BlendPX32(x, yi0, (A_u_short)((1 - yv0)));
			BlendPX32(x, yi1, (A_u_short)((1 - yv1)));
			if (dyi == 1) {
			}
			else if (dyi == 2) {
				BlendPX32(x, yi0 + 1, 0);
			}
			else {
				for (A_long i = yi0 + 1; i <= yi1 - 1; i++)
				{
					BlendPX32(x, i, 0);
				}
			}
		}
	}
#pragma endregion
	// ***************************************************************
#pragma region Fill_EN
	void Fill_EN(PointInfo p)
	{

		PF_FpLong r2 = p.s * 0.75 / 2;
		A_long ri = (A_long)(r2 + 0.5);
		if (ri <= 0) return;
		for (A_long i = 0; i < ri; i++)
		{
			PF_FpLong xx = PF_SQRT(PF_POW(ri, 2) - PF_POW(i, 2));

			PF_FpLong x0 = p.p.x - xx;
			PF_FpLong x1 = p.p.x + xx;
			A_long y0 = p.p.y + i;
			A_long y1 = p.p.y - i;
			(this->*m_XLINE_FUNC)(x0, x1, y0);
			(this->*m_XLINE_FUNC)(x0, x1, y1);
		}
	}

#pragma endregion
	// ***************************************************************

#pragma region Fill
	void FillWhite8()
	{
		PF_Pixel c = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
		PF_Pixel* data = (PF_Pixel*)m_data;
		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				data[target] = c;
				target++;
			}
			target += m_offsetWidth;
		}
	}
	void FillWhite16()
	{
		PF_Pixel16 c = { PF_MAX_CHAN16,PF_MAX_CHAN16, PF_MAX_CHAN16, PF_MAX_CHAN16 };
		PF_Pixel16* data = (PF_Pixel16*)m_data;
		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				data[target] = c;
				target++;
			}
			target += m_offsetWidth;
		}
	}
	void FillWhite32()
	{
		PF_PixelFloat c = { 1,1,1,1 };
		PF_PixelFloat* data = (PF_PixelFloat*)m_data;
		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				data[target] = c;
				target++;
			}
			target += m_offsetWidth;
		}
	}



#pragma endregion
	// ***************************************************************
#pragma region Colorize
	void Colorize8(PF_Pixel c)
	{
		PF_Pixel* data = (PF_Pixel*)m_data;

		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				A_u_char v = PF_MAX_CHAN8 - data[target].red;
				data[target] = c;
				data[target].alpha = v;
				target++;
			}
			target += m_offsetWidth;
		}
	}
	void Colorize16(PF_Pixel c)
	{
		PF_Pixel16* data = (PF_Pixel16*)m_data;
		PF_Pixel16 c2 = CONV8TO16(c);
		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				A_u_short v = PF_MAX_CHAN16 - data[target].red;
				data[target] = c2;
				data[target].alpha = v;
				target++;
			}
			target += m_offsetWidth;
		}
	}
	void Colorize32(PF_Pixel c)
	{
		PF_PixelFloat* data = (PF_PixelFloat*)m_data;
		PF_PixelFloat c2 = CONV8TO32(c);
		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				PF_FpShort v2 = data[target].red;
				if (v2 > 1) v2 = 1;
				PF_FpShort v = 1 - v2;
				data[target] = c2;
				data[target].alpha = v;
				target++;
			}
			target += m_offsetWidth;
		}
	}
#pragma endregion
	// ***************************************************************


	// ***************************************************************
public:
#pragma region Line
	void Line(PointInfo start, PointInfo last)
	{
		A_long dx = ABS(last.p.x - start.p.x);
		A_long dy = ABS(last.p.y - start.p.y);

		A_long gx = 1; if (last.p.x < start.p.x) gx = -1;
		A_long gy = 1; if (last.p.y < start.p.y) gy = -1;

		PF_FpLong sz = start.s / 2;
		PF_FpLong st = start.s / 2;
		PF_FpLong lt = last.s / 2;


		if (dx >= dy)
		{
			PF_FpLong ay = (PF_FpLong)dy / (PF_FpLong)dx;
			ay *= (PF_FpLong)gy;

			A_long x = start.p.x;
			PF_FpLong y = (PF_FpLong)start.p.y;

			//sz = sz * (PF_COS(PF_ATAN2(dx, dy)));

			for (A_long i = 0; i < dx; i++)
			{
				sz = st + (lt - st) * i / dx;

				(this->*m_YLINE_FUNC)(x, y - sz, y + sz);
				x += gx;
				y += ay;
			}
		}
		else {
			PF_FpLong ax = (PF_FpLong)dx / (PF_FpLong)dy;
			ax *= (PF_FpLong)gx;

			A_long y = start.p.y;
			PF_FpLong x = (PF_FpLong)start.p.x;

			//sz = sz * (PF_COS(PF_ATAN2(dx, dy)));

			for (A_long i = 0; i < dy; i++)
			{
				sz = st + (lt - st) * i / dy;
				(this->*m_XLINE_FUNC)(x - sz, x + sz, y);
				x += ax;
				y += gy;
			}

		}
	}
	void Line(CPointInfo pnts)
	{
		if (pnts.Count() <= 0) return;

		for (A_long i = 0; i < pnts.Count(); i++)
		{
			PointInfo s = pnts.Points[i];
			Fill_EN(s);
			PointInfo d = pnts.Points[i+1];
			if (d.s < 0) break;
			Line(s, d);
		}

	}
	void Line()
	{
		Line(Points);
	}
#pragma endregion
	void FillWhite()
	{
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			FillWhite32();
			break;
		case PF_PixelFormat_ARGB64:
			FillWhite16();
			break;
		case PF_PixelFormat_ARGB32:
		default:
			FillWhite8();
			break;
		}
	}
	void Colorize(PF_Pixel c)
	{
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			Colorize32(c);
			break;
		case PF_PixelFormat_ARGB64:
			Colorize16(c);
			break;
		case PF_PixelFormat_ARGB32:
		default:
			Colorize8(c);
			break;
		}
	}
	// ***************************************************************
	// ***************************************************************
	CLineDraw(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32,
		A_long bufSize = POINT_TABLE_SIZE
	)
	{
		Points.in_data = ind;
		in_data = ind;
		world = wld;
		m_format = fmt;

		if (bufSize < 256) bufSize = 256;

		if (wld != NULL) {
			m_data = wld->data;
			m_width = wld->width;
			m_height = wld->height;

			switch (m_format)
			{
			case PF_PixelFormat_ARGB128:
				m_widthTrue = wld->rowbytes / sizeof(PF_Pixel32);
				m_XLINE_FUNC = &CLineDraw::Xline32;
				m_YLINE_FUNC = &CLineDraw::Yline32;
				break;
			case PF_PixelFormat_ARGB64:
				m_widthTrue = wld->rowbytes / sizeof(PF_Pixel16);
				m_XLINE_FUNC = &CLineDraw::Xline16;
				m_YLINE_FUNC = &CLineDraw::Yline16;
				break;
			default:
			case PF_PixelFormat_ARGB32:
				m_widthTrue = wld->rowbytes / sizeof(PF_Pixel8);
				m_XLINE_FUNC = &CLineDraw::Xline8;
				m_YLINE_FUNC = &CLineDraw::Yline8;
				break;
			}
			m_offsetWidth = m_widthTrue - m_width;


			A_long bufLen = m_height * sizeof(A_long);
			bufLen += bufSize * sizeof(PointInfo);
			bufLen *= 3;
			m_bufH = PF_NEW_HANDLE(bufLen);
			if (m_bufH) {
				m_vurTbl = *(A_long**)m_bufH;
				PF_LOCK_HANDLE(m_bufH);
				for (A_long i = 0; i < m_height; i++)
				{
					m_vurTbl[i] = i * m_widthTrue;
				}
				A_long* v = m_vurTbl + m_height + 10;
				Points.SetParams(in_data, (PointInfoP)v, CPointInfo::PointsSizeMax,0);
				Points.Clear();

			}
			FillWhite();
		}

	}
	// ***************************************************************
	~CLineDraw()
	{
		if (m_bufH != NULL)
		{
			PF_UNLOCK_HANDLE(m_bufH);
			PF_DISPOSE_HANDLE(m_bufH);
			m_bufH = NULL;
		}
	}
	// ***************************************************************
	void exec()
	{
		FillWhite();
		PointInfo p0;
		PointInfo p1;
		PointInfo p2;
		PointInfo p3;
		p0.p.x = 00;
		p0.p.y = 00;
		p0.s = 20;

		p1.p.x = 150;
		p1.p.y = 200;
		p1.s = 50;

		p2.p.x = 300;
		p2.p.y = 800;
		p2.s = 30;

		p3.p.x = 400;
		p3.p.y = 100;
		p3.s = 2;

		Points.Push(p0);
		Points.Push(p1);
		Points.Push(p2);

		Points.insert(1, p3);

		Line();

	}
};
#endif