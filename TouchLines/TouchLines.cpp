//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "TouchLines.h"

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
	// *****************************************************************
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
		80,				//DFLT
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
		50,				//DFLT
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
		30,				//DFLT
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
	// *****************************************************************
	cs.AddTopic(STR_MODE_TOPIC, ID_MODE_TOPIC);
	cs.AddPopup(STR_MODE,
		STR_MODE_COUNT,
		STR_MODE_DFLT,
		STR_MODE_ITEMS,
		ID_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE,
		PF_ParamFlag_NONE,
		PF_PUI_NONE
	);
	cs.AddPoint(
		STR_CENTER,
		50,
		50,
		FALSE,
		ID_CENTER,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_INNER_LENGTH,	//Name
		0,				//VALID_MIN
		300,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_INNER_LENGTH
	);
	cs.AddFloatSlider(
		STR_OUTER_LENGTH,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		25,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_OUTER_LENGTH
	);
	cs.AddFloatSlider(
		STR_LENGTH_RAND,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH_RAND
	);
	cs.AddFloatSlider(
		STR_SIZE,	//Name
		0,				//VALID_MIN
		50,				//VALID_MAX
		0,				//SLIDER_MIN
		25,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SIZE
	);
	cs.AddFloatSlider(
		STR_SIZE_RAND,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_SIZE_RAND
	);
	cs.AddFloatSlider(
		STR_OPACITY,	//Name
		0,				//VALID_MIN
		300,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_OPACITY
	);
	cs.AddFloatSlider(
		STR_OPACITY_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_OPACITY_RAND
	);
	cs.AddSlider(
		STR_BLUR,	//Name
		0,			//VALID_MIN
		50,			//VALID_MAX
		0,			//SLIDER_MIN
		5,			//SLIDER_MAX
		0,				//DFLT
		ID_BLUR
	);
	cs.AddFloatSlider(
		STR_NOISE,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE
	);
	cs.AddCheckBox(
		STR_AUTO_SEED,	//Name
		"on",
		FALSE,
		ID_AUTO_SEED
	);
	cs.AddSlider(
		STR_SEED,	//Name
		-30000,			//VALID_MIN
		30000,			//VALID_MAX
		-10000,			//SLIDER_MIN
		10000,			//SLIDER_MAX
		0,				//DFLT
		ID_SEED
	);

	cs.EndTopic(ID_MODE_TOPIC_END);
	// *****************************************************************
	cs.AddTopic(STR_COLOR_TOPIC, ID_COLOR_TOPIC);
	cs.AddPopup(STR_COLOR_COUNT,
		8,
		1,
		STR_COLOR_ITEMS,
		ID_COLOR_COUNT,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	PF_Pixel default_colors[8] = {
		{ 0xFF, 0x00, 0x00, 0x00 },
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		{ 0xFF, 0xFF, 0xFF, 0x00 },
		{ 0xFF, 0x00, 0xFF, 0xFF },
		{ 0xFF, 0xFF, 0x00, 0x00 },
		{ 0xFF, 0x00, 0xFF, 0x00 },
		{ 0xFF, 0x00, 0x00, 0xFF },
		{ 0xFF, 0x7F, 0x7F, 0x7F }
	};
	for(A_long i=0; i<8;i++) {
		std::string name = STR_COLOR + std::to_string(i + 1);
		PF_ParamUIFlags ui_flags = PF_PUI_INVISIBLE;
		if (i==0) {
			ui_flags = PF_PUI_NONE;
		}
		cs.AddColor(	// color
			name.c_str(),
			default_colors[i],
			ID_COLOR_1+i,
			PF_ParamFlag_NONE,
			ui_flags
		);
	}
	cs.EndTopic(ID_COLOR_TOPIC_ND);
	// *****************************************************************
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
				A_long mode = 0;
				ERR(ae.GetPOPUP(ID_MODE, &mode));
				hide_themB[ID_ANGLE] = !(mode == 1);
				hide_themB[ID_CENTER] = !(mode == 2);
				A_long ccount = 0;
				ERR(ae.GetPOPUP(ID_COLOR_COUNT, &ccount));
				for (A_long i = 0; i < 8; i++) hide_themB[ID_COLOR_1+i] = TRUE;
				for (A_long i = 0; i < ccount; i++) hide_themB[ID_COLOR_1 + i] = FALSE;

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
	PF_Err 	err2 = PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data, out_data, params, extra, ID_NUM_PARAMS);
	if (!err) {
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_AUTO_SEED, &def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_VALUE, &infoP->value));
	infoP->value /= 100;
	ERR(ae->GetPOPUP(ID_THRESHOLD, &infoP->threshold_mode));
	ERR(ae->GetFLOAT(ID_LUM_LEVEL, &infoP->lum_level));
	infoP->lum_level /= 100;
	ERR(ae->GetFLOAT(ID_ALPHA_LEVEL, &infoP->alpha_level));
	infoP->alpha_level /= 100;
	ERR(ae->GetCOLOR(ID_TARGET_COLOR, &infoP->target_color));
	ERR(ae->GetFLOAT(ID_TARGET_COLOR_LEVEL, &infoP->target_color_level));
	infoP->target_color_level /= 100;
	ERR(ae->GetCHECKBOX(ID_DRAW_TARGET, &infoP->draw_target));

	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));
	PF_Fixed angle=0;
	ERR(ae->GetANGLE(ID_ANGLE, &angle));
	infoP->angle = F_FIX2FLT(angle);
	ERR(ae->GetPOINT(ID_CENTER, &infoP->center));


	ERR(ae->GetFLOAT(ID_INNER_LENGTH, &infoP->inner_length));
	infoP->inner_length /= 100;
	ERR(ae->GetFLOAT(ID_OUTER_LENGTH, &infoP->outer_length));
	infoP->outer_length /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH_RAND, &infoP->length_rand));
	infoP->length_rand /= 100;
	ERR(ae->GetFLOAT(ID_SIZE, &infoP->size));
	ERR(ae->GetFLOAT(ID_SIZE_RAND, &infoP->size_rand));
	infoP->size_rand /= 100;
	ERR(ae->GetFLOAT(ID_OPACITY, &infoP->opacity));
	infoP->opacity /= 100;
	ERR(ae->GetFLOAT(ID_OPACITY_RAND, &infoP->opacity_rand));
	infoP->opacity_rand /= 100;
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));
	ERR(ae->GetFLOAT(ID_NOISE, &infoP->noise));
	infoP->noise /= 100;


	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	infoP->seedAct = infoP->seed;
	
	ERR(ae->GetPOPUP(ID_COLOR_COUNT, &infoP->colorCount));

	for(A_long i=0; i<8; i++) {
		ERR(ae->GetCOLOR(ID_COLOR_1 + i, &infoP->colors[i]));
	}

	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;
	infoP->size = ae->downScale(infoP->size);
	infoP->blur = ae->downScale(infoP->blur);
	ERR(PF_FILL(NULL, NULL, ae->output));
	if (infoP->value <= 0 || infoP->opacity <= 0 || infoP->size <= 0
		|| (infoP->inner_length<=0 && infoP->outer_length<=0)) {
		if(infoP->isBlend) {
			ERR(BlendBehind(
				ae->in_data,
				ae->input,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP
			));
		}
		return err;
	}
	if(infoP->autoSeed) {
		
		infoP->seedAct = infoP->seed + ae->frame();
	}
	else {
		infoP->seedAct = infoP->seed;
	}
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
		infoP->value/20,
		infoP->seedAct
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
		return err;
	}
	if (infoP->mode == 1)
	{
		ERR(TouchStraight(
			ae,
			infoP,
			points
		));

	}
	else {
		ERR(TouchCenter(
			ae,
			infoP,
			points
		));
	}
	if (infoP->blur > 0) {
		ERR(Blur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->blur
		));
	}
	ERR(Mult(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		TRUE
	));
	if (infoP->noise > 0) {
		A_long count = (A_long)((double)ae->outputInfo.width * (double)ae->outputInfo.height * infoP->noise / 8);
		ERR(ApplyANoise(ae, count, infoP->seedAct));
	}
	if(infoP->isBlend) {
		ERR(BlendBehind(
			ae->in_data,
			ae->input,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP
		));
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

