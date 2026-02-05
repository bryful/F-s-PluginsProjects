

#include "ColorChangeFromPoint.h"


// **********************************************************
//-------------------------------------------------------------------------------------------------
static PF_Err
AlphaFill8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	outP->alpha = PF_MAX_CHAN8;

	double op = ((double)inP->alpha / PF_MAX_CHAN8);

	outP->blue = RoundByteDouble(  (double)infoP->dst8.blue * (1 - op) + (double)inP->blue * op +0.5);
	outP->green = RoundByteDouble((double)infoP->dst8.green * (1 - op) + (double)inP->green * op + 0.5);
	outP->red = RoundByteDouble((double)infoP->dst8.red * (1 - op) + (double)inP->red * op + 0.5);


	return err;
}
static PF_Err
AlphaFill16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	outP->alpha = PF_MAX_CHAN16;

	double op = ((double)inP->alpha / PF_MAX_CHAN16);

	outP->blue = RoundShortFpLong((double)infoP->dst16.blue * (1 - op) + (double)inP->blue * op + 0.5);
	outP->green = RoundShortFpLong((double)infoP->dst16.green * (1 - op) + (double)inP->green * op + 0.5);
	outP->red = RoundShortFpLong((double)infoP->dst16.red * (1 - op) + (double)inP->red * op + 0.5);


	return err;
}
static PF_Err
AlphaFill32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	outP->alpha = 1.0;

	double op = (double)inP->alpha;

	outP->blue = RoundFpShortDouble((double)infoP->dst32.blue * (1 - op) + (double)inP->blue * op);
	outP->green = RoundFpShortDouble((double)infoP->dst32.green * (1 - op) + (double)inP->green * op);
	outP->red = RoundFpShortDouble((double)infoP->dst32.red * (1 - op) + (double)inP->red * op);


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ColorChange8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel t = *inP;
	if ((t.red == infoP->src.red)
		&& (t.green == infoP->src.green)
		&& (t.blue == infoP->src.blue))
	{
		*outP = infoP->dst8;
	}
	else {
		*outP = *inP;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ColorChange16(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel t = CONV16TO8(*inP);
	if ((t.red == infoP->src.red)
		&& (t.green == infoP->src.green)
		&& (t.blue == infoP->src.blue))
	{
		*outP = infoP->dst16;
	}
	else {
		*outP = *inP;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ColorChange32(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel t = CONV32TO8(*inP);
	if ((t.red == infoP->src.red)
		&& (t.green == infoP->src.green)
		&& (t.blue == infoP->src.blue))
	{
		*outP = infoP->dst32;
	}
	else {
		*outP = *inP;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************
PF_Err ColorChangeFromPoint::ParamsSetup(
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
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_POINT,			/*"New Center"*/
		50,	// X
		50,	// Y
		0,	// Flag
		ID_POINT
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(STR_COLOR,
		0xFF,
		0x00,
		0x00,
		ID_COLOR
	);	
	//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err ColorChangeFromPoint::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_FixedPoint p;
	infoP->pointX = -1;
	infoP->pointY = -1;
	infoP->mode = MODE_NONE;
	ERR(GetPOINT_FIXED(ID_POINT, &p));
	if (!err)
	{
		infoP->pointX  = (A_long)((A_FpLong)p.x /65536.0 + 0.5);
		infoP->pointY = (A_long)((A_FpLong)p.y / 65536.0 + 0.5);
	}
	infoP->dst8 = { 0,0,0,0 };
	ERR(GetCOLOR(ID_COLOR, &infoP->dst8));
	if (!err)
	{
		infoP->dst16 = CONV8TO16(infoP->dst8);
		infoP->dst32 = CONV8TO32(infoP->dst8);
	}

	return err;
};

// **************************************************************
PF_Err GetTargetColor(NFsWorld *w, ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;

	infoP->mode = MODE_NONE;
	// Area Over
	if ((infoP->pointX < 0) || (infoP->pointY < 0) || (infoP->pointX >= w->width()) || (infoP->pointY >= w->height()))
	{
		return err;
	}
	infoP->src = w->GetPixelValue8(infoP->pointX, infoP->pointY);

	if (infoP->src.alpha == 0) {
		infoP->mode = MODE_ALPHA_ZERO;
	}
	else {
		infoP->mode = MODE_NORMAL;
	}
	return err;
}
// **********************************************************
PF_Err ColorChangeFromPoint::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFsWorld* src = new NFsWorld(input, in_data, pixelFormat());
	NFsWorld* dst = new NFsWorld(output, in_data, pixelFormat());
	
	GetTargetColor(src,infoP);

	dst->Copy(src);
	switch (infoP->mode)
	{
	case MODE_NORMAL:
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			iterate32(src->world, (void*)infoP, ColorChange32, dst->world);
			break;
		case PF_PixelFormat_ARGB64:
			iterate16(src->world, (void*)infoP, ColorChange16, dst->world);
			break;
		case PF_PixelFormat_ARGB32:
			iterate8(src->world, (void*)infoP, ColorChange8, dst->world);
			break;
		default:
			break;
		}
		break;
	case MODE_ALPHA_ZERO:
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			iterate32(src->world, (void*)infoP, AlphaFill32, dst->world);
			break;
		case PF_PixelFormat_ARGB64:
			iterate16(src->world, (void*)infoP, AlphaFill16, dst->world);
			break;
		case PF_PixelFormat_ARGB32:
			iterate8(src->world, (void*)infoP, AlphaFill8, dst->world);
			break;
		default:
			break;
		}
		break;
	case MODE_NONE:
	default:
		break;
	}


	delete src;
	delete dst;

	return err;

};
// **********************************************************

