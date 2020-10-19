#pragma once
#ifndef CDraw16_H
#define CDraw16_H

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"
#include "Params.h"




class CDraw16
{

protected:
private:
	A_long*				vurTbl;
	A_long				pntTblCount;
	PointInfo*			pntTblA;
	PointInfo*			pntTblB;

	PF_Handle			bufH = NULL;


public:
	PF_InData*			in_data;
	PF_EffectWorld*		world;
	PF_Pixel16*			 data;

	A_long				width;
	A_long				height;
	A_long				widthTrue;
	A_long				offsetWidth;


public:
	// **************************************************************
	CDraw16(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL)
	{
		//メンバの初期化
		in_data = NULL;
		world = NULL;
		data = NULL;
		width = 0;
		height = 0;
		widthTrue = 0;
		offsetWidth = 0;

		vurTbl = NULL;

		pntTblA = NULL;
		pntTblB = NULL;
		pntTblCount = 0;

		in_data = ind;
		world = wld;
		if (wld != NULL) {
			data = (PF_Pixel16*)wld->data;
			width = wld->width;
			height = wld->height;
			widthTrue = wld->rowbytes / sizeof(PF_Pixel16);
			offsetWidth = widthTrue - width;
		
		
			A_long bufSize = height * sizeof(A_long) + POINT_TABLE_SIZE * sizeof(PointInfo);
			bufSize *= 2;
			bufH = PF_NEW_HANDLE(bufSize);
			if (bufH) {
				vurTbl = *(A_long**)bufH;
				for (A_long i = 0; i < height; i++)
				{
					vurTbl[i] = i * widthTrue;
				}
				A_long* v = vurTbl + height + 10;
				pntTblA = (PointInfoP)v;
				pntTblB = (pntTblA + POINT_TABLE_SIZE + 10);
				for (A_long i = 0; i < POINT_TABLE_SIZE; i++)
				{
					pntTblA[i].p.x = 0;
					pntTblA[i].p.y = 0;
					pntTblA[i].s = -1;

					pntTblB[i].p.x = 0;
					pntTblB[i].p.y = 0;
					pntTblB[i].s = -1;
				}

			}
		}


	}
	// **************************************************************
	~CDraw16()
	{
		if (bufH != NULL)
		{
			PF_DISPOSE_HANDLE(bufH);
			bufH = NULL;
		}
	}
	// **************************************************************
	inline PF_Pixel16 GetPX(A_LPoint p){return data[p.x + vurTbl[p.y]];}
	inline void SetPX(A_LPoint p, PF_Pixel16 c) { data[p.x + vurTbl[p.y]] = c; }
	inline A_u_short GetPXR(A_LPoint p) { return data[p.x + vurTbl[p.y]].red; }
	inline void SetPXR(A_LPoint p, A_u_short c) { data[p.x + vurTbl[p.y]].red = c; }
	// **************************************************************
	inline void BlendPX(A_long x, A_long y, A_u_short v)
	{
		if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
		{
			A_long p = x + vurTbl[y];
			data[p].red = RoundShortFpLong((double)data[p].red * (double)v/ PF_MAX_CHAN16);
			data[p].green = RoundShortFpLong((double)data[p].green * (double)v / PF_MAX_CHAN16);
			data[p].blue = RoundShortFpLong((double)data[p].blue * (double)v / PF_MAX_CHAN16);

		}
	}	
	// **************************************************************
	void Xline(PF_FpLong x0, PF_FpLong x1, A_long y)
	{
		// xの2点の順を確認
		if (x0 > x1) {
			PF_FpLong temp = x0;
			x0 = x1;
			x1 = temp;
		}
		// 距離を獲得
		PF_FpLong dx = x1 - x0;
		// 距離が０ならおわり
		if (dx <= 0) { return; }
		
		// 書き込みの始点を求める
		A_long xi0 = (A_long)x0;
		// 始点の強さを求める
		PF_FpLong xv0 = 1.0 - (x0 - (PF_FpLong)xi0);
		// 終点を求める
		A_long xi1 = (A_long)x1;
		// 終点の強さを求める
		PF_FpLong xv1 = (x1 - (PF_FpLong)xi1);

		A_long dxi = xi1 - xi0;
		if (dxi==0) {
			BlendPX(xi0, y, (A_u_short)((1 - xv0)*PF_MAX_CHAN16));
		}
		else {
			BlendPX(xi0, y, (A_u_short)((1 - xv0) * PF_MAX_CHAN16));
			BlendPX(xi1, y, (A_u_short)((1 - xv1) * PF_MAX_CHAN16));
			if (dxi == 1) {
			}else if (dxi == 2) {
				BlendPX(xi0+1, y, 0);
			}else {
				for (A_long i = xi0 + 1; i <= xi1 - 1; i++)
				{
					BlendPX(i, y, 0);
				}
			}
		}
	}
	// **************************************************************
	void Yline(A_long x, PF_FpLong y0, PF_FpLong y1)
	{
		// xの2点の順を確認
		if (y0 > y1) {
			PF_FpLong temp = y0;
			y0 = y1;
			y1 = temp;
		}
		// 距離を獲得
		PF_FpLong dy = y1 - y0;
		// 距離が０ならおわり
		if (dy <= 0) { return; }

		// 書き込みの始点を求める
		A_long yi0 = (A_long)y0;
		// 始点の強さを求める
		PF_FpLong yv0 = 1.0 - (y0 - (PF_FpLong)yi0);
		// 終点を求める
		A_long yi1 = (A_long)y1;
		// 終点の強さを求める
		PF_FpLong yv1 = (y1 - (PF_FpLong)yi1);

		A_long dyi = yi1 - yi0;
		if (dyi == 0) {
			BlendPX(x, yi0, (A_u_short)((1 - yv0) * PF_MAX_CHAN16));
		}
		else {
			BlendPX(x, yi0, (A_u_short)((1 - yv0) * PF_MAX_CHAN16));
			BlendPX(x, yi1, (A_u_short)((1 - yv1) * PF_MAX_CHAN16));
			if (dyi == 1) {
			}
			else if (dyi == 2) {
				BlendPX(x, yi0+1, 0);
			}
			else {
				for (A_long i = yi0 + 1;i <= yi1 - 1; i++)
				{
					BlendPX(x, i, 0);
				}
			}
		}
	}
	
