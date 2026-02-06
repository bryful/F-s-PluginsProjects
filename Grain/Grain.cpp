//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Grain.h"

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

	NF_ParamsSetup cs(in_data, out_data);
	cs.AddFloatSilder(	
		STR_MASTER,		//Name
		0,				//VALID_MIN
		10000,			//VALID_MAX
		0,				//SLIDER_MIN
		200,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_MASTER
	);
	cs.AddFloatSilder(
		STR_AMOUNT,		//Name
		0,				//VALID_MIN
		600,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_AMOUNT
	);
	cs.AddFloatSilder(
		STR_SIZE,		//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		10,				//SLIDER_MAX
		4.5,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SIZE
	);
	cs.AddFloatSilder(
		STR_SUB_AMOUNT,		//Name
		0,				//VALID_MIN
		300,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		30,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SUB_AMOUNT
	);
	cs.AddFloatSilder(
		STR_SUB_SIZE,		//Name
		0,				//VALID_MIN
		300,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		25,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SUB_SIZE
	);
	// ----------------------------------------------------------------
	cs.AddSlider(	// noise offset
		STR_ACCENT_COUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		3000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_ACCENT_COUNT	//パラメータの名前

	);
	cs.AddFloatSilder(
		STR_ACCENT_LOLIMIT,		//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_ACCENT_LOLIMIT
	);
	cs.AddFloatSilder(
		STR_ACCENT_INTENSITY,		//Name
		0,				//VALID_MIN
		20000,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		75,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_ACCENT_INTENSITY
	);
	// ----------------------------------------------------------------
	cs.AddCheckBox(	// noise frame
		STR_MONOCHROME,
		STR_MONOCHROME2,
		TRUE,
		ID_MONOCHROME
	);
	cs.AddCheckBox(	// noise frame
		STR_AUTO_SEED,
		STR_AUTO_SEED2,
		TRUE,
		ID_AUTO_SEED,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddSlider(	// noise offset
		STR_SEED,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-1000,				//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED,
		PF_ParamFlag_NONE,
		PF_PUI_DISABLED
	);
	
	cs.Finalize();
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

	try{
		PF_Boolean b=FALSE;
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if((!err)&&(in_data->appl_id != 'PrMr')){
			//--------------------
			PF_Boolean b = FALSE;
			ERR(ae.GetCHECKBOX(ID_AUTO_SEED, &b));
			ERR(ae.UI_DISABLE(ID_SEED, b));
			out_data->out_flags |=  PF_OutFlag_REFRESH_UI;

		}
	}catch ( PF_Err & errP){
		err = errP;
	}
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
	PF_Err 	err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_AUTO_SEED,&def));
		PF_Boolean auto_seed = (PF_Boolean)def.u.bd.value;
		ERR(ae.SetOutFlag_NON_PARAM_VARY(auto_seed));
		ERR(ae.checkin_param(&def));

	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	ERR(ae->GetFLOAT(ID_MASTER, &infoP->master_percent));
	infoP->master_percent /= 100;
	if (infoP->master_percent < 0) infoP->master_percent = 0;
	ERR(ae->GetFLOAT(ID_AMOUNT, &infoP->amount));
	ERR(ae->GetFLOAT(ID_SIZE, &infoP->size));

	ERR(ae->GetFLOAT(ID_SUB_AMOUNT, &infoP->sub_amount_percent));
	infoP->sub_amount_percent /= 100;
	ERR(ae->GetFLOAT(ID_SUB_SIZE, &infoP->sub_size_percent));
	infoP->sub_size_percent /= 100;

	ERR(ae->GetADD(ID_ACCENT_COUNT, &infoP->accentCount));
	ERR(ae->GetFLOAT(ID_ACCENT_LOLIMIT, &infoP->accentCountLoLimit));
	infoP->accentCountLoLimit /= 100;
	ERR(ae->GetFLOAT(ID_ACCENT_INTENSITY, &infoP->accentIntensity));
	infoP->accentIntensity /= 100;

	ERR(ae->GetCHECKBOX(ID_MONOCHROME, &infoP->is_monochrome));
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->is_autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	A_long seed;
	if (infoP->is_autoSeed == TRUE) {
		seed = infoP->seed + ae->frame();
	}
	else {
		seed = infoP->seed;
	}
	if ((infoP->size > 0) && (infoP->amount > 0) && (infoP->master_percent > 0))
	{
		PF_FpShort amount = (PF_FpShort)(infoP->amount * infoP->master_percent);
		PF_FpShort size = (PF_FpShort)infoP->size;
		GrainFX(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			(PF_FpShort)size,
			(PF_FpShort)amount,
			(PF_FpShort)(infoP->sub_size_percent),
			(PF_FpShort)(infoP->sub_amount_percent),
			seed,
			(A_Boolean)(!infoP->is_monochrome)
		);
	}
	A_long accentCount = (A_long)((double)infoP->accentCount * infoP->master_percent + 0.5);

	if (accentCount > 0 && infoP->accentIntensity>0) {
		DrawSpatAccent(
			ae->output,
			ae->pixelFormat(),
			accentCount,
			(PF_FpShort)(infoP->accentCountLoLimit),
			(A_Boolean)(!infoP->is_monochrome),
			(PF_FpShort)(infoP->accentIntensity),
			seed
		);
	}
	
	return err;
}


