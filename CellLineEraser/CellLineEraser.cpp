//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "CellLineEraser.h"

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
	// ----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_COLOR_COUNT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		10,		//数値入力する場合の最大値
		0,		//スライダーの最小値 
		10,		//スライダーの最大値
		0,		//デフォルトの値
		ID_Color_COUNT
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color1,
		0x00,
		0x00,
		0x00,
		ID_Color1
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color2,
		0xFF,
		0x00,
		0x00,
		ID_Color2
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color3,
		0x00,
		0xFF,
		0x00,
		ID_Color3
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color4,
		0x00,
		0x00,
		0xFF,
		ID_Color4
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color5,
		0x80,
		0x00,
		0x00,
		ID_Color5
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color6,
		0x00,
		0x80,
		0x00,
		ID_Color6
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color7,
		0x00,
		0x00,
		0x80,
		ID_Color7
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color8,
		0x40,
		0x00,
		0x00,
		ID_Color1
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color9,
		0x00,
		0x40,
		0x00,
		ID_Color9
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color10,
		0x00,
		0x00,
		0x40,
		ID_Color10
	);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_KEEP_PIXELS,
		"on",
		FALSE,
		0,
		ID_KEEP_PIXELS
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_FillUnknownColors,
		"on",
		FALSE,
		0,
		ID_Fill_Color
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_FillColor,
		0xFF,
		0x00,
		0xFF,
		ID_Fill_Color
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MakeWhiteTransparent,
		"on",
		FALSE,
		0,
		ID_MakeWhiteTransparent
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;
		
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

	ERR(ae->GetADD(ID_Color_COUNT, &infoP->target_count));

	for (int i = 0; i < 10; i++)
	{
		PF_Pixel p;
		ERR(ae->GetCOLOR(ID_Color1 + i, &p));
		if (err == PF_Err_NONE) {
			infoP->targets[i] = p;
		}

	}

	ERR(ae->GetCHECKBOX(ID_KEEP_PIXELS, &infoP->KeepPixel));
	ERR(ae->GetCHECKBOX(ID_FillUnknownColors, &infoP->FillUnknownColors));
	ERR(ae->GetCOLOR(ID_Fill_Color, &infoP->GiveUpColor8));
	infoP->GiveUpColor16 = NF_Pixel8To16(infoP->GiveUpColor8);
	infoP->GiveUpColor32 = NF_Pixel8To32(infoP->GiveUpColor8);

	ERR(ae->GetCHECKBOX(ID_MakeWhiteTransparent, &infoP->MakeWhiteTransparent));


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data;
	in_data = ae->in_data;


	ERR(CellLineEraserSub(ae, infoP));
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

