//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "InnerFlare.h"

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
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COLOR,
		0xff,
		0xFF,
		PF_MAX_CHAN8,
		ID_COLOR
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_MINMAX,	//パラメータの名前
		-1000, 		//数値入力する場合の最小値
		1000,		//数値入力する場合の最大値
		-10,		//スライダーの最小値 
		10,		//スライダーの最大値
		0,		//デフォルトの値
		ID_MINMAX
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_BLUR,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		1000,		//数値入力する場合の最大値
		0,		//スライダーの最小値 
		100,		//スライダーの最大値
		0,		//デフォルトの値
		ID_BLUR
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HYPERBOLIC,	//Name
		-10,				//VALID_MIN
		50,					//VALID_MAX
		-2,					//SLIDER_MIN
		10,					//SLIDER_MAX
		1,					//CURVE_TOLERANCE
		0,					//DFLT
		1,					//PREC
		0,					//DISP
		0,					//WANT_PHASE
		ID_HYPERBOLIC
	);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_REVERCE,
		"on",
		FALSE,
		0,
		ID_REVERCE
	);
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		STR_WHITE,
		"on",
		FALSE,
		0,
		ID_WHITE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		STR_BLEND,
		"on",
		FALSE,
		0,
		ID_BLEND
	);
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
	
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCOLOR(ID_COLOR, &infoP->color));
	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));
	ERR(ae->GetCHECKBOX(ID_REVERCE, &infoP->reverce));
	ERR(ae->GetCHECKBOX(ID_WHITE, &infoP->isWhite));
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->isBlend));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;
	infoP->blur = (A_long)((PF_FpLong)infoP->blur * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den + 0.5);
	infoP->minmax = (A_long)((PF_FpLong)infoP->minmax * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den + 0.5);

	if ((infoP->blur <= 0) && (infoP->minmax == 0)) {
		if(infoP->isBlend) {
			ae->CopyInToOut();
		}
		else {
			PF_FILL(NULL,NULL,ae->output);
		}
		return err;
	}
	ERR(AlphaCopyM(ae->in_data, ae->input, ae->output, ae->pixelFormat(), ae->suitesP,
		infoP->reverce,infoP->isWhite));
	//return err;

	if (infoP->minmax != 0) {
		MinMax(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->minmax);
	}
	if (infoP->blur > 0) {
		Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, FALSE);
		Blur(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->blur);
		Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, TRUE);
	}
	if (infoP->hyperbolic != 0.0) {
		HyperbolicAlpha(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->hyperbolic);
	}
	ERR(AlphaCopyRM(ae->in_data, ae->input, ae->output, ae->pixelFormat(), ae->suitesP, 
		infoP->color, infoP->reverce,infoP->isWhite));

	if(infoP->isBlend) {
		ERR(BlendBehind(
			ae->in_data, ae->input, ae->output, ae->pixelFormat(), ae->suitesP));
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

