//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "TargetGrad.h"
#include <string>

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
	ret["guideEnabled"] = tableP->guideEnabled;
	ret["guideColor"] = PixelToJson(tableP->guideColor);
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
//-------------------------------------------------------------------------------------------------
static PF_Err GetPT(CFsAE* ae, PixelTable* table)
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
static PF_Err PTSet(CFsAE* ae, PixelTable* table)
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
static std::string directoryPath = "";

static PixelTable loadPref()
{
	PixelTable table;
	AEFX_CLR_STRUCT(table);
	table.ok = false;
	std::string p = GetPluginConfigFilePath("TargetGrad.pref");
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
static std::string loadPath()
{
	std::string ret;
	std::string p = GetPluginConfigFilePath("TargetGrad.dir");
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
static void savePath(std::string path)
{
	std::string ret;
	std::string p = GetPluginConfigFilePath("TargetGrad.dir");
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

	PixelTable table =loadPref();
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
		table.targetColorMode = 1;
		for(int i=0;i<COLOR_TABLE_COUNT;i++){
			table.target[i].red = defP[i][0];
			table.target[i].green = defP[i][1];
			table.target[i].blue = defP[i][2];
		}
		table.gradColor = { 0xFF,0xFF,0xFF,0xFF };
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
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-------------------------------------------------------------------------------------------------

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
	
	CFsAE ae;
	err = ae.HandleChangedParam(in_data, out_data, params, outputP, extraP, ID_NUM_PARAMS);
	if (extraP->param_index == ID_SAVE_BTN)
	{
		std::string ppath = loadPath();
		std::string p = SaveJsonFileDialog(std::string("Json Save"), ppath);
		if (p.empty() == FALSE)
		{
			savePath(p);
			PixelTable table;
			GetPT(&ae,&table);
			json m_json = PixelTableToJson(&table);


			std::ofstream writing_file;
			writing_file.open(p, std::ios::out);
			writing_file << m_json.dump(4) << std::endl;
			writing_file.close();
		}
	}
	else if (extraP->param_index == ID_LOAD_BTN)
	{
		std::string ppath = loadPath();
		std::string p = OpenJsonFileDialog(std::string("Json Save"), ppath);
		if (p.empty() == FALSE)
		{
			savePath(p);
			std::ifstream ifs(p);
			if (ifs.good())
			{
				directoryPath = p;
				json m_json;
				ifs >> m_json;
				PixelTable table = JsonToPixelTable(m_json);
				PTSet(&ae,&table);
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
static A_long FindColor(ParamInfo *infoP,PF_Pixel c,int mx)
{
	A_long ret = -1;
	if (mx <= 0) return ret;
	if (mx > COLOR_TABLE_COUNT) mx = COLOR_TABLE_COUNT;

	for(int i= 0; i < mx; i++){
		if(
			infoP->targetColors[i].red   == c.red   &&
			infoP->targetColors[i].green == c.green &&
			infoP->targetColors[i].blue  == c.blue ){
				ret = i;
				break;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	//
	ERR(ae->GetPOPUP(ID_TARGET, &infoP->targetColorMode));
	//
	int count = 0;
	for (int i = 0; i < COLOR_TABLE_COUNT; i++) {
		PF_Boolean enabled = FALSE;
		ERR(ae->GetCHECKBOX(ID_COLOR_ENABLED(i), &enabled));
		if (enabled==TRUE) {
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
	ERR(ae->GetFIXEDPOINT(ID_START_POS, &pp));
	if (!err)
	{
		infoP->startPos.x = (PF_FpLong)pp.x/ 65536.0;
		infoP->startPos.y = (PF_FpLong)pp.y/ 65536.0;
	}
	ERR(ae->GetFIXEDPOINT(ID_LAST_POS, &pp));
	if (!err)
	{
		infoP->lastPos.x = (PF_FpLong)pp.x / 65536.0;
		infoP->lastPos.y = (PF_FpLong)pp.y / 65536.0;
	}
	//
	ERR(ae->GetCHECKBOX(ID_AUTO_POS, &infoP->autoPos));
	PF_Fixed rot;
	ERR(ae->GetANGLE(ID_ROT, &rot));
	infoP->rot = FIX2FLT(rot);

	ERR(ae->GetFLOAT(ID_START_PER, &infoP->startPercent));
	infoP->startPercent /= 100.0;
	ERR(ae->GetFLOAT(ID_LAST_PER, &infoP->lastPercent));
	infoP->lastPercent /= 100.0;

	ERR(ae->GetFLOAT(ID_OFFSET_X, &infoP->offsetX));
	ERR(ae->GetFLOAT(ID_OFFSET_Y, &infoP->offsetY));

	ERR(ae->GetCHECKBOX(ID_GUIDE_ENABLED, &infoP->guideEnabled));
	ERR(ae->GetCOLOR(ID_GUIDE_COLOR, &infoP->guideColor));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Exec(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err	err = PF_Err_NONE;
	//画面をコピー
	//ERR(ae->CopyInToOut());

	ERR(ExtractColor(ae, infoP));

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

	ERR(RenderSimpleMask(ae, infoP,ae->output));

	if (infoP->guideEnabled) {
		A_long sx = static_cast<A_long>(std::lround(infoP->startPos2.x));
		A_long sy = static_cast<A_long>(std::lround(infoP->startPos2.y));
		A_long lx = static_cast<A_long>(std::lround(infoP->lastPos2.x));
		A_long ly = static_cast<A_long>(std::lround(infoP->lastPos2.y));
		A_long cx = static_cast<A_long>(std::lround(rp.centerPos2().x));
		A_long cy = static_cast<A_long>(std::lround(rp.centerPos2().y));

		if (infoP->autoPos) {
			ae->out->box(infoP->area.left,
				infoP->area.top,
				infoP->area.right,
				infoP->area.bottom,
				infoP->guideColor,
				infoP->guideColor
			);
		}

		//ガイド線の描画
		ae->out->Line(sx, sy,lx, ly, infoP->guideColor);
		ae->out->Line(sx+1, sy, lx+1, ly, infoP->guideColor);
		ae->out->Line(sx, sy+1, lx, ly+1, infoP->guideColor);

		A_FloatPoint c2DG = rp.centerPos2G(20);
		A_long x2 = static_cast<A_long>(std::lround(c2DG.x));
		A_long y2 = static_cast<A_long>(std::lround(c2DG.y));
		ae->out->Line(cx, cy, x2, y2, infoP->guideColor);
		ae->out->Line(cx+1, cy, x2+1, y2, infoP->guideColor);
		ae->out->Line(cx, cy+1, x2, y2+1, infoP->guideColor);

		A_FloatPoint s2D = rp.startPos2D();
		A_FloatPoint s2DG = rp.startPos2DG(40);
		sx = static_cast<A_long>(std::lround(s2D.x));
		sy = static_cast<A_long>(std::lround(s2D.y));
		x2 = static_cast<A_long>(std::lround(s2DG.x));
		y2 = static_cast<A_long>(std::lround(s2DG.y));
		ae->out->Line(sx, sy, x2, y2, infoP->guideColor);
		ae->out->Line(sx+1, sy, x2+1, y2, infoP->guideColor);
		ae->out->Line(sx, sy+1, x2, y2+1, infoP->guideColor);

		A_FloatPoint l2D = rp.lastPos2D();
		A_FloatPoint l2DG = rp.lastPos2DG(40);
		lx = static_cast<A_long>(std::lround(l2D.x));
		ly = static_cast<A_long>(std::lround(l2D.y));
		x2 = static_cast<A_long>(std::lround(l2DG.x));
		y2 = static_cast<A_long>(std::lround(l2DG.y));
		ae->out->Line(lx, ly, x2, y2, infoP->guideColor);
		ae->out->Line(lx+1, ly, x2+1, y2, infoP->guideColor);
		ae->out->Line(lx, ly+1, x2, y2+1, infoP->guideColor);


	}

	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
/*
	SmartFXに対応していないホスト(After Effects7以前のもの)はこの関数が呼び出されて描画する
	この関数を書いておけば一応v6.5対応になる
*/
static PF_Err 
Render ( 
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ParamInfo info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;
}
//-----------------------------------------------------------------------------------
/*
	SmartFX対応の場合、まずこの関数が呼ばれてパラメータの獲得を行う
*/
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extraP,sizeof(ParamInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ae.SetHostPreRenderData();
			ERR(GetParams(&ae,infoP));
			ERR(ae.UnSetPreRenderData());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extraP,ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){
		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ERR(Exec(&ae,infoP));
			ERR2(ae.UnsetSmartRender());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif

#include "Fs_Entry.h"
