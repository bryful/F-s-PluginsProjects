//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Chroma.h"

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
	// ----------------------------------------------------------------
	cs.AddFloatSlider(	// R
		STR_R_SCALE,			//Name
		50,				//VALID_MIN
		150,			//VALID_MAX
		80,				//SLIDER_MIN
		120,			//SLIDER_MAX
		99,				//DFLT
		2,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_R_SCALE
	);
	cs.AddFloatSlider(	// R
		STR_G_SCALE,			//Name
		50,				//VALID_MIN
		150,			//VALID_MAX
		80,				//SLIDER_MIN
		120,			//SLIDER_MAX
		100,				//DFLT
		2,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_G_SCALE
	);
	cs.AddFloatSlider(	// R
		STR_B_SCALE,			//Name
		50,				//VALID_MIN
		150,			//VALID_MAX
		80,				//SLIDER_MIN
		120,			//SLIDER_MAX
		101,				//DFLT
		2,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_G_SCALE
	);
	cs.AddSlider(	// noise offset
		STR_R_BLUR,		//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,			//スライダーの最大値
		0,				//デフォルトの値
		ID_R_BLUR
	);
	cs.AddSlider(	// noise offset
		STR_G_BLUR,		//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,			//スライダーの最大値
		0,				//デフォルトの値
		ID_G_BLUR
	);
	cs.AddSlider(	// noise offset
		STR_B_BLUR,		//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,			//スライダーの最大値
		0,				//デフォルトの値
		ID_B_BLUR
	);
	cs.AddPoint(
		STR_CENTER,
		50,
		50,
		FALSE,
		ID_CENTER
	);
	cs.AddPopup(STR_ALPHA_MODE,
		STR_ALPHA_COUNT,
		STR_ALPHA_DFLT,
		STR_ALPHA_ITEMS,
		ID_ALPHA_MODE
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
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	ERR(ae->GetFLOAT(ID_R_SCALE, &infoP->r_scale));
	infoP->r_scale /= 100;
	ERR(ae->GetFLOAT(ID_G_SCALE, &infoP->g_scale));
	infoP->g_scale /= 100;
	ERR(ae->GetFLOAT(ID_B_SCALE, &infoP->b_scale));
	infoP->b_scale /= 100;
	ERR(ae->GetADD(ID_R_BLUR, &infoP->r_blur));
	ERR(ae->GetADD(ID_G_BLUR, &infoP->g_blur));
	ERR(ae->GetADD(ID_B_BLUR, &infoP->b_blur));
	ERR(ae->GetPOINT(ID_CENTER, &infoP->center));
	ERR(ae->GetPOPUP(ID_ALPHA_MODE, &infoP->alpha_mode));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	if (infoP->r_scale != 100 || infoP->g_scale != 100 || infoP->b_scale != 100) {
		ERR(Chromatic(
			ae->in_data,
			ae->input,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->r_scale,
			infoP->g_scale,
			infoP->b_scale,
			infoP->center,
			infoP->alpha_mode
		));
	}
	if(infoP->r_blur > 0) {
		ERR(ChannelBlur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->r_blur,
			0  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
		));

	}
	if (infoP->g_blur > 0) {
		ERR(ChannelBlur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->g_blur,
			1  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
		));

	}
	if (infoP->b_blur > 0) {
		ERR(ChannelBlur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->b_blur,
			2  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
		));
	}
	A_long ab = MAX(infoP->r_blur, MAX(infoP->g_blur, infoP->b_blur));
	if (ab > 0) {
		ERR(ChannelBlur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			ab,
			3  // 処理するチャンネル (0:R, 1:G, 2:B, 3:A)
		));
	}
	ERR(Mult(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		TRUE
	));

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

