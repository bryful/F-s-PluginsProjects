//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "MaskFromRGB_Multi.h"

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
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_R,
					STR_ON,
					TRUE,
					0,
					ID_R
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_G,
					STR_ON,
					FALSE,
					0,
					ID_G
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_B,
					STR_ON,
					FALSE,
					0,
					ID_B
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_Y,
					STR_ON,
					FALSE,
					0,
					ID_Y
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_M,
					STR_ON,
					FALSE,
					0,
					ID_M
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_C,
					STR_ON,
					FALSE,
					0,
					ID_C
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
	PF_Pixel revc = col;
	A_u_long v = 0;

	if (col.alpha == 0){
		v = 0;
	}else{
		if ( (niP->colFlag[M_COL::Y])||(niP->colFlag[M_COL::M])||(niP->colFlag[M_COL::C]))
		{
			revc.blue	= RoundByteFpLong(PF_MAX_CHAN8 - col.blue);
			revc.green	= RoundByteFpLong(PF_MAX_CHAN8 - col.green);
			revc.red	= RoundByteFpLong(PF_MAX_CHAN8 - col.red);
			if (col.alpha <PF_MAX_CHAN8){
				double a = (double)col.alpha / PF_MAX_CHAN8;
				revc.red	= RoundByteFpLong( (double)revc.red * a); 
				revc.green	= RoundByteFpLong( (double)revc.green * a); 
				revc.blue	= RoundByteFpLong( (double)revc.blue * a); 
			}
		}else{
			if (col.alpha <PF_MAX_CHAN8){
				double a = (double)col.alpha / PF_MAX_CHAN8;
				col.red		= RoundByteFpLong( (double)col.red * a); 
				col.green	= RoundByteFpLong( (double)col.green * a); 
				col.blue	= RoundByteFpLong( (double)col.blue * a); 
			}
		}
		A_long ax = 0;
		v = 0;
		A_long bx;
		if (niP->colFlag[M_COL::R]){
			//ax = (A_long)col.blue  + (A_long)col.green - (A_long)col.blue  * (A_long)col.green /  PF_MAX_CHAN8;
			ax = MAX((A_long)col.blue  , (A_long)col.green);
			bx = (A_long)col.red;
			v +=  RoundByteFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::G])&&(v<PF_MAX_CHAN8)){
			//ax = (A_long)col.blue  + (A_long)col.red - (A_long)col.blue  * (A_long)col.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)col.blue  , (A_long)col.red);
			bx = (A_long)col.green;
			v +=  RoundByteFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::B])&&(v<PF_MAX_CHAN8)){
			//ax = (A_long)col.green  + (A_long)col.red - (A_long)col.green  * (A_long)col.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)col.green  , (A_long)col.red);
			bx = (A_long)col.blue;
			v +=  RoundByteFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::Y])&&(v<PF_MAX_CHAN8)){
			//ax = (A_long)revc.green  + (A_long)revc.red - (A_long)revc.green  * (A_long)revc.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)revc.green  , (A_long)revc.red);
			bx = (A_long)revc.blue;
			v +=  RoundByteFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::M])&&(v<PF_MAX_CHAN8)){
			//ax = (A_long)revc.blue  + (A_long)revc.red - (A_long)revc.blue  * (A_long)revc.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)revc.blue  , (A_long)revc.red);
			bx = (A_long)revc.green;
			v +=  RoundByteFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::C])&&(v<PF_MAX_CHAN8)){
			//ax = (A_long)revc.blue  + (A_long)revc.green - (A_long)revc.blue  * (A_long)revc.green /  PF_MAX_CHAN8;
			ax = MAX((A_long)revc.blue  , (A_long)revc.green);
			bx = (A_long)revc.red;
			v +=  RoundByteFpLong((double)(bx - ax)*niP->col_level);
		}
		if (v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;

	}

	outP->alpha = (A_u_char)v;
	outP->red	= PF_MAX_CHAN8;
	outP->green	= PF_MAX_CHAN8;
	outP->blue	= PF_MAX_CHAN8;
	
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
	PF_Pixel16 revc = col;
	A_u_long v = 0;

	if (col.alpha == 0){
		v = 0;
	}else{
		if ( (niP->colFlag[M_COL::Y])||(niP->colFlag[M_COL::M])||(niP->colFlag[M_COL::C]))
		{
			revc.blue	= RoundShortFpLong(PF_MAX_CHAN16 - col.blue);
			revc.green	= RoundShortFpLong(PF_MAX_CHAN16 - col.green);
			revc.red	= RoundShortFpLong(PF_MAX_CHAN16 - col.red);
			if (col.alpha <PF_MAX_CHAN16){
				double a = (double)col.alpha / PF_MAX_CHAN16;
				revc.red	= RoundShortFpLong( (double)revc.red * a); 
				revc.green	= RoundShortFpLong( (double)revc.green * a); 
				revc.blue	= RoundShortFpLong( (double)revc.blue * a); 
			}
		}else{
			if (col.alpha <PF_MAX_CHAN16){
				double a = (double)col.alpha / PF_MAX_CHAN16;
				col.red		= RoundShortFpLong( (double)col.red * a); 
				col.green	= RoundShortFpLong( (double)col.green * a); 
				col.blue	= RoundShortFpLong( (double)col.blue * a); 
			}
		}
		A_long ax = 0;
		v = 0;
		A_long bx;
		if (niP->colFlag[M_COL::R]){
			//ax = (A_long)col.blue  + (A_long)col.green - (A_long)col.blue  * (A_long)col.green /  PF_MAX_CHAN8;
			ax = MAX((A_long)col.blue  , (A_long)col.green);
			bx = (A_long)col.red;
			v +=  RoundShortFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::G])&&(v<PF_MAX_CHAN16)){
			//ax = (A_long)col.blue  + (A_long)col.red - (A_long)col.blue  * (A_long)col.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)col.blue  , (A_long)col.red);
			bx = (A_long)col.green;
			v +=  RoundShortFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::B])&&(v<PF_MAX_CHAN16)){
			//ax = (A_long)col.green  + (A_long)col.red - (A_long)col.green  * (A_long)col.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)col.green  , (A_long)col.red);
			bx = (A_long)col.blue;
			v +=  RoundShortFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::Y])&&(v<PF_MAX_CHAN16)){
			//ax = (A_long)revc.green  + (A_long)revc.red - (A_long)revc.green  * (A_long)revc.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)revc.green  , (A_long)revc.red);
			bx = (A_long)revc.blue;
			v +=  RoundShortFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::M])&&(v<PF_MAX_CHAN16)){
			//ax = (A_long)revc.blue  + (A_long)revc.red - (A_long)revc.blue  * (A_long)revc.red /  PF_MAX_CHAN8;
			ax = MAX((A_long)revc.blue  , (A_long)revc.red);
			bx = (A_long)revc.green;
			v +=  RoundShortFpLong((double)(bx - ax)*niP->col_level);
		}
		if ((niP->colFlag[M_COL::C])&&(v<PF_MAX_CHAN16)){
			//ax = (A_long)revc.blue  + (A_long)revc.green - (A_long)revc.blue  * (A_long)revc.green /  PF_MAX_CHAN8;
			ax = MAX((A_long)revc.blue  , (A_long)revc.green);
			bx = (A_long)revc.red;
			v +=  RoundShortFpLong((double)(bx - ax)*niP->col_level);
		}
		if (v>PF_MAX_CHAN16) v = PF_MAX_CHAN16;

	}

	outP->alpha = (A_u_short)v;
	outP->red	= PF_MAX_CHAN16;
	outP->green	= PF_MAX_CHAN16;
	outP->blue	= PF_MAX_CHAN16;
	
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
	PF_PixelFloat revc = col;
	double v = 0;

	if (col.alpha == 0){
		v = 0;
	}else{
		if (col.red>1.0) col.red =1.0;
		if (col.green>1.0) col.green =1.0;
		if (col.blue>1.0) col.blue =1.0;
		revc = col;
		if ( (niP->colFlag[M_COL::Y])||(niP->colFlag[M_COL::M])||(niP->colFlag[M_COL::C]))
		{
			revc.blue	= RoundFpShortDouble(1.0 - col.blue);
			revc.green	= RoundFpShortDouble(1.0 - col.green);
			revc.red	= RoundFpShortDouble(1.0 - col.red);
			if (col.alpha <1.0){
				double a = (double)col.alpha;
				revc.red	= RoundFpShortDouble( (double)revc.red * a); 
				revc.green	= RoundFpShortDouble( (double)revc.green * a); 
				revc.blue	= RoundFpShortDouble( (double)revc.blue * a); 
			}
		}else{
			if (col.alpha <1.0){
				double a = (double)col.alpha;
				col.red		= RoundFpShortDouble( (double)col.red * a); 
				col.green	= RoundFpShortDouble( (double)col.green * a); 
				col.blue	= RoundFpShortDouble( (double)col.blue * a); 
			}
		}
		double ax = 0;
		double bx = 0;
		v = 0;
		if (niP->colFlag[M_COL::R]){
			ax = MAX(col.blue  , col.green);
			bx = col.red;
			v +=  (bx - ax)*niP->col_level;
		}
		if ((niP->colFlag[M_COL::G])&&(v<1.0)){
			ax = MAX(col.blue  , col.red);
			bx = col.green;
			v +=  (bx - ax)*niP->col_level;
		}
		if ((niP->colFlag[M_COL::B])&&(v<1.0)){
			ax = MAX(col.green  , col.red);
			bx = col.blue;
			v += (bx - ax)*niP->col_level;
		}
		if ((niP->colFlag[M_COL::Y])&&(v<1.0)){
			ax = MAX(revc.green  , revc.red);
			bx = revc.blue;
			v += (bx - ax)*niP->col_level;
		}
		if ((niP->colFlag[M_COL::M])&&(v<1.0)){
			ax = MAX(revc.blue  , revc.red);
			bx = revc.green;
			v += (bx - ax)*niP->col_level;
		}
		if ((niP->colFlag[M_COL::C])&&(v<1.0)){
			ax = MAX(revc.blue  , revc.green);
			bx = revc.red;
			v += (bx - ax)*niP->col_level;
		}
		if (v>1.0) v = 1.0;

	}

	outP->alpha = (PF_FpShort)v;
	outP->red	= 1.0;
	outP->green	= 1.0;
	outP->blue	= 1.0;
	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_FpLong d = 100;
	ERR(ae->GetFLOAT(ID_LEVEL,&d));
	if(!err){
		infoP->col_level = d/100;
	}else{
		infoP->col_level = 1.0;
	}
	PF_Boolean b;
	for (int i= ID_R; i<= ID_C; i++ ){
		ERR(ae->GetCHECKBOX(i,&b));
		if (err) b = false;
		infoP->colFlag[i - ID_R] = b;
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
