//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "MuzzleFlash.h"

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
	cs.AddPoint(
		STR_PRONGS_POINT,
		50,
		50,
		FALSE,
		ID_PRONGS_POINT
	); cs.AddSlider(	// 
		STR_PRONGS_COUNT,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		100,			//スライダーの最大値
		3,				//デフォルトの値
		ID_PRONGS_COUNT
	);
	cs.AddFloatSlider(	// R
		STR_PRONGS_ROOT_SIZE,			//Name
		5,				//VALID_MIN
		200,			//VALID_MAX
		10,				//SLIDER_MIN
		50,				//SLIDER_MAX
		30,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_PRONGS_ROOT_SIZE
	);
	cs.AddFloatSlider(	// R
		STR_PRONGS_LENGTH,			//Name
		10,				//VALID_MIN
		2000,			//VALID_MAX
		20,				//SLIDER_MIN
		300,			//SLIDER_MAX
		200,				//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_PRONGS_LENGTH
	);
	cs.AddSlider(	// 
		STR_PRONGS_BRANCH,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		10,				//スライダーの最大値
		4,				//デフォルトの値
		ID_PRONGS_BRANCH
	);
	cs.AddAngle(	// angle
		STR_PRONGS_DIR,
		0,
		ID_PRONGS_DIR
	);
	cs.AddFloatSlider(	// R
		STR_PRONGS_ANGLE,	//Name
		0,				//VALID_MIN
		360,			//VALID_MAX
		0,				//SLIDER_MIN
		360,			//SLIDER_MAX
		360,			//DFLT
		0,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_PRONGS_ANGLE
	);
	cs.AddFloatSlider(	// R
		STR_PRONGS_SCALE,	//Name
		0,				//VALID_MIN
		10000,			//VALID_MAX
		0,				//SLIDER_MIN
		500,			//SLIDER_MAX
		100,			//DFLT
		0,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_PRONGS_SCALE
	);
	cs.AddColor(// color
		STR_PRONGS_COLOR,
		{ 0xFF, 0XFF,0xFF, 0xFF },
		ID_PRONGS_COLOR
	);
	cs.AddCheckBox(	// noise frame
		STR_AUTO_SEED,
		"",
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

	A_FloatPoint pos = { 0,0 };
	ERR(ae->GetFLOATPOINT(ID_PRONGS_POINT, &pos));
	infoP->pointX = (float)pos.x;
	infoP->pointY = (float)pos.y;
	ERR(ae->GetADD(ID_PRONGS_COUNT, &infoP->count));
	ERR(ae->GetFLOAT_float(ID_PRONGS_ROOT_SIZE, &infoP->rootSize));
	ERR(ae->GetFLOAT_float(ID_PRONGS_LENGTH, &infoP->length));
	ERR(ae->GetADD(ID_PRONGS_BRANCH, &infoP->branch));
	ERR(ae->GetANGLE(ID_PRONGS_DIR, &infoP->direction));
	ERR(ae->GetFLOAT_float(ID_PRONGS_ANGLE, &infoP->angle));
	ERR(ae->GetFLOAT_float(ID_PRONGS_SCALE, &infoP->scale));
	infoP->scale /= 100;
	ERR(ae->GetCOLOR(ID_PRONGS_COLOR, &infoP->color));
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
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
	//ERR(ae->CopyInToOut());
	infoP->length = (float)ae->downScale(infoP->length);
	if (infoP->length <=0|| infoP->scale<=0) {
		return err;
	}
	if (infoP->autoSeed) {
		infoP->seed += ae->frame()*2;
	}


	ERR(MuzzleFlashExec(
		ae->in_data,
		ae->suitesP,
		ae->pixelFormat(),
		ae->output,
		infoP->pointX,
		infoP->pointY,
		infoP->count,
		infoP->rootSize,
		infoP->length,
		infoP->branch,
		infoP->direction,
		infoP->angle,
		infoP->color,
		infoP->scale,
		infoP->seed
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

