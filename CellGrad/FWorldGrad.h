#pragma once
#ifndef FWorldGrad_H
#define FWorldGrad_H

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"
#include "../FsLibrary/FWorld.h"


class FWorldGrad : public FWorld
{
private:
	PF_Handle	m_scanlineH = NULL;

public:

	// ***************************************************************
#pragma region  Constractor
	FWorldGrad(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	): FWorld(wld,ind,fmt)
	{

		A_long sz = m_width;
		if (sz < m_height) sz = m_height;

		A_long bufLen = sz * sizeof(PF_FpLong);
		bufLen *= 2;
		m_scanlineH = PF_NEW_HANDLE(bufLen);
		if (m_scanlineH) {
			PF_LOCK_HANDLE(m_scanlineH);
		}
	}
	// ***************************************************************
public:
	virtual ~FWorldGrad()
	{
		if (m_scanlineH != NULL)
		{
			PF_UNLOCK_HANDLE(m_scanlineH);
			PF_DISPOSE_HANDLE(m_scanlineH);
			m_scanlineH = NULL;
		}
	}
#pragma endregion
	// ***************************************************************
#pragma region GradMask
private:
	PF_Err GradMask8()
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				m_data8[target].red = m_data8[target].alpha;
				m_data8[target].green = m_data8[target].alpha;
				m_data8[target].blue = m_data8[target].alpha;
				target++;
			}
			target += m_offsetWidth;
		}
		return err;
	}
	PF_Err GradMask16()
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				m_data16[target].red = m_data16[target].alpha;
				m_data16[target].green = m_data16[target].alpha;
				m_data16[target].blue = m_data16[target].alpha;
				target++;
			}
			target += m_offsetWidth;
		}
		return err;
	}
	PF_Err GradMask32()
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_long target = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			for (A_long x = 0; x < m_width; x++)
			{
				m_data32[target].red = m_data32[target].alpha;
				m_data32[target].green = m_data32[target].alpha;
				m_data32[target].blue = m_data32[target].alpha;
				target++;
			}
			target += m_offsetWidth;
		}
		return err;
	}
#pragma endregion

