#include "Filter.h"



static PF_Err
BaseCopy8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel bl = { 0,0,0,0 };

	double a = (double)inP->alpha * infoP->base_opacity;
	if (a < 0) a = 0; else if (a > PF_MAX_CHAN8) a = PF_MAX_CHAN8;
	if (a <= 0)
	{
		*outP = bl;
	}
	else if (a >= PF_MAX_CHAN8) {
		*outP = *inP;
	}
	else {
		outP->alpha = (A_u_char)a;
		outP->red = RoundByteDouble((double)inP->red * a / PF_MAX_CHAN8);
		outP->green = RoundByteDouble((double)inP->green * a / PF_MAX_CHAN8);
		outP->blue = RoundByteDouble((double)inP->blue * a / PF_MAX_CHAN8);
	}


	return err;
}

//==================================================================================
PF_Err Exec08(CAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData *in_data;
	in_data = ae->in_data;

	//baseの処理 Matにする
	if (infoP->base_enabled == TRUE) {
		ERR(ae->iterate8((refconType)infoP, BaseCopy8));
	}
	if ((infoP->paramset[0].enabled == FALSE)
		&& (infoP->paramset[1].enabled == FALSE)
		&& (infoP->paramset[2].enabled == FALSE)
		&& (infoP->paramset[3].enabled == FALSE)) {
	}
	else {
		//16bitの画像作成
		PF_EffectWorld world;
		AEFX_CLR_STRUCT(world);
		PF_EffectWorldPtr worldP = &world;

		//バッファーの作成
		ERR(ae->NewWorld(ae->output->width, ae->output->height, PF_PixelFormat_ARGB64,worldP));

		A_long w = ae->output->width;
		if (w < ae->output->height) w = ae->output->height;

		PF_Handle bufH = PF_NEW_HANDLE(sizeof(PF_Pixel16)*w * 2);
		if (bufH == NULL) return PF_Err_OUT_OF_MEMORY;



		ParamSetInfo si;
		for (A_long i = 0; i < PARAMSET_COUNT; i++)
		{
			AEFX_CLR_STRUCT(si);
			si = infoP->paramset[i];
			if (si.enabled == FALSE) continue;
			if (si.opacity <= 0) continue;
			//バッファーに転送
			ERR(World08To16(ae->input, worldP));
			//Matの処理
			ERR(ToPremultiply16(worldP));
			switch (si.extract_mode)
			{
			case EXTRACT_MODE::HI:
				ERR(ExtractHi16(worldP, si.border_hi, si.softness_hi));
				break;
			case EXTRACT_MODE::LO:
				ERR(ExtractLo16(worldP, si.border_lo, si.softness_lo));
				break;
			}
			if (si.brightness != 0)
			{
				ERR(Brightness16(worldP, si.brightness));
			}
			if ((si.minmax > 0) || (si.max > 0) || (si.blur > 0))
			{
				ERR(ToHarfSize16(worldP));
				if (si.minmax > 0)
				{
					ERR(Rev16(worldP));
					ERR(Max16(worldP, si.minmax, bufH));
					ERR(Rev16(worldP));
					ERR(Max16(worldP, si.minmax, bufH));
				}
				if (si.max > 0)
				{
					ERR(Max16(worldP, si.max, bufH));
				}
				if (si.blur > 0)
				{
					ERR(Blur16(ae,worldP, si.blur, bufH));
				}

				ERR(ToDoubleSize16(worldP, bufH));
			}

			//outへ転送 Blend処理
			ERR(Blend16To08(worldP, ae->output, si.blend_mode,si.opacity));

		}

		if (bufH != NULL)
		{
			PF_DISPOSE_HANDLE(bufH);
		}
		//バッファーを消す
		ERR(ae->DisposeWorld(worldP));
	}

	if (infoP->noise > 0) 
	{
		Noise08(ae->output, infoP->noise);
	}



	//Matから元に戻す
	if (infoP->alpha_on == TRUE)
	{
		ERR(AlphaON08(ae->output));
	}
	else
	{
		ERR(FromPremultiply08(ae->output));
	}

	return err;

}