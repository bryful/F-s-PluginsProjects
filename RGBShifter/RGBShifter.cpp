//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "RGBShifter.h"

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

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddPopup(STR_MODE_POP,
		STR_MODE_COUNT,
		STR_MODE_DFLT,
		STR_MODE_ITEMS,
		ID_MODE,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddFloatSilder(	// R
		STR_R,			//Name
		-100,				//VALID_MIN
		100,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_R
	);
	cs.AddFloatSilder(	// R
		STR_G,			//Name
		-100,				//VALID_MIN
		100,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_G
	);
	cs.AddFloatSilder(	// R
		STR_B,			//Name
		-100,				//VALID_MIN
		100,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_B
	);
	cs.AddFloatSilder(	// R
		STR_Y,			//Name
		-100,				//VALID_MIN
		100,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_Y,
		PF_PUI_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddFloatSilder(	// R
		STR_U,			//Name
		-100,				//VALID_MIN
		100,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_U,
		PF_PUI_NONE,
		PF_PUI_INVISIBLE

	);
	cs.AddFloatSilder(	// R
		STR_V,			//Name
		-100,				//VALID_MIN
		100,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_V,
		PF_PUI_NONE,
		PF_PUI_INVISIBLE

	);
	cs.AddColor(	// color
		STR_TARGETCOLOR,
		{ 0xFF, 0xFF, 0x00, 0x00 },
		ID_TARGETCOLOR,
		PF_PUI_NONE,
		PF_PUI_INVISIBLE

	);
	cs.AddFloatSilder(	// R
		STR_BLEND,			//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

	try{
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		
			ERR(ae.GetNewEffectStreamAll());
			if (!err){
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for ( A_long i=2; i<ID_BLEND; i++) hide_themB[i] =TRUE;
				A_long mode;
				ERR(ae.GetPOPUP(ID_MODE,&mode));
				switch (mode)
				{
					case 1: // RGBシフト
						hide_themB[ID_R] = FALSE;
						hide_themB[ID_G] = FALSE;
						hide_themB[ID_B] = FALSE;
						break;
					case 2: // YUVシフト
						hide_themB[ID_Y] = FALSE;
						hide_themB[ID_U] = FALSE;
						hide_themB[ID_V] = FALSE;
						break;
					case 3: // Target RGB
						hide_themB[ID_TARGETCOLOR] = FALSE;
						break;
				default:
					break;
				}
				for ( A_long i=2; i<ID_BLEND; i++) 
					ERR(ae.SetDynamicStreamFlag(i,AEGP_DynStreamFlag_HIDDEN,hide_themB[i]));
				
				
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

	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));

	ERR(ae->GetFLOAT(ID_R, &infoP->r_shift));
	infoP->r_shift /= 100.0f;
	ERR(ae->GetFLOAT(ID_G, &infoP->g_shift));
	infoP->g_shift /= 100.0f;
	ERR(ae->GetFLOAT(ID_B, &infoP->b_shift));
	infoP->b_shift /= 100.0f;
	ERR(ae->GetFLOAT(ID_Y, &infoP->y_shift));
	infoP->y_shift /= 100.0f;
	ERR(ae->GetFLOAT(ID_U, &infoP->u_shift));
	infoP->u_shift /= 100.0f;
	ERR(ae->GetFLOAT(ID_V, &infoP->v_shift));
	infoP->v_shift /= 100.0f;

	ERR(ae->GetCOLOR(ID_TARGETCOLOR, &infoP->target_color));

	ERR(ae->GetFLOAT(ID_BLEND, &infoP->blend));
	infoP->blend /= 100.0f;

	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	if ((infoP->mode == 1 &&
		infoP->r_shift == 0.0f &&
		infoP->g_shift == 0.0f &&
		infoP->b_shift == 0.0f
		)||
		(infoP->mode == 2 &&
		infoP->y_shift == 0.0f &&
		infoP->u_shift == 0.0f &&
		infoP->v_shift == 0.0f
		)|| infoP->blend>=100)
	{
		ERR(ae->CopyInToOut());
		return err;
	}

	ERR(RGBShiftFX(ae, infoP));
	
	if (infoP->blend > 0.0f) {
		PF_Fixed blend_ratio = F_FLT2FIX(infoP->blend);
		err = ae->suitesP->WorldTransformSuite1()->blend(
			ae->in_data->effect_ref,
			ae->output,  // 入力画像1
			ae->input,    // 入力画像2 (チェックアウトしたレイヤーなど)
			blend_ratio,       // 0 ~ 65536
			ae->output             // 出力先
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

