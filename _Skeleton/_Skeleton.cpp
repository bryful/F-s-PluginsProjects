//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "_Skeleton.h"

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

	CParamsSetup cs(in_data, out_data);
	cs.AddSlider(	// noise offset
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
	cs.AddFloatSilder(	// R
		STR_R,			//Name
		-2,				//VALID_MIN
		2,				//VALID_MAX
		-1,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_R
	);
	cs.AddFloatSilder(	// R
		STR_G,			//Name
		-2,				//VALID_MIN
		2,				//VALID_MAX
		-1,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_G
	);
	cs.AddFloatSilder(	// R
		STR_B,			//Name
		-2,				//VALID_MIN
		2,				//VALID_MAX
		-1,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_B
	);
	cs.AddFloatSilder(	// R
		STR_NOISE,			//Name
		0,				//VALID_MIN
		1,				//VALID_MAX
		0,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE
	);
		
	cs.AddCheckBox(	// noise frame
		STR_NOISE_FRAME1,
		STR_NOISE_FRAME2,
		TRUE,
		ID_NOISE_FRAME,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddSlider(	// noise offset
		STR_NOISE_OFFSET,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		300,			//スライダーの最大値
		10,				//デフォルトの値
		ID_NOISE_OFFSET,
		PF_ParamFlag_NONE,
		PF_PUI_DISABLED
	);
	cs.AddCheckBox(
		STR_HIDDEN_ON1,
		STR_HIDDEN_ON2,
		FALSE,
		ID_HIDDEN_ON,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddTopic(STR_TOPIC, ID_TOPIC, PF_ParamFlag_START_COLLAPSED);


	//cs.SetParamFlags();
	cs.AddSlider(	// add slider
		STR_ADD_SLIDER,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		10,				//デフォルトの値
		ID_ADD_SLIDER,
		PF_ParamFlag_CANNOT_TIME_VARY//これをつけるとキーフレームが撃てなくなる
	);
	cs.AddFixedSlider(		STR_FIXED_SLIDER,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		10,				//デフォルトの値
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		ID_FIXED_SLIDER,
		PF_ParamFlag_CANNOT_TIME_VARY//これをつけるとキーフレームが撃てなくなる
	);
	cs.AddFloatSilder(	// float slider
		STR_FLOAT_SLIDER,	//パラメータの名前
		-10000, 				//数値入力する場合の最小値
		10000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		500,				//デフォルトの値
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_FLOAT_SLIDER
	);
	cs.AddColor(	// color
		STR_COLOR,
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_COLOR,
		PF_ParamFlag_CANNOT_TIME_VARY//これをつけるとキーフレームが撃てなくなる
	);
	cs.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE
	);
	cs.AddPopup(		STR_POPUP,
		STR_POPUP_COUNT,
		STR_POPUP_DFLT,
		STR_POPUP_ITEMS,
		ID_POPUP
	);
	cs.AddPoint(
		STR_POINT,
		50, 
		50,
		FALSE,
		ID_POINT
	);
	cs.AddCheckBox(	// checkbox
		STR_CHECKBOX1,
		STR_CHECKBOX2,
		FALSE,
		ID_CHECKBOX
	);
	cs.EndTopic(ID_TOPIC_END);


	cs.AddButton(	// button
		STR_BUTTON1,
		STR_BUTTON2,
		ID_BUTTON,
		PF_ParamFlag_SUPERVISE
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

	try{
		CFsAE ae;
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
		if((!err)&&(in_data->appl_id != 'PrMr')){
			//--------------------
			
			ERR(ae.GetNewEffectStreamAll());
			if (!err){
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) hide_themB[i] =FALSE;
				PF_Boolean b;
				ERR(ae.GetCHECKBOX(ID_HIDDEN_ON,&b));
				if (b){
					for ( A_long i=ID_HIDDEN_ON+1; i<ID_NUM_PARAMS; i++)hide_themB[i] =TRUE;
				}
				for ( A_long i=1; i<ID_NUM_PARAMS-1; i++) 
					ERR(ae.SetDynamicStreamFlag(i,AEGP_DynStreamFlag_HIDDEN,hide_themB[i]));
			}
			//--------------------
			if (!err){
				PF_Boolean b = FALSE;
				ERR(ae.GetCHECKBOX(ID_NOISE_FRAME,&b));
				ERR(ae.UI_DISABLE(ID_NOISE_OFFSET, b));
			}
			//--------------------
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
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
	CFsAE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_NOISE_FRAME,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));

	ERR(ae->GetFLOAT(ID_R,&infoP->r));
	ERR(ae->GetFLOAT(ID_G,&infoP->g));
	ERR(ae->GetFLOAT(ID_B,&infoP->b));

	ERR(ae->GetFLOAT(ID_NOISE,&infoP->noise));
	ERR(ae->GetCHECKBOX(ID_NOISE_FRAME,&infoP->noise_frame));
	ERR(ae->GetADD(ID_NOISE_OFFSET,&infoP->noise_offset));


	ERR(ae->GetADD(ID_ADD_SLIDER,&infoP->add));
	ERR(ae->GetFIXED(ID_FIXED_SLIDER, &infoP->fxd));
	ERR(ae->GetFLOAT(ID_FLOAT_SLIDER, &infoP->flt));
	ERR(ae->GetCOLOR(ID_COLOR, &infoP->col));
	ERR(ae->GetANGLE(ID_ANGLE,&infoP->agl));
	ERR(ae->GetPOPUP(ID_POPUP, &infoP->pop));
	ERR(ae->GetFIXEDPOINT(ID_POINT,&infoP->pnt));
	ERR(ae->GetCHECKBOX(ID_CHECKBOX, &infoP->cbx));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

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
	if ((infoP->r != 0) || (infoP->g != 0) || (infoP->b != 0) || (infoP->noise > 0))
	{
		if (infoP->noise_frame == TRUE) {
			infoP->noise_offset;
		}
		else {
			infoP->noise_offset += ae->frame();
		}
		FilterInfo info;
		info.r = infoP->r;
		info.g = infoP->g;
		info.b = infoP->b;
		info.noise = infoP->noise;
		info.noise_offset = 0;
		if (infoP->noise_frame == TRUE) {
			info.noise_offset += ae->frame();
		}
		else {
			info.noise_offset = infoP->noise_offset;
		}
		FilterImage(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, &info);
	}
	if (infoP->cbx == TRUE) {
		char buf[256];
		PF_InData* in_data = ae->in_data;
		PF_SPRINTF(buf, "Point X:%f Y:%f", (double)infoP->pnt.x / 65536, (double)infoP->pnt.y / 65536);
		DrawDebugString(
			ae->output,
			ae->pixelFormat(),
			(A_long)((double)infoP->pnt.x/65536+0.f),
			(A_long)((double)infoP->pnt.y / 65536 + 0.f),
			buf,
			infoP->col
		);
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
		CFsAE ae;
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

