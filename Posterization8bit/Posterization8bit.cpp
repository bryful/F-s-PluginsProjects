//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Posterization8bit.h"


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
	PF_ADD_SLIDER(STR_LEVEL,	//パラメータの名前
		2, 			//数値入力する場合の最小値
		64,			//数値入力する場合の最大値
		2,			//スライダーの最小値 
		12,			//スライダーの最大値
		12,			//デフォルトの値
		ID_LEVEL
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_G,
					"on",
					TRUE,
					0,
					ID_GRAY
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
inline A_long PosSub8(double v, A_long level)
{
	A_long va = (A_long)((double)level * v + 0.5);
	va = (A_long)((double)va * PF_MAX_CHAN8 / (double)level + 0.5);
	return va;
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


	double a = (double)outP->alpha / PF_MAX_CHAN8;
	double r = (double)outP->red * a / PF_MAX_CHAN8;
	double g = (double)outP->green * a / PF_MAX_CHAN8;
	double b = (double)outP->blue * a / PF_MAX_CHAN8;

	A_long va = PosSub8(a, infoP->level);
	outP->alpha = RoundByteLong(va);

	double y = (0.299 * r + 0.587 * g + 0.114 * b);
	A_long vy = PosSub8(y, infoP->level);
	vy = (A_long)((double)vy *PF_MAX_CHAN8 / va + 0.5);

	if (infoP->grayOnly==TRUE)
	{
		outP->blue = outP->green = outP->red = RoundByteLong(vy);
	}
	else {
		double u = ((r * -0.169) - (g*0.331) + (b* 0.500))*2;
		double v = ((r *  0.500) - (g*0.419) - (b*-0.081))*2;

		A_long vu = PosSub8(u, infoP->level);
		vu = (A_long)((double)vu *PF_MAX_CHAN8 / ((double)va*2) + 0.5);

		A_long vv = PosSub8(v, infoP->level);
		vv = (A_long)((double)vv *PF_MAX_CHAN8 / ((double)va*2) + 0.5);

		A_long rr = (A_long)((double)vy                    + 1.402*(double)vv + 0.5);
		A_long gg = (A_long)((double)vy - 0.344*(double)vu - 0.714*(double)vv + 0.5);
		A_long bb = (A_long)((double)vy + 1.772*(double)vu                    + 0.5);

		outP->red = RoundByteLong(rr);
		outP->green = RoundByteLong(gg);
		outP->blue = RoundByteLong(bb);
	}



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

	ERR(ae->GetADD(ID_LEVEL, &infoP->level));

	ERR(ae->GetCHECKBOX(ID_GRAY,&infoP->grayOnly));

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
