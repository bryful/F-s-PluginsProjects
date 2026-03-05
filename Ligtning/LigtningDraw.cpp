#include "Ligtning.h"

// *****************************************************************************
inline A_FloatPoint Lerp(A_FloatPoint a, A_FloatPoint b, float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t
	};
}
// *****************************************************************************
inline float a_tan(A_FloatPoint sx, A_FloatPoint ex)
{
	return (float)(atan2f((float)(ex.y - sx.y), (float)(ex.x - sx.x))*180/PF_PI);
}
// *****************************************************************************
inline std::vector<a_linePrm> shiftPerPoints(const std::vector<a_linePrm>& points, float per)
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
inline float pntlength(
	double x0,
	double y0,
	double x1,
	double y1,
	double jaggedness
	)
{
	double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
	return (float)(length * jaggedness * 0.1f);
}
// *****************************************************************************
inline a_linePrm randPos(
	A_FloatPoint start,
	A_FloatPoint end,
	float len,
	float rot,A_long  seed1, A_long  seed2, A_long  seed3 )
{
	float angle = a_tan(start, end);
	angle = (float)(angle + 90 - 2 * 180 * hash_float(seed1, seed2, seed3) +rot);
	float angleRad = (float)(angle * PF_PI / 180);
	float radius = (float)(hash_float(seed1 + 500, seed2 + 200, seed3) * len);
	if (hash_float(seed1 + 600, seed2, seed3) < 0.10f)
	{
		if (radius < 10) radius *= 5;
		else radius *= 3;
	}
	a_linePrm a = { (float)start.x, (float)start.y, 0 };
	a.x += cosf(angleRad) * radius;
	a.y += sinf(angleRad) * radius;
	return a;
}
// *****************************************************************************
std::vector<a_linePrm> CalcPoints(
	A_FloatPoint start,
	A_FloatPoint end,
	float startWeight,
	float weight,
	int complexity,
	float jaggedness,
	float comple_angle,
	A_long fork,
	int seed
)
{
	std::vector<a_linePrm> result;


	float jag = pntlength(start.x,start.y,end.x,end.y,jaggedness);
	float sw = startWeight * hash_float(fork, 2000, seed);
	float tt = 1.0f / (float)(complexity + 2);
	for (int i = 0; i < complexity + 1; i++)
	{
		if (i == 0) {
			result.push_back({ (float)start.x, (float)start.y, sw });
			continue;
		}
		float t = (float)(i) / (float)(complexity + 2);
		t += tt - 2 * hash_float(fork + 200, i, seed) * tt;
		A_FloatPoint a = Lerp(start, end, t);


		a_linePrm pp = randPos(
			a,
			end,
			jag,
			(i%2==1) ? comple_angle : -comple_angle,
			fork,i, seed
		);
		
		pp.b = weight * 0.1f + 0.9f * weight * hash_float(fork * 10 + 800, i + 400, seed);
		result.push_back(pp);
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

	for (int fc = 0; fc < infoP->fork; fc++)
	{
		std::vector<a_linePrm> points = CalcPoints(
			{ (float)infoP->posTwin[0].x, (float)infoP->posTwin[0].y },
			{ (float)infoP->posTwin[1].x, (float)infoP->posTwin[1].y },
			(float)infoP->startWeight,
			(float)infoP->weight,
			infoP->complexity,
			(float)infoP->jaggedness,
			(float)infoP->comple_angle,
			fc,
			infoP->seedAct
		);
		float endWeight = (float)infoP->endWeight*hash_float(fc,987, infoP->seedAct);
		points.push_back({ (float)infoP->posTwin[1].x, (float)infoP->posTwin[1].y, (float)endWeight });

		A_long idx0 = (A_long)((points.size() - 1) * hash_float(fc, infoP->complexity, infoP->seedAct));
		A_long idx1 = (A_long)((points.size() - 1) * hash_float(fc, infoP->complexity + 100, infoP->seedAct));
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
			(float)infoP->startWeight / 2,
			(float)infoP->weight / 2,
			infoP->complexity,
			(float)infoP->jaggedness / 2,
			(float)infoP->comple_angle,
			fc,
			infoP->seedAct+100
		);
		endWeight = (float)infoP->weight * hash_float(fc, 412, infoP->seedAct);
		points2.push_back({ (float)points[idx1].x, (float)(float)points[idx1].y, (float)endWeight });

		if (idx1 < points.size() - 1) {
			a_linePrm lastP = points[points.size() - 1];
			lastP.b = 0;
			for (int i = idx1; i < points.size(); i++)
			{
				points2.push_back(lastP);
			}
		}
		if (idx0 > 0) {
			a_linePrm firstP = points[0];
			firstP.b = 0;
			for (int i = 0; i < idx0; i++)
			{
				points2.insert(points2.begin(), firstP);
			}
		}
		
		if (infoP->wipe != 1.0f) {
			points = shiftPerPoints(points, (float)infoP->wipe);
			points2 = shiftPerPoints(points2, (float)infoP->wipe);
		}
		draw_polyline(buf, offsetLinePrm(points, ox, oy), 1.0f);
		draw_polyline(buf, offsetLinePrm(points2, ox, oy), 1.0f);
	}
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
PF_Err LigtningDraw1_2Point(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->wipe <= 0 || infoP->wipe >= 2.0) return err;
	std::vector<std::vector<float>> buf(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;
	float impactRand = (float)ae->downScale(infoP->impactRand);

	for (int fc = 0; fc < infoP->fork; fc++)
	{
		std::vector< std::vector<a_linePrm>> lines;
		A_FloatPoint sp = { (float)infoP->posImpact[0].x,(float)infoP->posImpact[0].y };
		float et = 0.5f + (0.1f - 0.2f *hash_float(fc, 1300, infoP->seedAct));
		A_FloatPoint ep = Lerp({ (float)infoP->posImpact[1].x,(float)infoP->posImpact[1].y }, 
			{ (float)infoP->posImpact[2].x,(float)infoP->posImpact[2].y }, et);

		std::vector<a_linePrm> points = CalcPoints(
			{ (float)sp.x, (float)sp.y },
			{ (float)ep.x, (float)ep.y },
			(float)infoP->startWeight,
			(float)infoP->weight,
			infoP->complexity,
			(float)infoP->jaggedness,
			(float)infoP->comple_angle,
			fc,
			infoP->seedAct
		);
		float endWeight = (float)infoP->endWeight * hash_float(fc, 987, infoP->seedAct);
		points.push_back({ (float)ep.x, (float)ep.y, (float)endWeight });
		lines.push_back(points);
		
		if (lines[0].size() > 3) {
			for (int j = 0; j < infoP->impactBranch; j++)
			{
				int tl = (int)AE_CLAMP( (lines.size()) *( 1-2 * hash_float(fc, j+189, infoP->seedAct)),0, lines.size()-1);
				int idx0 = -1;
				int cnt = 0;
				while (idx0 < 0)
				{
					int i0 = (A_long)((lines[tl].size() - 1) * hash_float(fc, j + 120+cnt, infoP->seedAct));
					if (lines[tl][i0].b > 0) {
						idx0 = i0;
						break;
					}
					cnt++;
					if (cnt > lines[tl].size()*3) break;
				}
				int cc = (int)lines[tl].size()-idx0+1;
				if (cc < 3|| idx0<0) continue;
				A_FloatPoint sp = { (float)lines[tl][idx0].x,(float)lines[tl][idx0].y };
				A_FloatPoint ep = Lerp({ (float)infoP->posImpact[1].x,(float)infoP->posImpact[1].y },
					{ (float)infoP->posImpact[2].x,(float)infoP->posImpact[2].y }, hash_float(fc, 130+j, infoP->seedAct));
				ep.x += (float)(hash_float(fc, j + 230, infoP->seedAct) - 0.5f) * impactRand;
				ep.y += (float)(hash_float(fc, j + 530, infoP->seedAct) - 0.5f) * impactRand;
				std::vector<a_linePrm> points2 = CalcPoints(
					{ (float)sp.x, (float)sp.y },
					{ (float)ep.x, (float)ep.y },
					(float)infoP->weight,
					(float)infoP->weight,
					cc,
					(float)infoP->jaggedness,
					(float)infoP->comple_angle,
					fc,
					infoP->seedAct+ j + idx0
				);
				if (idx0 > 0) {
					a_linePrm firstP = points2[0];
					firstP.b = 0;
					for (int i = 0; i < idx0; i++)
					{
						points2.insert(points2.begin(), firstP);
					}
				}
				endWeight = (float)infoP->endWeight * hash_float(fc, j+412, infoP->seedAct);
				points2.push_back({ (float)ep.x, (float)ep.y, (float)endWeight });
				lines.push_back(points2);
			}
		}


		if (infoP->wipe != 1.0f) {
			if (lines.size() > 0 )
			{
				for(int j=0;j<lines.size();j++)
				{
					lines[j] = shiftPerPoints(lines[j], (float)infoP->wipe);
				}
			}
		}
		if  (lines.size() > 0)
		{
			for (int j = 0; j < lines.size(); j++)
			{
				
				draw_polyline(buf, offsetLinePrm(lines[j], ox, oy), 1.0f);
			}
		}
	}
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
PF_Err LigtningDrawRadical(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->wipe <= 0 || infoP->wipe >= 2.0) return err;
	std::vector<std::vector<float>> buf(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;
	float radiusOrg = (float)ae->downScale(infoP->radius);
	float radius = radiusOrg;
	for (int rc = 0; rc < infoP->radicalCount; rc++)
	{
		float angle = 360.0f * hash_float(985, rc + 800, infoP->seed);
		for (int fc = 0; fc < infoP->fork; fc++)
		{
			std::vector< std::vector<a_linePrm>> lines;
			A_FloatPoint sp = { (float)infoP->center.x,(float)infoP->center.y };

			if (infoP->radiusRand > 0)
			{
				radius = radiusOrg * (1.0f - (float)infoP->radiusRand) + radiusOrg * (float)infoP->radiusRand * hash_float(fc, rc + 500, infoP->seedAct);
			}
			float angle2 = angle + ( 0.5f -hash_float(fc+987, rc + 800, infoP->seed+2))*25.0f;
			angle2 = angle2 + (0.5f - hash_float(fc + 1987, rc + 400, infoP->seedAct)) * 5.0f;
			(rc % 2 == 0) ? angle2 += (float)infoP->radicalAngle +(float)ae->frame()/2 : (angle2 -= (float)infoP->radicalAngle + (float)ae->frame()/2);
			float angleRad = (float)(angle2 * PF_PI / 180);
			A_FloatPoint ep = {
				sp.x + cosf(angleRad) * radius,
				sp.y + sinf(angleRad) * radius
			};

			std::vector<a_linePrm> points = CalcPoints(
				{ (float)sp.x, (float)sp.y },
				{ (float)ep.x, (float)ep.y },
				(float)infoP->startWeight,
				(float)infoP->weight,
				infoP->complexity,
				(float)infoP->jaggedness,
				(float)infoP->comple_angle,
				fc,
				infoP->seedAct + rc + fc
			);
			float endWeight = (float)infoP->endWeight * hash_float(fc, rc+987, infoP->seedAct);
			points.push_back({ (float)ep.x, (float)ep.y, (float)endWeight });
			lines.push_back(points);

			int ccc = infoP->complexity / 2;
			if (lines[0].size() > 3&& ccc>0) {
				for (int j = 0; j < ccc; j++)
				{
					int tl = (int)AE_CLAMP((lines.size()-1) * (hash_float(fc+rc, j + 189, infoP->seedAct)), 0, lines.size() - 1);
					int idx0 = -1;
					int cnt = 0;
					while (idx0 < 0)
					{
						int i0 = (A_long)( (lines[tl].size() - 1) * hash_float(fc+rc+2 ,j + 120 + cnt, infoP->seedAct));
						if (lines[tl][i0].b > 0) {
							idx0 = i0;
							break;
						}
						cnt++;
						if (cnt > lines[tl].size() * 3) break;
					}
					int cc = (int)lines[tl].size() - idx0 + 1;
					if (cc < 3 || idx0 < 0) continue;
					A_FloatPoint sp2 = { (float)lines[tl][idx0].x,(float)lines[tl][idx0].y };

					angle2 = angle +35.0f * (0.5f -  hash_float(fc+23, rc + 1800, infoP->seed+100));
					angle2 = angle2 + 5.0f * (1.0f - 2.0f * hash_float(fc + 123, rc + 180, infoP->seedAct));
					(rc % 2 == 0) ? angle2 += (float)infoP->radicalAngle + (float)ae->frame()/2 : (angle2 -= (float)infoP->radicalAngle + (float)ae->frame()/2);
					float angleRad = (float)(angle2 * PF_PI / 180);
					if (infoP->radiusRand > 0)
					{
						radius = radiusOrg * (1.0f - (float)infoP->radiusRand) + radiusOrg * (float)infoP->radiusRand * hash_float(fc, rc + 1500+j, infoP->seedAct);
					}
					A_FloatPoint ep2 = {
						sp.x + cosf(angleRad) * radius,
						sp.y + sinf(angleRad) * radius
					};
					std::vector<a_linePrm> points3 = CalcPoints(
						{ (float)sp2.x, (float)sp2.y },
						{ (float)ep2.x, (float)ep2.y },
						(float)infoP->weight,
						(float)infoP->weight,
						cc,
						(float)infoP->jaggedness,
						(float)infoP->comple_angle,
						fc,
						infoP->seedAct + j + idx0 + fc + rc + 20
					);
					if (idx0 > 0) {
						a_linePrm firstP = points3[0];
						firstP.b = 0;
						for (int i = 0; i < idx0; i++)
						{
							points3.insert(points3.begin(), firstP);
						}
					}
					endWeight = (float)infoP->endWeight * hash_float(fc, j + 412, infoP->seedAct);
					points3.push_back({ (float)ep2.x, (float)ep2.y, (float)endWeight });
					lines.push_back(points3);
				}
			}


			if (infoP->wipe != 1.0f) {
				if (lines.size() > 0)
				{
					for (int j = 0; j < lines.size(); j++)
					{
						lines[j] = shiftPerPoints(lines[j], (float)infoP->wipe);
					}
				}
			}
			if (lines.size() > 0)
			{
				for (int j = 0; j < lines.size(); j++)
				{

					draw_polyline(buf, offsetLinePrm(lines[j], ox, oy), 1.0f);
				}
			}
		}
	}
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
PF_Err LigtningDraw2_2Point(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->wipe <= 0 || infoP->wipe >= 2.0) return err;
	std::vector<std::vector<float>> buf(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;

	for (int fc = 0; fc < infoP->fork; fc++)
	{
		A_long cpt = 32768 + ( (int)( (1 - 2*hash_float(fc, 300, 650)) * 32768));
		A_long spt = AE_CLAMP(cpt + (A_long)(8192 - 2* 8192 * hash_float(fc, 300, infoP->seedAct)), 0, 65535);
		A_long ept = AE_CLAMP(cpt + (A_long)(8192 - 2* 8192 * hash_float(fc, 400, infoP->seedAct)), 0, 65535);

		//A_long spt = ((int)(hash_float(fc, 300, infoP->seedAct) * 65536) % 65536);
		//A_long ept = ((int)(hash_float(fc, 700, infoP->seedAct) * 65536) % 65536);

		A_FloatPoint sp = Lerp(
			{ (float)infoP->posStart[0].x, (float)infoP->posStart[0].y },
			{ (float)infoP->posStart[1].x, (float)infoP->posStart[1].y },
			(float)spt / 65536.0f
		);
		A_FloatPoint ep = Lerp(
			{ (float)infoP->posEnd[0].x, (float)infoP->posEnd[0].y },
			{ (float)infoP->posEnd[1].x, (float)infoP->posEnd[1].y },
			(float)ept / 65536.0f
		);
		std::vector<a_linePrm> points = CalcPoints(
			sp,
			ep,
			(float)infoP->startWeight,
			(float)infoP->weight,
			infoP->complexity,
			(float)infoP->jaggedness,
			(float)infoP->comple_angle,
			fc,
			infoP->seedAct+fc
		);
		float endWeight = (float)infoP->endWeight * hash_float(fc, 987, infoP->seedAct);
		points.push_back({ (float)ep.x, (float)ep.y, (float)endWeight });

		A_long idx0 = (A_long)((points.size() - 1) * hash_float(fc, infoP->complexity, infoP->seedAct));
		A_long idx1 = (A_long)((points.size() - 1) * hash_float(fc, infoP->complexity + 100, infoP->seedAct));
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
			(float)infoP->startWeight / 2,
			(float)infoP->weight / 2,
			infoP->complexity,
			(float)infoP->jaggedness / 2,
			(float)infoP->comple_angle,
			fc,
			infoP->seedAct+100+fc
		);
		endWeight = (float)infoP->weight * hash_float(fc, 412, infoP->seedAct);
		points2.push_back({ (float)points[idx1].x, (float)(float)points[idx1].y, (float)endWeight });

		if (idx1 < points.size() - 1) {
			a_linePrm lastP = points[points.size() - 1];
			lastP.b = 0;
			for (int i = idx1; i < points.size(); i++)
			{
				points2.push_back(lastP);
			}
		}
		if (idx0 > 0) {
			a_linePrm firstP = points[0];
			firstP.b = 0;
			for (int i = 0; i < idx0; i++)
			{
				points2.insert(points2.begin(), firstP);
			}
		}

		if (infoP->wipe != 1.0f) {
			points = shiftPerPoints(points, (float)infoP->wipe);
			points2 = shiftPerPoints(points2, (float)infoP->wipe);
		}
		draw_polyline(buf, offsetLinePrm(points, ox, oy), 1.0f);
		draw_polyline(buf, offsetLinePrm(points2, ox, oy), 1.0f);
	}
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
PF_Err LigtningDrawPath(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->wipe <= 0 || infoP->wipe >= 2.0) return err;
	std::vector<std::vector<float>> buf(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;

	std::vector< PF_PathVertex> paths;
	PF_Boolean isOpened=FALSE;
	ERR(ae->GetPathFromUI(ID_PATH,&paths,&isOpened));
	if (paths.size() < 2) return err;

	std::vector<a_linePrm> basep;
	float jag = pntlength(
		paths[0].x,
		paths[0].y,
		paths[1].x,
		paths[1].y,
		infoP->jaggedness);
	if (isOpened)
	{
		a_linePrm firstP = { (float)paths[0].x,(float)paths[0].y,0.0f };
		basep.push_back( firstP);
		for (int i = 1; i < paths.size()-1; i++)
		{
			a_linePrm pp = randPos(
				{ (float)paths[i].x,(float)paths[i].y },
				{ (float)paths[i+1].x,(float)paths[i+1].y},
				jag,
				(i % 2 == 1) ? (float)infoP->comple_angle : (float)-infoP->comple_angle,
				500, i, infoP->seedAct
			);
			pp.b = (float)infoP->weight * hash_float(i, 1001, infoP->seedAct);
			basep.push_back(pp);
		}
		a_linePrm enfP = { (float)paths[paths.size()-1].x,(float)paths[paths.size() - 1].y,0.0f };
		basep.push_back(enfP);
	}
	else {
		for (int i = 0; i < paths.size(); i++)
		{
			A_FloatPoint start = { (float)paths[i].x, (float)paths[i].y };
			A_FloatPoint end= { 0,0 };
			if (i== paths.size()-1) {
				end = { (float)paths[i-1].x, (float)paths[i - 1].y };
			}
			else {
				end = { (float)paths[i + 1].x, (float)paths[i + 1].y };
			}

			a_linePrm pp = randPos(
				start,
				end,
				jag,
				(i % 2 == 1) ? (float)infoP->comple_angle : (float)-infoP->comple_angle,
				500, i, infoP->seedAct
			);
			pp.b = (float)(infoP->weight * hash_float(i, 1001, infoP->seedAct));
			basep.push_back(pp);
		}
	}



	for (A_long fc = 0; fc < infoP->fork; fc++) {
		std::vector<a_linePrm> points;
		for (A_long idx = 0; idx < basep.size() - 1; idx++)
		{
			float startWeight = (float)infoP->weight;
			float endWeight = (float)infoP->weight;
			if (idx == 0) {
				startWeight = (float)infoP->startWeight;
			}
			std::vector<a_linePrm> pointsD = CalcPoints(
				{ (float)basep[idx].x, (float)basep[idx].y },
				{ (float)basep[idx + 1].x, (float)basep[idx + 1].y },
				(float)startWeight,
				(float)infoP->weight,
				infoP->complexity,
				(float)infoP->jaggedness,
				(float)infoP->comple_angle,
				fc,
				infoP->seedAct+fc
			);
			if (pointsD.size() > 0) {
				points.insert(points.end(), pointsD.begin(), pointsD.end());
			}
		}
		points.push_back(
			{ (float)basep[basep.size() - 1].x, (float)basep[basep.size() - 1].y, (float)infoP->endWeight });
		if (isOpened == FALSE)
		{
			A_long ee = (A_long)points.size() - 1;
			std::vector<a_linePrm> pointsD = CalcPoints(
				{ (float)points[ee].x, (float)points[ee].y },
				{ (float)points[0].x, (float)points[0].y },
				(float)infoP->weight,
				(float)infoP->weight,
				infoP->complexity,
				(float)infoP->jaggedness,
				(float)infoP->comple_angle,
				fc,
				infoP->seedAct + 1000+fc
			);
			pointsD.push_back({ (float)points[0].x, (float)points[0].y, (float)infoP->startWeight });
			points.insert(points.end(), pointsD.begin(), pointsD.end());
		}
		std::vector<std::vector<a_linePrm>> sublines;
		for (int i = 0; i < paths.size()-1; i++)
		{
			int idx0 = (int)(hash_float(i+12, infoP->complexity + 10, infoP->seedAct) * infoP->complexity);
			int idx1 = (int)(hash_float(i+2, infoP->complexity + 100, infoP->seedAct) * infoP->complexity);
			idx0 = (int)AE_CLAMP(idx0 + (infoP->complexity + 1) * i,0, (int)points.size()-1);
			idx1 = (int)AE_CLAMP(idx1 + (infoP->complexity + 1) * (i+1), 0, (int)points.size() - 1);

			std::vector<a_linePrm> pointsD = CalcPoints(
				{ (float)points[idx0].x, (float)points[idx0].y },
				{ (float)points[idx1].x, (float)points[idx1].y },
				(float)infoP->weight / 2,
				(float)infoP->weight / 2,
				infoP->complexity,
				(float)infoP->jaggedness,
				(float)infoP->comple_angle,
				fc,
				infoP->seedAct+i
			);
			float bb = (float)(hash_float(i + 200, infoP->complexity + 108, infoP->seedAct)* infoP->weight/2);
			pointsD.push_back({ (float)points[idx1].x, (float)points[idx1].y, bb });
			if (idx0 > 0) {
				a_linePrm firstP = pointsD[0];
				firstP.b = 0;
				for (int i = 0; i < idx0; i++)
				{
					pointsD.insert(pointsD.begin(), firstP);
				}
			}
			if (pointsD.size() < points.size()) {
				a_linePrm lastP = pointsD[pointsD.size() - 1];
				lastP.b = 0;
				for (size_t i = pointsD.size(); i < points.size(); i++)
				{
					pointsD.push_back(lastP);
				}
				sublines.push_back(pointsD);
			}
			sublines.push_back(pointsD);
		}
		if (infoP->wipe != 1.0f) {
			points = shiftPerPoints(points, (float)infoP->wipe);
			if (sublines.size() > 0)
			{
				for (auto& subline : sublines)
				{
					subline = shiftPerPoints(subline, (float)infoP->wipe);
				}
			}
		}
		draw_polyline(buf, offsetLinePrm(points, ox, oy), 1.0f);
		if (sublines.size() > 0)
		{
			for (auto& subline : sublines)
			{
				draw_polyline(buf, offsetLinePrm(subline, ox, oy), 1.0f);
			}
		}
	}
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
		case MODE_PATH:
		{
			ERR(LigtningDrawPath(ae, infoP));
			break;
		}
		case MODE_2_2POINT:
		{
			ERR(LigtningDraw2_2Point(ae, infoP));
			break;
		}
		case MODE_1_2POINT:
		{
			ERR(LigtningDraw1_2Point(ae, infoP));
			break;
		}
		case MODE_RADICAL:
		{
			ERR(LigtningDrawRadical(ae, infoP));
			break;
		}
	}

	return err;
}
	