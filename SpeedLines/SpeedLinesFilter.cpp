#include "SpeedLines.h"
// 平行線描画のためのベクトルデータ構造体
struct LineDirection {
	float dx; // 角度方向の単位ベクトルX
	float dy; // 角度方向の単位ベクトルY
	float px; // 垂直方向(+90度)の単位ベクトルX
	float py; // 垂直方向(+90度)の単位ベクトルY
};
struct PosPrm {
	float side; 
	float offset;
	float line; 
	float opa;
	float size;
	A_long colIndex;
};

inline PosPrm Rand_PosPrm(ParamInfo *infoP, float bside,float blength, PF_FpLong near1, PF_FpLong far1, PF_FpLong nearPer,A_long idx, A_long ff,A_long seed)
{
	PosPrm prm;
	float near2 = (float)near1;
	if (nearPer > 0.0f)
	{
		near2 = (float)near1 + ((float)far1- (float)near1)* (float)nearPer;
	}
	prm.side = (float)((bside/2) * far1);
	prm.side = (float)(prm.side * near2 + prm.side * (1 - near2) * hash_float(idx * 10, ff, seed));
	
	float rnd = hash_float(idx * 15, ff + 15, seed);
	if (rnd < 0.1f) {
		prm.side *= 0.5f;
	}
	
	prm.offset = (float)(blength - 2 * blength * hash_float(idx * 20, ff+20, seed));
	prm.line = (float)(blength * infoP->length);
	prm.line = (float)(prm.line * infoP->length_rand + prm.line * (1 - infoP->length_rand) * hash_float(idx * 30, ff+30, seed));
	prm.opa = (float)infoP->opacity;
	prm.opa = (float)(prm.opa * infoP->opacityRand + prm.opa * (1 - infoP->opacityRand) * hash_float(idx * 40, ff+40, seed));
	prm.size = (float)(infoP->size);
	prm.size = (float)(prm.size * infoP->sizeRand + prm.size * (1 - infoP->sizeRand) * hash_float(idx* 50, ff+50, seed));
	prm.colIndex = (A_long)(hash_float(idx * 60, ff+60, seed) * infoP->colorCount);


	return prm;
}
/**
 * 描画ループの前（Pre-render）で一度だけ計算する
 * @param rot 角度(Degree)
 */
inline LineDirection PrepareLineDirection(float rot) {
	float rad = (float)(rot * M_PI / 180.0);
	LineDirection ld;

	// 角度方向の単位ベクトル
	ld.dx = cos(rad);
	ld.dy = sin(rad);

	// 垂直方向(+90度)の単位ベクトル
	// cos(a + 90) = -sin(a), sin(a + 90) = cos(a)
	ld.px = -ld.dy;
	ld.py = ld.dx;

	return ld;
}
typedef struct {
	float	cX, cY, sideX, sideY, offsetX, offsetY, lx0, ly0, lx1, ly1;
} LinePos;
/**
 * 各ラインの端点や、ピクセルごとの座標を求める高速化版
 * @param centerX, centerY 基準点
 * @param length1 角度方向の距離
 * @param length2 垂直方向の距離
 * @param ld 事前に計算したベクトル
 */
A_FloatPoint CalculatePosition(
	float centerX,
	float centerY,
	float length1,
	float length2,
	const LineDirection& ld)
{
	A_FloatPoint result;

	// sin, cosを呼び出さず、乗算と加算のみで算出
	result.x = (float)(centerX + length1 * ld.dx + length2 * ld.px);
	result.y = (float)(centerY + length1 * ld.dy + length2 * ld.py);

	return result;
}
inline LinePos SL_Position(
	float centerX,
	float centerY,
	float lengthSide,
	float lengthOffset,
	float lengthLine,
	const LineDirection& ld)
{
	LinePos result;
	result.cX = centerX;
	result.cY = centerY;
	result.sideX = (float)(result.cX + lengthSide * ld.dx);
	result.sideY = (float)(result.cY + lengthSide * ld.dy);
	result.offsetX = (float)(result.sideX + lengthOffset * ld.px);
	result.offsetY = (float)(result.sideY + lengthOffset * ld.py);

	float linel = lengthLine / 2;
	result.lx0 = (float)(result.offsetX + -linel * ld.px);
	result.ly0 = (float)(result.offsetY + -linel * ld.py);
	result.lx1 = (float)(result.offsetX + +linel * ld.px);
	result.ly1 = (float)(result.offsetY + +linel * ld.py);

	return result;
}


