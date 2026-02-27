//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Mosaic.h"

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
	cs.AddCheckBox(	// noise frame
		STR_MOSAIC_UNLINK,
		"on",
		FALSE,
		ID_MOSAIC_UNLINK,
		PF_ParamFlag_SUPERVISE
	); 
	cs.AddSlider(	// 
		STR_MOSAIC_W,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		6000,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		2000,			//スライダーの最大値
		36,				//デフォルトの値
		ID_MOSAIC_W
	);
	cs.AddSlider(	// 
		STR_MOSAIC_H,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		6000,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		2000,			//スライダーの最大値
		36,				//デフォルトの値
		ID_MOSAIC_H,
		PF_ParamFlag_NONE,
		PF_PUI_INVISIBLE
	);
	cs.AddPoint(
		STR_MOSAIC_CENTER,
		50,
		50,
		FALSE,
		ID_MOSAIC_CENTER
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
		err = ae.HandleChangedParam(in_data, out_data, params, outputP, extraP, ID_NUM_PARAMS);
		if (!err) {
			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = FALSE;
				PF_Boolean b;
				ERR(ae.GetCHECKBOX(ID_MOSAIC_UNLINK, &b));
				hide_themB[ID_MOSAIC_H] = !b;
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
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
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	ERR(ae->GetCHECKBOX(ID_MOSAIC_UNLINK, &infoP->unlink));
	ERR(ae->GetADD(ID_MOSAIC_W, &infoP->mosaicW));
	ERR(ae->GetADD(ID_MOSAIC_H, &infoP->mosaicH));
	ERR(ae->GetPOINT(ID_MOSAIC_CENTER, &infoP->mosaicCenter));


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	if (infoP->mosaicW <= 1 && infoP->mosaicH <= 1) return err;

	A_long mw = infoP->mosaicW;
	A_long mh = infoP->mosaicH;
	if(infoP->unlink==FALSE)
	{
		mh = mw;
	}

	A_long sx = (infoP->mosaicCenter.x - mw/2) % mw - mw;
	A_long sy = (infoP->mosaicCenter.y - mh / 2) % mh - mh;
	A_long cx = (ae->outputInfo.width-sx) / mw + 1;
	A_long cy = (ae->outputInfo.height-sy) / mh + 1;


	for( A_long y = 0; y < cy; y++) {
		for (A_long x = 0; x < cx; x++) {
			A_long px = sx + x * mw;
			A_long py = sy + y * mh;
			if (px >= ae->outputInfo.width) continue;
			if (py >= ae->outputInfo.height) continue;
			ERR(MosaicFlat(
				ae->output,
				ae->pixelFormat(),
				ae->outputInfo.widthTrue,
				px,
				py,
				mw,
				mh
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

