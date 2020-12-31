//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Filter.h"
#include <stdio.h>
#include "Filter_def.h"


//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_BASE_ENABLED,
		STR_ON,
		TRUE,
		0,
		ID_BASE_ENABLED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_BASE_OPACITY,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						100,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						100,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_BASE_OPACITY
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FILTER_OPACITY,	//Name
		0,						//VALID_MIN
		300,						//VALID_MAX
		0,						//SLIDER_MIN
		150,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FILTER_OPACITY
	);
	//----------------------------------------------------------------
#pragma region paramset
	char num[255] = { "\0" };
	for (int i = 0; i < PARAMSET_COUNT; i++)
	{
#pragma region topic
		SPRINTF(STR_TOPIC, i + 1);
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_START_COLLAPSED;
		PF_ADD_TOPIC(num, ID_NUM(i,ID_TOPIC));
#pragma endregion
		//----------------------------------------------------------------

#pragma region enabled
		//enabled
		SPRINTF(STR_ENABLED, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FLT_Enabled[i],
			0,
			ID_NUM(i, ID_BASE_ENABLED)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region extract
		SPRINTF(STR_EXTRACT, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_EXTRACT_COUNT,	//メニューの数
			FLT_EXTRACT_MODE[i],	//デフォルト
			STR_EXTRACT_ITEMS,
			ID_NUM(i, ID_EXTRACT)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region border_hi
		SPRINTF(STR_BORDER_HI, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			50,						//VALID_MIN
			100,					//VALID_MAX
			75,						//SLIDER_MIN
			100,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FLT_BORDER_HI[i],		//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			ID_NUM(i, ID_BORDER_HI)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region softness_hi
		SPRINTF(STR_SOFTNESS_HI, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			0,						//VALID_MIN
			50,					//VALID_MAX
			0,						//SLIDER_MIN
			50,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FLT_SOFTNESS_HI[i],		//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			ID_NUM(i, ID_SOFTNESS_HI)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region border_lo
		SPRINTF(STR_BORDER_LO, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			0,						//VALID_MIN
			50,					//VALID_MAX
			0,						//SLIDER_MIN
			30,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FLT_BORDER_LO[i],		//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			ID_NUM(i, ID_BORDER_LO)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region softness_lo
		SPRINTF(STR_SOFTNESS_LO, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			0,						//VALID_MIN
			30,					//VALID_MAX
			0,						//SLIDER_MIN
			30,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FLT_SOFTNESS_LO[i],		//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			ID_NUM(i, ID_SOFTNESS_LO)
		);
#pragma endregion
#pragma region brightness
		SPRINTF(STR_BRIGHT, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			-50,				//VALID_MIN
			50,					//VALID_MAX
			-20,				//SLIDER_MIN
			20,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FLT_BRIGHTNESS[i],		//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			ID_NUM(i, ID_SOFTNESS_LO)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region minmax
		SPRINTF(STR_MINMAX, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 			//数値入力する場合の最小値
			10,			//数値入力する場合の最大値
			0,				//スライダーの最小値 
			5,			//スライダーの最大値
			FLT_MINMAX[i],	//デフォルトの値
			ID_NUM(i, ID_MINMAX)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region max
		SPRINTF(STR_MAX, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 					//数値入力する場合の最小値
			50,				//数値入力する場合の最大値
			0,					//スライダーの最小値 
			20,				//スライダーの最大値
			FLT_MAX[i],			//デフォルトの値
			ID_NUM(i, ID_MAX)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region blur
		SPRINTF(STR_BLUR, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 					//数値入力する場合の最小値
			200,				//数値入力する場合の最大値
			0,					//スライダーの最小値 
			50,				//スライダーの最大値
			FLT_BLUR[i],		//デフォルトの値
			ID_NUM(i, ID_BLUR)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region opacity
		SPRINTF(STR_OPACITY, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			0,						//VALID_MIN
			100,					//VALID_MAX
			0,						//SLIDER_MIN
			100,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FLT_OPACITY[i],			//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			ID_NUM(i, ID_OPACITY)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region blend
		SPRINTF(STR_BLEND, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_BLEND_COUNT,	//メニューの数
			FLT_BLEND_MODE[i],	//デフォルト
			STR_BLEND_ITEMS,
			ID_NUM(i, ID_BLEND)
		);
#pragma endregion
		//----------------------------------------------------------------

#pragma region topic_end
		AEFX_CLR_STRUCT(def);
		PF_END_TOPIC(ID_NUM(i, ID_TOPIC_END));
#pragma endregion
		//----------------------------------------------------------------

	}
#pragma endregion

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_NOISE,	//Name
		0,							//VALID_MIN
		100,						//VALID_MAX
		0,							//SLIDER_MIN
		5,							//SLIDER_MAX
		1,							//CURVE_TOLERANCE
		0,							//DFLT
		1,							//PREC
		0,							//DISP
		0,							//WANT_PHASE
		ID_NOISE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_ALPHA_ON,
		STR_ON,
		FALSE,
		0,
		ID_ALPHA_ON
	);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
HandleChangedParam(
	PF_InData					*in_data,
	PF_OutData					*out_data,
	PF_ParamDef					*params[],
	PF_LayerDef					*outputP,
	PF_UserChangedParamExtra	*extraP)
{
	PF_Err				err = PF_Err_NONE;
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err
QueryDynamicFlags(	
	PF_InData		*in_data,	
	PF_OutData		*out_data,	
	PF_ParamDef		*params[],	
	void			*extra)	
{
	PF_Err 	err = PF_Err_NONE;
	return err;
}
//=======================================================================================
static
PF_Err
FrameSetup(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	return PF_Err_NONE;
}


//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_BASE_ENABLED, &infoP->base_enabled));
	ERR(ae->GetFLOAT(ID_BASE_OPACITY, &infoP->base_opacity));
	infoP->base_opacity /= 100;

	ERR(ae->GetFLOAT(ID_FILTER_OPACITY, &infoP->filter_opacity));
	infoP->filter_opacity /= 100;



	for (int i = 0; i < PARAMSET_COUNT; i++)
	{
		ERR(ae->GetCHECKBOX(ID_NUM(i,ID_BASE_ENABLED), &infoP->paramset[i].enabled));
		ERR(ae->GetPOPUP(ID_NUM(i, ID_EXTRACT), &infoP->paramset[i].extract_mode));
		ERR(ae->GetFLOAT(ID_NUM(i, ID_BORDER_HI), &infoP->paramset[i].border_hi));
		infoP->paramset[i].border_hi /= 100;
		ERR(ae->GetFLOAT(ID_NUM(i, ID_SOFTNESS_HI), &infoP->paramset[i].softness_hi));
		infoP->paramset[i].softness_hi /= 100;
		ERR(ae->GetFLOAT(ID_NUM(i, ID_BORDER_LO), &infoP->paramset[i].border_lo));
		infoP->paramset[i].border_lo /= 100;
		ERR(ae->GetFLOAT(ID_NUM(i, ID_SOFTNESS_LO), &infoP->paramset[i].softness_lo));
		infoP->paramset[i].softness_lo /= 100;
		ERR(ae->GetFLOAT(ID_NUM(i, ID_BRIGHT), &infoP->paramset[i].brightness));
		infoP->paramset[i].brightness /= 100;


		ERR(ae->GetADD(ID_NUM(i, ID_MINMAX), &infoP->paramset[i].minmax));
		infoP->paramset[i].minmax = ae->downScaleNoClip(infoP->paramset[i].minmax);
		ERR(ae->GetADD(ID_NUM(i, ID_MAX), &infoP->paramset[i].max));
		infoP->paramset[i].max = ae->downScaleNoClip(infoP->paramset[i].max);
		ERR(ae->GetADD(ID_NUM(i, ID_BLUR), &infoP->paramset[i].blur));
		infoP->paramset[i].blur = ae->downScaleNoClip(infoP->paramset[i].blur);

		ERR(ae->GetFLOAT(ID_NUM(i, ID_OPACITY), &infoP->paramset[i].opacity));
		infoP->paramset[i].opacity /= 100;
		infoP->paramset[i].opacity *= infoP->filter_opacity;
		if (infoP->paramset[i].opacity < 0) infoP->paramset[i].opacity = 0;
		else if (infoP->paramset[i].opacity > 1) infoP->paramset[i].opacity = 1;

		ERR(ae->GetPOPUP(ID_NUM(i, ID_BLEND), &infoP->paramset[i].blend_mode));

	}
	ERR(ae->GetFLOAT(ID_NOISE, &infoP->noise));
	infoP->noise /= 100;
	ERR(ae->GetCHECKBOX(ID_ALPHA_ON, &infoP->alpha_on));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	FsSrand(ae->frame());

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		break;
	case PF_PixelFormat_ARGB64:
		ERR(Exec16(ae, infoP));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(Exec08(ae, infoP));
		break;
	}
	return err;
}
#include "Filter_Entry.h"
