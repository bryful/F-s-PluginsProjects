

#include "Unmult_KNSW_Fake.h"


// **********************************************************

//-------------------------------------------------------------------------------------------------
static PF_Err 
UnMult8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;

	if (outP->alpha < PF_MAX_CHAN8) {
		outP->red = RoundByteLong(outP->red * outP->alpha / PF_MAX_CHAN8);
		outP->green = RoundByteLong(outP->green * outP->alpha / PF_MAX_CHAN8);
		outP->blue = RoundByteLong(outP->blue * outP->alpha / PF_MAX_CHAN8);
	}
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
UnMult16(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	if (outP->alpha < PF_MAX_CHAN16) {
		outP->red = RoundShort(outP->red * outP->alpha / PF_MAX_CHAN16);
		outP->green = RoundShort(outP->green * outP->alpha / PF_MAX_CHAN16);
		outP->blue = RoundShort(outP->blue * outP->alpha / PF_MAX_CHAN16);
	}
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
UnMult32(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;

	if (outP->alpha < 1.0) {
		outP->red = RoundFpShort(outP->red * outP->alpha);
		outP->green = RoundFpShort(outP->green * outP->alpha);
		outP->blue = RoundFpShort(outP->blue * outP->alpha);
	}
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));

	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************
PF_Err Unmult_KNSW_Fake::ParamsSetup(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP)
{
	PF_Err err = PF_Err_NONE;
	//Init();
	//m_cmd = PF_Cmd_PARAMS_SETUP;
	//in_data = in_dataP;
	//out_data = out_dataP;
	//PF_ParamDef		def;
	out_dataP->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err Unmult_KNSW_Fake::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	return err;
};
// **********************************************************
PF_Err Unmult_KNSW_Fake::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFsWorld* src = new NFsWorld(input, in_data, pixelFormat());
	NFsWorld* dst = new NFsWorld(output, in_data, pixelFormat());
	dst->Copy(src);

	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)infoP, UnMult32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)infoP, UnMult16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, UnMult8, dst->world);
		break;
	default:
		break;
	}


	delete src;
	delete dst;

	return err;

};
// **********************************************************

