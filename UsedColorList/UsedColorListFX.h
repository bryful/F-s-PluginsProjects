#pragma once
#ifndef UsedColorListFX_H
#define UsedColorListFX_H

#include "../NFsLibrary/AE_SDK.h"
#include "../NFsLibrary/AEInfo.h"
#include "../NFsLibrary/NFsWorld.h"

#include "UsedColorList.h"

class UsedColorListFX :public NFsWorld
{
private:

public:
	UsedColorListFX(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	) : NFsWorld(wld, ind, fmt)
	{

	}
	PF_Pixel RevColor(PF_Pixel p)
	{
		PF_Pixel ret = { 0,0,0,0 };
		ret.alpha = PF_MAX_CHAN8;
		double op = (double)p.alpha / PF_MAX_CHAN8;
		ret.red = RoundByteDouble(PF_MAX_CHAN8 - p.red * op + 0.5);
		ret.green = RoundByteDouble(PF_MAX_CHAN8 - p.green * op + 0.5);
		ret.blue = RoundByteDouble(PF_MAX_CHAN8 - p.blue * op + 0.5);
		return ret;
	}

	void RectCheck(A_Rect* rct)
	{
		A_long x0 = rct->left;
		A_long x1 = rct->right;
		A_long y0 = rct->top;
		A_long y1 = rct->bottom;

		A_long r = 0;
		if (x0 > x1) {
			r = x0; x0 = x1; x1 = r;
		}
		if (y0 > y1) {
			r = y0; y0 = y1; y1 = r;
		}
		if (x0 < 0) x0 = 0;
		if (y0 < 0) y0 = 0;
		if (x1 > width()) x1 = width();
		if (y1 > height()) y1 = height();

		if (x1 - x0 < 4) x1 = x0 + 4;
		if (y1 - y0 < 4) y1 = y0 + 4;
		rct->left = x0;
		rct->right = x1;
		rct->top = y0;
		rct->bottom = y1;

	}

