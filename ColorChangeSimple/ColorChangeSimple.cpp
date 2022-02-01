//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ColorChangeSimple.h"


PF_Err (*subFunc8)(refconType	refcon, A_long xL, A_long yL,PF_Pixel8	*inP, PF_Pixel8	*outP);
PF_Err (*subFunc16)(refconType	refcon, A_long xL, A_long yL,PF_Pixel16	*inP, PF_Pixel16	*outP);
PF_Err (*subFunc32)(refconType	refcon, A_long xL, A_long yL,PF_PixelFloat	*inP, PF_PixelFloat	*outP);

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (	PF_InData		*in_data,
							PF_OutData		*out_data,
							PF_ParamDef		*params[],
							PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LEVEL,	//パラメータの名前
					0, 			//数値入力する場合の最小値
					100,		//数値入力する場合の最大値
					0,			//スライダーの最小値 
					10,			//スライダーの最大値
					0,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LEVEL_FIXED
					);

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_SRC, 
					0xFF,
					0xFF,
					0xFF,
					ID_SRC_COLOR
					);

	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_DST, 
					0x00,
					0x00,
					0x00,
					ID_DST_COLOR
					);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}

//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		if ( (ABS(outP->red - niP->src.red)<=niP->level)&&(ABS(outP->green - niP->src.green)<=niP->level)&&(ABS(outP->blue - niP->src.blue)<=niP->level) ) {
			outP->red	= niP->dst.red;
			outP->green	= niP->dst.green;
			outP->blue	= niP->dst.blue;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		if ( (outP->red == niP->src.red)&&(outP->green == niP->src.green)&&(outP->blue == niP->src.blue) ) 
		{
			outP->red	= niP->dst.red;
			outP->green	= niP->dst.green;
			outP->blue	= niP->dst.blue;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
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
					
	if (niP){
		PF_Pixel p = CONV16TO8(*outP);
		if ( (ABS(p.red - niP->src.red)<=niP->level)&&(ABS(p.green - niP->src.green)<=niP->level)&&(ABS(p.blue - niP->src.blue)<=niP->level) ) {
			outP->red	= niP->dst16.red;
			outP->green	= niP->dst16.green;
			outP->blue	= niP->dst16.blue;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage16Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Pixel p = CONV16TO8(*outP);
		if ( (p.red == niP->src.red)&&(p.green == niP->src.green)&&(p.blue == niP->src.blue) )
		{
			outP->red	= niP->dst16.red;
			outP->green	= niP->dst16.green;
			outP->blue	= niP->dst16.blue;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
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
					
	if (niP){
		PF_Pixel p = CONV32TO8(*outP);
		if ( (ABS(p.red - niP->src.red)<=niP->level)&&(ABS(p.green - niP->src.green)<=niP->level)&&(ABS(p.blue - niP->src.blue)<=niP->level) ) {
			outP->red	= niP->dst32.red;
			outP->green	= niP->dst32.green;
			outP->blue	= niP->dst32.blue;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage32Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Pixel p = CONV32TO8(*outP);
		if ( (p.red == niP->src.red)&&(p.green == niP->src.green)&&(p.blue == niP->src.blue) ) 
		{		
			outP->red	= niP->dst32.red;
			outP->green	= niP->dst32.green;
			outP->blue	= niP->dst32.blue;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	PF_Fixed f;
	ERR(ae->GetFIXED(ID_LEVEL_FIXED,&f));
	if (!err) infoP->level = RoundByteLong((PF_MAX_CHAN8 * f / 100)/65536);
	ERR(ae->GetCOLOR(ID_SRC_COLOR,&infoP->src));
	ERR(ae->GetCOLOR(ID_DST_COLOR,&infoP->dst));
	if (!err){
		infoP->dst16 = CONV8TO16(infoP->dst);
		infoP->dst32 = CONV8TO32(infoP->dst);
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
	if ( infoP->level ==0){
		subFunc8 = FilterImage8Zero;
		subFunc16 = FilterImage16Zero;
		subFunc32 = FilterImage32Zero;
	}else{
		subFunc8 = FilterImage8;
		subFunc16 = FilterImage16;
		subFunc32 = FilterImage32;
	}
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,subFunc32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,subFunc16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,subFunc8));
		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
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
	return err;
}
//-----------------------------------------------------------------------------------
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