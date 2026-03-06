//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "BurstLines.h"

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err			err = PF_Err_NONE;

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddPoint(
		// center
		STR_LINE_CENTER,	//Name
		50,50,
		FALSE,
		ID_LINE_CENTER
	);
	cs.AddSlider(	// 
		STR_LINE_COUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		10000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		100,			//デフォルトの値
		ID_LINE_COUNT
	);
	
	cs.AddFloatSlider(	// R
		STR_DISTANCE,	//Name
		0,				//VALID_MIN
		9000,			//VALID_MAX
		0,				//SLIDER_MIN
		1000,			//SLIDER_MAX
		600,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_DISTANCE
	);
	cs.AddFloatSlider(	// R
		STR_LENGTH,	//Name
		0,				//VALID_MIN
		5000,			//VALID_MAX
		0,				//SLIDER_MIN
		1000,			//SLIDER_MAX
		500,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_LENGTH
	);
	cs.AddTopic(STR_PARAMS_TOIPC, ID_PARAMS_TOIPC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddSlider(	// 
		STR_LINE_SUB_COUNT,	//パラメータの名前
		1, 			//数値入力する場合の最小値
		100,		//数値入力する場合の最大値
		1,			//スライダーの最小値 
		10,			//スライダーの最大値
		3,			//デフォルトの値
		ID_LINE_SUB_COUNT
	);
	cs.AddFloatSlider(	// R
		STR_LINE_SUB_ANGLE,	//Name
		0,				//VALID_MIN
		45,				//VALID_MAX
		0,				//SLIDER_MIN
		15,				//SLIDER_MAX
		4,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_LINE_SUB_ANGLE
	);
	cs.AddFloatSlider(	// R
		STR_DISTANCE_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_DISTANCE_RAND
	);
	cs.AddFloatSlider(	// R
		STR_LENGTH_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_LENGTH_RAND
	);
	cs.AddFloatSlider(	// R
		STR_THICKNESS,	//Name
		0.5,			//VALID_MIN
		50,				//VALID_MAX
		0.5,			//SLIDER_MIN
		10,				//SLIDER_MAX
		3,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_THICKNESS
	);
	cs.AddFloatSlider(	// R
		STR_THICKNESS_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_THICKNESS_RAND
	);
	cs.AddFloatSlider(	// R
		STR_THICKNESS_START,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_THICKNESS_START
	);
	cs.AddFloatSlider(	// R
		STR_THICKNESS_END,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		75,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_THICKNESS_END
	);
	cs.AddFloatSlider(	// R
		STR_OPACITY_START,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_OPACITY_START
	);
	cs.AddFloatSlider(	// R
		STR_OPACITY_END,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		75,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_OPACITY_END
	);
	cs.AddAngle(	// angle
		STR_DIRECTION,
		0,
		ID_DIRECTION
	);
	cs.AddFloatSlider(	// R
		STR_ANGLE_RANGE,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_ANGLE_RANGE
	);
	cs.EndTopic(ID_PARAMS_TOIPC_END);
	// ***********************
	cs.AddTopic(STR_COLOR_TOIPC, ID_COLOR_TOIPC
	);
	cs.AddPopup(STR_COLOR_CNT,
		STR_COLOR_CNT_NUM,
		STR_COLOR_CNT_DFT,
		STR_COLOR_CNT_ITEMS,
		ID_COLOR_COUNT,
		PF_ParamFlag_SUPERVISE
	);
	PF_Pixel def_color[] = {
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		{ 0xFF, 0x00, 0x00, 0x00 },
		{ 0xFF, 0x80, 0x80, 0x80 },
		{ 0xFF, 0xFF, 0x00, 0x00 },
		{ 0xFF, 0x00, 0xFF, 0x00 },
		{ 0xFF, 0x00, 0x00, 0xFF },
		{ 0xFF, 0xFF, 0xFF, 0x00 },
		{ 0xFF, 0xFF, 0x00, 0xFF }
	};
	for (int i = 0; i < 8; i++) {
		PF_ParamUIFlags ui_flags = PF_PUI_INVISIBLE;
		if (i == 0) ui_flags = PF_PUI_NONE;
		cs.AddColor(	// color
			(STR_COLOR + std::to_string(i + 1)).c_str(),
			def_color[i],
			ID_COLOR_1 + i,
			PF_ParamFlag_NONE,
			ui_flags
		);
	}
	cs.EndTopic(ID_COLOR_TOIPC_END);
	// ***********************
	cs.AddCheckBox(	// noise frame
		"autoSeed",
		"on",
		FALSE,
		ID_AUTO_SEED,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddSlider(	// 
		"seed",	//パラメータの名前
		-30000, 				//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10000,			//スライダーの最大値
		0,			//デフォルトの値
		ID_SEED
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
			// ボタンが押されたときの処理
			/*if (extraP->param_index == ID_BUTTON)
			{
				A_char scriptCode[2048] = { '\0' };
				PF_SPRINTF(scriptCode, FS_ABOUT_DIALOG,
					NF_NAME,
					MAJOR_VERSION,
					MINOR_VERSION,
					__DATE__,
					NF_DESCRIPTION);

				ERR(ae.suitesP->UtilitySuite5()->AEGP_ExecuteScript(ae.ae_plugin_idP->my_id, scriptCode, TRUE, NULL, NULL));
				out_data->out_flags |= PF_OutFlag_REFRESH_UI;
			}*/
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				//とりあえず全てのパラメータを表示にする
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				for (A_long i = ID_COLOR_1; i <= ID_COLOR_8; i++) hide_themB[i] = TRUE;
				A_long mode;
				ERR(ae.GetPOPUP(ID_COLOR_COUNT, &mode));
				for (A_long i = 0; i < mode; i++) hide_themB[ID_COLOR_1+i] = FALSE;
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				if (!err) {
					out_data->out_flags |= PF_OutFlag_REFRESH_UI;
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
	PF_Err 	err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		//PF_ParamDef def;
		//AEFX_CLR_STRUCT(def);
		//ERR(ae.checkout_param(ID_AUTO_SEED,&def));
		PF_Boolean auto_seedB = FALSE;
		ae.GetCHECKBOX(ID_AUTO_SEED, &auto_seedB);
		ERR(ae.SetOutFlag_NON_PARAM_VARY(auto_seedB));
		//ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		//ERR(ae.checkin_param(&def));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOATPOINT(ID_LINE_CENTER, &infoP->center));
	ERR(ae->GetADD(ID_LINE_COUNT, &infoP->lineCount));
	ERR(ae->GetFLOAT_float(ID_DISTANCE, &infoP->distance));
	ERR(ae->GetFLOAT_float(ID_LENGTH, &infoP->length));

	ERR(ae->GetADD(ID_LINE_SUB_COUNT, &infoP->lineSubCount));
	ERR(ae->GetFLOAT_float(ID_LINE_SUB_ANGLE, &infoP->lineSubAngle));
	ERR(ae->GetFLOAT_float(ID_DISTANCE_RAND, &infoP->distanceRand));
	infoP->distanceRand /= 100.0f;
	ERR(ae->GetFLOAT_float(ID_LENGTH_RAND, &infoP->lengthRand));
	infoP->lengthRand /= 100.0f;
	ERR(ae->GetFLOAT_float(ID_THICKNESS, &infoP->thickness));
	ERR(ae->GetFLOAT_float(ID_THICKNESS_RAND, &infoP->thicknessRand));
	infoP->thicknessRand /= 100.0f;
	ERR(ae->GetFLOAT_float(ID_THICKNESS_START, &infoP->thick_start));
	infoP->thick_start /= 100.0f;
	ERR(ae->GetFLOAT_float(ID_THICKNESS_END, &infoP->thick_end));
	infoP->thick_end /= 100.0f;

	ERR(ae->GetFLOAT_float(ID_OPACITY_START, &infoP->opacity_start));
	infoP->opacity_start /= 100.0f;
	ERR(ae->GetFLOAT_float(ID_OPACITY_END, &infoP->opacity_end));
	infoP->opacity_end /= 100.0f;

	PF_Fixed d = 0;
	ERR(ae->GetANGLE(ID_DIRECTION, &d));
	infoP->direction = d / 65536.0f;
	ERR(ae->GetFLOAT_float(ID_ANGLE_RANGE, &infoP->angleRange));
	infoP->angleRange /= 100.0f;


	ERR(ae->GetPOPUP(ID_COLOR_COUNT, &infoP->colorCount));
	for (int i = 0; i < 8; i++) {
		ERR(ae->GetCOLOR(ID_COLOR_1 + i, &infoP->colors[i]));
	}
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->auto_seed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	infoP->seedAct = infoP->seed;
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
Exec(NF_AE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	PF_InData* in_data = ae->in_data;
	ERR(PF_FILL(NULL, NULL, ae->output));

	if (infoP->auto_seed == TRUE) {
		infoP->seedAct = infoP->seed + ae->frame();
	}
	ERR(BurstLinesExec(ae, infoP));

	ERR(Mult(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		TRUE
	));

#ifdef _DEBUG
#endif

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
		case PF_Cmd_FRAME_SETUP:
			//err = ae.FrameSetup(in_data, out_data, params, output, ID_NUM_PARAMS);
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

