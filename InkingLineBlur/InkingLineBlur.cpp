//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "InkingLineBlur.h"

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
	cs.AddTopic(STR_INKING_TOPIC, ID_INKING_TOPIC);
	cs.AddPopup(
		STR_INKING_COUNT,
		STR_INKING_NUMS,
		STR_INKING_DFLT,
		STR_INKING_ITEMS,
		ID_INKING_COUNT,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
		);
	PF_ParamFlags pf = PF_ParamFlag_NONE;
	PF_ParamUIFlags uif = PF_PUI_NONE;
	for(int i=0; i< COLOR_INKING_COUNT;i++) {
		std::string str = std::string(STR_INKING_COLOR) +std::to_string(i+1);
		if (i >0)
		{
			uif = PF_PUI_INVISIBLE;
		}
		cs.AddColor(	// color
			str.c_str(),
			{ 0xFF, (A_u_char)(0x00 + i * 10), (A_u_char)(0x00 + i * 10), (A_u_char)(0x00 + i * 10) },
			ID_INKING_COLOR(i),
			pf,
			uif
		);
	}
	// ----------------------------------------------------------------
	cs.AddPopup(
		STR_INKING_COLOR_MODE,
		2,
		1,
		STR_INKING_COLOR_ITEMS,
		ID_INKING_COLOR_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddColor(	// color
		STR_CUSTAM_COLOR,
		{ 0xFF, 0x35, 0x35, 0x35 },
		ID_CUSTUM_COLOR,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.EndTopic(ID_INKING_TOPIC_END);
	
	cs.AddSlider(	// 
		STR_MINMAX,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		1,				//デフォルトの値
		ID_MINMAX
	);
	cs.AddSlider(	// noise offset
		STR_BLUR,		//パラメータの名前
		0, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		0,				//デフォルトの値
		ID_BLUR
	);
	cs.AddFloatSlider(		// 
		STR_HYPERBOLIC,	//パラメータの名前
		-10,			//VALID_MIN
		10,				//VALID_MAX
		-10,			//SLIDER_MIN
		10,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_HYPERBOLIC
	);
	
	cs.AddTopic(STR_TARGET_TOPIC, ID_TARGET_TOPIC);
	cs.AddPopup(
		STR_TARGET_MODE,
		9,
		9,
		STR_TARGET_ITEMS,
		ID_TARGET_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	for (int i = 0; i < 8; i++) {
		uif = PF_PUI_INVISIBLE;
		std::string str = std::string(STR_TARGET_COLOR) + std::to_string(i + 1);
		cs.AddColor(	// color
			str.c_str(),
			{ 0xFF, (A_u_char)(0xFF - i * 10), (A_u_char)(0xFF - i * 10), 0xFF },
			ID_TARGET_COLOR(i),
			pf,
			uif
		);
	}
	cs.EndTopic(ID_TARGET_TOPIC_END);
	//cs.Finalize();
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
	PF_Err				err = PF_Err_NONE;
	
	try{
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
			err=ae.GetNewEffectStreamAll();
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;

				for (A_long i = 0; i < COLOR_INKING_COUNT; i++) hide_themB[ID_INKING_COLOR(i)] = TRUE;
				A_long c;
				ae.GetPOPUP(ID_INKING_COUNT, &c);
				if (c >0) {
					for (A_long i = 0; i < c; i++) {
						hide_themB[ID_INKING_COLOR(i)] = FALSE;
					}
				}
				c = 1;
				ae.GetPOPUP(ID_INKING_COLOR_MODE, &c);
				hide_themB[ID_CUSTUM_COLOR] = (c==1);
				for (A_long i = 0; i < COLOR_TARGET_COUNT; i++) hide_themB[ID_TARGET_COLOR(i)] = TRUE;
				ae.GetPOPUP(ID_TARGET_MODE, &c);
				if (c >= 1 && c <= 8) {
					for (A_long i = 0; i < c; i++) {
						hide_themB[ID_TARGET_COLOR(i)] = FALSE;
					}
				}
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
				{
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				}
				out_data->out_flags |=  PF_OutFlag_REFRESH_UI;
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

	ERR(ae->GetPOPUP(ID_INKING_COUNT, &infoP->inking_count));

	for(int i = 0; i < COLOR_INKING_COUNT; i++) {
		ERR(ae->GetCOLOR(ID_INKING_COLOR(i), &infoP->inking_colors[i]));
	}
	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));

	ERR(ae->GetPOPUP(ID_INKING_COLOR_MODE, &infoP->inking_color_mode));
	ERR(ae->GetCOLOR(ID_CUSTUM_COLOR, &infoP->custum_color));

	ERR(ae->GetPOPUP(ID_TARGET_MODE, &infoP->target_mode));
	if (infoP->target_mode >= 9) {
		infoP->isAll = TRUE;
		//infoP->target_mode = 0;
	}
	else {
		infoP->isAll = FALSE;
	}
	for (int i = 0; i < COLOR_TARGET_COUNT; i++) {
		ERR(ae->GetCOLOR(ID_TARGET_COLOR(i), &infoP->target_color[i]));
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;

	A_long min_max =  (A_long)((double)infoP->minmax * ae->downSaleValue()+0.5);
	A_long blur = (A_long)((double)infoP->blur * ae->downSaleValue() + 0.5);
	if((min_max ==0) && (blur ==0)) {
		PF_Pixel cl = { 0,0,0,0 };
		PF_FILL(&cl, NULL,ae->output);
		return err;
	}
	//画面をコピー
	//ERR(ae->CopyInToOut());
	ERR(SelectPixelsExec(
		ae->in_data, ae->input, 
		ae->output, 
		ae->pixelFormat(), 
		ae->suitesP, 
		infoP->inking_count, 
		infoP->inking_colors, 
		infoP->inking_color_mode, 
		infoP->custum_color
		));
	if (min_max!=0 || blur >0) {
		ERR(Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, FALSE));
	}
	if(min_max !=0) {
		ERR(MinMax(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, min_max));
	}
	if (blur > 0) {
		ERR(Blur(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, blur));
	}
	if (min_max != 0 || blur > 0) {
		ERR(Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, TRUE));
	}
	if (infoP->hyperbolic != 0.0) {
		ERR(HyperbolicAlpha(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->hyperbolic));
	}
	ERR(TargetPixelsExec(
		ae->in_data, 
		ae->input,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		infoP->target_mode,
		infoP->target_color,
		infoP->inking_count,
		infoP->inking_colors,
		infoP->isAll
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

