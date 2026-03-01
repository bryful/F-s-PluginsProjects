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
	// ----------------------------------------------------------------
	cs.AddTopic(STR_NOISE_TOPIC, ID_NOISE_TOPIC);
	cs.AddFloatSlider(	// R
		STR_NOISE,	//Name
		0,				//VALID_MIN
		100,			//VALID_MAX
		0,				//SLIDER_MIN
		10,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE
	);
	cs.AddCheckBox(	// noise frame
		STR_IS_COLOR,
		"on",
		FALSE,
		ID_IS_COLOR
	);
	cs.AddCheckBox(	// noise frame
		STR_AUTO_SEED,
		"on",
		FALSE,
		ID_AUTO_SEED
	);
	cs.AddSlider(	// 
		STR_SEED,		//パラメータの名前
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
	PF_Err 	err2 = PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data, out_data, params, extra, ID_NUM_PARAMS);
	if (!err) {
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_AUTO_SEED, &def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
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

	ERR(ae->GetFLOAT(ID_NOISE, &infoP->noise));
	infoP->noise /= 100;
	ERR(ae->GetCHECKBOX(ID_IS_COLOR, &infoP->isColor));
	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	infoP->mosaicW = ae->downScale(infoP->mosaicW);
	infoP->mosaicH = ae->downScale(infoP->mosaicH);
	if (infoP->mosaicW <= 1 && infoP->mosaicH <= 1) return err;
	if(infoP->autoSeed)
	{
		infoP->seed = infoP->seed+ae->frame();
	}
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
			PF_FpLong ar = 0;
			PF_FpLong ag = 0;
			PF_FpLong ab = 0;
			if (infoP->noise > 0) {
				if (infoP->isColor) {
					ar = infoP->noise - 2* hash_float(x, y + 00, infoP->seed) * infoP->noise;
					ag = infoP->noise - 2 * hash_float(x, y + 10, infoP->seed) * infoP->noise;
					ab = infoP->noise - 2 * hash_float(x, y + 90, infoP->seed) * infoP->noise;
				}
				else {
					ar = infoP->noise - 2 * hash_float(x, y + 00, infoP->seed) * infoP->noise;
					ag = ar;
					ab = ar;
				}
				switch (ae->pixelFormat())
				{
					case PF_PixelFormat_ARGB128:
					break;
					case PF_PixelFormat_ARGB64:
						ar = ar * PF_MAX_CHAN16;
						ag = ag * PF_MAX_CHAN16;
						ab = ab * PF_MAX_CHAN16;
						break;
					case PF_PixelFormat_ARGB32:
						ar = ar * PF_MAX_CHAN8;
						ag = ag * PF_MAX_CHAN8;
						ab = ab * PF_MAX_CHAN8;
						break;
				}
			}
			ERR(MosaicFlat(
				ae->output,
				ae->pixelFormat(),
				ae->outputInfo.widthTrue,
				px,
				py,
				mw,
				mh,
				ar,ag,ab
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

