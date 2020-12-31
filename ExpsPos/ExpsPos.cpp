//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ExpsPos.h"


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
//位置の指定
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos0", 00, 00, 0, ID_POS0);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos1", 10, 10, 0, ID_POS1);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos2", 20, 20, 0, ID_POS2);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos3", 30, 30, 0, ID_POS3);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos4", 40, 40, 0, ID_POS4);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos5", 50, 50, 0, ID_POS5);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos6", 60, 60, 0, ID_POS6);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos7", 70, 70, 0, ID_POS7);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos8", 80, 80, 0, ID_POS8);
	AEFX_CLR_STRUCT(def); PF_ADD_POINT("pos9", 90, 90, 0, ID_POS9);
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

	ERR(ae->GetFIXEDPOINT(ID_POS0, &infoP->pos[0]));
	ERR(ae->GetFIXEDPOINT(ID_POS1, &infoP->pos[1]));
	ERR(ae->GetFIXEDPOINT(ID_POS2, &infoP->pos[2]));
	ERR(ae->GetFIXEDPOINT(ID_POS3, &infoP->pos[3]));
	ERR(ae->GetFIXEDPOINT(ID_POS4, &infoP->pos[4]));
	ERR(ae->GetFIXEDPOINT(ID_POS5, &infoP->pos[5]));
	ERR(ae->GetFIXEDPOINT(ID_POS6, &infoP->pos[6]));
	ERR(ae->GetFIXEDPOINT(ID_POS7, &infoP->pos[7]));
	ERR(ae->GetFIXEDPOINT(ID_POS8, &infoP->pos[8]));
	ERR(ae->GetFIXEDPOINT(ID_POS9, &infoP->pos[9]));
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
