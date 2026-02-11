//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ColorSwitch.h"

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
	PF_Pixel 	def_color[] = {
		{ 0xFF, 0xFF, 0x00, 0x00 },
		{ 0xFF, 0x00, 0xFF, 0x00 },
		{ 0xFF, 0x00, 0x00, 0xFF },
		{ 0xFF, 0x00, 0xFF, 0xFF },
		{ 0xFF, 0xFF, 0x00, 0xFF },
		{ 0xFF, 0xFF, 0xFF, 0x00 },
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		{ 0xFF, 0x80, 0x80, 0x80 }
	}
	;
	// ----------------------------------------------------------------
	cs.AddCheckBox(	// noise frame
		STR_ENABLEALL,
		"on",
		FALSE,
		ID_ENABLEALL
	);
	// ----------------------------------------------------------------
	cs.AddSlider(	// 
		STR_ACTIVE_PARAM_COUNT,	//パラメータの名前
		1, 						//数値入力する場合の最小値
		COLOR_TABLE_MAX,		//数値入力する場合の最大値
		1,						//スライダーの最小値 
		COLOR_TABLE_MAX,		//スライダーの最大値
		4,				//デフォルトの値
		ID_ACTIVE_PARAM_COUNT,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	// ----------------------------------------------------------------
	cs.AddPopup(
		STR_MODE_POP,
		STR_MODE_COUNT,
		SwitchMode::SWITCH_MODE_REPLACE,
		STR_MODE_ITEMS,
		ID_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	// ----------------------------------------------------------------
	cs.AddTopic(
		STR_TOPIC, 
		ID_TOPIC_COLORS
	);

	// ************************************************
	std::string buf;
	for (A_long i = 0; i < COLOR_TABLE_MAX; i++)
	{
		PF_ParamUIFlags uif = PF_PUI_NONE;
		PF_ParamFlags pf = PF_ParamFlag_NONE;
		if (i >= 4)
		{
			uif = PF_PUI_INVISIBLE;
		}
		buf = std::string(STR_TURNON) + std::to_string(i);
		cs.AddCheckBox(	// check box
			buf.c_str(),
			"on",
			FALSE,
			ID_TURNON(i),
			pf,
			uif
		);
		buf = std::string(STR_OLD) + std::to_string(i);
		PF_Pixel pc = def_color[i % 8];
		cs.AddColor(	// old color
			buf.c_str(),
			pc,
			ID_OLD_COLOR(i),
			pf,
			uif
		);
		buf = std::string(STR_NEW) + std::to_string(i);
		pc.blue /= 2;
		pc.green /= 2;
		pc.red /= 2;
		cs.AddColor(	// new color
			buf.c_str(),
			pc,
			ID_NEW_COLOR(i),
			pf,
			uif
		);
	}
	// ************************************************
	cs.EndTopic(ID_TOPIC_END);
	// ************************************************
	cs.AddButton(	// button
		STR_ALL_TURNOFF1,
		STR_ALL_TURNOFF2,
		ID_ALL_TURNOFF,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddButton(	// button
		STR_SAVE_PARAMS1,
		STR_SAVE_PARAMS2,
		ID_SAVE_PARAMS,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddButton(	// button
		STR_LOAD_PARAMS1,
		STR_LOAD_PARAMS2,
		ID_LOAD_PARAMS,
		PF_ParamFlag_SUPERVISE
	);
	
	cs.Finalize();
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE* ae, ParamInfo* infoP)
{
	PF_Err		err = PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_ENABLEALL, &infoP->enable_All));
	ERR(ae->GetADD(ID_ACTIVE_PARAM_COUNT, &infoP->active_param_count));
	if (infoP->active_param_count < 1) {
		infoP->active_param_count = 1;
	}
	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));

	for (int i = 0; i < COLOR_TABLE_MAX; i++)
	{
		PF_Boolean turn_on = FALSE;
		ERR(ae->GetCHECKBOX(ID_TURNON(i), &infoP->turnon_colors[i]));
		ERR(ae->GetCOLOR(ID_OLD_COLOR(i), &infoP->old_colors[i]));
		ERR(ae->GetCOLOR(ID_NEW_COLOR(i), &infoP->new_colors[i]));
	}
	return err;
}
static PF_Err SetParams(NF_AE* ae, ParamInfo info)
{
	PF_Err err = PF_Err_NONE;

	ERR(ae->SetCHECKBOX(ID_ENABLEALL, info.enable_All));
	ERR(ae->SetADD(ID_ACTIVE_PARAM_COUNT, info.active_param_count));
	ERR(ae->SetPOPUP(ID_MODE, info.mode));
	for (int i = 0; i < COLOR_TABLE_MAX; i++)
	{
		ERR(ae->SetCHECKBOX(ID_TURNON(i), info.turnon_colors[i]));
		ERR(ae->SetCOLOR(ID_OLD_COLOR(i), info.old_colors[i]));
		ERR(ae->SetCOLOR(ID_NEW_COLOR(i), info.new_colors[i]));
	}
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
	if (!extraP) return err;
	try{
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
			if (extraP->param_index == ID_ALL_TURNOFF)
			{
				//すべてオフ
				for(int i=0;i<COLOR_TABLE_MAX;i++){
					ERR(ae.SetCHECKBOX(ID_TURNON(i),FALSE));
				}
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}else if (extraP->param_index == ID_SAVE_PARAMS){
				std::string ppath = loadTextFile(std::string(NF_NAME)+".pref");
				if (ppath.empty()) {
					ppath = GetDocumentsFolderPath()+"\\colortable.csj";
				}
				ppath = ChangeFileExtension(ppath, "csj");
				std::string p = SaveFileDialogWithCheck(
					std::string("csj Json save"), 
					ppath,
					{"*.csj"},//std::vector<std::string>
					std::string("csj Json File (*.csj)")
				);
				if (p.empty() == FALSE)
				{
					p = ChangeFileExtension(p, "csj");
					ParamInfo info;
					AEFX_CLR_STRUCT(info);
					GetParams(&ae, &info);
					json m_json = ParamInfoToJson(&info);
					std::ofstream writing_file;
					writing_file.open(p, std::ios::out);
					writing_file << m_json.dump(4) << std::endl;
					writing_file.close();
					saveTextFile(std::string(NF_NAME) + ".pref",p);
				}
				//out_data->out_flags |= PF_OutFlag_REFRESH_UI;
			}
			else if (extraP->param_index == ID_LOAD_PARAMS) {
				std::string ppath = loadTextFile(std::string(NF_NAME) + ".pref");
				if (ppath.empty()) {
					ppath = GetDocumentsFolderPath() + "\\colortable.csj";
				}
				else {
					ppath = ChangeFileExtension(ppath, "csj");
				}
				std::string p = OpenFileDialog(
					std::string("csj Json load"),
					ppath,
					{"*.csj"},//std::vector<std::string>
					std::string("csj Json File (*.csj)"));

				if (p.empty() == FALSE)
				{
					saveTextFile(std::string(NF_NAME) + ".pref", p);
					std::ifstream ifs(p);
					if (ifs.good())
					{
						json m_json;
						ifs >> m_json;
						ParamInfo info;
						if (!JsonToParamInfo(m_json, &info))
						{
							SetParams(&ae, info);
							out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
						}
					}
					else
					{
						err = PF_Err_BAD_CALLBACK_PARAM;
					}
				}
			}
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide1[COLOR_TABLE_MAX];
				A_Boolean hide2[COLOR_TABLE_MAX];
				for (A_long i = 0; i < COLOR_TABLE_MAX; i++) {
					hide1[i] = TRUE;
					hide2[i] = TRUE;
				}
				A_long cnt;
				A_long mode;
				err = ae.GetADD(ID_ACTIVE_PARAM_COUNT, &cnt);
				err = ae.GetPOPUP(ID_MODE, &mode);
				for (A_long i = 0; i < cnt; i++)
				{
					hide1[i] = FALSE;
					if (mode == 1) {
						hide2[i] = FALSE;
					}
				}
				for (A_long i = 0; i < COLOR_TABLE_MAX; i++)
				{
					ERR(ae.SetDynamicStreamFlag(ID_TURNON(i), AEGP_DynStreamFlag_HIDDEN, hide1[i]));
					ERR(ae.SetDynamicStreamFlag(ID_OLD_COLOR(i), AEGP_DynStreamFlag_HIDDEN, hide1[i]));
					ERR(ae.SetDynamicStreamFlag(ID_NEW_COLOR(i), AEGP_DynStreamFlag_HIDDEN, hide2[i]));
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
	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
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