	PF_Err DrawRect(A_Rect rct)
	{
		PF_Err err = PF_Err_NONE;
		RectCheck(&rct);


		for (A_long x = rct.left; x < rct.right; x++)
		{
			A_long yy = rct.top;
			PF_Pixel c = GetPix8(x, yy);
			SetPix8(x, yy, RevColor(c));
			yy += 1;
			c = GetPix8(x, yy);
			SetPix8(x, yy, RevColor(c));
			yy = rct.bottom - 1;
			c = GetPix8(x, yy);
			SetPix8(x, yy, RevColor(c));
			yy -= 1;
			c = GetPix8(x, yy);
			SetPix8(x, yy, RevColor(c));
		}

		for (A_long y = rct.top+2; y < rct.bottom - 2; y++)
		{
			A_long xx = rct.left;
			PF_Pixel c = GetPix8(xx, y);
			SetPix8(xx, y, RevColor(c));
			xx += 1;
			c = GetPix8(xx, y);
			SetPix8(xx, y, RevColor(c));
			xx = rct.right - 1;
			c = GetPix8(xx, y);
			SetPix8(xx, y, RevColor(c));
			xx -= 1;
			c = GetPix8(xx, y);
			SetPix8(xx, y, RevColor(c));
		}
		return err;

	}
	void  DrawGrid(PF_Pixel col,A_Rect rct)
	{
		RectCheck(&rct);
		for (A_long j = rct.top; j < rct.bottom; j++)
			for (A_long i = rct.left; i < rct.right; i++)
				SetPix8(i, j, col);

	}
	PF_Err DrawList(ParamInfo* infoP)
	{
		PF_Err			err = PF_Err_NONE;
		if (infoP->ColorCount <= 0) return err;

		int sx = width() / infoP->GridWidth;
		int sy = height() / infoP->GridHeight;


		for (A_long i = 0; i < infoP->ColorCount; i++)
		{
			A_Rect r = { 0,0,0,0 };
			r.left = (i / sx) * infoP->GridWidth;
			r.top = (i % sx) * infoP->GridHeight;
			r.right = r.left + infoP->GridWidth;
			r.bottom = r.top + infoP->GridHeight;

			DrawGrid(infoP->Colors[i], r);
		}


		return err;
	}
	A_long IndexOfColor(PF_Pixel* tbl, A_long tblMax,PF_Pixel c)
	{
		A_long ret = -1;
		if (tblMax <= 0) return ret;

		for (A_long i = 0; i < tblMax; i++)
		{
			if ((tbl[i].red == c.red) && (tbl[i].green == c.green) && (tbl[i].blue == c.blue))
			{
				ret = i;
				break;
			}
		}
		return ret;
	}
	void bubbleSort(PF_Pixel *p,A_long *s, A_long array_num) 
	{
		for (A_long i = 0; i < array_num - 1; i++) {
			//右からソート（左からやると変数が1つ増える）
			for (A_long j = array_num - 1; i < j; j--) {

				if (s[j] < s[j - 1]) { //左の数値が大きければスワップ（交換）
					//スワップ
					PF_Pixel tc = p[j];
					p[j] = p[j - 1];
					p[j - 1] = tc;
					A_long v = s[j];
					s[j] = s[j - 1];
					s[j - 1] = v;
				}
			}
		}
	}
	PF_Err ColorScan(ParamInfo* infoP)
	{
		PF_Err			err = PF_Err_NONE;

		RectCheck(&(infoP->Area));
		infoP->ColorCount = 0;
		for (A_long i = 0; i < 64; i++)
		{
			infoP->ColorsMax[i] = 0;
		}

		int cnt = 0;
		for (A_long y = infoP->Area.top; y < infoP->Area.bottom; y++)
		{
			for (A_long x = infoP->Area.left; x < infoP->Area.right; x++)
			{
				if (cnt >= 64) break;
				PF_Pixel c = GetPix8(x, y);
				if (c.alpha != PF_MAX_CHAN8) continue;
				if ((c.red == 0) && (c.blue == 0) && (c.green == 0)) continue;
				if ((c.red == PF_MAX_CHAN8) && (c.blue == PF_MAX_CHAN8) && (c.green == PF_MAX_CHAN8)) continue;

				if ((c.red == infoP->ExceptColor0.red) 
					&& (c.blue == infoP->ExceptColor0.blue) 
					&& (c.green == infoP->ExceptColor0.green)) continue;
				if ((c.red == infoP->ExceptColor1.red)
					&& (c.blue == infoP->ExceptColor1.blue)
					&& (c.green == infoP->ExceptColor1.green)) continue;
				if ((c.red == infoP->ExceptColor2.red)
					&& (c.blue == infoP->ExceptColor2.blue)
					&& (c.green == infoP->ExceptColor2.green)) continue;


				if (cnt == 0) {
					infoP->Colors[0] = c;
					infoP->ColorsMax[0] = 1;
					cnt = 1;
				}
				else {
					int idx = IndexOfColor(infoP->Colors, cnt, c);
					if (idx <0)
					{
						infoP->Colors[cnt] = c;
						infoP->ColorsMax[cnt] = 1;
						cnt++;
					}
					else {
						infoP->ColorsMax[idx]++;
					}
				}
			}
			if (cnt >= 64) break;
		}
		if (cnt > 0)
		{
			infoP->ColorCount = cnt;
			bubbleSort(infoP->Colors, infoP->ColorsMax, infoP->ColorCount);

			int cnt = infoP->ColorCount / 2;
			for (A_long i = 0; i < cnt; i++)
			{
				A_long ii = infoP->ColorCount - i - 1;
				PF_Pixel tc = infoP->Colors[i];
				infoP->Colors[i] = infoP->Colors[ii];
				infoP->Colors[ii] = tc;
				A_long v = infoP->ColorsMax[i];
				infoP->ColorsMax[i] = infoP->ColorsMax[ii];
				infoP->ColorsMax[ii] = v;
			}

		}

		return err;
	}
};

#endif