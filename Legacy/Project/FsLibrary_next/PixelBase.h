#pragma once
#ifndef PIXELBASE_H
#define PIXELBASE_H


#include "AE_Effect.h"

#define C8to16(A)		( (((A_long)(A) * PF_MAX_CHAN16) + PF_HALF_CHAN8) / PF_MAX_CHAN8 )
#define C8to32(A)		((double)(long)((double)A*10000.0/(double)PF_MAX_CHAN8 + 0.5)/10000.0)
#define C16to8(A)		( (((A_long)(A) * PF_MAX_CHAN8) + PF_HALF_CHAN16) / PF_MAX_CHAN16)
#define C16to32(A)      ((double)(long)((double)A*10000.0/(double)PF_MAX_CHAN16 + 0.5)/10000.0)
#define C32to8(A)		(  (A_long)((A)*PF_MAX_CHAN8+0.5) )
#define C32to16(A)		(  C8to16(C32to8(A)) )
#define ROUNDV(A)		do{if(A<0)A=0;else if(A>1)A=1;   }while(0)
#define ROUNDB(A)		do{if(A<0)A=0;else if(A>PF_MAX_CHAN8)A=PF_MAX_CHAN8;   }while(0)
#define ROUNDS(A)		do{if(A<0)A=0;else if(A>PF_MAX_CHAN16)A=PF_MAX_CHAN16;   }while(0)


#define FS_SHORT(A)				((double)(long)((double)A*10000.0 + 0.5)/10000.0)


class PixelBase
{
private:
	double m_r;
	double m_g;
	double m_b;
	double m_a;
	inline A_u_char RoundByte(double v) { double r = v; if (r < 0)r = 0; else if (r > PF_MAX_CHAN8)r = PF_MAX_CHAN8; return (A_u_char)r; }
	inline A_u_short RoundShort(double v) { double r = v; if (r < 0)r = 0; else if (r > PF_MAX_CHAN16)r = PF_MAX_CHAN16; return (A_u_short)r; }
	inline PF_FpShort RoundFloat(double v) { double r = v; if (r < 0)r = 0; else if (r > 1)r = 1; return (PF_FpShort)r; }

public:
	// *************************************************************
	A_u_char R8() { return (A_u_char)C32to8(m_r); }
	A_u_char G8() { return (A_u_char)C32to8(m_g); }
	A_u_char B8() { return (A_u_char)C32to8(m_b); }
	A_u_char A8() { return (A_u_char)C32to8(m_a); }
	A_u_short R16() { return (A_u_short)C32to16(m_r); }
	A_u_short G16() { return (A_u_short)C32to16(m_g); }
	A_u_short B16() { return (A_u_short)C32to16(m_b); }
	A_u_short A16() { return (A_u_short)C32to16(m_a); }
	double R32() { return m_r; }
	double G32() { return m_g; }
	double B32() { return m_b; }
	double A32() { return m_a; }
	// *************************************************************
	void SetR8(A_u_char v) { m_r = (double)C8to32(v); }
	void SetG8(A_u_char v) { m_g = (double)C8to32(v); }
	void SetB8(A_u_char v) { m_b = (double)C8to32(v); }
	void SetA8(A_u_char v) { m_a = (double)C8to32(v); }
	void SetR16(A_u_short v) { m_r = (double)C16to32(v); }
	void SetG16(A_u_short v) { m_g = (double)C16to32(v); }
	void SetB16(A_u_short v) { m_b = (double)C16to32(v); }
	void SetA16(A_u_short v) { m_a = (double)C16to32(v); }
	void SetR32(double v) { m_r = (double)(v); }
	void SetG32(double v) { m_g = (double)(v); }
	void SetB32(double v) { m_b = (double)(v); }
	void SetA32(double v) { m_a = (double)(v); }
	
	// *************************************************************
	void AddR32(double v) { m_r += (double)(v); }
	void AddG32(double v) { m_g += (double)(v); }
	void AddB32(double v) { m_b += (double)(v); }
	void AddA32(double v) { m_a += (double)(v); }
	void AddR16(A_u_short v) { m_r += (double)C16to32(v); }
	void AddG16(A_u_short v) { m_g += (double)C16to32(v); }
	void AddB16(A_u_short v) { m_b += (double)C16to32(v); }
	void AddA16(A_u_short v) { m_a += (double)C16to32(v); }
	void AddR8(A_u_char v) { m_r += (double)C8to32(v); }
	void AddG8(A_u_char v) { m_g += (double)C8to32(v); }
	void AddB8(A_u_char v) { m_b += (double)C8to32(v); }
	void AddA8(A_u_char v) { m_a += (double)C8to32(v); }

	// *************************************************************
	void DivR32(double v) { m_r /= (double)(v); }
	void DivG32(double v) { m_g /= (double)(v); }
	void DivB32(double v) { m_b /= (double)(v); }
	void DivA32(double v) { m_a /= (double)(v); }


