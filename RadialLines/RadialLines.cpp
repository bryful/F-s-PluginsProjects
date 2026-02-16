//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "RadialLines.h"

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

	PF_Pixel 	def_color[] = {
		{ 0xFF, 0x00, 0x00, 0x00 },
		{ 0xFF, 0x30, 0x30, 0x30 },
		{ 0xFF, 0x40, 0x40, 0x40 },
		{ 0xFF, 0x50, 0x50, 0x50 },
		{ 0xFF, 0xB0, 0xB0, 0xB0 },
		{ 0xFF, 0xCC, 0xCC, 0xCC },
		{ 0xFF, 0xDD, 0xDD, 0xDD },
		{ 0xFF, 0xFF, 0xFF, 0xFF }
	};

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddCheckBox(	// noise frame
		STR_AUTO_SEED,
		"on",
		TRUE,
		ID_AUTO_SEED

	);
	cs.AddSlider(	// 
		STR_SEED,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-1000,				//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED
	);
	cs.AddSlider(	// 
		STR_VALUE,	//パラメータの名前
		0, 			//数値入力する場合の最小値
		1000,		//数値入力する場合の最大値
		0,			//スライダーの最小値 
		100,		//スライダーの最大値
		50,			//デフォルトの値
		ID_VALUE
	);
	cs.AddPoint(
		STR_POS,
		50,
		50,
		FALSE,
		ID_POS
	);
	// ----------------------------------------------------------------
	cs.AddFloatSlider(	// R
		STR_LENGTH,		//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		80,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH
	);
	cs.AddFloatSlider(	// R
		STR_LENGTH_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		80,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH_RAND
	);
	// ----------------------------------------------------------------
	cs.AddFloatSlider(	// R
		STR_WEIGHT,		//Name
		0.5,			//VALID_MIN
		40,				//VALID_MAX
		0.5,			//SLIDER_MIN
		12,				//SLIDER_MAX
		6,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_WEIGHT
	);
	cs.AddFloatSlider(	// R
		STR_WEIGHT_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		80,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH_RAND
	);
	// ----------------------------------------------------------------
	cs.AddFloatSlider(	// R
		STR_OPACITY,		//Name
		0,				//VALID_MIN
		200,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_OPACITY
	);
	cs.AddFloatSlider(	// R
		STR_OPACITY_RAND,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		50,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_OPACITY_RAND
	);
	cs.AddFloatSlider(	// R
		STR_HYPERBOLIC,	//Name
		-10,				//VALID_MIN
		50,			//VALID_MAX
		-2,				//SLIDER_MIN
		10,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_HYPERBOLIC
	);
	cs.AddFloatSlider(	// R
		STR_NOISE,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		10,			//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE
	);
	cs.AddTopic(
		STR_COLOR_TOPIC,
		ID_COLOR_TOPIC
	);
	cs.AddPopup(	// 
		STR_COLOR_COUNT,	//パラメータの名前
		8, 					
		1,					
		"1|2|3|4|5|6|7|8",					 
		ID_COLOR_COUNT,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	for(int i = 0; i < 8; i++) {
		std::string str = STR_COLOR + std::to_string(i + 1);
		cs.AddColor(
			str.c_str(),	//パラメータの名前
			def_color[i],	//デフォルトの色
			ID_COLOR1 + i,
			PF_ParamFlag_NONE,
			(i == 0) ? PF_PUI_NONE : PF_PUI_INVISIBLE
		);
	}
	cs.EndTopic(ID_COLOR_END);
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
			if (!err) {
				//PF_Boolean b = FALSE;
				//ERR(ae.GetCHECKBOX(ID_AUTO_SEED, &b));
				//ERR(ae.UI_DISABLE(ID_SEED, b));
				ERR(ae.GetNewEffectStreamAll());
				if (!err) {
					A_Boolean hide_themB[ID_NUM_PARAMS];
					for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
					A_long color_count = 1;
					ERR(ae.GetADD(ID_COLOR_COUNT, &color_count));
					for (A_long i = 0 + 1; i < 8; i++)hide_themB[ID_COLOR1 + i] = TRUE;
					if (color_count>0) {
						for (A_long i = 0 + 1; i < color_count; i++)hide_themB[ID_COLOR1 + i] = FALSE;
					}
					for (A_long i = 1; i < ID_NUM_PARAMS; i++)
						ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				}
				out_data->out_flags |= PF_OutFlag_REFRESH_UI;
			}
		}
		
	}catch (PF_Err& errP) {
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
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_AUTO_SEED,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->auto_seed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	if (infoP->auto_seed) {
		infoP->seed += ae->frame();
	}
	ERR(ae->GetADD(ID_VALUE, &infoP->value));
	ERR(ae->GetFLOATPOINT(ID_POS, &infoP->cente_pos));
	
	ERR(ae->GetFLOAT(ID_LENGTH, &infoP->length_par));
	infoP->length_par /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH_RAND, &infoP->length_rand));
	infoP->length_rand /= 100;

	ERR(ae->GetFLOAT(ID_WEIGHT, &infoP->weight));
	ERR(ae->GetFLOAT(ID_LENGTH_RAND, &infoP->weight_rand));
	infoP->weight_rand /= 100;

	ERR(ae->GetFLOAT(ID_OPACITY, &infoP->opacity_par));
	infoP->opacity_par /= 100;
	ERR(ae->GetFLOAT(ID_OPACITY_RAND, &infoP->opacity_rand));
	infoP->opacity_rand /= 100;

	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));
	ERR(ae->GetFLOAT(ID_NOISE, &infoP->noise));
	infoP->noise /= 100;

	ERR(ae->GetPOPUP(ID_COLOR_COUNT, &infoP->colorCount));

	for(int i = 0; i < 8; i++) {
		ERR(ae->GetCOLOR(ID_COLOR1 + i, &infoP->colors[i]));
	}



	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err CalcParams(NF_AE* ae, ParamInfo* infoP)
{
	PF_Err		err = PF_Err_NONE;
	//タッチ線の上下左右の発生回数を計算
	infoP->values[0] =
	infoP->values[1] =
	infoP->values[2] =
	infoP->values[3] = infoP->value;
	if (infoP->value > 0) {
		//まず上下から
		A_long cy = ae->outputInfo.height / 2; //２分の一にしておく
		A_long py = AE_CLAMP((A_long)(infoP->cente_pos.y+0.5),0, ae->outputInfo.height);
		if ((cy / 2) == (py / 2)) {
			//大体同じなら数を一緒とする。
		}
		else if (py <= 0) {
			infoP->values[2] += infoP->values[0];
			infoP->values[0] = 0;
		}
		else if (py >= ae->outputInfo.height - 1) {
			infoP->values[0] += infoP->values[2];
			infoP->values[2] = 0;
		}
		else if (py < cy) {
			float vv = (float)(cy - py) / cy; // 0..1
			infoP->values[2] += (A_long)(infoP->values[0]* vv);
			infoP->values[0] = (A_long)(infoP->values[0]*(1-vv));
		}
		else {
			float vv = (float)(py - cy) / cy; // 0..1
			infoP->values[0] += (A_long)(infoP->values[2] * vv);
			infoP->values[2] = (A_long)(infoP->values[2] * (1-vv));
		}
		//まず左右から
		A_long cx = ae->outputInfo.width / 2; //２分の一にしておく
		A_long px = AE_CLAMP((A_long)(infoP->cente_pos.x + 0.5), 0, ae->outputInfo.width);
		if ((cx / 2) == (px / 2)) {
			//大体同じなら数を一緒とする。
		}
		else if (px <= 0) {
			infoP->values[1] += infoP->values[3];
			infoP->values[3] = 0;
		}
		else if (px >= ae->outputInfo.width - 1) {
			infoP->values[3] += infoP->values[1];
			infoP->values[1] = 0;
		}
		else if (px < cx) {
			float vv = (float)((double)(cx - px) / cx);
			infoP->values[1] += (A_long)(infoP->values[3] * vv);
			infoP->values[3] = (A_long)(infoP->values[3] * (1-vv));
		}
		else {
			float vv = (float)((double)(px - cx) / cx);
			infoP->values[3] += (A_long)(infoP->values[1] * vv);
			infoP->values[1] = (A_long)(infoP->values[1] * (1 - vv));
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;
	CalcParams(ae, infoP);
	//画面をコピー
	if(infoP->auto_seed) {
		infoP->seed += ae->frame();
	}
	PF_Pixel clear_color = { 0,0,0,0 };
	PF_FILL(&clear_color, NULL, ae->output);
	//ae->CopyInToOut();
	ERR(Exec8_16_32(ae, infoP));
	ERR(Mult(ae->in_data, ae->output, ae->pixelFormat(),ae->suitesP,TRUE));
	ERR(HyperbolicAlpha(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->hyperbolic));
	A_long noise_count = (A_long)(infoP->noise * ae->outputInfo.width* ae->outputInfo.height/8);
	ERR(ApplyANoise(ae, noise_count,infoP->seed));
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

