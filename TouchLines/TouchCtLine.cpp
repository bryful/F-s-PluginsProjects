#include "TouchLines.h"


A_FloatPoint CalculatePos(PF_Point satrt ,PF_Point end ,float per) {

	A_FloatPoint p;
	p.x = (float)(satrt.x + (end.x - satrt.x) * per);
	p.y = (float)(satrt.y + (end.y - satrt.y) * per);
	return p;

}



PF_Err TouchCenter(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<PF_Point>& points
)
{
	PF_Err err = PF_Err_NONE;
	if (points.size() <= 0 || infoP->opacity<=0||infoP->size<=0
		||(infoP->inner_length<=0 && infoP->outer_length<=0)) return err;
	for (A_long i = 0; i < (A_long)points.size(); i++) {
		PF_Point p = points[i];
		
		float iPer = (float)infoP->inner_length;
		iPer = (float)(iPer * (1 - infoP->length_rand) + iPer * infoP->length_rand * hash_float(i * 10, 10, infoP->seedAct));
		A_FloatPoint iP = CalculatePos(p, infoP->center, iPer);
		float oPer = (float)infoP->outer_length;
		oPer = (float)(oPer * (1 - infoP->length_rand) + oPer * infoP->length_rand * hash_float(i * 20, 20, infoP->seedAct));
		A_FloatPoint oP = CalculatePos(p, infoP->center, -oPer);

		float size = (float)infoP->size;
		size = (float)(size * (1-infoP->size_rand) + size * infoP->size_rand * hash_float(i * 20, 20, infoP->seedAct));
		float opa = (float)infoP->opacity;
		opa = AE_CLAMP((float)(opa * (1-infoP->opacity_rand) + opa * infoP->opacity_rand * hash_float(i * 30, 30, infoP->seedAct)),0,1.0f);
		A_long colIndex = 0;
		if (infoP->colorCount > 0) {
			colIndex =(A_long)((double)infoP->colorCount * hash_float(i * 40, 40, infoP->seedAct)) % infoP->colorCount;
		}
		if (oPer > 0) {
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				(float)oP.x, (float)oP.y, (float)p.x, (float)p.y,
				infoP->colors[colIndex],
				0, opa,
				size / 2, size));
		}
		if (iPer > 0) {
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				(float)p.x, (float)p.y, (float)iP.x, (float)iP.y,
				infoP->colors[colIndex],
				opa, 0,
				size, size / 2));
		}
	}

	return err;
}