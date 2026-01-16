#pragma once
#ifndef FRectGrad_H
#define FRectGrad_H

#include "../FsLibrary/Fs.h"



class FRectGrad
{
private:
	A_FloatRect		m_rect;
	A_FloatPoint	m_center;
	PF_FpLong		m_angle;

	PF_FpLong		m_dx;
	PF_FpLong		m_dy;

	PF_FpLong		m_dia;

	A_FloatPoint	m_start;
	A_FloatPoint	m_last;

	PF_FpLong		m_startOver;
	PF_FpLong		m_lastOver;

public:
	PF_InData* in_data;
	A_FloatPoint start() { return m_start; }
	A_FloatPoint last() { return m_last; }
private:
	void CalcPoint()
	{
		A_FloatPoint s0;
		A_FloatPoint s1;
		s0.x = s0.y = 0;

		PF_FpLong v = 8000;

		if ((m_angle >= 0) && (m_angle < 90)) {
			s0.x = v * PF_SIN(m_angle * PF_PI / 180);
			s0.y = -v * PF_COS(m_angle * PF_PI / 180);
		}else if ((m_angle >= 90) && (m_angle < 180)) {
			s0.x = v * PF_COS((m_angle - 90) * PF_PI / 180);
			s0.y = v * PF_SIN((m_angle - 90) * PF_PI / 180);
		}
		else if ((m_angle >= 180) && (m_angle < 270)) {
			s0.x = -v * PF_SIN((m_angle - 180) * PF_PI / 180);
			s0.y = v * PF_COS((m_angle - 180) * PF_PI / 180);
		}
		else if (m_angle >= 270)  {
			s0.x = -v * PF_COS((m_angle - 270) * PF_PI / 180);
			s0.y = -v * PF_SIN((m_angle - 270) * PF_PI / 180);
		}
		s1.x = s0.x * -1;
		s1.y = s0.y * -1;

		A_FloatPoint b0, b1;
		if ( ((m_angle >= 0) && (m_angle < m_dia))||((m_angle >= 360 - m_dia) && (m_angle < 360)))
		{
			b0.x = -v; 
			b0.y = -m_dy;
			b1.x = v;
			b1.y = -m_dy;
			CrossPointer(s0, s1, b0, b1, &m_start);
			b0.y = m_dy; 
			b1.y = m_dy;
			CrossPointer(s0, s1, b0, b1, &m_last);
		}
		else if ((m_angle >= m_dia) && (m_angle < 180 -m_dia))
		{
			b0.x = m_dx;
			b0.y = -v; 
			b1.x = m_dx;
			b1.y = v;
			CrossPointer(s0, s1, b0, b1, &m_start);
			b0.x = -m_dx;
			b1.x = -m_dx;
			CrossPointer(s0, s1, b0, b1, &m_last);

		}
		else if ((m_angle >= 180 - m_dia) && (m_angle < 180 + m_dia))
		{
			b0.x = -v;
			b0.y = m_dy;
			b1.x = v;
			b1.y = m_dy;
			CrossPointer(s0, s1, b0, b1, &m_start);
			b0.y = -m_dy;
			b1.y = -m_dy;
			CrossPointer(s0, s1, b0, b1, &m_last);
		}
		else if ((m_angle >= 180 + m_dia) && (m_angle < 360 - m_dia))
		{
			b0.x = -m_dx;
			b0.y = -v;
			b1.x = -m_dx;
			b1.y = v;
			CrossPointer(s0, s1, b0, b1, &m_start);
			b0.x = m_dx;
			b1.x = m_dx;
			CrossPointer(s0, s1, b0, b1, &m_last);
		}
		m_start.x *= m_startOver;
		m_start.y *= m_startOver;

		m_last.x *= m_lastOver;
		m_last.y *= m_lastOver;

		m_start.x += m_center.x;
		m_start.y += m_center.y;
		m_last.x += m_center.x;
		m_last.y += m_center.y;
	}
public:
	void SetStartOver(PF_FpLong p)
	{
		if (m_startOver == p) return;
		CalcPoint();
	}
	void SetLastOver(PF_FpLong p)
	{
		if (m_lastOver == p) return;
		CalcPoint();
	}
	void SetOver(PF_FpLong s, PF_FpLong l)
	{
		m_startOver = s;
		m_lastOver = l;
		CalcPoint();
	}
	void SetRect(A_LRect rct)
	{
		m_rect.left = (PF_FpLong)rct.left;
		m_rect.right = (PF_FpLong)rct.right;
		m_rect.top = (PF_FpLong)rct.top;
		m_rect.bottom = (PF_FpLong)rct.bottom;

		m_dx = (m_rect.right - m_rect.left) / 2;
		m_dy = (m_rect.bottom - m_rect.top) / 2;

		m_center.x = m_rect.left + m_dx;
		m_center.y = m_rect.top + m_dy;

		m_dia = PF_ATAN2(m_dx, m_dy) *180/PF_PI;
		CalcPoint();
	}
	PF_FpLong rotMod(PF_FpLong r)
	{
		PF_Fixed rr = (PF_Fixed)(r * (PF_Fixed)(1L << 16) + 0.5);
		rr %= (360L << 16);
		if (rr < 0) rr += (360L << 16);
		r = (PF_FpLong)rr / (PF_FpLong)(1L << 16);
		return r;

	}
	void SetAngle(PF_FpLong r)
	{
		r = rotMod(r);
		if (m_angle != r) {
			m_angle = r;
			CalcPoint();
		}
	}
	FRectGrad(PF_InData *ind, A_LRect rct,PF_FpLong r, PF_FpLong so, PF_FpLong lo )
	{
		m_dia = 0;
		in_data = ind;
		m_angle = rotMod(r);
		m_center.x = 100;
		m_center.y = 100;
		m_dx = 100;
		m_dy = 100;
		m_rect.left = m_center.x - m_dx;
		m_rect.right = m_center.x + m_dx;
		m_rect.top = m_center.y - m_dy;
		m_rect.bottom = m_center.y + m_dy;
		m_startOver = so;
		m_lastOver = lo;

		SetRect(rct);
	}
	virtual ~FRectGrad()
	{

	}
};

#endif

