#include "BurstLines.h"


PF_Err BurstLinesExec(
	NF_AE *ae,
	ParamInfo *infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->lineCount <= 0) {
		return err;
	}
	float dist = (float)ae->downScale(infoP->distance);
	float len = (float)ae->downScale(infoP->length);
	float tick = (float)ae->downScale(infoP->thickness);
	if (dist <= 0 || len <= 0 || tick <=0) {
		return err;
	}
	if (infoP->opacity_start <= 0 && infoP->opacity_end <= 0)  return err;

	for(A_long cnt=0; cnt < infoP->lineCount; cnt++) {
		
		float angle = infoP->direction;
		if (infoP->angleRange > 0) {
			angle += (360.0f * infoP->angleRange) * (0.5f - hash_float(cnt, 100, infoP->seedAct));
		}
		for (int  sc = 0; sc < infoP->lineSubCount; sc++) {
			float dist2 = dist;
			float distPer = 1.0f;
			if (infoP->distanceRand > 0) {
				distPer = 1 - infoP->distanceRand * hash_float(cnt, sc+200, infoP->seedAct);
			}
			dist2 *= distPer;
			if (dist2 <= 0) continue;
			float len2 = len;
			if (infoP->lengthRand > 0) {
				len2 = len2 * ((1 - infoP->lengthRand) + infoP->lengthRand * hash_float(cnt, sc +300, infoP->seedAct));
			}
			len2 *= distPer; // 距離が短いほど線も短くする
			if (len2 > dist2) len2 = dist2; // 線の長さが距離を超えないようにする
			if (len2 <= 0) continue;
			float tick2 = tick;
			if (infoP->thicknessRand > 0) {
				tick2 = tick2 * ((1 - infoP->thicknessRand) + infoP->thicknessRand * hash_float(cnt, sc+500, infoP->seedAct));
			}
			if (tick2 <= 0) continue;

			float st = tick2 * infoP->thick_start;
			float et = tick2 * infoP->thick_end;
			if (st <= 0 && et <= 0) continue;	


			float subAngle = angle + infoP->lineSubAngle*(0.5f- hash_float(cnt, sc + 500, infoP->seedAct));
			float rad = subAngle * (float)PF_PI / 180.0f;
			float xA = sinf(rad);
			float yA = -cosf(rad);

			float ep = AE_CLAMP(dist2 - len2, 0, dist2);
			float sx = (float)infoP->center.x + xA * dist2;
			float sy = (float)infoP->center.y + yA * dist2;
			float ex = (float)infoP->center.x + xA * ep;
			float ey = (float)infoP->center.y + yA * ep;

			A_long colIdx = (A_long)((float)(infoP->colorCount - 1) * hash_float(cnt, sc + 300, infoP->seedAct)+0.5f);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				sx, sy, ex, ey,
				infoP->colors[colIdx],
				infoP->opacity_start,
				infoP->opacity_end,
				st, et));
		}
	}

	return err;
}
