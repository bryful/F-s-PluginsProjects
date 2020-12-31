//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "MaskFromRGB.h"

static PF_Err (*func8)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_Pixel8	*inP, 
			PF_Pixel8	*outP);

static PF_Err (*func16)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_Pixel16	*inP, 
			PF_Pixel16	*outP);

static PF_Err (*func32)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_PixelFloat 	*inP, 
			PF_PixelFloat	*outP);
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
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_POPUP(		STR_POPUP, 
						STR_POPUP_COUNT,	//メニューの数
						STR_POPUP_DFLT,	//デフォルト
						STR_POPUP_ITEMS,
						ID_POPUP
						);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LEVEL,	//Name
						0,						//VALID_MIN
						5000,					//VALID_MAX
						50,						//SLIDER_MIN
						300,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						100,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_LEVEL
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
	PF_Err				err					= PF_Err_NONE;

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
	PF_Err 	err 	= PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Mask8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel col = *outP;
	A_u_char v = 0;
	if (col.alpha == 0){
		v		= 0;
	}else{
		if ((niP->mode >= M_MODE::YELLOW)&&(niP->mode < M_MODE::MAX)){
			col.blue	= RoundByteFpLong(PF_MAX_CHAN8 - col.blue);
			col.green	= RoundByteFpLong(PF_MAX_CHAN8 - col.green);
			col.red		= RoundByteFpLong(PF_MAX_CHAN8 - col.red);
		}
		if (col.alpha <PF_MAX_CHAN8){
			double a = (double)col.alpha / PF_MAX_CHAN8;
			col.red		= RoundByteFpLong( (double)col.red * a); 
			col.green	= RoundByteFpLong( (double)col.green * a); 
			col.blue	= RoundByteFpLong( (double)col.blue * a); 
		}
		switch(niP->mode){
		case M_MODE::RED:
		case M_MODE::CYAN:
			v =  RoundByteFpLong( (A_long)col.red - MAX((A_long)col.green,(A_long)col.blue));
			break;
		case M_MODE::GREEN:
		case M_MODE::MAGENTA:
			v =  RoundByteFpLong( (A_long)col.green - MAX((A_long)col.blue,(A_long)col.red));
			break;
		case M_MODE::BLUE:
		case M_MODE::YELLOW:
			v =  RoundByteFpLong( (A_long)col.blue - MAX((A_long)col.red,(A_long)col.green));
			break;
		case M_MODE::MAX:
			v = MAX((A_long)col.blue,MAX((A_long)col.red,(A_long)col.green));
			break;
		}
		v = RoundByteFpLong((double)v * niP->col_level);
	}

	col.alpha = v;
	col.red		= PF_MAX_CHAN8;
	col.green	= PF_MAX_CHAN8;
	col.blue	= PF_MAX_CHAN8;
	
	*outP = col;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Mask16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 col = *outP;
	A_u_short v = 0;
	if (col.alpha == 0){
		v		= 0;
	}else{
		if ((niP->mode >= M_MODE::YELLOW)&&(niP->mode < M_MODE::MAX)){
			col.blue	= RoundShortFpLong(PF_MAX_CHAN16 - col.blue);
			col.green	= RoundShortFpLong(PF_MAX_CHAN16 - col.green);
			col.red		= RoundShortFpLong(PF_MAX_CHAN16 - col.red);
		}
		if (col.alpha <PF_MAX_CHAN16){
			double a = (double)col.alpha / PF_MAX_CHAN16;
			col.red		= RoundShortFpLong( (double)col.red * a); 
			col.green	= RoundShortFpLong( (double)col.green * a); 
			col.blue	= RoundShortFpLong( (double)col.blue * a); 
		}
		switch(niP->mode){
		case M_MODE::RED:
		case M_MODE::CYAN:
			v =  RoundShortFpLong( (A_long)col.red - MAX((A_long)col.green,(A_long)col.blue));
			break;
		case M_MODE::GREEN:
		case M_MODE::MAGENTA:
			v =  RoundShortFpLong( (A_long)col.green - MAX((A_long)col.red,(A_long)col.blue));
			break;
		case M_MODE::BLUE:
		case M_MODE::YELLOW:
			v =  RoundShortFpLong( (A_long)col.blue - MAX((A_long)col.red,(A_long)col.green));
			break;
		case M_MODE::MAX:
			v = MAX((A_long)col.blue,MAX((A_long)col.red,(A_long)col.green));
			break;
		}
		v = RoundShortFpLong((double)v * niP->col_level);
	}

	col.alpha = v;
	col.red		= PF_MAX_CHAN16;
	col.green	= PF_MAX_CHAN16;
	col.blue	= PF_MAX_CHAN16;
	
	*outP = col;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Mask32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat col = *outP;
	PF_FpShort v = 0;
	if (col.alpha == 0){
		v		= 0;
	}else{
		if (col.blue>1.0) col.blue = 1.0;
		if (col.green>1.0) col.green = 1.0;
		if (col.red>1.0) col.red = 1.0;

		if ((niP->mode >= M_MODE::YELLOW)&&(niP->mode < M_MODE::MAX)){
			col.blue	= RoundFpShortDouble(1.0 - col.blue);
			col.green	= RoundFpShortDouble(1.0 - col.green);
			col.red		= RoundFpShortDouble(1.0 - col.red);
		}
		if (col.alpha <1.0){
			double a = (double)col.alpha;
			col.red		= RoundFpShortDouble( (double)col.red * a); 
			col.green	= RoundFpShortDouble( (double)col.green * a); 
			col.blue	= RoundFpShortDouble( (double)col.blue * a); 
		}
		switch(niP->mode){
		case M_MODE::RED:
		case M_MODE::CYAN:
			v =  RoundFpShortDouble( col.red - MAX(col.blue  , col.green));
			break;
		case M_MODE::GREEN:
		case M_MODE::MAGENTA:
			v =  RoundFpShortDouble( col.green - MAX(col.blue  , col.red));
			break;
		case M_MODE::BLUE:
		case M_MODE::YELLOW:
			v =  RoundFpShortDouble( col.blue - MAX(col.green  , col.red));
			break;
		case M_MODE::MAX:
			v = MAX(col.blue,MAX(col.red,col.green));
			break;
		}
		v = RoundFpShortDouble((double)v * niP->col_level);
	}

	col.alpha = v;
	col.red		= 1;
	col.green	= 1;
	col.blue	= 1;
	
	*outP = col;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	ERR(ae->GetPOPUP(ID_POPUP,&infoP->mode));
	PF_FpLong d = 100;
	ERR(ae->GetFLOAT(ID_LEVEL,&d));
	if(!err){
		infoP->col_level = d/100;
	}else{
		infoP->col_level = 1.0;
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

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,Mask32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,Mask16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,Mask8));
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