	// **************************************************************
	void Fill(PF_Pixel c)
	{
		A_long target = 0;

		PF_Pixel16 c2 = CONV8TO16(c);
		for (A_long y = 0; y < height; y++)
		{
			for (A_long x = 0; x < width; x++)
			{
				data[target] = c2;
				target++;
			}
			target += offsetWidth;
		}
	}
	// **************************************************************
	void Rev()
	{
		A_long target = 0;
		for (A_long y = 0; y < height; y++)
		{
			for (A_long x = 0; x < width; x++)
			{
				data[target].red = PF_MAX_CHAN16 - data[target].red;
				data[target].green = PF_MAX_CHAN16 - data[target].green;
				data[target].blue = PF_MAX_CHAN16 - data[target].blue;
				target++;
			}
			target += offsetWidth;
		}
	}
	// **************************************************************
	void Colorize(PF_Pixel c)
	{
		A_long target = 0;
		PF_Pixel16 c2 = CONV8TO16(c);
		for (A_long y = 0; y < height; y++)
		{
			for (A_long x = 0; x < width; x++)
			{
				A_u_short v = PF_MAX_CHAN16 - data[target].red;
				data[target] = c2;
				data[target].alpha = v;
				target++;
			}
			target += offsetWidth;
		}
	}
	// **************************************************************
	void Fill_EN(PointInfo p)
	{

		PF_FpLong r2 = p.s / 2;
		A_long ri = (A_long)(r2);


		for (A_long i = 0; i < ri; i++)
		{
			PF_FpLong xx = PF_SQRT(PF_POW(ri, 2) - PF_POW(i, 2));

			PF_FpLong x0 = p.p.x - xx;
			PF_FpLong x1 = p.p.x + xx;
			A_long y0 = p.p.y + i;
			A_long y1 = p.p.y - i;
			Xline(x0, x1, y0);
			Xline(x0, x1, y1);
		}


	}
	// **************************************************************
	void Line(PointInfo start,PointInfo last)
	{
		A_long dx = ABS(last.p.x - start.p.x);
		A_long dy = ABS(last.p.y - start.p.y);

		A_long gx = 1; if (last.p.x < start.p.x) gx = -1;
		A_long gy = 1; if (last.p.y < start.p.y) gy = -1;

		PF_FpLong sz = start.s / 2;
		PF_FpLong st = start.s / 2;
		PF_FpLong lt = last.s / 2;


		if (dx>=dy)
		{
			PF_FpLong ay = (PF_FpLong)dy / (PF_FpLong)dx;
			ay *= (PF_FpLong)gy;

			A_long x = start.p.x;
			PF_FpLong y = (PF_FpLong)start.p.y;
			
			//sz = sz / (PF_COS(PF_ATAN(dy / dx)));




			for (A_long i = 0; i < dx; i++)
			{
				sz = st + (lt - st)*i / dx;

				Yline(x, y-sz,y+sz);
				x += gx;
				y += ay;
			}
		}else {
			PF_FpLong ax = (PF_FpLong)dx / (PF_FpLong)dy;
			ax *= (PF_FpLong)gx;

			A_long y = start.p.y;
			PF_FpLong x = (PF_FpLong)start.p.x;

			//sz = sz / (PF_COS(PF_ATAN(dx / dy)));

			for (A_long i = 0; i < dy; i++)
			{
				sz = st + (lt - st) * i / dy;
				Xline(x-sz, x + sz, y);
				x += ax;
				y += gy;
			}

		}
	}
	
