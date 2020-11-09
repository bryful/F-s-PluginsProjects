#pragma once
#ifndef FBlendSelected_H
#define FBlendSelected_H

#include "Fs.h"
#include "FsUtils.h"

#include "FWorld.h"
#include "FBlend.h"


class FBlendSelected : public FBlend
{
private:
	PF_Boolean m_Enabled = FALSE;

	A_long m_selectCount = 0;
	PF_Pixel m_selectCol8[16];
	PF_Pixel16 m_selectCol16[16];
	PF_PixelFloat m_selectCol32[16];

	A_u_char	m_cmpLv8 = 0;
	A_u_short	m_cmpLv16 = 0;
	PF_FpShort	m_cmpLv32 = 0;

	// ***************************************************************
	A_long FindColor(PF_Pixel c)
	{
		A_long ret = -1;
		if (m_selectCount <= 0) return ret;

		for (A_long i = 0; i < m_selectCount; i++)
		{
			if ((m_selectCol8[i].red == c.red) && (m_selectCol8[i].green == c.green)(m_selectCol8[i].blue == c.blue))
			{
				ret = i;
				break;
			}
		}
		return ret;
	}
	// ***************************************************************
public:
	PF_InData* in_data = NULL;
	FWorld src = NULL;
	FWorld dst = NULL;

	PF_Boolean Enabled() { return m_Enabled; };
	PF_FpLong CmpLv() { return m_cmpLv32; };
	PF_FpLong SetCmpLv(PF_FpLong par)
	{
		if (par < 0) par = 0; else if (par > 1) par = 1;
		m_cmpLv32 = par;
		m_cmpLv8 = PF_MAX_CHAN8 * par;
		m_cmpLv16 = PF_MAX_CHAN16 * par;

	};

	// ***************************************************************
	FBlendSelected(
		PF_InData* ind,
		FWorld s,
		FWorld d

	) : FBlend(ind, s, d)
	{
		m_selectCount = 0;
		m_cmpLv32 = 0;
		m_cmpLv16 = 0;
		m_cmpLv8 = 0;
	}
	// ***************************************************************
	void PushColor(PF_Pixel c)
	{
		if (m_selectCount >= 16) return;
		A_long idx = FindColor(c);
		if (idx < 0)
		{
			m_selectCol8[m_selectCount] = c;
			m_selectCol16[m_selectCount] = CONV8TO16(c);
			m_selectCol32[m_selectCount] = CONV8TO32(c);
			m_selectCount++;
		}
	}
	// ***************************************************************
	virtual ~FBlendSelected()
	{
	}
private:
	// ***************************************************************
	void selectedCol8()
	{
		if (m_Enabled == FALSE) return;
		for (A_long y = 0; y < dst.height(); y++)
		{
			for (A_long x = 0; x < dst.width(); x++)
			{
				PF_Pixel s = src.GetPix8(x, y);
				for (A_long j = 0; j < m_selectCount; j++)
				{
					if (compPix8Lv(s, m_selectCol8[j], m_cmpLv8) == TRUE) {
						dst.SetPix8(x,y, s);
					}
				}
			}
		}
	}
	// ***************************************************************
	void selectedCol16()
	{
		if (m_Enabled == FALSE) return;
		for (A_long y = 0; y < dst.height(); y++)
		{
			for (A_long x = 0; x < dst.width(); x++)
			{
				PF_Pixel16 s = src.GetPix16(x, y);
				for (A_long j = 0; j < m_selectCount; j++)
				{
					if (compPix16Lv(s, m_selectCol16[j], m_cmpLv16) == TRUE) {
						dst.SetPix16(x,y, s);
					}
				}
			}
		}
	}
	// ***************************************************************
	void selectedCol32()
	{
		if (m_Enabled == FALSE) return;
		for (A_long y = 0; y < dst.height(); y++)
		{
			for (A_long x = 0; x < dst.width(); x++)
			{
				PF_PixelFloat s = src.GetPix32(x, y);
				for (A_long j = 0; j < m_selectCount; j++)
				{
					if (compPix32Lv(s, m_selectCol32[j], m_cmpLv32) == TRUE) {
						dst.SetPix32(x, y, s);
					}
				}
			}
		}
	}
	// ***************************************************************
public:
	void SelectedColCopy()
	{
		if (m_Enabled == FALSE) return;
		if (m_selectCount <= 0) return;
		switch (dst.pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			selectedCol32();
			break;
		case PF_PixelFormat_ARGB64:
			selectedCol16();
			break;
		case PF_PixelFormat_ARGB32:
			selectedCol8();
			break;
		}
	}

};
#endif