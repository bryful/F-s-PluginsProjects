//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Gradient.h"

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
	cs.AddPopup(STR_MODE_POP,
		STR_MODE_COUNT,
		STR_MODE_DFLT,
		STR_MODE_ITEMS,
		ID_MODE_POP
	);
	cs.AddPoint(
		STR_START_POS,
		25,
		25,
		FALSE,
		ID_START_POS
	);
	cs.AddColor(	// color
		STR_START_COLOR,
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_START_COLOR
	);
	cs.AddFloatSlider(	// R
		STR_START_OPACITY,			//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_START_OPA
	);
	cs.AddPoint(
		STR_END_POS,
		75,
		75,
		FALSE,
		ID_END_POS
	);
	cs.AddColor(	// color
		STR_END_COLOR,
		{ 0xFF, 0x00, 0x00, 0x00 },
		ID_END_COLOR
	);
	cs.AddFloatSlider(	// R
		STR_END_OPACITY,			//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_END_OPA
	);
	cs.AddFloatSlider(	// R
		STR_alpha_hyperbolic,			//Name
		-10,			//VALID_MIN
		50,				//VALID_MAX
		-2,				//SLIDER_MIN
		10,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_ID_ALPHA_HYPERBOLIC
	);
	cs.AddFloatSlider(	// R
		STR_rgb_hyperbolic,			//Name
		-10,			//VALID_MIN
		50,				//VALID_MAX
		-2,				//SLIDER_MIN
		10,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_ID_RGB_HYPERBOLIC
	);
	

	cs.AddButton(	// button
		STR_SWAP_COLOR_BTN,
		STR_SWAP_COLOR_BTN2,
		ID_SWAP_COLOR_BTN,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddCheckBox(
		STR_BLEND,
		"on",
		FALSE,
		ID_BLEND
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
			if (extraP->param_index == ID_SWAP_COLOR_BTN)
			{
				PF_Pixel start_color, end_color;
				PF_FpLong start_opa,end_opa;
				ERR(ae.GetCOLOR(ID_START_COLOR, &start_color));
				ERR(ae.GetCOLOR(ID_END_COLOR, &end_color));
				ERR(ae.GetFLOAT(ID_START_OPA, &start_opa));
				ERR(ae.GetFLOAT(ID_END_OPA, &end_opa));

				ERR(ae.SetCOLOR(ID_START_COLOR, end_color));
				ERR(ae.SetCOLOR(ID_END_COLOR, start_color));
				ERR(ae.SetFLOAT(ID_START_OPA, end_opa));
				ERR(ae.SetFLOAT(ID_END_OPA, start_opa));


				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
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

	ERR(ae->GetPOPUP(ID_MODE_POP, &infoP->mode));
	
	ERR(ae->GetFLOATPOINT(ID_START_POS, &infoP->start_pos));
	ERR(ae->GetCOLOR(ID_START_COLOR, &infoP->start_color));
	ERR(ae->GetFLOAT(ID_START_OPA, &infoP->start_opa));

	ERR(ae->GetFLOATPOINT(ID_END_POS, &infoP->end_pos));
	ERR(ae->GetCOLOR(ID_END_COLOR, &infoP->end_color));
	ERR(ae->GetFLOAT(ID_END_OPA, &infoP->end_opa));

	ERR(ae->GetFLOAT(ID_ID_ALPHA_HYPERBOLIC, &infoP->alpha_hyperbolic));
	ERR(ae->GetFLOAT(ID_ID_RGB_HYPERBOLIC, &infoP->rgb_hyperbolic));
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	err =Gradient(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		infoP->mode,
		infoP->start_pos,
		infoP->start_color,
		infoP->start_opa,
		infoP->end_pos,
		infoP->end_color,
		infoP->end_opa,
		infoP->rgb_hyperbolic,
		infoP->alpha_hyperbolic
		);

	if (infoP->isBlend)
	{
		ERR(BlendBehind(
			ae->in_data,
			ae->input, 
			ae->output, 
			ae->pixelFormat(), ae->suitesP));
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

