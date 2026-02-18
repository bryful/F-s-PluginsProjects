//-----------------------------------------------------------------------------------
/*
	NF-Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "TargetGrad.h"



#define DIALOG_PATH_FILE "TargetGrad.dir"
#define SETTINGS_FILE "TargetGrad.pref"
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
// **************************************************************************
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
	else {
		table.ok = false;
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
		else {
			table.ok = false;
		}
	}
	else {
		table.ok = false;
	}
	if (jsn.find("gradColor") != jsn.end()) {
		json ct3 = jsn["gradColor"];
		table.gradColor = JsonToPixel(ct3);
	}
	if (jsn.find("invert") != jsn.end()) {
		json ct = jsn["invert"];
		table.invert = ct.get<PF_Boolean>();
	}
	if (jsn.find("guideEnabled") != jsn.end()) {
		json ct = jsn["guideEnabled"];
		table.guideEnabled = ct.get<PF_Boolean>();
	}
	if (jsn.find("guideColor") != jsn.end()) {
		json ct = jsn["guideColor"];
		table.guideColor = JsonToPixel(ct);
	}
	return table;
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
	ERR(ae->GetCHECKBOX(ID_GUIDE_ENABLED, &table->guideEnabled));
	ERR(ae->GetCOLOR(ID_GUIDE_COLOR, &table->guideColor));

	return err;
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
	ret["guideEnabled"] = tableP->guideEnabled;
	ret["guideColor"] = PixelToJson(tableP->guideColor);
	return ret;
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
	ERR(ae->SetCHECKBOX(ID_GUIDE_ENABLED, table->guideEnabled));
	ERR(ae->SetCOLOR(ID_GUIDE_COLOR, table->guideColor));
	return err;
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
		table.guideEnabled = FALSE;
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
	def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_TOPIC_2POINT, ID_TOPIC_2POINT);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_START,
		75,	// X
		50,	// Y
		0,	// Flag
		ID_START_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_LAST,
		25,	// X
		50,	// Y
		0,	// Flag
		ID_LAST_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_2POINT_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_CHECKBOX(STR_AUTO_POS,
		STR_AUTO_POS2,
		FALSE,
		0,
		ID_AUTO_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ROT, 0, ID_ROT);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_START_PER,	//Name
		-300,						//VALID_MIN
		300,						//VALID_MAX
		-50,						//SLIDER_MIN
		50,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_START_PER
	);
	//----------------------------------------------------------------

	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LAST_PER,	//Name
		-300,						//VALID_MIN
		300,						//VALID_MAX
		-50,						//SLIDER_MIN
		50,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LAST_PER
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OFFSET_X,	//Name
		-2000,						//VALID_MIN
		2000,						//VALID_MAX
		-200,						//SLIDER_MIN
		200,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OFFSET_X
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OFFSET_Y,	//Name
		-2000,						//VALID_MIN
		2000,						//VALID_MAX
		-200,						//SLIDER_MIN
		200,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OFFSET_Y
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_CHECKBOX(STR_GUIDE_ENABLED,
		STR_GUIDE_ENABLED,
		table.guideEnabled,
		0,
		ID_GUIDE_ENABLED
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(
		STR_GUIDE_COLOR,
		table.guideColor.red,
		table.guideColor.green,
		table.guideColor.blue,
		ID_GUIDE_COLOR
	);
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
	PF_ADD_CHECKBOX(STR_BLEND,
		"on",
		FALSE,
		0,
		ID_BLEND
	);

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
		ppath = ChangeFileExtension(ppath, "tgj");
		std::string p =  SaveFileDialogWithCheck(
			std::string("tgj Json save"),
			ppath,
			{".tgj", "*.*"},
			std::string("tgj Json File (*.tgj)"));
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
			ppath = GetDocumentsFolderPath() + "\\ColorTable.tgj";
		}
		else {
			ppath = ChangeFileExtension(ppath, "tgj");
		}
		std::string p = OpenFileDialog(
			std::string("tgj Json load"),
			ppath,
			{".tgj", "*.*"},
			std::string("tgj Json File (*.tgj)"));

		if (p.empty() == FALSE)
		{
			saveDialogPath(p);
			std::ifstream ifs(p);
			if (ifs.good())
			{
				json m_json;
				ifs >> m_json;
				PixelTable table = JsonToPixelTable(m_json);
				if (table.ok == TRUE) {
					SetPixelTable(&ae, &table);
				}
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
//-------------------------------------------------------------------------------------------------
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
	PF_Err		err = PF_Err_NONE;

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
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));

	//
	PF_FixedPoint pp;
	ERR(ae->GetFIXEDPOINT(ID_START_POS, &pp));
	if (!err)
	{
		infoP->startPos.x = (PF_FpLong)pp.x / 65536.0;
		infoP->startPos.y = (PF_FpLong)pp.y / 65536.0;
	}
	ERR(ae->GetFIXEDPOINT(ID_LAST_POS, &pp));
	if (!err)
	{
		infoP->lastPos.x = (PF_FpLong)pp.x / 65536.0;
		infoP->lastPos.y = (PF_FpLong)pp.y / 65536.0;
	}
	if (infoP->invert == TRUE)
	{
		A_FloatPoint temp = infoP->startPos;
		infoP->startPos = infoP->lastPos;
		infoP->lastPos = temp;
	}
	//
	ERR(ae->GetCHECKBOX(ID_AUTO_POS, &infoP->autoPos));
	PF_Fixed rot;
	ERR(ae->GetANGLE(ID_ROT, &rot));
	infoP->rot = F_FIX2FLT(rot);

	ERR(ae->GetFLOAT(ID_START_PER, &infoP->startPercent));
	infoP->startPercent /= 100.0;
	ERR(ae->GetFLOAT(ID_LAST_PER, &infoP->lastPercent));
	infoP->lastPercent /= 100.0;

	ERR(ae->GetFLOAT(ID_OFFSET_X, &infoP->offsetX));
	ERR(ae->GetFLOAT(ID_OFFSET_Y, &infoP->offsetY));

	ERR(ae->GetCHECKBOX(ID_GUIDE_ENABLED, &infoP->guideEnabled));
	ERR(ae->GetCOLOR(ID_GUIDE_COLOR, &infoP->guideColor));
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));

	return err;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static PF_Err
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	//画面をコピー
	//ERR(ae->CopyInToOut());
	if ((infoP->targetColorMode == 1) && (infoP->targetColorCount <= 0)) {
		if (infoP->isBlend)
		{
			return ae->CopyInToOut();
		}
		else {
			PF_InData* in_data = ae->in_data;
			return PF_FILL(NULL, NULL, ae->output);
		}
	}
	if (infoP->targetColorMode != 3)
	{
		ERR(ExtractColor(ae, infoP));
	}

	RotPoint rp;
	if (infoP->autoPos)
	{
		rp.SetRect(infoP->area,
			infoP->rot,
			infoP->startPercent,
			infoP->lastPercent,
			infoP->offsetX,
			infoP->offsetY
		);
	}
	else {
		rp.Set2Point(
			infoP->startPos, infoP->lastPos,
			infoP->startPercent,
			infoP->lastPercent,
			infoP->offsetX,
			infoP->offsetY
		);
	}
	infoP->startPos2 = rp.startPos2();
	infoP->lastPos2 = rp.lastPos2();
	infoP->startPos2D = rp.startPos2D();
	infoP->lastPos2D = rp.lastPos2D();

	ERR(TargetGradMain(ae, infoP, ae->output));

	if (infoP->guideEnabled) {
		A_long sx = static_cast<A_long>(std::lround(infoP->startPos2.x));
		A_long sy = static_cast<A_long>(std::lround(infoP->startPos2.y));
		A_long lx = static_cast<A_long>(std::lround(infoP->lastPos2.x));
		A_long ly = static_cast<A_long>(std::lround(infoP->lastPos2.y));
		A_long cx = static_cast<A_long>(std::lround(rp.centerPos2().x));
		A_long cy = static_cast<A_long>(std::lround(rp.centerPos2().y));

		DrawWorld dw(ae->output, ae->pixelFormat());
		if (infoP->autoPos) {
			dw.Box(
				infoP->area.left,
				infoP->area.top,
				infoP->area.right,
				infoP->area.bottom,
				infoP->guideColor
			);
		}

		//ガイド線の描画

		DrawLine(
			ae->output,
			ae->pixelFormat(),
			sx,
			sy,
			lx,
			ly,
			infoP->guideColor
		);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			sx+1,
			sy,
			lx+1,
			ly,
			infoP->guideColor
		);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			sx,
			sy + 1,
			lx,
			ly + 1,
			infoP->guideColor
		);

		A_FloatPoint c2DG = rp.centerPos2G(20);
		A_long x2 = static_cast<A_long>(std::lround(c2DG.x));
		A_long y2 = static_cast<A_long>(std::lround(c2DG.y));
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			cx,cy,x2,y2,infoP->guideColor);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			cx+1, cy, x2+1, y2, infoP->guideColor);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			cx, cy + 1, x2, y2 + 1, infoP->guideColor);

		A_FloatPoint s2D = rp.startPos2D();
		A_FloatPoint s2DG = rp.startPos2DG(40);
		sx = static_cast<A_long>(std::lround(s2D.x));
		sy = static_cast<A_long>(std::lround(s2D.y));
		x2 = static_cast<A_long>(std::lround(s2DG.x));
		y2 = static_cast<A_long>(std::lround(s2DG.y));
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			sx, sy, x2, y2, infoP->guideColor);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			sx+1, sy, x2+1, y2, infoP->guideColor);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			sx, sy + 1, x2, y2 + 1, infoP->guideColor);

		A_FloatPoint l2D = rp.lastPos2D();
		A_FloatPoint l2DG = rp.lastPos2DG(40);
		lx = static_cast<A_long>(std::lround(l2D.x));
		ly = static_cast<A_long>(std::lround(l2D.y));
		x2 = static_cast<A_long>(std::lround(l2DG.x));
		y2 = static_cast<A_long>(std::lround(l2DG.y));
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			lx, ly, x2, y2, infoP->guideColor);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			lx+1, ly, x2+1, y2, infoP->guideColor);
		DrawLine(
			ae->output,
			ae->pixelFormat(),
			lx, ly + 1, x2, y2 + 1, infoP->guideColor);

	}
	if(infoP->isBlend){
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

