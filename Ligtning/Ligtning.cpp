//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Ligtning.h"

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
	cs.AddPopup(STR_MODE,
		STR_MODE_COUNT,
		STR_MODE_DFLT,
		STR_MODE_ITEMS,
		ID_MODE,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddCheckBox(	// noise frame
		STR_AUTO_SEED,
		"on",
		FALSE,
		ID_AUTO_SEED,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddSlider(	// 
		STR_SEED,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-1000,			//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED
	);
	// ******************************************************
	cs.AddPoint(
		STR_START_POS,
		25,
		25,
		FALSE,
		ID_START_POS
		);
	cs.AddPoint(
		STR_END_POS,
		75,
		75,
		FALSE,
		ID_END_POS
	);
	// ----------------------------------------------------------------
	cs.AddPath(
		STR_PATH,
		0,
		ID_PATH,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	// ----------------------------------------------------------------
	cs.AddPoint(
		STR_START1_POS,
		25,
		25,
		FALSE,
		ID_START1_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddPoint(
		STR_START2_POS,
		25,
		75,
		FALSE,
		ID_START2_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddPoint(
		STR_END1_POS,
		75,
		25,
		FALSE,
		ID_END1_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddPoint(
		STR_END2_POS,
		75,
		75,
		FALSE,
		ID_END2_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddSlider(	// 
		STR_2_2POINT_COUNT,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		100,			//スライダーの最大値
		4,				//デフォルトの値
		ID_2_2POINT_COUNT,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_START_RAND,//Name
		0,			//VALID_MIN
		1000,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_START_RAND,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_END_RAND,//Name
		0,			//VALID_MIN
		1000,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_END_RAND,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	// ----------------------------------------------------------------
	cs.AddPoint(
		STR_SRC_POS,
		25,
		50,
		FALSE,
		ID_SRC_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddPoint(
		STR_IMPACT1_POS,
		75,
		25,
		FALSE,
		ID_IMPACT1_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddPoint(
		STR_IMPACT2_POS,
		75,
		75,
		FALSE,
		ID_IMPACT2_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_IMPACT_RAND,//Name
		0,			//VALID_MIN
		2000,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_IMPACT_RAND,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddSlider(	// 
		STR_BRANCH_CNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		10,				//スライダーの最大値
		2,				//デフォルトの値
		ID_BRANCH_CNT,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	// ----------------------------------------------------------------
	cs.AddSlider(	// 
		STR_RADICAL_COUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		4,				//デフォルトの値
		ID_RADICAL_COUNT,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddAngle(
		STR_RADICAL_ANGLE,			//Name
		0,
		ID_RADICAL_ANGLE
	);
	cs.AddPoint(
		STR_CENTER_POS,
		50,
		50,
		FALSE,
		ID_CENTER_POS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_RADIUS,	//Name
		0,			//VALID_MIN
		2000,			//VALID_MAX
		0,				//SLIDER_MIN
		1000,			//SLIDER_MAX
		350,			//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_RADIUS,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_RADIUS_RAND,	//Name
		0,			//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,			//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_RADIUS_RAND,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	// ******************************************************
	cs.AddTopic(STR_PARAMS_TOPIC, ID_PARAMS_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_INVISIBLE
	);
	cs.AddFloatSlider(
		STR_WIPE,		//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		200,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_WIPE
	);
	cs.AddFloatSlider(
		STR_START_WEIGHT,//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		20,				//SLIDER_MAX
		5,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_START_WEIGHT
	);
	cs.AddFloatSlider(
		STR_END_WEIGHT,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		20,				//SLIDER_MAX
		5,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_END_WEIGHT
	);
	cs.AddFloatSlider(
		STR_WEIGHT,		//Name
		0,				//VALID_MIN
		300,			//VALID_MAX
		0,				//SLIDER_MIN
		50,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_WEIGHT
	);
	cs.AddSlider(	// noise offset
		STR_COMPLEXTTY,		//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,				//数値入力する場合の最大値
		0,				//スライダーの最小値 
		15,				//スライダーの最大値
		8,				//デフォルトの値
		ID_COMPLEXTTY
	);
	cs.AddFloatSlider(	// R
		STR_JAGGEDNESS,			//Name
		0,				//VALID_MIN
		500,			//VALID_MAX
		0,				//SLIDER_MIN
		150,				//SLIDER_MAX
		40,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_JAGGEDNESS
	);
	cs.AddAngle(
		STR_COMPL_ANGLE,			//Name
		0,
		ID_COMPL_ANGLE
	);
	cs.AddSlider(	// noise offset
		STR_FORK,		//パラメータの名前
		1, 				//数値入力する場合の最小値
		10,				//数値入力する場合の最大値
		1,				//スライダーの最小値 
		3,				//スライダーの最大値
		1,				//デフォルトの値
		ID_FORK
	);
	cs.AddColor(	// color
		STR_COLOR,
		{ 0xFF, 0xFF, 0xFF, 0x80 },
		ID_COLOR
	);
	cs.EndTopic(ID_PARAMS_TOPIC_END);
	// ******************************************************

	
	
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
				//とりあえず全てのパラメータを非表示にする
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = TRUE;
				// 常時表示はここでFALSEにする
				hide_themB[ID_MODE] = FALSE;
				hide_themB[ID_AUTO_SEED] = FALSE;
				hide_themB[ID_SEED] = FALSE;
				for (A_long i = ID_PARAMS_TOPIC; i <= ID_PARAMS_TOPIC_END; i++) hide_themB[i] = FALSE;

				A_long mode;
				ERR(ae.GetPOPUP(ID_MODE, &mode));
				switch (mode) {
					case MODE_2POINT:
						hide_themB[ID_START_POS] = FALSE;
						hide_themB[ID_END_POS] = FALSE;
						break;
					case MODE_PATH:
						hide_themB[ID_PATH] = FALSE;
						break;
					case MODE_2_2POINT:
						hide_themB[ID_START1_POS] = FALSE;
						hide_themB[ID_START2_POS] = FALSE;
						hide_themB[ID_END1_POS] = FALSE;
						hide_themB[ID_END2_POS] = FALSE;
						hide_themB[ID_2_2POINT_COUNT] = FALSE;
						hide_themB[ID_START_RAND] = FALSE;
						hide_themB[ID_END_RAND] = FALSE;
						break;
					case MODE_1_2POINT:
						hide_themB[ID_SRC_POS] = FALSE;
						hide_themB[ID_IMPACT1_POS] = FALSE;
						hide_themB[ID_IMPACT2_POS] = FALSE;
						hide_themB[ID_IMPACT_RAND] = FALSE;
						hide_themB[ID_BRANCH_CNT] = FALSE;
						break;
					case MODE_RADICAL:
						hide_themB[ID_RADICAL_COUNT] = FALSE;
						hide_themB[ID_RADICAL_ANGLE] = FALSE;
						hide_themB[ID_CENTER_POS] = FALSE;
						hide_themB[ID_RADIUS] = FALSE;
						hide_themB[ID_RADIUS_RAND] = FALSE;
						break;

				}
				// パラメータの表示/非表示を切り替える
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				if (!err && in_data->appl_id != 'PrMr') {
					/*
					PF_Boolean b = FALSE;
					ERR(ae.GetCHECKBOX(ID_AUTO_SEED, &b));
					ERR(ae.UI_DISABLE(ID_SEED, !b));
					*/
				}
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
	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->auto_seed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	infoP->seedAct = infoP->seed;

	ERR(ae->GetPOINT(ID_START_POS, &infoP->posTwin[0]));
	ERR(ae->GetPOINT(ID_END_POS, &infoP->posTwin[1]));

	ERR(ae->GetPOINT(ID_START1_POS, &infoP->posStart[0]));
	ERR(ae->GetPOINT(ID_START2_POS, &infoP->posStart[1]));
	ERR(ae->GetPOINT(ID_END1_POS, &infoP->posEnd[0]));
	ERR(ae->GetPOINT(ID_END2_POS, &infoP->posEnd[1]));
	ERR(ae->GetADD(ID_2_2POINT_COUNT, &infoP->Count2_2));
	ERR(ae->GetFLOAT(ID_START_RAND, &infoP->startRand));
	ERR(ae->GetFLOAT(ID_END_RAND, &infoP->endRand));

	ERR(ae->GetPOINT(ID_SRC_POS, &infoP->posImpact[0]));
	ERR(ae->GetPOINT(ID_IMPACT1_POS, &infoP->posImpact[1]));
	ERR(ae->GetPOINT(ID_IMPACT2_POS, &infoP->posImpact[2]));
	ERR(ae->GetFLOAT(ID_IMPACT_RAND, &infoP->impactRand));
	ERR(ae->GetADD(ID_BRANCH_CNT, &infoP->impactBranch));

	ERR(ae->GetADD(ID_RADICAL_COUNT, &infoP->radicalCount));
	PF_Fixed angle2 = 0;
	ERR(ae->GetANGLE(ID_RADICAL_ANGLE, &angle2));
	infoP->radicalAngle = angle2 / 65536.0;
	ERR(ae->GetPOINT(ID_CENTER_POS, &infoP->center));
	ERR(ae->GetFLOAT(ID_RADIUS, &infoP->radius));
	ERR(ae->GetFLOAT(ID_RADIUS_RAND, &infoP->radiusRand));
	infoP->radiusRand /= 100.0;

	ERR(ae->GetFLOAT(ID_WIPE, &infoP->wipe));
	infoP->wipe /= 100.0;
	ERR(ae->GetFLOAT(ID_START_WEIGHT, &infoP->startWeight));
	ERR(ae->GetFLOAT(ID_END_WEIGHT, &infoP->endWeight));
	ERR(ae->GetFLOAT(ID_WEIGHT, &infoP->weight));
	ERR(ae->GetADD(ID_COMPLEXTTY, &infoP->complexity));
	ERR(ae->GetFLOAT(ID_JAGGEDNESS, &infoP->jaggedness));
	infoP->jaggedness /= 100.0;
	PF_Fixed angle = 0;
	ERR(ae->GetANGLE(ID_COMPL_ANGLE, &angle));
	infoP->comple_angle = angle / 65536.0;
	ERR(ae->GetADD(ID_FORK, &infoP->fork));
	ERR(ae->GetCOLOR(ID_COLOR, &infoP->color));

	
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;
	ERR(PF_FILL(NULL, NULL, ae->output));

	if (infoP->weight <= 0) return err;

	ERR(LigtningDraw(ae, infoP));

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

