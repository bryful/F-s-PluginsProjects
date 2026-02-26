#include "TouchLines.h"






PF_Err TouchStraight(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<PF_Point>& points
)
{
	PF_Err err = PF_Err_NONE;
	if (points.size() <= 0 || infoP->opacity<=0||infoP->size<=0
		||(infoP->inner_length<=0 && infoP->outer_length<=0)) return err;
	float angle_rad = (float)(infoP->angle * (float)M_PI / 180.0f);
	float rdx = -sin(angle_rad);
	float rdy = cos(angle_rad);
	float lenBase = (float)(MAX(ae->outputInfo.width, ae->outputInfo.height)/2);
	for (A_long i = 0; i < (A_long)points.size(); i++) {
		PF_Point p = points[i];
		float cX = (float)p.x;
		float cY = (float)p.y;

		float olen = (float)infoP->outer_length * lenBase;
		float lenrand = (float)hash_float(i * 10, 10, infoP->seedAct);
		olen = (float)(olen * (1 - infoP->length_rand) + olen * infoP->length_rand * lenrand);
		float ilen = (float)infoP->inner_length * lenBase;
		ilen = (float)(ilen * (1 - infoP->length_rand) + ilen * infoP->length_rand * lenrand);
		if (olen <= 10 || ilen <= 10) continue;
		float ox0 = (float)(cX - rdx * olen);
		float oy0 = (float)(cY - rdy * olen);
		float ix0 = (float)(cX + rdx * ilen);
		float iy0 = (float)(cY + rdy * ilen);

		float size = (float)infoP->size;
		size = (float)(size * (1-infoP->size_rand) + size * infoP->size_rand * hash_float(i * 20, 20, infoP->seedAct));
		float opa = (float)infoP->opacity;
		opa = AE_CLAMP((float)(opa * (1-infoP->opacity_rand) + opa * infoP->opacity_rand * hash_float(i * 30, 30, infoP->seedAct)),0,1.0f);
		A_long colIndex = 0;
		if (infoP->colorCount > 0) {
			colIndex =(A_long)((double)infoP->colorCount * hash_float(i * 40, 40, infoP->seedAct)) % infoP->colorCount;
		}
		ERR(DrawAA_Line(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			ox0, oy0, cX, cY,
			infoP->colors[colIndex],
			0, opa,
			size / 2, size));
		ERR(DrawAA_Line(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			cX, cY, ix0, iy0,
			infoP->colors[colIndex],
			opa, 0,
			size , size / 2));

	}
	return err;
}