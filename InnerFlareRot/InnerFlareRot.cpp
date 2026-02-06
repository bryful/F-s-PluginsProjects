//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "InnerFlareRot.h"

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
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ROT, 0, ID_ROT);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LENGTH,	//Name
		-0,							//VALID_MIN
		1000,						//VALID_MAX
		0,							//SLIDER_MIN
		100,						//SLIDER_MAX
		1,							//CURVE_TOLERANCE
		0,							//DFLT
		1,							//PREC
		0,							//DISP
		0,							//WANT_PHASE
		ID_LENGTH
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OFFSET,	//Name
		0,						//VALID_MIN
		10,							//VALID_MAX
		1,							//SLIDER_MIN
		5,							//SLIDER_MAX
		1,							//CURVE_TOLERANCE
		1,							//DFLT
		1,							//PREC
		2,							//DISP
		0,							//WANT_PHASE
		ID_OFFSET
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		STR_REVERSE,
		STR_REVERSE2,
		FALSE,
		0,
		ID_REVERSE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_MINMAX,	//パラメータの名前
		-200, 		//数値入力する場合の最小値
		200,		//数値入力する場合の最大値
		-50,		//スライダーの最小値 
		50,		//スライダーの最大値
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
	PF_Err		err = PF_Err_NONE;
	ERR(ae->GetCOLOR(ID_COLOR, &infoP->color));
	PF_Fixed	fixedRot = 0;
	ERR(ae->GetANGLE(ID_ROT, &fixedRot));
	infoP->rot = F_FIX2FLT(fixedRot);
	ERR(ae->GetFLOAT(ID_LENGTH, &infoP->length));
	ERR(ae->GetCHECKBOX(ID_REVERSE, &infoP->reverse));
	ERR(ae->GetADD(ID_MINMAX, &infoP->minmax));
	ERR(ae->GetFLOAT(ID_OFFSET, &infoP->offset));
	ERR(ae->GetADD(ID_BLUR, &infoP->blur));
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err err = PF_Err_NONE;

	PF_InData* in_data = ae->in_data;
	infoP->blur = (A_long)((PF_FpLong)infoP->blur * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den + 0.5);
	infoP->length = ((PF_FpLong)infoP->length * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den);
	
	ERR(AlphaCopyDD(ae->in_data, ae->input, ae->output, ae->pixelFormat(), ae->suitesP, infoP->rot, infoP->length, infoP->offset,infoP->reverse));
	if (infoP->blur > 0 || infoP->minmax != 0) {
		Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, FALSE);
	}
	if (infoP->minmax != 0) {
		MinMax(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->minmax);
	}
	if (infoP->blur > 0) {
		Blur(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->blur);
	}
	Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, true);
	if (infoP->blur > 0 || infoP->minmax != 0) {
		Mult(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, TRUE);
	}
	if (infoP->hyperbolic != 0.0) {
		HyperbolicAlpha(ae->in_data, ae->output, ae->pixelFormat(), ae->suitesP, infoP->hyperbolic);
	}
	ERR(AlphaCopyRM(ae->in_data, ae->input, ae->output, ae->pixelFormat(), ae->suitesP, infoP->color, FALSE));
	
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

