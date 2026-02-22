//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Sputtering.h"

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

	PF_Pixel default_colors[TARGET_COLOR_COUNT] = {
		{ 255,255,0,0 },
		{ 255,0,255,0 },
		{ 255,0,0,255 },
		{ 255,255,255,0 },
		{ 255,255,0,255 },
		{ 255,0,255,255 },
		{ 255,255,128,0 },
		{ 255,128,0,255 }
	};

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddTopic(STR_TARGET_TOPIC, ID_TARGET_TOPIC);
	cs.AddPopup(STR_TARGET,
		STR_TARGET_COUNT,
		STR_TARGET_DFLT,
		STR_TARGET_ITEMS,
		ID_TARGET,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	for (A_long i=0; i < TARGET_COLOR_COUNT; i++) {
		std::string str = STR_TARGET_COLOR + std::to_string(i + 1);
		cs.AddColor(	// color
			str.c_str(),
			default_colors[i],
			ID_TARGET_COLOR1 + i,
			PF_ParamFlag_NONE,
			PF_PUI_INVISIBLE
		);
	}

	cs.AddCheckBox(	
		STR_BORDER_DIRECTION,
		"on",
		FALSE,
		ID_BORDER_DIRECTION,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddAngle(
		STR_DIRECTION_ANGLE,
		45,
		ID_DIRECTION_ANGLE,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.EndTopic(ID_TARGET_TOPIC_END);
	// ----------------------------------------------------------------
	cs.AddTopic(STR_PARAMS_TOPIC, ID_PARAMS_TOPIC);
	cs.AddLayer(	// layer
		STR_SPUT_LAYER,
		PF_LayerDefault_NONE,
		ID_SPUT_LAYER
	);
	cs.AddFloatSlider(	// R
		STR_SIZE,			//Name
		20,				//VALID_MIN
		200,			//VALID_MAX
		20,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SIZE
	);
	cs.AddFloatSlider(	// R
		STR_SIZE_RAND,			//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SIZE_RAND
	);
	cs.AddPopup(STR_SPUT_LOOP,
		STR_SPUT_LOOP_COUNT,
		STR_SPUT_LOOP_DFLT,
		STR_SPUT_LOOP_ITEMS,
		ID_SPUT_LOOP,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddSlider(	// noise offset
		STR_SPUT_LOOP_KOMA,	//パラメータの名前
		1, 		//数値入力する場合の最小値
		24,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		6,			//スライダーの最大値
		1,				//デフォルトの値
		ID_SPUT_LOOP_KOMA
	);
	cs.EndTopic(ID_PARAMS_TOPIC_END);
	// ----------------------------------------------------------------
	cs.AddTopic(STR_PARAMS2_TOPIC, ID_PARAMS2_TOPIC);

	cs.AddFloatSlider(	// R
		STR_AREA_RATE,			//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		10,			//SLIDER_MAX
		5,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_AREA_RATE
	);
	cs.AddFloatSlider(	// R
		STR_BODER_RATE,			//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		20,				//SLIDER_MAX
		10,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_BORDER_RATE
	);
	cs.AddSlider(	// noise offset
		STR_SPAWN_COUNT,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		200,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		50,				//スライダーの最大値
		10,				//デフォルトの値
		ID_SPAWN_COUNT
	);
	cs.AddFloatSlider(	// R
		STR_SPAWN_OFFSET,			//Name
		5,				//VALID_MIN
		1000,			//VALID_MAX
		5,				//SLIDER_MIN
		100,			//SLIDER_MAX
		50,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SPAWN_OFFSET
	);
	cs.AddFloatSlider(	// R
		STR_OPACITY,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_OPACITY
	);
	cs.AddFloatSlider(	// R
		STR_OPACITY_RAND,	//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		0,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_OPACITY_RAND
	);
	cs.AddCheckBox(
		STR_AUTO_SEED,
		"on",
		FALSE,
		ID_AUTO_SEED
	);
	cs.AddSlider(	// noise offset
		STR_SEED,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-1000,				//スライダーの最小値 
		1000,			//スライダーの最大値
		10,				//デフォルトの値
		ID_SEED
	);
	cs.EndTopic(ID_PARAMS2_TOPIC_END);
	// ----------------------------------------------------------------
	cs.AddTopic(STR_COLORS_TOPIC, ID_COLOR_TOPIC);
	cs.AddPopup(STR_COLORS_NUM,
		STR_COLORS_COUNT,
		STR_COLORS_DFLT,
		STR_COLORS_ITEMS,
		ID_COLOR_NUM,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	for(A_long i=0; i < TARGET_COLOR_COUNT; i++) {
		std::string str = STR_COLORS + std::to_string(i + 1);
		cs.AddColor(	// color
			str.c_str(),
			default_colors[i],
			ID_COLOR1 + i,
			PF_ParamFlag_NONE,
			PF_PUI_INVISIBLE
		);
	}

	cs.EndTopic(ID_COLOR_TOPIC_END);
	cs.AddCheckBox(
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
		ERR(ae.HandleChangedParam(in_data, out_data, params, outputP, extraP, ID_NUM_PARAMS));
		if (!err) {
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				A_long mode = 0;
				ERR(ae.GetPOPUP(ID_TARGET, &mode));
				for (A_long i = 0; i < TARGET_COLOR_COUNT; i++) hide_themB[ID_TARGET_COLOR1+i] = TRUE;
				if (mode > 1)
				{
					for (A_long i = 0; i < mode - 1; i++) hide_themB[ID_TARGET_COLOR1 + i] = FALSE;
				}
				PF_Boolean b = FALSE;
				hide_themB[ID_DIRECTION_ANGLE] = TRUE;
				ERR(ae.GetCHECKBOX(ID_BORDER_DIRECTION, &b));
				hide_themB[ID_DIRECTION_ANGLE] = !b;
				hide_themB[ID_AREA_RATE] = b;

				A_long sput_loop_mode = 0;
				ERR(ae.GetPOPUP(ID_SPUT_LOOP, &sput_loop_mode));
				//hide_themB[ID_SPUT_LOOP_KOMA] = !(sput_loop_mode==2);
				hide_themB[ID_AUTO_SEED] = (sput_loop_mode == 2);

				for (A_long i = 0; i < COLOR_COUNT; i++) hide_themB[ID_COLOR1 + i] = TRUE;
				mode = 0;
				ERR(ae.GetPOPUP(ID_COLOR_NUM, &mode));
				for (A_long i = 0; i < mode; i++) hide_themB[ID_COLOR1 + i] = FALSE;

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
		PF_Boolean autoSeed = (PF_Boolean)def.u.bd.value;
		ERR(ae.checkin_param(&def));
		PF_ParamDef def2;
		AEFX_CLR_STRUCT(def2);
		ERR(ae.checkout_param(ID_SPUT_LOOP, &def2));
		PF_Boolean isLoop = ( (A_long)def2.u.pd.value == 2);
		ERR(ae.checkin_param(&def2));

		ERR(ae.SetOutFlag_NON_PARAM_VARY(autoSeed|| isLoop));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	ERR(ae->GetPOPUP(ID_TARGET, &infoP->targetColorCount));
	infoP->targetColorCount -= 1;
	for(int i=0; i < TARGET_COLOR_COUNT; i++) {
		ERR(ae->GetCOLOR(ID_TARGET_COLOR1 + i, &infoP->targetColors[i]));
	}
	ERR(ae->GetCHECKBOX(ID_BORDER_DIRECTION, &infoP->is_border_direction));
	PF_Fixed fxd=0;
	ERR(ae->GetANGLE(ID_DIRECTION_ANGLE, &fxd));
	infoP->direction_angle = (float)fxd / 65536.0f;
	double  d = 0;
	ERR(ae->GetFLOAT(ID_SIZE, &d));
	infoP->size = (float)(d/100);
	ERR(ae->GetFLOAT(ID_SIZE_RAND, &d));
	infoP->sizeRandom = (float)(d / 100);
	ERR(ae->GetPOPUP(ID_SPUT_LOOP, &infoP->sput_loop_mode));
	ERR(ae->GetADD(ID_SPUT_LOOP_KOMA, &infoP->sput_loop_koma));

	ERR(ae->GetFLOAT(ID_AREA_RATE, &d));
	infoP->area_rate = (float)(d / 100);
	ERR(ae->GetFLOAT(ID_BORDER_RATE, &d));
	infoP->border_rate = (float)(d / 100);
	ERR(ae->GetADD(ID_SPAWN_COUNT, &infoP->spawn_count));
	ERR(ae->GetFLOAT(ID_SPAWN_OFFSET, &d));
	infoP->spawn_offset = (float)d;
	ERR(ae->GetFLOAT(ID_OPACITY, &d));
	infoP->opacity = (float)(d / 100);
	ERR(ae->GetFLOAT(ID_OPACITY_RAND, &d));
	infoP->opacityRand = (float)(d / 100);
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));

	ERR(ae->GetPOPUP(ID_COLOR_NUM, &infoP->colorCount));
	for (int i = 0; i < COLOR_COUNT; i++) {
		ERR(ae->GetCOLOR(ID_COLOR1 + i, &infoP->colors[i]));
	}
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;

	PF_FILL(NULL, NULL, ae->output);
	//不透明部分の検出、targetに書き込む
	std::vector<A_u_char> target;
	target.resize(ae->outputInfo.width* ae->outputInfo.height);

	ERR(TargetWorldExec(
		ae->in_data,
		ae->input,
		ae->pixelFormat(),
		ae->suitesP,
		infoP->targetColorCount,
		infoP->targetColors,
		&target
		));
	//targetをもとに、borderを検出してtarget2に書き込む
	std::vector<A_u_char> target2;
	target2.resize(ae->outputInfo.width * ae->outputInfo.height);
	ERR(BorderExec(
		ae->in_data,
		ae->suitesP,
		&target,
		&target2,
		ae->outputInfo.width,
		ae->outputInfo.height,
		infoP->is_border_direction,
		infoP->direction_angle
	));
	
	
	
	//ERR(ae->CopyInToOut());
	std::vector<std::vector<A_u_char>> sput = GetDefSpatData(infoP->size);

	PF_ParamDef checkout_param;
	err = PF_CHECKOUT_PARAM(in_data,
		ID_SPUT_LAYER,
		in_data->current_time,
		in_data->time_step,
		in_data->time_scale,
		&checkout_param);
	if (!err) {
		PF_EffectWorld* other_layer_world = &checkout_param.u.ld;
		if (other_layer_world->data) {
			int sz = 48;
				int cw = other_layer_world->width /sz;
				int ch = other_layer_world->height / sz;
				int nsz = (int)((float)sz * infoP->size + 0.5);
				int nw = cw * nsz;
				int nh = ch * nsz;
				if (nw > sz && nh > sz) {
					PF_EffectWorld temp_world;
					ae->NewWorld(nw, nh, ae->pixelFormat(), &temp_world);
					PF_InData* in_data = ae->in_data;
					PF_Rect src_rect = { 0,0,other_layer_world->width, other_layer_world->height };
					PF_Rect dst_rect = { 0,0,nw,nh };
					PF_COPY(other_layer_world, &temp_world, &src_rect, &dst_rect);
					sput = GetSpatDataFromWorld(&temp_world, ae->pixelFormat(), nsz, nsz);
					ae->DisposeWorld(&temp_world);
				}
		}
		// 4. 「他のレイヤ」をチェックイン（必須！）
		err = PF_CHECKIN_PARAM(in_data, &checkout_param);

	}
	// Target2をoutputに書き込む
	/*
	ERR(FromTargetWorldExec(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		&target2
	));
	*/
	if ((infoP->area_rate > 0 || infoP->border_rate > 0)&&(infoP->opacity>0)) {
		ERR(SputMain(
			ae,
			infoP,
			&target2,
			&sput
		));
	}
	if(infoP->isBlend) {
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
		{
			PF_PreRenderExtra* prExtra = reinterpret_cast<PF_PreRenderExtra*>(extraP);
			err = ae.PreRender(in_data, out_data, prExtra, sizeof(ParamInfo), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ae.SetHostPreRenderData();
					ERR(GetParams(&ae, infoP));
					ERR(ae.UnSetPreRenderData());
					ae.UnlockPreRenderData();
					PF_RenderRequest req = prExtra->input->output_request;
					PF_CheckoutResult checkout_res;
					err = prExtra->cb->checkout_layer(in_data->effect_ref,
						ID_SPUT_LAYER,
						ID_SPUT_LAYER,
						&req,
						in_data->current_time,
						in_data->time_step,
						in_data->time_scale,
						&checkout_res);
					if (!err) {
						UnionLRect(&checkout_res.result_rect, &prExtra->output->result_rect);
						UnionLRect(&checkout_res.max_result_rect, &prExtra->output->max_result_rect);
        
						prExtra->output->solid = FALSE;
					}
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		}
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

