//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "YuvControl.h"


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
	PF_ADD_FLOAT_SLIDER(STR_VALUE,	//Name
						-200,					//VALID_MIN
						200,					//VALID_MAX
						-200,					//SLIDER_MIN
						200,						//LIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_VALUE
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
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel bl = { 0,0,0,0 };

	if (outP->alpha == 0) {
		*outP = bl;
		return err;
	}

	double a = (double)outP->alpha / PF_MAX_CHAN8;

	//Mat
	double r = (double)outP->red * a / PF_MAX_CHAN8;
	double g = (double)outP->green * a / PF_MAX_CHAN8;
	double b = (double)outP->blue * a / PF_MAX_CHAN8;

	/*
	Y =  0.299R + 0.587G + 0.114B
	U = -0.169R - 0.331G + 0.500B
	V =  0.500R - 0.419G - 0.081B
	YUV to RGB

	R = 1.000Y          + 1.402V
	G = 1.000Y - 0.344U - 0.714V
	B = 1.000Y + 1.772U

	Y   =  0.300R + 0.590G + 0.110B
	B-Y = -0.300R - 0.590G + 0.890B
	R-Y =  0.700R - 0.590G - 0.110B
	Y,B-Y,R-Y to RGB

	R = 1.000Y              + 1.000(R-Y)
	G = 1.000Y - 0.186(B-Y) - 0.508(R-Y)
	B = 1.000Y + 1.000(B-Y)

	*/
	//yuv
	double y = (r *  0.299) +  (0.587 * g) + (0.114 * b);
	double u = (r * -0.169) -  (0.331 * g) + (0.500 * b);
	double v = (r *  0.500) -  (0.419 * g) - (0.081 * b);

	if (infoP->y > 0) {
		if (y < 1) {
			y = y + (1 - y)*infoP->y;
		}
	}
	else if (infoP->y == 0) {}
	else {
		y = y * (1+ infoP->y);
	}
	//rgb
	r = y + 1.402*v;
	g = y - 0.344*u - 0.714*v;
	b = y + 1.772*u;

	r = PF_MAX_CHAN8 * r / a;
	g = PF_MAX_CHAN8 * g / a;
	b = PF_MAX_CHAN8 * b / a;
	A_long rr = (A_long)(r + 0.5);
	A_long gg = (A_long)(g + 0.5);
	A_long bb = (A_long)(b + 0.5);

	if (rr < 0)rr = 0; else if (rr > PF_MAX_CHAN8) rr = PF_MAX_CHAN8;
	if (gg < 0)gg = 0; else if (gg > PF_MAX_CHAN8) gg = PF_MAX_CHAN8;
	if (bb < 0)bb = 0; else if (bb > PF_MAX_CHAN8) bb = PF_MAX_CHAN8;

	outP->red = (A_u_char)rr;
	outP->green = (A_u_char)gg;
	outP->blue = (A_u_char)bb;
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


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_VALUE,&infoP->y));
	infoP->y /= 100;
	//if (infoP->y < -1) { infoP->y = -1; }
	//else if (infoP->y > 1)infoP->y = 1;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if (infoP->y == 0) return err;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		//ERR(ae->iterate16((refconType)infoP,FilterImage16));
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