	// *************************************************************
	PixelBase(PF_Pixel8 p)
	{
		FromPx8(p);
	}
	// *************************************************************
	PixelBase(PF_Pixel16 p)
	{
		FromPx16(p);
	}
	// *************************************************************
	PixelBase(PF_Pixel32 p)
	{
		FromPx32(p);
	}
	// *************************************************************
	PixelBase()
	{
		m_r = 0;
		m_g = 0;
		m_b = 0;
		m_a = 0;
	}
	// *************************************************************
	PixelBase(A_u_char r, A_u_char g, A_u_char b, A_u_char a)
	{
		m_r = (double)C8to32(r);
		m_g = (double)C8to32(g);
		m_b = (double)C8to32(b);
		m_a = (double)C8to32(a);
	}
	// *************************************************************
	PixelBase(A_u_short r, A_u_short g, A_u_short b, A_u_short a)
	{
		m_r = (double)C16to32(r);
		m_g = (double)C16to32(g);
		m_b = (double)C16to32(b);
		m_a = (double)C16to32(a);
	}
	// *************************************************************
	PixelBase(PF_FpShort r, PF_FpShort g, PF_FpShort b, PF_FpShort a)
	{
		m_r = (double)(r);
		m_g = (double)(g);
		m_b = (double)(b);
		m_a = (double)(a);
	}


	// *************************************************************
	void FromPx8(PF_Pixel8 p)
	{
		m_r = (double)C8to32(p.red);
		m_g = (double)C8to32(p.green);
		m_b = (double)C8to32(p.blue);
		m_a = (double)C8to32(p.alpha);
	}
	// *************************************************************
	void FromPx16(PF_Pixel16 p)
	{
		m_r = (double)C16to32(p.red);
		m_g = (double)C16to32(p.green);
		m_b = (double)C16to32(p.blue);
		m_a = (double)C16to32(p.alpha);
	}
	// *************************************************************
	void FromPx32(PF_PixelFloat p)
	{
		m_r = (double)(p.red);
		m_g = (double)(p.green);
		m_b = (double)(p.blue);
		m_a = (double)(p.alpha);
	}
	// *************************************************************
	PF_Pixel ToPx8()
	{
		PF_Pixel p;
		p.red = RoundByte(C32to8(m_r));
		p.green = RoundByte(C32to8(m_g));
		p.blue = RoundByte(C32to8(m_b));
		p.alpha = RoundByte(C32to8(m_a));
		return p;

	}
	// *************************************************************
	PF_Pixel16 ToPx16()
	{
		PF_Pixel16 p;
		p.red = RoundShort(C32to16(m_r));
		p.green = RoundShort(C32to16(m_g));
		p.blue = RoundShort(C32to16(m_b));
		p.alpha = RoundShort(C32to16(m_a));
		return p;

	}
	// *************************************************************
	PF_PixelFloat ToPx32()
	{
		PF_PixelFloat p;
		p.red = RoundFloat(m_r);
		p.green = RoundFloat(m_g);
		p.blue = RoundFloat(m_b);
		p.alpha = RoundFloat(m_a);
		return p;
	}
	// *************************************************************
	void RoundValue()
	{
		ROUNDV(m_r);
		ROUNDV(m_g);
		ROUNDV(m_b);
		ROUNDV(m_a);

	}
	// *************************************************************
	void ToPremult()
	{
		if (m_a < 0) m_a = 0;
		else if (m_a > 1) m_a = 1;
		m_r = m_r * m_a;
		m_g = m_r * m_g;
		m_b = m_r * m_a;

	}
	// *************************************************************
	void FromPremult()
	{
		if (m_a < 0) m_a = 0;
		else if (m_a > 1) m_a = 1;

		if (m_a <= 0)
		{
			m_r = m_g = m_b = 0;
		}
		else {
			m_r = m_r / m_a;
			m_g = m_r / m_a;
			m_b = m_r / m_a;
		}

	}
	// *************************************************************
	double Lum()
	{
		return ((0.29891 * m_r) + (0.58661 * m_g) + (0.11448 * m_b));
	}
	// *************************************************************
	double MaxRGB()
	{
		return MAX(MAX(m_r, m_g), m_b);
	}
	// *************************************************************
	double Len(PF_Pixel p)
	{
		//pow()
	}
	// *************************************************************
	PixelBase operator + (PixelBase c)
	{
		PixelBase ret;
		ret.SetR32(m_r + c.R32());
		ret.SetG32(m_g + c.G32());
		ret.SetB32(m_g + c.B32());
		ret.SetA32(m_a + c.A32());
		return ret;

	}
	// *************************************************************
	void operator += (PixelBase c)
	{
		SetR32(m_r + c.m_r);
		SetG32(m_g + c.m_g);
		SetB32(m_g + c.m_b);
		SetA32(m_a + c.m_a);

	}
};



#endif