#pragma region Min
	PF_Err Min8(A_long v)
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_u_char *scanline = *(A_u_char**)m_scanlineH;

		for (A_long y = 0; y < m_height; y++)
		{
			A_long adrY = m_vurTbl[y];

			for (A_long x = 0; x < m_width; x++)
			{
				scanline[x] = m_data8[x + adrY].red;
			}
			for (A_long x = 0; x < m_width; x++)
			{
				A_long x0 = x - v;
				if (x0 < 0) x0 = 0;
				A_long x1 = x + v;
				if (x1 >= m_width) x1 = m_width -1;

				A_u_char loV = PF_MAX_CHAN8;
				for (A_long k = x0; k <=x1; k++)
				{
					if (loV > scanline[k]) loV = scanline[k];
					if (loV <= 0) break;
				}
				m_data8[x + adrY].red = loV;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			for (A_long y = 0; y < m_height; y++)
			{
				scanline[y] = m_data8[x + m_vurTbl[y]].red;
			}
			for (A_long y = 0; y < m_height; y++)
			{
				A_long y0 = y - v;
				if (y0 < 0) y0 = 0;
				A_long y1 = y + v;
				if (y1 >= m_height) y1 = m_height - 1;

				A_u_char loV = PF_MAX_CHAN8;
				for (A_long k = y0; k <= y1; k++)
				{
					if (loV > scanline[k]) loV = scanline[k];
					if (loV <= 0) break;
				}
				m_data8[x + m_vurTbl[y]].red = loV;
			}
		}

		return err;
	}
	PF_Err Min16(A_long v)
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_u_short* scanline = *(A_u_short**)m_scanlineH;

		for (A_long y = 0; y < m_height; y++)
		{
			A_long adrY = m_vurTbl[y];

			for (A_long x = 0; x < m_width; x++)
			{
				scanline[x] = m_data16[x + adrY].red;
			}
			for (A_long x = 0; x < m_width; x++)
			{
				A_long x0 = x - v;
				if (x0 < 0) x0 = 0;
				A_long x1 = x + v;
				if (x1 >= m_width) x1 = m_width - 1;

				A_u_short loV = PF_MAX_CHAN16;
				for (A_long k = x0; k <= x1; k++)
				{
					if (loV > scanline[k]) loV = scanline[k];
					if (loV <= 0) break;
				}
				m_data16[x + adrY].red = loV;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			for (A_long y = 0; y < m_height; y++)
			{
				scanline[y] = m_data16[x + m_vurTbl[y]].red;
			}
			for (A_long y = 0; y < m_height; y++)
			{
				A_long y0 = y - v;
				if (y0 < 0) y0 = 0;
				A_long y1 = y + v;
				if (y1 >= m_height) y1 = m_height - 1;

				A_u_short loV = PF_MAX_CHAN16;
				for (A_long k = y0; k <= y1; k++)
				{
					if (loV > scanline[k]) loV = scanline[k];
					if (loV <= 0) break;
				}
				m_data16[x + m_vurTbl[y]].red = loV;
			}
		}

		return err;
	}
	PF_Err Min32(A_long v)
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		PF_FpShort* scanline = *(PF_FpShort**)m_scanlineH;

		for (A_long y = 0; y < m_height; y++)
		{
			A_long adrY = m_vurTbl[y];

			for (A_long x = 0; x < m_width; x++)
			{
				scanline[x] = m_data32[x + adrY].red;
			}
			for (A_long x = 0; x < m_width; x++)
			{
				A_long x0 = x - v;
				if (x0 < 0) x0 = 0;
				A_long x1 = x + v;
				if (x1 >= m_width) x1 = m_width - 1;

				PF_FpShort loV = 1.0;
				for (A_long k = x0; k <= x1; k++)
				{
					if (loV > scanline[k]) loV = scanline[k];
					if (loV <= 0) break;
				}
				m_data32[x + adrY].red = loV;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			for (A_long y = 0; y < m_height; y++)
			{
				scanline[y] = m_data32[x + m_vurTbl[y]].red;
			}
			for (A_long y = 0; y < m_height; y++)
			{
				A_long y0 = y - v;
				if (y0 < 0) y0 = 0;
				A_long y1 = y + v;
				if (y1 >= m_height) y1 = m_height - 1;

				PF_FpShort loV = 1.0;
				for (A_long k = y0; k <= y1; k++)
				{
					if (loV > scanline[k]) loV = scanline[k];
					if (loV <= 0) break;
				}
				m_data32[x + m_vurTbl[y]].red = loV;
			}
		}

		return err;
	}
#pragma endregion

