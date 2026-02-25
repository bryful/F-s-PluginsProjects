//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "SpeedLines.h"

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
	cs.AddFloatSlider(
		STR_VALUE,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		10,				//SLIDER_MAX
		5,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_VALUE
	);
	 // *************************************************
	cs.AddTopic(STR_POS_TOPIC, ID_POS_TOPIC);
	cs.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE
	);

	cs.AddFloatSlider(
		STR_LENGTH1_VALUE,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		50,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH1_VALUE
	);
	cs.AddFloatSlider(
		STR_LENGTH1_FAR,	//Name
		100,			//VALID_MIN
		200,			//VALID_MAX
		100,			//SLIDER_MIN
		200,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH1_FAR
	);
	cs.AddFloatSlider(
		STR_LENGTH1_NEAR,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		35,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH1_NEAR
	);
	cs.AddFloatSlider(
		STR_LENGTH2_VALUE,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		50,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH2_VALUE
	);
	cs.AddFloatSlider(
		STR_LENGTH2_FAR,	//Name
		100,			//VALID_MIN
		200,			//VALID_MAX
		100,			//SLIDER_MIN
		200,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH2_FAR
	);
	cs.AddFloatSlider(
		STR_LENGTH2_NEAR,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		35,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH2_NEAR
	);
	cs.AddPoint(
		STR_CENTER,
		50,
		50,
		FALSE,
		ID_CENTER
	);
	cs.AddCheckBox(	// noise frame
		STR_SEED,
		"on",
		FALSE,
		ID_AUTO_SEED
	);
	cs.AddSlider(	// 
		STR_SEED,	//パラメータの名前
		-30000, 	//数値入力する場合の最小値
		30000,		//数値入力する場合の最大値
		0,			//スライダーの最小値 
		1000,		//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED
	);
	cs.EndTopic(ID_POS_TOPIC_END);
	// *************************************************
	cs.AddTopic(STR_LINE_TOPIC, ID_LINE_TOPIC);
	cs.AddFloatSlider(
		STR_LINE_LENGTH,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		80,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_LENGTH
	);
	cs.AddFloatSlider(
		STR_LINE_LENGTH_RAND,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_LENGTH_RAND
	);
	cs.AddFloatSlider(
		STR_LINE_SIZE,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		30,				//SLIDER_MAX
		15,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_SIZE
	);
	cs.AddFloatSlider(
		STR_LINE_SIZE_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		30,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_SIZE_RAND
	);
	cs.AddFloatSlider(
		STR_LINE_OPACITY,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_OPACITY
	);
	cs.AddFloatSlider(
		STR_LINE_OPACITY_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_OPACITY_RAND
	);
	cs.AddSlider(	// 
		STR_LINE_BLUR,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		20,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,			//スライダーの最大値
		0,				//デフォルトの値
		ID_LINE_BLUR
	);
	cs.AddFloatSlider(
		STR_LINE_NOISE,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LINE_NOISE
	);
	cs.EndTopic(ID_LINE_TOPIC_END);
	// *************************************************
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
	for(int i = 0; i < 8; i++) {
		std::string name = STR_COLOR +std::to_string(1+1);
		PF_ParamUIFlags ui_flags = PF_PUI_INVISIBLE;
		if(i==0) {
			ui_flags = PF_PUI_NONE;
		}
		cs.AddColor(
			name.c_str(),
			{ 255,(A_u_char)(36*i),(A_u_char)(36 * i),(A_u_char)(36 * i) },
			ID_COLOR1+i,
			PF_ParamFlag_NONE,
			ui_flags
		);
	}
	cs.EndTopic(ID_COLOR_TOPIC_END);
	// *************************************************
	cs.AddCheckBox(	// noise frame
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
		if (!err) 
		{
			ERR(ae.GetNewEffectStreamAll());
			if (!err) 
			{
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				A_long cc = 0;
				ERR(ae.GetPOPUP(ID_COLOR_COUNT, &cc));
				for (A_long i = ID_COLOR1; i <= ID_COLOR8; i++)hide_themB[i] = TRUE;
				for (A_long i = 0; i < cc; i++) hide_themB[ID_COLOR1 + i] = FALSE;


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
	PF_Fixed angle_fixed = 0;
	ERR(ae->GetANGLE(ID_ANGLE, &angle_fixed));
	infoP->angle = (float)F_FIX2FLT(angle_fixed);
	ERR(ae->GetFLOAT(ID_LENGTH1_VALUE, &infoP->length1_value));
	infoP->length1_value /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH1_FAR, &infoP->length1_far));
	infoP->length1_far /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH1_NEAR, &infoP->length1_near));
	infoP->length1_near /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH2_VALUE, &infoP->length2_value));
	infoP->length2_value /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH2_FAR, &infoP->length2_far));
	infoP->length2_far /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH2_NEAR, &infoP->length2_near));
	infoP->length2_near /= 100;
	ERR(ae->GetPOINT(ID_CENTER, &infoP->center));
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->is_autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));

	ERR(ae->GetADD(ID_LINE_BLUR, &infoP->blue));

	ERR(ae->GetFLOAT(ID_LINE_LENGTH, &infoP->length));
	infoP->length /= 100;
	ERR(ae->GetFLOAT(ID_LINE_LENGTH_RAND, &infoP->length_rand));
	infoP->length_rand /= 100;
	ERR(ae->GetFLOAT(ID_LINE_SIZE, &infoP->size));
	ERR(ae->GetFLOAT(ID_LINE_SIZE_RAND, &infoP->sizeRand));
	infoP->sizeRand /= 100;
	ERR(ae->GetFLOAT(ID_LINE_OPACITY, &infoP->opacity));
	infoP->opacity /= 100;
	ERR(ae->GetFLOAT(ID_LINE_OPACITY_RAND, &infoP->opacityRand));
	infoP->opacityRand /= 100;

	ERR(ae->GetFLOAT(ID_LINE_NOISE, &infoP->noise));
	infoP->noise /= 100;

	ERR(ae->GetPOPUP(ID_COLOR_COUNT, &infoP->colorCount));
	for(A_long i=0; i<8; i++) {
		ERR(ae->GetCOLOR(ID_COLOR1+i, &infoP->colors[i]));
	}
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	
	ERR(SpeedLineExec(ae, infoP));
	if (infoP->isBlend)
	{
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

