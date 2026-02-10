//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "CellEmboss.h"

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

	cs.AddPopup(STR_COUNT,
		9,
		2,
		STR_COUNT_ITEMS,
		ID_COUNT
	);
	// *************************
	cs.AddTopic(STR_COLOR_TOPIC, ID_COLOR_TOPIC);
	for (int i = 0; i < 8; i++) {
		std::string buf = std::string("color") +std::to_string(i+1);
		cs.AddColor(	// color
			buf.c_str(),
			{ 0xFF, (A_u_char)(i * 32), (A_u_char)(255 - i * 32), 0xFF },
			ID_COLOR(i)
		);
	}
	cs.EndTopic(ID_COLOR_TOPIC_END);
	// *************************

	cs.AddAngle(	// angle
		STR_DIRECTION,
		45,
		ID_DIRECTION//,
		//PF_ParamFlag_NONE,
		//PF_PUI_DISABLED
	);
	cs.AddPopup(STR_MODE,
		4,
		1,
		STR_MODE_ITEMS,
		ID_MODE
	);
	// ****************
	cs.AddTopic(STR_HI_TOPIC, ID_HI_TOPIC);
	cs.AddFloatSlider(
		STR_DISTANCE_HI,			//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		50,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_DISTANCE_HI
	);
	// ----------------------------------------------------------------
	cs.AddSlider(	// 
		STR_BLUR_HI,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		30,				//スライダーの最大値
		0,				//デフォルトの値
		ID_BLUR_HI//,
		//PF_ParamFlag_SUPERVISE |
		//PF_ParamFlag_CANNOT_TIME_VARY |
		//PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddColor(
		STR_COLOR_HI,
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_COLOR_HI
	);
	cs.EndTopic(ID_HI_TOPIC_END);
	// ***********
	cs.AddTopic(STR_LO_TOPIC, ID_LO_TOPIC);
	cs.AddFloatSlider(
		STR_DISTANCE_LO,			//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		50,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_DISTANCE_LO
	);
	
	// ----------------------------------------------------------------
	cs.AddSlider(	// 
		STR_BLUR_LO,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		30,				//スライダーの最大値
		0,				//デフォルトの値
		ID_BLUR_LO//,
		//PF_ParamFlag_SUPERVISE |
		//PF_ParamFlag_CANNOT_TIME_VARY |
		//PF_ParamFlag_CANNOT_INTERP
	);
	
	cs.AddColor(
		STR_COLOR_LO,
		{ 0xFF, 0x00, 0x00, 0x00 },
		ID_COLOR_LO
	);
	cs.EndTopic(ID_LO_TOPIC_END);
	
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
	//					err2				= PF_Err_NONE;
	
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

	PF_Fixed	fixed_angle=0;

	ERR(ae->GetPOPUP(ID_COUNT, &infoP->count));
	infoP->count -= 1;
	for (int i = 0; i < 8; i++) {
		ERR(ae->GetCOLOR(ID_COLOR(i), &infoP->colors[i]));
	}
	ERR(ae->GetANGLE(ID_DIRECTION, &fixed_angle));
	infoP->direction = F_FIX2FLT(fixed_angle);

	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));

	ERR(ae->GetFLOAT(ID_DISTANCE_HI, &infoP->distanceHI));
	ERR(ae->GetADD(ID_BLUR_HI, &infoP->blurHI));
	ERR(ae->GetCOLOR(ID_COLOR_HI, &infoP->colorHi));
	infoP->colorHi16 = NF_Pixel8To16(infoP->colorHi);
	infoP->colorHi32 = NF_Pixel8To32(infoP->colorHi);

	ERR(ae->GetFLOAT(ID_DISTANCE_LO, &infoP->distanceLO));
	ERR(ae->GetADD(ID_BLUR_LO, &infoP->blurLO));
	ERR(ae->GetCOLOR(ID_COLOR_LO, &infoP->colorLo));
	infoP->colorLo16 = NF_Pixel8To16(infoP->colorLo);
	infoP->colorLo32 = NF_Pixel8To32(infoP->colorLo);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	//ERR(ae->CopyInToOut());
	ERR(SelectPixelsExec(
		ae->in_data,
		ae->input,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		infoP->count,
		infoP->colors,
		SelectPixelMode::SP_MODE_MASK
		));
	ERR(EmbossHiFilter(ae, infoP));
	

	if (infoP->blurHI > 0 && (infoP->mode == 1 || infoP->mode == 3 || infoP->mode == 4)) {
		ERR(ChannelBlur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->blurHI,
			1
		));
	}
	if (infoP->blurLO > 0 && (infoP->mode == 2|| infoP->mode == 4)) {
		ERR(ChannelBlur(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->blurLO,
			2
		));
	}
	
	ERR(EmbossColoriseFilter(
		ae,
		infoP
	));
	


	return err;
	//OutputDebugStringA("After ChannelBlur LO\n");
	//char dbg[256];
	//sprintf_s(dbg, "DEBUG: blurHI=%d, blurLO=%d\n", infoP->blurHI, infoP->blurLO);
	//OutputDebugStringA(dbg);

	//OutputDebugStringA("Before ChannelBlur HI (Green)\n");
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

