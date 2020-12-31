//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ColorMatKey.h"


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
	PF_ADD_CHECKBOX(STR_ALPHA_REV,
		STR_ON,
		FALSE,
		0,
		ID_REV
	);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLED0,
		STR_ON,
		FALSE,
		0,
		ID_TARGET_ENABLE0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COLOR0,
		0xFF,
		0x00,
		0x00,
		ID_TARGET_COLOR0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_BORDER0,	//Name
		50,						//VALID_MIN
		100,						//VALID_MAX
		50,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_BORDER0
		);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_SOFT0,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		15,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_SOFT0
	);

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLED1,
		STR_ON,
		FALSE,
		0,
		ID_TARGET_ENABLE1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COLOR1,
		0x00,
		0xFF,
		0x00,
		ID_TARGET_COLOR1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_BORDER1,	//Name
		50,						//VALID_MIN
		100,						//VALID_MAX
		50,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_BORDER1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_SOFT1,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		15,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_SOFT1
	);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLED2,
		STR_ON,
		FALSE,
		0,
		ID_TARGET_ENABLE2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COLOR2,
		0x00,
		0x00,
		0xFF,
		ID_TARGET_COLOR2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_BORDER2,	//Name
		50,						//VALID_MIN
		100,						//VALID_MAX
		50,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_BORDER2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_SOFT2,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		15,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_SOFT2
	);
	//----------------------------------------------------------------
//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLED3,
		STR_ON,
		FALSE,
		0,
		ID_TARGET_ENABLE3
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COLOR3,
		0xFF,
		0x00,
		0xFF,
		ID_TARGET_COLOR3
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_BORDER3,	//Name
		50,						//VALID_MIN
		100,						//VALID_MAX
		50,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_BORDER3
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_SOFT3,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		15,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_SOFT3
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

