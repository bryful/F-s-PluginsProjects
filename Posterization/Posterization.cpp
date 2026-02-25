//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Posterization.h"

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
	PF_Pixel 	def_colors[] = {
		{ 0xFF, 0x00, 0x00, 0x00 },
		{ 0xFF, 0x2F, 0x40, 0x00 },
		{ 0xFF, 0x3F, 0x00, 0x40 },
		{ 0xFF, 0x4F, 0x40, 0x40 },
		{ 0xFF, 0x5F, 0x00, 0x60 },
		{ 0xFF, 0x6F, 0x60, 0x00 },
		{ 0xFF, 0x7F, 0x60, 0x60 },
		{ 0xFF, 0x8F, 0x80, 0x00 },
		{ 0xFF, 0x9F, 0x00, 0x80 },
		{ 0xFF, 0xAF, 0x80, 0x80 },
		{ 0xFF, 0xBF, 0xA0, 0x00 },
		{ 0xFF, 0xCF, 0x00, 0xA0 },
		{ 0xFF, 0xDF, 0xA0, 0xA0 },
		{ 0xFF, 0xEF, 0xC0, 0x00 },
		{ 0xFF, 0xFF, 0x00, 0xC0 }
	};
	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddPopup(STR_LEVEL,
		STR_LEVEL_COUNT,
		STR_LEVEL_DFLT,
		STR_LEVEL_ITEMS,
		ID_LEVEL,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddPopup(STR_MODE,
		STR_MODE_COUNT,
		STR_MODE_DFLT,
		STR_MODE_ITEMS,
		ID_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddCheckBox(	// noise frame
		STR_IS_COLOR,
		"on",
		FALSE,
		ID_IS_COLOR,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	 // ******************************************************
	cs.AddTopic(STR_COLOR_TOPIC, ID_COLOR_TOPIC,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE);
	for (int i = 0; i < COLOR_TABLE_MAX; i++) {
		std::string name = std::string(STR_COLOR) + std::to_string(i+1);
		if (i == 0) { name += " : start"; }
		else if (i == 7 - 1) { name += " : end"; }
		else if (i == 3)
		{
			{ name += " : mid"; }
		}
		PF_ParamUIFlags ui_flags = PF_PUI_INVISIBLE;
		if (i >=7) { // 最初の2色は常に表示、残りはオプションで表示
			ui_flags = PF_PUI_INVISIBLE;
		}
		cs.AddColor(	// color
			name.c_str(),
			def_colors[i],
			ID_COLOR_1+i,
			PF_ParamFlag_NONE,
			ui_flags
		);
	}
	cs.AddButton(	// button
		STR_BTN_TWEEN2,
		STR_BTN_TWEEN2B,
		ID_BTN_TWEEN2,
		PF_ParamFlag_SUPERVISE,
		PF_PUI_INVISIBLE
	);
	cs.AddButton(	// button
		STR_BTN_TWEEN3,
		STR_BTN_TWEEN3B,
		ID_BTN_TWEEN3,
		PF_ParamFlag_SUPERVISE,
		PF_PUI_INVISIBLE
	);
	cs.EndTopic(ID_COLOR_TOPIC_END);
	// ******************************************************
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
	cs.AddTopic(
		STR_BORDER_TOPIC, 
		ID_BORDER_TOPIC,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);

	cs.AddSlider(	// 
		STR_BORDER_INNER,	//パラメータの名前
		0, 			//数値入力する場合の最小値
		12,			//数値入力する場合の最大値
		0,			//スライダーの最小値 
		4,			//スライダーの最大値
		1,			//デフォルトの値
		ID_BORDER_INNER,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddSlider(	// 
		STR_BORDER_OUTER,	//パラメータの名前
		0, 			//数値入力する場合の最小値
		12,			//数値入力する場合の最大値
		0,			//スライダーの最小値 
		4,			//スライダーの最大値
		1,			//デフォルトの値
		ID_BORDER_OUTER,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddColor(	// color
		STR_BORDER_COLOR,
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_BORDER_COLOR,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.EndTopic(ID_BORDER_TOPIC_END);
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
			A_long level = 0;
			ERR(ae.GetPOPUP(ID_LEVEL, &level));
			level = LEVEL_TBL[level];
			if (extraP->param_index == ID_BTN_TWEEN2)
			{
				A_long idx0 = 0;
				A_long idx1 = level-1;
				PF_Pixel c0 = { 0,0,0 ,0 };
				PF_Pixel c1 = { 0,0,0 ,0 };
				ERR(ae.GetCOLOR(ID_COLOR_1 + idx0, &c0));
				ERR(ae.GetCOLOR(ID_COLOR_1 + idx1, &c1));
				for (int i = idx0+1; i < idx1; i++)
				{
					float t = (float)i / (idx1 - idx0);
					PF_Pixel ct;
					ct.alpha = 255;
					ct.red = (A_u_char)AE_CLAMP(((1 - t) * c0.red + t * c1.red),0,255);
					ct.green = (A_u_char)AE_CLAMP(((1 - t) * c0.green + t * c1.green), 0, 255);
					ct.blue = (A_u_char)AE_CLAMP(((1 - t) * c0.blue + t * c1.blue), 0, 255);
					ERR(ae.SetCOLOR(ID_COLOR_1 + i, ct));
				}
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
			else if (extraP->param_index == ID_BTN_TWEEN3)
			{
				A_long idx0 = 0;
				A_long idx1 = level/2;
				A_long idx2 = level - 1;
				PF_Pixel c0 = { 0,0,0 ,0 };
				PF_Pixel c1 = { 0,0,0 ,0 };
				PF_Pixel c2 = { 0,0,0 ,0 };
				ERR(ae.GetCOLOR(ID_COLOR_1 + idx0, &c0));
				ERR(ae.GetCOLOR(ID_COLOR_1 + idx1, &c1));
				ERR(ae.GetCOLOR(ID_COLOR_1 + idx2, &c2));
				for (int i = idx0 + 1; i < idx1; i++)
				{
					float t = (float)i / (idx1 - idx0);
					PF_Pixel ct;
					ct.alpha = 255;
					ct.red = (A_u_char)AE_CLAMP(((1 - t) * c0.red + t * c1.red), 0, 255);
					ct.green = (A_u_char)AE_CLAMP(((1 - t) * c0.green + t * c1.green), 0, 255);
					ct.blue = (A_u_char)AE_CLAMP(((1 - t) * c0.blue + t * c1.blue), 0, 255);
					ERR(ae.SetCOLOR(ID_COLOR_1 + i, ct));
				}
				for (int i = idx1 + 1; i < idx2; i++)
				{
					float t = (float)(i-idx1) / (idx2 - idx1);
					PF_Pixel ct;
					ct.alpha = 255;
					ct.red = (A_u_char)AE_CLAMP(((1 - t) * c1.red + t * c2.red), 0, 255);
					ct.green = (A_u_char)AE_CLAMP(((1 - t) * c1.green + t * c2.green), 0, 255);
					ct.blue = (A_u_char)AE_CLAMP(((1 - t) * c1.blue + t * c2.blue), 0, 255);
					ERR(ae.SetCOLOR(ID_COLOR_1 + i, ct));
				}
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				A_long mode = 0;
				PF_Boolean is_color = FALSE;
				ERR(ae.GetPOPUP(ID_MODE, &mode));
				ERR(ae.GetCHECKBOX(ID_MODE, &is_color));
				for (A_long i = ID_COLOR_TOPIC; i <= ID_BTN_TWEEN3; i++)hide_themB[i] = TRUE;
				if (is_color) {
					hide_themB[ID_COLOR_TOPIC] = FALSE;
					hide_themB[ID_BTN_TWEEN2] = FALSE;
					hide_themB[ID_BTN_TWEEN3] = FALSE;
					if (mode == 1 || mode == 2) {
						for (A_long i = 0; i < level; i++) hide_themB[ID_COLOR_1 + i] = FALSE;
					}
				}
				if (mode == 2 || mode == 3) {
					for (A_long i = ID_BORDER_TOPIC; i <= ID_BORDER_COLOR; i++)hide_themB[i] = FALSE;
				}
				else {
					for (A_long i = ID_BORDER_TOPIC; i <= ID_BORDER_COLOR; i++)hide_themB[i] = TRUE;
				}
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));

				if (mode == 1 || mode == 2) {
					for (int i = 0; i < level; i++)
					{
						std::string name = std::string(STR_COLOR) + std::to_string(i + 1);
						if (i == 0) { name += " : start"; }
						else if (i == level - 1) { name += " : end"; }
						else if( i== (level/2))
						{ 
							{ name += " : mid"; }
						}
						ae.SetUIName(i + ID_COLOR_1, (char*)name.c_str());
					}
					out_data->out_flags |= PF_OutFlag_SEND_UPDATE_PARAMS_UI;
				}


				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
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

	A_long d = 0;
	ERR(ae->GetPOPUP(ID_LEVEL, &d));
	ERR(ae->GetCHECKBOX(ID_IS_COLOR, &infoP->is_color));
	infoP->level = LEVEL_TBL[d];
	d = 0;
	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));
	for(int i=0;i<COLOR_TABLE_MAX;i++) {
		ERR(ae->GetCOLOR(ID_COLOR_1+i, &infoP->colors[i]));
	}

	ERR(ae->GetCHECKBOX(ID_BILATERAL_CB, &infoP->use_bilateral));
	ERR(ae->GetADD(ID_B_RADIUS, &infoP->radius));
	double dd = 0;
	ERR(ae->GetFLOAT(ID_B_SIGMA_S, &dd));
	infoP->sigma_s = (float)dd;
	dd = 0;
	ERR(ae->GetFLOAT(ID_B_SIGMA_R, &dd));
	infoP->sigma_r = (float)(dd / 100);
	if (infoP->sigma_r < 0) { infoP->sigma_r = 0; }
	else if (infoP->sigma_r > 1) { infoP->sigma_r = 1; }

	ERR(ae->GetADD(ID_BORDER_INNER, &infoP->border_inner));
	ERR(ae->GetADD(ID_BORDER_OUTER, &infoP->border_outer));
	ERR(ae->GetCOLOR(ID_BORDER_COLOR, &infoP->border_color));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	//ERR(ae->CopyInToOut());
	if(infoP->use_bilateral && infoP->radius>0) {
		ERR(Bilateral
		(
			ae->in_data,
			ae->input,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->radius,
			infoP->sigma_s,
			infoP->sigma_r
		));
	}
	else {
		ae->CopyInToOut();
	}


	ERR(PosterizationExec(
		ae->in_data,
		//ae->input,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		infoP->level
	));
	PF_Boolean is_border = ((infoP->border_inner > 0 || infoP->border_outer > 0) && (infoP->mode == 2 || infoP->mode == 3));

	if(is_border) {
		if (infoP->border_inner > 0) {
			ERR(ChannelMinMax(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				-infoP->border_inner,
				1
			));
		}
		if (infoP->border_outer > 0) {
			ERR(ChannelMinMax(
				ae->in_data,
				ae->output,
				ae->pixelFormat(),
				ae->suitesP,
				infoP->border_outer,
				2
			));
		}
	}

	A_long color_coount = 0;
	if(infoP->is_color) {
		color_coount = infoP->level;
	}
	ERR(PostExec(
		ae->in_data,
		//PF_EffectWorld* inputP,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		infoP->level,
		color_coount,
		infoP->colors,
		infoP->mode,
		infoP->border_color
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

