//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "FloodFill.h"

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
		{ 0xFF, 0xFF, 0x00, 0x00 },
		{ 0xFF, 0x00, 0xFF, 0x00 },
		{ 0xFF, 0x00, 0x00, 0xFF },
		{ 0xFF, 0x00, 0xFF, 0xFF },
		{ 0xFF, 0xFF, 0x00, 0xFF },
		{ 0xFF, 0xFF, 0xFF, 0x00 },
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		{ 0xFF, 0x00, 0x00, 0x00 }
	};
	
	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddPopup(	// 
		STR_PAINT_COUNT,	//パラメータの名前
		PAINT_COLOR_NUM,
		1,			
		STR_PAINT_ITEMS,				//スライダーの最小値 
		ID_PAINT_COUNT,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddTopic(STR_PAINT_TOPIC, ID_PAINT_TOPIC);
	std::string str;
	PF_ParamUIFlags uif = PF_PUI_NONE;
	PF_ParamFlags pf = PF_ParamFlag_NONE;

	for (int i=0; i < PAINT_COLOR_NUM; i++) {
		 str = std::string(STR_ENABLE_BAR) +std::string(STR_ENABLE) + std::to_string(i+1);
		 if (i > 0) {
			 uif = PF_PUI_INVISIBLE;
		 }
		cs.AddCheckBox(	
			str.c_str(),
			"on",
			FALSE,
			ID_ENABLE(i),
			pf,
			uif
		);
		// Guide
		str = std::string(STR_GUIDE) + std::to_string(i+1);
		cs.AddCheckBox(
			str.c_str(),
			"on",
			FALSE,
			ID_GUIDE(i),
			PF_ParamFlag_CANNOT_TIME_VARY | PF_ParamFlag_CANNOT_INTERP,
			uif
		);
		// Pos
		str = std::string(STR_POS) + std::to_string(i+1);
		cs.AddPoint(
			str.c_str(),
			10 + i * 10,
			10 + i * 10,
			FALSE,
			ID_POS(i),
			pf,
			uif
		);
		// Color
		str = std::string(STR_COLOR) + std::to_string(i+1);
		cs.AddColor(	
			str.c_str(),
			def_color[i% PAINT_COLOR_NUM],
			ID_COLOR(i),
			pf,
			uif
		);
	}
	
	cs.EndTopic(ID_PAINT_TOPIC_END);
	cs.AddColor(
		STR_GUIDE_COLOR,
		{0xFF,0xFF,0,0},
		ID_GUIDE_COLOR
	);
	cs.AddButton(	// button
		STR_GUIDE_ALL_CLEAR,
		"clear",
		ID_GUIDE_ALL_CLEAR,
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
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
			if (extraP->param_index == ID_GUIDE_ALL_CLEAR)
			{
				for(int i=0; i< PAINT_COLOR_NUM; i++) {
					ERR(ae.SetCHECKBOX(ID_GUIDE(i), FALSE));
				}
				if (!err) out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
			A_Boolean hide_themB[ID_NUM_PARAMS];
			ERR(ae.GetNewEffectStreamAll());
			for (A_long i = ID_ENABLE0; i < ID_PAINT_TOPIC_END; i++)
				hide_themB[i] = TRUE;
			A_long count = 0;
			ERR(ae.GetPOPUP(ID_PAINT_COUNT, &count));
			for (A_long i = 0; i < count; i++) {
				hide_themB[ID_ENABLE(i)] = FALSE;
				hide_themB[ID_POS(i)] = FALSE;
				hide_themB[ID_COLOR(i)] = FALSE;
				hide_themB[ID_GUIDE(i)] = FALSE;
			}
			for (A_long i = ID_ENABLE0; i < ID_PAINT_TOPIC_END; i++)
				ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
			if(!err) out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
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
	/*
	PF_Err 	err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_NOISE_AUTO,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	*/
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetPOPUP(ID_PAINT_COUNT, &infoP->paintCount));

	for(int i=0; i< PAINT_COLOR_NUM; i++) {
		ERR(ae->GetCHECKBOX(ID_ENABLE(i), &infoP->paintEnables[i]));
		ERR(ae->GetPOINT(ID_POS(i), &infoP->paintPoints[i]));
		ERR(ae->GetCOLOR(ID_COLOR(i), &infoP->paintColors[i]));
		ERR(ae->GetCHECKBOX(ID_GUIDE(i), &infoP->paintGuides[i]));
	}
	ERR(ae->GetCOLOR(ID_GUIDE_COLOR, &infoP->guideColor));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	for(int i=0; i< infoP->paintCount; i++) {
		if (infoP->paintEnables[i]) {
			ERR(Paint(
				ae->output,
				ae->pixelFormat(),
				infoP->paintPoints[i].x,
				infoP->paintPoints[i].y,
				infoP->paintColors[i]
			));
		}
	}
	for (int i = 0; i < infoP->paintCount; i++) {
		if (infoP->paintEnables[i] && infoP->paintGuides[i]) {
			ERR(DrawLine(
				ae->output,
				ae->pixelFormat(),
				0,
				infoP->paintPoints[i].y,
				ae->output->width,
				infoP->paintPoints[i].y,
				infoP->guideColor
				));
			ERR(DrawLine(
				ae->output,
				ae->pixelFormat(),
				infoP->paintPoints[i].x,
				0,
				infoP->paintPoints[i].x,
				ae->output->height,
				infoP->guideColor
			));
		}
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