	// **************************************************************
	void CearPointTbl()
	{
		if (bufH == NULL) return;
		pntTblCount = 0;
		for (A_long i = 0; i < POINT_TABLE_SIZE; i++)
		{
			pntTblA[i].p.x = 0;
			pntTblA[i].p.y = 0;
			pntTblA[i].s = -1;
			pntTblB[i].p.x = 0;
			pntTblB[i].p.y = 0;
			pntTblB[i].s = -1;
		}
	}// **************************************************************
	void CopyPointTbl()
	{
		if (bufH == NULL) return;
		if (pntTblCount <= 0) return;
		for (A_long i = 0; i < pntTblCount; i++)
		{
			pntTblA[i] = pntTblB[i];
			pntTblB[i].p.x = 0;
			pntTblB[i].p.y = 0;
			pntTblB[i].s = -1;
		}
	}
	// **************************************************************
	void PushPoint(PointInfo pi)
	{
		pntTblA[pntTblCount] = pi;
		pntTblCount++;
	}
	// **************************************************************
	void CenterPos(ParamInfoP infoP,A_long depth)
	{
		if (bufH == NULL) return;

		A_long sIndex = 0;
		A_long dIndex = 0;

		if (pntTblCount < 2) return;

		A_long idx = 0;
		for (A_long i = 0; i < pntTblCount; i++)
		{

			PointInfo s = pntTblA[i];
			PointInfo d = pntTblA[i + 1];
			if ((d.s < 0)||(i== pntTblCount-1)) {
				pntTblB[idx] = s;
				idx++;
				break;
			}
			PointInfo c;
			c.p.x = (s.p.x + d.p.x) / 2;
			c.p.y = (s.p.y + d.p.y) / 2;
			A_long len = (A_long)((double)infoP->lineMove * xorShiftDouble());
			len = (A_long)((double)len + (double)len * 0.1 * (depth));
			PF_FpLong ar = 1;
			PF_FpLong r = (360 * xorShiftDouble()) + infoP->offset;
			r = PF_FMOD(r, 360);
			if ((depth % 2) == 1) r = 360 - r;
			PF_FpLong sz = infoP->lineSize;
			sz *= (0.2 + 0.3* xorShiftDouble());
			sz *= (1 - 0.01 * depth);
			if (sz < 1) sz = 1;
			A_LPoint p = PosFromRot(len, r,in_data);
			c.p.x += p.x;
			c.p.y += p.y;
			c.s = sz;
			pntTblB[idx] = s;
			idx++;
			pntTblB[idx] = c;
			idx++;
		}
		pntTblCount = idx;
		CopyPointTbl();
	}
	
	// **************************************************************
	void LineAll()
	{
		A_long idx = 0;
		for (A_long i = 0; i < pntTblCount; i++)
		{
			PointInfo s = pntTblA[i];
			Fill_EN(s);
			PointInfo d = pntTblA[i + 1];
			if (d.s < 0) 	break;
			Line(s, d);
		}
	}
	
	// **************************************************************
	void exec(CFsAE* ae, ParamInfoP infoP)
	{
		if (world == NULL) return;


		init_xorShift( infoP->seed);

		PF_Pixel c = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
		Fill(c);
		
		ParamInfo pi = *infoP;

		CearPointTbl();
		for (A_long dc = 0; dc < pi.drawCount; dc++)
		{
			pi = *infoP;
			//RotPoint(&pi, pi.rotRandom, in_data);

			PointInfo p0 = RandomPoint(pi.start, pi.startRandX, pi.startRandY);
			PointInfo p1 = RandomPoint(pi.last, pi.lastRandX, pi.lastRandY);
			PushPoint(p0);
			PushPoint(p1);

			A_long depth = 0;
			for (A_long fc = 0; fc < pi.foldCount; fc++)
			{
				CenterPos(&pi, depth);
				depth++;

			}
			LineAll();
			pi.lineSize = infoP->lineSize;
			pi.lineMove = infoP->lineMove;
			PointInfo p2 = p0;
			PointInfo p3 = p1;
			if (pntTblCount > 4)
			{
				p2 = pntTblA[1];
			}
			p3 = RandomPoint(p3, pi.lastRandX / 2, pi.lastRandY / 2);


			for (A_long sc = 0; sc < pi.subCount; sc++) {
				pi.lineSize *= 0.5;
				if (pi.lineSize < 1) pi.lineSize = 1;
				pi.lineMove = (A_long)((double)pi.lineMove * 1.2);

				CearPointTbl();
				PushPoint(p2);
				PushPoint(p3);
				A_long depth = 0;
				for (A_long fc = 0; fc < pi.foldCount; fc++)
				{
					CenterPos(&pi, depth);
					depth++;

				}
				LineAll();
			}
			CearPointTbl();
		}
		
		Colorize(infoP->color);

	}
};

#endif