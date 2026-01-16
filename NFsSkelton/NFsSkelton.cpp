

#include "NFsSkelton.h"


// **********************************************************

//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = PF_MAX_CHAN8 * infoP->value;
	v = v - 2 * v * xorShiftDouble();

	outP->red = RoundByteFpLong((PF_FpLong)outP->red + v);
	outP->green = RoundByteFpLong((PF_FpLong)outP->green + v);
	outP->blue = RoundByteFpLong((PF_FpLong)outP->blue + v);

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

	PF_FpLong v = PF_MAX_CHAN16 * infoP->value;
	v = v - 2 * v * xorShiftDouble();

	outP->red = RoundShortFpLong((PF_FpLong)outP->red + v);
	outP->green = RoundShortFpLong((PF_FpLong)outP->green + v);
	outP->blue = RoundShortFpLong((PF_FpLong)outP->blue + v);


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

	PF_FpLong v = infoP->value;
	v = v - 2 * v * xorShiftDouble();

	outP->red = RoundFpShortDouble((PF_FpLong)outP->red + v);
	outP->green = RoundFpShortDouble((PF_FpLong)outP->green + v);
	outP->blue = RoundFpShortDouble((PF_FpLong)outP->blue + v);


	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************
PF_Err NFsSkelton::ParamsSetup(
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
	PF_ADD_FLOAT_SLIDER(STR_VALUE,	//Name
		0,						//VALID_MIN
		400,					//VALID_MAX
		0,						//SLIDER_MIN
		200,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_VALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CHECK,
		STR_ON,
		FALSE,
		0,
		ID_CHECK
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
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_POINT,			//"New Center"
	50,	// X
		50,	// Y
		0,	// Flag
		ID_POINT
		);
	*/

					
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err NFsSkelton::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetFLOAT(ID_VALUE, &infoP->value));
	if (!err)
	{
		infoP->value /= 100;
	}
	ERR(GetCHECKBOX(ID_CHECK, &infoP->check));
	return err;
};
// **********************************************************
PF_Err NFsSkelton::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFsWorld* src = new NFsWorld(input, in_data, pixelFormat());
	NFsSkeltonFX* dst = new NFsSkeltonFX(output, in_data, pixelFormat());
	dst->Copy(src);
	if (infoP->value > 0) {
		init_xorShift(frame());
		if (infoP->check == TRUE) {

			switch (pixelFormat())
			{
			case PF_PixelFormat_ARGB128:
				iterate32(src->world, (void*)infoP, NFsNoise32, dst->world);
				break;
			case PF_PixelFormat_ARGB64:
				iterate16(src->world, (void*)infoP, NFsNoise16, dst->world);
				break;
			case PF_PixelFormat_ARGB32:
				iterate8(src->world, (void*)infoP, NFsNoise8, dst->world);
				break;
			default:
				break;
			}
		}
		else {
			dst->Noise(infoP);
		}
	}


	delete src;
	delete dst;

	return err;

};
// **********************************************************

