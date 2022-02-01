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
	PF_Pixel bl = { 0,0,0,0 };
	if (outP->alpha == 0)
	{
		*outP = bl;
		return err;
	}

	//各チャンネルを１の実数へ そしてMatの処理
	double a = (double)outP->alpha / PF_MAX_CHAN8;
	double r = (double)outP->red * a / PF_MAX_CHAN8;
	double g = (double)outP->green * a / PF_MAX_CHAN8;
	double b = (double)outP->blue * a / PF_MAX_CHAN8;

	//まずアルファーの処理
	A_long va = (A_long)((double)infoP->level * a + 0.5);//levelが最大になるように四捨五入して整数整数化
	va = (A_long)((double)va * PF_MAX_CHAN8 / (double)infoP->level + 0.5);//255に変換
	outP->alpha = RoundByteLong(va);


	if (infoP->grayOnly==TRUE)
	{
		double y = (0.299 * r + 0.587 * g + 0.114 * b);
		A_long vy = (A_long)((double)infoP->level * y + 0.5); //levelが最大になるように四捨五入して整数整数化
		vy = (A_long)((double)vy * PF_MAX_CHAN8 / (double)infoP->level + 0.5); //255に変換
		vy = (A_long)((double)vy *PF_MAX_CHAN8 / va + 0.5); //Matの処理
		outP->blue = outP->green = outP->red = RoundByteLong(vy);
	}
	else {

		A_long vr = (A_long)((double)infoP->level * r + 0.5); //levelが最大になるように四捨五入して整数整数化
		vr = (A_long)((double)vr * PF_MAX_CHAN8 / (double)infoP->level + 0.5); //255に変換
		vr = (A_long)((double)vr *PF_MAX_CHAN8 / va + 0.5); //Matの処理

		A_long vg = (A_long)((double)infoP->level * g + 0.5); //levelが最大になるように四捨五入して整数整数化
		vg = (A_long)((double)vg * PF_MAX_CHAN8 / (double)infoP->level + 0.5); //255に変換
		vg = (A_long)((double)vg *PF_MAX_CHAN8 / va + 0.5); //Matの処理

		A_long vb = (A_long)((double)infoP->level * b + 0.5); //levelが最大になるように四捨五入して整数整数化
		vb = (A_long)((double)vb * PF_MAX_CHAN8 / (double)infoP->level + 0.5); //255に変換
		vb = (A_long)((double)vb *PF_MAX_CHAN8 / va + 0.5); //Matの処理

		outP->red = RoundByteLong(vr);
		outP->green = RoundByteLong(vg);
		outP->blue = RoundByteLong(vb);
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
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel16 bl = { 0,0,0,0 };
	if (outP->alpha == 0)
	{
		*outP = bl;
		return err;
	}

	//各チャンネルを１の実数へ そしてMatの処理
	double a = (double)outP->alpha / PF_MAX_CHAN16;
	double r = (double)outP->red * a / PF_MAX_CHAN16;
	double g = (double)outP->green * a / PF_MAX_CHAN16;
	double b = (double)outP->blue * a / PF_MAX_CHAN16;

	//まずアルファーの処理
	A_long va = (A_long)((double)infoP->level * a + 0.5);//levelが最大になるように四捨五入して整数整数化
	va = (A_long)((double)va * PF_MAX_CHAN16 / (double)infoP->level + 0.5);//255に変換
	outP->alpha = RoundShort(va);


	if (infoP->grayOnly == TRUE)
	{
		double y = (0.299 * r + 0.587 * g + 0.114 * b);
		A_long vy = (A_long)((double)infoP->level * y + 0.5); //levelが最大になるように四捨五入して整数整数化
		vy = (A_long)((double)vy * PF_MAX_CHAN16 / (double)infoP->level + 0.5); //255に変換
		vy = (A_long)((double)vy *PF_MAX_CHAN16 / va + 0.5); //Matの処理
		outP->blue = outP->green = outP->red = RoundShort(vy);
	}
	else {

		A_long vr = (A_long)((double)infoP->level * r + 0.5); //levelが最大になるように四捨五入して整数整数化
		vr = (A_long)((double)vr * PF_MAX_CHAN16 / (double)infoP->level + 0.5); //255に変換
		vr = (A_long)((double)vr *PF_MAX_CHAN16 / va + 0.5); //Matの処理

		A_long vg = (A_long)((double)infoP->level * g + 0.5); //levelが最大になるように四捨五入して整数整数化
		vg = (A_long)((double)vg * PF_MAX_CHAN16 / (double)infoP->level + 0.5); //255に変換
		vg = (A_long)((double)vg *PF_MAX_CHAN16 / va + 0.5); //Matの処理

		A_long vb = (A_long)((double)infoP->level * b + 0.5); //levelが最大になるように四捨五入して整数整数化
		vb = (A_long)((double)vb * PF_MAX_CHAN16 / (double)infoP->level + 0.5); //255に変換
		vb = (A_long)((double)vb *PF_MAX_CHAN16 / va + 0.5); //Matの処理

		outP->red = RoundShort(vr);
		outP->green = RoundShort(vg);
		outP->blue = RoundShort(vb);
	}



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
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_PixelFloat bl = { 0,0,0,0 };
	if (outP->alpha == 0)
	{
		*outP = bl;
		return err;
	}

	//各チャンネルを１の実数へ そしてMatの処理
	double a = (double)outP->alpha;
	double r = (double)outP->red * a;
	double g = (double)outP->green * a;
	double b = (double)outP->blue * a;

	//まずアルファーの処理
	A_long va = (A_long)((double)infoP->level * a + 0.5);//levelが最大になるように四捨五入して整数整数化
	a = (double)va / (double)infoP->level;//255に変換
	outP->alpha = RoundFpShortDouble(a);


	if (infoP->grayOnly == TRUE)
	{
		double y = (0.299 * r + 0.587 * g + 0.114 * b);
		A_long vy = (A_long)((double)infoP->level * y + 0.5); //levelが最大になるように四捨五入して整数整数化
		y = ((double)vy / (double)infoP->level); //255に変換
		y = y / a; //Matの処理
		outP->blue = outP->green = outP->red = RoundFpShortDouble(y);
	}
	else {

		A_long vr = (A_long)((double)infoP->level * r + 0.5); //levelが最大になるように四捨五入して整数整数化
		r = ((double)vr / (double)infoP->level); //255に変換
		r = r / a; //Matの処理

		A_long vg = (A_long)((double)infoP->level * g + 0.5); //levelが最大になるように四捨五入して整数整数化
		g = ((double)vg / (double)infoP->level); //255に変換
		g = g / a; //Matの処理

		A_long vb = (A_long)((double)infoP->level * b + 0.5); //levelが最大になるように四捨五入して整数整数化
		b = ((double)vb / (double)infoP->level); //255に変換
		b = b / a; //Matの処理

		outP->red = RoundFpShortDouble(r);
		outP->green = RoundFpShortDouble(g);
		outP->blue = RoundFpShortDouble(b);
	}


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
