//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "whiteInOut.h"


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
	PF_ADD_FLOAT_SLIDER(STR_Y,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						100,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_Y
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
FilterImage8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP->value >= 1.0) return err;

	PF_FpLong r = 1.0, g = 1.0, b = 1.0;
	PF_FpLong ra = 1.0, ga = 1.0, ba = 1.0;
	A_long rr, gg, bb;
	rr = gg = bb = PF_MAX_CHAN8;

	if (niP->value > 0) {
		r = (PF_FpLong)outP->red   / PF_MAX_CHAN8;
		g = (PF_FpLong)outP->green / PF_MAX_CHAN8;
		b = (PF_FpLong)outP->blue  / PF_MAX_CHAN8;

		ra = (1.0 - r)*OFFSET_VALUE * (1.0 - niP->value);
		ga = (1.0 - g)*OFFSET_VALUE * (1.0 - niP->value);
		ba = (1.0 - b)*OFFSET_VALUE * (1.0 - niP->value);

		r += ra;
		g += ga;
		b += ba;

		r /= niP->value;
		g /= niP->value;
		b /= niP->value;
		rr = (A_long)(r*PF_MAX_CHAN8 + 0.5);
		gg = (A_long)(g*PF_MAX_CHAN8 + 0.5);
		bb = (A_long)(b*PF_MAX_CHAN8 + 0.5);
		if (rr > PF_MAX_CHAN8) rr = PF_MAX_CHAN8;
		if (gg > PF_MAX_CHAN8) gg = PF_MAX_CHAN8;
		if (bb > PF_MAX_CHAN8) bb = PF_MAX_CHAN8;
	}

	outP->red   = (A_u_char)rr;
	outP->green = (A_u_char)gg;
	outP->blue  = (A_u_char)bb;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	if (niP->value >= 1.0) return err;

	PF_FpLong r = 1.0, g = 1.0, b = 1.0;
	PF_FpLong ra = 1.0, ga = 1.0, ba = 1.0;
	A_long rr, gg, bb;
	rr = gg = bb = PF_MAX_CHAN16;

	if (niP->value > 0) {
		r = (PF_FpLong)outP->red / PF_MAX_CHAN16;
		g = (PF_FpLong)outP->green / PF_MAX_CHAN16;
		b = (PF_FpLong)outP->blue / PF_MAX_CHAN16;

		ra = (1.0 - r)*OFFSET_VALUE * (1.0 - niP->value);
		ga = (1.0 - g)*OFFSET_VALUE * (1.0 - niP->value);
		ba = (1.0 - b)*OFFSET_VALUE * (1.0 - niP->value);

		r += ra;
		g += ga;
		b += ba;

		r /= niP->value;
		g /= niP->value;
		b /= niP->value;

		rr = (A_long)(r*PF_MAX_CHAN16 + 0.5);
		gg = (A_long)(g*PF_MAX_CHAN16 + 0.5);
		bb = (A_long)(b*PF_MAX_CHAN16 + 0.5);
		if (rr > PF_MAX_CHAN16) rr = PF_MAX_CHAN16;
		if (gg > PF_MAX_CHAN16) gg = PF_MAX_CHAN16;
		if (bb > PF_MAX_CHAN16) bb = PF_MAX_CHAN16;
	}

	outP->red = (A_u_short)rr;
	outP->green = (A_u_short)gg;
	outP->blue = (A_u_short)bb;



	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	if (niP->value >= 1.0) return err;

	PF_FpLong v = niP->value;


	PF_FpLong r = 1.0, g = 1.0, b = 1.0;
	PF_FpLong rx, gx, bx;

	rx = r = (PF_FpLong)outP->red;
	gx = g = (PF_FpLong)outP->green;
	bx = b = (PF_FpLong)outP->blue;

	if (rx > 1.0) rx = 1.0;
	if (gx > 1.0) gx = 1.0;
	if (bx > 1.0) bx = 1.0;


	PF_FpLong ra = 1.0, ga = 1.0, ba = 1.0;

	ra = (1.0 - rx)*OFFSET_VALUE * (1.0 - v);
	ga = (1.0 - gx)*OFFSET_VALUE * (1.0 - v);
	ba = (1.0 - bx)*OFFSET_VALUE * (1.0 - v);

	r += ra;
	g += ga;
	b += ba;

	if (v <= 0) v = 0.001;

	r /= v;
	g /= v;
	b /= v;

	outP->red = (PF_FpShort)r;
	outP->green = (PF_FpShort)g;
	outP->blue = (PF_FpShort)b;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_Y,&infoP->value));
	infoP->value /= 100;
	if (infoP->value < 0) infoP->value = 0;
	else if (infoP->value > 1.0) infoP->value = 1.0;
	infoP->value = 1.0 - infoP->value;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if (infoP->value >= 1.0) return err;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
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
