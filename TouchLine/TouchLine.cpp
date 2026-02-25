//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "TouchLine.h"

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
	cs.AddTopic(STR_TARGET_TOPIC, ID_TARGET_TOPIC);
	cs.AddFloatSlider(	// R
		STR_VALUE,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		1,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_VALUE
	);
	cs.AddPopup(STR_THRESHOLD,
		STR_THRESHOLD_COUNT,
		STR_THRESHOLD_DFLT,
		STR_THRESHOLD_ITEMS,
		ID_THRESHOLD,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddFloatSlider(	// R
		STR_LUM_LEVEL,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_LUM_LEVEL
	);
	cs.AddFloatSlider(	// R
		STR_ALPHA_LEVEL,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_ALPHA_LEVEL,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddColor(	// color
		STR_TARGET_COLOR,
		{ 0xFF, 0x00, 0x00, 0x00 },
		ID_TARGET_COLOR,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(	// R
		STR_TARGET_COLOR_LEVEL,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_TARGET_COLOR_LEVEL,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddCheckBox(
		STR_DRAW_TARGET,
		"on",
		FALSE,
		ID_DRAW_TARGET
	);
	cs.EndTopic(ID_TARGET_TOPIC_END);
	// *******************************
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
	PF_Err				err = PF_Err_NONE,
		err2 = PF_Err_NONE;

	try {
		NF_AE ae;
		err = ae.HandleChangedParam(in_data, out_data, params, outputP, extraP, ID_NUM_PARAMS);
		if (!err) {
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				A_long tmode = 0;
				ERR(ae.GetPOPUP(ID_THRESHOLD, &tmode));
				hide_themB[ID_LUM_LEVEL] = !(tmode == 1);
				hide_themB[ID_ALPHA_LEVEL] = !(tmode == 2);
				hide_themB[ID_TARGET_COLOR] = !(tmode == 3);
				hide_themB[ID_TARGET_COLOR_LEVEL] = !(tmode == 3);
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				out_data->out_flags |= PF_OutFlag_REFRESH_UI;

			}

		}
	}
	catch (PF_Err& errP) {
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

	ERR(ae->GetFLOAT(ID_VALUE, &infoP->value));
	ERR(ae->GetPOPUP(ID_THRESHOLD, &infoP->threshold_mode));
	ERR(ae->GetFLOAT(ID_LUM_LEVEL, &infoP->lum_level));
	infoP->lum_level /= 100;
	ERR(ae->GetFLOAT(ID_ALPHA_LEVEL, &infoP->alpha_level));
	infoP->alpha_level /= 100;
	ERR(ae->GetCOLOR(ID_TARGET_COLOR, &infoP->target_color));
	ERR(ae->GetFLOAT(ID_TARGET_COLOR_LEVEL, &infoP->target_color_level));
	infoP->target_color_level /= 100;
	ERR(ae->GetCHECKBOX(ID_DRAW_TARGET, &infoP->draw_target));
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	std::vector<std::vector<float>> mask1(ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
	if(infoP->threshold_mode == 1) {
		 ERR(ThresholdLum(
			ae->in_data,
			ae->input,
			ae->pixelFormat(),
			ae->suitesP,
			 &mask1,
			 (float)infoP->lum_level
		));
	}
	else if(infoP->threshold_mode == 2) {
		ERR(ThresholdAlpha(
			ae->in_data,
			ae->input,
			ae->pixelFormat(),
			ae->suitesP,
			&mask1,
			 (float)infoP->alpha_level
		));
	}
	else if (infoP->threshold_mode == 3) {
		ERR(ThresholdColor(
			ae->in_data,
			ae->input,
			ae->pixelFormat(),
			ae->suitesP,
			&mask1,
			infoP->target_color,
			(float)infoP->target_color_level
		));
	}
	std::vector<std::vector<float>> mask = CalcMask(
		ae->suitesP,
		&mask1
	);
	std::vector<PF_Point> points = TargetMask(
		&mask,
		infoP->value,
		ae->frame()
	);

	if (infoP->draw_target) {
		ERR(DrawMask(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			&mask
		));
		if(infoP->isBlend) {
			ERR(BlendBehind(
				ae->in_data,
				ae->input,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP
			));
		}
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