#pragma region  scanRect
	A_LRect scanRect8()
	{
		A_LRect ret;
		ret.left = m_width - 1;
		ret.right = 0;
		ret.top = m_height-1;
		ret.bottom = 0;

		A_long p = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			if (ret.left > 0) {
				p = m_width - 1;
				for (A_long x = 0; x <= ret.left; x++)
				{
					if (m_data8[x + m_vurTbl[y]].red > 0)
					{
						p = x;
						break;
					}
				}
				if (ret.left > p) ret.left = p;
			}


			if (ret.right < m_width - 1) {
				p = 0;
				for (A_long x = m_width - 1; x >= ret.right; x--)
				{
					if (m_data8[x + m_vurTbl[y]].red > 0)
					{
						p = x;
						break;
					}
				}
				if (ret.right < p) ret.right = p;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			if (ret.top > 0) {
				p = m_height - 1;
				for (A_long y = 0; y <= ret.top; y++)
				{
					if (m_data8[x + m_vurTbl[y]].red > 0)
					{
						p = y;
						break;
					}
				}
				if (ret.top > p) ret.top = p;
			}
			if (ret.bottom < m_height - 1) {
				p = 0;
				for (A_long y = m_height - 1; y >= ret.bottom; y--)
				{
					if (m_data8[x + m_vurTbl[y]].red > 0)
					{
						p = y;
						break;
					}
				}
				if (ret.bottom < p) ret.bottom = p;
			}

		}
		return ret;
	}
	A_LRect scanRect16()
	{
		A_LRect ret;
		ret.left = m_width - 1;
		ret.right = 0;
		ret.top = m_height - 1;
		ret.bottom = 0;

		A_long p = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			if (ret.left > 0) {
				p = m_width - 1;
				for (A_long x = 0; x <= ret.left; x++)
				{
					if (m_data16[x + m_vurTbl[y]].red > 0)
					{
						p = x;
						break;
					}
				}
				if (ret.left > p) ret.left = p;
			}


			if (ret.right < m_width - 1) {
				p = 0;
				for (A_long x = m_width - 1; x >= ret.right; x--)
				{
					if (m_data16[x + m_vurTbl[y]].red > 0)
					{
						p = x;
						break;
					}
				}
				if (ret.right < p) ret.right = p;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			if (ret.top > 0) {
				p = m_height - 1;
				for (A_long y = 0; y <= ret.top; y++)
				{
					if (m_data16[x + m_vurTbl[y]].red > 0)
					{
						p = y;
						break;
					}
				}
				if (ret.top > p) ret.top = p;
			}
			if (ret.bottom < m_height - 1) {
				p = 0;
				for (A_long y = m_height - 1; y >= ret.bottom; y--)
				{
					if (m_data16[x + m_vurTbl[y]].red > 0)
					{
						p = y;
						break;
					}
				}
				if (ret.bottom < p) ret.bottom = p;
			}

		}
		return ret;
	}
	A_LRect scanRect32()
	{
		A_LRect ret;
		ret.left = m_width - 1;
		ret.right = 0;
		ret.top = m_height - 1;
		ret.bottom = 0;

		A_long p = 0;
		for (A_long y = 0; y < m_height; y++)
		{
			if (ret.left > 0) {
				p = m_width - 1;
				for (A_long x = 0; x <= ret.left; x++)
				{
					if (m_data32[x + m_vurTbl[y]].red > 0)
					{
						p = x;
						break;
					}
				}
				if (ret.left > p) ret.left = p;
			}


			if (ret.right < m_width - 1) {
				p = 0;
				for (A_long x = m_width - 1; x >= ret.right; x--)
				{
					if (m_data32[x + m_vurTbl[y]].red > 0)
					{
						p = x;
						break;
					}
				}
				if (ret.right < p) ret.right = p;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			if (ret.top > 0) {
				p = m_height - 1;
				for (A_long y = 0; y <= ret.top; y++)
				{
					if (m_data32[x + m_vurTbl[y]].red > 0)
					{
						p = y;
						break;
					}
				}
				if (ret.top > p) ret.top = p;
			}
			if (ret.bottom < m_height - 1) {
				p = 0;
				for (A_long y = m_height - 1; y >= ret.bottom; y--)
				{
					if (m_data32[x + m_vurTbl[y]].red > 0)
					{
						p = y;
						break;
					}
				}
				if (ret.bottom < p) ret.bottom = p;
			}

		}
		return ret;
	}

#pragma endregion


#pragma region Max
	PF_Err Max8(A_long v)
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_u_char* scanline = *(A_u_char**)m_scanlineH;

		for (A_long y = 0; y < m_height; y++)
		{
			A_long adrY = m_vurTbl[y];

			for (A_long x = 0; x < m_width; x++)
			{
				scanline[x] = m_data8[x + adrY].red;
			}
			for (A_long x = 0; x < m_width; x++)
			{
				A_long x0 = x - v;
				if (x0 < 0) x0 = 0;
				A_long x1 = x + v;
				if (x1 >= m_width) x1 = m_width - 1;

				A_u_char loV = 0;
				for (A_long k = x0; k <= x1; k++)
				{
					if (loV < scanline[k]) loV = scanline[k];
					if (loV >= PF_MAX_CHAN8) break;
				}
				m_data8[x + adrY].red = loV;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			for (A_long y = 0; y < m_height; y++)
			{
				scanline[y] = m_data8[x + m_vurTbl[y]].red;
			}
			for (A_long y = 0; y < m_height; y++)
			{
				A_long y0 = y - v;
				if (y0 < 0) y0 = 0;
				A_long y1 = y + v;
				if (y1 >= m_height) y1 = m_height - 1;

				A_u_char loV = 0;
				for (A_long k = y0; k <= y1; k++)
				{
					if (loV < scanline[k]) loV = scanline[k];
					if (loV >= PF_MAX_CHAN8) break;
				}
				m_data8[x + m_vurTbl[y]].red = loV;
			}
		}

		return err;
	}
	PF_Err Max16(A_long v)
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		A_u_short* scanline = *(A_u_short**)m_scanlineH;

		for (A_long y = 0; y < m_height; y++)
		{
			A_long adrY = m_vurTbl[y];

			for (A_long x = 0; x < m_width; x++)
			{
				scanline[x] = m_data16[x + adrY].red;
			}
			for (A_long x = 0; x < m_width; x++)
			{
				A_long x0 = x - v;
				if (x0 < 0) x0 = 0;
				A_long x1 = x + v;
				if (x1 >= m_width) x1 = m_width - 1;

				A_u_short loV = 0;
				for (A_long k = x0; k <= x1; k++)
				{
					if (loV < scanline[k]) loV = scanline[k];
					if (loV >= PF_MAX_CHAN16) break;
				}
				m_data16[x + adrY].red = loV;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			for (A_long y = 0; y < m_height; y++)
			{
				scanline[y] = m_data16[x + m_vurTbl[y]].red;
			}
			for (A_long y = 0; y < m_height; y++)
			{
				A_long y0 = y - v;
				if (y0 < 0) y0 = 0;
				A_long y1 = y + v;
				if (y1 >= m_height) y1 = m_height - 1;

				A_u_short loV = 0;
				for (A_long k = y0; k <= y1; k++)
				{
					if (loV < scanline[k]) loV = scanline[k];
					if (loV >= PF_MAX_CHAN16) break;
				}
				m_data16[x + m_vurTbl[y]].red = loV;
			}
		}

		return err;
	}
	PF_Err Max32(A_long v)
	{
		PF_Err err = PF_Err_NONE;
		if (m_data == NULL) return PF_Err_OUT_OF_MEMORY;

		PF_FpShort* scanline = *(PF_FpShort**)m_scanlineH;

		for (A_long y = 0; y < m_height; y++)
		{
			A_long adrY = m_vurTbl[y];

			for (A_long x = 0; x < m_width; x++)
			{
				scanline[x] = m_data32[x + adrY].red;
			}
			for (A_long x = 0; x < m_width; x++)
			{
				A_long x0 = x - v;
				if (x0 < 0) x0 = 0;
				A_long x1 = x + v;
				if (x1 >= m_width) x1 = m_width - 1;

				PF_FpShort loV = 0;
				for (A_long k = x0; k <= x1; k++)
				{
					if (loV < scanline[k]) loV = scanline[k];
					if (loV >= 1) break;
				}
				m_data32[x + adrY].red = loV;
			}
		}
		for (A_long x = 0; x < m_width; x++)
		{
			for (A_long y = 0; y < m_height; y++)
			{
				scanline[y] = m_data32[x + m_vurTbl[y]].red;
			}
			for (A_long y = 0; y < m_height; y++)
			{
				A_long y0 = y - v;
				if (y0 < 0) y0 = 0;
				A_long y1 = y + v;
				if (y1 >= m_height) y1 = m_height - 1;

				PF_FpShort loV = 0;
				for (A_long k = y0; k <= y1; k++)
				{
					if (loV < scanline[k]) loV = scanline[k];
					if (loV >= 1) break;
				}
				m_data32[x + m_vurTbl[y]].red = loV;
			}
		}

		return err;
	}

#pragma endregion


public:
	PF_Err GradMask()
	{
		PF_Err err = PF_Err_NONE;
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			err = GradMask32();
			break;
		case PF_PixelFormat_ARGB64:
			err = GradMask16();
			break;
		case PF_PixelFormat_ARGB32:
			err = GradMask8();
			break;
		}
		return err;
	}
	PF_Err Min(A_long cnt)
	{
		PF_Err err = PF_Err_NONE;
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			err = Min32(cnt);
			break;
		case PF_PixelFormat_ARGB64:
			err = Min16(cnt);
			break;
		case PF_PixelFormat_ARGB32:
			err = Min8(cnt);
			break;
		}
		return err;
	}
	PF_Err Max(A_long cnt)
	{
		PF_Err err = PF_Err_NONE;
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			err = Max32(cnt);
			break;
		case PF_PixelFormat_ARGB64:
			err = Max16(cnt);
			break;
		case PF_PixelFormat_ARGB32:
			err = Max8(cnt);
			break;
		}
		return err;
	}
	A_LRect scanRect()
	{
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			return scanRect32();
		case PF_PixelFormat_ARGB64:
			return scanRect16();
		case PF_PixelFormat_ARGB32:
		default:
			 return scanRect8();
		}
	}
};
#endif