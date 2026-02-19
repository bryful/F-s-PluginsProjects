//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "LineExtraction.h"

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
	NF_ParamsSetup cs(in_data, out_data, ID_NUM_PARAMS);

	// ****************************
	cs.AddTopic(STR_PRE_TOPIC, ID_PRE_TOPIC);
	cs.AddCheckBox(
		STR_BILATERAL_CB,
		"on",
		TRUE,
		ID_BILATERAL_CB
	);
	cs.AddSlider(	// 
		STR_B_RADIUS,	//パラメータの名前
		1, 		//数値入力する場合の最小値
		10,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		10,			//スライダーの最大値
		2,				//デフォルトの値
		ID_B_RADIUS
	);
	cs.AddFloatSlider(	// R
		STR_B_SIGMA_S,			//Name
		0.01f,			//VALID_MIN
		20,				//VALID_MAX
		0.0f,				//SLIDER_MIN
		10,				//SLIDER_MAX
		1,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_B_SIGMA_S
	);
	cs.AddFloatSlider(	// R
		STR_B_SIGMA_R,			//Name
		0.1f,				//VALID_MIN
		100,			//VALID_MAX
		1,				//SLIDER_MIN
		75,				//SLIDER_MAX
		15,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_B_SIGMA_R
	);
	

	cs.EndTopic(ID_PRE_TOPIC_END);
	// ****************************
	cs.AddSlider(	// 
		STR_OUTER_WIDTH,	//パラメータの名前
		0, 					//数値入力する場合の最小値
		100,				//数値入力する場合の最大値
		0,					//スライダーの最小値 
		10,					//スライダーの最大値
		1,					//デフォルトの値
		ID_OUTER_WIDTH
	); cs.AddSlider(	// 
		STR_INNER_WIDTH,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,				//スライダーの最大値
		1,				//デフォルトの値
		ID_INNER_WIDTH
	);
	
	cs.AddColor(	// color
		STR_COLOR,
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_COLOR
	);
	cs.AddCheckBox(
		STR_POST_LEVEL_CB,
		"on",
		TRUE,
		ID_POST_LEVEL_CB
	);
	cs.AddFloatSlider(	// R
		STR_POST_LEVEL_LO,			//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		30,				//SLIDER_MAX
		5,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_POST_LEVEL_LO
	);
	cs.AddFloatSlider(	// R
		STR_POST_LEVEL_HI,			//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		40,				//SLIDER_MIN
		100,			//SLIDER_MAX
		90,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_POST_LEVEL_HI
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
	ERR(ae->GetCHECKBOX(ID_BILATERAL_CB, &infoP->isBilateral));
	ERR(ae->GetADD(ID_B_RADIUS, &infoP->bilateralCtx.radius));
	double dd=0;
	ERR(ae->GetFLOAT(ID_B_SIGMA_S, &dd));
	infoP->bilateralCtx.sigma_s = (float)dd;
	dd = 0;
	ERR(ae->GetFLOAT(ID_B_SIGMA_R, &dd));
	infoP->bilateralCtx.sigma_r = (float)(dd / 100);
	if (infoP->bilateralCtx.sigma_r < 0) { infoP->bilateralCtx.sigma_r = 0; }
	else if (infoP->bilateralCtx.sigma_r > 1) { infoP->bilateralCtx.sigma_r = 1; }

	ERR(ae->GetCHECKBOX(ID_POST_LEVEL_CB, &infoP->isLevel));
	ERR(ae->GetFLOAT(ID_POST_LEVEL_LO, &infoP->preLevelLo));
	ERR(ae->GetFLOAT(ID_POST_LEVEL_HI, &infoP->preLevelHi));
	infoP->preLevelLo /= 100;
	infoP->preLevelHi /= 100;
	if (infoP->preLevelLo > infoP->preLevelHi)
	{
		double t = infoP->preLevelLo;
		infoP->preLevelLo = infoP->preLevelHi;
		infoP->preLevelHi = t;
	}
	else if (infoP->preLevelLo == infoP->preLevelHi) {
		infoP->preLevelHi = infoP->preLevelLo+0.01;
	}


	ERR(ae->GetADD(ID_OUTER_WIDTH, &infoP->outerWidth));
	ERR(ae->GetADD(ID_INNER_WIDTH, &infoP->innerWidth));
	ERR(ae->GetCOLOR(ID_COLOR, &infoP->color));
	infoP->color16 = NF_Pixel8To16(infoP->color);
	infoP->color32 = NF_Pixel8To32(infoP->color);
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Exec(NF_AE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	infoP->outerWidth = (A_long)((PF_FpLong)infoP->outerWidth / ae->downSaleValue() + 0.5);
	infoP->innerWidth = (A_long)((PF_FpLong)infoP->innerWidth / ae->downSaleValue() + 0.5);
	if (infoP->isBilateral) {
		ERR(PF_BilateralExec(ae, infoP));
	}
	else {
		ae->CopyInToOut();
	}
	ERR(FilterImage(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP
	));
	
	if (infoP->outerWidth > 0)
	{
		ERR(ChannelMinMax(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->outerWidth,
			0 //red
		));
	}
	if (infoP->innerWidth > 0)
	{
		ERR(ChannelMinMax(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->innerWidth * -1,
			1 //green
		));

	}
	
	if (infoP->innerWidth > 0 || infoP->outerWidth > 0) {
		ERR(FilterSubImage(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->color
		));

		if (infoP->isLevel) {
			ERR(LevelExec(
				ae->in_data,
				ae->output,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				infoP->preLevelHi,
				infoP->preLevelLo
			));
		}
		if (infoP->isBlend) {
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

