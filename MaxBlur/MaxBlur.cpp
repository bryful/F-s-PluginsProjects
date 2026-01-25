//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "MaxBlur.h"
#include <string>


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
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_MINMAX,	//パラメータの名前
		-1000, 		//数値入力する場合の最小値
		1000,		//数値入力する場合の最大値
		-10,		//スライダーの最小値 
		10,		//スライダーの最大値
		0,		//デフォルトの値
		ID_MINMAX
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_BLUR,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		1000,		//数値入力する場合の最大値
		0,		//スライダーの最小値 
		100,		//スライダーの最大値
		0,		//デフォルトの値
		ID_BLUR
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HYPERBOLIC,	//Name
		-10,				//VALID_MIN
		50,					//VALID_MAX
		-2,					//SLIDER_MIN
		10,					//SLIDER_MAX
		1,					//CURVE_TOLERANCE
		0,					//DFLT
		1,					//PREC
		0,					//DISP
		0,					//WANT_PHASE
		ID_HYPERBOLIC
	);


	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-------------------------------------------------------------------------------------------------

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
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Exec(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ae->CopyInToOut(); // 元の画像をコピー
	if ((infoP->blur <= 0)&&(infoP->minmax==0)) return err;

	PF_WorldSuite2* ws2P;
	PF_PixelFormat pixelFormat;
	AEFX_AcquireSuite(ae->in_data,
		ae->out_data,
		kPFWorldSuite,
		kPFWorldSuiteVersion2,
		"Couldn't load suite.",
		(void**)&(ws2P));

	ws2P->PF_GetPixelFormat(ae->output, &pixelFormat);

	AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scope(
		ae->in_data,
		kPFIterate8Suite,
		kPFIterate8SuiteVersion1,
		ae->out_data
	);
	//ERR(TinyBlue(ae->in_data,ae->out_data,ae->output,infoP->radius));
	if (infoP->minmax != 0) {
		TinyMinMaxM(ae->in_data, ae->output, pixelFormat, iter_scope, infoP->minmax);
	}
	if (infoP->blur > 0) {
		TinyMultM(ae->in_data, ae->output, pixelFormat, iter_scope, FALSE);
		TinyBlueM(ae->in_data, ae->output, pixelFormat, iter_scope, infoP->blur);
		TinyMultM(ae->in_data, ae->output, pixelFormat, iter_scope, TRUE);
	}
	if (infoP->hyperbolic != 0.0) {
		HyperbolicAlphaM(ae->in_data, ae->output, pixelFormat, iter_scope, infoP->hyperbolic);
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