PF_Err SpeedLineExec(
	NF_AE *ae,
	ParamInfo* infoP
)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;

	ERR(PF_FILL(NULL, NULL, ae->output));
	if (infoP->value == 0 || infoP->opacity == 0 || infoP->size == 0 || infoP->length == 0
		|| (infoP->length1_value == 0 && infoP->length2_value == 0)) {
		if (infoP->isBlend) {
			ERR(ae->CopyInToOut());
		}
		return err;
	}
	A_long seed = infoP->seed;
	if(infoP->is_autoSeed) {
		seed = seed + ae->frame();
	}
	A_long lineCount = (A_long)(infoP->value* (double)MAX(ae->outputInfo.width, ae->outputInfo.height)+0.5f);
	A_long lineCount1 = (A_long)((double)lineCount * infoP->length1_value+0.5f);
	A_long lineCount2 = (A_long)((double)lineCount * infoP->length2_value + 0.5f);
	float baseLength = (float)MAX(ae->outputInfo.width, ae->outputInfo.height);
	float baseSide = (float)sqrt(pow(ae->outputInfo.width,2)+ pow(ae->outputInfo.height,2));

	LineDirection rotd = PrepareLineDirection(infoP->angle);
	if (lineCount1 > 0) {
		lineCount1 /= 3;
		if (lineCount1 <= 0) lineCount1 = 1;
		for (A_long i = 0; i < lineCount1; i++) {
			PosPrm prm = Rand_PosPrm(infoP, baseSide, baseLength,
				infoP->length1_near, infoP->length1_far, 0, i,
				10, seed);
			LinePos lp = SL_Position(
				(float)infoP->center.x,
				(float)infoP->center.y,
				prm.side,
				prm.offset,
				prm.line,
				rotd);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.lx0, lp.ly0, lp.offsetX, lp.offsetY,
				infoP->colors[prm.colIndex],
				0, prm.opa,
				prm.size / 2, prm.size));
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.offsetX, lp.offsetY, lp.lx1, lp.ly1,
				infoP->colors[prm.colIndex],
				prm.opa, 0,
				prm.size, prm.size / 2));
			// ***
			prm = Rand_PosPrm(infoP, baseSide, baseLength,
				infoP->length1_near, infoP->length1_far, 0.5, i,
				20, seed);
			lp = SL_Position(
				(float)infoP->center.x,
				(float)infoP->center.y,
				prm.side,
				prm.offset,
				prm.line,
				rotd);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.lx0, lp.ly0, lp.offsetX, lp.offsetY,
				infoP->colors[prm.colIndex],
				0, prm.opa,
				prm.size / 2, prm.size));
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.offsetX, lp.offsetY, lp.lx1, lp.ly1,
				infoP->colors[prm.colIndex],
				prm.opa, 0,
				prm.size, prm.size / 2));
			// ***
			prm = Rand_PosPrm(infoP, baseSide, baseLength,
				infoP->length1_near, infoP->length1_far, 0.7, i,
				30, seed);
			lp = SL_Position(
				(float)infoP->center.x,
				(float)infoP->center.y,
				prm.side,
				prm.offset,
				prm.line,
				rotd);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.lx0, lp.ly0, lp.offsetX, lp.offsetY,
				infoP->colors[prm.colIndex],
				0, prm.opa,
				prm.size / 2, prm.size));
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.offsetX, lp.offsetY, lp.lx1, lp.ly1,
				infoP->colors[prm.colIndex],
				prm.opa, 0,
				prm.size, prm.size / 2));
		}
	}
	// *********************
	if (lineCount2 > 0) {
		lineCount2 /= 3;
		if (lineCount2 <= 0) lineCount2 = 1;
		for (A_long i = 0; i < lineCount2; i++) {
			PosPrm prm = Rand_PosPrm(infoP, baseSide, baseLength,
				infoP->length2_near, infoP->length2_far, 0, i,
				30, seed);
			LinePos lp = SL_Position(
				(float)infoP->center.x,
				(float)infoP->center.y,
				-prm.side,
				prm.offset,
				prm.line,
				rotd);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.lx0, lp.ly0, lp.offsetX, lp.offsetY,
				infoP->colors[prm.colIndex],
				0, prm.opa,
				prm.size / 2, prm.size));
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.offsetX, lp.offsetY, lp.lx1, lp.ly1,
				infoP->colors[prm.colIndex],
				prm.opa, 0,
				prm.size, prm.size / 2));
			// ***
			prm = Rand_PosPrm(infoP, baseSide, baseLength,
				infoP->length2_near, infoP->length2_far, 0.6, i,
				75, seed);
			lp = SL_Position(
				(float)infoP->center.x,
				(float)infoP->center.y,
				-prm.side,
				prm.offset,
				prm.line,
				rotd);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.lx0, lp.ly0, lp.offsetX, lp.offsetY,
				infoP->colors[prm.colIndex],
				0, prm.opa,
				prm.size / 2, prm.size));
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.offsetX, lp.offsetY, lp.lx1, lp.ly1,
				infoP->colors[prm.colIndex],
				prm.opa, 0,
				prm.size, prm.size / 2));
			// ***
			prm = Rand_PosPrm(infoP, baseSide, baseLength,
				infoP->length2_near, infoP->length2_far, 0.8, i,
				86, seed);
			lp = SL_Position(
				(float)infoP->center.x,
				(float)infoP->center.y,
				-prm.side,
				prm.offset,
				prm.line,
				rotd);
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.lx0, lp.ly0, lp.offsetX, lp.offsetY,
				infoP->colors[prm.colIndex],
				0, prm.opa,
				prm.size / 2, prm.size));
			ERR(DrawAA_Line(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				lp.offsetX, lp.offsetY, lp.lx1, lp.ly1,
				infoP->colors[prm.colIndex],
				prm.opa, 0,
				prm.size, prm.size / 2));
		}
	}
	if (infoP->blue > 0) {
		ERR(Blur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->blue
		));
	}
	ERR(Mult(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		TRUE
	));
	if (infoP->noise > 0) {
		A_long count = (A_long)((double)ae->outputInfo.width * (double)ae->outputInfo.height * infoP->noise/8);
		ERR(ApplyANoise(ae, count, seed));
	}
	return PF_Err_NONE;
}