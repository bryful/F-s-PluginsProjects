//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "_Skeleton.h"

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
	cs.AddTopic(STR_NOISE_TOPIC, ID_NOISE_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddFloatSlider(	// R
		STR_NOISE_VALUE,			//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_NOISE_VALUE
	);
	cs.AddCheckBox(	// noise frame
		STR_NOISE_ISCOlOR,
		"",
		FALSE,
		ID_NOISE_ISCOLOR//,
		//PF_ParamFlag_SUPERVISE
	);

	cs.EndTopic(ID_NOISE_TOPIC_END);
	
	// ******************************************************
	cs.AddTopic(STR_RECT_TOPIC, ID_RECT_TOPIC,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	A_long def_p[4][4] = { {25,25}, {75, 25}, {75,75},{25,75} };
	for (int i = 0; i < 4; i++) {
		cs.AddPoint(
			(STR_RECT + std::to_string(i + 1)).c_str(),
			def_p[i][0],
			def_p[i][1],
			FALSE,
			ID_RECT_POS1 + i,
			PF_ParamFlag_NONE,
			PF_PUI_INVISIBLE
		);
	}
	cs.AddColor(	// color
		STR_RECT_COLOR,
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_RECT_COLOR
	);
	cs.EndTopic(ID_NOISE_TOPIC_END);
	// ******************************************************
	// ******************************************************
	cs.AddTopic(STR_LINE_TOPIC, ID_LINE_TOPIC,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	A_long def_p2[4][4] = { {25,25}, {50, 25}, {50,75},{75,75} };
	for (int i = 0; i < 4; i++) {
		cs.AddPoint(
			(STR_LINE + std::to_string(i + 1)).c_str(),
			def_p2[i][0],
			def_p2[i][1],
			FALSE,
			ID_LINE_POS1 + i*2,
			PF_ParamFlag_NONE,
			PF_PUI_INVISIBLE
		);
		cs.AddFloatSlider(	// R
			(STR_LINE_W + std::to_string(i + 1)).c_str(),			//Name
			0,				//VALID_MIN
			150,			//VALID_MAX
			0,				//SLIDER_MIN
			30,				//SLIDER_MAX
			5,				//DFLT
			1,				//PREC 小数点以下の桁数
			0,				//DISP
			FALSE,			//WANT_PHASE
			ID_LINE_W1 + i*2,
			PF_ParamFlag_NONE,
			PF_PUI_INVISIBLE
		);
	}
	cs.AddColor(	// color
		STR_LINE_COLOR,
		{ 0xFF, 0xFF, 0x00, 0xFF },
		ID_RECT_COLOR
	);
	cs.EndTopic(ID_NOISE_TOPIC_END);
	/*
	cs.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE,
		PF_ParamFlag_NONE,
		PF_PUI_DISABLED
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
	cs.AddPoint(
		STR_PAINT_POS,
		25,
		25,
		FALSE,
		ID_PAINT_POS
	);
	
	// ----------------------------------------------------------------


	cs.AddPopup(STR_CHAN_MINMAX_MODE,
		STR_CHAN_MINMAX_COUNT,
		STR_CHAN_MINMAX_DFLT,
		STR_CHAN_MINMAX_ITEMS,
		ID_CHAN_MINMAX_MODE
	);
	cs.AddSlider(	// noise offset
		STR_CHAN_MINMAX,		//パラメータの名前
		-1000, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		-100,				//スライダーの最小値 
		100,			//スライダーの最大値
		0,				//デフォルトの値
		ID_CHAN_MINMAX_VALUE
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
	cs.AddFloatSlider(	// R
		STR_START_OPA,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_DRAW_START_OPA
		);
	cs.AddFloatSlider(	// R
		STR_END_OPA,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_DRAW_END_OPA
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
	cs.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE,
		PF_ParamFlag_NONE,
		PF_PUI_DISABLED
	);
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
				A_long mode;
				ERR(ae.GetPOPUP(ID_MODE, &mode));
				switch (mode) {
					case MODE_NOISE:
						hide_themB[ID_NOISE_TOPIC] = FALSE;
						hide_themB[ID_NOISE_VALUE] = FALSE;
						hide_themB[ID_NOISE_ISCOLOR] = FALSE;
						break;
					case MODE_RECT:
						hide_themB[ID_RECT_TOPIC] = FALSE;
						for (int i = 0; i < 4; i++) hide_themB[ID_RECT_POS1 + i] = FALSE;
						hide_themB[ID_RECT_COLOR] = FALSE;
						break;
					case MODE_LINE:
						hide_themB[ID_LINE_TOPIC] = FALSE;
						for (int i = 0; i < 4; i++) {
							hide_themB[ID_LINE_POS1 + i*2] = FALSE;
							hide_themB[ID_LINE_W1 + i*2] = FALSE;
						}
						hide_themB[ID_LINE_COLOR] = FALSE;
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
	ERR(ae->GetFLOAT(ID_NOISE_VALUE, &infoP->noise));
	infoP->noise /= 100;
	ERR(ae->GetCHECKBOX(ID_NOISE_ISCOLOR, &infoP->noise_is_color));
	//*************
	for(int i=0;i<4;i++)
		ERR(ae->GetPOINT(ID_RECT_POS1 + i, &infoP->rect_pos[i]));
	ERR(ae->GetCOLOR(ID_RECT_COLOR, &infoP->rect_color));
	//*************
	for (int i = 0; i < 4; i++) 
	{
		PF_Point pos;
		ERR(ae->GetPOINT(ID_LINE_POS1 + i*2, &pos));
		PF_FpLong w;
		ERR(ae->GetFLOAT(ID_LINE_W1 + i * 2, &w));
		infoP->line_pos[i] = {(float)pos.x,(float)pos.y,(float)w};
	}
	ERR(ae->GetCOLOR(ID_LINE_COLOR, &infoP->line_color));
	/*
	ERR(ae->GetPOINT(ID_PAINT_POS, &infoP->paint_pos));

	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));
	ERR(ae->GetPOPUP(ID_CHAN_MINMAX_MODE, &infoP->chan_minmax_mode));
	infoP->chan_minmax_mode -= 1;
	ERR(ae->GetADD(ID_CHAN_MINMAX_VALUE, &infoP->chan_minmax_value));

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
	ERR(ae->GetFLOAT(ID_DRAW_START_OPA, &infoP->draw_start_opa));
	infoP->draw_start_opa /= 100;
	ERR(ae->GetFLOAT(ID_DRAW_END_OPA, &infoP->draw_end_opa));
	infoP->draw_end_opa /= 100;


	ERR(ae->GetCHECKBOX(ID_DEBUG_FONT_CB, &infoP->debug_font_cb));
	ERR(ae->GetPOINT(ID_DEBUG_FONT_POS, &infoP->debug_font_pos));
	ERR(ae->GetCOLOR(ID_DEBUG_FONT_COLOR, &infoP->debug_font_color));
	*/
	return err;
}
//-------------------------------------------------------------------------------------------------
static inline PF_Point ofP(PF_Point p, A_long x, A_long y)
{
	p.x += x;
	p.y += y;
	return p;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	if (infoP->auto_seed == TRUE) {
		infoP->seed = infoP->seed + ae->frame()*10;
	}
	std::vector<std::vector<float>> buf;
	if (infoP->mode == MODE_RECT) {
		buf.resize(ae->outputInfo.height,std::vector<float>(ae->outputInfo.width,0));
	}
	// ox,oyはオフセット。AEの描画関数は、レイヤーの原点を(0,0)とする座標系で描画するため、原点がずれている場合はオフセットを加える必要がある。
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;

	switch (infoP->mode)
	{
	case MODE_NOISE:
	{
		ERR(NoiseExec(
			ae->in_data,
			ae->input,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->noise,
			infoP->noise_is_color,
			infoP->seed
		));
		break;
	}
	case MODE_RECT:
	{
		std::vector<std::vector<float>> buf(
			ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
		draw_a_rect(
			buf,
			ofP(infoP->rect_pos[0], ox, oy), ofP(infoP->rect_pos[1], ox, oy), ofP(infoP->rect_pos[2], ox, oy), ofP(infoP->rect_pos[3], ox, oy),
			1.0f
		);
		ERR(DrawColorMask(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			&buf,
			infoP->rect_color
		));

		break;
	}
	case MODE_LINE:
	{
		std::vector<std::vector<float>> buf(
			ae->outputInfo.height, std::vector<float>(ae->outputInfo.width, 0));
		draw_polyline(
			buf,
			offsetLinePrm(4,infoP->line_pos, ox, oy),
			1.0f
		);
		ERR(DrawColorMask(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			&buf,
			infoP->line_color
		));
		break;
	}
	}
	/*
	//Paintサンプル
	if (infoP->paint_cb == TRUE) {
		Paint(
			ae->output,
			ae->pixelFormat(),
			infoP->paint_pos.x,
			infoP->paint_pos.y,
			infoP->paint_color
		);

	}

	// MinMax/Blurサンプル
	if ((infoP->minmax != 0) || (infoP->blur > 0))
	{
		Mult(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			FALSE
		);
	}
	if (infoP->minmax != 0) {
		ERR(MinMax(
			ae->in_data, 
			ae->output, 
			ae->pixelFormat(), 
			ae->suitesP, 
			infoP->minmax));
	}
	if (infoP->blur > 0) {
		ERR(Blur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->blur));
	}
	if ((infoP->minmax != 0) || (infoP->blur > 0))
	{
		Mult(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			TRUE
		);
	}
	if(infoP->chan_minmax_value !=0)
	{
		ERR(ChannelMinMax(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->chan_minmax_value,
			infoP->chan_minmax_mode
			));
	}
	// Noiseサンプル
	if ((infoP->noise_size > 0) && (infoP->noise_amount > 0))
	{
		A_long seed;
		if (infoP->noise_auto == TRUE) {
			seed = infoP->noise_seed + ae->frame();
		}
		else {
			seed = infoP->noise_seed;
		}
		NoiseAndBlock(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			(PF_FpShort)infoP->noise_size,
			(PF_FpShort)infoP->noise_amount,
			(PF_FpShort)infoP->noise_accent_amount,
			seed,
			(A_Boolean)infoP->noise_is_color
		);
	}
	if(infoP->draw>1)
	{
		A_long radius = 0;
		if ((infoP->draw == 5) || (infoP->draw == 6))
		{
			PF_FpLong dx = static_cast<PF_FpLong>(infoP->draw_end.x - infoP->draw_start.x);
			PF_FpLong dy = static_cast<PF_FpLong>(infoP->draw_end.y - infoP->draw_start.y);
			radius = (A_long)(sqrt(dx * dx + dy * dy) +0.5);
		}

		switch (infoP->draw)
		{
		case 2:
			//直線
			ERR(DrawLine(
				ae->output,
				ae->pixelFormat(),
				infoP->draw_start.x,
				infoP->draw_start.y,
				infoP->draw_end.x,
				infoP->draw_end.y,
				infoP->draw_color));
			ERR(DrawLine(
				ae->output,
				ae->pixelFormat(),
				infoP->draw_start.x+1,
				infoP->draw_start.y,
				infoP->draw_end.x+1,
				infoP->draw_end.y,
				infoP->draw_color));
			ERR(DrawLine(
				ae->output,
				ae->pixelFormat(),
				infoP->draw_start.x,
				infoP->draw_start.y+1,
				infoP->draw_end.x,
				infoP->draw_end.y+1,
				infoP->draw_color));
			break;
			case 3:
				//矩形
				ERR(DrawBox(
					ae->output,
					ae->pixelFormat(),
					infoP->draw_start.x,
					infoP->draw_start.y,
					infoP->draw_end.x,
					infoP->draw_end.y,
					infoP->draw_color));
				ERR(DrawBox(
					ae->output,
					ae->pixelFormat(),
					infoP->draw_start.x+1,
					infoP->draw_start.y+1,
					infoP->draw_end.x-1,
					infoP->draw_end.y-1,
					infoP->draw_color));
				ERR(DrawBox(
					ae->output,
					ae->pixelFormat(),
					infoP->draw_start.x + 2,
					infoP->draw_start.y + 2,
					infoP->draw_end.x - 2,
					infoP->draw_end.y - 2,
					infoP->draw_color));
				break;
			case 4:
				//楕円
				ERR(DrawBoxFill(
					ae->output,
					ae->pixelFormat(),
					infoP->draw_start.x,
					infoP->draw_start.y,
					infoP->draw_end.x,
					infoP->draw_end.y,
					infoP->draw_color));
				break;
			case 5:
				//円
				ERR(DrawCircle(
					ae->output,
					ae->pixelFormat(),
					infoP->draw_start,
					radius,
					infoP->draw_color));
				//円
				break;
			case 6:
				//円
				ERR(DrawCircleFill(
					ae->output,
					ae->pixelFormat(),
					infoP->draw_start,
					radius,
					infoP->draw_color));
				break;
			case 7:
				//アンチエイリアス
				ERR(DrawAA_Line(
					ae->in_data, 
					ae->output,
					ae->pixelFormat(),
					ae->suitesP,
					(float)infoP->draw_start.x,
					(float)infoP->draw_start.y,
					(float)infoP->draw_end.x,
					(float)infoP->draw_end.y,
					infoP->draw_color,
					(float)infoP->draw_start_opa,
					(float)infoP->draw_end_opa,
					5,
					2
					));
				break;
		};
	}

	if (infoP->debug_font_cb == TRUE) {
		char buf[256];
		PF_InData* in_data = ae->in_data;
		PF_SPRINTF(buf, "Debug Point X:%d Y:%d", infoP->debug_font_pos.x, infoP->debug_font_pos.y);
		DrawDebugString(
			ae->output,
			ae->pixelFormat(),
			infoP->debug_font_pos.x,
			infoP->debug_font_pos.y,
			buf,
			infoP->debug_font_color
		);
	}
	*/
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

