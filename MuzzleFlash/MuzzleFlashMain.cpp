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
	A_long		sparkCount,
	A_long		sparkSubCount,
	float		sparkScale,
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
	//メイン
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
	//中心からの枝
	for (A_long br = 0; br < branch*6; br++) {
		float rsize = rootSize * 0.1f + rootSize * 0.9f * (1.0f + 2.0f * hash_float(150, br + 100, seed));
		float rsizeS = rsize * 0.05f * hash_float(170, br + 50, seed);
		float len = 0.5f*rsize + ( (rsize + length * 0.2f) * hash_float(180, br + 60, seed));
		float angle2 = 360.0f - (360.0f - angle) * 0.6f;
		if (hash_float(150, br + 100, seed) <0.25f) {
			angle2 = 360.0f - (360.0f - angle) * 0.2f;
		}
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
	A_long pcount = 8;

	if (sparkCount > 0) {
		a_rectPrm points[10];
		for (int i = 0; i < 10; i++)
		{
			float v0 = 1.5f + 3.5f * hash_float(300, i + 100, seed);
			float v1 = 1.5f + 3.5f * 2 * hash_float(300, i + 110, seed);
			points[i].points[0] = { -v0 , -v1 };
			v0 = 1.5f + 3.5f * hash_float(310, i + 100, seed);
			v1 = 1.5f + 3.5f * hash_float(320, i + 120, seed);
			points[i].points[1] = { v0 , -v1 };
			v0 = 1.5f + 3.5f * hash_float(340, i + 110, seed);
			v1 = 1.5f + 3.5f * hash_float(360, i + 130, seed);
			points[i].points[2] = { v0 , v1 };
			v0 = 1.5f + 3.5f * hash_float(350, i + 150, seed);
			v1 = 1.5f + 3.5f * hash_float(370, i + 200, seed);
			points[i].points[3] = { -v0 , v1 };
		}
		for (int i = 0; i < sparkCount; i++)
		{
			float len = length * hash_float(180, i + 70, seed);
			float angle2 = 360.0f - (360.0f - angle) * 0.7f;
			if (hash_float(180, i + 70, seed) < 0.3f) {
				angle2 = 360.0f - (360.0f - angle) * 0.3f;
			}
			for (int j = 0; j < sparkSubCount; j++) {
				float lenB = len + length * 0.1f * (0.5f - 1.0f * hash_float(190, i + j * 100, seed));
				float angleB = angle2 + 35.0f * (0.5f - 1.0f * hash_float(290, i + j * 120, seed));
				float rot = direction + angleB * (0.5f - 1.0f * hash_float(301, i + j * 130, seed));
				float subAngleRad = rot * (float)PF_PI / 180.0f;
				if (scale != 1.0f) {
					len *= scale;
				}
				float x = sinf(subAngleRad) * lenB + pointX;
				float y = -cosf(subAngleRad) * lenB + pointY;

				int idx = (int)(9.0f * hash_float(300, i + 200, seed)) % 10;
				float ss = sparkScale*0.5f + 0.5f*sparkScale * (0.5f + hash_float(300, i + 300, seed));
				draw_a_rect(
					buf,
					OsRctPrm(points[idx], x, y, ss*scale),
					1.0f
				);
			}
		}
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
