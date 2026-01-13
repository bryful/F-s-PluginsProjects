#pragma once
#include "../FsLibrary/Fs.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

class RotPoint {
private:
	const A_FpLong M_PI = 3.14159265358979323846;
	A_FloatPoint m_startPosOrg;
	A_FloatPoint m_lastPosOrg;
	A_FloatPoint m_centerPosOrg;

	A_FloatPoint m_startPos2;
	A_FloatPoint m_lastPos2;
	A_FloatPoint m_centerPos2;

	A_FloatPoint m_startPos2D;
	A_FloatPoint m_lastPos2D;

	A_FpLong m_off_SetX;
	A_FpLong m_off_SetY;
	A_FpLong m_startPer;
	A_FpLong m_lastPer;
	A_FpLong m_rot;

	void Init()
	{
		m_startPos2.x = 0.0;
		m_startPos2.y = 0.0;
		m_lastPos2.x = 0.0;
		m_lastPos2.y = 0.0;
		m_centerPos2.x = 0.0;
		m_centerPos2.y = 0.0;

		m_startPosOrg.x = 0.0;
		m_startPosOrg.y = 0.0;
		m_lastPosOrg.x = 0.0;
		m_lastPosOrg.y = 0.0;
		m_centerPosOrg.x = 0.0;
		m_centerPosOrg.y = 0.0;

		m_off_SetX = 0.0;
		m_off_SetY = 0.0;

		m_startPos2D.x = 0.0;
		m_startPos2D.y = 0.0;
		m_lastPos2D.x = 0.0;
		m_lastPos2D.y = 0.0;

	};
	inline A_FloatPoint PointOnSegmentFraction(const A_FloatPoint& p1, const A_FloatPoint& p2, float frac)
	{
		//if (frac < 0.0f) frac = 0.0f;
		//if (frac > 1.0f) frac = 1.0f;
		A_FloatPoint out;
		// 精度を保つため double 演算を使用してから型を戻す
		out.x = static_cast<A_FpLong>(static_cast<double>(p1.x) + (static_cast<double>(p2.x) - static_cast<double>(p1.x)) * static_cast<double>(frac));
		out.y = static_cast<A_FpLong>(static_cast<double>(p1.y) + (static_cast<double>(p2.y) - static_cast<double>(p1.y)) * static_cast<double>(frac));
		return out;
	}
	inline A_FloatPoint gPos(A_FloatPoint p, A_long l)
	{
		p.x = p.x + sin((m_rot+90) * M_PI / 180) * l;
		p.y = p.y - cos((m_rot+90) * M_PI / 180) * l;
		return p;

	}
	void LineThroughCenterInRect(const PF_Rect& rct, A_FpLong angleDeg, A_FloatPoint& outA, A_FloatPoint& outB)
	{
		const double EPS = 1e-9;
		double left = static_cast<double>(rct.left);
		double top = static_cast<double>(rct.top);
		double right = static_cast<double>(rct.right);
		double bottom = static_cast<double>(rct.bottom);

		double cx = (left + right) * 0.5;
		double cy = (top + bottom) * 0.5;

		// After Effects の角度系に合わせた方向ベクトル
		// AE: 0 = 上(−Y)、増加は時計回り
		const double rad = static_cast<double>(angleDeg) * M_PI / 180.0;
		const double dirx = std::sin(rad);   // AE での X 成分
		const double diry = -std::cos(rad);  // AE の Y 下向きを考慮して符号反転

		// 交点候補収集
		std::vector<A_FloatPoint> pts;

		auto dmin = [](double a, double b) { return (a < b) ? a : b; };
		auto dmax = [](double a, double b) { return (a > b) ? a : b; };

		// 垂直辺
		if (std::abs(dirx) > EPS) {
			{
				double t = (left - cx) / dirx;
				double y = cy + diry * t;
				if (y >= dmin(top, bottom) - 1e-6 && y <= dmax(top, bottom) + 1e-6) {
					A_FloatPoint p; p.x = static_cast<A_FpLong>(left); p.y = static_cast<A_FpLong>(y);
					pts.push_back(p);
				}
			}
			{
				double t = (right - cx) / dirx;
				double y = cy + diry * t;
				if (y >= dmin(top, bottom) - 1e-6 && y <= dmax(top, bottom) + 1e-6) {
					A_FloatPoint p; p.x = static_cast<A_FpLong>(right); p.y = static_cast<A_FpLong>(y);
					pts.push_back(p);
				}
			}
		}

		// 水平辺
		if (std::abs(diry) > EPS) {
			{
				double t = (top - cy) / diry;
				double x = cx + dirx * t;
				if (x >= dmin(left, right) - 1e-6 && x <= dmax(left, right) + 1e-6) {
					A_FloatPoint p; p.x = static_cast<A_FpLong>(x); p.y = static_cast<A_FpLong>(top);
					pts.push_back(p);
				}
			}
			{
				double t = (bottom - cy) / diry;
				double x = cx + dirx * t;
				if (x >= dmin(left, right) - 1e-6 && x <= dmax(left, right) + 1e-6) {
					A_FloatPoint p; p.x = static_cast<A_FpLong>(x); p.y = static_cast<A_FpLong>(bottom);
					pts.push_back(p);
				}
			}
		}

		// 重複除去
		auto close = [](const A_FloatPoint& a, const A_FloatPoint& b) {
			const double tol = 1e-3;
			return (std::abs(static_cast<double>(a.x) - static_cast<double>(b.x)) < tol &&
				std::abs(static_cast<double>(a.y) - static_cast<double>(b.y)) < tol);
			};
		std::vector<A_FloatPoint> uniq;
		for (auto& p : pts) {
			bool found = false;
			for (auto& q : uniq) {
				if (close(p, q)) { found = true; break; }
			}
			if (!found) uniq.push_back(p);
		}

		// フォールバック
		if (uniq.size() < 2) {
			A_FloatPoint c; c.x = static_cast<A_FpLong>(cx); c.y = static_cast<A_FpLong>(cy);
			outA = c; outB = c;
			return;
		}

		// 中心からのベクトルと方向ベクトルの内積で前後を判定
		double bestForwardDot = -1e300;
		double bestBackwardDot = 1e300;
		size_t idxForward = 0, idxBackward = 1;
		bool haveForward = false, haveBackward = false;

		for (size_t i = 0; i < uniq.size(); ++i) {
			double vx = static_cast<double>(uniq[i].x) - cx;
			double vy = static_cast<double>(uniq[i].y) - cy;
			double dot = vx * dirx + vy * diry; // 正なら方向ベクトル側
			if (dot >= 0) {
				if (!haveForward || dot > bestForwardDot) {
					bestForwardDot = dot;
					idxForward = i;
					haveForward = true;
				}
			}
			else {
				if (!haveBackward || dot < bestBackwardDot) {
					bestBackwardDot = dot;
					idxBackward = i;
					haveBackward = true;
				}
			}
		}

		// 片側しか見つからない場合は距離で補う
		if (!haveForward || !haveBackward) {
			// 中心からの距離でソートして遠い2点を採る
			std::sort(uniq.begin(), uniq.end(), [&](const A_FloatPoint& a, const A_FloatPoint& b) {
				double da = (static_cast<double>(a.x) - cx) * (static_cast<double>(a.x) - cx) + (static_cast<double>(a.y) - cy) * (static_cast<double>(a.y) - cy);
				double db = (static_cast<double>(b.x) - cx) * (static_cast<double>(b.x) - cx) + (static_cast<double>(b.y) - cy) * (static_cast<double>(b.y) - cy);
				return da > db;
				});
			outA = uniq[0];
			outB = uniq[1];
			return;
		}

		outA = uniq[idxForward];
		outB = uniq[idxBackward];
	}
protected:
public:
	RotPoint() 
	{
		Init();
	};
	~RotPoint() {};
	
