#include "Sputtering.h"

PF_Err SputMain
(
	NF_AE* ae,
	ParamInfo* infoP,
	std::vector<A_u_char> *target,
	std::vector<std::vector<A_u_char>> *sput
)
{
	PF_Err err = PF_Err_NONE;
	size_t ss = target->size();
	float  area_rate = infoP->area_rate/(144*5);
	float  border_rate = infoP->border_rate/64;

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
	for (A_long y = 0; y < ae->outputInfo.height; y++) {
		for (A_long x = 0; x < ae->outputInfo.width; x++) {
			A_long mode = *(target->data() + (y * width + x)) ;
			
			float rand = hash_float(x, y, seed);
			float randScale = 1;
			if (mode == TARGET_AREA)
			{
				if (infoP->is_border_direction) {
					mode = TARGET_NONE;
				}
				else {
					if (area_rate < rand) mode = TARGET_NONE;
					randScale = 5;
				}
			}
			else if (mode == TARGET_BORDER) {
				if (border_rate < rand) mode = TARGET_NONE;
				randScale = 3;
			}

			if (mode != TARGET_NONE) {
				for (A_long c = 0; c < infoP->spawn_count; c++) {
					float angle = hash_float(x+c, y + 100, seed) * 360.0f;
					float radius = infoP->spawn_offset;
					radius *= hash_float(x+c, y + 230, seed);
					if (hash_float(x+c, y + 150, seed) < 0.2f) {
						radius *= randScale;
					}
					A_long xx = (A_long)(radius * sin(angle * M_PI / 180.0f));
					A_long yy = (A_long)(radius * -cos(angle * M_PI / 180.0f));
					float rscale = 1.0f;
					if (infoP->sizeRandom > 0) {
						rscale = (1 - infoP->sizeRandom) + hash_float(x+c, y + 200, seed) * infoP->sizeRandom;
					}
					A_long num = 0;
					if (infoP->sput_loop_mode == 1) { // Random
						 num = (A_long)(hash_float(x + c, y + 300, seed) * (sput->size() - 1));
					}
					else { // Loop
						
						num = (A_long)(hash_float(x + c, y + 300, seed) * 1000) + seed_loop;
						num %= sput->size();
					}
					A_long colorIndex = (A_long)(hash_float(x+c, y + 350, seed) * (infoP->colorCount))% infoP->colorCount;
					float opa = infoP->opacity;
					if (infoP->opacityRand > 0) {
						opa *= (1 - infoP->opacityRand) + hash_float(x+c, y + 400, seed) * infoP->opacityRand;
					}
					AE_CLAMP(opa, 0, 1.0f);
					BlendSpatToWorld(
						ae->output,
						ae->pixelFormat(),
						x + xx,
						y + yy,
						ScaleSpatBilinear(sput->at(num), rscale),
						infoP->colors[colorIndex],
						opa);
				}
			}
		}
	}

	return PF_Err_NONE;
}