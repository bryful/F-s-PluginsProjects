#pragma once

#include "AE_Effect.h"

#ifndef ABS
	#define ABS(x) ((x) >= 0 ? (x) : -(x))
#endif


class PF_Pixels
{
private:
	PF_FpLong	m_r;
	PF_FpLong	m_g;
	PF_FpLong	m_b;
	PF_FpLong	m_a;

	A_u_char ToAUC(PF_FpLong v)
	{
		A_long v2 = (A_long)(v * PF_MAX_CHAN8 + 0.5);
		if (v2 < 0) v2 = 0; else if (v2 > PF_MAX_CHAN8) v2 = PF_MAX_CHAN8;
		return (A_u_char)v2;
	}
	A_u_char ToAUS(PF_FpLong v)
	{
		A_long v2 = (A_long)(v * PF_MAX_CHAN16 + 0.5);
		if (v2 < 0) v2 = 0; else if (v2 > PF_MAX_CHAN16) v2 = PF_MAX_CHAN16;
		return (A_u_short)v2;
	}
	PF_FpShort ToFpShort(PF_FpLong v)
	{
		if (v < 0) v = 0; else if (v > 1) v = 1;
		return (PF_FpShort)v;
	}
public:
	// **********************************************************
	PF_Pixels(PF_FpLong r=0, PF_FpLong g = 0, PF_FpLong b = 0, PF_FpLong a = 0 )
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}
	PF_Pixels(PF_Pixel v)
	{
		SetValue(v);
	}
	PF_Pixels(PF_Pixel16 v)
	{
		SetValue(v);
	}
	PF_Pixels(PF_Pixel32 v)
	{
		SetValue(v);
	}

	// **********************************************************
	~PF_Pixels()
	{

	}
	// **********************************************************
	PF_FpLong R() { return m_r; }
	PF_FpLong G() { return m_g; }
	PF_FpLong B() { return m_b; }
	PF_FpLong A() { return m_a; }
	void SetR(PF_FpLong v) { m_r = v; }
	void SetG(PF_FpLong v) { m_g = v; }
	void SetB(PF_FpLong v) { m_b = v; }
	void SetA(PF_FpLong v) { m_a = v; }
	void AddR(PF_FpLong v) { m_r += v; }
	void AddG(PF_FpLong v) { m_g += v; }
	void AddB(PF_FpLong v) { m_b += v; }
	void AddA(PF_FpLong v) { m_a += v; }
	void DivR(PF_FpLong v) { m_r /= v; }
	void DivG(PF_FpLong v) { m_g /= v; }
	void DivB(PF_FpLong v) { m_b /= v; }
	void DivA(PF_FpLong v) { m_a /= v; }
	void MulR(PF_FpLong v) { m_r *= v; }
	void MulG(PF_FpLong v) { m_g *= v; }
	void MulB(PF_FpLong v) { m_b *= v; }
	void MulA(PF_FpLong v) { m_a *= v; }

	// **********************************************************
	void ToPremultiply()
	{
		m_r *= m_a;
		m_g *= m_a;
		m_b *= m_a;
	}
	// **********************************************************
	void ToPremultiply()
	{
		if (m_a <= 0)
		{
			m_r = m_g = m_b = 0;
		}
		else if (m_a < 1)
		{
			m_r /= m_a;
			m_g /= m_a;
			m_b /= m_a;
		}
	}
	// **********************************************************
	void SetValue(PF_Pixel v)
	{
		m_r = (PF_FpLong)v.red / PF_MAX_CHAN8;
		m_g = (PF_FpLong)v.green / PF_MAX_CHAN8;
		m_b = (PF_FpLong)v.blue / PF_MAX_CHAN8;
		m_a = (PF_FpLong)v.alpha / PF_MAX_CHAN8;
	}
	// **********************************************************
	void SetValue(PF_Pixel16 v)
	{
		m_r = (PF_FpLong)v.red / PF_MAX_CHAN16;
		m_g = (PF_FpLong)v.green / PF_MAX_CHAN16;
		m_b = (PF_FpLong)v.blue / PF_MAX_CHAN16;
		m_a = (PF_FpLong)v.alpha / PF_MAX_CHAN16;
	}
	// **********************************************************
	void SetValue(PF_PixelFloat v)
	{
		m_r = (PF_FpLong)v.red;
		m_g = (PF_FpLong)v.green;
		m_b = (PF_FpLong)v.blue;
		m_a = (PF_FpLong)v.alpha;
	}
	// **********************************************************
	PF_Pixel Value()
	{
		PF_Pixel ret;
		ret.red = ToAUC(m_r);
		ret.green = ToAUC(m_g);
		ret.blue = ToAUC(m_b);
		ret.alpha = ToAUC(m_a);
		return ret;
	}
	operator PF_Pixel() { return Value(); }
	operator PF_Pixel*() { PF_Pixel r = Value(); return &r; }

	// **********************************************************
	PF_Pixel16 Value16()
	{
		PF_Pixel16 ret;
		ret.red = ToAUS(m_r);
		ret.green = ToAUS(m_g);
		ret.blue = ToAUS(m_b);
		ret.alpha = ToAUS(m_a);
		return ret;
	}
	operator PF_Pixel16() { return Value16(); }
	operator PF_Pixel16*() { PF_Pixel16 r = Value16(); return &r; }
	// **********************************************************
	PF_Pixel32 Value32()
	{
		PF_Pixel32 ret;
		ret.red = ToFpShort(m_r);
		ret.green = ToFpShort(m_g);
		ret.blue = ToFpShort(m_b);
		ret.alpha = ToFpShort(m_a);
		return ret;
	}
	operator PF_Pixel32() { return Value32(); }
	operator PF_Pixel32*() { PF_Pixel32 r = Value32(); return &r; }
	// **********************************************************
	const PF_Boolean operator == (const PF_Pixels& v) const
	{
		return ((m_r == v.m_r) && (m_g == v.m_g) && (m_b == v.m_b) && (m_a == v.m_a));

	}
	// **********************************************************
	PF_Boolean Compare(const PF_Pixels& v, PF_FpLong lv = 0)
	{
		if (lv == 0) {
			return ((m_r == v.m_r) && (m_g == v.m_g) && (m_b == v.m_b));
		}
		else {
			return ((ABS(m_r - v.m_r) <= lv) && (ABS(m_g - v.m_g) <= lv) && (ABS(m_b - v.m_b) <= lv));
		}
	}
	// **********************************************************
	const PF_Boolean operator!=(const PF_Pixels& v) const
	{
		return !(*this == v);
	}
	// **********************************************************
	const PF_Pixels operator+(const PF_Pixels& v) const
	{
		PF_Pixels tmp;
		tmp.m_r = m_r + v.m_r;
		tmp.m_g = m_g + v.m_g;
		tmp.m_b = m_b + v.m_b;
		tmp.m_a = m_a + v.m_a;
		return tmp;
	}
	// **********************************************************
	void operator+=(const PF_Pixels& v)
	{
		m_r = m_r + v.m_r;
		m_g = m_g + v.m_g;
		m_b = m_b + v.m_b;
		m_a = m_a + v.m_a;
	}
	// **********************************************************
	const PF_Pixels operator/(const A_long& v) const
	{
		PF_Pixels tmp;
		tmp.m_r = m_r / v;
		tmp.m_g = m_g / v;
		tmp.m_b = m_b / v;
		tmp.m_a = m_a / v;
		return tmp;
	}
	// **********************************************************
	void operator/=(const A_long& v)
	{
		m_r = m_r / v;
		m_g = m_g / v;
		m_b = m_b / v;
		m_a = m_a / v;
	}
};





