#include "RadialLines.h"


inline A_FloatPoint LinePar(A_FloatPoint sP, A_FloatPoint eP, PF_FpLong par)
{
	A_FloatPoint ret{ 0,0 };
	ret.x = sP.x + (eP.x - sP.x) * par;
	ret.y = sP.y + (eP.y - sP.y) * par;
	return ret;
}
// *******************************************************************************
template <typename PixelType, typename ChannelType>
PF_Err Exec8_16_32T (
	NF_AE *ae,
	ParamInfo *infoP
)
{
	PF_Err			err = PF_Err_NONE;
	if (infoP->value <= 0) return err;

	PixelType cols[8];
	if constexpr (std::is_same<PixelType, PF_Pixel8>::value) {
		for (A_long i = 0; i < infoP->colorCount; i++) {
			cols[i] = infoP->colors[i];
		}
	}
	else if constexpr (std::is_same<PixelType, PF_Pixel16>::value)
	{
		for (A_long i = 0; i < infoP->colorCount; i++) {
			cols[i] = NF_Pixel8To16( infoP->colors[i]);
		}
	}
	else {
		for (A_long i = 0; i < infoP->colorCount; i++) {
			cols[i] = NF_Pixel8To32(infoP->colors[i]);
		}
	}
	float wh = (float)((A_long)(infoP->weight/2+0.5));

	if(infoP->values[0] > 0) {
		for (A_long i = 0; i < infoP->values[0]; i++)
		{
			//まず位置
			A_long x = (A_long)(hash_float(0, i, infoP->seed) * ae->outputInfo.width);
			//長さ
			PF_FpLong lenPar = infoP->length_par * (1 - infoP->length_rand) + hash_float(1, i, infoP->seed) * (infoP->length_par * infoP->length_rand);
			A_FloatPoint sP{ (float)x, -wh };
			A_FloatPoint eP = LinePar(sP, infoP->cente_pos, lenPar);
			//透明度
			PF_FpLong opaPar = infoP->opacity_par * (1 - infoP->opacity_rand) + hash_float(2, i, infoP->seed) * (infoP->opacity_par * infoP->opacity_rand);
			if (opaPar < 0) opaPar = 0;	else if (opaPar > 1) opaPar = 1;
			//太さ
			PF_FpLong weight = infoP->weight * (1 - infoP->weight_rand) + hash_float(3, i, infoP->seed) * (infoP->weight * infoP->weight_rand);

			PixelType color = { 0,0,0,0 };
			if (infoP->colorCount >= 1) {
				color = cols[(A_long)(hash_float(4, i, infoP->seed) * infoP->colorCount)];
			}
			if (opaPar > 0 && weight > 0 && lenPar > 0) {
				if constexpr (std::is_same<PixelType, PF_Pixel8>::value) {
					DrawAA_Line8(ae->in_data, ae->output, ae->suitesP,
						(float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, 
						color, (float)opaPar, 0,
						(float)weight, 0.001f);
				}
				else if constexpr (std::is_same<PixelType, PF_Pixel16>::value) {
					DrawAA_Line16(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
				else {
					DrawAA_Line32(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
			}
		}
	}
	if (infoP->values[1] > 0) {
		for (A_long i = 0; i < infoP->values[1]; i++)
		{
			//まず位置
			A_long y = (A_long)(hash_float(5, i, infoP->seed) * ae->outputInfo.height);
			//長さ
			PF_FpLong lenPar = infoP->length_par * (1 - infoP->length_rand) + hash_float(6, i, infoP->seed) * (infoP->length_par * infoP->length_rand);
			A_FloatPoint sP{ (float)(ae->output->width-1+wh), (float)y };
			A_FloatPoint eP = LinePar(sP, infoP->cente_pos, lenPar);
			//透明度
			PF_FpLong opaPar = infoP->opacity_par * (1 - infoP->opacity_rand) + hash_float(7, i, infoP->seed) * (infoP->opacity_par * infoP->opacity_rand);
			if (opaPar < 0) opaPar = 0;	else if (opaPar > 1) opaPar = 1;
			//太さ
			PF_FpLong weight = infoP->weight * (1 - infoP->weight_rand) + hash_float(8, i, infoP->seed) * (infoP->weight * infoP->weight_rand);

			PixelType color = { 0,0,0,0 };
			if (infoP->colorCount >= 1) {
				color = cols[(A_long)(hash_float(9, i, infoP->seed) * infoP->colorCount)];
			}
			if (opaPar > 0 && weight > 0 && lenPar > 0) {
				if constexpr (std::is_same<PixelType, PF_Pixel8>::value) {
					DrawAA_Line8(ae->in_data, ae->output, ae->suitesP,
						(float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, 
						color, (float)opaPar, 0,
						(float)weight, 0.001f);
				}
				else if constexpr (std::is_same<PixelType, PF_Pixel16>::value) {
					DrawAA_Line16(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
				else {
					DrawAA_Line32(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
			}
		}
	}
	if (infoP->values[2] > 0) {
		for (A_long i = 0; i < infoP->values[2]; i++)
		{
			//まず位置
			A_long x = (A_long)(hash_float(10, i, infoP->seed) * ae->outputInfo.width);
			//長さ
			PF_FpLong lenPar = infoP->length_par * (1 - infoP->length_rand) + hash_float(11, i, infoP->seed) * (infoP->length_par * infoP->length_rand);
			A_FloatPoint sP{ (float)x, (float)(ae->outputInfo.height-1+wh) };
			A_FloatPoint eP = LinePar(sP, infoP->cente_pos, lenPar);
			//透明度
			PF_FpLong opaPar = infoP->opacity_par * (1 - infoP->opacity_rand) + hash_float(12, i, infoP->seed) * (infoP->opacity_par * infoP->opacity_rand);
			if (opaPar < 0) opaPar = 0;	else if (opaPar > 1) opaPar = 1;
			//太さ
			PF_FpLong weight = infoP->weight * (1 - infoP->weight_rand) + hash_float(13, i, infoP->seed) * (infoP->weight * infoP->weight_rand);

			PixelType color = { 0,0,0,0 };
			if (infoP->colorCount >= 1) {
				color = cols[(A_long)(hash_float(14, i, infoP->seed) * infoP->colorCount)];
			}
			if (opaPar > 0 && weight > 0 && lenPar > 0) {
				if constexpr (std::is_same<PixelType, PF_Pixel8>::value) {
					DrawAA_Line8(ae->in_data, ae->output, ae->suitesP,
						(float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y,
						color, (float)opaPar, 0,
						(float)weight, 0.001f);
				}
				else if constexpr (std::is_same<PixelType, PF_Pixel16>::value) {
					DrawAA_Line16(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
				else {
					DrawAA_Line32(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
			}
		}
	}
	if (infoP->values[3] > 0) {
		for (A_long i = 0; i < infoP->values[3]; i++)
		{
			//まず位置
			A_long y = (A_long)(hash_float(15, i, infoP->seed) * ae->outputInfo.height);
			//長さ
			PF_FpLong lenPar = infoP->length_par * (1 - infoP->length_rand) + hash_float(16, i, infoP->seed) * (infoP->length_par * infoP->length_rand);
			A_FloatPoint sP{ -wh, (float)y };
			A_FloatPoint eP = LinePar(sP, infoP->cente_pos, lenPar);
			//透明度
			PF_FpLong opaPar = infoP->opacity_par * (1 - infoP->opacity_rand) + hash_float(17, i, infoP->seed) * (infoP->opacity_par * infoP->opacity_rand);
			if (opaPar < 0) opaPar = 0;	else if (opaPar > 1) opaPar = 1;
			//太さ
			PF_FpLong weight = infoP->weight * (1 - infoP->weight_rand) + hash_float(18, i, infoP->seed) * (infoP->weight * infoP->weight_rand);

			PixelType color = { 0,0,0,0 };
			if (infoP->colorCount >= 1) {
				color = cols[(A_long)(hash_float(19, i, infoP->seed) * infoP->colorCount)];
			}
			if (opaPar > 0 && weight > 0 && lenPar > 0) {
				if constexpr (std::is_same<PixelType, PF_Pixel8>::value) {
					DrawAA_Line8(ae->in_data, ae->output, ae->suitesP,
						(float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y,
						color, (float)opaPar, 0,
						(float)weight, 0.001f);
				}
				else if constexpr (std::is_same<PixelType, PF_Pixel16>::value) {
					DrawAA_Line16(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
				else {
					DrawAA_Line32(ae->in_data, ae->output, ae->suitesP, (float)sP.x, (float)sP.y, (float)eP.x, (float)eP.y, color, (float)opaPar, 0, (float)weight, 0.001f);
				}
			}
		}
	}
	return err;
}

PF_Err Exec8_16_32(
	NF_AE* ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB32:
		return Exec8_16_32T<PF_Pixel8, A_u_char>(ae, infoP);
	case PF_PixelFormat_ARGB64:
		return Exec8_16_32T<PF_Pixel16, A_u_short>(ae, infoP);
	case PF_PixelFormat_ARGB128:
		return Exec8_16_32T<PF_PixelFloat, PF_FpShort>(ae, infoP);
	}
	return err;
}
// *******************************************************************************
