#include "SputteringPosition.h"

PF_Err SputMain
(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<std::vector<A_u_char>> *sput
)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->count <= 0 || infoP->opacity <= 0||infoP->scale<=0 ||sput->size()<=0) return err;
	A_long width = ae->outputInfo.width;
	A_long height = ae->outputInfo.height;
	A_long widthTrue = ae->outputInfo.widthTrue;
	A_long seed = infoP->seed;
	if (infoP->autoSeed && infoP->sput_loop_mode == 1)
	{
		seed = infoP->seed + (ae->frame()/infoP->sput_loop_koma);
	}
	A_long seed_loop = infoP->seed;
	if (infoP->sput_loop_mode ==2)
	{
		seed_loop = infoP->seed + (ae->frame() / infoP->sput_loop_koma);
	}
	for (A_long idx = 0; idx < infoP->count; idx++) {
		A_long x = 0;
		A_long y = 0;
		PF_Point center;

		float dx = (float)(infoP->end_point.x - infoP->start_point.x) / 2;
		if (infoP->mode == 1) { // Circle
			float angle = hash_float(idx, 100, seed) * 360.0f;
			float radius1 = infoP->radius;
			radius1 *= hash_float(idx, 200, seed);
			float rnd1 = hash_float(idx, 150, seed);
			if (rnd1 < 0.1f) {
				radius1 *= 2.0f;
			}else if (rnd1 < 0.2f) {
				radius1 *= 1.3f;
			}else if (rnd1 < 0.3f) {
				radius1 *= 0.6f;
			}
			x = (A_long)(infoP->center.x + radius1 * sin(angle * M_PI / 180.0f));
			y = (A_long)(infoP->center.y + radius1 * -cos(angle * M_PI / 180.0f));
			if (infoP->is_anchor_point_enabled) {
				center = infoP->anchor_point;
			}
			else {
				center = infoP->center;
			}
		}
		else if (infoP->mode == 2) { // Line
			float dy = (float)(infoP->end_point.y - infoP->start_point.y) / 2;
			float dx2 = dx - hash_float(idx, 450, seed) * 2 * dx;
			float dy2 = dy - hash_float(idx, 750, seed) * 2 * dy;

			center.x = infoP->start_point.x + (A_long)dx;
			center.y = infoP->start_point.y + (A_long)dy;

			float rnd2 = hash_float(idx, 800, seed);
			if (rnd2 < 0.2f) {
				dx2 *= 0.2f;
				dy2 *= 0.2f;
			}
			else if (rnd2 < 0.4f) {
				dx2 *= 0.4f;
				dy2 *= 0.4f;
			}
			else if (rnd2 < 0.6f) {
				dx2 *= 0.6f;
				dy2 *= 0.6f;
			}
			x = center.x + (A_long)dx2;
			y = center.y + (A_long)dy2;
			if (infoP->is_anchor_point_enabled) {
				center = infoP->anchor_point;
			}
		}
		else 
		{ continue; 
		}
		for (A_long c = 0; c < infoP->spawn_count; c++) {
			float angle = hash_float(idx+c, 500+c, seed) * 360.0f;
			float radius = infoP->spawn_offset;
			radius *= hash_float(idx + c, 600 + c, seed);
			if (hash_float(idx + c, 700 + c, seed) < 0.2f) {
				radius *= 2;
			}
			A_long xx = (A_long)(radius * sin(angle * M_PI / 180.0f)) + x;
			A_long yy = (A_long)(radius * -cos(angle * M_PI / 180.0f)) + y;
			if(infoP->scale != 1.0f && infoP->is_scalable) {
				xx = (A_long)((float)(xx - center.x) * infoP->scale + center.x + 0.5);
				yy = (A_long)((float)(yy - center.y) * infoP->scale + center.y + 0.5);
			}
			float rscale = 1.0f;
			if (infoP->sizeRandom > 0) {
				rscale = (1 - infoP->sizeRandom) + hash_float(idx + c, 800 + c, seed) * infoP->sizeRandom;
			}
			A_long num = 0;
			if (infoP->sput_loop_mode == 1) { // Random
				num = (A_long)(hash_float(idx+c, 900, seed + c) * (sput->size() - 1));
			}
			else { // Loop

				num = (A_long)(hash_float(idx + c, 1900, seed + c) * 1000) + seed_loop;
				num %= sput->size();
			}
			A_long colorIndex = (A_long)(hash_float(idx + c, 550 + c, seed) * (infoP->colorCount)) % infoP->colorCount;
			float opa = infoP->opacity;
			if (infoP->opacityRand > 0) {
				opa *= (1 - infoP->opacityRand) + hash_float(idx + c, 2900 + c, seed) * infoP->opacityRand;
			}
			AE_CLAMP(opa, 0, 1.0f);
			if (opa > 0 && rscale > 0) {
				BlendSpatToWorld(
					ae->output,
					ae->pixelFormat(),
					xx,
					yy,
					ScaleSpatBilinear(sput->at(num), rscale),
					infoP->colors[colorIndex],
					opa);
			}
		}
	}
	return PF_Err_NONE;
}