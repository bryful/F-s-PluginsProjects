//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "TargetGradRadical.h"

#define DIALOG_PATH_FILE "TargetGradRadical.dir"
#define SETTINGS_FILE "TargetGradRadical.pref"

static json PixelToJson(PF_Pixel8 p)
{
	json ret =
	{
		{"r",p.red},
		{"g",p.green},
		{"b",p.blue}
	};
	return ret;
}
static PF_Pixel JsonToPixel(json jsn)
{
	PF_Pixel ret = { 0,0,0,0 };
	ret.red = ret.green = ret.blue = 0;
	ret.alpha = PF_MAX_CHAN8;
	PF_Pixel8 p = { 0,0,0,0 };
	if (jsn.find("r") != jsn.end()) {
		ret.red = jsn["r"].get<uint8_t>();
	}
	if (jsn.find("g") != jsn.end()) {
		ret.green = jsn["g"].get<uint8_t>();
	}
	if (jsn.find("b") != jsn.end()) {
		ret.blue = jsn["b"].get<uint8_t>();
	}
	return ret;
}
static json PixelTableToJson(PixelTable* tableP)
{
	json ret;
	json jcol = json::array();
	json jcole = json::array();

	ret["targetColorMode"] = tableP->targetColorMode;
	for (int i = 0; i < COLOR_TABLE_COUNT; i++)
	{
		jcol.push_back(PixelToJson(tableP->target[i]));
		jcole.push_back(tableP->targetEnabled[i]);
	}
	ret["colors"] = jcol;
	ret["enables"] = jcole;
	ret["gradColor"] = PixelToJson(tableP->gradColor);
	ret["invert"] = tableP->invert;
	return ret;
}
static PixelTable JsonToPixelTable(json jsn)
{
	PixelTable table;
	//bool ok = true;
	AEFX_CLR_STRUCT(table);
	table.ok = true;

	if (jsn.find("targetColorMode") != jsn.end()) {
		json ct = jsn["targetColorMode"];
		table.targetColorMode = ct.get<long>();
	}
	if (jsn.find("colors") != jsn.end()) {
		json ct = jsn["colors"];
		if (ct.is_array())
		{
			for (int i = 0; i < ct.size(); i++)
			{
				json it = ct[i];
				table.target[i] = JsonToPixel(it);
			}

		}
		else {
			table.ok = false;
		}
	}
	else {
		table.ok = false;
	}
	if (jsn.find("enables") != jsn.end()) {
		json ct2 = jsn["enables"];
		if (ct2.is_array())
		{
			for (int i = 0; i < ct2.size(); i++)
			{
				table.targetEnabled[i] = ct2[i].get<PF_Boolean>();
			}
		}
	}
	if (jsn.find("gradColor") != jsn.end()) {
		json ct3 = jsn["gradColor"];
		table.gradColor = JsonToPixel(ct3);
	}
	if (jsn.find("invert") != jsn.end()) {
		json ct = jsn["invert"];
		table.invert = ct.get<PF_Boolean>();
	}
	return table;
}
// *******************************************************************
// //-------------------------------------------------------------------------------------------------
static PF_Err GetPixelTable(NF_AE* ae, PixelTable* table)
{
	PF_Err		err = PF_Err_NONE;

	ERR(ae->GetPOPUP(ID_TARGET, &table->targetColorMode));
	int count = 0;
	for (int i = 0; i < COLOR_TABLE_COUNT; i++) {
		ERR(ae->GetCHECKBOX(ID_COLOR_ENABLED(i), &table->targetEnabled[i]));
		ERR(ae->GetCOLOR(ID_COLOR(i), &table->target[i]));
	}
	ERR(ae->GetCOLOR(ID_GRAD_COLOR, &table->gradColor));
	ERR(ae->GetCHECKBOX(ID_INVERT, &table->invert));

	return err;
}
static PF_Err SetPixelTable(NF_AE* ae, PixelTable* table)
{
	PF_Err err = PF_Err_NONE;

	ERR(ae->SetPOPUP(ID_TARGET, table->targetColorMode));
	for (int i = 0; i < COLOR_TABLE_COUNT; i++)
	{
		ERR(ae->SetCHECKBOX(ID_COLOR_ENABLED(i), table->targetEnabled[i]));
		ERR(ae->SetCOLOR(ID_COLOR(i), table->target[i]));
	}
	ERR(ae->SetCOLOR(ID_GRAD_COLOR, table->gradColor));
	ERR(ae->SetCHECKBOX(ID_INVERT, table->invert));
	return err;
}
static std::string loadDialogPath()
{
	std::string ret;
	std::string p = GetPluginConfigFilePath(DIALOG_PATH_FILE);
	if (!p.empty())
	{
		std::ifstream f(p.c_str(), std::ios::binary);
		if (f.is_open() && f.good())
		{
			std::ifstream ifs(p);
			if (ifs.good())
			{
				ifs >> ret;
			}
		}
	}
	return ret;
}
static void saveDialogPath(std::string path)
{
	std::string ret;
	std::string p = GetPluginConfigFilePath(DIALOG_PATH_FILE);
	if (!p.empty())
	{
		std::ifstream f(p.c_str(), std::ios::binary);
		if (p.empty() == FALSE)
		{
			std::ofstream writing_file;
			writing_file.open(p, std::ios::out);
			writing_file << path << std::endl;
			writing_file.close();
		}
	}
}
static PixelTable loadPref()
{
	PixelTable table;
	AEFX_CLR_STRUCT(table);
	table.ok = false;
	std::string p = GetPluginConfigFilePath(SETTINGS_FILE);
	if (!p.empty())
	{
		std::ifstream f(p.c_str(), std::ios::binary);
		if (f.is_open() && f.good())
		{
			std::ifstream ifs(p);
			if (ifs.good())
			{
				json m_json;
				ifs >> m_json;
				table = JsonToPixelTable(m_json);
			}
		}
	}
	return table;
}
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
	PF_ParamDef		def;

	PixelTable table = loadPref();
	A_u_char defP[8][3] = {
		{0xFF,0x00,0x00},
		{0x00,0xFF,0x00},
		{0x00,0x00,0xFF},
		{0xFF,0xFF,0x00},
		{0xFF,0x00,0xFF},
		{0x00,0xFF,0xFF},
		{0xFF,0xFF,0xFF},
		{0x00,0x00,0x00}
	};
	if (!table.ok) {
		table.targetColorMode = 3;
		for (int i = 0; i < COLOR_TABLE_COUNT; i++) {
			table.target[i].red = defP[i][0];
			table.target[i].green = defP[i][1];
			table.target[i].blue = defP[i][2];
		}
		table.gradColor = { 0xFF,0xFF,0x00,0xFF };
		table.guideEnabled = TRUE;
		table.guideColor = { 0xFF,0xFF,0xFF,0xFF };
		table.invert = FALSE;
	}



	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_TARGET,
		STR_TARGET_COUNT,	//メニューの数
		table.targetColorMode,	//デフォルト
		STR_TARGET_ITEMS,
		ID_TARGET
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_TOPIC_COLOR, ID_TOPIC_COLOR);
	//----------------------------------------------------------------

	std::string str;
	for (A_long i = 0; i < COLOR_TABLE_COUNT; i++) {
		AEFX_CLR_STRUCT(def);
		//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		str = STR_TARGET_CB + std::to_string(i + 1);
		PF_ADD_CHECKBOX(str.c_str(),
			STR_TARGET_CB2,
			(PF_Boolean)table.targetEnabled[i],
			0,
			ID_COLOR_ENABLED(i)
		);
		AEFX_CLR_STRUCT(def);
		//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		str = STR_TARGET + std::to_string(i + 1);
		PF_ADD_COLOR(
			str.c_str(),
			table.target[i].red,
			table.target[i].green,
			table.target[i].blue,
			ID_COLOR(i)
		);
	}
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_TCOLOR_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(
		STR_GRADCOLOR,
		table.gradColor.red,
		table.gradColor.green,
		table.gradColor.blue,
		ID_GRAD_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_CHECKBOX(STR_INVERT,
		STR_INVERT2,
		table.invert,
		0,
		ID_INVERT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_CENTER,
		50,	// X
		50,	// Y
		0,	// Flag
		ID_CENTER_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_BLUR,	//Name
		0,						//VALID_MIN
		4000,						//VALID_MAX
		0,						//SLIDER_MIN
		1000,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		500,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_BLUR
	);
	//----------------------------------------------------------------

	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FEATHER,	//Name
		0.02f,						//VALID_MIN
		100,					//VALID_MAX
		0.02f,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FEATHER
	);
	//----------------------------------------------------------------

	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HYPERBOLIC,	//Name
		0,						//VALID_MIN
		50,					//VALID_MAX
		0,					//SLIDER_MIN
		20,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		5,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_HYPERBOLIC
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_ASPECT,	//Name
		0,						//VALID_MIN
		2000,						//VALID_MAX
		0,						//SLIDER_MIN
		200,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_ASPECT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ANGLE, 0, ID_ANGLE);


	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON(STR_LAOD_CAP,
		STR_LOAD_BTN,
		0,
		PF_ParamFlag_SUPERVISE,
		ID_LOAD_BTN);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON("",
		STR_SAVE_BTN,
		0,
		PF_ParamFlag_SUPERVISE,
		ID_SAVE_BTN);

	//----------------------------------------------------------------
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

	NF_AE ae;
	err = ae.HandleChangedParam(in_data, out_data, params, outputP, extraP, ID_NUM_PARAMS);
	if (extraP->param_index == ID_SAVE_BTN)
	{
		std::string ppath = loadDialogPath();
		if (ppath.empty()) {
			ppath = GetDocumentsFolderPath() + "\\ColorTable.tsj";
		}
		ppath = ChangeFileExtension(ppath, "tsj");
		std::string p = SaveFileDialogWithCheck(
			std::string("tgr Json save"),
			ppath,
			std::vector<std::string>{".tsj", "*.*"},
			std::string("tsj Json File"));
		if (p.empty() == FALSE)
		{
			saveDialogPath(p);
			PixelTable table;
			GetPixelTable(&ae, &table);
			json m_json = PixelTableToJson(&table);


			std::ofstream writing_file;
			writing_file.open(p, std::ios::out);
			writing_file << m_json.dump(4) << std::endl;
			writing_file.close();
		}
	}
	else if (extraP->param_index == ID_LOAD_BTN)
	{
		std::string ppath = loadDialogPath();
		if (ppath.empty()) {
			ppath = GetDocumentsFolderPath() + "\\ColorTable.tsj";
		}
		else {
			ppath = ChangeFileExtension(ppath, "tsj");
		}
		std::string p = OpenFileDialog(
			std::string("tsj Json load"),
			ppath,
			std::vector<std::string>{".tsj", "*.*"},
			std::string("tsj Json File"));
		if (p.empty() == FALSE)
		{
			std::ifstream ifs(p);
			if (ifs.good())
			{
				saveDialogPath(p);
				json m_json;
				ifs >> m_json;
				PixelTable table = JsonToPixelTable(m_json);
				SetPixelTable(&ae, &table);
			}
			else
			{
				err = PF_Err_BAD_CALLBACK_PARAM;
			}
		}

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
static A_long FindColor(ParamInfo* infoP, PF_Pixel c, int mx)
{
	A_long ret = -1;
	if (mx <= 0) return ret;
	if (mx > COLOR_TABLE_COUNT) mx = COLOR_TABLE_COUNT;

	for (int i = 0; i < mx; i++) {
		if (
			infoP->targetColors[i].red == c.red &&
			infoP->targetColors[i].green == c.green &&
			infoP->targetColors[i].blue == c.blue) {
			ret = i;
			break;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	//
	ERR(ae->GetPOPUP(ID_TARGET, &infoP->targetColorMode));
	//
	int count = 0;
	for (int i = 0; i < COLOR_TABLE_COUNT; i++) {
		PF_Boolean enabled = FALSE;
		ERR(ae->GetCHECKBOX(ID_COLOR_ENABLED(i), &enabled));
		if (enabled == TRUE) {
			PF_Pixel c = { 0,0,0,0 };
			ERR(ae->GetCOLOR(ID_COLOR(i), &c));
			A_long idx = FindColor(infoP, c, count);
			if (idx == -1) {
				infoP->targetColors[count] = c;
				count++;
			}
		}
	}
	infoP->targetColorCount = count;
	//
	ERR(ae->GetCOLOR(ID_GRAD_COLOR, &infoP->gradColor));
	ERR(ae->GetCHECKBOX(ID_INVERT, &infoP->invert));

	//
	PF_FixedPoint pp;
	ERR(ae->GetFIXEDPOINT(ID_CENTER_POS, &pp));
	if (!err)
	{
		infoP->cenertPos.x = (PF_FpLong)pp.x / 65536.0;
		infoP->cenertPos.y = (PF_FpLong)pp.y / 65536.0;
	}
	//
	ERR(ae->GetFLOAT(ID_BLUR, &infoP->blur));

	ERR(ae->GetFLOAT(ID_FEATHER, &infoP->feather));
	infoP->feather /= 100;
	if (infoP->feather < 0.02) infoP->feather = 0.02;
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));
	ERR(ae->GetFLOAT(ID_ASPECT, &infoP->aspect));
	infoP->aspect /= 100;
	if (infoP->aspect <= 0) infoP->aspect = 0.01;


	PF_Fixed rot;
	ERR(ae->GetANGLE(ID_ANGLE, &rot));
	infoP->angle = F_FIX2FLT(rot);

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	//ERR(ae->CopyInToOut());
	if (infoP->blur <= 0) {
		return err;
	}
	if ((infoP->targetColorMode == 1) && (infoP->targetColorCount <= 0)) {
		return err;
	}
	if (infoP->targetColorMode != 3)
	{
		ERR(ExtractColor(ae, infoP));
	}
	ERR(RenderTargetGradRadial(ae, infoP, ae->output));

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

