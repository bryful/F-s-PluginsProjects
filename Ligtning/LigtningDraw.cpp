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
std::vector<a_linePrm> CalcPoints(
	A_FloatPoint start, 
	A_FloatPoint end, 
	float weight, 
	int complexity, 
	float jaggedness, 
	float comple_angle,
	int fork,
	int seed
)
{
	std::vector<a_linePrm> result;
	result.push_back({ (float)start.x, (float)start.y, 0 });
	for(A_long fcnt=0; fcnt<fork; fcnt++)
	{
		for (int i = 0; i < complexity; i++)
		{
			float t = (float)(i + 1) / (float)(complexity + 1);
			float tt = 1 / (float)(complexity + 1);
			t += tt  - 2* hash_float(fcnt,i,seed)*tt;
			A_FloatPoint a = Lerp(start, end, t);

			float angle = a_tan(a,end);
			angle += 90 - 2* 180 * hash_float(fcnt, i + 100, seed);
			if (i % 2 == 0) {
				angle += comple_angle;
			}
			else {
				angle -= comple_angle;
			}
			float angleRad = (float)(angle * PF_PI / 180);
			float r = (float)(hash_float(fcnt, i + 200, seed) * jaggedness);
			if (hash_float(fcnt, i + 300, seed) < 0.1f) r *= 3;
			a.x += cosf(angleRad) * r;
			a.y += sinf(angleRad) * r;
			float b = weight * 0.1f + 0.9f * weight * hash_float(fcnt, i + 400, seed);
			result.push_back({ (float)a.x, (float)a.y, b });
		}
		result.push_back({ (float)end.x, (float)end.y, 0 });

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

	std::vector<std::vector<float>> buf(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;

	std::vector<a_linePrm> points = CalcPoints(
		{ (float)infoP->posTwin[0].x, (float)infoP->posTwin[0].y },
		{ (float)infoP->posTwin[1].x, (float)infoP->posTwin[1].y },
		(float)infoP->weight,
		infoP->complexity,
		(float)infoP->jaggedness,
		(float)infoP->comple_angle,
		infoP->fork,
		infoP->seedAct
	);
	A_long idx0 = (A_long)((points.size()-1) * hash_float(infoP->fork, infoP->complexity, infoP->seedAct));
	A_long idx1 = (A_long)((points.size() - 1) * hash_float(infoP->fork, infoP->complexity+100, infoP->seedAct));
	if (idx0 == idx1)
	{
		idx1 = (idx1 + 1) % points.size();
	}
	std::vector<a_linePrm> points2 = CalcPoints(
		{ (float)points[idx0].x, (float)points[idx0].y },
		{ (float)points[idx1].x, (float)points[idx1].y },
		(float)infoP->weight/2,
		infoP->complexity,
		(float)infoP->jaggedness/2,
		(float)infoP->comple_angle,
		1,
		infoP->seedAct
	);
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
	