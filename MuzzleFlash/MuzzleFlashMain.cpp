#include "MuzzleFlashMain.h"

PF_Err MuzzleFlashExec(
	PF_InData* in_data,
	AEGP_SuiteHandler* suites,
	PF_PixelFormat pixelFormat,
	PF_EffectWorld* output,
	float		pointX,
	float		pointY,
	A_long		count,
	float		rootSize,
	float		length,
	A_long		branch,
	float		direction,
	float		angle,
	PF_Pixel	color,
	float		scale,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;
	if (count <= 0 || rootSize <= 0 || length <= 0 || branch <= 0 || scale <= 0) {
		return err;
	}
	std::vector<std::vector<float>> buf(output->height,std::vector<float>(output->width,0));
	a_linePrm ct = { 0,0,0 };
	a_linePrm ep = { 0,0,0 };
	for (A_long cnt = 0; cnt < count; cnt++) {
		float rot = direction + angle * (0.5f - hash_float(cnt, 100, seed));
		float angle2 = 360.0f - (360.0f - angle) * 0.8f;
		float rot2 = direction + angle2 * (0.5f - hash_float(cnt, 100, seed));
		for(A_long br =0; br< branch; br++) {
			float rsize = rootSize*0.5f + rootSize * 0.5f * (1.0f + 2.0f * hash_float(cnt, br + 200, seed));
			float rsizeS = rsize * 0.05f * hash_float(cnt, br + 250, seed);
			float addR = 5 * (1.0f - 2.0f * hash_float(cnt, br + 300, seed));
			float subRot = rot + addR;
			if (hash_float(cnt, br + 300, seed) <0.25f) {
				subRot = rot2 + addR;
			}
			float subAngleRad = subRot * (float)PF_PI / 180.0f;
			float ax = 0.1f * rsize * (1.0f - 2.0f * hash_float(cnt, br + 300, seed));
			float ay = 0.1f * rsize * (1.0f - 2.0f * hash_float(cnt, br + 400, seed));
			
			float len = length*0.5f + length * 0.5f * (1.0f - 2.0f * hash_float(cnt, br + 500, seed));
			if (hash_float(cnt, br + 600, seed) <0.1f) {
				len *= 1.3f;
			}
			if (scale!=1.0f) {
				len *= scale;
				ax *= scale;
				ay *= scale;
				rsize *= scale;
				//rsizeS *= scale;
			}
			ct.x = pointX + ax;
			ct.y = pointY + ay;
			ct.b = rsize;
			ep.x = ct.x + sinf(subAngleRad) * len;
			ep.y = ct.y - cosf(subAngleRad) * len;
			ep.b = rsizeS * scale;
			draw_a_line(buf, ct, ep, 1.0f);
		}
	}
	for (A_long br = 0; br < branch*4; br++) {
		float rsize = rootSize * 0.1f + rootSize * 0.9f * (1.0f + 2.0f * hash_float(150, br + 100, seed));
		float rsizeS = rsize * 0.05f * hash_float(170, br + 50, seed);
		float len =  rsize * 1.5f + length * 0.1f;
		float angle2 = 360.0f - (360.0f - angle) * 0.4f;
		float rot = direction + angle2 * (0.5f - hash_float(200, br+100, seed));
		float subAngleRad = rot * (float)PF_PI / 180.0f;
		if (scale != 1.0f) {
			len *= scale;
			rsize *= scale;
			//rsizeS *= scale;
		}
		ct.x = pointX;
		ct.y = pointY;
		ct.b = rsize;
		ep.x = ct.x + sinf(subAngleRad) * len;
		ep.y = ct.y - cosf(subAngleRad) * len;
		ep.b = rsizeS *scale;
		draw_a_line(buf, ct, ep, 1.0f);
	}

	DrawColorMask(
		in_data,
		output,
		pixelFormat,
		suites,
		&buf,
		color
	);

	return err;
}
