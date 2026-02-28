//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Threshold.h"

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
	std::vector<std::string> names = { STR_LUMA_LEVEL,STR_RED,STR_GREEN,STR_BLUE,STR_ALPHA };
	std::vector<std::string> names2 = { STR_LUMA_LEVEL,STR_RED_LEVEL,STR_GREEN_LEVEL,STR_BLUE_LEVEL,STR_ALPHA_LEVEL };

	cs.AddPopup(STR_MODE,
		2,
		1,
		STR_MODE_ITEMS,
		ID_MODE,
		PF_ParamFlag_SUPERVISE
	);
	for (int i = 0; i < 5; i++) {
		PF_Boolean cd = FALSE;
		if (i == 4)cd = TRUE;
		PF_ParamUIFlags ui_flags = PF_PUI_INVISIBLE;
		if (i == 0 || i == 4)ui_flags = PF_PUI_NONE;
		cs.AddCheckBox(	// noise frame
			names[i].c_str(),
			"on",
			cd,
			ID_LUMA_CB + i,
			PF_ParamFlag_SUPERVISE,
			ui_flags
		);
	}
	for (int i = 0; i < 5; i++) {
		PF_ParamUIFlags ui_flags = PF_PUI_INVISIBLE;
		if ( i == 4)ui_flags = PF_PUI_NONE;
		cs.AddFloatSlider(	// R
			names2[i].c_str(),			//Name
			0,				//VALID_MIN
			100,			//VALID_MAX
			0,				//SLIDER_MIN
			100,			//SLIDER_MAX
			50,				//DFLT
			1,				//PREC 小数点以下の桁数
			0,				//DISP
			FALSE,			//WANT_PHASE
			ID_LUMA_LEVEL +i,
			PF_ParamFlag_NONE,
			ui_flags
		);
	}


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
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = TRUE;
				hide_themB[ID_MODE] = FALSE;
				hide_themB[ID_ALPHA_CB] = FALSE;
				A_long mode=0;
				PF_Boolean b=FALSE;
				ERR(ae.GetPOPUP(ID_MODE, &mode));
				if (mode == 1) {
					hide_themB[ID_LUMA_CB] = FALSE;
					ERR(ae.GetCHECKBOX(ID_LUMA_CB, &b));
					hide_themB[ID_LUMA_LEVEL] = !b;
				}
				else if (mode == 2) {
					hide_themB[ID_RED_CB] = FALSE;
					hide_themB[ID_GREEN_CB] = FALSE;
					hide_themB[ID_BLUE_CB] = FALSE;
					ERR(ae.GetCHECKBOX(ID_RED_CB, &b));
					if (b) {
						hide_themB[ID_RED_LEVEL] = FALSE;
					}
					ERR(ae.GetCHECKBOX(ID_GREEN_CB, &b));
					if (b) {
						hide_themB[ID_GREEN_LEVEL] = FALSE;
					}
					ERR(ae.GetCHECKBOX(ID_BLUE_CB, &b));
					if (b) {
						hide_themB[ID_BLUE_LEVEL] = FALSE;
					}
				}
				
				hide_themB[ID_ALPHA_CB] = FALSE;
				ERR(ae.GetCHECKBOX(ID_ALPHA_CB, &b));
				hide_themB[ID_ALPHA_LEVEL] = !b;

				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));


			}
			out_data->out_flags |= PF_OutFlag_REFRESH_UI;

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

	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));
	for(int i=0;i<5;i++)
	{
		ERR(ae->GetCHECKBOX(ID_LUMA_CB + i, &infoP->enabled[i]));
		ERR(ae->GetFLOAT(ID_LUMA_LEVEL + i, &infoP->level[i]));
		infoP->level[i] /= 100;
	}
	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	ERR(FilterImage(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		infoP));

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