//-------------------------------------------------------------------------------------------------
static PF_Err
ColorMatKey8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_InData *in_data;
	in_data = infoP->in_data;

	PF_FpShort r, g, b, a;
	a = (PF_FpShort)inP->alpha;
	r = (PF_FpShort)inP->red * (PF_FpShort)inP->alpha / PF_MAX_CHAN8;
	g = (PF_FpShort)inP->green * (PF_FpShort)inP->alpha / PF_MAX_CHAN8;
	b = (PF_FpShort)inP->blue * (PF_FpShort)inP->alpha / PF_MAX_CHAN8;

	PF_PixelFloat pf;
	pf.red = r / PF_MAX_CHAN8;
	pf.green = g / PF_MAX_CHAN8;
	pf.blue = b / PF_MAX_CHAN8;
	pf.alpha = a / PF_MAX_CHAN8;

	LABA lab = RgbToLab(pf);
	//色の距離測定
	double v = 0;
	double v2 = 0;

	for (A_long idx = 0; idx < 4; idx++) {
		if (infoP->ci[idx].target_enabled == TRUE) {
			v = 1.0 - (PF_SQRT(
				PF_POW(lab.L - infoP->ci[idx].target_lab.L, 2)
				+ PF_POW(lab.A - infoP->ci[idx].target_lab.A, 2)
				+ PF_POW(lab.B - infoP->ci[idx].target_lab.B, 2))
				/ PF_SQRT(3)
				);
			if (infoP->ci[idx].target_softness == 0) {
				if (v >= infoP->ci[idx].target_border) {
					v = 1.0;
				}
				else {
					v = 0;
				}
			}
			else {
				if (v <= infoP->ci[idx].target_start) {
					v = 0;
				}
				else if (v >= infoP ->ci[idx].target_border) {
					v = 1.0;
				}
				else {
					v = (v - infoP->ci[idx].target_start) / (infoP->ci[idx].target_softness);
				}

			}
			v2 += v;
			if (v2 > 1.0) v2 = 1.0;
		}
	}



	//------------




	*outP = *inP;
	a = PF_MAX_CHAN8 - a;
	outP->alpha = RoundByteLong((A_long)(v2*PF_MAX_CHAN8 + 0.5 - a));
	if (infoP->rev == TRUE) {
		outP->alpha = PF_MAX_CHAN8 - outP->alpha;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ColorMatKey16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_InData *in_data;
	in_data = infoP->in_data;

	PF_FpShort r, g, b, a;
	r = (PF_FpShort)inP->red * (PF_FpShort)inP->alpha / PF_MAX_CHAN16;
	g = (PF_FpShort)inP->green * (PF_FpShort)inP->alpha / PF_MAX_CHAN16;
	b = (PF_FpShort)inP->blue * (PF_FpShort)inP->alpha / PF_MAX_CHAN16;
	a = (PF_FpShort)inP->alpha;

	PF_PixelFloat pf;
	pf.red = r / PF_MAX_CHAN16;
	pf.green = g / PF_MAX_CHAN16;
	pf.blue = b / PF_MAX_CHAN16;
	pf.alpha = a / PF_MAX_CHAN16;

	LABA lab = RgbToLab(pf);
	//色の距離測定
	double v = 0;
	double v2 = 0;

	for (A_long idx = 0; idx < 4; idx++) {
		if (infoP->ci[idx].target_enabled == TRUE) {
			v = 1.0 - (PF_SQRT(
				PF_POW(lab.L - infoP->ci[idx].target_lab.L, 2)
				+ PF_POW(lab.A - infoP->ci[idx].target_lab.A, 2)
				+ PF_POW(lab.B - infoP->ci[idx].target_lab.B, 2))
				/ PF_SQRT(3)
				);
			if (infoP->ci[idx].target_softness == 0) {
				if (v >= infoP->ci[idx].target_border) {
					v = 1.0;
				}
				else {
					v = 0;
				}
			}
			else {
				if (v <= infoP->ci[idx].target_start) {
					v = 0;
				}
				else if (v >= infoP->ci[idx].target_border) {
					v = 1.0;
				}
				else {
					v = (v - infoP->ci[idx].target_start) / (infoP->ci[idx].target_softness);
				}

			}
			v2 += v;
			if (v2 > 1.0) v2 = 1.0;
		}
	}



	//------------




	*outP = *inP;
	a = PF_MAX_CHAN16 - a;
	outP->alpha = RoundShort((A_long)(v2*PF_MAX_CHAN16 + 0.5 - a));
	if (infoP->rev == TRUE) {
		outP->alpha = PF_MAX_CHAN16 - outP->alpha;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ColorMatKey32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_InData *in_data;
	in_data = infoP->in_data;

	PF_FpShort r, g, b, a;
	a = (PF_FpShort)inP->alpha;
	if (a < 0) a = 0; else if (a > 1) a = 1;
	r = (PF_FpShort)inP->red * a;
	g = (PF_FpShort)inP->green * a;
	b = (PF_FpShort)inP->blue * a;

	PF_PixelFloat pf;
	pf.red = r;
	pf.green = g;
	pf.blue = b;
	pf.alpha = a;

	LABA lab = RgbToLab(pf);
	//色の距離測定
	double v = 0;
	double v2 = 0;

	for (A_long idx = 0; idx < 4; idx++) {
		if (infoP->ci[idx].target_enabled == TRUE) {
			v = 1.0 - (PF_SQRT(
				PF_POW(lab.L - infoP->ci[idx].target_lab.L, 2)
				+ PF_POW(lab.A - infoP->ci[idx].target_lab.A, 2)
				+ PF_POW(lab.B - infoP->ci[idx].target_lab.B, 2))
				/ PF_SQRT(3)
				);
			if (infoP->ci[idx].target_softness == 0) {
				if (v >= infoP->ci[idx].target_border) {
					v = 1.0;
				}
				else {
					v = 0;
				}
			}
			else {
				if (v <= infoP->ci[idx].target_start) {
					v = 0;
				}
				else if (v >= infoP->ci[idx].target_border) {
					v = 1.0;
				}
				else {
					v = (v - infoP->ci[idx].target_start) / (infoP->ci[idx].target_softness);
				}

			}
			v2 += v;
			if (v2 > 1.0) v2 = 1.0;
		}
	}



	//------------




	*outP = *inP;
	outP->alpha = RoundFpShortDouble(v2 - (1 - a));
	if (infoP->rev == TRUE) {
		outP->alpha = (PF_FpShort)(1.0 - outP->alpha);
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_REV, &infoP->rev));


	A_long id = ID_TARGET_ENABLE0;
	double d;
	PF_Pixel c;
	PF_PixelFloat cf;
	for (A_long idx = 0; idx < 4; idx++) {

		ERR(ae->GetCHECKBOX(id, &infoP->ci[idx].target_enabled));
		id++;

		ERR(ae->GetCOLOR(id, &c));
		cf = CONV8TO32(c);
		infoP->ci[idx].target_lab = RgbToLab(cf);
		id++;

		ERR(ae->GetFLOAT(id, &d));
		d /= 100;
		if (d < 0) d = 0;
		else if (d > 1) d = 1;
		infoP->ci[idx].target_border = d;
		id++;

		ERR(ae->GetFLOAT(id, &d));
		d /= 100;
		if (d < 0) d = 0;
		else if (d > 1) d = 1;
		infoP->ci[idx].target_softness = d;
		id++;

		infoP->ci[idx].target_start = infoP->ci[idx].target_border - infoP->ci[idx].target_softness;
		if (infoP->ci[idx].target_start < 0) infoP->ci[idx].target_start = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	infoP->in_data = ae->in_data;

	if ((infoP->ci[0].target_enabled == FALSE)
		&& (infoP->ci[1].target_enabled == FALSE)
		&& (infoP->ci[2].target_enabled == FALSE)
		&& (infoP->ci[3].target_enabled == FALSE)
		) {
		return err;
	}

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP, ColorMatKey32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP, ColorMatKey16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,ColorMatKey8));
		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
/*
	SmartFXに対応していないホスト(After Effects7以前のもの)はこの関数が呼び出されて描画する
	この関数を書いておけば一応v6.5対応になる
*/
static PF_Err 
Render ( 
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ParamInfo info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;
}
//-----------------------------------------------------------------------------------
/*
	SmartFX対応の場合、まずこの関数が呼ばれてパラメータの獲得を行う
*/
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extraP,sizeof(ParamInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ae.SetHostPreRenderData();
			ERR(GetParams(&ae,infoP));
			ERR(ae.UnSetPreRenderData());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extraP,ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){
		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ERR(Exec(&ae,infoP));
			ERR2(ae.UnsetSmartRender());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif

#include "Fs_Entry.h"
