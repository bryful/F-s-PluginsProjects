//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ExpsColors.h"


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
	PF_ADD_CHECKBOX(STR_ON,
		STR_ON2,
		TRUE,
		0,
		ID_ON
	);

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color0", 0xFF, 0xFF, 0xFF, ID_COLOR0);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color1", 0xFF, 0x00, 0x00, ID_COLOR1);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color2", 0x00, 0xFF, 0x00, ID_COLOR2);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color3", 0x00, 0x00, 0xFF, ID_COLOR3);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color4", 0xFF, 0xFF, 0x00, ID_COLOR4);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color5", 0xFF, 0x00, 0xFF, ID_COLOR5);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color6", 0x00, 0xFF, 0xFF, ID_COLOR6);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color7", 0x00, 0x00, 0x00, ID_COLOR7);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color8", 0x80, 0x80, 0x80, ID_COLOR8);
	AEFX_CLR_STRUCT(def); PF_ADD_COLOR("color9", 0xFF, 0x80, 0xFF, ID_COLOR9);

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
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_ON, &infoP->on));

	ERR(ae->GetCOLOR(ID_COLOR0, &infoP->col[0]));
	ERR(ae->GetCOLOR(ID_COLOR1, &infoP->col[1]));
	ERR(ae->GetCOLOR(ID_COLOR2, &infoP->col[2]));
	ERR(ae->GetCOLOR(ID_COLOR3, &infoP->col[3]));
	ERR(ae->GetCOLOR(ID_COLOR4, &infoP->col[4]));
	ERR(ae->GetCOLOR(ID_COLOR5, &infoP->col[5]));
	ERR(ae->GetCOLOR(ID_COLOR6, &infoP->col[6]));
	ERR(ae->GetCOLOR(ID_COLOR7, &infoP->col[7]));
	ERR(ae->GetCOLOR(ID_COLOR8, &infoP->col[8]));
	ERR(ae->GetCOLOR(ID_COLOR9, &infoP->col[9]));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	if (infoP->on == TRUE) {
		ERR(ae->CopyInToOut());
	}
	else {
		ERR(ae->out->clear());
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
