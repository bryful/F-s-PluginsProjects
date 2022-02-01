#pragma once

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"

#ifndef PointInfo_H
#define PointInfo_H
typedef struct PointInfo {
	A_LPoint	p;
	PF_FpLong	s;

} PointInfo, * PointInfoP, ** PointInfoH;
#endif

class CPointInfo
{
	// **************************************************************
private:
	A_long m_Count = 0;
	A_long m_Size = 0;
	// **************************************************************
public:
	PointInfo* Points = NULL;
	static const A_long PointsSizeMax = 1024;
	PF_InData* in_data = NULL;
	A_long Count() {return m_Count;}
	A_long PointsSize() { return m_Size; }
	// **************************************************************
	CPointInfo()
	{
	}
	void SetParams(PF_InData* ind, PointInfoP buf, A_long sz = 1024, A_long cnt = 0)
	{
		in_data = ind;
		Points = buf;
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
		}
	}
	// **************************************************************
	inline void Push(PointInfo p)
	{
		if (m_Count >= m_Size-1) return;
		Points[m_Count] = p;
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

	}
		// **************************************************************
	inline void insert(A_long idx, PointInfo p)
	{
		if ((m_Size <= 0) || (m_Count <= 0)) return;
		if ((idx < 0)||(idx>= m_Size)) return;

		if (idx >= m_Count) {
			Points[m_Count] = p;
		}
		else {
			A_long st = m_Count;
			for (A_long i = st; i > idx; i--)
			{
				Points[i] = Points[i - 1];
			}
			Points[idx] = p;
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
	inline PointInfo Random(PointInfo s, A_long rx, A_long ry)
	{
		PointInfo ret = s;
		if (rx > 0)
		{
			ret.p.x += (A_long)(-(double)rx + (double)rx * 2 * xorShiftDouble());
		}
		if (ry > 0)
		{
			ret.p.y += (A_long)(-(double)ry + (double)ry * 2 * xorShiftDouble());
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
			A_long len = (A_long)((double)lineMove * xorShiftDouble());
			len = (A_long)((double)len + (double)len * 0.1 * (depth));

			PF_FpLong r = (360 * xorShiftDouble()) + RotOffset;
			if ((depth % 2) == 1) r = -r;
			A_long rr = (A_long)(r * (PF_FpLong)(1L<<16) + 0.5);
			rr %= (360L << 16);
			if (rr < 0) rr += (360L << 16);
			r = (PF_FpLong)rr / (PF_FpLong)(1L << 16);
			PF_FpLong sz = lineSize;
			sz *= (0.1 + 0.9 * xorShiftDouble());
			sz *= (1 - 0.1 * depth);
			if (sz < 1) sz = 1;
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
};

