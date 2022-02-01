
//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "LineDetection.h"


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
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_DELTA_CB,
					STR_CB,
					TRUE,
					0,
					ID_DELTA_CB
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_DELTA_OPACITY,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					500,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					300,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_DELTA_OPACITY
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_ALPHA_CB,
					STR_CB,
					FALSE,
					0,
					ID_ALPHA_CB
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_ALPHA_OPACITY,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					500,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					300,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_ALPHA_OPACITY
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_LINE_COLOR, 
					0x0,
					0x0,
					0x0,
					ID_LINE_COLOR
					);
	/*
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	def.flags 	= 	PF_ParamFlag_START_COLLAPSED;
	PF_ADD_TOPIC(STR_TOPIC, ID_TOPIC);
	//****************************************************************
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR_OPACITY,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR_OPACITY
					);
	//****************************************************************
	//0
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR0_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR0_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR0, 
					0x10,
					0x10,
					0x10,
					ID_COLOR0
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR0_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR0_LEVEL
					);
	//****************************************************************
	//1
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR1_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR1_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR1, 
					0xFF,
					0x0,
					0x0,
					ID_COLOR1
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR1_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR1_LEVEL
					);
	//****************************************************************
	//2
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR2_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR2_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR2, 
					0x0,
					0xFF,
					0x0,
					ID_COLOR2
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR2_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR2_LEVEL
					);
	//****************************************************************
	//3
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR3_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR3_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR3, 
					0x0,
					0x0,
					0xFF,
					ID_COLOR1
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR3_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR3_LEVEL
					);
	//****************************************************************
	//4
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR4_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR4_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR4, 
					0xFF,
					0xFF,
					0x0,
					ID_COLOR4
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR4_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR4_LEVEL
					);
	//****************************************************************
	//5
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR5_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR5_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR5, 
					0xFF,
					0x0,
					0xFF,
					ID_COLOR6
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR5_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR5_LEVEL
					);
	//****************************************************************
	//6
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR6_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR6_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR6, 
					0x0,
					0xFF,
					0xFF,
					ID_COLOR6
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR6_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR6_LEVEL
					);
	//****************************************************************
	//7
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_COLOR7_CB,
					STR_CB,
					FALSE,
					0,
					ID_COLOR7_CB
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR7, 
					0xFF,
					0xFF,
					0xFF,
					ID_COLOR7
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR7_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR7_LEVEL
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_END);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	*/
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

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
	PF_Err 	err 	= PF_Err_NONE,
			err2 	= PF_Err_NONE;
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	ERR(ae->GetCHECKBOX(ID_DELTA_CB,&infoP->delta_flg));
	ERR(ae->GetFIXED_PER2ONE(ID_DELTA_OPACITY,&infoP->delta_opacity));
	
	ERR(ae->GetCHECKBOX(ID_ALPHA_CB,&infoP->alpha_flg));
	ERR(ae->GetFIXED_PER2ONE(ID_ALPHA_OPACITY,&infoP->alpha_opacity));
	
	ERR(ae->GetCOLOR(ID_LINE_COLOR,&infoP->line_color));

	/*
	ERR(ae->GetFIXED_PER2ONE(ID_COLOR_OPACITY,&infoP->color_opacity));
	
	//-----------------------
	infoP->tbl_count =0;
	for (A_long i=0; i<7; i++){
		PF_Pixel c;
		PF_Fixed f;
		PF_Boolean b;
		A_long idx = ID_COLOR0_CB +i*3;
		ERR(ae->GetCHECKBOX(idx + 0,&b));
		if (!err){
			if ( b==TRUE){
				ERR(ae->GetCOLOR(idx + 1,&c));
				ERR(ae->GetFIXED_PER2ONE(idx + 2,&f));
				if ( !err){
					infoP->color_tbl[infoP->tbl_count] = c;
					infoP->level_tbl[infoP->tbl_count] = f;
					infoP->tbl_count++;
				}
			}
		}
	}
	*/
	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if ( (infoP->delta_flg==FALSE)&&(infoP->alpha_flg==FALSE) ){
		ERR(ae->CopyInToOut());
		return err;
	}
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		FindTargetDelta32(ae,infoP);
		//FindTarget32(ae,infoP);
		break;
	case PF_PixelFormat_ARGB64:
		FindTargetDelta16(ae,infoP);
		//FindTarget16(ae,infoP);
		break;
	case PF_PixelFormat_ARGB32:
		FindTargetDelta8(ae,infoP);
		//FindTarget8(ae,infoP);
		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (
	PF_InData			*in_data,
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