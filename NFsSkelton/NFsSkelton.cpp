

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
