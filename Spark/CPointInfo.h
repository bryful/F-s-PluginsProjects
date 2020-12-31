#pragma once

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"

#ifndef PointInfo_H
#define PointInfo_H
typedef struct PointInfo {
	A_LPoint	p;
	PF_FpLong	s;
	A_long		index;

} PointInfo, * PointInfoP, ** PointInfoH;
#endif

class CPointInfo
{
	// **************************************************************
private:
	A_long m_Count = 0;
	A_long m_CountBack = 0;
	A_long m_Size = 0;
	// **************************************************************
public:
	PointInfo* Points = NULL;
	PointInfo* PointsBak = NULL;
	static const A_long PointsSizeMax = 1024;
	PF_InData* in_data = NULL;
	A_long Count() {return m_Count;}
	A_long CountBak() { return m_CountBack; }
	A_long PointsSize() { return m_Size; }
	// **************************************************************
	CPointInfo()
	{
	}
	void SetParams(PF_InData* ind, PointInfoP buf, A_long sz = 1024, A_long cnt = 0)
	{
		in_data = ind;
		Points = buf;
		PointsBak = buf + sz + 100;
		m_Size = sz;
		m_Count = cnt;

	}
	// **************************************************************
	CPointInfo(PF_InData* ind, PointInfoP buf,  A_long sz=1024 ,A_long cnt=0)
	{
		SetParams(ind, buf, sz, cnt);
	}
	// **************************************************************
	inline void Clear()
	{
		if (m_Size <= 0) return;
		m_Count = 0;
		for (A_long i = 0; i < m_Size; i++)
		{
			Points[i].p.x = 0;
			Points[i].p.y = 0;
			Points[i].s = -1;
			Points[i].index = -1;
		}
	}
	// **************************************************************
	inline void Push(PointInfo p)
	{
		if (m_Count >= m_Size-1) return;
		Points[m_Count] = p;
		Points[m_Count].index = m_Count;
		m_Count++;
	}
	// **************************************************************
	inline PointInfo First(A_long idx=0)
	{
		return Points[idx];
	}
	inline PointInfo Last(A_long idx=0)
	{
		idx = m_Count - 1 - idx;
		return Points[idx];
	}
	// **************************************************************
	inline void swap(A_long idx0, A_long idx1)
	{
		if (idx0 == idx1) return;
		if (idx0 > idx1) {
			A_long tmp = idx0;
			idx0 = idx1;
			idx1 = tmp;
		}
		if ((idx0 >= m_Count) || (idx1 < 0)) return;

		PointInfo temp = Points[idx0];
		Points[idx0] = Points[idx1];
		Points[idx1] = temp;
		Points[idx0].index = idx0;
		Points[idx1].index = idx1;

	}
		// **************************************************************
	inline void insert(A_long idx, PointInfo p)
	{
		if ((m_Size <= 0) || (m_Count <= 0)) return;
		if ((idx < 0)||(idx>= m_Size)) return;

		if (idx >= m_Count) {
			Points[m_Count] = p;
			Points[m_Count].index = m_Count;
		}
		else {
			A_long st = m_Count;
			for (A_long i = st; i > idx; i--)
			{
				Points[i] = Points[i - 1];
				Points[i].index = i;
			}
			Points[idx] = p;
			Points[idx].index = idx;
		}
		m_Count++;
	}
	// **************************************************************
	inline PointInfo Center(PointInfo s, PointInfo d)
	{
		PointInfo ret;
		ret.p.x = (s.p.x + d.p.x) / 2;
		ret.p.y = (s.p.y + d.p.y) / 2;
		ret.s = (s.s + d.s) / 2;
		return ret;
	}
	// **************************************************************
	inline PointInfo ShiftFromRot(PointInfo s, PF_FpShort rot, A_long len)
	{
		PointInfo ret = s;
		if (len <= 0) return ret;
		A_long rr = (A_long)( (double)(1L<<16) * rot + 0.5);
		rr %= (360L <<16);
		if (rr < 0) rr += (360L << 16);
		rot = (PF_FpShort)rr / (PF_FpShort)(1L << 16);
		PF_FpShort r = (PF_FpShort)(rot * PF_PI / 180);
		ret.p.x += (A_long)((PF_FpShort)len * PF_COS(r) + 0.5);
		ret.p.y += (A_long)((PF_FpShort)len * PF_SIN(r) + 0.5);
		return ret;

	}
	// **************************************************************
	inline PointInfo EnFromRot(
		A_LPoint s, 
		PF_FpLong rot,
		A_long len,
		PF_FpLong aspect,
		PF_FpLong rot2
		)
	{
		PointInfo ret;
		ret.p.x = s.x;
		ret.p.y = s.y;

		if (len <= 0) return ret;
		A_long rr = (A_long)((double)(1L << 16) * rot + 0.5);
		rr %= (360L << 16);
		if (rr < 0) rr += (360L << 16);
		rot = (PF_FpShort)rr / (PF_FpShort)(1L << 16);

		PF_FpShort r = (PF_FpShort)(rot * PF_PI / 180);
		ret.p.x += (A_long)((PF_FpShort)len * PF_COS(r) + 0.5);
		ret.p.y += (A_long)((PF_FpShort)len * PF_SIN(r) * aspect + 0.5);

		if (rot2 != 0) {
			PF_FpLong dx = (PF_FpLong)ret.p.x - (PF_FpLong)s.x;
			PF_FpLong dy = (PF_FpLong)ret.p.y - (PF_FpLong)s.y;
			PF_FpLong len2 = (PF_FpLong)PF_HYPOT(dx, dy);
			PF_FpLong r2 = PF_ATAN2(dy, dx) * 180 / PF_PI;
			r2 = r2 + rot2;
			rr = (A_long)((double)(1L << 16) * r2 + 0.5);
			rr %= (360L << 16);
			if (rr < 0) rr += (360L << 16);
			r2 = (PF_FpShort)rr / (PF_FpShort)(1L << 16);

			r2 = (PF_FpShort)(r2 * PF_PI / 180);
			ret.p.x = s.x + (A_long)((PF_FpShort)len2 * PF_COS(r2) + 0.5);
			ret.p.y = s.y + (A_long)((PF_FpShort)len2 * PF_SIN(r2) + 0.5);
		}
		return ret;

	}
	// **************************************************************
	inline PointInfo Random(PointInfo s, A_long rx, A_long ry)
	{
		PointInfo ret = s;
		if (rx > 0)
		{
			ret.p.x += (A_long)(-(double)rx + (double)rx * 2 * xorShiftDouble());
		}
		else {
			xorShiftDouble();
		}
		if (ry > 0)
		{
			ret.p.y += (A_long)(-(double)ry + (double)ry * 2 * xorShiftDouble());
		}
		else {
			xorShiftDouble();
		}
		return ret;
	}
	// **************************************************************
	void CalcCenterPos(
		PF_FpLong lineSize,
		A_long lineMove,
		PF_FpLong RotOffset, 
		A_long depth)
	{
		if (m_Count<2) return;


		A_long idx = 0;
		A_long cnt = m_Count;
		for (A_long i = cnt-1; i >=1; i--)
		{

			PointInfo s = Points[i-1];
			PointInfo d = Points[i];

			PointInfo c;
			c.p.x = (s.p.x + d.p.x) / 2;
			c.p.y = (s.p.y + d.p.y) / 2;
			A_long len = (A_long)((double)lineMove * xorShiftMDouble());
			len = (A_long)((double)len + (double)len * 0.1 * (depth));

			PF_FpLong r = (360 * xorShiftMDouble()) + RotOffset;
			if ((depth % 2) == 1) r = -r;
			A_long rr = (A_long)(r * (PF_FpLong)(1L<<16) + 0.5);
			rr %= (360L << 16);
			if (rr < 0) rr += (360L << 16);
			r = (PF_FpLong)rr / (PF_FpLong)(1L << 16);
			PF_FpLong sz = lineSize;
			sz *= (0.1 + 0.9 * xorShiftMDouble());
			sz *= (1 - 0.1 * depth);
			//if (sz < 1) sz = 1;
			c = ShiftFromRot(c, (PF_FpShort)r, len);
			c.s = sz;
			insert(i, c);
		}
	}
	// **************************************************************
	void RemoveTail(A_long idx)
	{
		if ((m_Count <= 0)||(idx>=m_Count)) return;
		for (A_long i = idx; i < m_Count; i++)
		{
			Points[i].p.x = 0;
			Points[i].p.y = 0;
			Points[i].s = -1;
			Points[i].index = -1;
		}
		m_Count = idx;
	}
	// **************************************************************
	void CalcCenterPos2(
		PF_FpLong lineSize,
		A_long lineMove,
		PF_FpLong RotOffset,
		A_long depth,
		A_long start = 0
		)
	{
		if (m_Count-start < 1) return;

		A_long idx = 0;
		A_long cnt = m_Count;
		for (A_long i = cnt - 1; i >= start + 1; i--)
		{
			PointInfo s = Points[i - 1];
			PointInfo d = Points[i];

			PointInfo c;
			c.p.x = (s.p.x + d.p.x) / 2;
			c.p.y = (s.p.y + d.p.y) / 2;
			A_long len = (A_long)((double)lineMove * xorShiftMDouble());
			len = (A_long)((double)len + (double)len * 0.1 * (depth));

			PF_FpLong r = (360 * xorShiftMDouble()) + RotOffset;
			if ((depth % 2) == 1) r = -r;
			A_long rr = (A_long)(r * (PF_FpLong)(1L << 16) + 0.5);
			rr %= (360L << 16);
			if (rr < 0) rr += (360L << 16);
			r = (PF_FpLong)rr / (PF_FpLong)(1L << 16);
			PF_FpLong sz = lineSize;
			sz *= (0.1 + 0.9 * xorShiftMDouble());
			sz *= (1 - 0.1 * depth);
			//if (sz < 1) sz = 1;
			c = ShiftFromRot(c, (PF_FpShort)r, len);
			c.s = sz;
			insert(i, c);
		}
	}
	// **************************************************************
	PF_FpLong RotFramPoints(PointInfo s, PointInfo d)
	{
		PF_FpLong ret = 0;
		A_long dx = d.p.x - s.p.x;
		A_long dy = d.p.y - s.p.y;
		PF_FpLong r = PF_ATAN2(dx, dy);
		if (r < 0) r = r + 2 * PF_PI;
		return r * 180 / PF_PI;
	}
	// **************************************************************
	PointInfo CalcRotPos(PointInfo s, PointInfo d, PF_FpLong ar)
	{
		PF_FpLong ret = 0;
		A_long dx = d.p.x - s.p.x;
		A_long dy = d.p.y - s.p.y;
		//角度を求める
		PF_FpLong r = PF_ATAN2(dy ,dx);
		if (r < 0) r = r + 2 * PF_PI;
		r = r * 180 / PF_PI;

		r += ar;

		r = r * PF_PI / 180;

		PF_FpLong len = PF_SQRT(PF_POW(dx, 2) + PF_POW(dy, 2));
		A_long x = (A_long)(len * PF_COS(r)+0.5);
		A_long y = (A_long)(len * PF_SIN(r)+0.5);

		d.p.x = s.p.x + x;
		d.p.y = s.p.y + y;
		return d;

	}
	// **************************************************************
	void Wipe(PF_FpLong par)
	{
		if ((par == 1)||(m_Count==0)) return;
		//すべて消えてる状態
		if ((par <= 0) || (par >= 2))
		{
			Points[0].s = -1;
			Points[1].s = -1;
			return;
		}
		else {
			A_long c = m_Count - 1;
			if ((par > 0) && (par < 1)) {
				A_long idx = (A_long)((PF_FpLong)c * par);
				PF_FpLong idxP = (double)c * par - (PF_FpLong)idx;
				for (A_long i = 0; i < idx; i++)
				{
					//何もしない
				}
				Points[idx+1].s *= idxP;
				Points[idx + 2].s = -1;
			}
			else {
				A_long idx = (A_long)((PF_FpLong)c * (par-1));
				PF_FpLong idxP = (double)c * (par-1) - (PF_FpLong)idx;
				for (A_long i = 0; i < idx; i++)
				{
					Points[i].s = 0;
				}
				Points[idx].s *= (1-idxP);
			}

		}
	}
	// **************************************************************
	PointInfo PointInterRandom(PointInfo p0, PointInfo p1)
	{
		PointInfo ret;
		A_long dx = p1.p.x - p0.p.x;
		A_long dy = p1.p.y - p0.p.y;

		PF_FpLong len = PF_HYPOT(dx,dy);
		PF_FpLong r = PF_ATAN2(dy, dx);

		len = len * xorShiftMDouble();
		ret.p.x = p0.p.x + (A_long)(PF_COS(r) * len +0.5);
		ret.p.y = p0.p.y + (A_long)(PF_SIN(r) * len + 0.5);

		return ret;

	}
	// **************************************************************
	PointInfo PointInterRandomR(PointInfo p0, PointInfo p1)
	{
		PointInfo ret;
		A_long dx = p1.p.x - p0.p.x;
		A_long dy = p1.p.y - p0.p.y;

		PF_FpLong len = PF_HYPOT(dx, dy);
		PF_FpLong r = PF_ATAN2(dy, dx);

		len = len * xorShiftRDouble();
		ret.p.x = p0.p.x + (A_long)(PF_COS(r) * len + 0.5);
		ret.p.y = p0.p.y + (A_long)(PF_SIN(r) * len + 0.5);

		return ret;

	}
	// **************************************************************
	void  PointStrom(PointInfo* tbl, PointInfo *st, PointInfo* lt)
	{
		//スタート位置
		A_long sdx = tbl[1].p.x - tbl[0].p.x;
		A_long sdy = tbl[1].p.y - tbl[0].p.y;
		A_long ldx = tbl[3].p.x - tbl[2].p.x;
		A_long ldy = tbl[3].p.y - tbl[2].p.y;

		PF_FpLong lenStart = PF_HYPOT(sdx, sdy);
		PF_FpLong lenLast = PF_HYPOT(ldx, ldy);
		PF_FpLong sr = PF_ATAN2(sdy, sdx);
		PF_FpLong lr = PF_ATAN2(ldy, ldx);

		PF_FpLong rnd = xorShiftRDouble();

		lenStart = lenStart * rnd;
		st->p.x = tbl[0].p.x + (A_long)(PF_COS(sr) * lenStart + 0.5);
		st->p.y = tbl[0].p.y + (A_long)(PF_SIN(sr) * lenStart + 0.5);

		rnd += -0.1 + 0.2* xorShiftRDouble();
		if (rnd < 0) rnd = 0; else if (rnd > 1)rnd = 1;
		lenLast = lenLast * rnd;
		lt->p.x = tbl[2].p.x + (A_long)(PF_COS(lr) * lenLast + 0.5);
		lt->p.y = tbl[2].p.y + (A_long)(PF_SIN(lr) * lenLast + 0.5);

	}
	// **************************************************************
	PointInfo PointInter(PointInfo p0, PointInfo p1)
	{
		PointInfo ret;
		A_long dx = (p1.p.x - p0.p.x)/2;
		A_long dy = (p1.p.y - p0.p.y)/2;

		ret.p.x = p0.p.x + dx;
		ret.p.y = p0.p.y + dy;

		return ret;

	}
	// **************************************************************
	void Backup()
	{
		if (m_Size > 0)
		{
			for (int i = 0; i < m_Size; i++)
			{
				PointsBak[i] = Points[i];
			}
		}
		m_CountBack = m_Count;
	}
	// **************************************************************
};

