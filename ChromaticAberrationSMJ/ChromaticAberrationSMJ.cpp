

#include "ChromaticAberrationSMJ.h"


// **********************************************************

//-------------------------------------------------------------------------------------------------
static PF_Err 
CA8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);


	PF_Pixel p = infoP->inIn->GetPixD8((PF_FpLong)xL + 20.5, (PF_FpLong)yL + 20.5);

	*outP = p;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise16(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise32(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);


	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************
PF_Err ChromaticAberrationSMJ::ParamsSetup(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP)
{
	PF_Err err = PF_Err_NONE;
	Init();
	m_cmd = PF_Cmd_PARAMS_SETUP;
	in_data = in_dataP;
	out_data = out_dataP;
	PF_ParamDef		def;
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_R_SCALE,	//Name
		50,						//VALID_MIN
		150,					//VALID_MAX
		80,						//SLIDER_MIN
		120,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_R_SCALE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_G_SCALE,	//Name
		50,						//VALID_MIN
		150,					//VALID_MAX
		80,						//SLIDER_MIN
		120,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_G_SCALE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_B_SCALE,	//Name
		50,						//VALID_MIN
		150,					//VALID_MAX
		80,						//SLIDER_MIN
		120,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_B_SCALE
	);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_CENTER,		//"New Center"
		50,	// X
		50,	// Y
		0,	// Flag
		ID_CENTER
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_H_OFFSET,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_H_OFFSET
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_V_OFFSET,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_V_OFFSET
	);
	/*
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(	STR_NOISE_OFFSET,	//パラメータの名前
					-30000, 		//数値入力する場合の最小値
					30000,			//数値入力する場合の最大値
					0,				//スライダーの最小値
					300,			//スライダーの最大値
					10,				//デフォルトの値
					ID_NOISE_OFFSET
					);	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_HIDDEN_ON1,
					STR_HIDDEN_ON2,
					FALSE,
					0,
					ID_HIDDEN_ON
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_R,	//Name
						-2,						//VALID_MIN
						2,						//VALID_MAX
						-1,						//SLIDER_MIN
						1,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_R
						);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_NOISE_FRAME1,
					STR_NOISE_FRAME2,
					TRUE,
					0,
					ID_NOISE_FRAME
					);
	//----------------------------------------------------------------
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
		PF_ADD_TOPIC(STR_TOPIC, ID_TOPIC);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COLOR,
					0xFF,
					0xFF,
					0xFF,
					ID_COLOR
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FIXED(	STR_FIXED_SLIDER,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値
					100,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ
					0,
					0,
					ID_FIXED_SLIDER
					);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ANGLE,0,ID_ANGLE);
	//----------------------------------------------------------------

	*/

					
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err ChromaticAberrationSMJ::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetFLOAT(ID_R_SCALE, &infoP->rscale));
	if (!err)
	{
		infoP->rscale /= 100;
	}
	ERR(GetFLOAT(ID_G_SCALE, &infoP->gscale));
	if (!err)
	{
		infoP->gscale /= 100;
	}
	ERR(GetFLOAT(ID_B_SCALE, &infoP->bscale));
	if (!err)
	{
		infoP->bscale /= 100;
	}
	PF_FixedPoint fp;
	ERR(GetPOINT_FIXED(ID_CENTER, &fp));
	if (!err)
	{
		infoP->center.x = (A_FpLong)((A_FpLong)fp.x / 65536);
		infoP->center.y = (A_FpLong)((A_FpLong)fp.y / 65536);
	}
	ERR(GetFLOAT(ID_H_OFFSET, &infoP->hoffset));
	if (!err)
	{
		infoP->hoffset /= 100;
	}
	ERR(GetFLOAT(ID_V_OFFSET, &infoP->voffset));
	if (!err)
	{
		infoP->voffset /= 100;
	}
	return err;
};
// **********************************************************
PF_Err ChromaticAberrationSMJ::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFsWorld* src = new NFsWorld(input, in_data, pixelFormat());
	ChromaticAberrationSMJFX* dst = new ChromaticAberrationSMJFX(output, in_data, pixelFormat());
	infoP->inIn = src;
	dst->Copy(src);
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//iterate32(src->world, (void*)infoP, NFsNoise32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		//iterate16(src->world, (void*)infoP, NFsNoise16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, CA8, dst->world);
		break;
	default:
		break;
	}

	delete src;
	delete dst;

	return err;

};
// **********************************************************
