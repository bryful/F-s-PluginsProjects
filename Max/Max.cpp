//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "Max.h"


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
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_MAX,	//パラメータの名前
					-512, 				//数値入力する場合の最小値
					512,			//数値入力する場合の最大値
					-50,				//スライダーの最小値 
					50,			//スライダーの最大値
					0,				//デフォルトの値
					ID_MAX
					);
	
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_DIR1, 
						3,	//メニューの数
						1,	//デフォルト
						STR_DIR2,
						ID_DIR
						);
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_CH1, 
						3,	//メニューの数
						1,	//デフォルト
						STR_CH2,
						ID_CHANNEL
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_OUT,
					"on",
					FALSE,
					0,
					ID_OUT
					);


	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
InOut8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	if (niP->isOutOnly){
		A_long a = (A_long)outP->alpha - (A_long)inP->alpha;
		outP->alpha = RoundByteFpLong(a);
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
InOut16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	if (niP->isOutOnly){
		A_long a = (A_long)outP->alpha - (A_long)inP->alpha;
		outP->alpha = RoundShort(a);
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
InOut32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	if (niP->isOutOnly){
		double a = outP->alpha - inP->alpha;
		outP->alpha = RoundFpShortDouble(a);
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_MAX,&infoP->max));
	if (!err) infoP->max = ae->downScaleNoClip(infoP->max);
	ERR(ae->GetPOPUP(ID_DIR,&infoP->dir));
	ERR(ae->GetPOPUP(ID_CHANNEL,&infoP->ch));
	ERR(ae->GetCHECKBOX(ID_OUT,&infoP->isOutOnly));

	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ERR(ae->CopyInToOut());
	if (infoP->max !=0){
		ERR(ae->out->Minimax(infoP->max,infoP->dir,infoP->ch));
		if (infoP->isOutOnly){
			switch(ae->pixelFormat())
			{
			case PF_PixelFormat_ARGB128:
				ERR(ae->iterate32((refconType)infoP,InOut32));
				break;
			case PF_PixelFormat_ARGB64:
				ERR(ae->iterate16((refconType)infoP,InOut16));
				break;
			case PF_PixelFormat_ARGB32:
				ERR(ae->iterate8((refconType)infoP,InOut8));
				break;
			}
		}
	}
	return err;
}
//-----------------------------------------------------------------------------
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