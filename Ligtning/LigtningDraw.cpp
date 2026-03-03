#include "Ligtning.h"

// *****************************************************************************
A_FloatPoint Lerp(A_FloatPoint a, A_FloatPoint b, float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t
	};
}
// *****************************************************************************
float a_tan(A_FloatPoint sx, A_FloatPoint ex)
{
	return (float)(atan2f((float)(ex.y - sx.y), (float)(ex.x - sx.x))*180/PF_PI);
}
// *****************************************************************************
std::vector<a_linePrm> shiftPerPoints(const std::vector<a_linePrm>& points, float per)
{
	std::vector<a_linePrm> result;
	if (per <= 0.0f || per >= 2.0f) return result;
	if (per == 1.0f || points.size() < 2) return points;

	// ローカルコピーを作成することで const 制約を回避
	std::vector<a_linePrm> workPoints = points;

	if (per > 1.0f) {
		per = 2.0f - per;
		std::reverse(workPoints.begin(), workPoints.end());
	}

	A_long n = (A_long)workPoints.size();
	float shiftPer = 1.0f / (float)(n - 1);
	A_long shift = (A_long)(per / shiftPer);
	if (shift >= n - 1) return workPoints;

	float t = (per - shift * shiftPer) / shiftPer;

	for (A_long i = 0; i <= shift; i++)
	{
		result.push_back(workPoints[i]);
	}

	if (t > 0.0f)
	{
		A_FloatPoint ep = Lerp(
			{ workPoints[shift].x,     workPoints[shift].y },
			{ workPoints[shift + 1].x, workPoints[shift + 1].y },
			t
		);
		result.push_back({ (float)ep.x, (float)ep.y, workPoints[shift + 1].b * t });
	}
	return result;
}
// *****************************************************************************
std::vector<a_linePrm> CalcPoints(
	A_FloatPoint start, 
	A_FloatPoint end, 
	float startWeight,
	PF_Boolean startFlag,
	float endWeight,
	PF_Boolean endFlag,
	float weight,
	int complexity, 
	float jaggedness, 
	float comple_angle,
	int fork,
	int seed
)
{
	std::vector<a_linePrm> result;

	float length = (float)sqrtf((float)((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y)));
	float jag = length * jaggedness;
	float sw = startWeight * hash_float(complexity, 2000, seed);
	float ew = endWeight * hash_float(complexity, 1000, seed);
	for(A_long fcnt=0; fcnt<fork; fcnt++)
	{
		float tt = 1.0f  / (float)(complexity + 2);
		for (int i = 0; i < complexity+2; i++)
		{
			if (i == 0 && startFlag) {
				result.push_back({ (float)start.x, (float)start.y, sw });
				continue;
			}else if (i == complexity + 1 && endFlag) {
				result.push_back({ (float)end.x, (float)end.y, ew });
				continue;
			}
			float t = (float)(i) / (float)(complexity + 1);
			t += tt  - 2* hash_float(fcnt,i,seed)*tt;
			A_FloatPoint a = Lerp(start, end, t);

			float angle = a_tan(a,end);
			float angleAdd = 120 - 2* 240 * hash_float(fcnt, i + 100, seed);
			if (hash_float(fcnt, i + 150, seed) < 0.2f)
			{
				if (angleAdd < 5 && angleAdd > -5) angleAdd = (float)((angleAdd+0.5)*5);
				else if (angleAdd < 20 && angleAdd > -20) angleAdd *= 3;
				else if (angleAdd > 160 || angleAdd < -180) angleAdd *= 0.5f;
			}
			angle += angleAdd;
			if (i % 2 == 0) {
				angle += comple_angle;
			}
			else {
				angle -= comple_angle;
			}
			float angleRad = (float)(angle * PF_PI / 180);
			float r = (float)(hash_float(fcnt, i + 200, seed) * jag);
			if (hash_float(fcnt, i + 300, seed) < 0.15f) 
			{
				if (r < 10) r *= 5;
				else r *= 3;
			}
			a.x += cosf(angleRad) * r;
			a.y += sinf(angleRad) * r;
			float b = weight * 0.1f + 0.9f * weight * hash_float(fcnt, i + 400, seed);
			result.push_back({ (float)a.x, (float)a.y, b });
		}

	}
	return result;
}

// *****************************************************************************
// *****************************************************************************
PF_Err LigtningDraw2Point(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->wipe <= 0 || infoP->wipe >= 2.0) return err;
	std::vector<std::vector<float>> buf(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;

	std::vector<a_linePrm> points = CalcPoints(
		{ (float)infoP->posTwin[0].x, (float)infoP->posTwin[0].y },
		{ (float)infoP->posTwin[1].x, (float)infoP->posTwin[1].y },
		(float)infoP->startWeight,
		TRUE,
		(float)infoP->endWeight,
		TRUE,
		(float)infoP->weight,
		infoP->complexity,
		(float)infoP->jaggedness,
		(float)infoP->comple_angle,
		infoP->fork,
		infoP->seedAct
	);
	A_long idx0 = (A_long)((points.size()-1) * hash_float(infoP->fork, infoP->complexity, infoP->seedAct));
	A_long idx1 = (A_long)((points.size() - 1) * hash_float(infoP->fork, infoP->complexity+100, infoP->seedAct));
	if (idx0 > idx1) {
		A_long tmp = idx0;
		idx0 = idx1;
		idx1 = tmp;
	}
	
	if (idx0 == idx1)
	{
		idx1 = (idx1 + 1) % points.size();
	}
	std::vector<a_linePrm> points2 = CalcPoints(
		{ (float)points[idx0].x, (float)points[idx0].y },
		{ (float)points[idx1].x, (float)points[idx1].y },
		(float)infoP->startWeight/2,
		TRUE,
		(float)infoP->endWeight/2, 
		TRUE,
		(float)infoP->weight/2,
		infoP->complexity,
		(float)infoP->jaggedness/2,
		(float)infoP->comple_angle,
		1,
		infoP->seedAct
	);
	if (infoP->wipe != 1.0f) {
		points = shiftPerPoints(points, (float)infoP->wipe);
		points2 = shiftPerPoints(points2, (float)infoP->wipe);
	}
	draw_polyline(buf, offsetLinePrm(points,ox,oy),1.0f);
	draw_polyline(buf, offsetLinePrm(points2, ox, oy), 1.0f);

	ERR(DrawColorMask(
		ae->in_data,
		ae->output, 
		ae->pixelFormat(),
		ae->suitesP,
		&buf, 
		infoP->color));

	return err;
}
// *****************************************************************************
// *****************************************************************************
PF_Err LigtningDraw(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;

	if(infoP->auto_seed)
	{
		infoP->seedAct = infoP->seed + ae->frame();
	}

	switch (infoP->mode)
	{
		case MODE_2POINT:
		{
			ERR(LigtningDraw2Point(ae, infoP));
			break;
		}
	}

	return err;
}
	