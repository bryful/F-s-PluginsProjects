//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "Mosaic.h"

//-------------------------------------------------------------------------------------------------
// Aboutダイアログ
static PF_Err About (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.About(in_data,out_data,params,output);
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err GlobalSetup (	PF_InData		*in_data,
							PF_OutData		*out_data,
							PF_ParamDef		*params[],
							PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetup(in_data,out_data,params,output);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetdown(
	PF_InData	*in_data)
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetdown(in_data);
	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup (	PF_InData		*in_data,
								PF_OutData		*out_data,
								PF_ParamDef		*params[],
								PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


static PF_Err SequenceSetdown (	PF_InData		*in_data,
								PF_OutData		*out_data,
								PF_ParamDef		*params[],
								PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


static PF_Err SequenceResetup (	PF_InData		*in_data,
								PF_OutData		*out_data,
								PF_ParamDef		*params[],
								PF_LayerDef		*output )
{
	return PF_Err_NONE;
}

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

	//----------------------------------------------------
	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SIZE,//パラメータの名前
					1, 				//数値入力する場合の最小値
					1920,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					256,			//スライダーの最大値
					1,					//デフォルトの値
					ID_SIZE
					);

	//----------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POINT(STR_POS			/*"New Center"*/, 
				50,	// X
				50,	// Y
				0,	// Flag
				ID_POS
				);
	//----------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_RAND,	//パラメータの名前
					0, 			//数値入力する場合の最小値
					100,		//数値入力する場合の最大値
					0,			//スライダーの最小値 
					20,			//スライダーの最大値
					0,			//デフォルトの値
					1,			//数値表示に関するフラグ 
					0,
					0,
					ID_RAND
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_RAND_GRAY,
					STR_ON,
					FALSE,
					0,
					ID_RAND_GRAY
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_FRAME_RAND_ON,
					STR_ON,
					FALSE,
					0,
					ID_FRAME_RAND_ON
					);
	//----------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;

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
	/*
	AEGP_SuiteHandler		suites(in_data->pica_basicP);
	if ((PF_Boolean)params[ID_ANIMATED_CB]->u.bd.value == TRUE){
		params[ID_ANIMATED_ADD]->ui_flags |= PF_PUI_DISABLED;
	}else{
		params[ID_ANIMATED_ADD]->ui_flags  &= ~PF_PUI_DISABLED;
	}
	ERR(suites.ParamUtilsSuite1()->PF_UpdateParamUI(in_data->effect_ref, 
													ID_ANIMATED_ADD,
													params[ID_ANIMATED_ADD]));

	if (!err){
		out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
	}
	*/
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
	
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	CFsAE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_FRAME_RAND_ON,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_SIZE,&infoP->size));
	if (!err)infoP->size = ae->downScale(infoP->size);
	ERR(ae->GetFIXEDPOINT(ID_POS,&infoP->pos));
	ERR(ae->GetFIXED2FpShort(ID_RAND,&infoP->rand));
	if (!err) infoP->rand/=100;
	ERR(ae->GetCHECKBOX(ID_RAND_GRAY,&infoP->rand_gray));
	ERR(ae->GetCHECKBOX(ID_FRAME_RAND_ON,&infoP->frameRandOn));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ae->CopyInToOut();
	if (infoP->size>1)
	{
		FsMosaicParam mp;
		mp.sizeX		= infoP->size;
		mp.sizeY		= infoP->size;
		mp.pos			= infoP->pos;
		mp.rand			= infoP->rand;
		mp.isRandGray	= infoP->rand_gray;
		mp.isFrameRand	= infoP->frameRandOn;
		ERR(ae->out->mosaic(&mp));
	}
	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err Render (	PF_InData		*in_data,
						PF_OutData		*out_data,
						PF_ParamDef		*params[],
						PF_LayerDef		*output )
{
	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ERR(ae.CopyInToOut());
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
//-----------------------------------------------------------------------------------
static PF_Err 
RespondtoAEGP ( 	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extraP)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extraP));

	return err;
}


//-----------------------------------------------------------------------------------
DllExport	PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP)
{
	PF_Err		err = PF_Err_NONE;
	
	try
	{
		CFsAE ae;
		switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_data);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceResetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
					err = Render(in_data,out_data,params,output);
					break;
#if defined(SUPPORT_SMARTFX)
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_data,out_data,reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(	in_data,out_data,reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
#endif
			case PF_Cmd_COMPLETELY_GENERAL:
				err = RespondtoAEGP(in_data,out_data,params,output,extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;		
			case PF_Cmd_USER_CHANGED_PARAM:
				err = HandleChangedParam(	in_data,
											out_data,
											params,
											output, 
											reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;

			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_data,
										out_data,
										params,
										reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
