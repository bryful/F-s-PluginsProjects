

#include "NFsSkelton.h"


// **********************************************************

//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;

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
	PF_ParamDef		def;
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_VALUE,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
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
	ERR(GetCHECKBOX(ID_CHECK, &infoP->check));
	return err;
};
// **********************************************************
PF_Err NFsSkelton::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	return err;

};
// **********************************************************
