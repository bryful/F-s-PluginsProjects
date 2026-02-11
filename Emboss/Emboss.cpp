//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Emboss.h"

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
	cs.AddAngle(	// angle
		STR_DIRECTION,
		45,
		ID_DIRECTION//,
		//PF_ParamFlag_NONE,
		//PF_PUI_DISABLED
	);
	cs.AddFloatSlider(
		STR_DISTANCE,			//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		50,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_DISTANCE
	);
	/*
	// ----------------------------------------------------------------
	cs.AddCheckBox(	// noise frame
		STR_PAINT_CB,
		"on",
		FALSE,
		ID_PAINT_CB
	);
	cs.AddPoint(
		STR_PAINT_POS,
		25,
		25,
		FALSE,
		ID_PAINT_POS
	);
	cs.AddColor(	// color
		STR_PAINT_COLOR,
		{ 0xFF, 0xFF, 0x00, 0xFF },
		ID_PAINT_COLOR
	);
	// ----------------------------------------------------------------
	cs.AddSlider(	// 
		STR_MINMAX,	//パラメータの名前
		-1000, 		//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		-100,				//スライダーの最小値 
		100,			//スライダーの最大値
		0,				//デフォルトの値
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
	// ----------------------------------------------------------------
	
	cs.AddFloatSlider(	// R
		STR_NOISE_AMOUNT,			//Name
		0,				//VALID_MIN
		300,			//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE_AMOUNT
	);
	cs.AddFloatSlider(	// R
		STR_NOISE_ACCENT_AMOUNT,			//Name
		0,				//VALID_MIN
		5000,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE_ACCENT_AMOUNT
	);
	cs.AddCheckBox(	// noise frame
		STR_NOISE_ISCOlOR,
		"on",
		TRUE,
		ID_NOISE_ISCOLOR
	);

	cs.AddCheckBox(	// noise frame
		STR_NOISE_AUTO,
		"on",
		TRUE,
		ID_NOISE_AUTO,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddSlider(	// noise offset
		STR_NOISE_SEED,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-1000,				//スライダーの最小値 
		1000,			//スライダーの最大値
		10,				//デフォルトの値
		ID_NOISE_SEED,
		PF_ParamFlag_NONE,
		PF_PUI_DISABLED
	);
	// ----------------------------------------------------------------
	cs.AddPopup(STR_DRAW_POP,
		STR_DRAW_COUNT,
		STR_DRAW_DFLT,
		STR_DRAW_ITEMS,
		ID_DROW_POP
	);
	cs.AddPoint(
		STR_START_POS,
		60,
		60,
		FALSE,
		ID_START_POS
	);
	cs.AddPoint(
		STR_END_POS,
		80,
		80,
		FALSE,
		ID_END_POS
	);
	cs.AddColor(	// color
		STR_DRAW_COLOR,
		{ 0xFF, 0x00, 0xFF, 0xFF },
		ID_DRAW_COLOR
	);
	// ----------------------------------------------------------------
	cs.AddCheckBox(	// noise frame
		STR_DEBUG_FONT_CB,
		"on",
		FALSE,
		ID_DEBUG_FONT_CB
	);
	cs.AddPoint(
		STR_DEBUG_FONT_POS,
		0,
		0,
		FALSE,
		ID_DEBUG_FONT_POS
	);
	cs.AddColor(	// color
		STR_DEBUG_FONT_COLOR,
		{ 0xFF, 0xFF, 0x00, 0x00 },
		ID_DEBUG_FONT_COLOR
	);
	// ----------------------------------------------------------------
	cs.AddCheckBox(	// noise frame
		STR_HIDDEN_ON1,
		"on",
		TRUE,
		ID_NOISE_AUTO,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);

	cs.AddTopic(STR_TOPIC, ID_TOPIC, 
		PF_ParamFlag_START_COLLAPSED,
		PF_PUI_DISABLED);

	cs.EndTopic(ID_TOPIC_END);

	cs.AddButton(	// button
		STR_BUTTON1,
		STR_BUTTON2,
		ID_BUTTON,
		PF_ParamFlag_SUPERVISE
	);
	*/
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
	//					err2				= PF_Err_NONE;
	/*
	try{
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
			if (extraP->param_index == ID_BUTTON)
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
			}
		}
		*/
		/*
			ERR(ae.GetNewEffectStreamAll());
			if (!err){
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) hide_themB[i] =FALSE;
				PF_Boolean b;
				ERR(ae.GetCHECKBOX(ID_HIDDEN_ON,&b));
				if (b){
					for ( A_long i=ID_HIDDEN_ON+1; i<ID_NUM_PARAMS; i++)hide_themB[i] =TRUE;
				}
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) 
					ERR(ae.SetDynamicStreamFlag(i,AEGP_DynStreamFlag_HIDDEN,hide_themB[i]));
				
				
			}
		*/
	/*
		if((!err)&&(in_data->appl_id != 'PrMr')){
			//--------------------
			
			if (!err){
				PF_Boolean b=FALSE;
				ERR(ae.GetCHECKBOX(ID_HIDDEN_ON,&b));
				ERR(ae.UI_DISABLE(ID_TOPIC, !b));
				ERR(ae.UI_DISABLE(ID_ANGLE, !b));
			}
			//--------------------
			if (!err){
				PF_Boolean b = FALSE;
				ERR(ae.GetCHECKBOX(ID_NOISE_AUTO,&b));
				ERR(ae.UI_DISABLE(ID_NOISE_SEED, b));
			}
			//--------------------
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
		}
	}catch ( PF_Err & errP){
		err = errP;
	}
	*/
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
	/*
	PF_Err 	err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_NOISE_AUTO,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	*/
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_Fixed	fixed_angle;
	ERR(ae->GetANGLE(ID_DIRECTION, &fixed_angle));
	infoP->direction = F_FIX2FLT(fixed_angle);
	ERR(ae->GetFLOAT(ID_DISTANCE, &infoP->distance));
	/*
	ERR(ae->GetCHECKBOX(ID_PAINT_CB, &infoP->paint_cb));
	ERR(ae->GetPOINT(ID_PAINT_POS, &infoP->paint_pos));
	ERR(ae->GetCOLOR(ID_PAINT_COLOR, &infoP->paint_color));

	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));

	ERR(ae->GetFLOAT(ID_NOISE_SIZE,&infoP->noise_size));
	ERR(ae->GetFLOAT(ID_NOISE_AMOUNT,&infoP->noise_amount));
	ERR(ae->GetFLOAT(ID_NOISE_ACCENT_AMOUNT, &infoP->noise_accent_amount));
	ERR(ae->GetCHECKBOX(ID_NOISE_ISCOLOR, &infoP->noise_is_color));
	ERR(ae->GetCHECKBOX(ID_NOISE_AUTO, &infoP->noise_auto));
	ERR(ae->GetADD(ID_NOISE_SEED, &infoP->noise_seed));

	ERR(ae->GetPOPUP(ID_DROW_POP, &infoP->draw));
	ERR(ae->GetPOINT(ID_START_POS, &infoP->draw_start));
	ERR(ae->GetPOINT(ID_END_POS, &infoP->draw_end));
	ERR(ae->GetCOLOR(ID_DRAW_COLOR, &infoP->draw_color));

	ERR(ae->GetCHECKBOX(ID_DEBUG_FONT_CB, &infoP->debug_font_cb));
	ERR(ae->GetPOINT(ID_DEBUG_FONT_POS, &infoP->debug_font_pos));
	ERR(ae->GetCOLOR(ID_DEBUG_FONT_COLOR, &infoP->debug_font_color));
	*/
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	infoP->distance *= ae->downSaleValue();
	//画面をコピー
	//ERR(ae->CopyInToOut());
	ERR(EmbossFilter(ae, infoP));
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