	A_FloatPoint startPos2() { return m_startPos2; };
	A_FloatPoint lastPos2() { return m_lastPos2; };
	A_FloatPoint centerPos2() { return m_centerPos2; };
	A_FloatPoint startPos2D() { return m_startPos2D; };
	A_FloatPoint lastPos2D() { return m_lastPos2D; };

	A_FloatPoint startPos2G(A_long l) { return gPos(m_startPos2, l); };
	A_FloatPoint lastPos2G(A_long l) { return gPos(m_lastPos2, l); };
	A_FloatPoint centerPos2G(A_long l) { return gPos(m_centerPos2, l); };
	A_FloatPoint startPos2DG(A_long l) { return gPos(m_startPos2D, l); };
	A_FloatPoint lastPos2DG(A_long l) { return gPos(m_lastPos2D, l); };

	void Set2Point(
		A_FloatPoint st, A_FloatPoint lt,
		A_FpLong stPar, A_FpLong ltPar,
		A_FpLong offX, A_FpLong offY
		)
	{
		m_startPosOrg = st;
		m_lastPosOrg = lt;
		m_centerPosOrg.x = (st.x + lt.x) / 2.0;
		m_centerPosOrg.y = (st.y + lt.y) / 2.0;
		m_startPer = stPar;
		m_lastPer = ltPar;
		m_off_SetX = offX;
		m_off_SetY = offY;

		m_startPos2.x = st.x + offX;
		m_startPos2.y = st.y + offY;
		m_lastPos2.x = lt.x + offX;
		m_lastPos2.y = lt.y + offY;
		m_centerPos2.x = (m_startPos2.x + m_lastPos2.x) / 2.0;
		m_centerPos2.y = (m_startPos2.y + m_lastPos2.y) / 2.0;

		m_startPos2D = PointOnSegmentFraction(m_startPos2, m_lastPos2, static_cast<float>(m_startPer));
		m_lastPos2D = PointOnSegmentFraction(m_lastPos2, m_startPos2,  static_cast<float>(m_lastPer));
	
		double dx = static_cast<double>(m_lastPos2.x - m_startPos2.x);
		double dy = static_cast<double>(m_lastPos2.y - m_startPos2.y);
		// AE の Y は下向き -> atan2 に渡す前に dy を反転
		double math_deg = std::atan2(-dy, dx) * 180.0 / M_PI;
		double ae_deg = 90.0 - math_deg+180;
		ae_deg = std::fmod(ae_deg, 360.0);
		if (ae_deg < 0.0) ae_deg += 360.0;
		m_rot = static_cast<A_FpLong>(ae_deg);
	};
	void SetRect(
		PF_Rect rct,
		A_FpLong angleDeg,
		A_FpLong stPar, A_FpLong ltPar,
		A_FpLong offX, A_FpLong offY
	)
	{
		LineThroughCenterInRect(rct, angleDeg, m_startPosOrg, m_lastPosOrg);

		m_rot = angleDeg;
		m_centerPosOrg.x = (m_startPosOrg.x + m_lastPosOrg.x) / 2.0;
		m_centerPosOrg.y = (m_startPosOrg.y + m_lastPosOrg.y) / 2.0;

		m_startPer = stPar;
		m_lastPer = ltPar;
		m_off_SetX = offX;
		m_off_SetY = offY;

		m_startPos2.x = m_startPosOrg.x + offX;
		m_startPos2.y = m_startPosOrg.y + offY;
		m_lastPos2.x = m_lastPosOrg.x + offX;
		m_lastPos2.y = m_lastPosOrg.y + offY;
		m_centerPos2.x = m_centerPosOrg.x + offX;
		m_centerPos2.y = m_centerPosOrg.y + offY;

		m_startPos2D = PointOnSegmentFraction(m_startPos2, m_lastPos2, static_cast<float>(m_startPer));
		m_lastPos2D = PointOnSegmentFraction(m_lastPos2, m_startPos2, static_cast<float>(m_lastPer));

	};

};