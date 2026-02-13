//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2026
*/
//-----------------------------------------------------------------------------------


#include "Thin.h"

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
	cs.AddTopic(STR_TARGET_COLOR_TOPIC, ID_TARGET_COLOR_TOPIC);
	//	PF_ParamFlag_START_COLLAPSED,
	//	PF_PUI_DISABLED);
	cs.AddPopup(STR_TARGET_COLOR_MODE,
		STR_TARGET_COLOR_COUNT,
		STR_TARGET_COLOR_DFLT,
		STR_TARGET_COLOR_ITEMS,
		ID_TARGET_COLOR_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP

	);
	cs.AddFloatSlider(	// R
		STR_TARGET_LEVEL,			//Name
		0,				//VALID_MIN
		10,				//VALID_MAX
		0,				//SLIDER_MIN
		10,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_TARGET_LEVEL
	);
	for(int i=0; i< TARGET_COLOR_MAX; i++) {
		PF_ParamFlags flags = PF_ParamFlag_NONE;
		PF_ParamUIFlags ui_flags = PF_PUI_NONE;
		if (i > 0) {
			ui_flags = PF_PUI_INVISIBLE;
		}
		std::string str = STR_TARGET_COLOR + std::to_string(i + 1);
		cs.AddColor(	// color
			str.c_str(),
			{ 0xFF, (A_u_char)( i * 30), (A_u_char)(i * 30), (A_u_char)(i * 30) },
			ID_TARGET_COLOR(i),
			flags,
			ui_flags
		);
	}
	cs.EndTopic(ID_TARGET_COLOR_TOPIC_END);
	cs.AddSlider(	// 
		STR_THIN_VALUE,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		20,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,			//スライダーの最大値
		0,				//デフォルトの値
		ID_THIN_VALUE
	);
	cs.AddTopic(ID_PARAMS_TOPIC, ID_TARGET_COLOR_TOPIC);
	cs.AddCheckBox(	// noise frame
		STR_WHITE,
		"on",
		FALSE,
		ID_WHITE
	);
	cs.AddCheckBox(	// noise frame
		STR_ALPHAZERO,
		"on",
		FALSE,
		ID_ALPHAZERO
	);
	cs.AddCheckBox(	// noise frame
		STR_EDGE,
		"on",
		FALSE,
		ID_EDGE
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
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				PF_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 0; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				A_long a = 0;
				for (A_long i = 0; i < TARGET_COLOR_MAX; i++) hide_themB[ID_TARGET_COLOR(i)] = TRUE;
				ERR(ae.GetPOPUP(ID_TARGET_COLOR_MODE, &a));
				if (a >0) {
					for (A_long i = 0; i < a; i++)hide_themB[ID_TARGET_COLOR(i)] = FALSE;
				}
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));

				if (!err) {
					out_data->out_flags |=  PF_OutFlag_REFRESH_UI;
				}
			}
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
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetPOPUP(ID_TARGET_COLOR_MODE, &infoP->target_color_count));
	ERR(ae->GetFLOAT(ID_TARGET_LEVEL, &infoP->target_level));

	for(int i=0; i<TARGET_COLOR_MAX; i++) {
		ERR(ae->GetCOLOR(ID_TARGET_COLOR(i), &infoP->target_color[i]));
	}
	ERR(ae->GetADD(ID_THIN_VALUE, &infoP->thin_value));

	ERR(ae->GetCHECKBOX(ID_WHITE, &infoP->white));
	ERR(ae->GetCHECKBOX(ID_ALPHAZERO, &infoP->alphaZero));
	ERR(ae->GetCHECKBOX(ID_EDGE, &infoP->edge));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	if ((infoP->target_color_count <= 0) || (infoP->thin_value <= 0))
	{
		return err;
	}
	switch (ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(thinExec32(ae, infoP));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(thinExec16(ae, infoP));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(thinExec8(ae, infoP));
		break;
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