//-----------------------------------------------------------------------------------
DllExport
PF_Err PluginDataEntryFunction2(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB2 inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT_EXT2(
		inPtr,
		inPluginDataCallBackPtr,
		NF_NAME, // Name
		NF_MATCHNAME, // Match Name
		NF_CATEGORY, // Category
		AE_RESERVED_INFO, // Reserved Info
		"EffectMain",	// Entry point
		"https://github.com/bryful");	// support URL

	return result;
}
DllExport	PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	void* extraP)
{
	PF_Err			err = PF_Err_NONE,
					err2 = PF_Err_NONE;

	try
	{
		NF_AE ae;
		switch (cmd) {
		case PF_Cmd_ABOUT:
			err = ae.About(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = ae.GlobalSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETDOWN:
			err = ae.GlobalSetdown(in_data);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_SEQUENCE_SETUP:
			break;
		case PF_Cmd_SEQUENCE_SETDOWN:
			break;
		case PF_Cmd_SEQUENCE_RESETUP:
			break;
		case PF_Cmd_RENDER:
			err = ae.Render(in_data, out_data, params, output, ID_NUM_PARAMS);
			if (!err) {
				ParamInfo info;
				ERR(GetParams(&ae, &info));
				ERR(Exec(&ae, &info));
			}
			break;
		case PF_Cmd_SMART_PRE_RENDER:
			err = ae.PreRender(in_data, out_data, reinterpret_cast<PF_PreRenderExtra*>(extraP),sizeof(ParamInfo), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ae.SetHostPreRenderData();
					ERR(GetParams(&ae, infoP));
					ERR(ae.UnSetPreRenderData());
					ae.UnlockPreRenderData();
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		case PF_Cmd_SMART_RENDER:
			err = ae.SmartRender(in_data, out_data, reinterpret_cast<PF_SmartRenderExtra*>(extraP), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ERR(Exec(&ae, infoP));
					ERR2(ae.UnsetSmartRender());
					ae.UnlockPreRenderData();
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		case PF_Cmd_COMPLETELY_GENERAL:
		{
			AEGP_SuiteHandler suites(in_data->pica_basicP);

			suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
				"%s",
				reinterpret_cast<A_char*>(extraP));
			break;
		}
		case PF_Cmd_DO_DIALOG:
		{
			//err = PopDialog(in_data,out_data,params,output);
			break;
		}
		case PF_Cmd_USER_CHANGED_PARAM:
		{
			err = HandleChangedParam(in_data,
				out_data,
				params,
				output,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));

			break;
		}
		case PF_Cmd_QUERY_DYNAMIC_FLAGS:
			err = QueryDynamicFlags(in_data,
				out_data,
				params,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}
	if (err != PF_Err_NONE) {
		AEGP_SuiteHandler suites(in_data->pica_basicP);

		suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
			"%d", cmd
			);
	}
	return err;
}

